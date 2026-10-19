# Native PC port: plan

Draft for review. Decisions that need the owner are marked **Decide**.

## Where the code is

- **Level 1:** every game function except the two startup stubs (`start`, `func_80026800`) has C. 774 of 866
  match the original exactly; the rest are near misses under `#ifdef NON_MATCHING`, which were checked in
  DuckStation: a disc built with them boots, loads Courier and plays.
- **Levels 2–11:** each builds byte-identical with 419–441 functions taken from Level 1's C, plus 63–70
  near misses. What is left per level is mostly that level's own code (about 50 KB in Level 2) and engine
  functions that differ from Level 1 only in constants.
- **PsyQ 4.4 libraries:** 580 identified functions (libgs `GsTMD*`/`GsSortObject4`, libgte, libgpu, libsnd/
  libspu, libcd, libetc/libpad, libapi events, libc). They stay asm in the PS1 build; the port replaces them.
- **Data:** each level's initialised data is one asm blob, already "shiftable": every pointer in it is a
  symbol, so it can be re-emitted for another target.

## What the port needs

1. **A host build of the game C.** Compile `src/levelNN` for x86 (`-DNATIVE_PORT`, which empties the `REG()`/
   `KEEP()` matching hints). The code stores pointers in 32-bit struct fields and uses fixed PS1 addresses
   (the 1 KB scratchpad at `0x1F800000`), so the first build should be **32-bit** (`-m32`) with the
   scratchpad mapped at its fixed address; 64-bit can come later.
2. **GTE in C.** `include/gte.h` is inline MIPS asm; the port needs a software GTE behind the same macros
   (RTPS/RTPT/NCLIP/MVMVA/OP/AVSZ…, fixed-point and flag behaviour matter for culling).
3. **Graphics.** The game builds PS1 GPU primitives (POLY_FT4, POLY_GT3, SPRT, LINE_F2, DR_MODE…) into
   ordering tables and hands them to libgpu/libgs. Least risky: implement libgpu/libgs on the host and
   draw the primitive packets with a small **PS1-GPU-style renderer** (textured/gouraud triangles, CLUTs
   from a 1024x512 VRAM image, semi-transparency modes), so no game rendering code changes. Higher
   resolution and widescreen fall out of rendering at a larger size; the wide-FOV mod already exists.
4. **Audio.** libsnd/libspu: SEQ/VAB music and SFX (SPU ADPCM decoding plus a mixer, or a reimplementation
   of the sequencer), and CD-XA streamed audio.
5. **CD and video.** File reads from the user's own extracted disc; STR movies need an MDEC decoder.
6. **Input.** The pad path (`Pad_Update`, `Pad_StickToSpeed`) on SDL gamepads, plus **mouse and keyboard**
   (required): mouse deltas straight into yaw and pitch, reusing what the twin-stick mod does for look.
7. **Frame rate (required: high/uncapped).** Game logic runs at 20 Hz (`VSync(60 / g_FrameCap)`, frame
   counters everywhere). Keep the simulation at a fixed 20 Hz tick and **interpolate rendering** between
   the last two ticks (camera and object transforms), so speed and timing stay correct at any FPS.
8. **Saves.** Memory card functions to files.

## Proposed order

1. Host build of Level 1 with stub libraries, running headless to its main loop (finds 32-bit and
   address assumptions early).
2. Software GTE + libgpu renderer: first visible frames of Courier.
3. Pad input, then CD file access, then audio.
4. The other levels; shell (front-end menus) and movie player.
5. 60+ FPS interpolation, mouse and keyboard, widescreen/high resolution.

## Decide

- **Renderer backend:** SDL2 + OpenGL 3.3 (simple, portable) is the suggestion; Vulkan or SDL_GPU possible.
- **Level structure:** each level EXE is its own copy of the engine. Simplest: one host module per level
  (like the original EXEs, loaded by the front-end), unifying the engine later. Alternative: one engine
  built from Level 1's C, with per-level differences patched in (more work up front, smaller result).
- **Licensing:** emulator GTE/SPU code (DuckStation, Mednafen) is GPL or non-commercial; writing our own or
  using permissively licensed parts keeps options open. Game data always comes from the user's disc.
- **Near misses:** keep porting with them (they run in DuckStation), or require exact matches first for
  code the port depends on. O8 found one real bug in a banked function (value read instead of address),
  so `tools/difftest` checks are worth running on the important ones.
