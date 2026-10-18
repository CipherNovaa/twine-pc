"""Splice one function's C into its source file, build, and report the compiler error or checksum result.
Keeps the change only if the build still matches."""
import argparse
import fcntl
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import auto_match as am  # noqa: E402


def integrate(func, code, level="level01", keep=True, replace_decls=False):
    # Several processes (permuter, parallel decompilers) integrate into the same files: one at a time.
    with open(ROOT / "build/.integrate.lock", "w") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        return _integrate(func, code, level, keep, replace_decls)


def _integrate(func, code, level, keep, replace_decls):
    for c_file in sorted((ROOT / "src" / level).glob("*.c")):
        original = c_file.read_text()
        hits = [(folder, f) for folder, f in am.INCLUDE_RE.findall(original) if f == func]
        if hits:
            break
    else:
        return f"{func}: no INCLUDE_ASM line found"
    folder = hits[0][0]
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
    text = text.replace(f'INCLUDE_ASM("{folder}", {func});', body.strip())
    if new:
        head, sep, rest = text.partition("\n\n")
        text = head + "\n\n" + "\n".join(new) + "\n" + sep + rest
    c_file.write_text(am.sync_prototypes(text))
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
    args = ap.parse_args()
    if args.c_file:
        code = "".join(l for l in Path(args.c_file).read_text().splitlines(True) if not l.startswith("#include"))
    else:
        import json
        code = json.loads(Path(args.results).read_text())[args.func]["code"]
    print(integrate(args.func, code, keep=not args.dry, replace_decls=args.replace_decls))


if __name__ == "__main__":
    main()
