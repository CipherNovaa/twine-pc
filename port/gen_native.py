"""Generate the per-level pieces of a native build from the matching build's ELF and the native objects.

  gen_native.py LEVEL OUTDIR GAME_OBJS... -- RUNTIME_OBJS...

Writes into OUTDIR:
  syms.ld   data symbols at their PS1 addresses (the level's data image is loaded there at run time)
  stubs.c   a weak stub for every function the game calls that nothing native defines yet (logs the call)
  funcs.c   PS1 address -> native function for every game/library function the build defines, so calls
            through function pointers read from PS1 data reach native code
"""
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
# PsyQ libc functions with standard behaviour: the host C library's versions serve.
HOST_LIBC = {"printf", "sprintf", "strlen", "strcpy", "strncpy", "strcat", "strcmp", "strncmp", "strtok", "strtol",
             "strstr", "strchr", "memchr", "memcpy", "memset", "memmove", "memcmp", "toupper", "tolower", "exit",
             "abs", "atoi"}


def nm(paths, *flags):
    out = subprocess.run(["nm", *flags, *map(str, paths)], capture_output=True, text=True, check=True).stdout
    return [l.split() for l in out.splitlines() if l.strip() and not l.endswith(":")]


def psyq_names(level):
    """level name -> PsyQ name for library functions the signature tool left unnamed. psyq_names.txt has
    Level 1's names; other levels bind each Level 1 name to theirs in config/<level>_syms.ld (L1_X = Y)."""
    names = dict(l.split() for l in (ROOT / "port/psyq_names.txt").read_text().splitlines()
                 if l.strip() and not l.startswith("#"))
    if level == "level01":
        return names
    out = {}
    for line in (ROOT / f"config/{level}_syms.ld").read_text().splitlines():
        m = re.match(r"L1_(\w+) = (\w+);", line)
        if m and m.group(1) in names:
            out[m.group(2)] = names[m.group(1)]
    return out


def ps1_symbols(level):
    """name -> address in the matching build; the game's main() is game_main natively (common.h GAME_MAIN)"""
    ps1 = {}
    for f in nm([ROOT / f"build/{level}/{level}.elf"], "-n"):
        if len(f) == 3 and re.fullmatch(r"[0-9a-f]{8}", f[0]):
            ps1.setdefault("game_main" if f[2] == "main" else f[2], int(f[0], 16))
    return ps1


def main():
    level, outdir = sys.argv[1], Path(sys.argv[2])
    sep = sys.argv.index("--")
    game_objs, rt_objs = sys.argv[3:sep], sys.argv[sep + 1:]
    objs = game_objs + rt_objs

    ps1 = ps1_symbols(level)
    text_start = ps1["main_TEXT_START"]
    data_start = ps1["main_DATA_START"]

    defined, undefined = {}, set()
    for f in nm(objs):
        if len(f) == 3 and f[1] in "TWV":
            defined[f[2]] = f[1]
        elif len(f) == 3 and f[1] in "DdBbRrC":
            defined.setdefault(f[2], f[1])
        elif len(f) == 2 and f[0] in "Uw":
            undefined.add(f[1])
    undefined -= set(defined)

    def is_code(name, addr):
        if text_start <= addr < data_start:
            return True
        # hand-written GTE code (POLY.c) at the end of .data
        return name.startswith("func_") and addr >= data_start

    # library functions the game calls by address-based names, implemented natively under their PsyQ names
    names = psyq_names(level)
    aliases = {n: names[n] for n in undefined if n in names and defined.get(names[n]) in ("T", "W")}
    if level != "level01":
        for line in (ROOT / f"config/{level}_syms.ld").read_text().splitlines():
            m = re.match(r"(L1_\w+) = (\w+);", line)
            if m and m.group(1) in undefined:
                target = names.get(m.group(2), m.group(2))
                if defined.get(target) in ("T", "W") or target in HOST_LIBC:
                    aliases[m.group(1)] = target

    syms, stubs = [], []
    for name in sorted(undefined):
        if name.startswith(("GTE_", "port_", "_GLOBAL_OFFSET")) or name not in ps1 or name in HOST_LIBC \
                or name in aliases:
            continue
        addr = ps1[name]
        (stubs if is_code(name, addr) else syms).append((name, addr))
    missing = sorted(n for n in undefined if n not in ps1 and not n.startswith(("GTE_", "port_")))

    # the BSS range the entry point clears: its two lui/addiu address pairs
    exe = (ROOT / f"build/{level}/{level}.exe").read_bytes()
    pc0, load = int.from_bytes(exe[0x10:0x14], "little"), int.from_bytes(exe[0x18:0x1C], "little")
    w = [int.from_bytes(exe[0x800 + pc0 - load + 4 * k:0x804 + pc0 - load + 4 * k], "little") for k in range(4)]
    s16 = lambda v: v - 0x10000 if v & 0x8000 else v
    bss = [((w[0] & 0xFFFF) << 16) + s16(w[1] & 0xFFFF), ((w[2] & 0xFFFF) << 16) + s16(w[3] & 0xFFFF)]
    # ... and then jumps to main: whatever implements that address (native C or recompiled) starts the game
    entry = [int.from_bytes(exe[0x800 + pc0 - load + 4 * k:0x804 + pc0 - load + 4 * k], "little") for k in range(16)]
    jmp = next(e for e in entry if e >> 26 == 2)
    main_addr = (pc0 & 0xF0000000) | ((jmp & 0x3FFFFFF) << 2)
    main_name = next((n for n, a in sorted(ps1.items()) if a == main_addr and defined.get(n) in ("T", "W")),
                     "port_recomp_main")

    outdir.mkdir(parents=True, exist_ok=True)
    (outdir / "syms.ld").write_text(
        "/* Generated by port/gen_native.py: PS1 data symbols at their addresses, then library aliases. */\n"
        + "".join(f"{n} = 0x{a:08X};\n" for n, a in syms)
        + "".join(f"{n} = {aliases[n]};\n" for n in sorted(aliases)))
    (outdir / "stubs.c").write_text(
        "/* Generated by port/gen_native.py: functions nothing native implements yet. */\n"
        "void port_unimplemented(const char *name, unsigned addr);\n"
        + "".join(f"__attribute__((weak)) int {n}(void) {{ port_unimplemented(\"{n}\", 0x{a:08X}u); return 0; }}\n"
                  for n, a in stubs))
    # every native function with a PS1 address (game C, library implementations, stubs)
    funcs = sorted({(ps1[n], n) for n, t in defined.items() if t in "TW" and n in ps1 and is_code(n, ps1[n])}
                   | {(a, n) for n, a in stubs} | {(ps1[n], aliases[n]) for n in aliases})
    (outdir / "funcs.c").write_text(
        "/* Generated by port/gen_native.py: PS1 address -> native function, and the level's layout. */\n"
        f"const int port_level = {int(level[-2:])};\n"
        f"const unsigned port_bss_start = 0x{bss[0]:08X}u, port_bss_end = 0x{bss[1]:08X}u;\n"
        "typedef struct { unsigned addr; void *fn; } PortFunc;\n"
        + "".join(f"void {n}();\n" for n in sorted({n for _, n in funcs} | {main_name}))
        + f"void (*const port_game_main)() = {main_name};\n"
        + "const PortFunc port_funcs[] = {\n"
        + "".join(f"    {{0x{a:08X}u, (void *){n}}},\n" for a, n in funcs)
        + "};\nconst int port_func_count = sizeof(port_funcs) / sizeof(port_funcs[0]);\n")
    print(f"{level}: {len(syms)} data symbols, {len(aliases)} library aliases, {len(stubs)} stubbed functions, "
          f"{len(funcs)} PS1 entry points"
          + (f"; no PS1 address for: {' '.join(missing)}" if missing else ""))


if __name__ == "__main__":
    main()
