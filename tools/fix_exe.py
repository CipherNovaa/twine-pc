"""Pad a linked PS-X EXE to a whole number of CD sectors and write its text size into the header.
For an unmodified matching build this reproduces the original file exactly.
Usage: fix_exe.py FILE.exe"""
import struct
import sys
from pathlib import Path


def main():
    p = Path(sys.argv[1])
    d = bytearray(p.read_bytes())
    d += bytes(-len(d) % 0x800)
    struct.pack_into("<I", d, 0x1C, len(d) - 0x800)
    p.write_bytes(d)


if __name__ == "__main__":
    main()
