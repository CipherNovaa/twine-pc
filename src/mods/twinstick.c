/* Modern twin-stick controls for every level: the left stick moves and strafes, the right stick turns and
 * looks up and down (no need to hold the aim button to look). Analog mode is switched on automatically.
 *
 * MOD_REPLACES: Pad_StickToSpeed func_8005D760 func_80061DC0
 *
 * Names are LEVEL01's; `make cmod` maps them onto each level (tools/port_symbols.py). */
#include "twine.h"

extern u8 g_PadAxes[4]; /* right X, right Y, left X, left Y; 0x7F is centred */
extern s8 D_800B0E54;   /* the pad is in analog mode (Pad_Update) */
extern u8 D_800B0E48;   /* PadSetMainMode has been sent (Pad_Update sends it again when 0) */
extern s32 D_800DDD8C;  /* strafe left */
extern s32 D_800DDD90;  /* strafe right */
extern s32 D_800DDCC4;  /* turn left */
extern s32 D_800DDDB8;  /* turn right */
extern s16 D_800DE0DC;  /* turn amount */
extern s32 D_8009CD00;  /* with g_CutScene: the mapper leaves the flags alone while set */
extern s32 D_8009CD4C;  /* aim mode (aim button held): func_80043EE0 runs instead of the on-foot update */
extern s32 D_800B0C64;  /* aim mode's pitch, -0x280 (up) to 0x320 (down); 4096 = 360 degrees */
void func_8005D760_orig(void);
void func_80061DC0_orig(void);

/* Looking up and down is an offset on the player model's pitch (s16 at model+0x60, signed), which the
 * on-foot camera (func_8004DA60: camera pitch = model pitch - 0x17) and firing (func_80076CDC) follow.
 * While walking, the game sets that pitch from the ground slope every frame (func_800617AC, called from
 * the on-foot update func_80061DC0), so the offset goes on after that update. Explicitly initialized:
 * gcc 2.8 then puts them in .data, which every level load resets. */
static s32 s_LookOffset = 0;
static s32 s_BasePitch = 0;  /* the game's own pitch */
static s32 s_Written = 0;    /* the pitch we last stored, to notice when the game sets a new one */

/* Analog stick to speed with a 3-unit dead zone and a quadratic curve, scaled for frame rate. Axis 0 is
 * turning/aiming yaw and axis 1 aiming pitch. The original reads the left stick; this reads the right. */
void Pad_StickToSpeed(s32 axis, s32 maxSpeed, s32 *out) {
    s32 deadzone;
    s32 raw;
    s16 pos;
    s16 mag;
    s16 speed;

    deadzone = 3;
    if (axis == 0) {
        raw = g_PadAxes[0] - 0x7F;
    } else {
        raw = -g_PadAxes[1] + 0x7F;
    }
    pos = raw;
    if (pos < 0) {
        mag = -raw - deadzone;
    } else {
        mag = raw - deadzone;
    }
    if (mag < 0) {
        mag = 0;
    }
    speed = (s16)(mag * mag) / (s16)((s16)((0x80 - deadzone) * (0x80 - deadzone)) / maxSpeed);
    if (pos < 0) {
        speed = -speed;
    }
    if (g_FrameRate < g_FrameCap) {
        speed = speed * g_FrameCap / g_FrameRate;
    }
    *out = speed;
}

/* Input mapper, called every frame before the player update: turns the pad into movement flags (left stick
 * Y forward/back, left stick X turn, buttons through g_Args->buttonMap). Afterwards the left stick X strafes
 * and the right stick X turns instead. */
void func_8005D760(void) {
    s32 strafe;
    s32 turn;

    if (!(g_Args->flags2 & 0x100)) {
        /* The front-end's analog option; level init copies it into D_800B0E54 from then on. */
        g_Args->flags2 |= 0x100;
        D_800B0E54 = 1;
        D_800B0E48 = 0;
    }
    func_8005D760_orig();
    if (D_800B0E54 == 0 || g_CutScene != 0 || D_8009CD00 != 0) {
        return;
    }
    strafe = g_PadAxes[2] - 0x7F;
    turn = g_PadAxes[0] - 0x7F;
    if (strafe < -0x40) {
        D_800DDD8C = 1;
    } else if (strafe > 0x40) {
        D_800DDD90 = 1;
    }
    D_800DDCC4 = turn < -0x32;
    D_800DDDB8 = turn > 0x32;
    D_800DE0DC = (D_800DDCC4 || D_800DDDB8) ? turn : 0;

    /* Aim mode drives its own pitch (it starts from the model pitch, offset included): follow it, so the
     * view stays put when the aim button is released. */
    if (D_8009CD4C != 0) {
        s_LookOffset = D_800B0C64 + 0x17 - s_BasePitch;
    }
}

/* On-foot update, then the right stick Y looks up and down on top of the game's pitch. */
void func_80061DC0(void) {
    s16 *pitch;
    s32 look;
    s32 p;

    func_80061DC0_orig();
    if (D_800B0E54 == 0 || g_CutScene != 0 || D_8009CD00 != 0) {
        return;
    }
    pitch = (s16 *)((u8 *)M2C_FIELD(g_Player, void **, 0x90) + 0x60);
    if (*pitch != s_Written) {
        s_BasePitch = *pitch;
    }
    /* Same direction as aim mode (func_80043EE0), which follows the REVERSE SNIPER option. */
    Pad_StickToSpeed(1, 0x40, &look);
    if (g_Args->reverseSniper == 0) {
        look = -look;
    }
    p = s_BasePitch + s_LookOffset + look;
    if (p < -0x280 + 0x17) {
        p = -0x280 + 0x17;
    } else if (p > 0x320 + 0x17) {
        p = 0x320 + 0x17;
    }
    s_LookOffset = p - s_BasePitch;
    *pitch = p;
    s_Written = *pitch;
}
