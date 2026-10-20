"""Generate the per-module pieces of the native build from the matching build's ELF and the native objects.

A module is one of the game's executables (boot = the SLUS_012.72 stub, movie, shell, level01..level11). Each
is linked on its own (`ld -r`) and then made private but for its descriptor, so the executables' identical
names and addresses can live in one program; the runtime switches between them when the stub runs one
(BIOS Exec, port/src/main.c).

  gen_native.py MODULE OUTDIR MODULE_OBJS... -- RUNTIME_OBJS...

Writes into OUTDIR:
  syms.ld    data symbols at their PS1 addresses (the executable's image is loaded there at run time) and
             names bound to functions of the module (for its partial link)
  stubs.c    a weak stub for every function the game calls that nothing native defines yet (logs the call)
  funcs.c    the module descriptor: PS1 address -> native function for every function the module or the
             runtime implements (calls through function pointers read from PS1 data, trampolines), the
             recompiled functions, main and the BSS range; and a jump for each name that is a runtime
             function (native libraries, host libc)
"""
import functools
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
EXTRACTED = ROOT / "workbench/extracted"
# PsyQ libc functions with standard behaviour: the host C library's versions serve.
HOST_LIBC = {"printf", "sprintf", "strlen", "strcpy", "strncpy", "strcat", "strcmp", "strncmp", "strtok", "strtol",
             "strstr", "strchr", "memchr", "memcpy", "memset", "memmove", "memcmp", "toupper", "tolower", "exit",
             "abs", "atoi"}


def nm(paths, *flags):
    out = subprocess.run(["nm", *flags, *map(str, paths)], capture_output=True, text=True, check=True).stdout
    return [l.split() for l in out.splitlines() if l.strip() and not l.endswith(":")]


def disc_path(module):
    """the executable's path on the disc (and under workbench/extracted)"""
    if module.startswith("level"):
        n = int(module[-2:])
        return f"L{n:02d}/LEVEL{n:02d}.EXE"
    return {"boot": "SLUS_012.72", "shell": "SHDATA/SHELL.EXE", "movie": "MOVIE.EXE"}[module]


def asm_code_files(module):
    """the module's assembly files that hold code (splat's .data files hold data, and the GTE module)"""
    return sorted(p for p in (ROOT / f"asm/{module}").rglob("*.s") if "/data/" not in str(p))


def asm_functions(paths):
    """[(address, size, name)] of the `nonmatching NAME, SIZE` functions in these assembly files"""
    out = []
    for path in paths:
        out += [(int(a, 16), int(s, 16), n) for n, s, a in re.findall(
            r"^nonmatching (\w+), 0x(\w+)\n\n\w+ \1\n.*?/\* [0-9A-F]+ ([0-9A-F]{8}) ", path.read_text(), re.M | re.S)]
    return sorted(out)


BINDING = re.compile(r"^(?:PROVIDE\()?L1_(\w+) = (\w+)\)?;", re.M)


def bindings(module):
    """Level 1 name -> the module's name, from config/<module>_syms.ld (`L1_X = Y;` or `PROVIDE(L1_X = Y);`)"""
    path = ROOT / f"config/{module}_syms.ld"
    return dict(BINDING.findall(path.read_text())) if path.exists() else {}


def psyq_names(module):
    """module name -> Level 1 name (the PsyQ name for library functions), so the module's copies of functions
    the runtime implements natively use the native version. psyq_names.txt names Level 1's library functions
    the signature tool left unnamed. Other modules bind Level 1 names to theirs in config/<module>_syms.ld, and
    their library functions are found by code (library_layout), as func_<address>."""
    names = dict(l.split() for l in (ROOT / "port/psyq_names.txt").read_text().splitlines()
                 if l.strip() and not l.startswith("#"))
    # native_names.txt: the runtime's versions of functions in any module, by address
    own = {f"func_{a.upper()}": n for m, a, n in (l.split() for l in (ROOT / "port/native_names.txt").read_text()
                                                    .splitlines() if l.strip() and not l.startswith("#"))
           if m == module}
    if module == "level01":
        return {**names, **own}
    # only real names: an address-based Level 1 name (func_8004B324) is another function in this module
    real = lambda n: not re.fullmatch(r"(func|D)_[0-9A-F]{8}", n)
    out = {y: names.get(x, x) for x, y in bindings(module).items() if real(names.get(x, x))}
    for a, _, x in library_layout(module):
        if real(names.get(x, x)):
            out.setdefault(f"func_{a:08X}", names.get(x, x))
    return {**out, **own}


@functools.lru_cache(None)
def library_layout(module):
    """[(address, size, Level 1 name)] of Level 1's library functions in another module: where a binding puts
    them, else where their code is (addresses masked out). Small functions (call thunks, setters) can have
    several identical copies: the one at the same distance from a placed neighbour as in Level 1 is it (the
    libraries link in the same order). The module's asm can have two of these as one function, which the
    recompiler splits (recomp.py functions)."""
    sys.path.insert(0, str(ROOT / "tools"))
    import shared_code

    l1_lib = asm_functions([ROOT / "asm/level01/psyq_libs.s"])
    # where library code can be: a level's library segment, or anywhere in the other executables
    lib = ROOT / f"asm/{module}/psyq_libs.s"
    t_funcs = asm_functions([lib] if lib.exists() else asm_code_files(module))
    lo, hi = t_funcs[0][0], t_funcs[-1][0] + t_funcs[-1][1]
    l1_addr, l1_words = shared_code.load(EXTRACTED / "L01/LEVEL01.EXE")
    t_addr, t_words = shared_code.load(EXTRACTED / disc_path(module))
    t_syms = ps1_symbols(module)
    bound = bindings(module)
    placed = {a: t_syms[bound[x]] for a, _, x in l1_lib if bound.get(x) in t_syms}
    index = {}
    for i in range(len(t_words) - 1):
        index.setdefault((t_words[i], t_words[i + 1]), []).append(i)
    l1_raw, t_raw = raw_words(EXTRACTED / "L01/LEVEL01.EXE"), raw_words(EXTRACTED / disc_path(module))
    size_of = {a: (s, x) for a, s, x in l1_lib}

    def best_raw(a, hits):
        """the copies agreeing most with Level 1's: words equal unmasked (thunks differ in a table offset), and
        calls to functions already placed there (call thunks differ in what they call)"""
        n = size_of[a][0] // 4
        i = (a - l1_addr) // 4

        def agree(x, y):
            if x == y:
                return True
            if x >> 26 == 3 and y >> 26 == 3:
                return placed.get(0x80000000 | (x & 0x3FFFFFF) << 2) == 0x80000000 | (y & 0x3FFFFFF) << 2
            return False
        score = {h: sum(agree(x, y) for x, y in zip(l1_raw[i:i + n], t_raw[(h - t_addr) // 4:(h - t_addr) // 4 + n]))
                 for h in hits}
        top = max(score.values(), default=0)
        return [h for h in hits if score[h] == top]

    found = {}
    for a, size, x in l1_lib:
        if a in placed or size < 8:
            continue
        i = (a - l1_addr) // 4
        seq = l1_words[i:i + size // 4]
        found[a] = [t_addr + 4 * j for j in index.get(tuple(seq[:2]), []) if t_words[j:j + len(seq)] == seq
                    and lo <= t_addr + 4 * j < hi]
    # unique copies place their neighbours too, and those theirs
    placed.update({a: h[0] for a, h in found.items() if len(h) == 1})
    changed = True
    while changed:
        changed = False
        anchors = sorted(placed.items())
        for a, hits in found.items():
            if len(hits) > 1 and a not in placed:
                prev = max((p for p in anchors if p[0] < a), default=None)
                nxt = min((p for p in anchors if p[0] > a), default=None)
                # the copies whose instructions agree most with Level 1's unmasked (thunks differ in a table
                # offset), then the one at its neighbour's distance
                at_distance = lambda c: [h for h in c if any(p and h - p[1] == a - p[0] for p in (prev, nxt))]
                near = best_raw(a, hits)
                if len(near) > 1:
                    near = at_distance(near) or best_raw(a, at_distance(hits))
                if len(near) == 1:
                    placed[a] = near[0]
                    changed = True
    return sorted({(t, size_of[a][0], size_of[a][1]) for a, t in placed.items()})


def raw_words(path):
    """an executable's text+data as instruction words, unmasked"""
    data = path.read_bytes()
    size = int.from_bytes(data[0x1C:0x20], "little")
    return [int.from_bytes(data[0x800 + k:0x804 + k], "little") for k in range(0, size, 4)]


def ps1_symbols(module):
    """name -> address in the matching build; the game's main() is game_main natively (common.h GAME_MAIN)"""
    ps1 = {}
    for f in nm([ROOT / f"build/{module}/{module}.elf"], "-n"):
        if len(f) == 3 and re.fullmatch(r"[0-9a-f]{8}", f[0]):
            ps1.setdefault("game_main" if f[2] == "main" else f[2], int(f[0], 16))
    return ps1


def entry_layout(module):
    """What the executable's entry point does first: clear BSS (two lui/addiu address pairs), then jump to
    main. Returns (pc0, text size, (bss start, bss end), main address or None when the entry calls it
    differently (the boot stub: the runtime does its entry, main.c))."""
    exe = (ROOT / f"build/{module}/{module}.exe").read_bytes()
    pc0, load = int.from_bytes(exe[0x10:0x14], "little"), int.from_bytes(exe[0x18:0x1C], "little")
    size = int.from_bytes(exe[0x1C:0x20], "little")
    w = [int.from_bytes(exe[0x800 + pc0 - load + 4 * k:0x804 + pc0 - load + 4 * k], "little") for k in range(16)]
    s16 = lambda v: v - 0x10000 if v & 0x8000 else v
    bss = (((w[0] & 0xFFFF) << 16) + s16(w[1] & 0xFFFF), ((w[2] & 0xFFFF) << 16) + s16(w[3] & 0xFFFF))
    jmp = next((e for e in w if e >> 26 == 2), None)
    main = (pc0 & 0xF0000000) | ((jmp & 0x3FFFFFF) << 2) if jmp is not None else None
    return pc0, size, bss, main


def main():
    module, outdir = sys.argv[1], Path(sys.argv[2])
    sep = sys.argv.index("--")
    mod_objs, rt_objs = sys.argv[3:sep], sys.argv[sep + 1:]

    ps1 = ps1_symbols(module)
    text_start = ps1["main_TEXT_START"]
    data_start = ps1["main_DATA_START"]

    def symbols(objs):
        defined, undefined = {}, set()
        for f in nm(objs):
            if len(f) == 3 and f[1] in "TWV":
                defined[f[2]] = f[1]
            elif len(f) == 3 and f[1] in "DdBbRrC":
                defined.setdefault(f[2], f[1])
            elif len(f) == 2 and f[0] in "Uw":
                undefined.add(f[1])
        return defined, undefined

    mod_defined, mod_undefined = symbols(mod_objs)
    rt_defined, _ = symbols(rt_objs)
    defined = {**rt_defined, **mod_defined}
    undefined = mod_undefined - set(mod_defined)
    is_fn = lambda n: defined.get(n) in ("T", "W")

    def is_code(name, addr):
        if text_start <= addr < data_start:
            return True
        # hand-written GTE code (POLY.c) at the end of .data
        return name.startswith("func_") and addr >= data_start

    # library functions the game calls by address-based names, implemented natively under their PsyQ names
    names = psyq_names(module)
    aliases = {n: names[n] for n in undefined if n in names and is_fn(names[n])}
    # another level's names bound to this module's (L1_X = Y, L2_X = Y, ...): the native version when there
    # is one, else the module's own function (C or recompiled)
    cfg = ROOT / f"config/{module}_syms.ld"
    if module != "level01" and cfg.exists():
        for x, y in re.findall(r"^(?:PROVIDE\()?(L\d+_\w+) = (\w+)\)?;", cfg.read_text(), re.M):
            if x in undefined:
                for target in (names.get(y), y):
                    if is_fn(target) or target in HOST_LIBC:
                        aliases[x] = target
                        break
    # the other way round (func_8005D7A0 = L1_func_8005D760): a name the code uses for a function the module
    # defines under another name at its address (recomp.py leaves such functions to that definition)
    defined_at = {}
    for n, a in ps1.items():
        if n in mod_defined and is_fn(n):
            defined_at.setdefault(a, n)
    for n in undefined:
        if n not in aliases and n not in rt_defined and n in ps1 and defined_at.get(ps1[n], n) != n:
            aliases[n] = defined_at[ps1[n]]

    syms, stubs = [], []
    for name in sorted(undefined):
        if name.startswith(("GTE_", "port_", "_GLOBAL_OFFSET")) or name not in ps1 or name in HOST_LIBC \
                or name in aliases or name in rt_defined:
            continue
        addr = ps1[name]
        (stubs if is_code(name, addr) else syms).append((name, addr))
    missing = sorted(n for n in undefined if n not in ps1 and not n.startswith(("GTE_", "port_"))
                     and n not in HOST_LIBC and n not in rt_defined and n not in aliases)

    pc0, text_size, bss, main_addr = entry_layout(module)
    main_name = None
    if main_addr is not None:
        main_name = next((n for n, a in sorted(ps1.items()) if a == main_addr and n in mod_defined
                          and is_fn(n)), "port_recomp_main")

    # aliases: to the module's own functions in the partial link's script, to the runtime's by renaming
    in_module = {n: t for n, t in aliases.items() if t in mod_defined}
    to_runtime = {n: t for n, t in aliases.items() if t not in mod_defined}

    outdir.mkdir(parents=True, exist_ok=True)
    (outdir / "syms.ld").write_text(
        "/* Generated by port/gen_native.py: PS1 data symbols at their addresses, then names bound to functions\n"
        "   of the module. */\n"
        + "".join(f"{n} = 0x{a:08X};\n" for n, a in syms)
        + "".join(f"{n} = {in_module[n]};\n" for n in sorted(in_module)))
    (outdir / "stubs.c").write_text(
        "/* Generated by port/gen_native.py: functions nothing native implements yet. */\n"
        "void port_unimplemented(const char *name, unsigned addr);\n"
        + "".join(f"__attribute__((weak)) int {n}(void) {{ port_unimplemented(\"{n}\", 0x{a:08X}u); return 0; }}\n"
                  for n, a in stubs))
    # every native function with a PS1 address (game C, library implementations, stubs, recompiled code)
    # (recompiled entry points the ELF has no function symbol for are named func_<address>)
    own = {n: ps1[n] if n in ps1 else int(n[5:], 16) for n in mod_defined
           if is_fn(n) and (n in ps1 or re.fullmatch(r"func_[0-9A-F]{8}", n))}
    funcs = sorted({(a, n) for n, a in own.items() if is_code(n, a)}
                   | {(a, n) for n, a in stubs} | {(ps1[n], aliases[n]) for n in aliases if n in ps1})
    # library functions implemented natively that nothing calls by name (function pointers, recompiled calls)
    if module != "level01":
        have = {a for a, _ in funcs}
        funcs = sorted(set(funcs) | {(a, names[f"func_{a:08X}"]) for a, _, _ in library_layout(module)
                                     if a not in have and is_fn(names.get(f"func_{a:08X}"))})
    # one function per address (a trampoline each)
    by_addr = {}
    for a, n in funcs:
        by_addr.setdefault(a, n)
    funcs = sorted(by_addr.items())
    level = int(module[-2:]) if module.startswith("level") else 0
    (outdir / "funcs.c").write_text(
        "/* Generated by port/gen_native.py: the module descriptor (PS1 address -> native function, recompiled\n"
        "   functions, main, layout). */\n"
        '#include "module.h"\n'
        + "/* names that are the runtime's functions (native libraries, host libc) */\n"
        + "".join(f'__asm__(".globl {n}\\n{n}: jmp {to_runtime[n]}");\n' for n in sorted(to_runtime))
        + "".join(f"void {n}();\n" for n in sorted({n for _, n in funcs} | ({main_name} if main_name else set())))
        + "extern const RcEntry port_rc_funcs[];\nextern const int port_rc_func_count;\n"
        + "static const PortFunc funcs[] = {\n"
        + "".join(f"    {{0x{a:08X}u, (void *){n}}},\n" for a, n in funcs)
        + "};\n"
        + f"const PortModule port_module_{module} = {{\n"
        + f'    "{module}", "{disc_path(module)}", {level},\n'
        + "    funcs, sizeof(funcs) / sizeof(funcs[0]), port_rc_funcs, &port_rc_func_count,\n"
        + f"    (s32 (*)(s32, void *)){main_name or 0},\n"
        + f"    0x{pc0:08X}u, 0x{text_size:X}u, 0x{bss[0]:08X}u, 0x{bss[1]:08X}u,\n"
        + "};\n")
    print(f"{module}: {len(syms)} data symbols, {len(aliases)} library aliases ({len(to_runtime)} native), "
          f"{len(stubs)} stubbed functions, {len(funcs)} PS1 entry points"
          + (f"; no PS1 address for: {' '.join(missing)}" if missing else ""))


if __name__ == "__main__":
    main()
