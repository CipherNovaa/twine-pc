/* Modern twin-stick controls for every level: the left stick moves and strafes, the right stick turns and
 * looks up and down (no need to hold the aim button to look). Analog mode is switched on automatically.
 * In the native PC build the mouse turns and looks too, on foot and in aim mode.
 *
 * MOD_REPLACES: Pad_StickToSpeed func_8005D760 func_80061DC0 func_800617AC func_80061B70
 * MOD_REPLACES_NATIVE: func_80043EE0 func_8004FD3C func_80051AE0
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
extern s32 D_800B0C64;  /* aim mode's pitch, -0x280 (down) to 0x320 (up); 4096 = 360 degrees */
void func_8005D760_orig(void);
void func_80061DC0_orig(void);
void func_800617AC_orig(void *obj);
void func_80061B70_orig(u8 *obj);
static s16 s_Slope = 0; /* the ground's pitch, as the game set it (func_800617AC) */
#ifdef NATIVE_PORT
int port_sprint(void);
#endif

#ifdef NATIVE_PORT
extern s32 g_PadHeld;
extern s32 g_PadPressed;
extern s32 D_800DDCC8; /* the reload button pressed, held (the button mapper's) */
extern s32 D_800DDD98;
int port_use_kind(void);
void func_80051AE0_orig(u8 **obj);
extern s32 D_8009CD54; /* aim mode is gliding back out */
extern s16 D_8009CD5C; /* frames the aim has held still */
extern u16 D_800B0C5C; /* aim mode's yaw */
void func_80043EE0_orig(void);
void func_8004FD3C_orig(s16 *a0, s16 *a1);
/* The mouse's movement since the last call, in angle units (4096 = 360 degrees; pitch positive is up);
 * port/src/video.c. */
void port_mouse_look(s32 *yaw, s32 *pitch);
static s32 s_MouseYaw = 0;
static s32 s_MousePitch = 0;
static s32 s_MouseTicks = 0; /* ticks since the mouse last moved, counting down from 20 */
extern u8 D_800AC284[]; /* +0x108 set with +0x104 == 1: a menu has the controls (the mapper clears the flags) */
/* Who has the controls: 1 Bond (the gamepad's buttons then work through the game's button settings: fire,
 * aim, ...), 0 a menu (the pause menu is g_CutScene 7), 2 a cut scene; port/src/video.c. */
void port_controls_state(int state);
#endif

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
#ifdef NATIVE_PORT
    /* The game's reload button also uses things; the PC's keys and the gamepad have one of each (port_use_kind):
     * the weapon's reload reads these flags, the use func_80051AE0 */
    if (!(port_use_kind() & 2)) {
        D_800DDCC8 = 0;
        D_800DDD98 = 0;
    }
    if (g_CutScene == 7 || (D_800AC284[0x108] != 0 && *(s32 *)(D_800AC284 + 0x104) == 1)) {
        port_controls_state(0);
    } else {
        port_controls_state(g_CutScene != 0 || D_8009CD00 != 0 ? 2 : 1);
    }
#endif
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
#ifdef NATIVE_PORT
    port_mouse_look(&s_MouseYaw, &s_MousePitch);
    if (s_MouseYaw != 0 || s_MousePitch != 0) {
        s_MouseTicks = 20;
    } else if (s_MouseTicks > 0) {
        s_MouseTicks--;
    }
#endif

    /* Aim mode drives its own pitch (it starts from the model pitch, offset included): follow it, so the
     * view stays put when the aim button is released. */
    if (D_8009CD4C != 0) {
        s_LookOffset = D_800B0C64 + 0x17 - s_BasePitch;
    }
}

/* Walking, the game points the model (and with it the camera and the aim) along the ground: its pitch is the
 * slope 500 units ahead, set every frame here (with the turning). With the sticks looking, it is level ground's
 * instead, so the view stays where the player put it whatever the ground does, as in PC shooters. */
void func_800617AC(void *obj) {
    s16 *pitch = (s16 *)((u8 *)M2C_FIELD(g_Player, void **, 0x90) + 0x60);
    func_800617AC_orig(obj);
    s_Slope = *pitch;
    if (D_800B0E54 != 0) {
        *pitch = 0; /* level ground: the look (func_80061DC0's offset) is then the pitch itself */
    }
}

/* Walking: the step along the ground, which the game shortens by the model's pitch (the slope's). That is
 * the slope's here too, not where the player looks. In the native build Shift sprints. */
void func_80061B70(u8 *obj) {
    s16 *pitch = (s16 *)(*(u8 **)(obj + 0x90) + 0x60);
    s16 keep = *pitch;
    if (D_800B0E54 != 0) {
        *pitch = s_Slope;
    }
    func_80061B70_orig(obj);
    *pitch = keep;
#ifdef NATIVE_PORT
    /* Sprint (hold): half as fast again, on foot */
    if (port_sprint() && g_CutScene == 0 && D_8009CD00 == 0) {
        s16 *v = (s16 *)((u8 *)g_Player + 0x98);
        v[0] = v[0] * 3 / 2;
        v[2] = v[2] * 3 / 2;
    }
#endif
}

/* On-foot update, then the right stick Y looks up and down on top of the game's pitch. */
void func_80061DC0(void) {
    s16 *pitch;
    s32 look;
    s32 p;

#ifdef NATIVE_PORT
    /* The mouse turns the model before the update turns it further by the stick and moves the player. */
    if (s_MouseYaw != 0 && g_CutScene == 0 && D_8009CD00 == 0) {
        s16 *yaw = (s16 *)((u8 *)M2C_FIELD(g_Player, void **, 0x90) + 0x62);
        *yaw = (*yaw + s_MouseYaw) & 0xFFF;
    }
    s_MouseYaw = 0;
#endif
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
#ifdef NATIVE_PORT
    look += s_MousePitch;
    s_MousePitch = 0;
#endif
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

#ifdef NATIVE_PORT
/* Using things (doors, people, ...), on the reload button's press: not when only the reload key or button is
 * down (port_use_kind) */
void func_80051AE0(u8 **obj) {
    s32 pressed = g_PadPressed;
    if (!(port_use_kind() & 1)) {
        g_PadPressed &= ~g_Args->buttonMap[3];
    }
    func_80051AE0_orig(obj);
    g_PadPressed = pressed;
}

/* Aim mode (the aim button held): the mouse moves the aim like the stick does, within the same limits. */
void func_80043EE0(void) {
    s32 moved;
    s32 p;

    moved = s_MouseYaw != 0 || s_MousePitch != 0;
    if (D_8009CD54 == 0 && (g_PadHeld & g_Args->buttonMap[5])) {
        D_800B0C5C = (D_800B0C5C + s_MouseYaw) & 0xFFF;
        p = D_800B0C64 + s_MousePitch;
        if (p < -0x280) {
            p = -0x280;
        } else if (p > 0x320) {
            p = 0x320;
        }
        D_800B0C64 = p;
    } else {
        moved = 0;
    }
    s_MouseYaw = 0;
    s_MousePitch = 0;
    func_80043EE0_orig();
    if (moved) {
        D_8009CD5C = 0; /* not holding still */
    }
}
#endif

#ifdef NATIVE_PORT
/* The camera turns after the player through a spring; while the mouse is in use it turns with them at once,
 * as aiming with a mouse wants (and as the frames drawn between ticks expect: port/mods/smooth.c). */
void func_8004FD3C(s16 *a0, s16 *a1) {
    u8 *c;

    if (s_MouseTicks == 0 || g_CutScene != 0) {
        func_8004FD3C_orig(a0, a1);
        return;
    }
    c = (u8 *)&g_Camera;
    *(u16 *)(c + 0x80) = g_Camera.rotX;
    *(u16 *)(c + 0x82) = g_Camera.rotY;
    *(u16 *)(c + 0x84) = g_Camera.rotZ;
    *(s32 *)(c + 0x24) = 0;
    *(s32 *)(c + 0x28) = 0;
    *(s32 *)(c + 0x2C) = 0;
}
#endif
