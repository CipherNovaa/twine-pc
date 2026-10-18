"""Generate an all-assembly splat config (+ checksum) for LEVELnn.EXE, using LEVEL01 as the reference
for where the PsyQ library code ends."""
import hashlib
import struct
import sys
from pathlib import Path

from capstone import CS_ARCH_MIPS, CS_MODE_LITTLE_ENDIAN, CS_MODE_MIPS32, Cs

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import apply_mods as am  # noqa: E402

LAST_LIB_FUNC_L01 = 0x8009C0D0  # _SsReadDeltaValue, last function before .data in LEVEL01


def lo16(v):
    return v - 0x10000 if v & 0x8000 else v


def main():
    """Usage: gen_level_config.py N  |  gen_level_config.py DISCNAME basename TEXT_END_HEX"""
    if len(sys.argv) == 2:
        n = int(sys.argv[1])
        name, base, text_end = f"LEVEL{n:02d}.EXE", f"level{n:02d}", None
    else:
        name, base, text_end = sys.argv[1], sys.argv[2], int(sys.argv[3], 16)
    path = am.source_for(name)
    data = path.read_bytes()
    pc, _, load, size = struct.unpack_from("<4I", data, 0x10)
    md = Cs(CS_ARCH_MIPS, CS_MODE_MIPS32 + CS_MODE_LITTLE_ENDIAN)
    md.detail = True

    if text_end is None:
        last = am.find_equivalent(am.source_for("LEVEL01.EXE").read_bytes(), LAST_LIB_FUNC_L01, data)
        if last is None:
            raise SystemExit(f"{name}: could not locate the last library function")
        end = next(i.address + 8 for i in md.disasm(data[0x800 + last - load:], last) if i.mnemonic == "jr" and i.op_str == "$ra")
    else:
        end = text_end
    data_start = (end + 15) & ~15
    ins = list(md.disasm(data[0x800 + pc - load:0x800 + pc - load + 16], pc))
    bss_end = (ins[2].operands[1].imm << 16) + lo16(ins[3].operands[2].imm & 0xFFFF)
    file_end = load + size

    (ROOT / f"config/{base}.yaml").write_text(f"""name: {name}
sha1: {hashlib.sha1(data).hexdigest()}
options:
  basename: {base}
  target_path: {path.relative_to(ROOT)}
  elf_path: build/{base}/{base}.elf
  base_path: ..
  platform: psx
  compiler: PSYQ

  asm_path: asm/{base}
  src_path: src/{base}
  build_path: build/{base}

  ld_script_path: linkers/{base}.ld
  undefined_funcs_auto_path: linkers/{base}_undefined_funcs_auto.txt
  undefined_syms_auto_path: linkers/{base}_undefined_syms_auto.txt

  find_file_boundaries: False
  o_as_suffix: True
  use_legacy_include_asm: False
  section_order: [".rodata", ".text", ".data", ".bss"]
  symbol_addrs_path: []
  subalign: 2
  string_encoding: ASCII
  data_string_encoding: ASCII
  rodata_string_guesser_level: 2
  data_string_guesser_level: 2

segments:
  - name: header
    type: header
    start: 0x0

  - name: main
    type: code
    start: 0x800
    vram: 0x{load:08X}
    bss_size: 0x{bss_end - file_end:X}
    subsegments:
      - [0x800, asm]
      - [0x{0x800 + data_start - load:X}, data]
  - [0x{0x800 + size:X}]
""")
    (ROOT / f"config/{base}.sha1").write_text(f"{hashlib.sha1(data).hexdigest()}  build/{base}/{base}.exe\n")
    print(f"{name}: text ends {end:08X}, data {data_start:08X}, bss to {bss_end:08X}")


if __name__ == "__main__":
    main()
