/* Jump (native PC build only): Space makes Bond jump, about half a metre, over low things and up onto ledges, and
 * he falls rather than drops where the floor ends below him. He keeps walking, turning, aiming and firing in the
 * air. Not on skis (Cold Reception), crouched, in cut scenes or while the game has the controls.
 *
 * Bond's feet are his model's y (+0x2C of g_Player+0x90; y grows downwards), which func_80060E28 (his step, every
 * frame) sets to the floor below him after moving him: func_80060AC0 asks func_800705A8 for the floor under him
 * nearest his feet' height. The walls he bumps into (func_80061068) are tested from his feet up to his eyes. So in
 * the air his feet are set where the jump has them before the step, which then tests the walls from there (a low
 * wall is under his feet) and finds the floor nearest to there (a ledge's top, once he is above it); after it they
 * go back up to the jump's height until it comes down to that floor. Where there is no floor at all under him he
 * doesn't go.
 *
 * MOD_REPLACES_NATIVE: func_80060E28
 *
 * Names are LEVEL01's; port/mods.py maps them onto each level. */
#include "twine.h"

extern void *g_Player;
extern s32 D_8009DB3C __attribute__((weak)); /* on skis (Cold Reception's only) */
extern u8 g_CutScene;
extern s32 D_8009CDA8;      /* the frame counter */
extern s32 D_800E2F54;      /* the level's floors */
int port_jump(void);
void func_80060E28_orig(u8 *obj, s16 dx, s16 dz);
s32 func_800705A8(s32 floors, SVECTOR *at, s32 arg2, s32 arg3);

#define SKIING (g_Args->level == 1 && &D_8009DB3C && D_8009DB3C)
#define W(p, off, type) (*(type *)((u8 *)(p) + (off)))

#define LIFT 130    /* upwards speed at the jump (units a frame; Bond's eyes are 1400 up) */
#define FALL 22     /* gravity, a frame */
#define DROP 260    /* a floor further down than this is fallen to */

static int air;     /* in the air */
static s32 feet;    /* his feet's height there */
static s32 speed;   /* downwards */
static s32 last_frame;

void func_80060E28(u8 *obj, s16 dx, s16 dz) {
    u8 *m = W(obj, 0x90, u8 *);
    s32 *pos = (s32 *)(m + 0x28);
    s32 before = pos[1], x = pos[0], z = pos[2], floor;
    SVECTOR at;
    int jump = port_jump();

    if (obj != g_Player || D_8009CDA8 - last_frame > 2 || D_8009CDA8 < last_frame || g_CutScene || SKIING)
        air = 0;
    last_frame = D_8009CDA8;
    if (obj != g_Player) {
        func_80060E28_orig(obj, dx, dz);
        return;
    }
    if (!air && jump && !g_CutScene && !SKIING && !(W(W(obj, 0x94, u8 *), 0x24, s32) & 0x400)) {
        air = 1;
        feet = before;
        speed = -LIFT;
    }
    if (air)
        pos[1] = feet;
    func_80060E28_orig(obj, dx, dz);
    if (!air) {
        /* walked off an edge: he falls from where he was */
        if (pos[1] - before > DROP) {
            air = 1;
            feet = before;
            speed = 0;
        } else
            return;
    }
    at.vx = pos[0];
    at.vy = pos[1];
    at.vz = pos[2];
    floor = func_800705A8(D_800E2F54, &at, 0, 0);
    if (floor == -0x7FFF) {
        /* nothing under him: back where he was */
        pos[0] = x;
        pos[2] = z;
        W(obj, 0x98, s16) = 0;
        W(obj, 0x9C, s16) = 0;
        floor = pos[1];
    }
    /* he lands on a floor he comes down onto, not on one above his feet (the top of a ledge he is still below) */
    if (speed > 0 && feet <= floor && feet + speed >= floor) {
        air = 0;
        feet = floor;
    } else {
        feet += speed;
        speed += FALL;
    }
    pos[1] = feet;
}
