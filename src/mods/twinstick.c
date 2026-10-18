/* Modern twin-stick controls for every level: the left stick moves and strafes, the right stick turns and
 * aims. Analog mode is switched on automatically.
 *
 * MOD_REPLACES: Pad_StickToSpeed func_8005D760
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
void func_8005D760_orig(void);

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
}
