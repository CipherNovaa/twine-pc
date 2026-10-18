"""List or extract a level archive (LVLnnCD.BIN).

The archive starts with a table of 0x1C-byte entries, `char name[26]; u16 start_sector;`, ended by an
empty name. Sectors are 0x800 bytes; a file runs until the next entry's start sector (or the end of the
archive), so sizes are rounded up to whole sectors.

Usage: cdbin.py list LVL01CD.BIN
       cdbin.py extract LVL01CD.BIN OUT_DIR
       cdbin.py pack ORIGINAL.BIN DIR NEW.BIN   (same files and order as ORIGINAL, contents from DIR;
                                                  files may change size, the table is rewritten)"""
import struct
import sys
from pathlib import Path

SECTOR = 0x800
ENTRY = 0x1C


def read_toc(data):
    entries = []
    for off in range(0, len(data), ENTRY):
        name = data[off:off + 26].split(b"\0")[0].decode("ascii", "replace")
        if not name:
            break
        entries.append([name, struct.unpack_from("<H", data, off + 26)[0]])
    starts = sorted({s for _, s in entries}) + [len(data) // SECTOR]
    nxt = {s: starts[i + 1] for i, s in enumerate(starts[:-1])}
    return [(name, start * SECTOR, (nxt[start] - start) * SECTOR) for name, start in entries]


def main():
    cmd, path = sys.argv[1], Path(sys.argv[2])
    data = path.read_bytes()
    toc = read_toc(data)
    if cmd == "list":
        for name, off, size in toc:
            print(f"{off:08X} {size:8d}  {name}")
        print(f"{len(toc)} files")
    elif cmd == "extract":
        out = Path(sys.argv[3])
        for name, off, size in toc:
            dest = out / name
            dest.parent.mkdir(parents=True, exist_ok=True)
            dest.write_bytes(data[off:off + size])
        print(f"extracted {len(toc)} files to {out}")
    elif cmd == "pack":
        src, out = Path(sys.argv[3]), Path(sys.argv[4])
        names = [n for n, _, _ in toc]
        table_sectors = -(-len(names) * ENTRY // SECTOR) or 1
        first = min(o for _, o, _ in toc) // SECTOR
        assert first >= table_sectors, "table does not fit before the first file"
        body, sectors, placed = bytearray(), {}, {}
        for name, off, size in sorted(toc, key=lambda e: e[1]):
            if off in placed:  # several names can share one file
                sectors[name] = placed[off]
                continue
            blob = (src / name).read_bytes()
            blob += bytes(-len(blob) % SECTOR)
            placed[off] = sectors[name] = first + len(body) // SECTOR
            body += blob
        head = bytearray(data[:first * SECTOR])
        for i, name in enumerate(names):
            struct.pack_into("<H", head, i * ENTRY + 26, sectors[name])
        out.write_bytes(head + body)
        print(f"packed {len(names)} files into {out} ({len(head) + len(body)} bytes)")


if __name__ == "__main__":
    main()
