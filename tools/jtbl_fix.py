"""Point a C function's switch jump tables at the original tables in the level's data.

gcc puts each switch table in .rdata, which has no place in the original layout. The original tables are
already in the level's data (`dlabel jtbl_X`, entries `.word jtgt_Y`). This filter runs between cc1 and
maspsx: it drops gcc's tables, refers to jtbl_X instead, and defines the global jtgt_Y labels at gcc's case
labels. Tables pair up in order of first use with the jtbl_ symbols listed for the function in
config/<level>_jtbl.txt.
Right after a fresh split the entries are still raw addresses, which already point at the right code.

usage: jtbl_fix.py <level> < cc1.s > out.s"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def blob_tables(level):
    tables, cur = {}, None
    for f in sorted((ROOT / "asm" / level / "data").glob("*.s")):
        for line in f.read_text().splitlines():
            m = re.match(r"dlabel (jtbl_\w+)", line)
            if m:
                cur = tables.setdefault(m.group(1), [])
            elif cur is not None:
                m = re.search(r"\.word (\w+)", line)
                # A block can run on into other data (strings): the table is its leading code pointers.
                if m and (m.group(1).startswith("jtgt_") or re.fullmatch(r"0x800[2-9A-F][0-9A-F]{4}", m.group(1))):
                    cur.append(m.group(1))
                elif m or line.startswith("enddlabel"):
                    cur = None
    return tables


def target_tables(level, func):
    # A fresh split writes no asm for functions already in C, so the names live in config.
    for line in (ROOT / "config" / f"{level}_jtbl.txt").read_text().splitlines():
        if line.split()[:1] == [func]:
            return line.split()[1:]
    return []


def main():
    level = sys.argv[1]
    lines = sys.stdin.read().split("\n")
    if not any(l.startswith("\t.word\t$L") for l in lines):
        sys.stdout.write("\n".join(lines))
        return
    # Drop the jump tables (.rdata blocks holding only a label and `.word $Ln` entries), noting their owner.
    out, gcc_tables, func, i = [], {}, None, 0
    while i < len(lines):
        m = re.match(r"\t\.ent\t(\w+)", lines[i])
        if m:
            func = m.group(1)
        if lines[i] == "\t.rdata":
            j = i + 1
            while j < len(lines) and lines[j] != "\t.text":
                j += 1
            body = [l for l in lines[i + 1:j] if not l.startswith("\t.align")]
            if body and re.fullmatch(r"\$L\d+:", body[0]) and body[1:] and \
                    all(re.fullmatch(r"\t\.word\t\$L\d+", l) for l in body[1:]):
                gcc_tables[body[0][:-1]] = (func, [l.split("\t")[-1] for l in body[1:]])
                i = j + 1
                continue
        out.append(lines[i])
        i += 1

    # Uses in instructions: `lui %hi($Ln)` at -O2, `la $4,$Ln` at -O0.
    insns = "\n".join(l for l in out if not l.endswith(":") and not l.startswith("\t.word"))
    blob, rename, labels = blob_tables(level), {}, {}
    for f in dict.fromkeys(owner for owner, _ in gcc_tables.values()):
        used = [l for l in dict.fromkeys(re.findall(r"\$L\d+", insns)) if gcc_tables.get(l, (None,))[0] == f]
        targets = target_tables(level, f)
        if len(used) != len(targets):
            sys.exit(f"jtbl_fix: {f} has {len(used)} switch tables, the original uses {len(targets)}")
        for lab, jt in zip(used, targets):
            entries = gcc_tables[lab][1]
            if len(entries) > len(blob.get(jt, [])):
                sys.exit(f"jtbl_fix: {f}: {lab} has {len(entries)} entries, {jt} has {len(blob.get(jt, []))}")
            rename[lab] = jt
            for e, t in zip(entries, blob[jt]):
                if not t.startswith("jtgt_"):
                    continue  # raw addresses (fresh split, before make_shiftable): nothing to define
                if labels.setdefault(e, t) != t:
                    sys.exit(f"jtbl_fix: {f}: {e} would be both {labels[e]} and {t}")

    text = "\n".join(out)
    for lab, jt in rename.items():
        text = re.sub(re.escape(lab) + r"(?!\d)", jt, text)
    res = []
    for line in text.split("\n"):
        res.append(line)
        if line.endswith(":") and line[:-1] in labels:
            res += [f"\t.globl\t{labels[line[:-1]]}", f"{labels[line[:-1]]}:"]
    sys.stdout.write("\n".join(res))


if __name__ == "__main__":
    main()
