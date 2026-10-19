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

## Sound

- Sound table at `g_Args+0x130`: 0x69 entries of 8 bytes {u8 prog, tone, note, voice, vol, rnd, pad[2]}.
- `func_80026D08(id, flags)` / `func_80026F30(id, flags)`: play a sound effect (volume `g_Args+0x144`,
  scaled by `+0x138` when flags & 2, and by the entry's volume); returns the voice.
- `func_80027248(id, pos, flags)`: positional sound: pan from `func_8003B808(pos)` into `g_Args+0x136/0x137`.
- `func_80027394(pos, voice, id, flags)`: updates a playing voice's volume.
- `func_80026C90(keep)`: silences voices 0–23 except `keep`.
- Likely PsyQ names: `func_800814CC` = SsUtSetVVol, `func_80081458` = SsUtKeyOffV, `func_800815A0` =
  SsUtKeyOn.

## Files, CD, levels

- Original source file names leaked by assert/error strings, with a function in each: font.c
  (`func_800289C0`), d3dcdrom.c (`func_8002A854`, `func_8002BF34`, `func_8002C490`), fileio.c
  (`func_80030994`), ob_mgr.c (`func_80039520`, `func_8003A71C`), prim_ob.c (`func_80058694`), indoor.c
  (`func_800600EC`), hmdlb01.c (`func_80067F18`); also init.c, guybo.c, SHARED.c, IMAGE.c, ANIM.c, POLY.c.
- `func_8002AC90(a, b)`: file-name compare (skips a's first char, stops at `;`, `\` == `/`).
- `func_8002AFF8(idx)`: CdSearchFile on `D_8009C204[idx].name` (12-byte records).

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

- `func_8004AF0C`: fade overlay init (two full-screen semi-transparent POLY_F4 + DR_MODE); colour via
  `func_8004B028` from `D_8009D000`.
- `func_8004B9FC(obj)` / `func_8004BB00`: object animation scripts (pc at obj+0x78 / entry+0x10). Opcodes
  0xFFFFF00A advance, 0xFFFFF000 jump, 0xFFFFF005 end + destroy; other words point to a u16 index →
  `func_8004BC70(D_800D2D30[idx], flags, 0)`.
- Morphs: `func_8004CBF4` loads a morph header (vertex buffer cached in `D_800D8BE8[type]`),
  `func_8004CDA0` parses the target list, `func_8004CE74` copies base SVECTORs, `func_8004CF00` blends up to
  8 weighted targets on the GTE, `func_8004D2D0` updates all `AnimEnt`s each frame.
- `func_8004D770(set, i, j)`: list index from elapsed time (loops when `pad8[j] == 1`).
- `func_8004E794`: camera init (GsInitCoordinate2 on `D_800D3238` and `g_Camera+0x30`, eye height 0x578,
  `D_800B0C10` = GsRVIEW2 with super = `g_Camera+0x30`).

- `func_80052CA8(out, chr)`: point in front of a character (dist 150, or 400 when crouched).
- `func_80052C0C(chr)`: every 40 frames spawns an effect at that point (`func_80065058`).
- `func_8005B5AC(chr, ...)`: claims one of 6 slots (`D_800DE0C8` used flags, `D_800DDEF0` owners).
- `func_8005B96C(chr, model, sub)`: fills the character's animation data, then `func_8005BA30`,
  `func_8005B7BC`.
- `func_8005CFC8(q, rx, ry, rz)`: s16 quaternion (4096 = 1.0) → Euler angles.
- `func_8005D694`: crouch button (toggle option = `g_Args->flags2` bit 5).
- `func_800617AC`: on-foot turning: turn speed clamped by obj+0x52, scaled by g_FrameCap/g_FrameRate; sets
  rotY, `gPsi`, +0xA0; probes the ground 125 units ahead (`func_800705A8`) → model pitch = ratan2(dy, 500).
- Sin/cos table `D_800A0370`: s32 entries, sine in the low half, cosine in the high half.
- Characters: +0x06 u8 type, +0x2C timestamp, +0x56 s16 turn speed, +0x90 model, +0x94 sub (+0x0A slot,
  +0x24 flags: 0x400 crouched), +0xA7 u8 id, +0xC0 animation.

- `func_800305B4`: checked `malloc3` (prints `D_800A74E0` and `exit(-1)` on failure); counts heap use in
  `Args+0x1F4`. `func_80032880(tim, image)`: TIM magic check + GsGetTimInfo.
- `func_80035964(obj)`: out-of-range test (box of size `D_800E2DE0` around the player).
- `func_8003B750(p)`: volume by distance: 127 − |v|·127/64000, clamped to 0..127.
- `func_800449BC` / `func_80044A60`: aim zoom in / out: steps Weapon+0x68 by `D_800D43AC` between
  `D_800D43F0` and `D_800D43F4`, then `func_8007A040(weapon)`.
- `func_8004F348`: 1 while the camera is more than 200 from its target on any axis (still moving).
  `func_8005038C(a0..a3)`: `func_80050180`, `func_8004FD3C`, then that test.
- `func_80058D00`: frees every effect in both pools (byte +0x9B = 0).
- `func_80069850(arg0)`: resets `D_8009E410..1C`, `D_800E23DC[3]`, `D_800E23EC[3]`.
- `func_80076104(idx)` / `func_80076014(idx)`: load the weapon's file `D_8009DB50[Weapon+0x5B]` into
  `D_800E2F58+4` synchronously / asynchronously in CD-archive mode (`func_8002C490`, done callback
  `func_800760B4`); load state `D_800F0F5C` (0 idle, 1 loading, 2 error).
- Weapon: +0x54/+0x56 s16 pair; +0x5B u8 file index; +0x68 s16 zoom.

- `func_80026B44(mode)`: random jitter (modes 1–4).
- `func_8002C168(name, buf)`: file loader → length or −1. CD-archive mode: name lookup in `D_800AFF2C`
  (`func_8002BF34`) + read (`func_8002A854`); otherwise the dev/PC loader `func_8002BE24`.
- `func_8003DE58(p1..p4)`: 2D segment intersection (Graphics Gems III), intermediates kept in globals
  `D_800D3FA8..D_800D4114`.
- `func_800475BC`: pops the on-screen message queue (`Msg` records, 0x2C bytes at `D_800D5DA4`, count
  `D_800D7CB4`) into the current message `D_800D4958`.
- `func_80049B84(idx, yoff)`: menu backdrop: gradient POLY_G4 (`D_800D79D4`), grey POLY_F4 box sized by the
  item count `D_8009CFBE` (`D_800D7554`), DR_MODE (`D_800D8138`).
- `func_8005397C` / `func_8006D3C0`: approximate 3D distance (max + mid/8 + (mid+min)/4), VECTOR / SVECTOR.
- `func_80058D5C(obj, yaw, pitch, r)`: direction vector at obj+0x3E/0x40/0x42.
- `func_8006412C`: free slot in `D_800DF540` (5 × 0x18). `func_800655B8`: init `D_800DF1F4[3]`.
- `func_80064A10` / `func_80064C28` / `func_80064E40`: per-frame effect animation (spin, shrink, wobble
  facing the camera), frame-rate compensated with constants per 20 fps.

- `func_80062300`: per-frame draw setup: GsGetLs(`D_800D3238`, `D_800DF10C`) → GTE matrices, then
  `func_80062394`, `func_80062784`, `func_80062B68`. `func_8006F6AC`: terrain draw (`func_8006E054`,
  `func_8006E9E0(D_800E2DBC)`).
- `func_800669A4(in, out, obj)`: local → world transform (rotation at obj+0x18, translation obj+0x38..).
- `func_8003EA24(v, m, outAngles)`: rotate v by m, then direction → Euler angles (ratan2).
- `func_8003EB00(obj)` / `func_800634C0(obj)`: attach an object to a model bone (0x8C-byte bone entries,
  GsCOORDINATE2 at +0x10): world position of an offset + direction angles.
- `func_8007545C(first, last)`: draws objects first..last from `D_800AFFC8`; the held weapon
  (`D_8009CD98`) is drawn with view `D_800F11D0` and the weapon's own FOV (`g_Weapons[i]+0xAA`).
- `func_800498E4(screenXY)` / `func_8004973C(VECTOR v, out)`: project the aim point (`D_800DDDDC` /
  `D_800DDDCC`) to the screen (crosshair).
- `func_80059C14(obj)`: projects 8 points (obj+0x184 → screen at obj+0x1C4) plus an optional line
  (laser?).
- `func_80076CDC(obj)`: fires the weapon: muzzle position into scratchpad `D_1F8003D4..`, projectile via
  `func_80076BF8`, cooldown obj+0x38 = 500, launched with `func_8003B53C(proj, yaw, pitch, 500)` along the
  aim angles in aim mode, else the player model's angles.
- `func_800381CC(obj, type)`: debris / shell-casing spawner (random pitch and spin).
- `func_8005BC80(obj, anim)`: interpolates a root offset between two animation frames (`D_800DDEE4`, 0x4C
  bytes per frame), blend factor anim+0x18 (4.12).

- `func_8003237C(n, arg)`: n ≥ 3 → load the next EXE (`func_8006B1DC`); else set the loadpoint
  (`D_8009E9C4+0x10 = &D_8009E9DC[n*16]`), `func_8004B04C`, `D_800D2F90 = 2`.
- `func_80037A24`: per-frame update of the 6 `AnimSlot`s.
- `func_80045344(...)`: sets up a POLY_FT4 and returns its tpage.
- `func_80058A58(type)`: reuses the oldest effect in the `D_800DA5C4` pool.
- `func_80058DFC(grid)`: 12×2 green TILEs + 2 blue TILEs (`D_800DD818`) — a HUD grid (radar?).
- `func_8005B508` / `func_8005BA30(obj)`: release the 6 character slots / copy a model's work matrix to
  `D_800DE004[slot]`.
- `func_8002E4DC`: resets the scratchpad muzzle position; spawns `D_8009D26C` (`func_80039A9C`).

- `func_8002F624`: ordering tables (`D_800B8F88`, `D_800B8EFC`, `D_800D2AFC`, two buffers each).
- `func_80037768(obj, proj)`: can this projectile damage this object (u16 obj+0xE: 0x7FFF never, 0 always,
  0x400 only the player, 0x800 not the player, else a type mask vs proj+0x3A).
- `func_800493C0`: builds the green crosshair (2 LINE_F2 × 2 buffers in `D_800D7C24`).
- `func_800637B0(x, y, z)`: allocates one of 16 × 0x34-byte records at `D_800DF200`.
- `DevMenu_HandleInput`: `g_DevMenu` = {active; cursor; count[12]; vals[12]}; rows 7–11 mirror
  `g_Invincible`/`g_GodMode`, `D_8009CD6C` (inverted), `D_8009CA8C`, `D_8009CDE8`, `D_8009CD6E`.

- `func_8003C1BC`: spawns an object from a spawn record. Parameter word bits: 0–2 scale, 3–6 rotation,
  7–11 path, 12–19 value at +8, 20–22 argument to `func_80053790` (AI script) for type-6 objects, 24–26
  field +0x68. Flags word: bits 0–7 type, bit 8 already spawned, bit 9 → state +0xA4 = 2.
- `func_80036938`: auto-aim target: closest visible object inside a yaw/pitch cone → `g_Player+0x5C`
  (target), +0xC4/+0xC6 (yaw/pitch difference), +0x110 (distance), `D_800DDE30`.
- `func_80031678`: per-frame level state: checkpoint reload, colour cycles and UV scrolls from the header at
  `D_800D2FC0->+4` ({nCol, nUv, ColCycle *, pad, UvScroll *}, 0x14-byte entries), fades.
- `func_8004A398`: scope/crosshair reticle (POLY_FT4 `D_800DDC70`), type `D_8009D8F0` (1–4), size from zoom
  `D_800DE0D0`, position `D_800D4480`.
- `func_80063F84`: 3 spark effects (`D_800DF1F4[3]`).
- `func_80075920`: HMD model instance init (PsyQ libgs HMD style: units, coordinate units, primitive
  drivers, GsLinkAnim).
- `func_80038C8C`: object destroyed: drop carried object, death sound (def+0x5D), spawn drops (def+5,
  def+6), award score def+4 (`func_800391B0`).
- -O1 file `text_8005EDF8.c` (0x8005EDF8–0x800617AC): object management around indoor.c:
  `func_800600EC` loads up to `g_Args+0x218` CLUTs into the 64×16 table and backs it up;
  `func_80061708(id)` switches cut-scene (stops the player); `func_800604B0`/`func_80060588` object light
  colours (light index obj+0xCD into `D_8009EADC`); `func_8005EE6C` collision probe (radius 500 for the
  player, else obj+0x2A); `func_80060AC0` snaps a model to the floor; `func_800615C8` floor probe + motion;
  `func_8005FF60` kills all objects; `func_80060798`/`func_80060614` knockback arc (state 0x25/0x26, base
  height +0xAC, vertical speed +0xAE, angle +0xB0, spin +0x9E..); `func_8005FAA8` removes an object;
  `func_8005F93C` removes it when out of range; `func_8005F788` per-frame object update; `func_80061068`
  movement collision vs objects and world (full move, then X-only / Z-only).
- `func_80061B70`: on-foot movement speed (forward/strafe); `func_80061AB0`: strafe/lean clamp.
- indoor.c CLUT handling: `D_800DE0E8` = 64×16 CLUT table, `D_800DE8E8` = backup,
  `D_800DE0E4` = per-slot CLUT index; `func_8006043C(a, b)` copies a CLUT row, `func_80060410` uploads it.

- `func_800373FC(proj, obj)`: applies a hit: impact sound, reactions (`func_800632AC`), health −=
  proj+0x34 if `func_80037768` allows; kills of some types counted in `g_Args+0x2CC`; death → obj+0xA4 = 2,
  death anim, `func_80038C8C`; else hit flash `func_80062F38`. Always removes the projectile.
- `func_80031B58(obj)`: script op "go to loadpoint" (records at `D_8009E9DC`).
- `func_8002E5C4`: level init; `D_8009CDD4` / `D_8009CDD8` are `void (*)(void)` hooks.
- HUD: `func_800456D0` sets up the HUD POLY_FT4s (`D_800D7588[2][7]`); `func_8004626C` blinking icon;
  `func_800474AC` slides HUD prims by `D_8009D014`; `func_80047B50` cinema letterbox bars (height
  `D_8009D01A`); `func_80049220` centred 64×32 sprite.
- `func_8004BC70(tmdObjs, model, idx)`: points the model at a TMD object and precomputes primitive run lengths.
- `func_8004C498(idx, e)`: copies a weapon definition (`g_Args->unk228`, 0x70-byte records).
- `func_8004C944(obj)`: ammo pickup. `func_8004CB2C(obj, w)`: ammo amount from clip size (random part).
- Sound (cont.): `func_80026F30` fails if id ≥ 105, 20+ voices busy (`D_8009C1E8`), or during cut scenes;
  Args +0x134 pitch offset, +0x13A master volume.

- Effect/sprite pools: `func_80062394` projects the camera-facing sprites (pool `D_800DB884`, 30 × 0xA0: pos
  SVECTOR +0x54, w/h +0x78/+0x7A, rotation +0x9C, corners +0x84..+0x92, otz +0x82, active +0x9B);
  `func_80062784` oriented 3D quads (pool `D_800DA5C4`, rotation +0x6C, owner obj +0x74, attached to a bone);
  `func_80062B68` updates and projects sparks (pool `D_800DCB4C`, 30 × 0x6C, 10-frame life);
  `func_80062FAC` spawns one spark.
- `func_80049470(obj, out)`: projects the object's bounding-box centre to the screen (target marker).
- `func_8004841C`: light glows / lens flare for up to 5 lights (`D_800B0CB0`), at most 2 drawn per frame.
- `func_8006CB88(poly)`: GT3 triangle setup: clip flags, nclip, OT z = (3·max+min)/4, colours (normal,
  night-vision green, flat grey); returns 0 culled / 1 draw / 2 subdivide.
- `func_80069D60(obj)`: builds a child object's matrix from two rotations plus table offsets.
- `func_8006E620(planes, min, max)`: box vs view frustum (7 planes).

- `func_800448F4`: entering aim mode: zoom limits from the weapon (+0x54/+0x56 u16 → `D_800D43CC`/`AC`,
  +0x68 → `D_800D43F4`/`F0`).
- `func_800391B0(obj, type)`: replaces an object with a new one of `type` at the same place (keeps script
  pointer +0x60, links +0x88/+0x8C), destroys the old one.
- `func_8003A898(type, arg1)`: effect slot from `D_800B0AB4` (0x34 bytes, count `D_800B0C84`), plays the
  type's sound (byte 0x5C of its 0x70-byte record in `g_Args->unk228`).
- `func_8003AA14(a, b)`: does segment a–b cross the 512×240 screen edge.
- `func_8003B808(src)`: stereo pan/volume for a sound relative to the camera → `g_Args+0x137/+0x136`.
- `func_8003BC98(rot, coord, part)` / `func_8003CABC`: rotation → GsCOORDINATE2 matrix (marks part+0x88
  dirty); `func_8003C8A4(a, b)` height band + radius test; `func_8003CABC(a, b)` is a inside b's view cone
  (range b+0x52, radius a+0x2A) — enemy sight.
- `func_8003E494`: direction index from flag pairs `D_800D4384..8F`.
- `func_80043B14(first, count)`: menu cursor navigation with d-pad and analog stick (edges at 0x4E/0xB3).
- `func_80045084`: links the `D_800D5F40` prims into the OT (addPrim loop).

- Camera helpers: `func_8005099C` snaps the camera to its target; `func_8004EAB8(obj)` copies the camera
  rotation to a model; `func_8004DFF8(pos)` aim-camera collision probe (`func_800743E8`); `func_8004E89C`
  camera init constants; `func_8004EB20` cut-scene camera aim at the actor's target.
- `func_8004E37C`: footsteps: step 300 (150 crouched), loop sound 0x50 while moving, footstep sound at each
  phase wrap; walk phase `D_8009D3FC`.
- `func_8004E5D8`: **head-bob**: phase `D_8009D3FC+2` advances by `D_8009D3F8`; offset `D_8009D3BC` =
  (`D_8009D3F0`·cos, −|`D_8009D3F4`·sin|, 0) rotated by camera yaw and added to the camera target.
  (Mod hook for "no head-bob".)
- `func_80052B2C(out, chr)`: eye position (player: camera; others: model + height, halved crouched).
- `func_80053684(x, z, obj, dist)`: Manhattan distance test.
- `func_8004CD14(hdr, buf)`: morph setup (reverse of `func_8004CE74`).
- Cut-scene actor (`D_800D90C0`): `func_80053A7C` init, `func_80053BE0` per-frame update, `func_80053ED8`
  picks the best point of interest among 27 `Pickup` entries (`D_800DA270`; per-type range table
  `D_8009D660`).

- Effects (cont.): `func_80057C40` resets all effect pools at level init (classes with count 0 point at the
  start of BSS: `D_800AB540`, `_b`, `_c`); `func_8005885C(slot, type)` initialises a 0xA0-byte slot from its
  template `D_8009D8B8[type]` (0x14 bytes: size, flags (bits 4–7 pool class), RGB, UV, w/h, clut, tpage);
  `func_80058B6C(type)` allocates a 0x6C-byte slot (only type 0x50 = sparks has a pool); `func_80058B10(owner)`
  frees an owner's class-0x80 effects.
- `func_8005B6CC(obj)` / `func_8005B76C(obj)`: character colour (normal / night-vision red / grey; dead).
- `func_8005BAE8(obj, anim)`: root motion between two keyframes (`D_800DDEE0` tables, 6-byte {x,y,z}).

## Enemy AI: waypoints and navmesh

- `func_800523E4(obj, path)`: nearest waypoint on a path (1-based, 0xFF if none); sets turn bit 2.
- `func_80052630(obj, dir)`: waypoint follower. Waypoint = low byte of obj+0x6C, path = (obj+0x64 & 0x7F) − 1;
  flags 8 = reached, 0x10 = end of path; turn amount `D_800D9232`, clamped to ±0xB6.
- `func_80052808(obj, x, y, z)`: navmesh routing toward a point: zone lookup `func_800705A8`, route table
  `nav->0x10[self][target]` (two u16 node indices), state at ai+0x23C {zone, state, node0, node1}.
- Data: `D_8009D5A8` = per-path {x, z} waypoints (`s32 **`), `D_8009D5AC` = waypoint count per path (`s16 *`);
  navmesh at `D_8009CE38`: +4 zone count, +0xC SVECTOR nodes, +0x10 route table.
- `func_80051728(obj, tgt)`: **sight test**: range and FOV halved when the target is the sneaking player;
  distance (+0x110), yaw/pitch to target (+0xC4/+0xC6), raycast `func_80050A10`, 3-bit "seen" counter at
  obj+0x64 bits 25–27 (bits 29–31 = alert level, near range −= level·128). Returns 0x40 spotted, 0x100
  roughly facing, 0x10 near, 0x8000 < 200, 0x20000000 < far range.
- AI sub (obj+0x118): +0x48 near range, +0x4C sight range, +0x50 FOV, +0x84 far range, +0x1D0 point-of-
  interest type mask, +0x1D4 chosen point, +0x1D8.. fallback target position.
- AI state at obj+0x118; flags at ai+0x10: 2 = turn left, 8 = reached, 0x10 = path end, 0x4000 = steering.

## AI scripts

Command word: byte 0 opcode, byte 1 destination variable, u16 at +2 operand modes (2 bits per operand:
0 none, 1 script variable, otherwise immediate), operands follow.

- `func_80055060`: "call native": `D_8009E480[idx]()` (idx < `D_8009E490`), index from byte 2 or a variable;
  result to vars[byte 3]. Error text `D_800A8A2C` and wait = 1 when out of range.
- `func_8005483C` / `func_80054AC8`: set camera target X/Y/Z / rotation X/Y/Z (each operand: keep, variable,
  or immediate).
- AI slots: 7 × 0x248 bytes at `D_800D9278` (in-use flag bit 1 at +0x1CC); `func_800540CC` allocates,
  `func_8005414C` frees.
- `func_80056110` (op 0x23): vars[n] = abs(vars[n]).
- `func_800572F4`: play sound `func_80027248(id or vars[id], model+0x14, flag)`, optional volume into
  `g_Args+0x138` (2-word form).
- `func_8005747C` (op 0x1B): play animation: id byte 1, start/end frames bytes 2/3 (−1 = last frame);
  wait = frames × anim+0x20; loops when start == end; `func_80057AD8`.
- `func_80056F90`: "switch-gosub": vars[byte 1] indexes an n-entry table (n = byte 2); pushes the return
  address on the script call stack (+0x1B0: {count; u32 *items[4]}).
- `func_8005743C`: set wait counter (`obj->wait` = immediate or variable), pc += 1 word.
- `func_80055620` / `func_80055778`: no-op opcodes that skip their operands.
- `func_80055598` (op 0x10): chr->0x94->+0x34 = angle. `func_80055670` (op 0x12): set model position.
- `func_80056154` (op 0x24): line-of-sight probe from the model (y − 200) along heading + angle: world
  collision `func_800746D0`, then object hit `func_80050A70`; vars[byte 1] = 0/1/2, `D_8009D5FC` = hit object.
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

## Non-natural matches (revisit)

These match byte for byte but through tricks rather than natural source; worth another look:

- `func_80061AB0`: an empty `__asm__("" : "+r"(t))` barrier to get `sra; negu; addu` for `b -= a >> 4`.
- `func_80063F34`: the permuter gave `func_80065614` a `volatile int` parameter.
- `func_80037768`: a `volatile` read (it also matches with -fno-cse-skip-blocks — maybe another -O1 module).
