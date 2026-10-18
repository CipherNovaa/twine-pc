# AI script VM (Level 1)

Characters run bytecode scripts. On disc they're bundled in `Lnn/AIBnn.BIN` inside the level archive; the EXE still contains development file names such as `GOON.AIB` and `SNIPER.AIB`.

- `func_800575FC(cmd, obj, ...)` runs one command. The opcode is the command word's low byte (`*(u8 *)cmd`). Values of 0x44 and up go to handler 0x43.
- The handler table has 0x44 entries at `0x8009D7A8` (`D_8009D7A8`).
- A handler returns 1 or 0 and moves the script on by writing `obj->pc` (`ScriptObj`, offset 0xA4). `obj->wait` is at 0x1C4 and the script variables `obj->vars[0x21]` start at 0x20.
- Example: opcode 0x2F (`func_80056570`) stores the result of `func_80054098(arg2)` into `obj->vars[(cmd >> 8) & 0xFF]` and advances one word.

## Where the scripts come from (Level 1)

`func_8005370C` loads `L01/AIB01.BIN` (at most 0x4000 bytes) and splits it into six scripts using the table at `D_8009D5B4` (8-byte entries: name pointer, size). The start of each script goes into `D_800D9248[6]`.

| # | script | size | offset in AIB01.BIN |
|---|---|---|---|
| 0 | `GOON.AIB` (regular guards) | 0x25E0 | 0x0000 |
| 1 | `PILOT.AIB` | 0x318 | 0x25E0 |
| 2 | `SNIPER.AIB` | 0x848 | 0x28F8 |
| 3 | `ELECTRA.AIB` | 0x634 | 0x3140 |
| 4 | `BONDNIS.AIB` (Bond in cutscenes) | 0x38 | 0x3774 |
| 5 | `DIRECTOR.AIB` | 0x1B4 | 0x37AC |

`func_80053790(character, n)` gives a character script `n` (a negative `n` means 5). `func_80057A24(obj, script)` clears the script state and sets base (`+0xA0`) and pc (`+0xA4`) to the start of the script, so execution starts at offset 0.

Editing a script without changing its size only needs `tools/cdbin.py`. Changing a size also means changing the table in the EXE, for example with a C mod.

## Opcodes

The length column comes from looking at how each handler writes `pc` (`cmd + 4/8/12`). It's a starting point for a disassembler, not verified semantics.

| opcode | handler | bytes of code | command length |
|---|---|---|---|
| `0x00` | `func_800571B8` | 316 | 2 words |
| `0x01` | `func_80057194` | 36 | computed (jump, call or variable) |
| `0x02` | `func_800570F8` | 156 | 1 word |
| `0x03` | `func_80057074` | 132 | computed (jump, call or variable) |
| `0x04` | `func_80056F90` | 228 | computed (jump, call or variable) |
| `0x05` | `func_80056894` | 1160 | 1 word |
| `0x06` | `func_800566F8` | 128 | does not set pc |
| `0x07` | `func_8005519C` | 104 | does not set pc |
| `0x08` | `func_80056614` | 228 | 3 words |
| `0x09` | `func_80055204` | 152 | computed (jump, call or variable) |
| `0x0A` | `func_8005529C` | 96 | 2 words |
| `0x0B` | `func_800552FC` | 96 | 2 words |
| `0x0C` | `func_80055494` | 40 | 2 words |
| `0x0D` | `func_800554BC` | 44 | 2 words |
| `0x0E` | `func_80055544` | 40 | 2 words |
| `0x0F` | `func_8005556C` | 44 | 2 words |
| `0x10` | `func_80055598` | 136 | computed (jump, call or variable) |
| `0x11` | `func_80055620` | 80 | computed (jump, call or variable) |
| `0x12` | `func_80055670` | 264 | computed (jump, call or variable) |
| `0x13` | `func_80055778` | 80 | computed (jump, call or variable) |
| `0x14` | `func_800557C8` | 44 | does not set pc |
| `0x15` | `func_800557F4` | 32 | 1 word |
| `0x16` | `func_80055870` | 8 | does not set pc |
| `0x17` | `func_80055878` | 100 | does not set pc |
| `0x18` | `func_80055BA8` | 132 | does not set pc |
| `0x19` | `func_800573A4` | 108 | 1 word |
| `0x1A` | `func_80055F58` | 252 | 2 words |
| `0x1B` | `func_8005747C` | 384 | 1 word |
| `0x1C` | `func_8005743C` | 64 | 1 word |
| `0x1D` | `func_800572F4` | 176 | computed (jump, call or variable) |
| `0x1E` | `func_80055E0C` | 24 | 1 word |
| `0x1F` | `func_80055E24` | 100 | 1 word |
| `0x20` | `func_80055E88` | 104 | does not set pc |
| `0x21` | `func_800554E8` | 40 | 2 words |
| `0x22` | `func_8005683C` | 88 | computed (jump, call or variable) |
| `0x23` | `func_80056110` | 68 | 1 word |
| `0x24` | `func_80056154` | 516 | computed (jump, call or variable) |
| `0x25` | `func_80056D1C` | 628 | 1 word |
| `0x26` | `func_80055814` | 92 | computed (jump, call or variable) |
| `0x27` | `func_80055510` | 52 | 1 word |
| `0x28` | `func_80056358` | 100 | computed (jump, call or variable) |
| `0x29` | `func_800563BC` | 112 | 1 word |
| `0x2A` | `func_8005642C` | 24 | 1 word |
| `0x2B` | `func_80056444` | 104 | computed (jump, call or variable) |
| `0x2C` | `func_800564AC` | 84 | 1 word |
| `0x2D` | `func_80056500` | 56 | computed (jump, call or variable) |
| `0x2E` | `func_80056538` | 56 | 1 word |
| `0x2F` | `func_80056570` | 108 | 1 word |
| `0x30` | `func_800565DC` | 56 | 1 word |
| `0x31` | `func_80057410` | 44 | 1 word |
| `0x32` | `func_8005535C` | 100 | 2 words |
| `0x33` | `func_800553C0` | 212 | computed (jump, call or variable) |
| `0x34` | `func_80056054` | 188 | 2 words |
| `0x35` | `func_80054604` | 152 | 1 word |
| `0x36` | `func_800547C4` | 120 | 1 word |
| `0x37` | `func_8005483C` | 296 | computed (jump, call or variable) |
| `0x38` | `func_80054964` | 356 | computed (jump, call or variable) |
| `0x39` | `func_80054AC8` | 304 | computed (jump, call or variable) |
| `0x3A` | `func_80054BF8` | 428 | computed (jump, call or variable) |
| `0x3B` | `func_80054DA4` | 148 | computed (jump, call or variable) |
| `0x3C` | `func_80054E38` | 252 | computed (jump, call or variable) |
| `0x3D` | `func_80054F34` | 168 | computed (jump, call or variable) |
| `0x3E` | `func_80054FDC` | 116 | 1 word |
| `0x3F` | `func_80055050` | 8 | does not set pc |
| `0x40` | `func_80055058` | 8 | does not set pc |
| `0x41` | `func_80055060` | 268 | 1 word |
| `0x42` | `func_8005516C` | 48 | does not set pc |
| `0x43` | `func_80055EF0` | 104 | does not set pc |

## What the opcodes do (from matched handlers)

Filled in as handlers are decompiled; see also docs/functions.md.

| opcode | handler | does |
|---|---|---|
| `0x00` | `func_800571B8` | jump/restart: pc = base + cmd[1]*4, clears the action list and selected vars, resets the animation |
| `0x03` | `func_80057074` | return from gosub (pops pc from the call stack at +0x1B0) |
| `0x04` | `func_80056F90` | switch-gosub: vars[byte 1] indexes an n-entry table (n = byte 2), pushes the return address |
| `0x07` | `func_8005519C` | end_action: wait = 1 and pc never advances (the script stops) |
| `0x08` | `func_80056614` | vars[byte 1] = a + rand() % b (a, b at +4/+8, immediate or variable); 3 words |
| `0x10` | `func_80055598` | set the character's angle (sub+0x34) |
| `0x11` | `func_80055620` | no-op (skips its operands) |
| `0x12` | `func_80055670` | set the model position from the non-zero x/y/z operands |
| `0x13` | `func_80055778` | no-op (skips its operands) |
| `0x19` | `func_800573A4` | set the animation rate (byte 1; 0 or less → 2) |
| `0x1B` | `func_8005747C` | play animation: id byte 1, start/end frames bytes 2/3 (-1 = last), wait for it |
| `0x1C` | `func_8005743C` | wait N frames (immediate or variable) |
| `0x1D` | `func_800572F4` | play a positional sound (optional volume, 2-word form) |
| `0x22` | `func_8005683C` | mark every entry of the action list (+0xA8) pending (flag 1) |
| `0x23` | `func_80056110` | vars[n] = abs(vars[n]) |
| `0x24` | `func_80056154` | line-of-sight probe along heading + angle: vars[byte 1] = 0 clear / 1 / 2 hit object |
| `0x2B` | `func_80056444` | target position (+0x1D8..) and heading from the chosen point of interest (+0x1D4) |
| `0x35` | `func_80054604` | StartNIS (start a cutscene): NIS number byte 2, mode byte 3 |
| `0x36` | `func_800547C4` | EndNIS (end the cutscene) |
| `0x37` | `func_8005483C` | set camera target X/Y/Z (each operand: keep, variable or immediate) |
| `0x38` | `func_80054964` | move the camera to its target over N frames |
| `0x39` | `func_80054AC8` | set camera rotation X/Y/Z |
| `0x3A` | `func_80054BF8` | rotate the camera to its target rotation over N frames |
| `0x3C` | `func_80054E38` | camera looks at the nearest object of a type (0 = none) |
| `0x3D` | `func_80054F34` | CD/XA audio: play track id, 0xB1 = stop |
| `0x3E` | `func_80054FDC` | locate a CD file (stream preload) |
| `0x41` | `func_80055060` | call native: D_8009E480[idx](), result to vars[byte 3] |

Helpers used by opcode 0x05: `func_8005416C` compares two commands (operand modes and the operand words they
select); `func_80054268` / `func_8005430C` search the action list at ScriptObj+0xA8 ({count; n; {flag; cmd *}
items[]}) in items[0..count) / items[count..n); `func_800543BC` finds the first pending one.
StartNIS/EndNIS work through `func_80054414` / `func_8005469C`, which set `D_8009CF54` (in cutscene) and the
seen-NIS bits in the bitfield word at `D_800D90C0+0x124`.
