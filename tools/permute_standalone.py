"""Permute self-contained candidate files (with their own includes/externs), search only, no integration.
Usage: permute_standalone.py FILE.c... --opt -O2 --minutes 5 --threads 4"""
import argparse
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import permute  # noqa: E402

GCC = ROOT / "tools/old-gcc/2.8.1-psx"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("files", nargs="+")
    ap.add_argument("--opt", default="-O2")
    ap.add_argument("--minutes", type=int, default=5)
    ap.add_argument("--threads", type=int, default=4)
    args = ap.parse_args()
    for f in map(Path, args.files):
        func = f.stem
        d = permute.WORK / f"standalone_{func}"
        d.mkdir(parents=True, exist_ok=True)
        subprocess.run([str(GCC / "cpp"), "-P", "-undef", "-D__GNUC__=2", "-D_LANGUAGE_C", f"-I{ROOT}/include",
                        str(f), str(d / "base.c")], check=True)
        tmp = permute.setup(func, "", args.opt)
        for name in ("target.o", "target.s", "compile.sh", "settings.toml"):
            (d / name).write_bytes((tmp / name).read_bytes())
        (d / "compile.sh").chmod(0o755)
        subprocess.run(["timeout", f"{args.minutes * 60}", permute.am.PY, str(ROOT / "tools/decomp-permuter/permuter.py"),
                        str(d), "-j", str(args.threads), "--stop-on-zero", "--quiet", "--best-only"],
                       capture_output=True, text=True)
        outs = sorted(d.glob("output-*/source.c"), key=lambda p: int(p.parent.name.split("-")[1]))
        print(f"{func}: {outs[0].parent.name if outs else 'no improvement'}", flush=True)


if __name__ == "__main__":
    main()
