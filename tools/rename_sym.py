"""Rename symbols across the symbol file, C sources and headers: OLD=NEW[:type] ...

OLD may be an existing name (func_8002DC74, D_800B0B00, or a name already in the symbol file).
Re-run `make split` afterwards (with asm/<level> removed) so the per-function .s files follow."""
import argparse
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("renames", nargs="+")
    ap.add_argument("--level", default="level01")
    args = ap.parse_args()

    sym_path = ROOT / f"config/symbol_addrs.{args.level}.txt"
    sym_text = sym_path.read_text()
    sources = list((ROOT / "src" / args.level).glob("*.c")) + list((ROOT / "include").glob("*.h"))
    texts = {p: p.read_text() for p in sources}

    for item in args.renames:
        old, new = item.split("=", 1)
        attrs = ""
        if ":" in new:
            new, typ = new.split(":", 1)
            attrs = f" type:{typ}"
        line_re = re.compile(rf"^{re.escape(old)} = (0x[0-9A-Fa-f]+);(.*)$", re.M)
        m = line_re.search(sym_text)
        if m:
            sym_text = line_re.sub(lambda mm: f"{new} = {mm.group(1)};{mm.group(2)}", sym_text)
        else:
            am = re.fullmatch(r"(?:func|D|jtbl)_([0-9A-Fa-f]{8})", old)
            if not am:
                raise SystemExit(f"{old}: not in {sym_path.name} and has no address in its name")
            kind = "func" if old.startswith("func_") else None
            attr = f" // type:{kind}" if kind else (f" //{attrs}" if attrs else "")
            sym_text = sym_text.rstrip("\n") + f"\n{new} = 0x{am.group(1).upper()};{attr}\n"
        word = re.compile(rf"\b{re.escape(old)}\b")
        for p in texts:
            texts[p] = word.sub(new, texts[p])
        print(f"{old} -> {new}")

    sym_path.write_text(sym_text)
    for p, t in texts.items():
        if p.read_text() != t:
            p.write_text(t)


if __name__ == "__main__":
    main()
