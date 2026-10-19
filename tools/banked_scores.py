"""Score every banked NON_MATCHING function: compile each level01 C file with -DNON_MATCHING and compare
against the EXE (bad words, relocations masked). Prints `bad/size func file`, closest first.
Run from the repository root: .venv/bin/python tools/banked_scores.py"""
import re, struct, subprocess, sys, tempfile
sys.path.insert(0, "tools")
import auto_match as am
from match_test import AS, RELOC_MASKS
from pathlib import Path
G = "tools/old-gcc/2.8.1-psx"
data = Path("workbench/extracted/L01/LEVEL01.EXE").read_bytes(); load = struct.unpack_from("<I", data, 0x18)[0]
addrs = {}
for line in subprocess.run(["mips-elf-nm", "build/level01/level01.elf"], capture_output=True, text=True).stdout.splitlines():
    p = line.split()
    if len(p) == 3 and p[1] in "Tt": addrs[p[2]] = int(p[0], 16)
sizes = {}
for s in Path("asm/level01/nonmatchings").glob("*/*.s"):
    m = re.search(r"nonmatching (\w+), 0x([0-9A-F]+)", s.read_text())
    if m: sizes[m.group(1)] = int(m.group(2), 16)
out = []
for c in sorted(Path("src/level01").glob("*.c")):
    text = c.read_text()
    banked = re.findall(r'#else\nINCLUDE_ASM\("[^"]+", (\w+)\);\n#endif', text)
    if not banked: continue
    with tempfile.TemporaryDirectory() as td:
        subprocess.run([f"{G}/cpp", "-P", "-undef", "-D__GNUC__=2", "-D_LANGUAGE_C", "-DNON_MATCHING", "-Iinclude", "-Isrc", str(c), f"{td}/u.i"], check=True)
        subprocess.run([f"{G}/cc1", "-quiet", "-G0", am.file_opt(c.stem) or "-O2", "-o", f"{td}/u.s", f"{td}/u.i"], check=True, capture_output=True)
        subprocess.run(f".venv/bin/python tools/jtbl_fix.py level01 < {td}/u.s | .venv/bin/python tools/maspsx/maspsx.py --aspsx-version=2.56 --expand-div > {td}/u2.s", shell=True, check=True)
        subprocess.run(AS + ["-Iinclude", "-o", f"{td}/u.o", f"{td}/u2.s"], check=True)
        subprocess.run(["mips-elf-objcopy", "-O", "binary", "-j", ".text", f"{td}/u.o", f"{td}/t.bin"], check=True)
        t = Path(f"{td}/t.bin").read_bytes()
        syms = {}
        for l in subprocess.run(["mips-elf-readelf", "-sW", f"{td}/u.o"], capture_output=True, text=True).stdout.splitlines():
            p = l.split()
            if len(p) >= 8 and p[3] == "FUNC": syms[p[7]] = (int(p[1], 16), int(p[2]))
        rel = {}
        for l in subprocess.run(["mips-elf-readelf", "-rW", f"{td}/u.o"], capture_output=True, text=True).stdout.splitlines():
            p = l.split()
            if len(p) >= 3 and p[2] in RELOC_MASKS and re.fullmatch(r"[0-9a-f]{8}", p[0]): rel[int(p[0], 16)] = RELOC_MASKS[p[2]]
    for f in banked:
        if f not in syms or f not in sizes: out.append((9999, 0, f, c.stem)); continue
        off, size = syms[f]; tsz = sizes[f]
        o0 = 0x800 + int(f[5:], 16) - load if f.startswith("func_") else None
        if o0 is None: out.append((9998, tsz // 4, f, c.stem)); continue
        bad = sum(1 for k in range(0, min(size, tsz), 4) if (struct.unpack_from("<I", data, o0 + k)[0] ^ struct.unpack_from("<I", t, off + k)[0]) & ~rel.get(off + k, 0) & 0xFFFFFFFF)
        bad += abs(size - tsz) // 4
        out.append((bad, tsz // 4, f, c.stem))
for bad, n, f, s in sorted(out):
    print(f"{bad:4d}/{n:<4d} {f} {s}")
