"""Static recompiler: PS1 functions that are still MIPS assembly -> C for the native build.

  recomp.py LEVEL OUT.c NATIVE_OBJS...

Recompiles every function of the level that nothing native defines: the PsyQ library segment, the
hand-written GTE module at the end of .data, and game functions still included as asm. Each becomes
`rc_<addr>(R3000 *c)` over a register context, plus an exported `NAME(...)` wrapper so native code (and
the PS1-address trampolines) can call it. Calls from recompiled code go straight to other recompiled
functions, or to native ones with the MIPS argument registers and stack arguments.

Semantics follow the R3000: branch delay slots, the load delay slot, HI/LO, unaligned lwl/lwr/swl/swr,
division by zero results; GTE (cop2) instructions call the software GTE. The I/O area isn't mapped, so
recompiled code that touches hardware registers faults: such library functions need a native version.
"""
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "port"))
from gen_native import HOST_LIBC, nm, ps1_symbols, psyq_names  # noqa: E402

# Calls between native and recompiled code carry this many arguments: $a0-$a3 and the rest from the PS1
# stack (sp+16...). Extra ones are harmless: native callees ignore them, recompiled ones never read them.
NARGS = 16
STACK_ARGS = ", ".join(f"M32(R(29) + {16 + 4 * k})" for k in range(NARGS - 4))

REG = ["zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
       "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"]


def functions(level):
    """[(name, addr, size)] of the level's code that exists only as assembly."""
    addr = ps1_symbols(level)
    out = []
    files = [ROOT / f"asm/{level}/psyq_libs.s"] + sorted((ROOT / f"asm/{level}/nonmatchings").glob("*/*.s"))
    for path in files:
        for m in re.finditer(r"^nonmatching (\w+), (0x[0-9A-Fa-f]+)", path.read_text(), re.M):
            if m.group(1) in addr:
                out.append((m.group(1), addr[m.group(1)], int(m.group(2), 16)))
    # hand-written code in .data (dlabel func_... / enddlabel): size from the labels' addresses
    for path in sorted((ROOT / f"asm/{level}/data").glob("*.s")):
        text = path.read_text()
        for m in re.finditer(r"^dlabel (func_\w+)\n(.*?)^enddlabel \1", text, re.M | re.S):
            words = re.findall(r"/\* \w+ ([0-9A-F]{8}) \*/", m.group(2))
            if words and m.group(1) in addr:
                last = int(words[-1], 16)
                out.append((m.group(1), addr[m.group(1)], last + 4 - addr[m.group(1)]))
    out = sorted(set(out), key=lambda f: f[1])
    # main can be a label inside another asm function (start jumps there): give it an entry of its own
    main = main_address(level)
    if all(a != main for _, a, _ in out):
        owner = next(((n, a, s) for n, a, s in out if a < main < a + s), None)
        if owner:
            out.append(("port_recomp_main", main, owner[1] + owner[2] - main))
    out += data_code(level, out, addr)
    out = sorted(set(out), key=lambda f: f[1])
    # padding after a function belongs to it (code can fall through it into the next function)
    for k in range(len(out) - 1):
        n, a, s = out[k]
        gap = out[k + 1][1] - (a + s)
        if 0 < gap <= 16:
            out[k] = (n, a, s + gap)
    return out, addr


def main_address(level):
    """the game's main: where the EXE's entry point (start) jumps"""
    exe = (ROOT / f"build/{level}/{level}.exe").read_bytes()
    pc0, load = int.from_bytes(exe[0x10:0x14], "little"), int.from_bytes(exe[0x18:0x1C], "little")
    for k in range(16):
        w = int.from_bytes(exe[0x800 + pc0 - load + 4 * k:0x804 + pc0 - load + 4 * k], "little")
        if w >> 26 == 2:
            return (pc0 & 0xF0000000) | ((w & 0x3FFFFFF) << 2)
    return None


def data_code(level, funcs, addr):
    """Hand-written code inside .data (the GTE module at its end) that other code calls without a function
    label: entries are the jal/j targets that land in .data; each runs to its last `jr $ra` that no branch
    jumps past."""
    exe = (ROOT / f"build/{level}/{level}.exe").read_bytes()
    base = int.from_bytes(exe[0x18:0x1C], "little") - 0x800
    data_start = addr["main_DATA_START"]
    data_end = addr["main_DATA_END"]

    def word(a):
        return int.from_bytes(exe[a - base:a - base + 4], "little")

    def size_of(entry):
        a, reach = entry, entry
        while a < data_end:
            i = Insn(a, word(a))
            if i.kind() == "branch":
                reach = max(reach, i.btarget())
            if i.kind() == "jr" and i.rs == 31 and a + 8 > reach:
                return a + 8 - entry
            a += 4
        return a - entry

    known = {a for _, a, _ in funcs}
    found, todo = [], [(a, s) for _, a, s in funcs]
    while todo:
        start, size = todo.pop()
        for a in range(start, start + size, 4):
            i = Insn(a, word(a))
            if i.kind() in ("jal", "j") and data_start <= i.target < data_end and i.target not in known:
                known.add(i.target)
                s = size_of(i.target)
                found.append((f"func_{i.target:08X}", i.target, s))
                todo.append((i.target, s))
    return found


def s16(v):
    return v - 0x10000 if v & 0x8000 else v


class Insn:
    def __init__(self, pc, w):
        self.pc, self.w = pc, w
        self.op = w >> 26
        self.rs, self.rt, self.rd = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        self.sa, self.funct = (w >> 6) & 31, w & 63
        self.imm, self.simm = w & 0xFFFF, s16(w & 0xFFFF)
        self.target = ((pc + 4) & 0xF0000000) | ((w & 0x3FFFFFF) << 2)

    def kind(self):
        """'branch' (conditional, PC-relative), 'j', 'jal', 'jr', 'jalr', 'bal' or None"""
        op, f = self.op, self.funct
        if op == 0 and f == 8:
            return "jr"
        if op == 0 and f == 9:
            return "jalr"
        if op == 2:
            return "j"
        if op == 3:
            return "jal"
        if op in (4, 5, 6, 7):
            return "branch"
        if op == 1:
            return "bal" if self.rt in (16, 17) else "branch"
        return None

    def btarget(self):
        return (self.pc + 4 + (self.simm << 2)) & 0xFFFFFFFF

    def reads(self):
        """registers this instruction reads"""
        op, f = self.op, self.funct
        if op == 0:
            if f in (0, 2, 3):
                return {self.rt}
            if f in (8, 9, 17, 19):
                return {self.rs}
            if f in (16, 18, 12, 13):
                return set()
            return {self.rs, self.rt}
        if op == 1 or op in (6, 7):
            return {self.rs}
        if op in (4, 5):
            return {self.rs, self.rt}
        if op in (2, 3, 15):
            return set()
        if op in (16, 18):
            return {self.rt} if self.rs in (4, 6) else set()
        if op in (40, 41, 42, 43, 46):
            return {self.rs, self.rt}
        if op in (34, 38):
            return {self.rs, self.rt}
        return {self.rs}

    def load_target(self):
        """register written by a load with a delay slot (not lwl/lwr, which merge), else None"""
        if self.op in (32, 33, 35, 36, 37) and self.rt:
            return self.rt
        if self.op in (16, 18) and self.rs in (0, 2) and not (self.w >> 25) & 1 and self.rt:
            return self.rt
        return None


def R(n):
    return "0" if n == 0 else f"R({n})"


def set_reg(n, expr):
    return "" if n == 0 else f"R({n}) = {expr};"


def ea(i):
    return f"R({i.rs}) + {i.simm}" if i.rs else f"{i.simm & 0xFFFFFFFF}u"


def value(i):
    """C for a non-control instruction (statement), or None if unknown"""
    op, f = i.op, i.funct
    rs, rt, rd = R(i.rs), R(i.rt), R(i.rd)
    if op == 0:
        if f == 0:
            return set_reg(i.rd, f"{rt} << {i.sa}") if i.w else ""
        if f == 2:
            return set_reg(i.rd, f"{rt} >> {i.sa}")
        if f == 3:
            return set_reg(i.rd, f"(u32)((s32){rt} >> {i.sa})")
        if f == 4:
            return set_reg(i.rd, f"{rt} << ({rs} & 31)")
        if f == 6:
            return set_reg(i.rd, f"{rt} >> ({rs} & 31)")
        if f == 7:
            return set_reg(i.rd, f"(u32)((s32){rt} >> ({rs} & 31))")
        if f == 12:
            return f"port_rc_syscall(c, 0x{i.pc:08X}u);"
        if f == 13:
            return ""
        if f == 16:
            return set_reg(i.rd, "c->hi")
        if f == 17:
            return f"c->hi = {rs};"
        if f == 18:
            return set_reg(i.rd, "c->lo")
        if f == 19:
            return f"c->lo = {rs};"
        if f == 24:
            return f"{{ s64 p = (s64)(s32){rs} * (s32){rt}; c->lo = (u32)p; c->hi = (u32)(p >> 32); }}"
        if f == 25:
            return f"{{ u64 p = (u64){rs} * {rt}; c->lo = (u32)p; c->hi = (u32)(p >> 32); }}"
        if f == 26:
            return f"rc_div(c, (s32){rs}, (s32){rt});"
        if f == 27:
            return f"rc_divu(c, {rs}, {rt});"
        ops = {32: "+", 33: "+", 34: "-", 35: "-", 36: "&", 37: "|", 38: "^"}
        if f in ops:
            return set_reg(i.rd, f"{rs} {ops[f]} {rt}")
        if f == 39:
            return set_reg(i.rd, f"~({rs} | {rt})")
        if f == 42:
            return set_reg(i.rd, f"(s32){rs} < (s32){rt}")
        if f == 43:
            return set_reg(i.rd, f"{rs} < {rt}")
        return None
    if op in (8, 9):
        return set_reg(i.rt, f"{rs} + {i.simm}")
    if op == 10:
        return set_reg(i.rt, f"(s32){rs} < {i.simm}")
    if op == 11:
        return set_reg(i.rt, f"{rs} < {i.simm & 0xFFFFFFFF}u")
    if op == 12:
        return set_reg(i.rt, f"{rs} & 0x{i.imm:X}")
    if op == 13:
        return set_reg(i.rt, f"{rs} | 0x{i.imm:X}")
    if op == 14:
        return set_reg(i.rt, f"{rs} ^ 0x{i.imm:X}")
    if op == 15:
        return set_reg(i.rt, f"0x{i.imm << 16:X}u")
    loads = {32: "(u32)(s32)(s8)LD8", 33: "(u32)(s32)(s16)LD16", 35: "LD32", 36: "LD8", 37: "LD16"}
    if op in loads:
        return set_reg(i.rt, f"{loads[op]}({ea(i)})")
    if op == 34:
        return set_reg(i.rt, f"rc_lwl({rt}, {ea(i)})")
    if op == 38:
        return set_reg(i.rt, f"rc_lwr({rt}, {ea(i)})")
    stores = {40: "ST8", 41: "ST16", 43: "ST32"}
    if op in stores:
        return f"{stores[op]}({ea(i)}, {rt});"
    if op == 42:
        return f"rc_swl({rt}, {ea(i)});"
    if op == 46:
        return f"rc_swr({rt}, {ea(i)});"
    if op == 50:
        return f"GTE_mtc2({i.rt}, M32({ea(i)}));"
    if op == 58:
        return f"M32({ea(i)}) = GTE_mfc2({i.rt});"
    if op == 18:
        if (i.w >> 25) & 1:
            return f"GTE_cmd(0x{i.w:08X}u);"
        if i.rs == 0:
            return set_reg(i.rt, f"GTE_mfc2({i.rd})")
        if i.rs == 2:
            return set_reg(i.rt, f"GTE_cfc2({i.rd})")
        if i.rs == 4:
            return f"GTE_mtc2({i.rd}, {rt});"
        if i.rs == 6:
            return f"GTE_ctc2({i.rd}, {rt});"
        return None
    if op == 16:
        if i.rs == 0:
            return set_reg(i.rt, f"c->cop0[{i.rd & 15}]")
        if i.rs == 4:
            return f"c->cop0[{i.rd & 15}] = {rt};"
        if i.rs == 16:
            return ""  # rfe
        return None
    return None


def load_value(i):
    """(expression, register) for a delayed load"""
    stmt = value(i)
    m = re.fullmatch(r"R\((\d+)\) = (.*);", stmt)
    return m.group(2), int(m.group(1))


def cond(i):
    rs, rt = R(i.rs), R(i.rt)
    if i.op == 4:
        return f"{rs} == {rt}"
    if i.op == 5:
        return f"{rs} != {rt}"
    if i.op == 6:
        return f"(s32){rs} <= 0"
    if i.op == 7:
        return f"(s32){rs} > 0"
    if i.op == 1:
        return f"(s32){rs} < 0" if i.rt in (0, 16) else f"(s32){rs} >= 0"
    raise ValueError


class Gen:
    def __init__(self, level, funcs, addr_of, native):
        self.level = level
        self.funcs = funcs                      # recompiled: [(name, addr, size)]
        self.entry = {a: n for n, a, s in funcs}
        self.native = native                    # name -> True for natively defined functions
        self.name_at = {}
        for n, a in addr_of.items():
            if n in native:
                self.name_at.setdefault(a, n)
        exe = (ROOT / f"build/{level}/{level}.exe").read_bytes()
        self.exe, self.base = exe, int.from_bytes(exe[0x18:0x1C], "little") - 0x800
        self.used_natives = set()
        self.main_addr = addr_of.get("game_main")

    def word(self, a):
        o = a - self.base
        return int.from_bytes(self.exe[o:o + 4], "little")

    def call(self, target, pc):
        """C for a call to a PS1 address from recompiled code"""
        if target in self.entry:
            return f"rc_{target:08X}(c);"
        n = self.name_at.get(target)
        if n:
            self.used_natives.add(n)
            return f"{{ u32 a[16] = {{R(4), R(5), R(6), R(7), {STACK_ARGS}}}; R(2) = port_call16((void *){n}, a); }}"
        return f"port_rc_call(c, 0x{target:08X}u);"

    def extent(self, start, size):
        """[lo, hi) to translate for the function at `start`: grows to take in code its branches reach in
        the middle of neighbouring functions (where the signature tool split one function in two)."""
        lo, hi = start, start + size
        changed = True
        while changed:
            changed = False
            for a in range(lo, hi, 4):
                i = Insn(a, self.word(a))
                k = i.kind()
                tgt = i.btarget() if k == "branch" else i.target if k == "j" else None
                if tgt is None or lo <= tgt < hi or tgt in self.entry:
                    continue
                owner = self.owner(tgt)
                if owner:
                    lo, hi = min(lo, owner[0]), max(hi, owner[1])
                    changed = True
        return lo, hi

    def owner(self, addr):
        for n, a, s in self.funcs:
            if a <= addr < a + s:
                return a, a + s
        return None

    def function(self, name, entry, size):
        start, end = self.extent(entry, size)
        ins = [Insn(a, self.word(a)) for a in range(start, end, 4)]
        at = {i.pc: k for k, i in enumerate(ins)}
        labels = set()
        for i in ins:
            k = i.kind()
            if k == "branch" and start <= i.btarget() < end:
                labels.add(i.btarget())
            if k == "j" and start <= i.target < end:
                labels.add(i.target)
        # jump tables: words in the image that point into this function
        jtargets = sorted(t for t in self.jtbl_targets if start < t < end and t in at)
        labels |= set(jtargets)
        out = [f"/* {name} */", f"void rc_{entry:08X}(R3000 *c) {{"]
        if entry != start:
            labels.add(entry)
            out.append(f"    goto L_{entry:08X};")
        k = 0
        delay_done = set()
        while k < len(ins):
            i = ins[k]
            if i.pc in labels:
                out.append(f"L_{i.pc:08X}:;")
            if i.pc in delay_done:
                # already emitted inside the preceding branch; only reached by a jump to the delay slot
                k += 1
                continue
            kind = i.kind()
            if kind:
                d = ins[k + 1] if k + 1 < len(ins) else None
                dcode = (value(d) if d else "") or ""
                if d and value(d) is None:
                    dcode = f"port_rc_bad(0x{d.pc:08X}u, 0x{d.w:08X}u);"
                after = []
                if d and d.pc in labels:
                    # something jumps to the delay slot itself: skip it on the fall-through path
                    after = [f"goto L_{d.pc + 4:08X};", f"L_{d.pc:08X}:; {dcode}"]
                    labels.add(d.pc + 4)
                if d:
                    delay_done.add(d.pc)
                out += self.control(i, kind, dcode, start, end)
                out += after
                k += 2
                continue
            # load delay: the next instruction still sees the old register value
            lt = i.load_target()
            nxt = ins[k + 1] if k + 1 < len(ins) else None
            if lt and nxt and not nxt.kind() and nxt.pc not in labels and lt in nxt.reads():
                expr, reg = load_value(i)
                ncode = value(nxt)
                if ncode is None:
                    ncode = f"port_rc_bad(0x{nxt.pc:08X}u, 0x{nxt.w:08X}u);"
                out.append(f"    {{ u32 ld = {expr}; {ncode} R({reg}) = ld; }}")
                k += 2
                continue
            code = value(i)
            if code is None:
                code = f"port_rc_bad(0x{i.pc:08X}u, 0x{i.w:08X}u);"
            if code:
                out.append(f"    {code}")
            k += 1
        # falling off the end continues into the next function
        out.append(f"    {self.call(end, end)} return;")
        out.append("}")
        return "\n".join(out)

    def control(self, i, kind, dcode, start, end):
        pc = i.pc
        if kind == "branch":
            t = i.btarget()
            go = f"goto L_{t:08X};" if start <= t < end else f"{{ {self.call(t, pc)} return; }}"
            return [f"    {{ int t = {cond(i)}; {dcode} if (t) {go} }}"]
        if kind == "bal":
            t = i.btarget()
            return [f"    {{ int t = {cond(i)}; {dcode} R(31) = 0x{pc + 8:08X}u; if (t) {{ {self.call(t, pc)} }} }}"]
        if kind == "j":
            t = i.target
            if start <= t < end:
                return [f"    {dcode} goto L_{t:08X};"]
            return [f"    {dcode} {self.call(t, pc)} return;"]
        if kind == "jal":
            return [f"    {dcode} R(31) = 0x{pc + 8:08X}u; {self.call(i.target, pc)}"]
        if kind == "jalr":
            return [f"    {{ u32 t = {R(i.rs)}; {dcode} {set_reg(i.rd, f'0x{pc + 8:08X}u')} port_rc_call(c, t); }}"]
        # jr
        if i.rs == 31:
            return [f"    {dcode} return;"]
        cases = "".join(f" case 0x{t:08X}u: goto L_{t:08X};" for t in sorted(self.local_targets(start, end)))
        return [f"    {{ u32 t = {R(i.rs)}; {dcode} switch (t) {{{cases} default: port_rc_call(c, t); return; }} }}"]

    def local_targets(self, start, end):
        return [t for t in self.jtbl_targets if start < t < end and (t - start) % 4 == 0]

    def run(self, out_path):
        # candidate jump-table targets: every aligned word of the image that points into recompiled code
        lo = min(a for _, a, _ in self.funcs)
        hi = max(a + s for _, a, s in self.funcs)
        self.jtbl_targets = set()
        for o in range(0x800, len(self.exe) - 3, 4):
            v = int.from_bytes(self.exe[o:o + 4], "little")
            if lo <= v < hi:
                self.jtbl_targets.add(v)
        bodies = [self.function(n, a, s) for n, a, s in self.funcs]
        exports = []
        for n, a, s in self.funcs:
            if n in self.native or n in HOST_LIBC:
                continue
            params = ", ".join(f"u32 a{k}" for k in range(NARGS))
            copies = " ".join(f"M32(sp + {16 + 4 * (k - 4)}) = a{k};" for k in range(4, NARGS))
            exports.append(f"u32 {n}({params}) {{\n"
                           f"    R3000 ctx; R3000 *c = &ctx; u32 sp = port_rc_enter();\n"
                           f"    R(4) = a0; R(5) = a1; R(6) = a2; R(7) = a3; R(29) = sp; R(31) = 0;\n"
                           f"    {copies}\n"
                           f"    rc_{a:08X}(c);\n    port_rc_leave(sp);\n    return R(2);\n}}")
        text = ["/* Generated by port/recomp.py from the PS1 build of " + self.level + ". Do not edit. */",
                '#include "recomp.h"', "#define R(n) c->r[n]", ""]
        text += [f"void rc_{a:08X}(R3000 *c);" for _, a, _ in self.funcs]
        text += [f"u32 {n}();" for n in sorted(self.used_natives)]
        text += [""] + bodies + [""] + exports
        text += ["", "const struct { u32 addr; RcFunc fn; } port_rc_funcs[] = {"]
        text += [f"    {{0x{a:08X}u, rc_{a:08X}}}," for _, a, _ in self.funcs]
        text += ["};", "const int port_rc_func_count = sizeof(port_rc_funcs) / sizeof(port_rc_funcs[0]);", ""]
        Path(out_path).write_text("\n".join(text))
        return len(self.funcs), len(exports)


def main():
    level, out = sys.argv[1], sys.argv[2]
    native = {f[2] for f in nm(sys.argv[3:]) if len(f) == 3 and f[1] in "TW"}
    names = psyq_names(level)
    native |= {n for n, t in names.items() if t in native}
    funcs, addr_of = functions(level)
    # natively implemented names keep their native version; host libc names use the host's
    native_set = native | (HOST_LIBC & set(addr_of))
    # start() (BSS clear, jump to main) is done by the runtime
    todo = [(n, a, s) for n, a, s in funcs if n not in native_set and n != "start"]
    g = Gen(level, todo, addr_of, {n: True for n in native_set})
    # aliased names (psyq_names) resolve to the native implementation
    for n, t in names.items():
        if n in addr_of and t in native:
            g.name_at[addr_of[n]] = t
    nf, ne = g.run(out)
    print(f"{level}: recompiled {nf} functions ({ne} exported), calls {len(g.used_natives)} native functions")


if __name__ == "__main__":
    main()
