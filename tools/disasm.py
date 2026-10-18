import argparse
import struct

from capstone import CS_ARCH_MIPS, CS_MODE_LITTLE_ENDIAN, CS_MODE_MIPS32, Cs


def load_psx_exe(path):
    data = open(path, "rb").read()
    if data[:8] != b"PS-X EXE":
        raise SystemExit(f"{path}: not a PS-X EXE")
    pc, gp, load_addr, size = struct.unpack_from("<4I", data, 0x10)
    return data, pc, gp, load_addr, size


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("exe")
    ap.add_argument("vaddr", nargs="?", help="hex address; defaults to entry point")
    ap.add_argument("-n", "--count", type=int, default=40)
    args = ap.parse_args()

    data, pc, gp, load_addr, size = load_psx_exe(args.exe)
    print(f"; entry=0x{pc:08x} gp=0x{gp:08x} load=0x{load_addr:08x} size=0x{size:x}")

    vaddr = int(args.vaddr, 16) if args.vaddr else pc
    off = 0x800 + (vaddr - load_addr)
    md = Cs(CS_ARCH_MIPS, CS_MODE_MIPS32 + CS_MODE_LITTLE_ENDIAN)
    for i, insn in enumerate(md.disasm(data[off:off + args.count * 4], vaddr)):
        print(f"{insn.address:08x}:  {insn.mnemonic:8s} {insn.op_str}")


if __name__ == "__main__":
    main()
