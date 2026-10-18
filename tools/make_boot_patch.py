"""Print a mods-format patch that skips the front-end and boots straight into level N (1-11)."""
import sys

level = int(sys.argv[1])
if not 1 <= level <= 11:
    raise SystemExit("level must be 1-11")
print(f"""# Boot straight into level {level}, using the boot stub's own skip-front-end path.
SLUS_012.72  0x800101E4  14400003  10000003  # bnez $v0 -> b: always skip SHELL.EXE
SLUS_012.72  0x800100BC  A2000230  {0x24090000 | level:08X}  # addiu $t1, $zero, {level} (replaces a store to an already-zeroed field)
SLUS_012.72  0x800100C0  A6060188  A6090188  # g_Args.level = $t1""")
