"""Build one level with C mods from src/mods/ (and -DMOD_<NAME> for the level's own C files).

Mods are written with LEVEL01 names. For another level, tools/port_symbols.py maps those names: functions
a mod replaces (its `MOD_REPLACES:` line) and the globals it uses are renamed with -D when compiling the mod.
The level's original definition of each replaced function is made weak, so the mod's version wins at link
time, and gets an alias `<name>_orig` the mod can call.

Usage: build_cmod.py LEVEL "MOD1 MOD2 ..."   -> build/<LEVEL>_cmod/<LEVEL>.exe"""
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
PY = str(ROOT / ".venv/bin/python")
GCC = ROOT / "tools/old-gcc/2.8.1-psx"
CPPFLAGS = ["-P", "-undef", "-D__GNUC__=2", "-D_LANGUAGE_C", "-Iinclude", "-Isrc"]
ASFLAGS = ["-EL", "-march=r3000", "-mtune=r3000", "-no-pad-sections", "-G0", "-Iinclude"]
IDENT = re.compile(r"\b[A-Za-z_]\w*\b")


def run(cmd, **kw):
    r = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, **kw)
    if r.returncode:
        sys.exit(f"failed: {' '.join(map(str, cmd))}\n{r.stdout}{r.stderr}")
    return r.stdout


def main():
    level, mods = sys.argv[1], sys.argv[2].split()
    build = ROOT / f"build/{level}_cmod"
    sources = [p for m in mods for p in [ROOT / f"src/mods/{m.lower()}.c"] if p.exists()]
    # Objects get -DMOD_* flags and weakened symbols, so start over whenever the set of mods changes.
    stamp = build / "cmods.txt"
    if not stamp.exists() or stamp.read_text() != " ".join(sorted(mods)):
        subprocess.run(["rm", "-rf", str(build)])
        build.mkdir(parents=True)
        stamp.write_text(" ".join(sorted(mods)))
    run(["make", f"LEVEL={level}", f"CMODS={' '.join(mods)}", "-j8", "cmod-objs"])

    replaces = []
    for src in sources:
        m = re.search(r"MOD_REPLACES:([^\n*]*)", src.read_text())
        replaces += m.group(1).split() if m else []
    dupes = {f for f in replaces if replaces.count(f) > 1}
    if dupes:
        sys.exit(f"two mods replace the same function: {' '.join(sorted(dupes))}")
    used = set()
    for src in sources:
        used |= set(IDENT.findall(src.read_text()))
    used |= {f"{f}_orig" for f in replaces}

    # LEVEL01 name -> name in this level
    defines, aliases, mapping = [], [], {}
    if level != "level01":
        out = run([PY, "tools/port_symbols.py", level, *sorted(used | set(replaces))])
        for line in out.splitlines():
            m = re.match(r"(\w+) = (\w+)( [+-] 0x[0-9A-F]+)?;", line)
            if m:
                mapping[m.group(1)] = (m.group(2), m.group(3) or "")
        for f in replaces:
            if f not in mapping or mapping[f][1]:
                sys.exit(f"{level}: no clean match for replaced function {f}")
        for name, (target, off) in sorted(mapping.items()):
            if off:  # an offset can't be a C name: go through a unique linker alias
                aliases.append(f"cmod_{name} = {target}{off};")
                defines.append(f"-D{name}=cmod_{name}")
            else:
                defines.append(f"-D{name}={target}")
        for f in replaces:
            defines.append(f"-D{f}_orig={mapping[f][0]}_orig")

    # Compile the mods.
    mod_objs = []
    for src in sources:
        base = build / "src/mods" / src.stem
        base.parent.mkdir(parents=True, exist_ok=True)
        run([str(GCC / "cpp"), *CPPFLAGS, *defines, str(src), "-o", f"{base}.i"])
        run([str(GCC / "cc1"), "-quiet", "-G0", "-O2", "-o", f"{base}.cc1.s", f"{base}.i"])
        s = run([PY, "tools/maspsx/maspsx.py", "--aspsx-version=2.56", "--expand-div"],
                input=Path(f"{base}.cc1.s").read_text())
        Path(f"{base}.s").write_text(s)
        run(["mips-elf-as", *ASFLAGS, "-o", f"{base}.o", f"{base}.s"])
        mod_objs.append(f"{base}.o")

    # Weaken the originals the mods replace, keeping them reachable as <name>_orig.
    objs = sorted(build.rglob("*.o"))
    objs = [o for o in objs if "src/mods" not in str(o)]
    for f in replaces:
        sym = mapping[f][0] if level != "level01" else f
        for o in objs:
            nm = run(["mips-elf-nm", str(o)])
            m = re.search(rf"^([0-9a-f]+) ([TtWw]) {re.escape(sym)}$", nm, re.M)
            if not m:
                continue
            if re.search(rf" {re.escape(sym)}_orig$", nm, re.M):
                break  # already done on an earlier run
            run(["mips-elf-objcopy", f"--weaken-symbol={sym}",
                 f"--add-symbol={sym}_orig=.text:0x{m.group(1)},global,function", str(o)])
            break
        else:
            sys.exit(f"{level}: {sym} ({f}) is not defined in any object")

    # Linker script: the splat one, pointed at this build directory, with the mod objects added.
    ld = (ROOT / f"linkers/{level}.ld").read_text().replace(f"build/{level}/", f"build/{level}_cmod/")
    lines = ld.splitlines()
    for sect in (".rodata", ".text", ".data", ".bss"):
        last = max(i for i, l in enumerate(lines) if l.strip().endswith(f"({sect});") and f"build/{level}_cmod/" in l)
        for o in reversed(mod_objs):
            lines.insert(last + 1, f"        {Path(o).relative_to(ROOT)}({sect});")
    (build / f"{level}.ld").write_text("\n".join(lines) + "\n")
    (build / "cmod_aliases.txt").write_text("\n".join(aliases) + "\n")

    elf, exe = build / f"{level}.elf", build / f"{level}.exe"
    extra = ["-T", f"config/{level}_syms.ld"] if (ROOT / f"config/{level}_syms.ld").exists() else []
    run(["mips-elf-ld", "-EL", "--no-check-sections", "-T", str(build / f"{level}.ld"),
         "-T", f"linkers/{level}_undefined_syms_auto.txt", "-T", f"linkers/{level}_undefined_funcs_auto.txt",
         "-T", str(build / "cmod_aliases.txt"), *extra, "-Map", str(build / f"{level}.map"), "-o", str(elf)])
    run(["mips-elf-objcopy", "-O", "binary", str(elf), str(exe)])
    run([PY, "tools/fix_exe.py", str(exe)])
    print(f"{level}: {exe.relative_to(ROOT)} ({exe.stat().st_size} bytes; mods: {' '.join(mods)})")


if __name__ == "__main__":
    main()
