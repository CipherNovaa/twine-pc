"""Bring LEVEL01's C to another level that shares the engine (same source files, same link order).

  port_level.py split LEVEL   give config/LEVEL.yaml C subsegments where LEVEL01's C files (and so their
                              optimization levels) start in LEVEL, and write config/LEVEL_opt.mk; then
                              run `make LEVEL=LEVEL split`
  port_level.py port LEVEL    replace INCLUDE_ASM in src/LEVEL with LEVEL01's C for every function that is
                              identical there (modulo addresses); keeps only functions that compile
                              byte-identical, then checks the EXE. Writes config/LEVEL_syms.ld and
                              config/LEVEL_jtbl.txt. Rerun after more LEVEL01 functions are matched.

Ported code keeps LEVEL01's names behind an L1_ prefix: each file starts with `#define <name> L1_<name>`
for the mapped LEVEL01 symbols it uses, and config/LEVEL_syms.ld binds each L1_<name> to LEVEL's own
symbol (paired by port_symbols.py). So a LEVEL01 name can never bind to an unrelated LEVEL symbol that
happens to have the same address-based name. Functions defined in ported C are aliased the other way
(`<LEVEL name> = L1_<name>;`) for the asm that still calls them."""
import re
import struct
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import auto_match  # noqa: E402
import port_symbols  # noqa: E402
from match_test import RELOC_MASKS  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
VRAM_BASE = 0x80026000  # vram - ROM offset, the same in every level EXE
SUBSEG_RE = re.compile(r"^(\s*)- \[(0x[0-9A-F]+), (\w+)(?:, (\w+))?\](.*)$", re.M)
IDENT = re.compile(r"\b[A-Za-z_]\w*\b")
# In generated files, declarations for this level's own C go below this line (integrate_one puts them there).
NATIVE_MARK = "/* This level's own declarations (kept by tools/port_level.py): */"


def subsegments(level):
    """[(rom offset, type, name)] from the level's yaml."""
    text = (ROOT / f"config/{level}.yaml").read_text()
    return [(int(m.group(2), 16), m.group(3), m.group(4)) for m in SUBSEG_RE.finditer(text)]


def opt_files(level):
    mk = ROOT / f"config/{level}_opt.mk"
    out = {}
    if mk.exists():
        for m in re.finditer(r"^(O[01])_FILES := (.*)$", mk.read_text(), re.M):
            for stem in m.group(2).split():
                out[stem] = "-" + m.group(1)
    return out


# ---------------------------------------------------------------------------------------------- split

def split(level):
    pairs, _ = port_symbols.pairings(level)
    t_starts = sorted(port_symbols.asm_lines(level)[1])
    l1_funcs = sorted(port_symbols.shared_code.level01_functions())
    l1_segs = subsegments("level01")
    l1_c = [(VRAM_BASE + off, name) for off, kind, name in l1_segs if kind == "c"]
    lib = next(VRAM_BASE + off for off, kind, name in l1_segs if kind == "asm")
    data_off = next(off for off, kind, name in l1_segs if kind == "data")
    bounds = [a for a, _ in l1_c] + [lib]
    opts = opt_files("level01")

    l1_addr, l1_words = port_symbols.shared_code.load(port_symbols.shared_code.EXTRACTED / "L01/LEVEL01.EXE")
    n = int(level[-2:])
    t_addr, t_words = port_symbols.shared_code.load(port_symbols.shared_code.EXTRACTED / f"L{n:02d}/LEVEL{n:02d}.EXE")

    def region_start(lo, hi, prev_end):
        """LEVEL address where the LEVEL01 region [lo, hi) starts: its first function's copy if that is
        shared; else the function most like it (masked instructions) among this level's functions between
        the previous region's last shared one and this region's first shared one; else the first of those."""
        first = next(((a, s, n) for a, s, n in l1_funcs if lo <= a < hi), None)
        if first and first[2] in pairs:
            return pairs[first[2]]
        gap_end = min((pairs[n] for a, s, n in l1_funcs if lo <= a < hi and n in pairs), default=None)
        gap = [a for a in t_starts if a > prev_end and (gap_end is None or a < gap_end)]
        if first and gap:
            import difflib
            ref = l1_words[(first[0] - l1_addr) // 4:(first[0] + first[1] - l1_addr) // 4]
            def body(a):
                end = next((b for b in t_starts if b > a), a + 4 * len(ref))
                return t_words[(a - t_addr) // 4:(end - t_addr) // 4]
            score, best = max((difflib.SequenceMatcher(None, ref, body(a), autojunk=False).ratio(), a) for a in gap)
            if score >= 0.6:
                return best
        return next(a for a in t_starts if a > prev_end)

    starts, prev_end = [], 0
    for lo, hi in zip(bounds, bounds[1:] + [None]):
        hi = hi or 0xFFFFFFFF
        starts.append(region_start(lo, hi, prev_end))
        shared = [pairs[n] for a, s, n in l1_funcs if lo <= a < hi and n in pairs]
        prev_end = max(shared) if shared else starts[-1]
    # -O0 code always sets up a frame pointer (`addu $fp, $sp, $zero`) and -O1/-O2 code never does, so the
    # -O0 regions' edges go exactly where the frame-pointer functions start and end (this level's own
    # functions next to a boundary would otherwise land on the wrong side).
    n = int(level[-2:])
    t_addr, t_words = port_symbols.shared_code.load(port_symbols.shared_code.EXTRACTED / f"L{n:02d}/LEVEL{n:02d}.EXE")

    def has_fp(a):
        end = next((b for b in t_starts if b > a), a + 64)
        return any(t_words[k] == 0x03A0F021 for k in range((a - t_addr) // 4, (end - t_addr) // 4))

    for k, (l1_start, l1_name) in enumerate(l1_c):
        if opts.get(l1_name) != "-O0":
            continue
        inside = [a for a in t_starts if starts[k] <= a < starts[k + 1]]
        fp = [a for a in inside if has_fp(a)]
        if fp:
            starts[k] = fp[0]
            starts[k + 1] = next(a for a in t_starts if a > fp[-1])
    # The data segment starts where it does in this level's yaml.
    t_text = (ROOT / f"config/{level}.yaml").read_text()
    t_data = next(off for off, kind, name in subsegments(level) if kind == "data")

    lines, o0, o1 = [], [], []
    for (l1_start, l1_name), start in zip(l1_c, starts):
        stem = f"text_{start:08X}"
        opt = opts.get(l1_name, "-O2")
        lines.append(f"      - [0x{start - VRAM_BASE:X}, c, {stem}]  # {opt[1:]}, LEVEL01 {l1_name}")
        (o0 if opt == "-O0" else o1 if opt == "-O1" else []).append(stem)
    lines.append(f"      - [0x{starts[-1] - VRAM_BASE:X}, asm, psyq_libs]  # PsyQ libraries")
    lines.append(f"      - [0x{t_data:X}, data]")
    # Replace the subsegment list.
    block = re.search(r"(    subsegments:\n)((?:      - .*\n)+)", t_text)
    t_text = t_text[:block.start(2)] + "\n".join(lines) + "\n" + t_text[block.end(2):]
    (ROOT / f"config/{level}.yaml").write_text(t_text)
    (ROOT / f"config/{level}_opt.mk").write_text(
        f"# Optimization level per C file (see tools/port_level.py)\nO0_FILES := {' '.join(o0)}\nO1_FILES := {' '.join(o1)}\n")
    print("\n".join(lines))
    print(f"data stays at 0x{t_data:X} (LEVEL01: 0x{data_off:X})")


# ----------------------------------------------------------------------------------------------- port

def parse_c(text):
    """Split a C file into header material (file-scope lines that aren't functions, in order) and
    {function name: (kind, text)} with kind 'c' (matched C) or 'banked' (the body of a NON_MATCHING block)."""
    lines = text.split("\n")
    header, funcs, i = [], {}, 0

    def depth_of(s):
        s = re.sub(r"/\*.*?\*/|//.*$|'[^']*'|\"(?:\\.|[^\"\\])*\"", "", s)
        return s.count("{") - s.count("}")

    def take_block(i):
        """Lines from i to the end of the brace block that starts at or after i. Inside #if/#else only the
        first branch counts (both branches may open the same block)."""
        depth, j, opened, cond = 0, i, False, []
        while j < len(lines):
            s = lines[j].strip()
            if s.startswith("#if"):
                cond.append(True)
            elif s.startswith("#else") and cond:
                cond[-1] = False
            elif s.startswith("#endif") and cond:
                cond.pop()
            elif all(cond):
                depth += depth_of(lines[j])
                opened = opened or "{" in re.sub(r"/\*.*?\*/", "", lines[j])
            j += 1
            if opened and depth == 0:
                break
        return j

    while i < len(lines):
        line = lines[i]
        if "/*" in line and "*/" not in line.split("/*")[-1]:  # a multi-line comment at file scope
            j = next(k for k in range(i + 1, len(lines)) if "*/" in lines[k]) + 1
            header += lines[i:j]
            i = j
            continue
        if line.startswith("INCLUDE_ASM("):
            i += 1
            continue
        if line.startswith("#") and line.endswith("\\"):  # a multi-line #define
            j = i
            while lines[j].endswith("\\"):
                j += 1
            header += lines[i:j + 1]
            i = j + 1
            continue
        if line == "#ifdef NON_MATCHING":
            # the matching #else/#endif (banked bodies can hold conditionals of their own)
            depth, j, k = 0, i + 1, None
            while True:
                s = lines[j].strip()
                if s.startswith("#if"):
                    depth += 1
                elif s == "#endif":
                    if depth == 0:
                        break
                    depth -= 1
                elif s == "#else" and depth == 0:
                    k = j - i - 1
                j += 1
            block = lines[i + 1:j]
            m = re.match(r'INCLUDE_ASM\("[^"]+", (\w+)\);', block[k + 1])
            funcs[m.group(1)] = ("banked", "\n".join(block[:k]))
            i = j + 1
            continue
        code = re.sub(r"/\*.*?\*/", "", line).strip()
        is_func = (line[:1].isalpha() or line[:1] == "_") and "(" in code and not code.endswith(";") \
            and not code.startswith(("typedef", "struct", "union", "enum", "extern", "static const", "#"))
        if is_func:
            # A function ends at the first `}` in column 0 (counting braces fails when a macro opens a block).
            j = next(k for k in range(i + 1, len(lines)) if lines[k] == "}") + 1
            body = "\n".join(lines[i:j])
            name = re.search(r"(\w+)\s*\(", code).group(1)
            funcs[name] = ("c", body)
            i = j
            continue
        if depth_of(line) > 0:  # a multi-line typedef / initializer at file scope
            j = take_block(i)
            header += lines[i:j]
            i = j
            continue
        header.append(line)
        i += 1
    return header, funcs


class Src:
    """An earlier level whose own C is ported into later levels."""
    def __init__(self, **kw):
        self.__dict__.update(kw)


PREFIXED = re.compile(r"L\d+_")  # names of code ported from another level (L1_X, L2_X, ...)


def PREFIXED_OTHER(S, name):
    """A name from a source other than S (those are declared by that source's own code here)."""
    m = PREFIXED.match(name)
    return bool(m) and m.group(0) != S.pre


def l2_decl_name(line):
    """The name a one-line declaration declares (a typedef's is the last name before the `;`)."""
    s = line.split("/*")[0].strip()
    if s.startswith("typedef"):
        d = re.search(r"(\w+)\s*(?:\[[^\]]*\]\s*)*;\s*$", s)
    else:
        d = re.match(r"[^(]*?(\w+)\s*(?:\(|\[|;|$)", s)
    return d.group(1) if d else None


def defined_name(code):
    """Name of the function a definition defines (banked blocks are keyed by their INCLUDE_ASM name)."""
    m = re.search(r"(\w+)\s*\(", re.sub(r"/\*.*?\*/", "", code, flags=re.S))
    return m.group(1) if m else None


def is_native(name, same_named):
    """C a level got on its own (not from LEVEL01 or LEVEL02 sources). Ported code is either prefixed
    (L1_/L2_), or a LEVEL01 function that pairs with the same-named function here (same address); a
    LEVEL01 name alone proves nothing, since the level can have a different function at that address."""
    return bool(name) and name not in same_named and not PREFIXED.match(name)


def level01_sources():
    """{LEVEL01 C file stem: (header lines, funcs)} in address order."""
    out = {}
    for c in sorted((ROOT / "src/level01").glob("*.c")):
        out[c.stem] = parse_c(c.read_text())
    return out


def elf_symbols(elf):
    out = subprocess.run(["mips-elf-nm", str(elf)], capture_output=True, text=True, check=True).stdout
    return {line.split()[-1] for line in out.splitlines() if len(line.split()) >= 2}


def syms_ld_entries():
    """LEVEL01's hand-written linker symbols: [(name, expression)]."""
    p = ROOT / "config/level01_syms.ld"
    return re.findall(r"^(\w+) = ([^;]+);", p.read_text(), re.M) if p.exists() else []


def target_exe(level):
    data = (ROOT / f"workbench/extracted/L{level[-2:]}/{level.upper()}.EXE").read_bytes()
    load = struct.unpack_from("<I", data, 0x18)[0]
    return data, load


def object_mismatches(level, obj, wanted, exe, load):
    """Keys of ported functions whose bytes differ from the EXE (relocations masked).
    wanted: {name the object defines: (key, address in this level)}."""
    text = subprocess.run(["mips-elf-objcopy", "-O", "binary", "-j", ".text", str(obj), "/dev/stdout"],
                          capture_output=True, check=True).stdout
    syms = {}
    for line in subprocess.run(["mips-elf-readelf", "-sW", str(obj)], capture_output=True, text=True).stdout.splitlines():
        p = line.split()
        if len(p) >= 8 and p[3] == "FUNC" and p[7] in wanted:
            syms[p[7]] = (int(p[1], 16), int(p[2]))
    rel = {}
    for line in subprocess.run(["mips-elf-readelf", "-rW", str(obj)], capture_output=True, text=True).stdout.splitlines():
        p = line.split()
        if len(p) >= 3 and p[2] in RELOC_MASKS and re.fullmatch(r"[0-9a-f]{8}", p[0]):
            rel[int(p[0], 16)] = RELOC_MASKS[p[2]]
    bad = set()
    for name, (l1, addr) in wanted.items():
        if name not in syms:
            bad.add(l1)
            continue
        off, size = syms[name]
        o = 0x800 + addr - load
        for k in range(0, size, 4):
            mask = ~rel.get(off + k, 0) & 0xFFFFFFFF
            if (struct.unpack_from("<I", exe, o + k)[0] ^ struct.unpack_from("<I", text, off + k)[0]) & mask:
                bad.add(l1)
                if "-v" in sys.argv:
                    print(f"  {name} +0x{k:X}: {level} {struct.unpack_from('<I', exe, o + k)[0]:08X}, "
                          f"ours {struct.unpack_from('<I', text, off + k)[0]:08X} ({obj.name} +0x{off + k:X})")
                break
    return bad


def nonmatching_errors(level, t_stem, renamed):
    """LEVEL01 names of banked functions in a generated file that fail to compile with -DNON_MATCHING."""
    gcc = ROOT / "tools/old-gcc/2.8.1-psx"
    c = ROOT / f"src/{level}/{t_stem}.c"
    pre = subprocess.run([str(gcc / "cpp"), "-P", "-undef", "-D__GNUC__=2", "-D_LANGUAGE_C", "-DNON_MATCHING",
                          f"-I{ROOT}/include", f"-I{ROOT}/src", str(c)], capture_output=True, text=True)
    opt = opt_files(level).get(t_stem, "-O2")
    r = subprocess.run([str(gcc / "cc1"), "-quiet", "-G0", opt, "-o", "/dev/null"], input=pre.stdout,
                       capture_output=True, text=True)
    bad, current = set(), None
    for line in r.stderr.splitlines():
        m = re.search(r"In function `(\w+)'", line)
        if m or "At top level" in line:
            current = m.group(1) if m else None
            continue
        if re.search(r":\d+: (?!warning)", line):
            name = current or (re.search(r"`(\w+)'", line) or [None, None])[1]
            if name:
                bad.add(name[3:] if name.startswith("L1_") else name)
    return bad


def port(level):
    def native_addr(func, stem):
        s = ROOT / f"asm/{level}/nonmatchings/{stem}/{func}.s"
        m = re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ", s.read_text()) if s.exists() else None
        return int(m.group(1), 16) if m else None

    pairs, votes = port_symbols.pairings(level)
    smap = port_symbols.symbol_map(votes)
    l1_globals = elf_symbols(ROOT / "build/level01/level01.elf")
    ld_entries = syms_ld_entries()
    absolute = {n for n, e in ld_entries if re.fullmatch(r"0x[0-9A-Fa-f]+", e.strip())}
    aliased = {n: e for n, e in ld_entries if n not in absolute}
    renamed = {n for n, (t, d) in smap.items() if t != n} | set(aliased)
    known = set(smap) | set(aliased) | absolute
    t_name = {pairs[n]: smap[n][0] for n in pairs if n in smap and smap[n][1] == 0}
    same_named = {n for n, (t, d) in smap.items() if t == n and d == 0}
    # LEVEL01 functions the last port put in under their own name (no L1_): only those unprefixed
    # definitions are LEVEL01's; anything else is this level's own C, even with a LEVEL01 name
    record = ROOT / f"config/{level}_samename.txt"
    if record.exists():
        same_named = set(record.read_text().split())
    inv = {tn: n for n, a in pairs.items() if (tn := t_name.get(a))}

    l1 = level01_sources()

    # Further sources: functions written for an earlier level itself (LEVEL02, LEVEL03, ...; not taken from
    # LEVEL01), in that order. Their code names that level's symbols; those get an L<n>_ prefix here, bound
    # to this level's symbols like L1_ names.
    srcs = []
    for k in range(2, int(level[-2:])):
        s_level = f"level{k:02d}"
        if not (ROOT / f"src/{s_level}").exists():
            continue
        S = Src(level=s_level, key=f"L{k}", pre=f"L{k}_", items={}, decls=[], exclude=set())
        S.pairs, votes = port_symbols.pairings(level, src=s_level)
        S.smap = port_symbols.symbol_map(votes)
        tname = {S.pairs[n]: S.smap[n][0] for n in S.pairs if n in S.smap and S.smap[n][1] == 0}
        S.inv = {tn: n for n, a in S.pairs.items() if (tn := tname.get(a))}
        S.globals = {s for s in elf_symbols(ROOT / f"build/{s_level}/{s_level}.elf") if not PREFIXED.match(s)}
        rec = ROOT / f"config/{s_level}_samename.txt"
        s_same = set(rec.read_text().split()) if rec.exists() else set()
        decl_names = set()
        for c in sorted((ROOT / f"src/{s_level}").glob("*.c")):
            h, f = parse_c(c.read_text())
            if NATIVE_MARK in h:
                for l in h[h.index(NATIVE_MARK) + 1:]:
                    # one declaration per name: a level's files can declare a symbol with different types
                    key = l2_decl_name(l) or l
                    if l.strip() and key not in decl_names:
                        decl_names.add(key)
                        S.decls.append(l)
            for n, (k2, code) in f.items():
                if k2 == "c" and is_native(defined_name(code), s_same):
                    S.items[n] = code
        # A function brought over gets its prototype from its definition (another file of that level may
        # declare it differently).
        protos = {}
        for n, code in S.items.items():
            first = code.split("{")[0].strip()
            if ";" not in first:  # not K&R
                protos[n] = re.sub(r"\s+", " ", first) + ";"
        S.decls = [protos.get(next(iter(re.findall(r"(\w+)\s*\(", l)), None), l) if "(" in l else l for l in S.decls]
        S.decls += [p for n, p in protos.items() if n not in decl_names]
        srcs.append(S)
    by_pre = {S.pre: S for S in srcs}

    def src_rename(S, text):
        return IDENT.sub(lambda m: S.pre + m.group(0) if m.group(0) in S.globals else m.group(0), text)

    def src_portable(S, code):
        """Every symbol the code names must exist here: its own level's (bound through S.smap), and names
        it took from other sources (L1_X, L<k>_X)."""
        for i in set(IDENT.findall(code)):
            if i in S.globals and i not in S.smap:
                return False
            m = PREFIXED.match(i)
            if m and m.group(0) == "L1_" and not (i[3:] in smap or i[3:] in aliased):
                return False
            if m and m.group(0) != "L1_" and (m.group(0) not in by_pre or i[len(m.group(0)):] not in by_pre[m.group(0)].smap):
                return False
        return True

    # After a re-split (file boundaries moved), the previous files wait in a pool: their own C goes to
    # whichever new file has the function now.
    pool_dir = ROOT / f"build/{level}/port_pool"
    pool_funcs, pool_decls = {}, {}  # function -> (kind, code, old file); old file -> its own declarations
    for c in sorted(pool_dir.glob("*.c")) if pool_dir.exists() else ():
        h, f = parse_c(c.read_text())
        for key, (k, code) in f.items():
            if is_native(defined_name(code), same_named):
                pool_funcs.setdefault(key, (k, code, c.name))
        pool_decls[c.name] = [l for l in (h[h.index(NATIVE_MARK) + 1:] if NATIVE_MARK in h else []) if l.strip()]
    native_drop = set()  # this level's own C that no longer compiles next to the regenerated code

    # LEVEL01's text gets its names renamed to L1_ when it's put in. Only names the shared headers declare
    # (g_Args, ...) use #defines instead, since the headers can't be rewritten; those mean the same engine
    # variable in every level, so this level's own C wants them too. A file-wide #define of an address
    # name (D_800F415C) would also hit this level's own C, where the same name is a different variable.
    hdr_names = set()
    for h in (ROOT / "include").glob("*.h"):
        hdr_names |= set(IDENT.findall(h.read_text()))
    hdr_names &= renamed

    def l1_rename(s):
        s = IDENT.sub(lambda m: "L1_" + m.group(0) if m.group(0) in renamed and m.group(0) not in hdr_names
                      else m.group(0), s)
        # asm("name") labels bypass the preprocessor
        return re.sub(r'(asm|__asm__)\("(\w+)"\)', lambda m: f'{m.group(1)}("L1_{m.group(2)}")'
                      if m.group(2) in renamed else m.group(0), s)
    l1_stems = [name for off, kind, name in subsegments("level01") if kind == "c"]
    t_stems = [name for off, kind, name in subsegments(level) if kind == "c"]
    if len(l1_stems) != len(t_stems):
        sys.exit(f"{level} has {len(t_stems)} C files, LEVEL01 {len(l1_stems)}: run `port_level.py split {level}` first")

    def portable(text):
        """Every LEVEL01 global the code names must have a counterpart in this level."""
        return all(i in known for i in set(IDENT.findall(text)) if i in l1_globals)

    # Local labels other asm jumps to (cross-function branches): a function defining one must stay asm in
    # every build, so it can't be banked here.
    defined, referenced = {}, set()
    for s in (ROOT / f"asm/{level}").rglob("*.s"):
        txt = s.read_text()
        own = set(re.findall(r"^\s*(\.L[0-9A-F]{8}):", txt, re.M))
        defined[s.stem] = own
        referenced |= set(re.findall(r"(?<![\w.])(\.L[0-9A-F]{8})\b", txt)) - own
    exclude = set()  # LEVEL01 names that failed to compile or match
    nm_exclude = set()  # banked LEVEL01 names whose C doesn't compile here
    asm_dir = ROOT / f"asm/{level}/nonmatchings"
    exe, load = target_exe(level)
    for attempt in range(20):
        ported, banked, jtbl_lines, native_names = {}, {}, [], set()
        for S in srcs:
            S.ported, S.used = {}, set()
        moved = set()  # pooled functions that landed in a different file than before
        native_stem = {}
        used1 = set()  # LEVEL01 names (L1_X) that LEVEL02's C uses here
        for l1_stem, t_stem in zip(l1_stems, t_stems):
            header, funcs = l1[l1_stem]
            c_path = ROOT / f"src/{level}/{t_stem}.c"
            current = orig = c_path.read_text()
            native_decls, native_funcs, cur_header = [], {}, []
            pooled, src_bodies = [], []
            if "port_level.py" in orig.split("\n", 1)[0]:
                orig = (ROOT / f"build/{level}/port_base/{t_stem}.c").read_text()
                # Work done on this level itself (integrate_one --level): declarations below the marker,
                # and function definitions that didn't come from LEVEL01. Keep them.
                cur_header, cur_funcs = parse_c(current)
                if NATIVE_MARK in cur_header:
                    native_decls = [l for l in cur_header[cur_header.index(NATIVE_MARK) + 1:] if l.strip()]
                native_funcs = {n: it for n, it in cur_funcs.items() if is_native(defined_name(it[1]), same_named)}
            else:
                (ROOT / f"build/{level}/port_base").mkdir(parents=True, exist_ok=True)
                (ROOT / f"build/{level}/port_base/{t_stem}.c").write_text(orig)
            body = []
            for line in orig.split("\n"):
                m = re.match(r'INCLUDE_ASM\("([^"]+)", (\w+)\);', line)
                if not m:
                    # splat writes empty functions as C; keep those (the LEVEL01 header includes common.h)
                    if not line.startswith("#include"):
                        body.append(line)
                    continue
                if m.group(2) not in native_funcs and m.group(2) in pool_funcs and m.group(2) not in native_drop:
                    k, code, old = pool_funcs[m.group(2)]
                    native_funcs[m.group(2)] = (k, code)
                    pooled.append((old, code))
                    if old != f"{t_stem}.c":
                        moved.add(m.group(2))
                if m.group(2) in native_funcs and m.group(2) not in native_drop:  # done on this level itself
                    native_names.add(m.group(2))
                    native_stem.setdefault(t_stem, []).append(m.group(2))
                    kind, code = native_funcs[m.group(2)]
                    body.append(code if kind == "c" else f"#ifdef NON_MATCHING\n{code}\n#else\n{line}\n#endif")
                    continue
                n = inv.get(m.group(2))
                item = funcs.get(n) if n and n not in exclude else None
                # Banked (NON_MATCHING) LEVEL01 functions stay asm here: the matching build uses the asm.
                if item and item[0] == "c" and portable(item[1]):
                    ported[n] = t_stem
                    body.append(l1_rename(item[1]))
                    jt = re.findall(r"%hi\((jtbl_\w+)\)", (asm_dir / m.group(1).split("/")[-1] / f"{m.group(2)}.s").read_text())
                    if jt:  # keyed by the name the C defines (no L1_ when both levels use the same name)
                        jtbl_lines.append(f"{'L1_' + n if n in renamed else n} {' '.join(dict.fromkeys(jt))}")
                elif item and item[0] == "banked" and n not in nm_exclude and portable(item[1]) \
                        and not defined.get(m.group(2), set()) & referenced:
                    # LEVEL01's near-miss C, for NON_MATCHING builds; the matching build keeps this level's asm.
                    banked[n] = t_stem
                    body.append(f"#ifdef NON_MATCHING\n{l1_rename(item[1])}\n#else\n{line}\n#endif")
                    jt = re.findall(r"%hi\((jtbl_\w+)\)", (asm_dir / m.group(1).split("/")[-1] / f"{m.group(2)}.s").read_text())
                    if jt:
                        jtbl_lines.append(f"{'L1_' + n if n in renamed else n} {' '.join(dict.fromkeys(jt))}")
                else:
                    for S in srcs:  # an earlier level's own C
                        n2 = S.inv.get(m.group(2))
                        if n2 in S.items and n2 not in S.exclude and src_portable(S, S.items[n2]):
                            S.ported[n2] = t_stem
                            body.append(src_rename(S, S.items[n2]))
                            src_bodies.append((S, body[-1]))
                            jt = re.findall(r"%hi\((jtbl_\w+)\)", (asm_dir / m.group(1).split("/")[-1] / f"{m.group(2)}.s").read_text())
                            if jt:
                                jtbl_lines.append(f"{S.pre}{n2} {' '.join(dict.fromkeys(jt))}")
                            break
                    else:
                        body.append(line)
            l1_header = [l1_rename(l) for l in header]
            # Pooled functions bring their old file's own declarations: all of them when it is the same
            # file; else only what their code names that isn't declared here (the old file's other
            # declarations can use types only its header has).
            for old in dict.fromkeys(o for o, c in pooled if o == f"{t_stem}.c"):
                native_decls += [d for d in pool_decls[old] if d not in native_decls]
            here = {l2_decl_name(l) for l in header + native_decls} - {None}
            for old, code in pooled:
                if old != f"{t_stem}.c":
                    ids = set(IDENT.findall(code))
                    for d in pool_decls[old]:
                        nm = l2_decl_name(d)
                        if nm in ids and nm not in here:
                            native_decls.append(d)
                            here.add(nm)
            src_header = []
            have = {l2_decl_name(l) for l in l1_header} - {None}
            for S in dict.fromkeys(S for S, b in src_bodies):
                # only what this file's functions from that level use (with the typedefs those declarations
                # need): the declarations' types come from that level's headers, which other files here
                # lack; skip what the LEVEL01 header here already declares (by name) and other sources' names
                cand = [(l2_decl_name(d), d) for d in (src_rename(S, d) for d in S.decls)
                        if l2_decl_name(d) not in have and not PREFIXED_OTHER(S, l2_decl_name(d) or "")]
                need, changed = set(IDENT.findall("\n".join(b for S2, b in src_bodies if S2 is S))), True
                while changed:
                    changed = False
                    for name, d in cand:
                        if name in need and d not in src_header:
                            src_header.append(d)
                            need |= set(IDENT.findall(d))
                            changed = True
                have |= {l2_decl_name(d) for d in src_header} - {None}
            src_header.sort(key=lambda d: not d.startswith("typedef"))
            # this level's own functions may rely on declarations the previous generated header had (e.g.
            # one that came with another level's code): keep those that the new one lacks
            declared = {l2_decl_name(l) for l in l1_header + src_header + native_decls} - {None}
            own = "\n".join(code for n, (k, code) in native_funcs.items() if n in native_stem.get(t_stem, ()))
            needed = set(IDENT.findall(own)) - declared
            for l in cur_header:
                nm = l2_decl_name(l)
                if nm in needed and l.strip() and not l.startswith("#"):
                    native_decls.append(l)
                    needed.discard(nm)
            text = "\n".join(l1_header + src_header + [NATIVE_MARK] + native_decls) + "\n" + "\n".join(body) + "\n"
            for S in srcs:
                S.used |= {i[len(S.pre):] for i in IDENT.findall(text) if i.startswith(S.pre)}
            # LEVEL01 names (L1_X) this file's text uses that no rename covers (LEVEL02's C, this level's
            # own C, or functions too small to pair): the level's same-named symbol
            used1 |= {i[3:] for i in IDENT.findall(text) if i.startswith("L1_")}
            used = set(IDENT.findall(text)) & hdr_names
            defines = "".join(f"#define {n} L1_{n}\n" for n in sorted(used))
            # prototypes agree with the definitions here (a LEVEL01 prototype can share a name with this
            # level's own function), as integrate_one does
            text = auto_match.sync_prototypes(text)
            new_text = (f"/* Generated by tools/port_level.py from src/level01/{l1_stem}.c: shared functions in C, "
                        f"the rest asm. Edit LEVEL01 and rerun. */\n{defines}\n{text}")
            if new_text != c_path.read_text():
                c_path.write_text(new_text)
                # a rewrite within the second of the last build can look older than its object to make
                (ROOT / f"build/{level}/src/{level}/{t_stem}.o").unlink(missing_ok=True)

        ld = [f"/* Generated by tools/port_level.py: LEVEL01 names (L1_ prefix) bound to {level}'s symbols. */"]
        for n, (t, d) in sorted(smap.items()):
            if t == n:
                continue
            if n in ported and d == 0:
                ld.append(f"{t} = L1_{n};")
            elif n in banked and d == 0:
                # whichever exists: the asm (matching build) or the C (NON_MATCHING build)
                ld.append(f"PROVIDE(L1_{n} = {t});")
                ld.append(f"PROVIDE({t} = L1_{n});")
            elif n not in ported:
                ld.append(f"L1_{n} = {t}{f' + 0x{d:X}' if d > 0 else f' - 0x{-d:X}' if d < 0 else ''};")
        for x in sorted(used1 - renamed - set(aliased)):  # same name in both levels: no L1_ alias above
            ld.append(f"L1_{x} = {x};")
        for S in srcs:
            for n2, (t2, d2) in sorted(S.smap.items()):
                if n2 not in S.used:
                    continue
                if n2 in S.ported and d2 == 0:
                    ld.append(f"{t2} = {S.pre}{n2};")
                elif n2 not in S.ported:
                    ld.append(f"{S.pre}{n2} = {t2}{f' + 0x{d2:X}' if d2 > 0 else f' - 0x{-d2:X}' if d2 < 0 else ''};")
        for n, e in ld_entries:
            if n in absolute:
                ld.append(f"{n} = {e.strip()};")
            else:
                ld.append(f"L1_{n} = {IDENT.sub(lambda m: 'L1_' + m.group(0) if m.group(0) in renamed else m.group(0), e)};")
        (ROOT / f"config/{level}_syms.ld").write_text("\n".join(ld) + "\n")
        # every function of this level under its own name too, for its own C (matched here or later)
        keyed = {l.split()[0] for l in jtbl_lines}
        for s in sorted(asm_dir.glob("*/*.s")):
            jt = re.findall(r"%hi\((jtbl_\w+)\)", s.read_text())
            if jt and s.stem not in keyed:
                keyed.add(s.stem)
                jtbl_lines.append(f"{s.stem} {' '.join(dict.fromkeys(jt))}")
        (ROOT / f"config/{level}_jtbl.txt").write_text(
            "# Switch jump tables each function uses, in order of first use (tools/jtbl_fix.py).\n" + "\n".join(jtbl_lines) + "\n")

        nm_failed = set()
        for t_stem in sorted(set(banked.values())):
            nm_failed |= nonmatching_errors(level, t_stem, renamed)
        if nm_failed:
            print(f"pass {attempt}: {len(nm_failed)} banked functions don't compile: {' '.join(sorted(nm_failed))}")
            # an earlier level's own C (L<k>_X) that is wrong here too
            src_failed = {(S, n[len(S.pre):]) for n in nm_failed for S in srcs if n.startswith(S.pre)}
            nm_failed -= {S.pre + n for S, n in src_failed}
            n_failed = nm_failed & native_names
            nm_failed -= n_failed
            if nm_failed <= nm_exclude and all(n in S.exclude for S, n in src_failed) and not n_failed - native_drop:
                if moved - native_drop:  # declarations that came along with moved functions: drop those
                    print(f"  dropped back to asm (moved to another file, declarations clash): {' '.join(sorted(moved))}")
                    native_drop |= moved
                    continue
                sys.exit("the NON_MATCHING errors are outside the banked functions (declarations?)")
            nm_exclude |= nm_failed
            for S, n in src_failed:
                S.exclude.add(n)
            native_drop |= n_failed
            continue
        r = subprocess.run(["make", f"LEVEL={level}", "-j8", "-k"], cwd=ROOT, capture_output=True, text=True)
        def owner(name):
            m = PREFIXED.match(name or "")
            if m and m.group(0) in by_pre:
                return (by_pre[m.group(0)].key, name[len(m.group(0)):])
            if name and name.startswith("L1_"):
                return ("L1", name[3:])
            if name in native_names:
                return ("N", name)
            return ("L1", name) if name in ported else None

        failed, unattributed, current = {}, [], None
        for line in r.stderr.splitlines():
            m = re.search(r"In function `(\w+)'", line)
            if m or "At top level" in line:
                current = owner(m.group(1)) if m else None
                continue
            if re.search(r"\.i:\d+: (?!warning)", line):  # an error (cc1 prints warnings with "warning:")
                key = current or owner((re.search(r"`(L[12]_\w+)'", line) or [None, None])[1])
                if key:
                    failed.setdefault(key, line.split(": ", 1)[-1])
                else:
                    unattributed.append(line)
        if unattributed and not failed:
            if moved - native_drop:
                print(f"  dropped back to asm (moved to another file, declarations clash): {' '.join(sorted(moved))}")
                native_drop |= moved
                continue
            print("\n".join(unattributed[:10]))
            sys.exit("compile errors outside ported functions")
        if failed:
            print(f"pass {attempt}: {len(failed)} functions don't compile: {' '.join(sorted(s + ':' + n for s, n in failed))}")
            if "-v" in sys.argv:
                for n, e in sorted(failed.items()):
                    print(f"  {n}: {e}")
            exclude |= {n for s, n in failed if s == "L1"}
            for S in srcs:
                S.exclude |= {n for s, n in failed if s == S.key}
            native_drop |= {n for s, n in failed if s == "N"}
            if native_drop:
                print(f"  dropped back to asm (this level's own C no longer compiles): {' '.join(sorted(native_drop))}")
            continue
        if "Error" in r.stderr and not (ROOT / f"build/{level}/{level}.elf").exists():
            print(r.stderr[-3000:])
            sys.exit("build failed outside a ported function")
        bad = set()
        for t_stem in t_stems:
            obj = ROOT / f"build/{level}/src/{level}/{t_stem}.o"
            wanted = {("L1_" + n if n in renamed else n): (("L1", n), pairs[n]) for n, s in ported.items() if s == t_stem}
            for S in srcs:
                wanted.update({S.pre + n: ((S.key, n), S.pairs[n]) for n, s in S.ported.items() if s == t_stem})
            # this level's own C too: a stale copy of excluded code must not pass as native
            for f in native_names & set(native_stem.get(t_stem, ())):
                a = native_addr(f, t_stem)
                if a is not None and f not in wanted:
                    wanted[f] = (("N", f), a)
            if wanted and obj.exists():
                bad |= object_mismatches(level, obj, wanted, exe, load)
        if bad:
            print(f"pass {attempt}: {len(bad)} functions differ: {' '.join(sorted(s + ':' + n for s, n in bad))}")
            exclude |= {n for s, n in bad if s == "L1"}
            for S in srcs:
                S.exclude |= {n for s, n in bad if s == S.key}
            native_drop |= {n for s, n in bad if s == "N"}
            continue
        ok = r.returncode == 0 and ": OK" in r.stdout
        if not ok and (ROOT / f"build/{level}/{level}.exe").exists():
            # Bytes matched function by function (relocations masked), so a symbol is bound wrongly
            # somewhere: find the ported functions the linked EXE differs in and leave them out.
            built = (ROOT / f"build/{level}/{level}.exe").read_bytes()
            diff = [k for k in range(0x800, min(len(built), len(exe))) if built[k] != exe[k]]
            syms = sorted((int(a, 16), name) for a, typ, name in (
                l.split() for l in subprocess.run(["mips-elf-nm", "-n", str(ROOT / f"build/{level}/{level}.elf")],
                                                   capture_output=True, text=True).stdout.splitlines() if len(l.split()) == 3)
                if typ in "Tt")
            hit = set()
            for k in diff:
                addr = k - 0x800 + load
                at = max((s[0] for s in syms if s[0] <= addr), default=None)
                # several names can sit at one address (this level's name is an alias of L1_/L2_ ones)
                for key in filter(None, (owner(n) for a, n in syms if a == at)):
                    hit.add(key)
            if hit:
                print(f"pass {attempt}: the linked EXE differs in {' '.join(sorted(s + ':' + n for s, n in hit))}")
                exclude |= {n for s, n in hit if s == "L1"}
                for S in srcs:
                    S.exclude |= {n for s, n in hit if s == S.key}
                native_drop |= {n for s, n in hit if s == "N"}
                continue
        print(f"{level}: {len(ported)} functions from LEVEL01's C, "
              + "".join(f"{len(S.ported)} from {S.level.upper()}'s, " for S in srcs)
              + f"EXE {'matches' if ok else 'DOES NOT MATCH'}")
        if ok:
            record.write_text("\n".join(sorted(n for n in list(ported) + list(banked) if n not in renamed)) + "\n")
        if ok and pool_dir.exists():
            import shutil
            shutil.rmtree(pool_dir)
        if not ok:
            print((r.stdout + r.stderr)[-2000:])
        return ok
    sys.exit("gave up after 20 passes")


if __name__ == "__main__":
    import fcntl
    # Helpers integrate into the same files (tools/integrate_one.py): one at a time.
    with open(ROOT / "build/.integrate.lock", "w") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        {"split": split, "port": port}[sys.argv[1]](sys.argv[2])
