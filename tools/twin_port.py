"""Give a level's asm functions the C of their twins in other levels: the same code modulo addresses (the
pairing masks them, see shared_code.py), where some level already has matched C. Unlike port_level.py,
which only takes code that is byte-identical, this also adapts constants that differ between the levels
(struct offsets, array sizes, counts): every immediate that differs and has no relocation is looked up as
a literal in the C and replaced. Names are mapped through the two functions' asm, so the result is the
level's own C (its own symbol names) and goes in with integrate_one.py --level.

  twin_port.py LEVEL [FUNC ...] [--dry] [-v]    try every asm (or banked) function of LEVEL with a twin"""
import argparse
import re
import struct
import subprocess
import sys
import tempfile
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import auto_match as am  # noqa: E402
import integrate_one  # noqa: E402
import port_level as pl  # noqa: E402
import port_symbols  # noqa: E402
import shared_code  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
LEVELS = [f"level{n:02d}" for n in range(1, 12)]
INSN = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8}) ([0-9A-F]{8}) \*/\s+(\S+)\s*(.*)$")
REF = re.compile(r"%(?:hi|lo)\((\w+)(?: \+ 0x([0-9A-F]+))?\)")
LITERAL = re.compile(r"(?<![\w.])(0[xX][0-9A-Fa-f]+|\d+)([uUlL]*)(?![\w.])")
HDR_NAMES = am.header_decl_names()
HDR_DECLS = {}
for _h in sorted((ROOT / "include").glob("*.h")):
    HDR_DECLS.update(am.top_level_decls(_h.read_text()))


class Level:
    """What one level has: its functions' words, which of them are C, and its linked symbols."""
    def __init__(self, level):
        self.level = level
        n = int(level[-2:])
        exe = shared_code.EXTRACTED / f"L{n:02d}/LEVEL{n:02d}.EXE"
        data = exe.read_bytes()
        self.load = struct.unpack_from("<I", data, 0x18)[0]
        size = struct.unpack_from("<I", data, 0x1C)[0]
        self.raw = list(struct.unpack_from(f"<{size // 4}I", data, 0x800))
        self.exe = data
        self.funcs = {name: (a, s) for a, s, name in shared_code.functions(level)}
        self.asm_path = {p.stem: p for p in (ROOT / f"asm/{level}/nonmatchings").glob("*/*.s")}
        # status from the source files: plain INCLUDE_ASM, banked (NON_MATCHING block) or C
        self.asm, self.banked, self.c_defs, self.file_of = set(), set(), {}, {}
        for c in sorted((ROOT / f"src/{level}").glob("*.c")):
            text = c.read_text()
            depth = 0
            for line in text.split("\n"):
                s = line.strip()
                if s == "#ifdef NON_MATCHING":
                    depth += 1
                m = re.match(r'INCLUDE_ASM\("[^"]+", (\w+)\);', line)
                if m:
                    (self.banked if depth else self.asm).add(m.group(1))
                    self.file_of[m.group(1)] = c
                if s == "#endif" and depth:
                    depth -= 1
            for name, (kind, code) in pl.parse_c(text)[1].items():
                if kind == "c":
                    self.c_defs[name] = (c, code)
        self.addr, self.at = {}, defaultdict(list)
        elf = ROOT / f"build/{level}/{level}.elf"
        for line in subprocess.run(["mips-elf-nm", str(elf)], capture_output=True, text=True).stdout.splitlines():
            p = line.split()
            if len(p) == 3:
                a = int(p[0], 16)
                self.addr.setdefault(p[2], a)
                self.at[a].append(p[2])

    def words(self, name):
        a, s = self.funcs[name]
        i = (a - self.load) // 4
        return self.raw[i:i + s // 4]

    def masked(self, name):
        return tuple(shared_code.masked(w) for w in self.words(name))

    def c_for(self, name):
        """(file, code) of the C this level compiles for its function NAME, under whatever name it defines."""
        a = self.funcs[name][0]
        for n in self.at.get(a, ()):
            if n in self.c_defs:
                return self.c_defs[n]
        return None

    def sym_addr(self, name):
        if name in self.addr:
            return self.addr[name]
        m = re.search(r"_([0-9A-F]{8})$", name)
        return int(m.group(1), 16) if m else None

    def insns(self, name):
        """[(word, refs [(symbol, addend)])] of the function's asm."""
        out = []
        for line in self.asm_path[name].read_text().splitlines():
            m = INSN.match(line)
            if m:
                refs = [(r.group(1), int(r.group(2) or "0", 16)) for r in REF.finditer(m.group(4))]
                if m.group(3) in ("jal", "j") and not m.group(4).startswith(".L"):
                    refs.append((m.group(4).strip(), 0))
                out.append((int.from_bytes(bytes.fromhex(m.group(2)), "little"), refs))  # bytes as in the file
        return out


def s16(v):
    return v - 0x10000 if v & 0x8000 else v


def const_pairs(src, s_name, dst, t_name):
    """Immediates that differ between the two functions where no symbol is involved: [(src value, dst
    value)] (16-bit fields, plus lui+ori/addiu constants); None if anything else differs."""
    a, b = src.insns(s_name), dst.insns(t_name)
    if len(a) != len(b):
        return None
    pairs, lui = [], {}
    for (wa, ra), (wb, rb) in zip(a, b):
        op = wa >> 26
        for (na, oa), (nb, ob) in zip(ra, rb):  # sym + offset: the offset can come from the C too
            if oa != ob:
                pairs.append((oa, ob, "const"))
        if ra or rb or op in (0x02, 0x03):  # symbols; j/jal (absolute even to a local label)
            continue
        if wa != wb and (wa ^ wb) & 0xFFFF0000:
            return None
        if op == 0x0F:  # lui: the full constant comes with the next ori/addiu on the register
            lui[(wa >> 16) & 31] = (wa & 0xFFFF, wb & 0xFFFF)
            continue
        if op in (0x09, 0x0D) and (wa >> 21) & 31 in lui:
            (ha, hb), rs = lui.pop((wa >> 21) & 31), (wa >> 21) & 31
            if op == 0x0D:
                va, vb = (ha << 16) | (wa & 0xFFFF), (hb << 16) | (wb & 0xFFFF)
            else:
                va, vb = ((ha << 16) + s16(wa & 0xFFFF)) & 0xFFFFFFFF, ((hb << 16) + s16(wb & 0xFFFF)) & 0xFFFFFFFF
            if va != vb:
                pairs.append((va, vb, "const"))
            continue
        if wa != wb and op == 0 and (wa & 0x3F) in (0, 2, 3) and (wa ^ wb) & ~0x7C0 == 0:
            pairs.append(((wa >> 6) & 31, (wb >> 6) & 31, "shift"))  # sll/srl/sra amount
            continue
        if wa != wb:
            pairs.append((wa & 0xFFFF, wb & 0xFFFF, "imm"))
    for r, (ha, hb) in lui.items():  # lui alone: a constant << 16
        if ha != hb:
            pairs.append((ha << 16, hb << 16, "const"))
    return pairs


def lit_value(s):
    return int(s, 8) if re.fullmatch(r"0\d+", s) else int(s, 0)


def lit_text(like, w):
    return f"0x{w:X}" if like.lower().startswith("0x") else str(w)


def substitute(texts, pairs, skip=frozenset()):
    """Replace, in each of TEXTS, the literals each differing immediate comes from: a literal of the
    value itself, or one factor of a product of literals (`30 * 0xA0`). Occurrences numbered in SKIP stay
    (a value can change in one place and not another). Returns (texts, problems, number of occurrences)."""
    problems = []
    want = {}
    for a, b, kind in pairs:
        if kind == "shift":  # a shift amount: from a literal shift count, or a division's magic
            forms = [(a, b)] if a > 1 else []
        elif kind == "const":  # as written, or negated (`-0x11940`)
            s32v = lambda v: v - 2 ** 32 if v & 0x80000000 else v
            forms = [(a, b), (-s32v(a), -s32v(b))]
        else:  # the literal may be the field as written, its signed value, or that negated (x - 4); or the
            # low half of a negative constant (`-0x8CA0` is lui 0xFFFF + 0x7360)
            forms = [(a, b), (s16(a), s16(b)), (-s16(a), -s16(b)), (0x10000 - a, 0x10000 - b)]
        for va, vb in forms:
            if va in want and want[va] != vb:
                problems.append(f"0x{va:X} maps to both 0x{want[va]:X} and 0x{vb:X}")
            want.setdefault(va, vb)
    found, occ = set(), [0]

    def product(m):
        x, y = lit_value(m.group(1)), lit_value(m.group(3))
        p = x * y
        if p in want and want[p] > 0 and want[p] != p and (want[p] % y == 0 or want[p] % x == 0):
            found.add(p)
            occ[0] += 1
            if occ[0] - 1 in skip:
                return f"\0{m.group(0)}\1"
            w = want[p]
            if w % y == 0:  # (wrapped so the literal pass below leaves it alone)
                return f"\0{lit_text(m.group(1), w // y)}{m.group(2)}{m.group(3)}\1"
            if w % x == 0:
                return f"\0{m.group(1)}{m.group(2)}{lit_text(m.group(3), w // x)}\1"
        return m.group(0)

    def rep(m):
        v = lit_value(m.group(1))
        if v in want and want[v] != v and want[v] >= 0:  # a negated form only fits a positive literal
            found.add(v)
            occ[0] += 1
            if occ[0] - 1 in skip:
                return m.group(0)
            return lit_text(m.group(1), want[v]) + m.group(2)
        return m.group(0)
    def divisor(n, a, b):
        """A division by constant N compiles to a multiply by the magic ceil(2^(32+s) / N): the divisor
        whose magic is B, when A is N's magic; else None."""
        for sh in range(33):
            for av in (a, a + 2 ** 32):
                if av and abs(2 ** (32 + sh) / av - n) < 0.5 and -(-2 ** (32 + sh) // n) in (av, av + 1):
                    # the shift that goes with it can differ too (after the mult: sra by the shift)
                    shifts = [y for x, y, k in pairs if k == "shift" and x == sh] or [sh]
                    for sh2 in shifts + list(range(33)):
                        for bv in (b, b + 2 ** 32):
                            m = round(2 ** (32 + sh2) / bv) if bv else 0
                            if m > 1 and -(-2 ** (32 + sh2) // m) in (bv, bv + 1):
                                return m
        return None
    magic = [(a, b) for a, b, kind in pairs if kind == "const"]

    def division(m):
        n = lit_value(m.group(2))
        for a, b in magic:
            w = divisor(n, a, b)
            if w and w != n:
                found.add(a)
                occ[0] += 1
                if occ[0] - 1 in skip:
                    return f"\0{m.group(0)}\1"
                return f"\0{m.group(1)}{lit_text(m.group(2), w)}{m.group(3)}\1"
        return m.group(0)
    div = re.compile(r"([/%]\s*)(0[xX][0-9A-Fa-f]+|\d+)([uUlL]*)(?![\w.])")
    texts = [div.sub(division, t) for t in texts]
    prod = re.compile(rf"(?<![\w.])(0[xX][0-9A-Fa-f]+|\d+)(\s*\*\s*)(0[xX][0-9A-Fa-f]+|\d+)(?![\w.])")
    marked = [prod.sub(product, t) for t in texts]
    # literals outside the products just rewritten
    out = ["".join(LITERAL.sub(rep, part) if k % 2 == 0 else part
                   for k, part in enumerate(re.split("[\x00\x01]", t))) for t in marked]
    for a, b, kind in pairs:
        s32v = a - 2 ** 32 if a & 0x80000000 else a
        if kind != "shift" and not ({a, s16(a), -s16(a), 0x10000 - a} if kind != "const" else {a, -s32v}) & found:
            problems.append(f"no literal for 0x{a:X} (-> 0x{b:X})")
    return out, problems, occ[0]


def header_items(text):
    """File-scope typedefs (multi-line ones joined), #defines and one-line declarations of a C file,
    as {name: text}, plus its #include lines."""
    header, _ = pl.parse_c(text)
    items, includes, i = {}, [], 0
    while i < len(header):
        line = header[i]
        if line.startswith("#include"):
            includes.append(line)
        elif line.startswith("#define"):
            j = i
            while header[j].endswith("\\"):
                j += 1
            m = re.match(r"#define (\w+)", line)
            items.setdefault(m.group(1), "\n".join(header[i:j + 1]))
            i = j
        elif line.startswith("typedef") and not line.split("/*")[0].strip().endswith(";"):
            j, depth = i, 0
            while True:
                depth += header[j].count("{") - header[j].count("}")
                if depth == 0 and header[j].split("/*")[0].strip().endswith(";"):
                    break
                j += 1
            joined = " ".join(l.split("//")[0].strip() for l in header[i:j + 1])
            items.setdefault(pl.l2_decl_name(joined), joined)
            i = j
        else:
            n = am.decl_name(line) if line.split("/*")[0].strip().endswith(";") else None  # also (*fp)(...)
            if n:
                items.setdefault(n, line.split("/*")[0].strip())
        i += 1
    return items, includes


_SMAPS = {}


def global_map(src, dst):
    """SRC symbol -> (DST symbol, offset delta) from every function the two levels share."""
    if (src.level, dst.level) not in _SMAPS:
        _SMAPS[(src.level, dst.level)] = port_symbols.symbol_map(port_symbols.pairings(dst.level, src=src.level)[1])
    return _SMAPS[(src.level, dst.level)]


def map_names(code, src, s_name, dst, t_name, want=None):
    """Rename every symbol the source C names to the target level's symbol at the matching address.
    WANT: constants that change ({source value: target value}), for symbols the asm only reaches at an
    offset (`D_X + 30 * 0xA0`). Returns (code, {old: new}, problems)."""
    pairs, bases = [], defaultdict(set)
    for (_, ra), (_, rb) in zip(src.insns(s_name), dst.insns(t_name)):
        for (na, oa), (nb, ob) in zip(ra, rb):
            aa, ab = src.sym_addr(na), dst.sym_addr(nb)
            if aa is not None and ab is not None:
                pairs.append((aa + oa, ab + ob))
                bases[aa].add(ab)  # `sym + offset` in both: the symbols pair even if the offsets differ
    syms = {}
    for x in dict.fromkeys(pl.IDENT.findall(code)):
        if x in src.addr and not x.startswith("jtgt_"):
            syms[x] = x
        elif x in HDR_NAMES and "L1_" + x in src.addr:
            syms[x] = "L1_" + x
    for x in re.findall(r'(?:asm|__asm__|__asm)\("(\w+)"\)', code):
        if x in src.addr:
            syms[x] = x
    starts = sorted({src.addr[s] for s in syms.values()})
    in_exe = lambda a: src.load <= a < 0x80200000
    mapping, problems = {}, []
    for x, s in syms.items():
        a = src.addr[s]
        if a == src.funcs[s_name][0]:
            continue
        nxt = next((b for b in starts if b > a), a + 0x10000)
        if len(bases.get(a, ())) == 1:
            t = next(iter(bases[a]))
        else:
            exact = {ab - aa for aa, ab in pairs if aa == a}
            d = exact or {ab - aa for aa, ab in pairs if a <= aa < nxt}
            g = [global_map(src, dst)[n] for n in src.at.get(a, ()) if n in global_map(src, dst)]
            if len(d) == 1:
                t = a + d.pop()
            elif not d and not in_exe(a):
                t = a  # an absolute address (scratchpad, hardware): the same in every level
            elif not d and len({dst.sym_addr(n) + o for n, o in g if dst.sym_addr(n) is not None}) == 1:
                t = dst.sym_addr(g[0][0]) + g[0][1]  # not in this function's asm; other functions pair it
            else:
                # only reached as `x + offset` where the offset is a constant that changes
                ends = {ab - want[aa - a] for aa, ab in pairs if aa > a and (aa - a) in (want or {})}
                if len(ends) == 1 and dst.at.get(next(iter(ends))):
                    t = ends.pop()
                else:
                    problems.append(f"{x}: {'not referenced' if not d else 'ambiguous'} in the asm")
                    continue
        names = dst.at.get(t, [])
        if not names:  # the asm's offsets don't lead to a symbol: a changed constant offset, or what
            # other functions pair it with
            ends = {ab - want[aa - a] for aa, ab in pairs if aa > a and (aa - a) in (want or {})}
            if len(ends) == 1 and dst.at.get(next(iter(ends))):
                t = next(iter(ends))
                names = dst.at[t]
        if not names:
            g = {dst.sym_addr(n) + o for n, o in (global_map(src, dst)[n] for n in src.at.get(a, ())
                                                  if n in global_map(src, dst)) if dst.sym_addr(n) is not None}
            if len(g) == 1:
                t = g.pop()
                names = dst.at.get(t, [])
        bare = x[3:] if x.startswith("L1_") else x
        if bare in HDR_NAMES:  # a shared header's name (other levels' files: L1_<name>), typed there
            pick = [n for n in names if n == x] or [n for n in names if n in (bare, "L1_" + bare)]
        elif x in names:  # the same name here (two names of one symbol in the source stay two)
            pick = [x]
        elif not in_exe(t):
            pick = names
        else:
            pick = [n for n in names if re.fullmatch(r"(D|func|jtbl)_[0-9A-F]{8}", n)] or \
                   [n for n in names if n.startswith("L1_")] or names
        pick = [n for n in pick if n not in mapping.values()] or pick
        if not pick:
            problems.append(f"{x}: no symbol at 0x{t:08X} in {dst.level}")
            continue
        mapping[x] = pick[0]
    out = pl.IDENT.sub(lambda m: mapping.get(m.group(0), m.group(0)), code)
    out = re.sub(r'((?:asm|__asm__|__asm)\(")(\w+)("\))', lambda m: m.group(1) + mapping.get(m.group(2), m.group(2)) + m.group(3), out)
    return out, mapping, problems


def candidates(src, s_name, dst, t_name, skip=frozenset()):
    """([(includes, C text for integrate_one)] to try, problems): constants replaced in the code only,
    then also in the typedefs, #defines and declarations it brings (a changed typedef gets a new name)."""
    c_file, code = src.c_for(s_name)
    defined = pl.defined_name(code)
    code = re.sub(rf"\b{re.escape(defined)}\(", "__TWIN_SELF__(", code, count=1)
    cp = const_pairs(src, s_name, dst, t_name)
    if cp is None:
        return [], ["differs beyond immediates"]
    # declarations the code needs, from the source file (renamed) or the shared headers; typedefs
    # and #defines transitively. A declaration naming its symbol with asm("...") keeps its C name.
    items, includes = header_items(c_file.read_text())
    labeled = [items[x] for x in dict.fromkeys(pl.IDENT.findall(code))
               if x in items and re.search(r'(?:asm|__asm__|__asm)\("\w+"\)', items[x]) and x not in src.addr]
    want = {}
    for a, b, k in cp:
        for va, vb in ([(a, b)] if k == "const" else [(-s16(a), -s16(b)), (s16(a), s16(b))] if k == "imm" else []):
            if va > 0 and va != vb:
                want.setdefault(va, vb)
    code, mapping, problems = map_names(code + "\n\0\n" + "\n".join(labeled), src, s_name, dst, t_name, want)
    if problems:
        return [], problems
    code, labeled = code.split("\n\0\n")[0], [l for l in code.split("\n\0\n")[1].split("\n") if l]
    code = code.replace("__TWIN_SELF__(", t_name + "(")
    used = set(pl.IDENT.findall(code))
    rev = {v: k for k, v in mapping.items()}
    decls, typedefs, defines = [], [], []
    for y in sorted(used):
        x = rev.get(y)
        if x is None or y == t_name:
            continue
        d = items.get(x)
        bare = x[3:] if x.startswith("L1_") else x
        if bare in ("exit", "abort") and y != bare:  # gcc's builtins know these don't return; a new name doesn't
            decls.append(f"void {y}(s32) __attribute__((noreturn));")
            continue
        if d is None or d.startswith("#define"):
            # declared by a shared header (in other levels' files through `#define g_Args L1_g_Args`)
            bare = x[3:] if x.startswith("L1_") else x
            d = HDR_DECLS.get(bare)
            if d is None:
                problems.append(f"{x}: no declaration")
                continue
            d = re.sub(rf"\b{re.escape(bare)}\b", y, d)
        else:
            d = re.sub(rf"\b{re.escape(x)}\b", y, d)
        d = re.sub(r'((?:asm|__asm__|__asm)\(")(\w+)("\))', lambda m: m.group(1) + mapping.get(m.group(2), m.group(2)) + m.group(3), d)
        decls.append(d)
    if problems:
        return [], problems
    decls += labeled
    need = set(pl.IDENT.findall(code + "\n".join(decls)))
    changed = True
    while changed:
        changed = False
        for n, d in items.items():
            if n in need and n not in mapping and n not in rev and d not in typedefs + defines \
                    and (d.startswith("typedef") or (d.startswith("#define") and not re.fullmatch(r"#define (\w+) L1_\1", d))):
                (typedefs if d.startswith("typedef") else defines).append(d)
                need |= set(pl.IDENT.findall(d))
                changed = True
    order = list(items.values())
    typedefs = sorted((d for d in typedefs if pl.l2_decl_name(d) not in HDR_NAMES), key=order.index)
    defines.sort(key=order.index)
    inc = "\n".join(includes)
    out = []
    (code_a,), p_a, n_a = substitute([code], cp, skip)
    out.append((inc, "\n".join(defines + typedefs + decls) + "\n\n" + code_a + "\n", n_a))
    pieces, p_b, n_b = substitute([code] + defines + typedefs + decls, cp, skip)
    code_b, rest = pieces[0], pieces[1:]
    d_b, t_b, decl_b = rest[:len(defines)], rest[len(defines):len(defines) + len(typedefs)], rest[len(defines) + len(typedefs):]
    renames = {}
    for old, new in zip(typedefs, t_b):
        if old != new:
            n = pl.l2_decl_name(old)
            renames[n] = f"{n}_{dst.level[-2:]}"
    if renames:
        ren = lambda t: pl.IDENT.sub(lambda m: renames.get(m.group(0), m.group(0)), t)
        code_b, d_b, t_b, decl_b = ren(code_b), [ren(t) for t in d_b], [ren(t) for t in t_b], [ren(t) for t in decl_b]
    b = (inc, "\n".join(d_b + t_b + decl_b) + "\n\n" + code_b + "\n", n_b)
    if b[1] != out[0][1]:
        out.append(b)
    return out, p_b if len(out) > 1 else p_a


def dealias(text, t_name, target):
    """Where the target file already declares a name differently (s16 vs s32, another prototype), give
    this function its own declaration of the symbol: a new C name bound with asm("symbol")."""
    decls, body = am.split_m2c(text)
    existing = am.top_level_decls(target.read_text())
    for line in target.read_text().splitlines():  # functions the file defines
        m = am.DEF_RE.match(line)
        if m and m.group(2) not in existing:
            existing[m.group(2)] = m.group(1).strip() + ";"
    norm = lambda d: " ".join(d.split("/*")[0].split())
    renames, out = {}, []
    for d in decls:
        n = am.decl_name(d)
        if n and n in existing and norm(existing[n]) != norm(d) and n not in HDR_NAMES:
            # (no L<n>_ prefix: port_level reads those as other levels' names)
            alias = f"{pl.PREFIXED.sub('', n, count=1)}_{t_name[5:] if t_name.startswith('func_') else t_name}"
            renames[n] = alias
            d = re.sub(rf"\b{re.escape(n)}\b", alias, d, count=1)
            if not d.startswith("typedef") and not re.search(r'(?:asm|__asm__|__asm)\(', d):
                d = re.sub(r"(\s*__attribute__.*)?;\s*$", lambda m: f' asm("{n}")' + (m.group(1) or "") + ";", d)
            out.append((d, True))
        else:
            out.append((d, False))
    if not renames:
        return text
    ren = lambda t: pl.IDENT.sub(lambda m: renames.get(m.group(0), m.group(0)), t)
    # the other declarations can use a renamed typedef (an alias's own asm("name") must stay)
    return "\n".join(d if done else ren(d) for d, done in out) + "\n\n" + ren(body) + "\n"


def precheck(dst, t_name, includes, text, opt):
    a, s = dst.funcs[t_name]
    orig = dst.exe[0x800 + a - dst.load:0x800 + a - dst.load + s]
    with tempfile.TemporaryDirectory() as td:
        res, err = am.compile_unit(includes + "\n" + text, opt, td)
    if not res:
        return err.splitlines()[-1] if err else "compile"
    bad = am.compare(orig, *res)
    return None if bad == 0 else f"{bad} words differ"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("level")
    ap.add_argument("funcs", nargs="*")
    ap.add_argument("--dry", action="store_true", help="don't integrate, only report")
    ap.add_argument("-v", action="store_true")
    ap.add_argument("--keep", help="write candidates here")
    args = ap.parse_args()
    lv = {l: Level(l) for l in LEVELS}
    dst = lv[args.level]
    by_key = defaultdict(list)
    for l, L in lv.items():
        for name, (a, s) in L.funcs.items():
            if s >= 16:
                by_key[L.masked(name)].append((l, name))
    opts = pl.opt_files(args.level)
    targets = args.funcs or sorted(dst.asm | dst.banked)
    done = fail = 0
    for t in targets:
        if t not in dst.funcs or dst.funcs[t][1] < 16:
            continue
        twins = []
        for l, s in by_key[dst.masked(t)]:
            if l == args.level or s in lv[l].asm or s in lv[l].banked or not lv[l].c_for(s):
                continue
            cp = const_pairs(lv[l], s, dst, t)
            if cp is not None:
                twins.append((len(cp), l, s))
        if not twins:
            continue
        twins.sort()
        stem = dst.file_of[t].stem
        opt = opts.get(stem, "-O2")
        result = None
        for n, l, s in twins[:3]:
            cands, problems = candidates(lv[l], s, dst, t)
            if not cands:
                result = (l, s, "; ".join(problems[:3]))
                continue
            switch = any(r for _, refs in dst.insns(t) for r, _ in refs if r.startswith("jtbl_"))
            tries = [c[:2] for c in cands]
            # then leave some occurrences of the changed values as they were (all subsets, if few)
            for k, (_, _, n) in enumerate(cands):
                if not switch and 0 < n <= 6:
                    for bits in range(1, 2 ** n):
                        skip = frozenset(i for i in range(n) if bits >> i & 1)
                        c2, _ = candidates(lv[l], s, dst, t, skip)
                        if k < len(c2) and c2[k][:2] not in tries:
                            tries.append(c2[k][:2])
            for includes, text in tries:
                err = None if switch else precheck(dst, t, includes, text, opt)
                if err:
                    result = (l, s, err + (f" ({'; '.join(problems[:2])})" if problems else ""))
                    continue
                if args.keep:
                    Path(args.keep).mkdir(parents=True, exist_ok=True)
                    (Path(args.keep) / f"{t}.c").write_text(includes + "\n" + text)
                if args.dry:
                    result = (l, s, "OK (pre-check)" if not switch else "switch: not pre-checked")
                    break
                r = integrate_one.integrate(t, dealias(text, t, dst.file_of[t]), level=args.level)
                result = (l, s, r.split("\n")[0] + (" | " + r.split("\n")[1].strip() if "\n" in r else ""))
                if ": OK" in r:
                    break
            if result and "OK" in result[2]:
                break
        ok = result and "OK" in result[2]
        done += ok
        fail += not ok
        print(f"{t}: from {result[0]} {result[1]}: {result[2]}" if result else f"{t}: no candidate", flush=True)
    print(f"{args.level}: {done} ok, {fail} not")


if __name__ == "__main__":
    main()
