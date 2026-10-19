"""Splice one function's C into its source file, build, and report the compiler error or checksum result.
Keeps the change only if the build still matches."""
import argparse
import fcntl
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import auto_match as am  # noqa: E402


def integrate(func, code, level="level01", keep=True, replace_decls=False, nonmatching=False):
    # Several processes (permuter, parallel decompilers) integrate into the same files: one at a time.
    with open(ROOT / "build/.integrate.lock", "w") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        return _integrate(func, code, level, keep, replace_decls, nonmatching)


def asm_block(text, folder, func):
    """The text that stands for the function in the matching build: its INCLUDE_ASM line, or the whole
    `#ifdef NON_MATCHING ... #else INCLUDE_ASM #endif` block when a non-matching version is banked."""
    line = f'INCLUDE_ASM("{folder}", {func});'
    lines = text.split("\n")
    i = lines.index(line)
    if 0 < i < len(lines) - 1 and lines[i - 1] == "#else" and lines[i + 1] == "#endif":
        start = max(k for k in range(i) if lines[k] == "#ifdef NON_MATCHING")
        return "\n".join(lines[start:i + 2])
    return line


def nonmatching_compiles(c_file, text):
    """cpp + cc1 of the file with -DNON_MATCHING; returns the compiler error or None."""
    gcc = ROOT / "tools/old-gcc/2.8.1-psx"
    src = ROOT / "build/.nonmatching_check.c"
    src.write_text(text)
    pre = subprocess.run([str(gcc / "cpp"), "-P", "-undef", "-D__GNUC__=2", "-D_LANGUAGE_C", "-DNON_MATCHING",
                          f"-I{ROOT}/include", f"-I{ROOT}/src", str(src)], capture_output=True, text=True)
    if pre.returncode:
        return pre.stderr.strip()[-300:]
    r = subprocess.run([str(gcc / "cc1"), "-quiet", "-G0", am.file_opt(c_file.stem), "-o", "/dev/null"],
                       input=pre.stdout, capture_output=True, text=True)
    errs = [l for l in r.stderr.splitlines() if "warning" not in l and "In function" not in l]
    return "\n  ".join(errs[:6]) if r.returncode else None


def _integrate(func, code, level, keep, replace_decls, nonmatching=False):
    for c_file in sorted((ROOT / "src" / level).glob("*.c")):
        original = c_file.read_text()
        hits = [(folder, f) for folder, f in am.INCLUDE_RE.findall(original) if f == func]
        if hits:
            break
    else:
        return f"{func}: no INCLUDE_ASM line found"
    folder = hits[0][0]
    # Matching hints go through the common.h macros, which compile away in the native port.
    code = re.sub(r'(register [^;=()]*?\w)\s+(?:__)?asm\(("\$?\w+")\)', r"\1 REG(\2)", code)
    code = re.sub(r'__asm__\(\s*""\s*:\s*"\+r"\((\w+)\)\s*\);', r"KEEP(\1);", code)
    decls, body = am.split_m2c(code)
    if decls is None:
        return f"{func}: could not find the function definition in the code"
    existing = am.top_level_decls(original)
    new = [d.strip() for d in decls if am.decl_name(d) not in existing and am.decl_name(d) not in am.header_decl_names()]
    text = original
    if replace_decls:
        for d in decls:
            n = am.decl_name(d)
            if n in existing and existing[n] != d.split("/*")[0].strip() and n not in am.header_decl_names():
                text = "\n".join(d.strip() if am.decl_name(l) == n and l.split("/*")[0].strip() == existing[n] else l
                                 for l in text.splitlines()) + "\n"
    new += [d for d in am.missing_context_decls(original + "\n".join(new), body, am.context_decls()) if d not in new]
    old = asm_block(text, folder, func)
    if nonmatching:
        # Banked for the native port: the matching build keeps the asm.
        text = text.replace(old, f'#ifdef NON_MATCHING\n{body.strip()}\n#else\nINCLUDE_ASM("{folder}", {func});\n#endif')
    else:
        text = text.replace(old, body.strip())
    if new:
        # After the last typedef of the file's declaration block, so new declarations can use the types;
        # new typedefs go first.
        new = sorted(new, key=lambda d: not d.startswith("typedef "))
        lines = text.split("\n")
        first_code = next(i for i, l in enumerate(lines) if l.startswith("INCLUDE_ASM(") or am.DEF_RE.match(l))
        typedefs = [i for i, l in enumerate(lines[:first_code]) if l.startswith("typedef ")]
        at = typedefs[-1] + 1 if typedefs else text.partition("\n\n")[0].count("\n") + 1
        mark = [i for i, l in enumerate(lines[:first_code]) if "kept by tools/port_level.py" in l]
        if mark:  # a generated file (other levels): below the marker, where port_level keeps them
            at = mark[0] + 1
        text = "\n".join(lines[:at] + new + lines[at:])
    text = am.sync_prototypes(text)
    if nonmatching:
        err = nonmatching_compiles(c_file, text)
        if err:
            return f"{func}: FAILED (NON_MATCHING compile)\n  {err}"
    c_file.write_text(text)
    r = subprocess.run(["make", f"LEVEL={level}", "-j8"], cwd=ROOT, capture_output=True, text=True)
    ok = r.returncode == 0 and ": OK" in r.stdout
    if not (ok and keep):
        c_file.write_text(original)
        # The restored file can share the failed build's mtime, so make would keep the stale object.
        (ROOT / f"build/{level}/src/{level}/{c_file.stem}.o").unlink(missing_ok=True)
        subprocess.run(["make", f"LEVEL={level}", "-j8"], cwd=ROOT, capture_output=True, text=True)
    if ok:
        return f"{func}: OK, build still matches" + ("" if keep else " (not kept)")
    errs = [l for l in r.stderr.splitlines() if "warning" not in l and not l.startswith("make")]
    if not errs and "FAILED" in r.stdout:
        errs = ["checksum mismatch: compiles, but the build no longer matches"]
    return f"{func}: FAILED\n  " + "\n  ".join(errs[:6])


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("func")
    ap.add_argument("c_file", nargs="?", help="file with decls + function; default: m2c code from --results")
    ap.add_argument("--results", default=str(ROOT / "build/auto_match_refresh.json"))
    ap.add_argument("--dry", action="store_true")
    ap.add_argument("--replace-decls", action="store_true", help="overwrite conflicting existing declarations")
    ap.add_argument("--level", default="level01")
    ap.add_argument("--nonmatching", action="store_true",
                    help="bank a near miss under #ifdef NON_MATCHING (the matching build keeps the asm)")
    args = ap.parse_args()
    if args.c_file:
        code = "".join(l for l in Path(args.c_file).read_text().splitlines(True) if not l.startswith("#include"))
    else:
        import json
        code = json.loads(Path(args.results).read_text())[args.func]["code"]
    print(integrate(args.func, code, level=args.level, keep=not args.dry, replace_decls=args.replace_decls,
                    nonmatching=args.nonmatching))


if __name__ == "__main__":
    main()
