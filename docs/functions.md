# What the functions do (LEVEL01)

Notes gathered while matching, for naming and for the port. Addresses are LEVEL01's. Where a function has
been renamed, the current name is used.

## Text, message boxes, menus

- `func_80028958(text)`: strlen of the text minus the number of `_` characters.
- `func_8002872C(text, x, y)`: adds a text entry. Uppercases the text unless `D_8009C1ED` is set, uses
  entry number `D_800AC54C`, stores x/y at +0xC/+0xE of the 0x60-byte entries in `D_800AB744`, returns the
  new count.
- `func_80028858(text, *x, *w)`: text width in pixels (glyph table `D_800AE940`, 8 bytes per glyph, width
  byte at +2, plus spacing `D_800AEDD4`); sets `*x` so the text is centred on the display width.
- `func_80029504(s16 *c)`: maps control codes 5, 7 and 12–31 to special glyphs 0x5F–0x71 (button icons?).
- `func_80029868`: opens a message box (`D_800AC284`, layout below).
- `func_8002A3C8(centered)`: draws the message box text line by line. `\n` ends a line, `#` is a space, `\t`
  is a page break (advance and return 0). Lines are 10 px apart. Returns 1 when the text is finished.
- `DevMenu_Draw`: cursor bar (x 0x14–0x112, y = cursor*8+0x18) and 12 lines of "label value"; labels from
  `D_8009CEE0`, values index the name tables in `D_8009CEB0`, a negative value prints `D_800A7A24`.
- `func_800411BC(*x, *y, step)`: controls screen helper; for each of the 8 `Args.buttonMap` entries finds
  the mask in `D_800A8390` and prints the name from `D_800A8370`.
- `func_800417C4`: pause menu per-frame update. Start or a cheat code closes it (`func_800414AC`). Confirm
  dispatches on the cursor `D_800B0794`: resume, objectives `func_80042A94`, inventory `func_80042E34`,
  messages `func_80041A38`, sound `func_800433F4`, options `func_8004218C`, restart `RestartPrompt_Open`,
  abort (`unk115 = 1`, `flags |= 0x80`, then `RestartPrompt_Open`). Back closes.
- `func_80041A38`: messages list from `D_800D4444` (12-byte records), `D_800A8420` if empty; update
  handler `func_80041BA0`.
- `func_80041C28`: opens the selected message (`D_8009CFB4 = 1`; clears `D_8009CFC4` if it is the current
  message `D_800D4468`); body via `func_800419CC`; update handler `func_80041CF0`.
- `func_80041DBC`: one-line prompt `D_800A842C`; `D_800D4410 = 0`, `Args.flags |= 0x40`; update handler
  `func_80041E78`.
- `func_80042D1C`: objectives screen update. Back → `PauseMenu_Open`; confirm →
  `func_8006981C(D_800D4474[cursor])`, redraw via `func_80042B28` (only when `D_8009CA8C`).
- `func_80042E6C` / `func_80043078`: inventory draw / update. Lists owned weapons (flags bit 11) into
  `s8 D_800D446C[]`; confirm sets `D_8009EEC4` = chosen weapon; button 0x80 toggles flags bit 12.
- `func_80043200`: controls screen draw (update `func_800433A8`).
- `func_800433F4`: sound screen; clamps `D_8009CFB8` to 0..0x68, shows `Args` s16 at +0x140 and +0x13E
  (volumes?); update `func_80043610`.

## Files, CD, levels

- `func_8002A61C(name, *out)`: finds a file on the CD: drive mode 0x80, waits 3 frames on
  `g_Args->unk2DC` (vblank counter), `*out` = 2 if not found, else `func_800834D0(&file)`.
- `func_8002B0A0`: CD command 9 (pause?) up to 1000 tries, then clears `g_Args->flags` bit 3 and resets
  the CD-audio state.
- `func_80030118`: loading screen progress bar (grows the bar prim `D_800D2BC4` on some frames).
- `func_800303B4(mode)`: moves the stack between the scratchpad (0x1F8003D0) and the RAM stack top
  `D_800D2E74`. `func_80026800` ≈ GetSP, `func_800855BC` ≈ SetSP.
- `func_80030D08(mode, buf, name)` / `func_80030DA0(data, len, mode)`: load a config file and parse it line
  by line; mode 5 sends each line not starting with `;` to `func_80030B04`, otherwise lines starting with
  `.` go to the empty stub `func_80061700`.
- `func_800310B4(base)`: level data fixup: header offsets 0x0–0x14 → pointers, init calls, choose the
  player start (restart record if `D_8009CE28`, else `D_8009E9C4+0x10`, else `D_8009E9C4`), shift two
  object lists by the change in world origin (`D_800D2FC0+0x18` vs saved `D_800D2FB0`), then
  `D_800D2F8C` and `D_8009CE44`.
- `func_800314A0`: checkpoint/restart: saves {id, gPsi, player X/Y/Z} to `D_800D2F94`, reloads level files
  named from (level, level, id), loads a third file into `D_8009CE38` (fatal if over 0x1800 bytes).
- `func_80048DE4(name)`: 0 if `strlen(name) < 35`; else, when not in CD-archive mode,
  `sprintf(Args.exePath, D_800A85D4, name)` + `func_8006B230(0, exePath)` and returns 1.

## World

- `func_8003FF9C`: distance cull over the 0x44-byte records at `D_8009CF7C` (count `D_8009CF80`):
  anything more than 0x7D00 from the player in X or Z is deactivated.
- `func_800406E0(*idx)`: next active record in that array, wrapping.
- `func_80040778(v0, v1, v2, p)`: point-in-triangle by angle sum (acos `func_800365B4`, 4096 = 360°) of
  normalized vectors (`func_80040978`); true if the sum ≥ 0xF3C.

## Player, camera, timing

- Frame timing: `VSync(60 / g_FrameCap)` ends each frame (`func_8002F7CC`), `g_FrameCap = 20` (set in
  `func_8002C844`). The vblank callback `func_80026914` counts vblanks in `g_Args->unk2DC` and sets
  `g_FrameRate = 60 / vblanks per frame`. Only ~22 functions compensate for slowdown (most against 20,
  `Pad_StickToSpeed` against `g_FrameCap`); AI, animation and timers count frames.
- Per frame: `func_80079E10` = input mapper `func_8005D760`, then the on-foot update `func_80061DC0`
  (turn `func_800617AC`, move `func_80061B70`), or the aim update `func_80043EE0` while aiming
  (`D_8009CD4C`), or `func_80053B64` in cut scenes.
- `func_800617AC` sets the player model's pitch (s16 at `(g_Player+0x90)->+0x60`) from the ground slope
  every frame. The on-foot camera `func_8004DA60` uses camera pitch = model pitch − 0x17, yaw = `gPsi`.
- Aim mode: `func_8004DD9C` enters it (aim pitch `D_800B0C64` = model pitch − 0x17, aim yaw `D_800B0C5C` =
  `gPsi`, camera position saved in `D_800D9200`); `func_80043EE0` moves the aim (pitch −0x280..0x320) and
  zoom (`D_800B0C74`); `func_8004E07C` applies it to the camera and FOV; `func_80044898` leaves it
  (`gPsi` = aim yaw). Firing (`func_80076CDC`) aims along the model pitch/yaw, or the aim pitch/yaw.
- `func_8004F3E8`: camera springs: position and rotation chase their targets, scaled by `g_FrameRate`.
- `func_8004D978`: per-frame camera: by camera mode `D_800B0A94` (14 → on-foot `func_8004DA60`, 7 → aim
  `func_8004E07C`), then `func_8004DBCC`, `GsSetRefView2(&D_800B0C10)`, `GsGetLs(D_800D3238, D_800B0808)`;
  `D_8009CD30` = view yaw.
- `func_80049A88`: crosshair: two LINE_F2 per buffer (`D_800D7C24`); red at the aim point
  (`func_800498E4`) when `D_800DDE30`, else green at screen centre.
- `func_80076BF8(arg0, arg1, type)`: fires a projectile if the weapon has ammo (`func_80039A9C`), clip--,
  `D_800D9208` = weapon+0x5C >> 26.
- `func_8005E228(obj)`: player damage/death. Skipped with `g_GodMode`/`D_8009CF54`. Health (+0x34) ≤ 0 →
  death (`func_800509E4`); otherwise a hurt flash, plus rumble (`func_8002F564`) if the vibration option
  (`g_Args` byte 4 bit 7) is on.
- `func_8004CA5C(obj, arg1)`: pickup: tries `func_8004C768`, `func_8004C824`, `func_8004C82C`,
  `func_8004C944`, `func_800693DC` in turn, shows the message (`func_80048B4C`), removes the object.
- `func_800693DC(obj)`: pickup for weapon slot 2.
- `func_80069478`: resets per-level globals (current weapon `D_800B0DD4`, HUD, player position).
- `func_80069330`: level defaults (object type ids into `D_8009D104..`).
- `func_8007567C(obj)`: draws an object's model (GsGetLwUnit/GsGetLsUnit, GsSortUnit into the OT).
- `func_8004B0B0`: initialises two pairs of 0x28-byte semi-transparent primitives.
- `func_8005EB6C`: angle-sector bitmasks (64-unit sectors) in `D_800DE0D4` / `D_800DE0C4`.
- `func_80061FB4`: effect templates (0x14 bytes) in `*D_8009D8B8`: white/black smoke, bullet holes (plain,
  glass, wood, 2), fireball.

- `func_800321F0(on)`: night-vision tint over the colour list at `D_800D2FC0->+4` (0x14-byte entries,
  colour at +0xC): green = (r+g+b)>>3 + 0x30; off restores. Then `func_8006ECBC` / `func_8006F034`.
- `func_800324D0(tim, x, y)` / `func_80032768(buf)`: upload one TIM at a new position / a chain of TIMs
  (GsGetTimInfo, LoadImage `func_8007F2EC`, DrawSync `func_8007F0D8`, LoadClut).
- `func_80032E4C(row, col)`: POLY_F4 highlight box (`D_800D308C`, one per buffer) into the current OT
  (`D_800B8EFC` per-buffer 0x14-byte struct, OT at +4; `D_800B8F24` = active buffer).
- `func_80035030(a, b)`: 8-way sector of the heading difference between two objects.
- `func_800350F0(obj)`: object lifetime (+0x58 lifetime, +0x2C start frame, `D_8009CDA8` frame counter) and
  per-type updates (0x25, 0x2C/0x2E, 0x28).
- `func_80035BBC(obj)`: raycast along the velocity (`func_800746D0(D_800E2F2C, ...)`); hit point in
  scratchpad 0x1F8003D4.., normal to obj+0x11C, slides the velocity. `func_800359EC` records hits.
- `func_800361BC`: player damage (skipped when invincible / god mode / cut scene).
- `func_80036670(obj, tgt)`: aim point `D_800DDDCC`.
- `func_800368A8`: resets `g_Weapons[6]`.
- `func_8003B940(obj, x, y, z, model, count)`: allocates the object's model parts (0x8C each: GsDOBJ2 +
  GsCOORDINATE2 at +0x10 + SVECTOR rotation at +0x60), hierarchy from `D_800D86E0` (0x7E-byte rows),
  linked with GsLinkObject4.
- `func_8003BFB8`: spawns entries (0x18 bytes, groups `D_8009CF70[i]`, counts `D_8009CF6C[i]`) within range
  `D_800E2DE0` of the player via `func_8003C1BC` (this is how enemies appear).
- `func_8003FC60`: clears the `D_8009CF7C` list, then `func_80058DFC(&D_800DDA40)`.

- `func_800289C0(idx, text)`: replaces a text entry's text (padded to the old visible length), redraws via
  `func_800291B0`. Text entries (`TextEnt`, 0x60 bytes at `D_800AB744`): +0 w, +0xC x, +0xE y, +0x10 text.
- `func_8002D708`: per-frame mission end / restart: timers `D_8009CACC`/`D_8009CAD0`, then the restart
  prompt (`unk115`) or `g_Args->flags |= 0x80`.
- `func_8004EF44`: per-frame end of level, by `g_CutScene`: 1 → `func_8004EC70`; 2 (success) → music
  `D_800E23E8`, fade, `D_8009CA90 = 1` after 20 frames; 3 (failure) → text `D_800A88D0`, music by health.
- `func_8002DD6C(obj)`: applies the object's scale (s16 obj+0x6A, 4096 = 1.0) to its bounds and part
  positions (hierarchy rows `*D_800D86E0`, index def+0x5F).
- `func_80032648(tim)`: uploads one TIM (magic 0x10; LoadImage, LoadClut/LoadClut2, DrawSync).
- `func_800378FC(obj, anim, off, arg3)`: one of 6 animation slots (`AnimSlot`, 0x24 bytes at `D_800D30D8`).
- `func_80039520(obj)` / `func_8003A71C(obj)`: free / assign the object's `AnimEnt` (index at obj+0xA9).
- `func_8004D5A8(data)`: parses "TT" animation data into `ListSet D_800D8C7C[]` (≤ 20 sets of ≤ 8 lists).
- `func_800757C0(obj, anim, arg2)`: starts animation `anim` on every track in obj+0x70 (count obj+0x6C).
- `func_800481B0`: initialises `POLY_FT4 D_800D4984[2][32][2]` (semi-transparent).
- `func_8004900C`: health feedback: level = 8 − health >> 14; sounds on change (`func_80026F30`); flashing
  `D_800D7BE8`. Player obj+0xA4 = 2 when dead.
- `func_80058694(type)`: allocates an effect from the templates `*D_8009D8B8`; pools of 30 × 0xA0 bytes at
  `D_800DB884` / `D_800DA5C4`; free slot when byte +0x9B == 0.
- `func_80076F24(obj, idx)`: stops the object (velocity +0x98..+0xA2 = 0), `func_80065614(obj, 1, 5)`,
  sound 0x43, removes it.

- `func_80069B74`: per-frame tick (countdown `D_800E23F8` → `func_8006A3E0`; then `func_8003801C`,
  `func_80069AB4`).
- `func_80066098` / `func_800662A8`: spawn up to 10 effects across an object's bounding box (s16 min/max
  pairs at +8..+0x12). `func_80065100`: generic spawner (count s16 at +0x18, scale at +0x1E).
- `func_8006C8F4(arg0, ctx, idx)`: DR_TWIN texture-window prims around a POLY_GT3 (scrolling UVs).
- `func_8006F034` / `func_8006EEA8` / `func_8006ECBC`: refresh the terrain's gouraud colours from the colour
  table: plain; grey for unlit (flag `D_8009CABE`); night-vision green (flag `D_8009CABD`). Terrain mesh
  `D_800E2DA8`: +4 face count, +0x1C colour table, +0x20 faces (20 bytes), +0x30/+0x34 POLY_GT3 buffers.
- `func_8006FA54(base)` / `func_8006F9A4(hdr)`: relocate terrain data offsets; colour into `D_800B8FB0`.
- `func_8006DD20(a, b, c, out)`: normal = normalize(a−b) × normalize(c−a) (`func_8008C5D8` ≈ VectorNormal).
- `func_800755BC(obj)`: object animation countdown timer (+0x80).
- `func_80070070(...)`: floor finder: of the faces with a downward normal, the one whose height
  (`func_8006FED4`) is closest to pos.y. `func_8007240C`: collects colliding faces (`func_80071638`,
  `Vec_Dot`, `func_80072190`).

## AI scripts

- `func_800571B8(cmd, ScriptObj *, actor)`: opcode: jump/restart (pc = base + cmd[1]*4), clears the list at
  +0xA8 and vars selected by mask +0x1C, resets the actor's animation; returns 2.
- `func_80057B2C(anim, a, b, c, d)`: sets animation fields (+0x10/+0x14/+0x12/+0x16), recomputes lengths.

## Struct layouts

- `g_DevMenu` (0x800D3024): +0x04 s32 cursor; +0x38 s32 values[12].
- Message box `D_800AC284`: +0xF4/+0xF6 s16 w/h, +0xF8/+0xFA s16 x/y, +0xFC s16 timer (0x1E),
  +0x104 s32 mode, +0x108 u8 active, +0x109 char text[300], +0x238 char *read pointer, +0x23E/+0x240 s16
  colours.
- Smart objects at `D_800AFFC8`: 0x124 bytes; +0x00 definition pointer; +0x04 u8 active; +0x07 type & 0x3F;
  +0x0B part count; +0x34 health; +0x4C/+0x64/+0x68 flags; +0x7C attribute; +0x88 owner; +0x90 parts
  (0x8C each, position at +0x28/+0x2C/+0x30, rotation s16 at +0x60/+0x62/+0x64); +0x98/+0x9A/+0x9C s16
  velocity; +0x10C hit records; +0x114 model; +0x11C SVECTOR normal; +0x122 u16 collision flags.
- `D_8009CF7C` records (0x44 bytes): +0x00 u8 active, +0x10/+0x14/+0x18 s16 position, +0x1C object pointer.
- `D_800D4444` message records (12 bytes): +0 char *title, +8 s32 id.
- Weapon: +0 name string; flags bit 11 = owned, bit 12 = inventory toggle.
- ScriptObj: +0x10/+0x18 flag words, +0x1C var-clear mask, +0xA0 script base, +0xA4 pc, +0xA8 list
  {count; n; {s32, s32} items[]}, +0x1B0 word, +0x1CC flags.
- Weapon +0x5C: a u32 bitfield (top 6 bits are a value).
- Known wrong prototypes (callers work around them with casts): `func_8004C824` takes a pointer;
  `func_80044BAC`/`func_80044BBC` colours are u8; `ratan2` is `s32 (s32, s32)`.
- `Menu` (`g_MenuUpdateFunc`, 0x800D441C): +0x04 is the "resume" row index; the real cursor is
  `D_800B0794`. Neighbouring globals (0x800D439C–0x800D4474) are probably part of a larger menu struct.
