/* Smooth motion at the display's frame rate (native PC build only). The game moves in 20 Hz ticks and draws
 * once per tick; while it waits for the next tick, the runtime has its render step draw the frame again as
 * often as there is time for (port/src/interp.c), with the camera between where it was at the last two ticks
 * and turned by mouse movement the game hasn't taken yet. Those drawings change nothing (the runtime puts the
 * machine back after each); the tick's own drawing comes last, as always.
 *
 * The camera is a coordinate system (g_Camera+0x30) placed at g_Camera.pos and turned by the angles at
 * g_Camera+0x80 (pitch, yaw, roll); the view (D_800B0C10) looks along its z axis. A drawing in between rebuilds
 * the camera's matrices from interpolated angles and position as the camera update does (func_8004D978).
 *
 * MOD_REPLACES: func_8002F7CC
 *
 * Names are LEVEL01's; port/mods.py maps them onto each level. */
#include "twine.h"

extern s32 D_800B0C10[]; /* the view (GsRVIEW2): viewpoint, reference point, roll, coordinate system */
extern s32 D_800D3238[]; /* the world's coordinate system */
extern s32 D_800B0808[]; /* its local-screen matrix */
extern s32 D_8009CD30;   /* the view's heading */
extern s32 D_8009CD00;
void func_8002F7CC_orig(void);
void func_8003BC98(void *rot, void *coord, s32 part);
void GsSetRefView2(void *view);
void GsGetLs(void *coord, void *m);
s32 ratan2(s32 y, s32 x);
int port_smooth_begin(s32 *t, s32 *yaw, s32 *pitch);
int port_mouse_recent(void);
void port_smooth_end(void);
void port_smooth_present(void);

#define CAM_ROT ((s16 *)((u8 *)&g_Camera + 0x80))
#define CAM_POS (&g_Camera.posX)

typedef struct {
    s16 rot[3];
    s32 pos[3];
} View;

static View s_Prev;
static s32 s_HavePrev = 0;

/* signed difference of two angles (4096 = 360 degrees) */
static s32 angle_diff(s32 a, s32 b) {
    s32 d = (b - a) & 0xFFF;
    return d >= 0x800 ? d - 0x1000 : d;
}

/* One drawing between ticks, `t` (0-4096) of the way from the last tick's camera to this one's. While the
 * mouse is in use, the camera's angles are this tick's turned by what the mouse has moved since the game took
 * it, rather than interpolated: looking around then lags nothing. */
static void draw_between(const View *cur, s32 t, s32 yaw, s32 pitch, s32 mouse) {
    s32 p0, p, i;

    for (i = 0; i < 3; i++) {
        CAM_POS[i] = s_Prev.pos[i] + (cur->pos[i] - s_Prev.pos[i]) * t / 4096;
        CAM_ROT[i] = mouse ? cur->rot[i] : s_Prev.rot[i] + angle_diff(s_Prev.rot[i], cur->rot[i]) * t / 4096;
    }
    /* the mouse turns the player (and the camera with it) at the next tick (twinstick.c) */
    if (mouse) {
        CAM_ROT[1] += yaw;
        /* within the game's limits for looking up and down */
        p0 = ((CAM_ROT[0] & 0xFFF) ^ 0x800) - 0x800;
        p = p0 + pitch;
        if (pitch > 0 && p > 0x320) {
            p = p0 > 0x320 ? p0 : 0x320;
        } else if (pitch < 0 && p < -0x280) {
            p = p0 < -0x280 ? p0 : -0x280;
        }
        CAM_ROT[0] = p;
    }
    func_8003BC98(CAM_ROT, (u8 *)&g_Camera + 0x30, 0);
    GsSetRefView2(D_800B0C10);
    GsGetLs(D_800D3238, D_800B0808);
    gte_SetRotMatrix(D_800B0808);
    gte_SetTransMatrix(D_800B0808);
    D_8009CD30 = (CAM_ROT[1] + ratan2(D_800B0C10[3] - D_800B0C10[0], D_800B0C10[5] - D_800B0C10[2])) & 0xFFF;
    func_8002F7CC_orig();
    port_smooth_end();
}

void func_8002F7CC(void) {
    View cur;
    s32 t, yaw, pitch, mouse, i;

    for (i = 0; i < 3; i++) {
        cur.rot[i] = CAM_ROT[i];
        cur.pos[i] = CAM_POS[i];
    }
    /* a cut, not motion: nothing in between */
    for (i = 0; i < 3; i++) {
        if (!s_HavePrev || abs(cur.pos[i] - s_Prev.pos[i]) > 0x800 || abs(angle_diff(s_Prev.rot[i], cur.rot[i])) > 0x400) {
            s_Prev = cur;
            break;
        }
    }
    mouse = port_mouse_recent() && g_CutScene == 0 && D_8009CD00 == 0;
    while (port_smooth_begin(&t, &yaw, &pitch)) {
        draw_between(&cur, t, yaw, pitch, mouse);
    }
    s_Prev = cur;
    s_HavePrev = 1;
    func_8002F7CC_orig();
    /* The tick's own frame; with the mouse in use, one turned by what it has moved since is shown instead. */
    if (mouse && port_smooth_begin(&t, &yaw, &pitch)) {
        draw_between(&cur, 4096, yaw, pitch, mouse);
    } else {
        port_smooth_present();
    }
}
