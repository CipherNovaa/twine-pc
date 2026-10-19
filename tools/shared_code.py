"""Find which LEVEL01 functions appear (modulo addresses) in the other executables."""
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
EXTRACTED = ROOT / "workbench/extracted"
MEMOPS = {0x08, 0x09, 0x0D, 0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2B, 0x32, 0x3A}  # addi(u)/ori/loads/stores/lwc2/swc2
SP, ZERO = 29, 0


def masked(word):
    op = word >> 26
    if op in (0x02, 0x03):
        return word & 0xFC000000
    if op == 0x0F:
        return word & 0xFFFF0000
    if op in MEMOPS and ((word >> 21) & 31) not in (SP, ZERO):
        return word & 0xFFFF0000
    return word


def load(path):
    data = path.read_bytes()
    load_addr, size = struct.unpack_from("<2I", data, 0x18)
    body = data[0x800:0x800 + size]
    words = [masked(w) for w in struct.unpack_from(f"<{len(body) // 4}I", body)]
    return load_addr, words


def level01_functions():
    return functions("level01")


def functions(level):
    """(address, size, name) of every function in the level's asm."""
    funcs = []
    for s in sorted((ROOT / "asm" / level).rglob("*.s")):
        txt = s.read_text()
        for name, size, body in re.findall(r"nonmatching (\w+), 0x([0-9A-F]+)\n\n\w+ \1\n(.*?)endlabel", txt, re.S):
            m = re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ", body)
            if m:
                funcs.append((int(m.group(1), 16), int(size, 16), name))
    return sorted(set(funcs))


def main():
    l1_addr, l1_words = load(EXTRACTED / "L01/LEVEL01.EXE")
    funcs = [f for f in level01_functions() if f[1] >= 16]
    lib_start = 0x80076FE4
    index = {}
    for addr, size, name in funcs:
        i = (addr - l1_addr) // 4
        seq = tuple(l1_words[i:i + size // 4])
        index.setdefault(seq[:4], []).append((seq, addr, size, name))

    targets = [EXTRACTED / f"L{n:02d}/LEVEL{n:02d}.EXE" for n in range(2, 12)] + [EXTRACTED / "SHDATA/SHELL.EXE"]
    found_in = {f[2]: set() for f in funcs}
    print(f"LEVEL01: {sum(1 for f in funcs if f[0] < lib_start)} game + {sum(1 for f in funcs if f[0] >= lib_start)} library functions (>=16 bytes)")
    for t in targets:
        _, words = load(t)
        hits = set()
        for i in range(len(words) - 4):
            for seq, addr, size, name in index.get(tuple(words[i:i + 4]), ()):
                if tuple(words[i:i + len(seq)]) == seq:
                    hits.add(name)
        game_hits = [f for f in funcs if f[2] in hits and f[0] < lib_start]
        lib_hits = [f for f in funcs if f[2] in hits and f[0] >= lib_start]
        for n in hits:
            found_in[n].add(t.stem)
        print(f"  {t.stem:8s} game {len(game_hits):4d} ({sum(f[1] for f in game_hits) // 1024:3d} KB)   lib {len(lib_hits):3d}   code size {len(words) * 4 // 1024} KB")

    game = [f for f in funcs if f[0] < lib_start]
    levels = {t.stem for t in targets if t.stem.startswith("LEVEL")}
    in_all = [f for f in game if levels <= found_in[f[2]]]
    in_some = [f for f in game if found_in[f[2]] & levels and not levels <= found_in[f[2]]]
    only = [f for f in game if not found_in[f[2]] & levels]
    in_shell = [f for f in game if "SHELL" in found_in[f[2]]]
    print(f"\nLEVEL01 game functions: in ALL other levels {len(in_all)} ({sum(f[1] for f in in_all) // 1024} KB), "
          f"in some {len(in_some)} ({sum(f[1] for f in in_some) // 1024} KB), "
          f"LEVEL01-only {len(only)} ({sum(f[1] for f in only) // 1024} KB); also in SHELL: {len(in_shell)}")
    if "--list-only" in sys.argv:
        for a, s, n in only:
            print(f"    only: {n} @ {a:08X} size 0x{s:X}")


if __name__ == "__main__":
    main()
