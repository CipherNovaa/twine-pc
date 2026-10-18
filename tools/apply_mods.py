"""Apply word patches (mods/*.patch) to copies of the game executables, verifying original bytes.

Patch line format:  <DISC FILE NAME>  <vaddr>  <original word>  <new word>  # comment
LEVEL01.EXE is taken from our matching build; everything else from the extracted disc."""
import argparse
import sys
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BUILT = {"LEVEL01.EXE": ROOT / "build/level01/level01.exe"}


def source_for(name):
    if name in BUILT:
        return BUILT[name]
    hits = [p for p in (ROOT / "workbench/extracted").rglob(name) if p.is_file()]
    if len(hits) != 1:
        raise SystemExit(f"{name}: expected one extracted file, found {len(hits)}")
    return hits[0]


LEVELS = [f"LEVEL{n:02d}.EXE" for n in range(1, 12)]
WINDOW = 12


def fuzzy(word):
    """Wildcard pointer-like words; mask address fields of jumps, lui and memory ops."""
    if 0x80000000 <= word < 0x80200000:
        return None
    op = word >> 26
    if op in (0x02, 0x03):
        return word & 0xFC000000
    if op == 0x0F or (op in (0x08, 0x09, 0x0D, 0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2B) and (word >> 21) & 31 not in (0, 29)):
        return word & 0xFFFF0000
    return word


def words_of(img):
    size = struct.unpack_from("<I", img, 0x1C)[0]
    return list(struct.unpack_from(f"<{size // 4}I", img, 0x800))


def find_equivalent(src_img, vaddr, dst_img):
    """Locate the word in dst_img corresponding to vaddr in src_img via a fuzzy context window."""
    src_load = struct.unpack_from("<I", src_img, 0x18)[0]
    dst_load = struct.unpack_from("<I", dst_img, 0x18)[0]
    src, dst = words_of(src_img), words_of(dst_img)
    i = (vaddr - src_load) // 4
    pattern = [fuzzy(w) for w in src[max(0, i - WINDOW):i + WINDOW + 1]]
    anchor = i - max(0, i - WINDOW)
    fdst = [fuzzy(w) for w in dst]
    hits = []
    for j in range(len(dst) - len(pattern) + 1):
        if all(p is None or p == fdst[j + k] for k, p in enumerate(pattern)):
            hits.append(j + anchor)
            if len(hits) > 1:
                return None
    return dst_load + hits[0] * 4 if hits else None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("patches", nargs="+")
    ap.add_argument("--out-dir", default=str(ROOT / "build/mod"))
    ap.add_argument("--all-levels", action="store_true", help="also apply LEVEL01 patches to LEVEL02-11 by context match")
    ap.add_argument("--base-original", action="store_true",
                    help="patch the extracted retail LEVEL01.EXE instead of our build (identical bytes while matching)")
    args = ap.parse_args()
    if args.base_original:
        BUILT.clear()

    level01_orig = source_for("LEVEL01.EXE").read_bytes()
    images = {}
    for patch in args.patches:
        for lineno, raw in enumerate(Path(patch).read_text().splitlines(), 1):
            line = raw.split("#", 1)[0].strip()
            if not line:
                continue
            name, vaddr, old, new = line.split()
            vaddr, old, new = int(vaddr, 16), int(old, 16), int(new, 16)
            targets = [(name, vaddr)]
            if args.all_levels and name == "LEVEL01.EXE":
                for other in LEVELS[1:]:
                    dst = source_for(other).read_bytes()
                    va = find_equivalent(level01_orig, vaddr, dst)
                    if va is None:
                        print(f"warning: {patch}:{lineno}: no unique match in {other}, skipped", file=sys.stderr)
                    else:
                        targets.append((other, va))
            for tname, tva in targets:
                if tname not in images:
                    images[tname] = bytearray(source_for(tname).read_bytes())
                img = images[tname]
                off = 0x800 + tva - struct.unpack_from("<I", img, 0x18)[0]
                cur = struct.unpack_from("<I", img, off)[0]
                if cur != old:
                    raise SystemExit(f"{patch}:{lineno}: {tname} @ {tva:08X} is {cur:08X}, expected {old:08X}")
                struct.pack_into("<I", img, off, new)

    out = Path(args.out_dir)
    out.mkdir(parents=True, exist_ok=True)
    for name, img in images.items():
        (out / name).write_bytes(img)
        print(f"{name}={out / name}")


if __name__ == "__main__":
    main()
