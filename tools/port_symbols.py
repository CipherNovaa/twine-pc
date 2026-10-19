"""Map LEVEL01 symbol names onto another level that shares the engine code.

Functions are matched by their instruction words with addresses masked out (as in shared_code.py). For each
matched pair the two levels' asm is walked side by side, and the symbols in %hi()/%lo() operands and jal/j
targets are paired up. A LEVEL01 name gets a mapping when all its pairings agree.

Usage: port_symbols.py LEVEL [NAME ...]   prints `NAME = <symbol in LEVEL> [+ offset];` lines
       (with no names, every LEVEL01 symbol that could be mapped is printed)"""
import re
import struct
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import shared_code  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
LINE_RE = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/\s+\w+\s*(.*)$")
REF_RE = re.compile(r"%(?:hi|lo)\((\w+)(?: \+ (0x[0-9A-F]+))?\)|^(\w+)$")
CALLS = re.compile(r"^\s*/\* [0-9A-F]+ [0-9A-F]{8} [0-9A-F]{8} \*/\s+(?:jal|j)\s+(\w+)\s*$")


def asm_lines(level):
    """vram -> operand text, and vram -> function name for each function start."""
    ops, starts = {}, {}
    for s in (ROOT / "asm" / level).rglob("*.s"):
        if "/data/" in str(s):
            continue
        pending = None
        for line in s.read_text().splitlines():
            g = re.match(r"glabel (\w+)", line)
            if g:
                pending = g.group(1)
                continue
            m = LINE_RE.match(line)
            if m:
                a = int(m.group(1), 16)
                ops[a] = line
                if pending:
                    starts[a] = pending
                    pending = None
    return ops, starts


def refs(line):
    """Symbol references in one instruction, as (name, offset) in operand order."""
    out = []
    c = CALLS.match(line)
    if c:
        return [(c.group(1), 0)]
    for m in re.finditer(r"%(?:hi|lo)\((\w+)(?: \+ (0x[0-9A-F]+))?\)", line):
        out.append((m.group(1), int(m.group(2) or "0", 16)))
    return out


def pairings(level, src="level01"):
    """SRC function name -> LEVEL address of its only copy (a function start there), and for every
    SRC symbol the pairings seen across all shared functions, {(LEVEL symbol, offset delta): count}."""
    n, s = int(level[-2:]), int(src[-2:])
    l1_addr, l1_words = shared_code.load(shared_code.EXTRACTED / f"L{s:02d}/LEVEL{s:02d}.EXE")
    t_addr, t_words = shared_code.load(shared_code.EXTRACTED / f"L{n:02d}/LEVEL{n:02d}.EXE")
    funcs = [f for f in shared_code.functions(src) if f[1] >= 16]
    index = defaultdict(list)
    for i in range(len(t_words) - 4):
        index[tuple(t_words[i:i + 4])].append(i)

    l1_ops, _ = asm_lines(src)
    t_ops, t_starts = asm_lines(level)
    pairs, votes = {}, defaultdict(Counter)
    for addr, size, name in funcs:
        i = (addr - l1_addr) // 4
        seq = l1_words[i:i + size // 4]
        hits = [j for j in index.get(tuple(seq[:4]), []) if t_words[j:j + len(seq)] == seq]
        if len(hits) != 1:
            continue
        ta = t_addr + hits[0] * 4
        if ta in t_starts:
            pairs[name] = ta
            votes[name][(t_starts[ta], 0)] += 1
        for k in range(size // 4):
            a, b = refs(l1_ops.get(addr + 4 * k, "")), refs(t_ops.get(ta + 4 * k, ""))
            if len(a) != len(b):
                continue
            for (n1, o1), (n2, o2) in zip(a, b):
                votes[n1][(n2, o2 - o1)] += 1
    return pairs, votes


def symbol_map(votes):
    """LEVEL01 name -> (LEVEL symbol, offset delta), for names whose pairings all agree."""
    return {name: next(iter(v)) for name, v in votes.items() if len(v) == 1}


def main():
    level = sys.argv[1]
    wanted = set(sys.argv[2:])
    _, votes = pairings(level)
    for name, (target, delta) in sorted(symbol_map(votes).items()):
        if wanted and name not in wanted:
            continue
        if target == name:
            continue
        print(f"{name} = {target}{f' + 0x{delta:X}' if delta > 0 else f' - 0x{-delta:X}' if delta < 0 else ''};")
    missing = wanted - set(votes)
    for name in sorted(missing):
        print(f"/* {name}: not found in {level} */")


if __name__ == "__main__":
    main()
