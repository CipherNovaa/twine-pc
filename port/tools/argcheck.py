"""Find calls in the game's C that pass fewer arguments than the callee reads.

The matching build can't show these: when the original code left the value in the argument register
already, the C without that argument compiles to the same bytes. Natively the callee reads garbage.

  argcheck.py LEVEL     (callee parameters = argument registers its asm reads before writing them)
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
level = sys.argv[1] if len(sys.argv) > 1 else "level01"

INSN = re.compile(r"/\* \w+ \w{8} \w{8} \*/\s+(\S+)\s*(.*)")
A_REGS = ["$a0", "$a1", "$a2", "$a3"]


# BIOS functions behind the libc thunks (`addiu $t2, $zero, 0xA0; jr $t2; addiu $t1, $zero, n`): arguments
BIOS_ARGS = {(0xA0, 0x0D): 3, (0xA0, 0x13): 1, (0xA0, 0x15): 2, (0xA0, 0x17): 2, (0xA0, 0x18): 3, (0xA0, 0x19): 2,
             (0xA0, 0x1A): 3, (0xA0, 0x1B): 1, (0xA0, 0x1E): 2, (0xA0, 0x1F): 2, (0xA0, 0x24): 2, (0xA0, 0x25): 1,
             (0xA0, 0x26): 1, (0xA0, 0x28): 2, (0xA0, 0x2A): 3, (0xA0, 0x2B): 3, (0xA0, 0x2E): 3, (0xA0, 0x30): 1,
             (0xA0, 0x3F): 1, (0xA0, 0x49): 1, (0xB0, 0x08): 4, (0xB0, 0x09): 1, (0xB0, 0x0B): 1, (0xB0, 0x0C): 1,
             (0xB0, 0x0D): 1, (0xB0, 0x38): 1, (0xB0, 0x3F): 1}


def params(lines):
    """number of argument registers read before written (first basic block and beyond, linear scan)"""
    text = " ".join(lines)
    thunk = re.search(r"addiu\s+\$t2, \$zero, (0x[AB]0).*jr\s+\$t2.*addiu\s+\$t1, \$zero, (0x[0-9A-F]+)", text)
    if thunk:
        return BIOS_ARGS.get((int(thunk.group(1), 16), int(thunk.group(2), 16)), 0)
    written, read = set(), set()
    for line in lines:
        m = INSN.search(line)
        if not m:
            continue
        op, args = m.group(1), [a.strip() for a in m.group(2).split(",")]
        regs = re.findall(r"\$a[0-3]", m.group(2))
        if not regs:
            continue
        dst = args[0] if args and args[0] in A_REGS and op not in ("sw", "sh", "sb", "swl", "swr", "beq", "bne",
                                                                    "beqz", "bnez", "bgez", "bltz", "blez", "bgtz",
                                                                    "jr", "jalr", "mtc2", "ctc2", "mult", "multu",
                                                                    "div", "divu", "swc2") else None
        srcs = [r for r in regs if r != dst] if dst else regs
        if dst and args.count(dst) > 1:
            srcs.append(dst)
        for r in srcs:
            if r not in written:
                read.add(r)
        if dst:
            written.add(dst)
        if op in ("jal", "jalr"):
            break
    return max((A_REGS.index(r) + 1 for r in read), default=0)


callee = {}
for path in list((ROOT / f"asm/{level}/nonmatchings").glob("*/*.s")) + [ROOT / f"asm/{level}/psyq_libs.s"]:
    cur, body = None, []
    for line in path.read_text().splitlines():
        m = re.match(r"glabel (\w+)", line)
        if m:
            if cur:
                callee[cur] = params(body)
            cur, body = m.group(1), []
        elif cur:
            body.append(line)
    if cur:
        callee[cur] = params(body)

# other levels' names bound to this level's functions (L1_X = Y, L2_X = Y) read what Y reads
cfg = ROOT / f"config/{level}_syms.ld"
if cfg.exists():
    for x, y in re.findall(r"^(?:PROVIDE\()?(L\d+_\w+) = (\w+)\)?;", cfg.read_text(), re.M):
        if y in callee:
            callee.setdefault(x, callee[y])

CALL = re.compile(r"\b(\w+)\s*\(")
for path in sorted((ROOT / f"src/{level}").glob("*.c")):
    text = path.read_text()
    for lineno, line in enumerate(text.splitlines(), 1):
        s = line.strip()
        # skip declarations (a type before the name: `s32 f(s32);`), not call statements (`f(a);`)
        if s.startswith(("#", "//", "/*")) or re.match(r"^(?:[A-Za-z_]\w*[\s\*]+)+\w+\s*\([^;]*\)\s*[;{]\s*(/\*.*)?$", s) \
                and not re.search(r"=|\breturn\b|\bif\b|\bwhile\b|\belse\b|\bdo\b|\bcase\b", s):
            continue
        for m in CALL.finditer(line):
            name = m.group(1)
            if name not in callee or not callee[name]:
                continue
            # count top-level arguments
            depth, i, args, cur = 1, m.end(), 0, ""
            while i < len(line) and depth:
                ch = line[i]
                if ch == "(":
                    depth += 1
                elif ch == ")":
                    depth -= 1
                elif ch == "," and depth == 1:
                    args += 1
                if depth:
                    cur += ch
                i += 1
            if depth:
                continue
            n = 0 if not cur.strip() or cur.strip() == "void" else args + 1
            if n < callee[name]:
                print(f"{path.name}:{lineno}: {name} called with {n} args, reads {callee[name]}: {s[:110]}")
