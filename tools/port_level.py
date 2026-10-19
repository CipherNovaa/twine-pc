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
import port_symbols  # noqa: E402
from match_test import RELOC_MASKS  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
VRAM_BASE = 0x80026000  # vram - ROM offset, the same in every level EXE
SUBSEG_RE = re.compile(r"^(\s*)- \[(0x[0-9A-F]+), (\w+)(?:, (\w+))?\](.*)$", re.M)
IDENT = re.compile(r"\b[A-Za-z_]\w*\b")


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

    def region_start(lo, hi, prev_end):
        """LEVEL address where the LEVEL01 region [lo, hi) starts: its first function's copy if that is
        shared, else the first LEVEL function after the previous region's last shared one."""
        first = next((a for a, s, n in l1_funcs if lo <= a < hi), None)
        name = next((n for a, s, n in l1_funcs if a == first), None)
        if name in pairs:
            return pairs[name]
        return next(a for a in t_starts if a > prev_end)

    starts, prev_end = [], 0
    for lo, hi in zip(bounds, bounds[1:] + [None]):
        hi = hi or 0xFFFFFFFF
        starts.append(region_start(lo, hi, prev_end))
        shared = [pairs[n] for a, s, n in l1_funcs if lo <= a < hi and n in pairs]
        prev_end = max(shared) if shared else starts[-1]
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
        if line == "#ifdef NON_MATCHING":
            j = lines.index("#endif", i)
            block = lines[i + 1:j]
            k = block.index("#else")
            m = re.match(r'INCLUDE_ASM\("[^"]+", (\w+)\);', block[k + 1])
            funcs[m.group(1)] = ("banked", "\n".join(block[:k]))
            i = j + 1
            continue
        code = re.sub(r"/\*.*?\*/", "", line).strip()
        is_func = (line[:1].isalpha() or line[:1] == "_") and "(" in code and not code.endswith(";") \
            and not code.startswith(("typedef", "struct", "union", "enum", "extern", "static const", "#"))
        if is_func:
            j = take_block(i)
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


def object_mismatches(level, obj, wanted, pairs_by_l1, exe, load):
    """Names of L1_ functions in the object whose bytes differ from the EXE (relocations masked)."""
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
    for name in wanted:
        if name not in syms:
            bad.add(name)
            continue
        off, size = syms[name]
        o = 0x800 + pairs_by_l1[name[3:]] - load
        for k in range(0, size, 4):
            mask = ~rel.get(off + k, 0) & 0xFFFFFFFF
            if (struct.unpack_from("<I", exe, o + k)[0] ^ struct.unpack_from("<I", text, off + k)[0]) & mask:
                bad.add(name)
                if "-v" in sys.argv:
                    print(f"  {name} +0x{k:X}: {level} {struct.unpack_from('<I', exe, o + k)[0]:08X}, "
                          f"ours {struct.unpack_from('<I', text, off + k)[0]:08X} ({obj.name} +0x{off + k:X})")
                break
    return bad


def port(level):
    pairs, votes = port_symbols.pairings(level)
    smap = port_symbols.symbol_map(votes)
    l1_globals = elf_symbols(ROOT / "build/level01/level01.elf")
    ld_entries = syms_ld_entries()
    absolute = {n for n, e in ld_entries if re.fullmatch(r"0x[0-9A-Fa-f]+", e.strip())}
    aliased = {n: e for n, e in ld_entries if n not in absolute}
    renamed = {n for n, (t, d) in smap.items() if t != n} | set(aliased)
    known = set(smap) | set(aliased) | absolute
    t_name = {pairs[n]: smap[n][0] for n in pairs if n in smap and smap[n][1] == 0}
    inv = {tn: n for n, a in pairs.items() if (tn := t_name.get(a))}

    l1 = level01_sources()
    l1_stems = [name for off, kind, name in subsegments("level01") if kind == "c"]
    t_stems = [name for off, kind, name in subsegments(level) if kind == "c"]
    if len(l1_stems) != len(t_stems):
        sys.exit(f"{level} has {len(t_stems)} C files, LEVEL01 {len(l1_stems)}: run `port_level.py split {level}` first")

    def portable(text):
        """Every LEVEL01 global the code names must have a counterpart in this level."""
        return all(i in known for i in set(IDENT.findall(text)) if i in l1_globals)

    exclude = set()  # LEVEL01 names that failed to compile or match
    asm_dir = ROOT / f"asm/{level}/nonmatchings"
    exe, load = target_exe(level)
    for attempt in range(20):
        ported, jtbl_lines = {}, []
        for l1_stem, t_stem in zip(l1_stems, t_stems):
            header, funcs = l1[l1_stem]
            c_path = ROOT / f"src/{level}/{t_stem}.c"
            orig = c_path.read_text()
            if "port_level.py" in orig.split("\n", 1)[0]:
                orig = (ROOT / f"build/{level}/port_base/{t_stem}.c").read_text()
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
                n = inv.get(m.group(2))
                item = funcs.get(n) if n and n not in exclude else None
                # Banked (NON_MATCHING) LEVEL01 functions stay asm here: the matching build uses the asm.
                if item and item[0] == "c" and portable(item[1]):
                    ported[n] = t_stem
                    body.append(item[1])
                    jt = re.findall(r"%hi\((jtbl_\w+)\)", (asm_dir / m.group(1).split("/")[-1] / f"{m.group(2)}.s").read_text())
                    if jt:
                        jtbl_lines.append(f"L1_{n} {' '.join(dict.fromkeys(jt))}")
                else:
                    body.append(line)
            text = "\n".join(header) + "\n" + "\n".join(body) + "\n"
            used = set(IDENT.findall(text)) & renamed
            # asm("name") labels bypass the preprocessor: point them at the L1_ name too.
            text = re.sub(r'(asm|__asm__)\("(\w+)"\)', lambda m: f'{m.group(1)}("L1_{m.group(2)}")'
                          if m.group(2) in renamed else m.group(0), text)
            defines = "".join(f"#define {n} L1_{n}\n" for n in sorted(used))
            c_path.write_text(f"/* Generated by tools/port_level.py from src/level01/{l1_stem}.c: shared functions in C, "
                              f"the rest asm. Edit LEVEL01 and rerun. */\n{defines}\n{text}")

        ld = [f"/* Generated by tools/port_level.py: LEVEL01 names (L1_ prefix) bound to {level}'s symbols. */"]
        for n, (t, d) in sorted(smap.items()):
            if t == n:
                continue
            if n in ported and d == 0:
                ld.append(f"{t} = L1_{n};")
            elif n not in ported:
                ld.append(f"L1_{n} = {t}{f' + 0x{d:X}' if d > 0 else f' - 0x{-d:X}' if d < 0 else ''};")
        for n, e in ld_entries:
            if n in absolute:
                ld.append(f"{n} = {e.strip()};")
            else:
                ld.append(f"L1_{n} = {IDENT.sub(lambda m: 'L1_' + m.group(0) if m.group(0) in renamed else m.group(0), e)};")
        (ROOT / f"config/{level}_syms.ld").write_text("\n".join(ld) + "\n")
        (ROOT / f"config/{level}_jtbl.txt").write_text(
            "# Switch jump tables each function uses, in order of first use (tools/jtbl_fix.py).\n" + "\n".join(jtbl_lines) + "\n")

        r = subprocess.run(["make", f"LEVEL={level}", "-j8", "-k"], cwd=ROOT, capture_output=True, text=True)
        failed, unattributed, current = {}, [], None
        for line in r.stderr.splitlines():
            m = re.search(r"In function `L1_(\w+)'", line)
            if m or "At top level" in line or "In function" in line:
                current = m.group(1) if m else None
                continue
            if re.search(r"\.i:\d+: (?!warning)", line):  # an error (cc1 prints warnings with "warning:")
                n = current or (re.search(r"`L1_(\w+)'", line) or [None, None])[1]
                if n:
                    failed.setdefault(n, line.split(": ", 1)[-1])
                else:
                    unattributed.append(line)
        if unattributed and not failed:
            print("\n".join(unattributed[:10]))
            sys.exit("compile errors outside ported functions")
        if failed:
            print(f"pass {attempt}: {len(failed)} functions don't compile: {' '.join(sorted(failed))}")
            if "-v" in sys.argv:
                for n, e in sorted(failed.items()):
                    print(f"  {n}: {e}")
            exclude |= set(failed)
            continue
        if "Error" in r.stderr and not (ROOT / f"build/{level}/{level}.elf").exists():
            print(r.stderr[-3000:])
            sys.exit("build failed outside a ported function")
        bad = set()
        for t_stem in t_stems:
            obj = ROOT / f"build/{level}/src/{level}/{t_stem}.o"
            wanted = {f"L1_{n}" for n, s in ported.items() if s == t_stem}
            if wanted and obj.exists():
                bad |= {w[3:] for w in object_mismatches(level, obj, wanted, pairs, exe, load)}
        if bad:
            print(f"pass {attempt}: {len(bad)} functions differ: {' '.join(sorted(bad))}")
            exclude |= bad
            continue
        ok = r.returncode == 0 and ": OK" in r.stdout
        print(f"{level}: {len(ported)} functions from LEVEL01's C; EXE {'matches' if ok else 'DOES NOT MATCH'}")
        if not ok:
            print((r.stdout + r.stderr)[-2000:])
        return ok
    sys.exit("gave up after 20 passes")


if __name__ == "__main__":
    {"split": split, "port": port}[sys.argv[1]](sys.argv[2])
