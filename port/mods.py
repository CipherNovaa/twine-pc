"""C mods in the native build (port/Makefile PORT_MODS): the PS1 build's C mods (src/mods/, tools/build_cmod.py)
compiled into a level's module, plus native-only mods (port/mods/).

A mod is written with LEVEL01 names and replaces game functions (its `MOD_REPLACES:` line, and
`MOD_REPLACES_NATIVE:` for what only the native build replaces). For the level, its names are mapped like the
PS1 build does; the object defining each replaced function gets a copy with that definition weakened and kept
reachable as `<name>_orig`, so the mod's version wins in the module's partial link. A replaced function that
has no C (recompiled) is listed for port/recomp.py, which exports the original as `<name>_orig` and sends
calls to it to the mod.

port/fixes/<module>.c is built into that module whatever the mods: the runtime's changes to the executable's own
code (named as in the module), replacing functions the same way.

Usage: mods.py MODULE OUTDIR "MOD ..." "CC CFLAGS" GAME_OBJ...
  -> OUTDIR/mods/: the mods' objects, the weakened copies, objs.txt (the module's game objects to link, the
     mods' included), mods.ld (linker aliases), recompiled.txt (replaced functions recomp.py makes) and
     replaced_c.txt (replaced functions that have C, which recomp.py then never makes)"""
import re
import shlex
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import build_cmod  # noqa: E402


def run(cmd):
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode:
        sys.exit(f"failed: {' '.join(map(str, cmd))}\n{r.stdout}{r.stderr}")
    return r.stdout


def source(mod):
    for d in ("port/mods", "src/mods"):
        p = ROOT / d / f"{mod.lower()}.c"
        if p.exists():
            return p
    sys.exit(f"no mod {mod} (port/mods/, src/mods/)")


def rename_definitions(module, obj, copy, syms, cc):
    """(Windows) obj made again from its C as copy, the definitions of syms renamed <sym>_orig, every use left"""
    asm, c = copy.with_suffix(".s"), copy.with_suffix(".c")
    # (its C as port/Makefile compiles it: src/<module>/<name>.c less the MIPS GTE macros)
    src = (ROOT / "src" / module / obj.name).with_suffix(".c")
    c.write_text("".join(l for l in src.read_text().splitlines(True)
                         if not re.match(r"#define gte_[A-Za-z0-9_]*\(.*__asm__", l)))
    run([*shlex.split(cc), "-S", str(c), "-o", str(asm)])
    c.unlink()
    text = asm.read_text()
    for sym in syms:
        s = re.escape(sym)
        text = re.sub(rf"^{s}:", f"{sym}_orig:", text, flags=re.M)
        text = re.sub(rf"^(\t\.globl\t){s}$", rf"\g<1>{sym}_orig", text, flags=re.M)
        text = re.sub(rf"^(\t\.def\t){s};", rf"\g<1>{sym}_orig;", text, flags=re.M)
    asm.write_text(text)
    run([*shlex.split(cc), "-c", str(asm), "-o", str(copy)])
    asm.unlink()


def main():
    module, outdir, mods, cc, objs = sys.argv[1], Path(sys.argv[2]), sys.argv[3].split(), sys.argv[4], sys.argv[5:]
    out = outdir / "mods"
    out.mkdir(parents=True, exist_ok=True)
    for old in out.glob("*.o"):
        old.unlink()
    if not module.startswith("level"):  # the mods change gameplay code: levels only
        mods = []
    sources = [source(m) for m in mods]
    replaces = build_cmod.replaced_functions(sources, native=True)
    # port/mod_names.txt: names the symbol matching can't place
    known = {n: t for m, n, t in (l.split() for l in (ROOT / "port/mod_names.txt").read_text().splitlines()
                                  if l.strip() and not l.startswith("#")) if m == module}
    defines, aliases, defined_as = build_cmod.level_names(module, sources, replaces, known) if sources else ([], [], {})
    # port/fixes/<module>.c: what the runtime changes in this executable (always built in, with its own names)
    fix = ROOT / "port/fixes" / f"{module}.c"
    fixes = build_cmod.replaced_functions([fix], native=True) if fix.exists() else []
    clash = set(fixes) & set(defined_as.values())
    if clash:
        sys.exit(f"{module}: a mod and port/fixes/{fix.name} replace {' '.join(sorted(clash))}")
    defined_as.update({f: f for f in fixes})

    links, recompiled, replaced_c = list(objs), [], []
    # The Windows build (port/Makefile PORT_OS=windows): a PE object calls a function of its own without naming it
    # (the assembler resolves the call), so weakening the definition would leave those calls to it; the object is
    # made again from its C instead, with the definition alone renamed <name>_orig (what weakening gives on Linux).
    windows = "mingw32" in cc
    renames = {}
    for f, sym in defined_as.items():
        for i, o in enumerate(links):
            if windows:
                m = re.search(rf"^([0-9a-f]+) T {re.escape(sym)}$", run(["nm", objs[i]]), re.M)
                if m:
                    renames.setdefault(i, []).append(sym)
                    links[i] = str(out / f"game_{Path(objs[i]).name}")
                    replaced_c.append(sym)
                    break
                continue
            m = re.search(rf"^([0-9a-f]+) T {re.escape(sym)}$", run(["nm", o]), re.M)
            if not m:
                continue
            copy = out / f"game_{Path(objs[i]).name}"
            if o != str(copy):
                run(["cp", o, str(copy)])
            run(["objcopy", f"--weaken-symbol={sym}", f"--add-symbol={sym}_orig=.text:0x{m.group(1)},global,function",
                 str(copy)])
            links[i] = str(copy)
            replaced_c.append(sym)
            break
        else:
            recompiled.append(sym)
    for i, syms in renames.items():
        rename_definitions(module, Path(objs[i]), Path(links[i]), syms, cc)
    for src in sources:
        o = out / f"{src.stem}.o"
        run([*shlex.split(cc), *defines, "-c", str(src), "-o", str(o)])
        links.append(str(o))
    if fix.exists():
        o = out / f"fix_{module}.o"
        run([*shlex.split(cc), "-c", str(fix), "-o", str(o)])
        links.append(str(o))
    (out / "objs.txt").write_text("\n".join(links) + "\n")
    (out / "mods.ld").write_text("/* Generated by port/mods.py */\n" + "".join(f"{a}\n" for a in aliases))
    (out / "recompiled.txt").write_text("".join(f"{n}\n" for n in recompiled))
    (out / "replaced_c.txt").write_text("".join(f"{n}\n" for n in replaced_c))
    if mods or fixes:
        print(f"{module}: mods {' '.join(mods + [fix.name] * bool(fixes))} "
              f"(replacing {' '.join(f'{f}={s}' if f != s else f for f, s in defined_as.items())})")


if __name__ == "__main__":
    main()
