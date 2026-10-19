"""Phase 1: run m2c on every INCLUDE_ASM function, compile each candidate in isolation,
and record exact matches. Phase 2 (--integrate): splice matches into the real C files,
verifying the full build checksum and bisecting out anything that breaks it."""
import argparse
import json
import re
import struct
import subprocess
import sys
import tempfile
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from match_test import AS, RELOC_MASKS, compare  # noqa: E402

GCC = ROOT / "tools/old-gcc/2.8.1-psx"
PY = str(ROOT / ".venv/bin/python")
INCLUDE_RE = re.compile(r'^INCLUDE_ASM\("([^"]+)", (\w+)\);$', re.M)
SKIP_ASM = re.compile(r"jtbl|jlabel|_jt\b|%gp_rel|\bsyscall\b|\bmfc0\b|\bmtc0\b|\bctc2\b|\bcfc2\b|\bmtc2\b|\bmfc2\b|\blwc2\b|\bswc2\b|\bcop2\b|\brfe\b")
BAD_C = re.compile(r"M2C_ERROR|MULT_HI|MULTU_HI|M2C_TRAP_IF|GLUE_F64|M2C_CARRY|M2C_LWL|saved_reg|M2C_STRUCT_COPY|memcpy")


def level_info(level):
    data = (ROOT / f"workbench/extracted/L{level[-2:]}/{level.upper()}.EXE").read_bytes()
    load_addr = struct.unpack_from("<I", data, 0x18)[0]
    mk = (ROOT / "Makefile").read_text()
    o0 = set(re.search(r"O0_FILES := (.*)", mk).group(1).split())
    return data, load_addr, o0


def file_opt(stem):
    """Optimization level the original developers used for a C file (Makefile O0_FILES / O1_FILES)."""
    for mk in sorted((ROOT / "config").glob("*_opt.mk")):
        for level in ("O0", "O1"):
            m = re.search(rf"{level}_FILES := (.*)", mk.read_text())
            if m and stem in m.group(1).split():
                return f"-{level}"
    return "-O2"


def asm_info(s_path):
    txt = s_path.read_text()
    size = int(re.search(r"nonmatching \w+, 0x([0-9A-F]+)", txt).group(1), 16)
    vram = int(re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ", txt).group(1), 16)
    return txt, size, vram


def compile_unit(c_text, opt, td):
    td = Path(td)
    (td / "u.c").write_text(c_text)
    steps = [
        [str(GCC / "cpp"), "-P", "-undef", "-D__GNUC__=2", "-D_LANGUAGE_C", f"-I{ROOT}/include",
         f"-I{ROOT}/tools/m2c", str(td / "u.c"), str(td / "u.i")],
        [str(GCC / "cc1"), "-quiet", "-G0", opt, "-o", str(td / "u.cc1.s"), str(td / "u.i")],
    ]
    for cmd in steps:
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
        if r.returncode:
            return None, (r.stderr or "").strip()[-300:]
    with open(td / "u.cc1.s") as fin, open(td / "u.s", "w") as fout:
        r = subprocess.run([PY, str(ROOT / "tools/maspsx/maspsx.py"), "--aspsx-version=2.56", "--expand-div"],
                           stdin=fin, stdout=fout, stderr=subprocess.PIPE, text=True, timeout=60)
    if r.returncode:
        return None, "maspsx: " + r.stderr[-300:]
    r = subprocess.run(AS + [f"-I{ROOT}/include", "-o", str(td / "u.o"), str(td / "u.s")],
                       capture_output=True, text=True, cwd=ROOT)
    if r.returncode:
        return None, "as: " + r.stderr[-300:]
    sections = subprocess.run(["mips-elf-readelf", "-SW", str(td / "u.o")], capture_output=True, text=True).stdout
    for name in (".rodata", ".rdata", ".data", ".sdata", ".bss", ".sbss"):
        m = re.search(rf"\s{re.escape(name)}\s+\S+\s+\S+\s+\S+\s+([0-9a-f]+)", sections)
        if m and int(m.group(1), 16):
            return None, f"emits {name}"
    subprocess.run(["mips-elf-objcopy", "-O", "binary", "-j", ".text", str(td / "u.o"), str(td / "t.bin")], check=True)
    relocs = {}
    for line in subprocess.run(["mips-elf-readelf", "-rW", str(td / "u.o")], capture_output=True, text=True).stdout.splitlines():
        p = line.split()
        if len(p) >= 3 and p[2] in RELOC_MASKS:
            relocs[int(p[0], 16)] = RELOC_MASKS[p[2]]
    return ((td / "t.bin").read_bytes(), relocs), None


CONTEXT = ROOT / "build/m2c_context.c"


def build_context(level):
    """Preprocessed C with our headers plus every file-scope declaration and definition prototype."""
    seen, lines = set(header_decl_names()), []
    for c_file in sorted((ROOT / "src" / level).glob("*.c")):
        # One-line typedefs first, in file order: declarations below may use them.
        for line in c_file.read_text().splitlines():
            m = re.match(r"typedef .*?(\w+)\s*(\[[^\]]*\])?\s*;\s*$", line)
            if m and m.group(1) not in seen:
                seen.add(m.group(1))
                lines.append(line)
    for c_file in sorted((ROOT / "src" / level).glob("*.c")):
        text = c_file.read_text()
        for name, decl in top_level_decls(text).items():
            if name and name not in seen:
                seen.add(name)
                lines.append(decl)
        for line in text.splitlines():
            m = DEF_RE.match(line)
            if m and not line.startswith((" ", "\t")) and m.group(2) not in seen:
                seen.add(m.group(2))
                lines.append(m.group(1).strip() + ";")
    src = ROOT / "build/m2c_context_src.c"
    src.write_text('#include "twine.h"\n' + "\n".join(lines) + "\n")
    subprocess.run([str(GCC / "cpp"), "-P", "-undef", "-D__GNUC__=2", "-D_LANGUAGE_C", "-DM2CTX",
                    f"-I{ROOT}/include", str(src), str(CONTEXT)], check=True)
    return len(lines)


def try_function(job):
    func, s_path, opt, orig = job
    asm = Path(s_path).read_text()
    if SKIP_ASM.search(asm):
        return func, "skip-asm", None
    ctx = ["--context", str(CONTEXT)] if CONTEXT.exists() else []
    r = subprocess.run([PY, str(ROOT / "tools/m2c/m2c.py"), "-t", "mipsel-gcc-c", "--globals=used",
                        "--valid-syntax", "--no-cache", *ctx, s_path], capture_output=True, text=True, timeout=120)
    code = r.stdout
    if r.returncode or not code.strip():
        return func, "m2c-fail", None
    if BAD_C.search(code):
        return func, "m2c-unclean", code
    ctx_src = ROOT / "build/m2c_context_src.c"
    unit = (ctx_src.read_text() if CONTEXT.exists() else '#include "common.h"\n') + code
    with tempfile.TemporaryDirectory() as td:
        res, err = compile_unit(unit, opt, td)
    if not res:
        return func, ("compile: " + err.splitlines()[-1][:120]) if err else "compile", code
    bad = compare(orig, *res)
    return func, ("match" if bad == 0 else f"diff {bad}"), code


ELEM_SIZE = {"s8": 1, "u8": 1, "s16": 2, "u16": 2, "s32": 4, "u32": 4, "M2C_UNK": 4, "f32": 4, "s64": 8, "u64": 8}
SIGN_FLIP = {"s8": "u8", "u8": "s8", "s16": "u16", "u16": "s16", "s32": "u32", "u32": "s32"}


def fixup_variants(code):
    """Candidate rewrites for common m2c near-miss patterns."""
    variants = []
    sizes = {}
    for m in re.finditer(r"^\s+(\w+)\s*(\*+)\s*(\w+);", code, re.M):
        base, stars, name = m.groups()
        sizes[name] = 4 if len(stars) > 1 else ELEM_SIZE.get(base)
    def stride_fix(m):
        name, num = m.group(1), m.group(2)
        size = sizes.get(name)
        n = int(num, 0)
        if size and size > 1 and n % size == 0:
            return f"{name} += 0x{n // size:X};"
        return m.group(0)
    fixed = re.sub(r"\b(\w+) \+= (0x[0-9A-Fa-f]+|\d+);", stride_fix, code)

    def expr_stride_fix(m):
        name, op, num = m.group(1), m.group(2), m.group(3)
        size = sizes.get(name)
        n = int(num, 0)
        if size and size > 1 and n % size == 0:
            return f"{name} {op} 0x{n // size:X}"
        return m.group(0)
    expr_fixed = re.sub(r"\b(\w+) ([+-]) (0x[0-9A-Fa-f]+|\d+)\b(?!\s*[;=])", expr_stride_fix, fixed)
    # m2c treats unknown-type symbols as byte-addressed; M2C_UNK is 4 bytes, so &sym + N gets scaled.
    byte_externs = re.sub(r"^extern M2C_UNK (\w+);", r"extern u8 \1;", code, flags=re.M)
    bit_tests = re.sub(r"\(\(?([\w\[\]\.\->]+)\)? >> (\d+)\) & 1\b",
                       lambda m: f"({m.group(1)} & 0x{1 << int(m.group(2)):X}) != 0", code)
    variants += [fixed, expr_fixed, byte_externs, bit_tests]
    combo = re.sub(r"^extern M2C_UNK (\w+);", r"extern u8 \1;", expr_fixed, flags=re.M)
    variants.append(combo)
    for base in (code, fixed, combo):
        for m in re.finditer(r"^extern (s8|u8|s16|u16|s32|u32) (\w+)(\[\])?;", base, re.M):
            flipped = base[:m.start(1)] + SIGN_FLIP[m.group(1)] + base[m.end(1):]
            variants.append(flipped)
    # m2c often invents a return value that is just a leftover register.
    m = re.search(r"^(s32|u32|s16|u16|s8|u8|M2C_UNK|void \*|s32 \*) (\w+\()", code, re.M)
    if m:
        v = code[:m.start(1)] + "void" + code[m.end(1):]
        v = re.sub(r"return \(?[^;]*?\)? ?([^;]*);", lambda mm: (mm.group(1) + ";") if mm.group(1).strip() else "return;", v)
        variants.append(v)
    # O0 files tend to use register parameters.
    variants.append(re.sub(r"^(\w[\w\s\*]*\b\w+\()(.+)(\) \{)$",
                           lambda mm: mm.group(1) + ", ".join("register " + a.strip() if a.strip() != "void" else a for a in mm.group(2).split(",")) + mm.group(3),
                           code, count=1, flags=re.M))
    # `*((i * 4) + base)` where m2c typed base as an integer: index it as an array instead.
    def index_fix(signed):
        def rep(mm):
            idx, n, base = mm.group(1), int(mm.group(2)), mm.group(3)
            t = {4: "s32", 2: "s16", 1: "s8"}[n] if signed else {4: "u32", 2: "u16", 1: "u8"}[n]
            return f"(({t} *){base})[{idx}]"
        return re.sub(r"\*\(\((\w+) \* ([124])\) \+ (M2C_FIELD\([^()]+\)|\w+)\)", rep, code)
    for signed in (True, False):
        v = index_fix(signed)
        if v != code:
            variants.append(v)
    # Field signedness, one access at a time.
    fields = list(re.finditer(r"M2C_FIELD\(([^,]+), (s8|u8|s16|u16|s32|u32) \*,", code))
    for fm in fields[:10]:
        variants.append(code[:fm.start(2)] + SIGN_FLIP[fm.group(2)] + code[fm.end(2):])
    return list(dict.fromkeys(v for v in variants if v != code))


def try_fixups(job):
    func, opt, orig, code = job
    for variant in fixup_variants(code):
        with tempfile.TemporaryDirectory() as td:
            res, _ = compile_unit('#include "common.h"\n' + variant, opt, td)
        if res and compare(orig, *res) == 0:
            return func, "match", variant
    return func, "nofix", None


COMPILE_FIXUPS = "--compile-failures" in sys.argv


def phase_fixups(level, results_path, max_diff, workers):
    data, load_addr, o0 = level_info(level)
    results = json.loads(Path(results_path).read_text())
    jobs = []
    for c_file in sorted((ROOT / "src" / level).glob("*.c")):
        opt = file_opt(c_file.stem)
        for folder, func in INCLUDE_RE.findall(c_file.read_text()):
            v = results.get(func)
            if not v or not v.get("code"):
                continue
            if v["status"].startswith("diff"):
                if int(v["status"].split()[1]) > max_diff:
                    continue
            elif not (v["status"].startswith("compile") and COMPILE_FIXUPS):
                continue
            _, size, vram = asm_info(ROOT / folder / f"{func}.s")
            off = 0x800 + vram - load_addr
            jobs.append((func, opt, data[off:off + size], v["code"]))
    print(f"fixups: trying {len(jobs)} near-misses", flush=True)
    fixed = {}
    with ProcessPoolExecutor(workers) as ex:
        for func, status, code in ex.map(try_fixups, jobs):
            if status == "match":
                fixed[func] = {"status": "match", "code": code}
    out = Path(results_path).with_name(Path(results_path).stem + "_fixups.json")
    out.write_text(json.dumps(fixed, indent=1))
    print(f"fixups matched {len(fixed)}: {sorted(fixed)}  -> {out}")


def phase_try(level, max_size, workers, out):
    data, load_addr, o0 = level_info(level)
    jobs = []
    for c_file in sorted((ROOT / "src" / level).glob("*.c")):
        opt = file_opt(c_file.stem)
        for folder, func in INCLUDE_RE.findall(c_file.read_text()):
            s_path = ROOT / folder / f"{func}.s"
            _, size, vram = asm_info(s_path)
            if size > max_size:
                continue
            off = 0x800 + vram - load_addr
            jobs.append((func, str(s_path), opt, data[off:off + size]))
    jobs.sort(key=lambda j: len(j[3]))
    print(f"context: {build_context(level)} declarations", flush=True)
    print(f"trying {len(jobs)} functions with {workers} workers", flush=True)
    results = {}
    with ProcessPoolExecutor(workers) as ex:
        for i, (func, status, code) in enumerate(ex.map(try_function, jobs, chunksize=4)):
            results[func] = {"status": status, "code": code}
            if (i + 1) % 50 == 0:
                n = sum(1 for v in results.values() if v["status"] == "match")
                print(f"  {i + 1}/{len(jobs)} done, {n} matches", flush=True)
    Path(out).write_text(json.dumps(results, indent=1))
    summary = {}
    for v in results.values():
        k = v["status"].split(":")[0].split(" ")[0]
        summary[k] = summary.get(k, 0) + 1
    print("summary:", summary)


DECL_RE = re.compile(r"^(?:extern\s+.*?|[A-Za-z_][\w\s\*]*?)\b(\w+)\s*(\[[^\]]*\])?\s*(\(.*\))?\s*;")


def split_m2c(code):
    """Separate m2c output into top-level declaration lines and the function definition."""
    lines = code.strip("\n").splitlines()
    for i, line in enumerate(lines):
        if re.match(r"^[A-Za-z_][\w\s\*]*\b\w+\(.*\)\s*\{?\s*$", line) and not line.rstrip().endswith(";"):
            decls = [l for l in lines[:i] if l.strip()]
            return decls, "\n".join(lines[i:])
    return None, None


def decl_name(line):
    s = line.split("/*")[0].strip()
    if not s.endswith(";"):
        return None
    m = re.match(r"^[^(=]*?\b(\w+)\(", s)
    if m:
        return m.group(1)
    m = re.search(r"\(\s*\*\s*(\w+)\s*\)", s)
    if m:
        return m.group(1)
    m = re.match(r"^(?:extern\s+|static\s+)?[A-Za-z_][\w\s\*]*?\b(\w+)\s*(\[[^\]]*\])*\s*;$", s)
    if m:
        return m.group(1)
    m = re.match(r"^typedef\b.*?(\w+)\s*(\[[^\]]*\])*\s*;$", s)  # one-line typedef struct {...} X;
    if m:
        return m.group(1)
    m = re.match(r"^(?:extern\s+)?(?:struct|union)\b.*\}\s*\**\s*(\w+)\s*(\[[^\]]*\])*\s*;$", s)  # extern struct {...} X;
    return m.group(1) if m else None


def top_level_decls(text):
    """Declarations at file scope only (brace depth 0), keyed by declared name."""
    out, depth = {}, 0
    for line in text.splitlines():
        stripped = line.split("/*")[0].strip()
        if depth == 0 and stripped.endswith(";") and not stripped.startswith("INCLUDE_") and not line[:1].isspace():
            n = decl_name(stripped)
            if n:
                out[n] = stripped
        depth += line.count("{") - line.count("}")
    return out


DEF_RE = re.compile(r"^([A-Za-z_][\w\s\*]*?\b(\w+)\((.*)\))\s*\{\s*$")


def sync_prototypes(text):
    """Rewrite file-scope prototypes so they agree with functions defined in this file."""
    defs = {}
    for line in text.splitlines():
        m = DEF_RE.match(line)
        if m and not line.startswith((" ", "\t")):
            proto = m.group(1).strip()
            params = proto[proto.index("(") + 1:proto.rindex(")")].split(",")
            if all(re.fullmatch(r"\s*\w+\s*", p) for p in params) and params[0].strip() != "void":
                proto = proto[:proto.index("(")] + "()"  # K&R definition: the identifier list isn't a prototype
            defs[m.group(2)] = proto + ";"
    out, depth = [], 0
    for line in text.splitlines():
        stripped = line.split("/*")[0].strip()
        if depth == 0 and stripped.endswith(";") and "(" in stripped:
            n = decl_name(stripped)
            if n in defs:
                line = defs[n]
        out.append(line)
        depth += line.count("{") - line.count("}")
    return "\n".join(out) + ("\n" if text.endswith("\n") else "")


def header_decl_names():
    names = set()
    for h in (ROOT / "include").glob("*.h"):
        names |= set(top_level_decls(h.read_text()))
    return names


IDENT_RE = re.compile(r"\b((?:func|D|jtbl)_[0-9A-F]{8}|g_\w+|[A-Z][A-Za-z]+_[A-Za-z]\w*)\b")


def context_decls():
    src = ROOT / "build/m2c_context_src.c"
    return top_level_decls(src.read_text()) if src.exists() else {}


def missing_context_decls(text, body, ctx):
    """Context declarations for identifiers used in body but not declared in text or headers."""
    have = set(top_level_decls(text)) | header_decl_names()
    defined = {m.group(2) for m in map(DEF_RE.match, text.splitlines()) if m}
    out = []
    for name in dict.fromkeys(IDENT_RE.findall(body)):
        if name not in have and name not in defined and name in ctx:
            out.append(ctx[name])
    return out


def build_ok(level):
    r = subprocess.run(["make", f"LEVEL={level}", "-j8"], cwd=ROOT, capture_output=True, text=True)
    return r.returncode == 0 and ": OK" in r.stdout


def phase_integrate(level, results_path):
    results = json.loads(Path(results_path).read_text())
    matched = {f: v["code"] for f, v in results.items() if v["status"] == "match"}
    always_skip = {"g_Args"} | header_decl_names()
    ctx = context_decls()
    total_kept = 0
    for c_file in sorted((ROOT / "src" / level).glob("*.c")):
        original = c_file.read_text()
        funcs = [(folder, f) for folder, f in INCLUDE_RE.findall(original) if f in matched]
        if not funcs:
            continue
        existing = top_level_decls(original)

        def render(chosen):
            text = original
            new_decls = {}
            for folder, f in chosen:
                decls, body = split_m2c(matched[f])
                for d in decls:
                    n = decl_name(d)
                    if n and n not in always_skip and n not in existing and n not in new_decls:
                        new_decls[n] = d.strip()
                for d in missing_context_decls(original + "\n".join(new_decls.values()), body, ctx):
                    new_decls.setdefault(decl_name(d), d)
                text = text.replace(f'INCLUDE_ASM("{folder}", {f});', body.strip())
            if new_decls:
                head, sep, rest = text.partition("\n\n")
                text = head + "\n\n" + "\n".join(new_decls.values()) + "\n" + sep + rest
            return sync_prototypes(text)

        def usable(folder, f):
            decls, body = split_m2c(matched[f])
            if decls is None:
                return False
            return True

        candidates = [x for x in funcs if usable(*x)]
        kept = []

        def attempt(group):
            c_file.write_text(render(kept + group))
            return build_ok(level)

        def bisect(group):
            if not group:
                return
            if attempt(group):
                kept.extend(group)
                return
            if len(group) == 1:
                return
            mid = len(group) // 2
            bisect(group[:mid])
            bisect(group[mid:])

        for i in range(0, len(candidates), 24):
            bisect(candidates[i:i + 24])
        c_file.write_text(render(kept))
        if not build_ok(level):
            c_file.write_text(original)
            print(f"{c_file.name}: final verify failed, reverted")
            continue
        total_kept += len(kept)
        print(f"{c_file.name}: integrated {len(kept)}/{len(funcs)} matched functions", flush=True)
    print(f"total integrated: {total_kept}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--level", default="level01")
    ap.add_argument("--max-size", type=lambda s: int(s, 0), default=0x300)
    ap.add_argument("--workers", type=int, default=8)
    ap.add_argument("--results", default=str(ROOT / "build/auto_match_level01.json"))
    ap.add_argument("--integrate", action="store_true")
    ap.add_argument("--fixups", type=int, metavar="MAX_DIFF", help="retry near-misses with common rewrites")
    ap.add_argument("--compile-failures", action="store_true", help="with --fixups, also retry m2c output that did not compile")
    args = ap.parse_args()
    if args.fixups is not None:
        phase_fixups(args.level, args.results, args.fixups, args.workers)
    elif args.integrate:
        phase_integrate(args.level, args.results)
    else:
        phase_try(args.level, args.max_size, args.workers, args.results)


if __name__ == "__main__":
    main()
