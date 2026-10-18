"""Compile candidate C for single functions with several old-gcc/maspsx setups and
compare against the original binary, masking linker-relocated fields."""
import argparse
import itertools
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
AS = ["mips-elf-as", "-EL", "-march=r3000", "-mtune=r3000", "-no-pad-sections", "-G0"]
RELOC_MASKS = {"R_MIPS_26": 0x03FFFFFF, "R_MIPS_HI16": 0xFFFF, "R_MIPS_LO16": 0xFFFF}


def load_exe(path):
    data = Path(path).read_bytes()
    load_addr = struct.unpack_from("<I", data, 0x18)[0]
    return data, load_addr


def func_sizes(asm_path):
    """{name: size} for every function in the given .s file or directory tree."""
    files = [asm_path] if asm_path.is_file() else asm_path.rglob("*.s")
    out = {}
    for f in files:
        txt = f.read_text()
        for m in re.finditer(r"nonmatching (\w+), 0x([0-9A-F]+)\n\n\w+ \1\n(.*?)endlabel", txt, re.S):
            vm = re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ", m.group(3))
            out[m.group(1)] = (int(m.group(2), 16), int(vm.group(1), 16) if vm else None)
    return out


def compile_c(c_file, gcc, opt, aspsx, workdir):
    s1 = workdir / "out.s"
    s2 = workdir / "out.ms.s"
    obj = workdir / "out.o"
    cc1 = ROOT / "tools/old-gcc" / gcc / "cc1"
    pre = workdir / "out.i"
    r = subprocess.run([str(ROOT / "tools/old-gcc" / gcc / "cpp"), "-P", "-undef", "-D__GNUC__=2", "-D_LANGUAGE_C",
                        f"-I{ROOT}/include", str(c_file), str(pre)], capture_output=True, text=True)
    if r.returncode:
        return None, "cpp: " + r.stderr.strip()[-200:]
    r = subprocess.run([str(cc1), "-quiet", opt, "-G0", "-o", str(s1), str(pre)],
                       capture_output=True, text=True)
    if r.returncode:
        return None, r.stderr.strip().splitlines()[-1] if r.stderr else "cc1 failed"
    ms = subprocess.run([sys.executable, str(ROOT / "tools/maspsx/maspsx.py"), f"--aspsx-version={aspsx}",
                         "--expand-div"], stdin=s1.open(), capture_output=True, text=True)
    if ms.returncode:
        return None, "maspsx: " + ms.stderr.strip()[-200:]
    s2.write_text(ms.stdout)
    r = subprocess.run(AS + [f"-I{ROOT}/include", "-o", str(obj), str(s2)], capture_output=True, text=True, cwd=ROOT)
    if r.returncode:
        return None, "as: " + r.stderr.strip()[-200:]
    text = workdir / "text.bin"
    subprocess.run(["mips-elf-objcopy", "-O", "binary", "-j", ".text", str(obj), str(text)], check=True)
    relocs = {}
    for line in subprocess.run(["mips-elf-readelf", "-rW", str(obj)], capture_output=True, text=True).stdout.splitlines():
        parts = line.split()
        if len(parts) >= 3 and parts[2] in RELOC_MASKS:
            relocs[int(parts[0], 16)] = RELOC_MASKS[parts[2]]
    return (text.read_bytes(), relocs), None


def compare(orig, built, relocs):
    """Returns number of mismatching instruction words (0 = match)."""
    if len(built) < len(orig) or any(built[len(orig):]):
        return max(len(orig), len(built.rstrip(b"\0"))) // 4
    bad = 0
    for off in range(0, len(orig), 4):
        mask = ~relocs.get(off, 0) & 0xFFFFFFFF
        a = struct.unpack_from("<I", orig, off)[0]
        b = struct.unpack_from("<I", built, off)[0]
        if (a & mask) != (b & mask):
            bad += 1
    return bad


def show_diff(args, data, load_addr, sizes):
    from capstone import CS_ARCH_MIPS, CS_MODE_LITTLE_ENDIAN, CS_MODE_MIPS32, Cs
    md = Cs(CS_ARCH_MIPS, CS_MODE_MIPS32 + CS_MODE_LITTLE_ENDIAN)
    combo = (args.gcc[0], args.opt[0], args.aspsx[0])
    for c_file in map(Path, args.c_files):
        name = c_file.stem
        size, addr = sizes[name]
        off = 0x800 + addr - load_addr
        orig = data[off:off + size]
        with tempfile.TemporaryDirectory() as td:
            res, err = compile_c(c_file, *combo, Path(td))
        if not res:
            print(name, "ERROR", err)
            continue
        built, relocs = res
        print(f"==== {name}  {' '.join(combo)}   (target | ours)")
        def dis(b, i):
            if i + 4 > len(b):
                return ""
            insn = next(md.disasm(b[i:i + 4], 0), None)
            return f"{insn.mnemonic} {insn.op_str}" if insn else b[i:i + 4].hex()
        for i in range(0, max(len(orig), len(built.rstrip(b'\0'))), 4):
            a, b = dis(orig, i), dis(built, i)
            mask = ~relocs.get(i, 0) & 0xFFFFFFFF
            same = i + 4 <= min(len(orig), len(built)) and \
                (struct.unpack_from("<I", orig, i)[0] & mask) == (struct.unpack_from("<I", built, i)[0] & mask)
            print(f"{'  ' if same else '!!'} {i:4x}  {a:34s} | {b}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("c_files", nargs="+")
    ap.add_argument("--exe", default=ROOT / "workbench/extracted/L01/LEVEL01.EXE")
    ap.add_argument("--asm", default=ROOT / "asm/level01", type=Path)
    ap.add_argument("--gcc", nargs="+", default=sorted(p.name for p in (ROOT / "tools/old-gcc").iterdir()))
    ap.add_argument("--opt", nargs="+", default=["-O2", "-O1"])
    ap.add_argument("--aspsx", nargs="+", default=["2.34", "2.56", "2.67", "2.79"])
    ap.add_argument("--diff", action="store_true", help="show disassembly diff for the first combo")
    args = ap.parse_args()

    data, load_addr = load_exe(args.exe)
    sizes = func_sizes(args.asm)
    if args.diff:
        show_diff(args, data, load_addr, sizes)
        return
    combos = list(itertools.product(args.gcc, args.opt, args.aspsx))
    scores = {c: 0 for c in combos}
    per_func = {}
    with tempfile.TemporaryDirectory() as td:
        for c_file in map(Path, args.c_files):
            name = c_file.stem
            size, addr = sizes[name]
            off = 0x800 + addr - load_addr
            orig = data[off:off + size]
            for combo in combos:
                res, err = compile_c(c_file, *combo, Path(td))
                bad = compare(orig, *res) if res else 999
                per_func[(name, combo)] = bad if res else err
                scores[combo] += 0 if bad == 0 else 1
    names = [Path(c).stem for c in args.c_files]
    print(f"{'compiler':14s} {'opt':4s} {'aspsx':6s} " + " ".join(f"{n[-8:]:>9s}" for n in names) + "  matched")
    for combo in sorted(combos, key=lambda c: scores[c]):
        cells = []
        for n in names:
            v = per_func[(n, combo)]
            cells.append(f"{'OK' if v == 0 else (str(v) + ' bad' if isinstance(v, int) else 'ERR'):>9s}")
        print(f"{combo[0]:14s} {combo[1]:4s} {combo[2]:6s} " + " ".join(cells) + f"  {len(names) - scores[combo]}/{len(names)}")
    errs = {v for v in per_func.values() if isinstance(v, str)}
    for e in sorted(errs)[:5]:
        print("error:", e)


if __name__ == "__main__":
    main()
