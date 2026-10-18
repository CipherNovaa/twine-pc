"""Decompilation progress for a level: game functions and bytes in C vs still INCLUDE_ASM.
Function boundaries come from the linker map, so run `make` first."""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
GAME_CODE = {"level01": (0x80026800, 0x80076FE4)}


def main():
    level = sys.argv[1] if len(sys.argv) > 1 else "level01"
    lo, hi = GAME_CODE[level]
    syms = {}
    for line in (ROOT / f"build/{level}/{level}.map").read_text().splitlines():
        m = re.match(r"^\s+0x([0-9a-f]{8,16})\s+([A-Za-z_]\w*)\s*$", line)
        if m:
            addr = int(m.group(1), 16) & 0xFFFFFFFF
            if lo <= addr < hi:
                syms.setdefault(addr, m.group(2))
    addrs = sorted(syms)
    size = {syms[a]: (addrs[i + 1] if i + 1 < len(addrs) else hi) - a for i, a in enumerate(addrs)}

    tot_f = tot_b = c_f = c_b = 0
    for c in sorted((ROOT / "src" / level).glob("*.c")):
        text = c.read_text()
        asm = set(re.findall(r'^INCLUDE_ASM\("[^"]+", (\w+)\);', text, re.M))
        defined = set(re.findall(r"^[A-Za-z_][\w \*]*?\b(\w+)\([^;{]*\)\s*\{?\s*$", text, re.M)) & set(size)
        names = asm | defined
        b_all = sum(size.get(n, 0) for n in names)
        b_done = sum(size.get(n, 0) for n in defined)
        tot_f += len(names); c_f += len(defined); tot_b += b_all; c_b += b_done
        print(f"{c.name:18s} {len(defined):4d}/{len(names):4d} functions  {100 * b_done / max(b_all, 1):5.1f}% of bytes")
    print(f"{'total':18s} {c_f:4d}/{tot_f:4d} functions ({100 * c_f / tot_f:.1f}%), "
          f"{c_b // 1024}/{tot_b // 1024} KB ({100 * c_b / tot_b:.1f}% of game-code bytes) in C")


if __name__ == "__main__":
    main()
