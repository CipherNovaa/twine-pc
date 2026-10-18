"""Make a split level shiftable: after this, code and data can grow or shrink and everything still links.
The matching build stays byte-identical. Run after `splat split` on a tree that currently builds matching.

- Raw `.word 0x8002....` pointers in the data section (switch jump tables, function tables) become
  symbol references; jump-table targets get global `jtgt_<addr>` labels in the function's asm.
- Absolute linker symbols that fall inside the EXE become `<nearest label> + offset`.
- BSS symbols become `bss_anchor + offset`, a label at the exact end of the data (main_BSS_START is
  rounded up to 16 bytes, but some levels start BSS on an 8-byte boundary). The zero padding at the end of the data file (which
  overlaps the start of BSS) is removed; tools/fix_exe.py restores it after linking.
- The header's initial PC becomes the entry symbol.

Usage: make_shiftable.py LEVEL"""
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path.cwd()
WORD_RE = re.compile(r"^(\s*/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/ \.word )0x([0-9A-F]{8})\s*$")
NON_WRITING = {"sw", "sh", "sb", "swl", "swr", "swc2", "jr", "jalr", "j", "jal", "mult", "multu", "div", "divu",
               "mthi", "mtlo", "mtc0", "mtc2", "ctc2", "teq", "tne", "break", "syscall"}
CALLER_SAVED = {"$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$ra"} | {f"$t{i}" for i in range(10)}
LINKER_NAME = re.compile(r"_(ROM|VRAM|DATA|TEXT|RODATA|BSS)(_START|_END|_SIZE)?$|^__romPos$|^_gp$")


def defined_symbols(elf):
    """Global symbols that move with their section, as {addr: name} and {name: addr}."""
    out = subprocess.run(["mips-elf-nm", "-n", str(elf)], capture_output=True, text=True, check=True).stdout
    by_addr, by_name = {}, {}
    for line in out.splitlines():
        parts = line.split()
        if len(parts) != 3 or not parts[1] in "TDRB" or "." in parts[2] or LINKER_NAME.search(parts[2]):
            continue
        a = int(parts[0], 16) & 0xFFFFFFFF
        by_addr.setdefault(a, parts[2])
        by_name[parts[2]] = a
    return by_addr, by_name


def main():
    level = sys.argv[1]
    elf = ROOT / f"build/{level}/{level}.elf"
    asm = ROOT / "asm" / level
    if re.search(r"\.word [A-Za-z_]\w*\s+/\* Initial PC", (asm / "header.s").read_text()):
        sys.exit(f"{level}: already shiftable (re-run `make split` to start from fresh splat output)")
    by_addr, by_name = defined_symbols(elf)
    addrs = sorted(by_addr)

    # The entry routine clears BSS starting at its first %hi() symbol.
    pc_addr = int(re.search(r"\.word 0x([0-9A-F]+)\s+/\* Initial PC", (asm / "header.s").read_text()).group(1), 16)
    entry = by_addr[pc_addr]
    for f in asm.rglob("*.s"):
        text = f.read_text()
        at = text.find(f"glabel {entry}\n")
        if at >= 0:
            bss_name = re.search(r"%hi\((\w+)\)", text[at:]).group(1)
            break
    abs_syms = {}
    sym_file = ROOT / f"linkers/{level}_undefined_syms_auto.txt"
    for line in sym_file.read_text().splitlines():
        m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);", line)
        if m:
            abs_syms[m.group(1)] = int(m.group(2), 16)
    bss_start = by_name.get(bss_name, abs_syms.get(bss_name))
    exe_lo = int(re.search(r"\.word 0x([0-9A-F]+)\s+/\* \.text start", (asm / "header.s").read_text()).group(1), 16)

    def nearest(a):
        lo, hi = 0, len(addrs)
        while lo < hi:
            mid = (lo + hi) // 2
            if addrs[mid] <= a:
                lo = mid + 1
            else:
                hi = mid
        base = addrs[lo - 1]
        return by_addr[base] if a == base else f"{by_addr[base]} + 0x{a - base:X}"

    bss_names = {a: n for n, a in abs_syms.items() if a >= bss_start}

    def addr_name(a):
        return nearest(a) if a < bss_start else bss_names.get(a, f"bss_anchor + 0x{a - bss_start:X}")

    # 0. Hand-written code that lives in the data section (the GTE polygon module after "POLY.c").
    linker_defs = []
    for data in sorted((asm / "data").glob("*.s")):
        n, a = symbolize_code_in_data(data, addr_name, bss_start)
        linker_defs += a
        if n:
            print(f"{data.name}: {n} absolute jumps/address loads in data-section code symbolized")

    # 1. Data: drop the zero tail that overlaps BSS, and symbolize raw pointers into the EXE.
    bss_labels = {}
    targets = {}
    for data in sorted((asm / "data").glob("*.s")):
        lines = data.read_text().splitlines()
        out = []
        cut = False
        for line in lines:
            m = re.match(r"\s*/\* [0-9A-F]+ ([0-9A-F]{8}) ", line)
            if m and int(m.group(1), 16) >= bss_start:
                cut = True
            lm = re.match(r"(?:dlabel|glabel|jlabel) (\w+)", line)
            if lm and lm.group(1) in by_name and by_name[lm.group(1)] >= bss_start:
                cut = True
            if cut:
                while out and (not out[-1].strip() or out[-1].startswith("nonmatching ")):
                    out.pop()
                if lm:
                    bss_labels[lm.group(1)] = by_name[lm.group(1)]
                v = re.search(r"\.(?:word|short|byte)\s+(\S+)", line)
                if v and v.group(1) not in ("0x00000000", "0x0000", "0x00", "0"):
                    sys.exit(f"{data}: non-zero data after BSS start: {line}")
                continue
            w = WORD_RE.match(line)
            if w:
                val = int(w.group(3), 16)
                if exe_lo <= val < bss_start:
                    if val in by_addr:
                        line = w.group(1) + by_addr[val]
                    else:
                        targets.setdefault(val, []).append(len(out))
                        line = w.group(1) + f"@@{val:08X}"
                elif val >= bss_start and val < bss_start + 0x100000 and val in by_addr:
                    line = w.group(1) + by_addr[val]
            out.append(line)
        # Mid-function targets: give them labels in the function asm, otherwise fall back to label + offset.
        placed = place_jump_targets(asm, set(targets))
        text = "\n".join(out) + "\n"
        for val in targets:
            name = placed.get(val) or nearest(val)
            text = text.replace(f"@@{val:08X}", name)
        if cut:
            text += "\n.globl bss_anchor\nbss_anchor:\n"  # BSS starts exactly here
        data.write_text(text)
        targets.clear()
        print(f"{data.name}: {len(placed)} jump-table labels added, {len(bss_labels)} labels moved to BSS")

    # 2. Linker symbols: relative to the nearest moving label, or to the start of BSS.
    new = []
    moved = 0
    for line in sym_file.read_text().splitlines():
        m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);", line)
        if m:
            a = int(m.group(2), 16)
            if a >= bss_start and a < 0x80200000:
                line = f"{m.group(1)} = bss_anchor + 0x{a - bss_start:X};"
                moved += 1
            elif exe_lo <= a < bss_start:
                line = f"{m.group(1)} = {nearest(a)};"
                moved += 1
        new.append(line)
    for name, a in sorted(bss_labels.items(), key=lambda kv: kv[1]):
        new.append(f"{name} = bss_anchor + 0x{a - bss_start:X};")
    new += linker_defs
    sym_file.write_text("\n".join(new) + "\n")
    print(f"{sym_file.name}: {moved} absolute symbols made relative, {len(bss_labels)} added")

    # 3. Code: a `lui` whose register later feeds loads/stores with raw offsets that splat did not pair.
    all_syms = {**abs_syms, **by_name}
    for f in list((asm / "nonmatchings").rglob("*.s")) + list(asm.glob("*.s")):
        n = fix_raw_hi_lo(f, exe_lo, bss_start, addr_name, all_syms)
        if n:
            print(f"{f.name}: {n} unpaired address accesses symbolized")

    # 4. Header: entry point by name (fix_exe.py writes the text size after linking).
    hdr = asm / "header.s"
    h = hdr.read_text()
    pc = re.search(r"\.word 0x([0-9A-F]+)(\s+/\* Initial PC \*/)", h)
    if pc:
        hdr.write_text(h.replace(pc.group(0), f".word {by_addr[int(pc.group(1), 16)]}{pc.group(2)}"))


GPR = ["zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
       "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"]
POLY_MARKER = '.asciz "POLY.c"'  # source-file tag splat shows in front of the module


def symbolize_code_in_data(path, addr_name, bss_start):
    """Splat leaves the polygon module as raw words. Rewrite its absolute j/jal into label references and let
    the linker fill in the halves of its lui/ori address pairs, so the module still works after it moves."""
    lines = path.read_text().splitlines()
    words = []
    for k, line in enumerate(lines):
        w = WORD_RE.match(line)
        if w and int(w.group(2), 16) < bss_start:
            words.append((k, int(w.group(2), 16), int(w.group(3), 16)))
    marker = next((k for k, l in enumerate(lines) if l.rstrip().endswith(POLY_MARKER)), None)
    start = next((i for i, w in enumerate(words) if marker is not None and w[0] > marker), None)
    if start is None:
        return 0, []
    region = words[start:]
    lo_addr, hi_addr = region[0][1], region[-1][1] + 4
    by_addr = {a: (k, v) for k, a, v in region}
    labels, changed, linker_defs = set(), 0, []
    for idx, (k, a, v) in enumerate(region):
        op = v >> 26
        if op in (2, 3):
            tgt = ((a + 4) & 0xF0000000) | ((v & 0x3FFFFFF) << 2)
            if lo_addr <= tgt < hi_addr:
                labels.add(tgt)
                lines[k] = re.sub(r"\.word 0x[0-9A-F]{8}$", f"{'jal' if op == 3 else 'j'} .Ldcode_{tgt:08X}", lines[k])
                changed += 1
        elif op == 0x0F and idx + 1 < len(region):
            rt, imm = (v >> 16) & 31, v & 0xFFFF
            k2, a2, v2 = region[idx + 1]
            if 0x8002 <= imm < 0x8020 and v2 >> 26 == 0x0D and (v2 >> 21) & 31 == rt:
                # ori zero-extends, so %hi (which assumes a sign-extended low half) is wrong whenever bit 15
                # is set; let the linker compute both halves and add them to the opcode words instead.
                target = (imm << 16) | (v2 & 0xFFFF)
                name = addr_name(target)
                sym = f"poly_addr_{a:08X}"
                lines[k] = re.sub(r"\.word 0x[0-9A-F]{8}$", f".word 0x{v & 0xFFFF0000:08X} + {sym}_hi", lines[k])
                lines[k2] = re.sub(r"\.word 0x[0-9A-F]{8}$", f".word 0x{v2 & 0xFFFF0000:08X} + {sym}_lo", lines[k2])
                linker_defs.append(f"{sym}_hi = (({name}) >> 16) & 0xFFFF;")
                linker_defs.append(f"{sym}_lo = ({name}) & 0xFFFF;")
                changed += 1
    out = []
    for k, line in enumerate(lines):
        if k == region[0][0]:
            out += [".set noreorder", ".set noat"]
        m = re.match(r"\s*/\* [0-9A-F]+ ([0-9A-F]{8}) ", line)
        if m and int(m.group(1), 16) in labels and k in {r[0] for r in region}:
            out.append(f".Ldcode_{int(m.group(1), 16):08X}:")
        out.append(line)
        if k == region[-1][0]:
            out += [".set at", ".set reorder"]
    path.write_text("\n".join(out) + "\n")
    return changed, linker_defs


BRANCHES = {"beq", "bne", "beqz", "bnez", "blez", "bgtz", "bltz", "bgez", "b", "bc2f", "bc2t"}
CALLS = {"jal", "jalr", "bal", "bltzal", "bgezal"}
INS_RE = re.compile(r"\s*/\* [0-9A-F]+ [0-9A-F]{8} [0-9A-F]{8} \*/\s+(\w+)\s*(.*)$")


def functions(lines):
    """Yield (start, end) line ranges of each function in an asm file."""
    starts = [k for k, l in enumerate(lines) if l.startswith("glabel")] + [len(lines)]
    for a, b in zip(starts, starts[1:]):
        yield a, b


def must_reach(lines, lo, hi, def_line, reg):
    """Line indices in [lo, hi) where `reg` certainly still holds the value written at def_line
    (forward must-analysis over the function's control flow, with MIPS delay slots)."""
    ins, label_at = {}, {}
    for k in range(lo, hi):
        lab = re.match(r"\s*(\.L[0-9A-F]{8}):|(?:jlabel|alabel) (\w+)", lines[k])
        if lab:
            label_at[lab.group(1) or lab.group(2)] = k
        m = INS_RE.match(lines[k])
        if m:
            ins[k] = (m.group(1), m.group(2).strip())
    order = sorted(ins)
    if not order:
        return set()
    nxt = {a: b for a, b in zip(order, order[1:])}
    first_at = lambda line: next((x for x in order if x >= line), None)

    succ, clobber_edge = {}, set()
    slots = {}
    for k in order:
        op, args = ins[k]
        if op in BRANCHES or op in CALLS or op in ("j", "jr"):
            if k in nxt:
                slots[nxt[k]] = k
    for k in order:
        op, args = ins[k]
        if op in BRANCHES or op in CALLS or op in ("j", "jr"):
            succ[k] = [nxt[k]] if k in nxt else []
        elif k in slots:
            cop, cargs = ins[slots[k]]
            out = []
            tgt = cargs.split(",")[-1].strip()
            if (cop in BRANCHES or cop == "j") and tgt in label_at and first_at(label_at[tgt]) is not None:
                out.append(first_at(label_at[tgt]))
            if cop == "jr" and cargs != "$ra":  # switch: this function's jump-table targets
                out += [first_at(t) for n, t in label_at.items() if n.startswith("jtgt_") and first_at(t) is not None]
            if (cop in BRANCHES and cop != "b") or cop in CALLS:
                if k in nxt:
                    out.append(nxt[k])
                    if cop in CALLS and reg in CALLER_SAVED:
                        clobber_edge.add((k, nxt[k]))
            succ[k] = out
        else:
            succ[k] = [nxt[k]] if k in nxt else []

    def writes(k):
        op, args = ins[k]
        return (args.split(",")[0].strip() == reg and op not in NON_WRITING
                and op not in BRANCHES and op not in CALLS and op not in ("j", "jr"))

    # The entry and alternate entry points can be reached with anything in the register.
    pinned = {order[0]} | {first_at(t) for n, t in label_at.items() if not n.startswith((".L", "jtgt_"))} - {None}
    preds = {}
    for k in order:
        for t in succ[k]:
            preds.setdefault(t, []).append(k)
    state_in = {k: k not in pinned and k in preds for k in order}
    state_out = {}
    changed = True
    while changed:
        changed = False
        for k in order:
            if state_in[k] and k not in pinned:
                if not all(state_out.get(p, True) and (p, k) not in clobber_edge for p in preds.get(k, [])):
                    state_in[k] = False
                    changed = True
            out = True if k == def_line else (False if writes(k) else state_in[k])
            if state_out.get(k) != out:
                state_out[k] = out
                changed = True
    return {k for k in order if state_in[k] and k != def_line}


def fix_raw_hi_lo(path, exe_lo, bss_start, addr_name, all_syms):
    """After `lui $r, %hi(SYM)` or a raw `lui $r, 0x800A`, rewrite `OFF($r)` accesses with a raw offset
    into %lo of the address they reach, wherever that lui is the only possible source of $r."""
    lines = path.read_text().splitlines()
    changed = 0
    for lo, hi_line in functions(lines):
        for i in range(lo, hi_line):
            m = re.search(r"lui\s+(\$\w+), (?:\((0x[0-9A-F]+) >> 16\)|%hi\((\w+)(?: \+ (0x[0-9A-F]+))?\))", lines[i])
            if not m:
                continue
            reg = m.group(1)
            if m.group(2):
                hi, base = int(m.group(2), 16), None
            else:
                if m.group(3) not in all_syms:
                    continue
                base = all_syms[m.group(3)] + int(m.group(4) or "0", 16)
                hi = (base + 0x8000) & 0xFFFF0000
            if not (exe_lo & 0xFFFF0000) <= hi < bss_start + 0x60000:
                continue
            uses = []
            for j in sorted(must_reach(lines, lo, hi_line, i, reg)):
                im = INS_RE.match(lines[j])
                op, args = im.group(1), im.group(2)
                mem = re.search(r"(?<![\w)])(-?0x[0-9A-F]+|-?\d+)\(" + re.escape(reg) + r"\)", args)
                if mem:
                    uses.append((j, mem.group(0), f"({reg})", hi + int(mem.group(1), 0)))
                add = re.match(r"(\$\w+), " + re.escape(reg) + r", (-?0x[0-9A-F]+|-?\d+)$", args.strip())
                if op == "addiu" and add:
                    uses.append((j, f"{reg}, {add.group(2)}", f"{reg}, ", hi + int(add.group(2), 0)))
            uses = [u for u in uses if exe_lo <= u[3] < bss_start + 0x60000]
            if not uses:
                continue
            if base is None:
                lines[i] = lines[i].replace(m.group(0), f"lui {reg}, %hi({addr_name(uses[0][3])})")
            for j, old, keep, a in uses:
                if base is not None and 0 <= a - all_syms[m.group(3)] < 0x1000:
                    d = a - all_syms[m.group(3)]
                    ref = m.group(3) + (f" + 0x{d:X}" if d else "")
                else:
                    ref = addr_name(a)
                lines[j] = lines[j].replace(old, f"%lo({ref}){keep}" if keep.startswith("(") else f"{keep}%lo({ref})")
                changed += 1
    if changed:
        path.write_text("\n".join(lines) + "\n")
    return changed


def place_jump_targets(asm, wanted):
    """Insert `jlabel jtgt_<addr>` before each wanted instruction in the level's asm files."""
    placed = {}
    if not wanted:
        return placed
    files = list((asm / "nonmatchings").rglob("*.s")) + [p for p in asm.glob("*.s") if p.name != "header.s"]
    for f in files:
        text = f.read_text()
        hits = [a for a in wanted if f" {a:08X} " in text]
        if not hits:
            continue
        lines = text.splitlines()
        out = []
        for line in lines:
            m = re.match(r"\s*/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/", line)
            if m and int(m.group(1), 16) in wanted and int(m.group(1), 16) not in placed:
                a = int(m.group(1), 16)
                out.append(f"jlabel jtgt_{a:08X}, global")
                placed[a] = f"jtgt_{a:08X}"
            out.append(line)
        f.write_text("\n".join(out) + "\n")
    return placed


if __name__ == "__main__":
    main()
