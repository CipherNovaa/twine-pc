# TWINE: development guide

A work-in-progress matching decompilation of **007: The World Is Not Enough** (PlayStation, NTSC-U, SLUS-01272, Black Ops Entertainment, 2000).

"Matching" means the C source recompiles to a byte-identical copy of the original game. Every change is checked against the retail checksum, so the source can be modded with confidence.

You need your own copy of the game. No game data is in this repository.

## Status

| | |
|---|---|
| `LEVEL01.EXE` rebuilt from source | **byte-identical** (`make`) |
| All 14 executables (boot stub, menu, movie player, 11 levels) | **byte-identical** (`make LEVEL=level05`, `LEVEL=shell`, ...) |
| Full disc image from rebuilt executables | **byte-identical** to the retail dump (`tools/build_all.sh`) |
| All 11 levels are **shiftable** | code and data can grow or shrink and the game still runs. Tested by inserting 4 KB early in the code of levels 1, 2 and 6; in Level 1, gameplay, the pause menu and the developer menu all work. Mods can be written in C without matching. |
| C mods in **every level** | `make cmod CMODS="TWINSTICK WIDEFOV"`: modern twin-stick controls and a wider field of view, checked in DuckStation (twin-stick in Levels 1, 2 and 4, and played with a DualSense; wide FOV in Level 2) |
| Level 1 game functions in C | 774 of 866 functions match exactly (89%). 90 more are near-miss C under `#ifdef NON_MATCHING` (the matching build keeps their asm; a disc built with them plays fine in DuckStation), so every function except the two startup stubs has C. Run `tools/progress.py` and `tools/banked_scores.py` for current numbers. |
| Levels 2–11 | Each builds byte-identical with about 420–440 functions from Level 1's C, 100–118 from Level 2's (levels 3–11), and 90–120 of its own (`tools/port_level.py`, `auto_match.py --level`), plus 63–70 near misses. Roughly 170–210 functions per level are still asm, mostly that level's own code. |
| Compiler | GCC 2.8.x, Sony PSX build (old-gcc `2.8.1-psx`), `-O2 -G0`, no `-fno-builtin`. Three source regions are `-O0`. |
| SDK | PsyQ 4.4 (580 library functions auto-identified) |

The 11 levels share one engine. 617 of Level 1's 841 game functions appear unchanged in every other level, in the same link order, so most decompiled code applies to all levels. See `docs/port_plan.md` for the plan for a native PC port.

## Native PC port

`make -C port` builds `build/port/twine`: the whole game as one native Linux program (32-bit x86). The decompiled C is compiled for the PC; code still in assembly is statically recompiled to C (`port/recomp.py`); the console's hardware and libraries are written anew for the host in `port/src` (software GPU, GTE, SPU with XA streaming audio, MDEC movies, CD reading straight from the disc image, memory cards, SDL3 for the window, input and sound). It plays the game from the PlayStation boot animation through the logos, intro movie, menus and briefings into the missions and back, with saving and loading. Between the game's 20 Hz ticks the camera moves smoothly at the display's refresh rate, and the mouse looks around.

Building needs gcc with 32-bit support, 32-bit SDL3 (`lib32-sdl3`), `glslc` (`shaderc`) and `xxd` for the hardware renderer's shaders, and the matching builds of all 14 executables (`tools/build_all.sh`, see Setup).

Running needs your disc image (`.bin` with raw 2352-byte sectors) and, for the boot animation, your console's BIOS (e.g. `SCPH1001.BIN`; without one only the title card shows). By default the program looks for `workbench/disc/007 - The World Is Not Enough (USA).bin` and a `bios/` folder in the current directory.

**Settings** go in `~/.config/twine/twine.ini` (or `$XDG_CONFIG_HOME/twine/twine.ini`), one `key = value` per line; each stands for the environment variable `TWINE_<KEY>`, which wins if both are set. The most used are also on the front end's OPTIONS screen, under PORT SETTINGS (mouse speed, invert mouse, pad layout, smooth motion, HD movies, vibration, infinite grenades, autoload), which writes them here:

| key | |
|---|---|
| `disc`, `bios` | the disc image; the BIOS file or a folder with it |
| `fullscreen = 1` | start in full screen (F11 or Alt+Enter switches at any time) |
| `display` | which display: its number or part of its name (`port.log` lists them), e.g. `display = HDMI-A-1` |
| `aspect = fill` | stretch the picture over the whole screen (for a 4:3 CRT driven at 1280x1024); by default it keeps 4:3 |
| `vsync = 0` | show frames as fast as they come instead of at the display's refresh |
| `smooth = 0` | no frames between the game's ticks |
| `fps_max` | frames at most that many a second (`0`: no limit); by default three times the display's refresh rate, so that each refresh gets one drawn at most a third of a refresh before it |
| `mouse_sens`, `mouse_invert = 1` | mouse look speed (default 1), inverted up/down |
| `mouse = 0` | leave the mouse alone (no mouse look; for playing with a gamepad) |
| `filter` | how the picture is scaled: `sharp` (default: even pixels, no blur), `nearest`, `linear` |
| `renderer = hw` | draw with the graphics card (Vulkan) instead of in software (`sw`, the default for now): the PlayStation's drawing is done again by the card, at `scale` times its resolution, and shown instead (movies in 24-bit colour still come from the software side) |
| `scale` | with `renderer = hw`: 1-8 times the PlayStation's resolution (default 1). A picture larger than the window is averaged down to it (supersampling) |
| `truecolor` | with `renderer = hw`: `1` draws without the PlayStation's 15-bit colour and dithering (the default above scale 1), `0` with them (the default at scale 1, where the picture is then the software renderer's pixel for pixel) |
| `present` | with `renderer = hw`: how frames reach the display: `vsync` (each waits for the display's refresh), `mailbox` (the newest frame waiting replaces older ones; presenting doesn't wait) or `immediate` (at once, tearing). Unset, `vsync` decides |
| `rumble = 0` | no gamepad rumble (it follows the game's vibration option otherwise) |
| `deadzone` | the gamepad sticks' dead zone in percent (default 12) |
| `gamecontrollerdb.txt` | not a setting: a file next to `twine.ini` in SDL's gamepad mapping format (as the community's gamecontrollerdb.txt), for gamepads SDL doesn't know |
| `controls = original` | the gamepad's buttons as the PlayStation pad's, set in the game's own options (by default they follow a modern layout in the missions, below) |
| `card1`, `card2` | memory card images (raw 128 KB, as emulators use). Slot 1 defaults to `~/.local/share/twine/card1.mcd`, made on first use; slot 2 to `card2.mcd` there if it exists; `none` leaves a slot empty |
| `autoload` | the memory card slot whose save the front end loads after the title screen (`1`, the default, or `2`; `off`): CONTINUE then goes straight to the missions |
| `hd = 0` | the PS1's own movies even where an HD clip is there |
| `infinite_grenades = 1` | a cheat: throwing a grenade leaves as many as there were |
| `devcheats = 1` | the developers' cheat codes work: on the pause menu in the missions (see Developer cheat codes below; SELECT SELECT CIRCLE CIRCLE then SELECT CIRCLE completes the mission), and the front end's own codes, which the retail game never checks (below) |
| `no_intro = 1` | skip the boot animation and title card |
| `level = N` | start straight in mission folder N (1-11) |

**Controls** (F1 shows them over the game). Keyboard and mouse: WASD move, the mouse looks (inside missions), left button fires, right button aims, E or F uses things (doors, people), R reloads, Space or the mouse's back button punches, G throws a grenade, Shift (held) sprints, 1-9 pick a weapon, Q goes back to the last one, Z/X or the wheel change weapons, C or Ctrl crouches, Tab shows messages, Esc or Enter pauses; arrows move and turn in missions. In menus (the phone's pause menu too): arrows, Enter or Space to choose, Esc or Backspace to go back. Keys are read by their place on the keyboard, so other layouts (AZERTY, ...) have them in the same places. Aiming (right button or L2) and strafing leans out, as in the original. The actions follow the game's own button settings in the options menu. Gamepad, in missions: the left stick moves, the right stick looks, R2 fires, L2 aims, Square reloads, Triangle uses things, Circle crouches, R3 punches, R1 throws a grenade, L3 sprints until the stick lets go, the d-pad's left and right change weapons, L1 goes back to the last one, Start pauses, Back or the touchpad shows messages; in menus it is a PlayStation pad. Its sticks have a dead zone (the `deadzone` setting, in percent, 12 by default), and the keyboard keeps working while a gamepad is connected.

**Save states**: F5 saves the mission as it is, F6 loads it again (from anywhere: the front end, another mission, a later session). One state is kept, in `~/.local/share/twine/states/`; a new version of the port can't load the old one's.

**A save with everything unlocked:** `tools/make_unlocked_card.py CARD OUT.mcd` takes a card with a save the game made (any progress) and writes a card whose save has all 11 missions available and completed, difficulty 007 (`--difficulty agent` otherwise), and all six bonus cheats unlocked for Options > Cheats (`--cheats-on` also switches them on). Saved as `~/.local/share/twine/card2.mcd` it is the card in slot 2: Options > Memory Card > Load > slot 2, then use CONTINUE (NEW GAME starts over). The script's comments describe the save format.

The title card's lettering is baked from the MGS1 Codec font, © 2025 Andrew Gleeson, licensed under [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/); the glyph data in `port/src/intro_font.h` is adapted from it and shared under the same license (`port/tools/bake_intro_font.py` re-bakes it).

For testing: `TWINE_HEADLESS=1` runs without a window, `TWINE_SHOT=frames` saves frames as PPM files, `TWINE_TEST_PAD`/`TWINE_TEST_STICK`/`TWINE_TEST_LOOK` stand in for the controls, `TWINE_PROFILE=1` samples where the time goes (`port/tools/profile.py`), `TWINE_FPSLOG=1` logs the frame rate and pacing every ten seconds, `TWINE_CD_LOG`, `TWINE_BIOS_LOG`, `TWINE_SMOOTH_LOG`, `TWINE_PAD_LOG` log more to `port.log` (written in the current directory), and `TWINE_CORE=1` turns a fatal error into a core dump (`2`: also a write where the console has no hardware, the mark of a stray pointer). With `TWINE_RENDERER=hw`, headless runs draw on the graphics card too (SDL's offscreen video driver): `TWINE_SHOT` frames are the card's picture at full resolution plus `screen_NNNN.ppm`, what a 1280x960 window would show; `TWINE_HW_COMPARE=1` (scale 1) compares every shown frame with the software renderer's VRAM and logs how much of it is identical (and writes `cmp_NNNN.ppm` for `TWINE_SHOT` frames: software, card, difference); `TWINE_HW_STATS=1` logs the renderer's work and its CPU and GPU time every 100 frames; `TWINE_HW_PROBE`, `TWINE_HW_LOG` and `TWINE_HW_DEBUG` are described in `port/src/gpu_hw.c`. With the hardware renderer the software GPU keeps what it is sent and draws it only when something reads that part of VRAM (`port/src/gpu.c`): `TWINE_SW_DEFER=0` has it draw everything at once, `TWINE_SW_DEFER=1` keeps it doing so under `TWINE_HW_COMPARE` (which then checks what it kept), `TWINE_SW_DEFER_LOG=1` logs what made it draw. `make -C port hwtest` builds `build/port/hwtest`, which checks the hardware renderer against the software one on random GPU commands, also those the game never uses (mask bit, texture windows, VRAM copies). The source files' header comments describe the rest.

## Setup

Arch/CachyOS packages: `ghidra`, `mips-elf-binutils`, `duckstation-qt-bin` (AUR), plus `cmake`/`gcc`/`python`.

```sh
python -m venv .venv && .venv/bin/pip install "splat64[mips]" capstone pycparser
# tools that are cloned or downloaded into tools/ (all gitignored):
#   tools/mkpsxiso   github.com/Lameguy64/mkpsxiso (build with cmake)
#   tools/old-gcc    github.com/decompals/old-gcc releases (2.8.1-psx at least)
#   tools/maspsx     github.com/mkst/maspsx (then: git -C tools/maspsx apply ../patches/maspsx-mflo-load-nop.patch)
#   tools/m2c        github.com/matt-kempster/m2c
```

The maspsx patch adds a nop real ASPSX inserts when a load sits between `mflo`/`mfhi` and a `mult`/`div` that doesn't read the loaded register (ASPSX always keeps two instructions between them).

Put your disc dump in `workbench/disc/` and extract it with `dumpsxiso` into `workbench/extracted/` (with `-s workbench/extracted/disc.xml`).

## Building

| command | result |
|---|---|
| `make` | build `LEVEL01.EXE` from source and verify its SHA-1 |
| `make split` | re-run splat (after changing `config/level01.yaml` or symbol names); for Level 1 this also re-applies `tools/make_shiftable.py` |
| `make disc` | full disc image with our `LEVEL01.EXE`, `build/disc/twine.cue` |
| `make play` | playable disc with the developer cheat codes unlocked in every level, `build/disc/twine_play.cue` |
| `make mod` | test disc with every mod in `mods/` (boots straight into Level 1), `build/disc/twine_mod.cue` |
| `tools/build_all.sh` | build all 14 executables and a disc from them, `build/disc/twine_all.cue` |
| `make cmod CMODS=TWINSTICK` | playable disc with C mods compiled in (see below), `build/disc/twine_cmod.cue` |
| `tools/make_boot_patch.py N` | print a patch that boots straight into level N |

Load the `.cue` file in DuckStation.

## Mods (`mods/*.patch`)

Each line patches one 32-bit word and checks the original value first. Patches written for Level 1 are carried over to every other level by matching the surrounding code.

- `debug_cheats.patch` turns the developer cheat codes back on. The retail game disables them through the CD-archive flag at `g_Args+0x1BC`.
- `boot_to_level1.patch` skips the front-end and boots straight into Level 1. It uses the boot stub's own "skip shell" switch.
- `debug_camera_page.patch` starts every level with the CAMERA INFO debug page showing.
- `invincible.patch` starts every level with invincibility on.
- `fov_wide.patch` widens the field of view (projection distance 300 → 240, about 81° → 94° horizontal). Weapon zoom still works on top of it.

To pick your own set of mods: `make mod MODS="mods/debug_cheats.patch mods/fov_wide.patch" MOD_NAME=twine_custom`.

### Developer cheat codes (with `debug_cheats`)

Enter these **on the pause menu**. Every code starts with **SELECT SELECT CIRCLE CIRCLE**, followed by:

| then | effect |
|---|---|
| L2 R2 | **open the hidden developer menu** |
| SQUARE SQUARE SQUARE SQUARE | toggle invincibility |
| R1 R1 L1 L1 | all weapons |
| L1 L1 R1 R1 | refill ammo |
| SELECT CIRCLE | complete mission (probable) |
| SELECT SQUARE | mission failed |
| R1 R1 R1 / L1 L1 L1 | object-cycling mode / next object |
| SQUARE LEFT SQUARE / CIRCLE CIRCLE CIRCLE | visual modes: objects tinted red / drawn as black silhouettes (from the code; not yet seen in game) |
| UP UP SELECT, DOWN DOWN SELECT, SQUARE RIGHT SQUARE, SQUARE SQUARE SQUARE SELECT, SQUARE CIRCLE SQUARE CIRCLE | not identified yet |

The front end has four codes of its own that the retail game never checks (the native port's `devcheats` setting turns them back on; enter them on the main menu): SELECT SELECT CIRCLE CIRCLE then **L1 L1 R1 R1** makes every mission selectable, **L1 ×7** shows every movie in the Movies menu, **L2 L2 R2 R2** turns on a sprite position editor that prints positions to the log, and **(SELECT R1 L2) ×5** sets a flag that skips ahead in the front end.

Unverified: after every code, the game rewrites the player's `+0xE` field (health?) depending on god mode. Turn invincibility on first if Bond dies right after entering a code.

The **developer menu** uses Up/Down to choose an option, Left/Right to change it and Start to close. It has these options:
- DISPLAY: 9 debug pages (framerate, camera, player position, object counts, profiling, and more)
- EDIT: CAMERA is a **free-fly camera** you move with the D-pad
- INVINCIBILTY (the developers' spelling) and IGNORE WALLS (noclip)
- DISPLAY HUD and DISPLAY OBJECTS on/off
- TERRAIN and SUBDIVISION rendering toggles

## C mods (`make cmod`)

Because the build is shiftable, mods can be ordinary C. `make cmod CMODS="TWINSTICK ..."` rebuilds **all 11 levels** with the mods (`tools/build_cmod.py`) and makes a normal disc (front-end included), `build/disc/twine_cmod.cue`. The matching build never sees mod code.

There are two kinds:
- **`src/mods/<name>.c`: works in every level.** A mod is written with Level 1's names. It lists the functions it replaces on a `MOD_REPLACES:` line, and can call the original as `<name>_orig`. For each level:
  - `tools/port_symbols.py` maps those names onto that level. Each Level 1 function is found in the other level by its address-masked instructions, then the symbols in matching instructions are paired up. The other levels have no C source; all the names come from this.
  - The level's own definition of a replaced function is made weak with `objcopy`, so the mod's version wins at link time, and gets an `_orig` alias.
- **`#ifdef MOD_<NAME>` inside the decompiled Level 1 source:** Level 1 only. Use this for small changes in the middle of a function.

| C mod | where | effect |
|---|---|---|
| `TWINSTICK` | `src/mods/twinstick.c`, every level | modern twin-stick controls (below) |
| `WIDEFOV` | `src/mods/widefov.c`, every level | wider field of view: projection 300 → 240, about 81° → 94° horizontal (`workbench/screenshots/16_*`) |
| `INVINCIBLE` | `src/mods/invincible.c`, every level | start every level with invincibility on |
| `DEVCHEATS` | Level 1 `Cheat_CheckCodes` (`#ifdef`) | developer cheat codes and menu (`debug_cheats.patch` does the same for every level on the matching build) |

For example, `make cmod CMODS="TWINSTICK WIDEFOV"`. Each mod must replace different functions, because two mods can't both replace the same one.

`mods/*.patch` files that patch a level EXE don't apply to C-mod builds, because the addresses have moved. Boot-stub patches still work. For testing, `tools/make_disc.py ... --replace SLUS_012.72=<stub with boot_to_level1> LEVEL01.EXE=build/level01_cmod/level01.exe` skips the front-end.

**`TWINSTICK`: modern twin-stick controls** (every level)
- **Left stick:** moves forward/back and **strafes**.
- **Right stick:** **turns**, and **looks up and down while you move**. The retail game ignores the right stick completely, and only lets you look up and down in aim mode (hold R1), where you can't move. The look direction follows the game's REVERSE SNIPER option, the same as aim mode. In aim mode the right stick aims.
- **Analog mode:** turned on automatically. The retail game keeps the pad in digital mode unless the front-end's analog option is set (bit 8 of `g_Args->flags2`).
- **Code:** three functions in `src/mods/twinstick.c`:
  - It wraps the input mapper (`func_8005D760`), which runs every frame and turns the pad into movement flags. Afterwards, the left stick X sets the strafe flags and the right stick X sets the turn flags.
  - It wraps the on-foot update (`func_80061DC0`). The game sets the player model's pitch from the ground slope there, and the camera and firing follow that pitch; the mod adds a look offset on top, driven by the right stick Y.
  - It replaces `Pad_StickToSpeed` with a version that reads the right stick.
- **Verified in DuckStation** in Levels 1, 2 and 4. With retail code, the right stick does nothing and the left stick turns; with the mod, the right stick turns and the left stick strafes. See `workbench/screenshots/10_*`, `11_*`, `14_*` and `15_*`. Level 11 boots into analog mode with the mod. Looking up and down was tested by hand with a DualSense.
- **In DuckStation:** set the controller type to **Analog Controller (DualShock)**. A DualSense maps the way you'd expect.

How controls work in this engine, for further mods:
- `Pad_Update` reads the pad. In analog mode (pad ID 0x73) it sets `D_800B0E54 = 1` and copies the sticks to `g_PadAxes` = [right X, right Y, left X, left Y], centred on 0x7F.
- `func_8005D760` maps them to intent flags. Left stick Y: forward `D_800DDC44` / back `D_800DDCC0`. Left stick X: turn left `D_800DDCC4` / turn right `D_800DDDB8`, and `D_800DE0DC` (signed). Button map (`g_Args->buttonMap[8]`): fire `D_800DDEEC`, strafe left `D_800DDD8C`, strafe right `D_800DDD90`, crouch `D_800DDD94`, sniper `D_800DDDB4`, and others.
- Movement (`func_80061B70`) is digital: ±0xE1 per frame, or ±0xA0 on diagonals. Turning (`func_800617AC`) is analog, through `Pad_StickToSpeed(0, 0x50)`: 3-unit dead zone and a quadratic curve. Aiming (`func_80043EE0`) uses `Pad_StickToSpeed` with per-weapon speeds (Weapon fields `0x53` yaw, `0x58` pitch).

## Data mods: level archives, textures, enemies, AI scripts

Each level's assets are in one archive, `Lnn/LVLnnCD.BIN`. The game looks files up by name through the archive's table, so files can be replaced and can change size. This was verified by growing a file and moving every file after it: Level 1 still loads and plays.

```sh
tools/cdbin.py list    workbench/extracted/L01/LVL01CD.BIN
tools/cdbin.py extract workbench/extracted/L01/LVL01CD.BIN workbench/cdbin/L01
# edit files in workbench/cdbin/L01, then
tools/cdbin.py pack    workbench/extracted/L01/LVL01CD.BIN workbench/cdbin/L01 build/LVL01CD.BIN
tools/make_disc.py build/disc/data.xml --replace LVL01CD.BIN=build/LVL01CD.BIN   # then mkpsxiso as in the Makefile
```

Repacking unmodified files gives a byte-identical archive. The archive starts with a table of 0x1C-byte entries (`char name[26]; u16 start_sector`), with 2 KB sectors.

What's inside (Level 1):

| file | contents |
|---|---|
| `KG011.INI`, `KG012.INI`, `KG013.INI` | **plain-text** setup for the level's three sections. **`maxguys = 6`** caps how many characters are active at once. Raising it to 12 loads and plays fine, but the opening scene has the same guards, so the guards themselves are placed by the map and scripts (probably `DATA01.BIN` and `AIB01.BIN`). `[guy_slot n]` picks the character model and `[tex_slot n]` its texture (ski guards, Elektra, Bond). |
| `L01/AIB01.BIN` | **AI scripts**: bytecode for the game's script VM (dispatcher `func_800575FC`, 0x44-opcode table at `0x8009D7A8`). The EXE still contains the development file names `GOON.AIB`, `SNIPER.AIB`, `PILOT.AIB`, `DIRECTOR.AIB`, `ELECTRA.AIB`, `BONDNIS.AIB`. A disassembler for this bytecode would make enemy behaviour editable; the opcode table is in [`docs/ai_script_vm.md`](ai_script_vm.md). |
| `L01/LVL01TIM.BIN` | 80 TIM images: **HUD, weapons, crates, vehicles, effects**. Tinting them shows up in game (`workbench/screenshots/13_*`). |
| `L01/L01S1-3.TIM`, `.TRN`, `.TRK` | per-section texture page (8-bit), terrain and track data |
| `L01/TEX01x.BIN` (`TEX PSX`), `L01/GUY01x.BIN` (`GUY `) | character textures and models |
| `L01/*.HMD` | Sony HMD models (weapons, vehicles) |
| `SHDATA/LOAD_01.TIM`, `LOAD1.TIM` | loading screen and "Loading" bar |
| `SOUNDFX/L1FX.VH/.VB` | sound effect bank |

How enemies get into a level (for an enemy-count mod; still to be done):
- Map entries are instanced by `func_8003C1BC(entry, x, y, z)`.
- The word at `entry+0xC` holds the object type in its low byte, with bit 8 set once the object is instanced. The type indexes a table of 0x70-byte type records at `g_Args->unk228`; byte 1 of a record is the object class, and class 6 is a character.
- `func_8003C6B0` creates the object from a fixed pool. It returns nothing when the pool is full, which is presumably what `maxguys` limits.
- Characters live in a 0x124-byte array at `D_800AFFC8`. Each gets its AI script through `func_80053790(obj, n)`, with `n` taken from bits of `entry+0x10`.
- So more enemies means more map entries (they come from the level data; which file is still to be worked out) plus a higher `maxguys`.

## Shiftable build (writing mods in C)

The original game only works if every function and variable stays at its original address, because compiled code and data hold absolute addresses. After `make split`, `tools/make_shiftable.py` rewrites the generated assembly so every address is a symbol the linker fills in:

| fixed address in splat output | rewritten as |
|---|---|
| switch jump tables in `.data` (raw `0x8002....` words) | references to `jtgt_<addr>` labels added in the function's asm |
| absolute linker symbols inside the EXE | `nearest label + offset` |
| BSS variables | `bss_anchor + offset`, a label at the exact end of the data (the zero padding at the end of the EXE is now added by `tools/fix_exe.py`) |
| `lui`/`lw` pairs splat did not match up | `%hi`/`%lo` of the symbol (found with a control-flow analysis per function) |
| the hand-written GTE polygon module that lives in `.data` (after the `"POLY.c"` string) | its absolute `j`/`jal` become label references. For its `lui`/`ori` pairs the linker computes both halves (`.word opcode + poly_addr_<addr>_hi`), because `%hi` assumes a sign-extended low half and `ori` doesn't sign-extend. |
| header entry point and text size | the entry function's symbol and a size computed by `tools/fix_exe.py` |

The matching build is unchanged byte for byte, so `make` still checks the SHA-1. To see that shifting works, insert some bytes early in `.text` (for example `__asm__(".section .text\n.space 0x1000\n");` at the top of `src/level01/text_80026B00.c`), run `make build/level01/level01.exe` (the SHA-1 check will fail, as expected) and boot the result.

Caveats:
- The `mods/*.patch` files patch fixed addresses. Use them only on the matching build; with a shifted build, write the change in C instead.
- Decompiling a function that uses a switch also means moving its jump table out of `asm/level01/data/` (the table refers to that function's `jtgt_` labels).
- All 11 levels are shiftable (`SHIFTABLE` in the Makefile) and still match. Levels 1, 2 and 6 were play-tested with 4 KB inserted at the start of `.text`. C mods in `src/mods/` work in all of them (see C mods above).

## Notes for modders (Level 1 addresses)

Every one of these is shared engine code, so it's in every level at a shifted address.

**Camera**
- Struct at `0x800AFFE4`:
  - target X/Y/Z: `+0x04`/`+0x08`/`+0x0C`
  - rotation X/Y/Z: `+0x10`/`+0x12`/`+0x14` (s16, 4096 = 360°)
  - position X/Y/Z: `+0x48`/`+0x4C`/`+0x50`
- The player's heading is `gPsi` at `0x800AFF9C`.
- The camera-mode update functions are clustered at `0x8004DB00`–`0x80050A00`.

**Field of view**
- `g_Projection` (u16, `0x800B0090`) is set to 300 in the graphics init (`0x8002F6A4`). Lower values give a wider view.
- It's written to the GTE `H` register by `func_8004DFA8` (leaving scope). `func_8004E07C` adds weapon zoom on top.
- The game renders at 512×240.

**Controller**
- Button bits use PsyQ's layout. Held, previous and newly-pressed buttons are at `0x800B0E44`, `0x800B0E50` and `0x800B0E4C`.
- Analog axes are a `u8[4]` at `0x800B0E5C`, in the order [RX, RY, LX, LY], with 0x7F as center.
- **Only the left stick is used.** The right stick is read every frame but nothing uses it, so twin-stick look is a natural mod.
- Stick-to-speed curve: `func_8004472C` (deadzone 3, quadratic, compensates for frame rate). The on-foot controller is `func_80043EE0`.
- Button mapping: `g_Args+0x148` holds a `u16[8]` for use weapon, prev weapon, next weapon, reload, crouch, sniper, strafe L and strafe R. The defaults are Cross, Triangle, Square, Circle, L1, R1, L2 and R2, set by the boot stub.

**Weapons**
- `g_Weapons` at `0x800B00A4`: six 0xB8-byte slots.
  - `flags`: `+0x38` (`|= 0x1800` = owned)
  - ammo: `+0x44`
  - clip: `+0x48`
  - reserve: `+0x4A`
  - clipSize: `+0x50` (u8)
  - maxAmmo: `+0x5C`

**Player**
- The player object pointer is at `0x800B0B00` (health at `+0xE`, 0x7FFF = full).
- The world position is copied into `g_Args+0x208`/`+0x20C`/`+0x210`.

## Layout

```
config/       splat configs for all 14 executables, symbol names (symbol_addrs.level01.txt), checksums,
              level01_syms.ld (absolute symbols the C needs, e.g. scratchpad addresses)
src/level01/  decompiled C: one file per optimization region (text_<addr>.c)
src/mods/     C mods that work in every level (make cmod)
asm/<exe>/    splat output, made shiftable by tools/make_shiftable.py (regenerated by make split)
include/      common.h (types, m2c macros), twine.h (game structs and globals), gte.h, psyq.h
mods/         word patches for the matching build
docs/         reverse-engineering notes: functions.md (what matched functions do, struct layouts, frame
              timing, camera/aim), ai_script_vm.md
tools/        build and analysis scripts; Ghidra scripts in tools/ghidra_scripts
workbench/    your disc, BIOS, extracted files, screenshots (never committed)
```

Main tools:
- **Matching:** `auto_match.py` (m2c + compile + compare, with fixups), `match_test.py` (try one function), `permute.py` (decomp-permuter on near-misses), `integrate_one.py` (put a match into the source), `progress.py`
- **Building and mods:** `make_shiftable.py`, `fix_exe.py`, `build_cmod.py`, `port_symbols.py`, `port_level.py` (Level 1's C into the other levels), `twin_port.py` (a matched function's C into levels where it differs only in constants), `jtbl_fix.py` (switch jump tables), `apply_mods.py`, `make_boot_patch.py`, `make_disc.py`, `build_all.sh`
- **Data:** `cdbin.py` (level archives)

## Workflow for decompiling a function

1. Pick an `INCLUDE_ASM(...)` line in `src/level01/*.c`.
2. Get a starting point with `.venv/bin/python tools/m2c/m2c.py -t mipsel-gcc-c --globals=used --valid-syntax asm/level01/nonmatchings/<file>/<func>.s`, or from the Ghidra decompiler.
3. Iterate on the C in a scratch file named `<func>.c`, using `tools/match_test.py <file> --gcc 2.8.1-psx --opt=-O2 --diff`.
4. Replace the `INCLUDE_ASM` line with the C and run `make`. It must still print `OK`. `tools/integrate_one.py <func> <file.c>` does this and keeps the change only if the build still matches; with `--nonmatching` it banks a near miss under `#ifdef NON_MATCHING` instead (build a disc that runs those with `make cmod CMODS=NONMATCHING LEVELS=level01`).
5. After matching more functions, run `tools/twin_port.py levelNN` for each level (copies of the function that differ only in constants or addresses, such as struct offsets and array sizes), then `tools/port_level.py port levelNN` for levels 2–11 in order (identical copies).

`tools/auto_match.py` automates steps 2–4 for functions that m2c gets right on the first try. It also retries common m2c mistakes:
- pointer strides
- signedness
- `void` returns
- `register` parameters

`tools/permute.py` runs [decomp-permuter](https://github.com/simonlindholm/decomp-permuter) on near-misses and integrates any that reach score 0.

Matching tips learned on this codebase:
- **Unoptimized regions** often use `register` parameters.
- **GTE code** is written with inline-asm macros (`include/gte.h`). Include the explicit `nop` after `mfc2` reads.
- **Call `abs()` directly.** GCC expands it as a builtin.
- **Look for `volatile`.** It shows up as reloads inside loops that would otherwise be hoisted.
- **Typed structs often fix a mismatch.** Writing a struct with an array field (`obj->vars[i]`) instead of raw pointer maths sometimes flips the operand order.

## Ghidra

The project is in `tools/ghidra_projects/TWINE`. It contains all 11 levels (in `levels/`) plus the shell, movie player and boot stub (in `system/`), all imported with [ghidra_psx_ldr](https://github.com/lab313ru/ghidra_psx_ldr).

Names from `config/symbol_addrs.level01.txt` are applied with `tools/ghidra_scripts/ApplySymbols.java`.
