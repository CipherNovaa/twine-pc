"""Run decomp-permuter on near-miss functions and integrate any that reach score 0.

Usage: permute.py --results build/auto_match_ctx.json --max-diff 10 --minutes 8 --parallel 3 --threads 5"""
import argparse
import json
import re
import shutil
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import auto_match as am  # noqa: E402
import integrate_one  # noqa: E402

GCC = ROOT / "tools/old-gcc/2.8.1-psx"
WORK = ROOT / "build/permuter"


def setup(func, code, opt):
    d = WORK / func
    shutil.rmtree(d, ignore_errors=True)
    d.mkdir(parents=True)
    src = d / "src.c"
    ctx = ROOT / "build/m2c_context_src.c"
    # Code that brings its own #include (a hand-written candidate) is compiled standalone.
    standalone = code.lstrip().startswith("#include")
    src.write_text(code if standalone else (ctx.read_text() if ctx.exists() else '#include "twine.h"\n') + code)
    subprocess.run([str(GCC / "cpp"), "-P", "-undef", "-D__GNUC__=2", "-D_LANGUAGE_C", f"-I{ROOT}/include",
                    str(src), str(d / "base.c")], check=True)
    s_path = next((ROOT / "asm/level01/nonmatchings").glob(f"*/{func}.s"))
    (d / "target.s").write_text(f'.include "macro.inc"\n.set noat\n.set noreorder\n.section .text\n.include "{s_path}"\n')
    subprocess.run(am.AS + [f"-I{ROOT}/include", "-o", str(d / "target.o"), str(d / "target.s")], check=True, cwd=ROOT)
    (d / "compile.sh").write_text(f"""#!/bin/sh
set -e
T=$(mktemp -d)
"{GCC}/cc1" -quiet {opt} -G0 -o "$T/a.s" "$1"
"{am.PY}" "{ROOT}/tools/maspsx/maspsx.py" --aspsx-version=2.56 --expand-div < "$T/a.s" > "$T/b.s"
cd "{ROOT}" && {' '.join(am.AS)} -I"{ROOT}/include" -o "$3" "$T/b.s"
rm -rf "$T"
""")
    (d / "compile.sh").chmod(0o755)
    (d / "settings.toml").write_text(f'func_name = "{func}"\ncompiler_type = "gcc"\nobjdump_command = "mips-elf-objdump -drz -m mips:3000"\n')
    return d


def extract_function(source, func):
    m = re.search(rf"^[A-Za-z_][\w\s\*]*\b{func}\(.*\)\s*\{{\s*$", source, re.M)
    if not m:
        return None
    depth, i = 0, m.start()
    for j in range(m.start(), len(source)):
        if source[j] == "{":
            depth += 1
        elif source[j] == "}":
            depth -= 1
            if depth == 0:
                return source[i:j + 1]
    return None


def run_one(func, code, opt, minutes, threads):
    d = setup(func, code, opt)
    subprocess.run(["timeout", f"{minutes * 60}", am.PY, str(ROOT / "tools/decomp-permuter/permuter.py"), str(d),
                    "-j", str(threads), "--stop-on-zero", "--quiet", "--best-only"],
                   capture_output=True, text=True)
    (d / "done").touch()
    zero = sorted(d.glob("output-0-*/source.c"))
    if not zero:
        best = sorted(d.glob("output-*/source.c"), key=lambda p: int(p.parent.name.split("-")[1]))
        return func, None, (best[0].parent.name if best else "no improvement")
    cand = zero[0].read_text()
    body = re.sub(r"\)\s*\n\s*\{", ") {", extract_function(cand, func), count=1)
    decls, _ = am.split_m2c(code)
    decls = [d for d in decls if not d.startswith("#")]
    base_decls = am.top_level_decls((d / "base.c").read_text())
    norm = lambda t: " ".join(t.split())
    changed = [v for k, v in am.top_level_decls(cand).items()
               if k in base_decls and norm(base_decls[k]) != norm(v) and k != func and not v.startswith("typedef")]
    replaced = {am.decl_name(c) for c in changed}
    decls = [d for d in decls if am.decl_name(d) not in replaced]
    note = "score 0" + (f" (permuter changed declarations: {changed})" if changed else "")
    return func, "\n".join(decls + changed) + "\n\n" + body + "\n", note


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--results", default=str(ROOT / "build/auto_match_ctx.json"))
    ap.add_argument("--max-diff", type=int, default=10)
    ap.add_argument("--minutes", type=int, default=8)
    ap.add_argument("--parallel", type=int, default=3)
    ap.add_argument("--threads", type=int, default=5)
    ap.add_argument("--limit", type=int, default=1000)
    args = ap.parse_args()

    _, _, o0 = am.level_info("level01")
    results = json.loads(Path(args.results).read_text())
    src = {c.stem: c.read_text() for c in (ROOT / "src/level01").glob("*.c")}
    jobs = []
    for f, v in results.items():
        if not v["status"].startswith("diff") or int(v["status"].split()[1]) > args.max_diff:
            continue
        stem = next((k for k, t in src.items() if f"{f});" in t), None)
        if stem and not (WORK / f / "done").exists():
            jobs.append((int(v["status"].split()[1]), f, v["code"], am.file_opt(stem)))
    jobs = sorted(jobs)[:args.limit]
    print(f"permuting {len(jobs)} functions, {args.parallel} at a time x {args.threads} threads, {args.minutes} min each", flush=True)
    found = 0
    with ThreadPoolExecutor(args.parallel) as ex:
        futures = [ex.submit(run_one, f, code, opt, args.minutes, args.threads) for _, f, code, opt in jobs]
        for fut in futures:
            func, new_code, note = fut.result()
            if new_code:
                verdict = integrate_one.integrate(func, new_code, replace_decls=True)
                found += verdict.endswith("matches")
                print(f"{func}: {note} -> {verdict.splitlines()[0]}", flush=True)
            else:
                print(f"{func}: {note}", flush=True)
    print(f"permuter integrated {found}")


if __name__ == "__main__":
    main()
