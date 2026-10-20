#!/usr/bin/env python3
"""Write a PS1 memory card image with a fully unlocked save for
007: The World Is Not Enough (SLUS-01272).

Usage: make_unlocked_card.py GAME_SAVE.mcd OUT.mcd [--difficulty agent|007] [--cheats-on]
Put OUT.mcd in slot 2 (native port: TWINE_CARD2=OUT.mcd), then Options > Memory Card > Load >
memory card slot 2 > yes; the game goes straight to mission select. In the main menu use CONTINUE
(NEW GAME wipes progress).

The front end (SHDATA/SHELL.EXE) saves one 1-block file "BASLUS-01272":
0x400 bytes written from RAM 0x800DE1E0 =
  0x000-0x1FF  title frame ("SC", Shift-JIS title, CLUT, 3 icon frames)
  0x200-0x347  save_data (0x148 bytes, built by func_800439E0 from g_Args,
               restored by func_80043CAC; only check: byte 0 == 4)
  0x348-0x3FF  zero (the copy is 0x200 bytes; the rest of the block is zero)

save_data layout (offsets from block+0x200; "Args" = boot stub g_Args):
  +0x00 u8      version, must be 4 ("bad card header!")
  +0x01 u8      reverse sniper            (Args+0x117)
  +0x02 u8[5]   audio levels              (Args+0x142,+0x13A,+0x13E,+0x140,+0x144)
  +0x07 u8      pad
  +0x08 u32     flags2                    (Args+0x04; option, cheat-on and cheat-unlocked bits)
  +0x0C u32     Args+0x08                 (unused, 0)
  +0x10 u8[11]  mission completed         (Args+0x17C; win movies, "won the game" check)
  +0x1B u8[11]  mission available         (Args+0x170; mission select, intro movies)
  +0x26 u8      difficulty 1=agent 2=007  (Args+0x12A; anything else asserts on load)
  +0x27 u8      pad
  +0x28 u16[12] button map                (Args+0x148; 8 used)
  +0x40 [11][3] best scores {char initials[4]; u32 score} (Args+0x0C)
Missions are in game order: Courier, King's Ransom, Cold Reception, Russian
Roulette, Night Watch, Masquerade, Flashpoint, City of Walkways, Turncoat,
Fallen Angel, Meltdown.  There is no checksum in the save itself; only the card
directory frames carry one (XOR of bytes 0..0x7E stored at 0x7F).

flags2 cheat bits (Options > Cheats, func_80046894 / func_80046BA4):
  cheat            unlocked bit   on bit   earned by (func_8004DB98)
  all weapons          24           15     Turncoat on 007 under 7 min
  stealthy bond        19           10     Night Watch, condition D_800738CC == 0
  invulnerable         25           16     Meltdown on 007 under 10 min
  power goons          21           12     Russian Roulette under 15 min
  power guns           22           13     Flashpoint, condition D_800738CC == 0
  infinite ammo        23           14     Fallen Angel on 007, D_800738CC == 0
  bit 4 = "agent" difficulty marker (kept in sync with +0x26 on load).
The SECRET WIN GAME movie needs Meltdown completed with difficulty 007; the
other movies follow the available/completed arrays.
"""
import argparse
import functools
import struct
import sys

CARD_SIZE = 0x20000
BLOCK = 0x2000
FRAME = 0x80
SAVE_NAME = b"BASLUS-01272"
SAVE_DATA_OFF = 0x200
SAVE_DATA_LEN = 0x148
NUM_MISSIONS = 11

CHEATS = {  # name: (unlocked bit, on bit)
    "all_weapons": (24, 15),
    "stealthy_bond": (19, 10),
    "invulnerable": (25, 16),
    "power_goons": (21, 12),
    "power_guns": (22, 13),
    "infinite_ammo": (23, 14),
}
UNLOCK_MASK = functools.reduce(lambda m, b: m | (1 << b[0]), CHEATS.values(), 0)  # 0x03E80000
ON_MASK = functools.reduce(lambda m, b: m | (1 << b[1]), CHEATS.values(), 0)      # 0x0001F400
AGENT_BIT = 1 << 4


def xor_frame(fr):
    return functools.reduce(lambda a, b: a ^ b, fr[:0x7F], 0)


def find_save(card):
    """Return the 0x2000-byte block holding BASLUS-01272 in a raw card image."""
    for slot in range(1, 16):
        fr = card[slot * FRAME:(slot + 1) * FRAME]
        state = struct.unpack_from("<I", fr, 0)[0]
        name = fr[0x0A:0x0A + 20].split(b"\0")[0]
        if state == 0x51 and name == SAVE_NAME:
            return bytes(card[slot * BLOCK:(slot + 1) * BLOCK])
    raise SystemExit("template card has no %s save" % SAVE_NAME.decode())


def load_template(path):
    data = open(path, "rb").read()
    if len(data) == CARD_SIZE and data[:2] == b"MC":
        return find_save(data)
    if len(data) == BLOCK and data[:2] == b"SC":   # bare save block
        return data
    raise SystemExit("%s: expected a raw 128 KB card (.mcd/.mcr) or an 8 KB save block" % path)


def unlock(block, difficulty, cheats_on):
    blk = bytearray(block)
    sd = SAVE_DATA_OFF
    if blk[sd] != 4:
        raise SystemExit("template save_data version is %d, expected 4" % blk[sd])
    flags2 = struct.unpack_from("<I", blk, sd + 0x08)[0]
    flags2 |= UNLOCK_MASK
    flags2 = (flags2 | ON_MASK) if cheats_on else (flags2 & ~ON_MASK)
    flags2 = (flags2 | AGENT_BIT) if difficulty == 1 else (flags2 & ~AGENT_BIT)
    struct.pack_into("<I", blk, sd + 0x08, flags2)
    for i in range(NUM_MISSIONS):
        blk[sd + 0x10 + i] = 1   # completed
        blk[sd + 0x1B + i] = 1   # available
    blk[sd + 0x26] = difficulty
    return bytes(blk)


def build_card(block):
    card = bytearray(CARD_SIZE)
    # frame 0: "MC" header
    card[0:2] = b"MC"
    # directory: slot 1 = our save, 2..15 free
    for slot in range(1, 16):
        fr = bytearray(FRAME)
        if slot == 1:
            struct.pack_into("<IIH", fr, 0, 0x51, BLOCK, 0xFFFF)
            fr[0x0A:0x0A + len(SAVE_NAME)] = SAVE_NAME
        else:
            struct.pack_into("<IIH", fr, 0, 0xA0, 0, 0xFFFF)
        card[slot * FRAME:(slot + 1) * FRAME] = fr
    # frames 16-35: broken sector list (none); 36-62: unused (0xFF); 63: write test
    for f in range(16, 36):
        fr = bytearray(FRAME)
        struct.pack_into("<IIH", fr, 0, 0xFFFFFFFF, 0, 0xFFFF)
        card[f * FRAME:(f + 1) * FRAME] = fr
    for f in range(36, 63):
        card[f * FRAME:(f + 1) * FRAME] = b"\xff" * FRAME
    for f in range(0, 36):
        fr = card[f * FRAME:(f + 1) * FRAME]
        card[f * FRAME + 0x7F] = xor_frame(fr)
    card[63 * FRAME:64 * FRAME] = card[0:FRAME]   # write test frame = copy of the header frame
    card[BLOCK:2 * BLOCK] = block
    return bytes(card)


def describe(block):
    sd = SAVE_DATA_OFF
    flags2 = struct.unpack_from("<I", block, sd + 8)[0]
    names = ["Courier", "King's Ransom", "Cold Reception", "Russian Roulette", "Night Watch",
             "Masquerade", "Flashpoint", "City of Walkways", "Turncoat", "Fallen Angel", "Meltdown"]
    out = ["flags2 = 0x%08X, difficulty = %s" % (flags2, {1: "agent", 2: "007"}.get(block[sd + 0x26], block[sd + 0x26]))]
    for i, n in enumerate(names):
        out.append("  %-17s available=%d completed=%d" % (n, block[sd + 0x1B + i], block[sd + 0x10 + i]))
    for n, (u, o) in CHEATS.items():
        out.append("  cheat %-14s unlocked=%d on=%d" % (n, flags2 >> u & 1, flags2 >> o & 1))
    return "\n".join(out)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("template", help="card image (raw 128 KB) or 8 KB save block made by the game")
    ap.add_argument("output", help="card image to write (raw 128 KB, .mcd/.mcr)")
    ap.add_argument("--difficulty", choices=["agent", "007"], default="007",
                    help="saved difficulty (default 007: also unlocks the SECRET WIN GAME movie)")
    ap.add_argument("--cheats-on", action="store_true",
                    help="also switch every bonus cheat on (they can be toggled in Options > Cheats)")
    args = ap.parse_args()
    block = load_template(args.template)
    block = unlock(block, 2 if args.difficulty == "007" else 1, args.cheats_on)
    card = build_card(block)
    with open(args.output, "wb") as f:
        f.write(card)
    print("wrote %s" % args.output)
    print(describe(block))


if __name__ == "__main__":
    sys.exit(main())
