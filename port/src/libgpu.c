/* libgpu on the host: the game's ordering tables, primitives, drawing/display environments and image
 * transfers go to the software GPU (gpu.c). Drawing completes immediately. Signatures follow PsyQ's
 * libgpu.h. */
#include <string.h>
#include "port.h"
#include "gpu.h"

typedef struct {
    s16 x, y, w, h;
} RECT;

typedef struct {
    RECT clip;
    s16 ofs[2];
    RECT tw;
    u16 tpage;
    u8 dtd, dfe, isbg, r0, g0, b0;
    u32 dr_env[16];
} DRAWENV;

typedef struct {
    RECT disp, screen;
    u8 isinter, isrgb24, pad0, pad1;
} DISPENV;

static void (*drawsync_callback)(void);
static DRAWENV cur_drawenv;
static DISPENV cur_dispenv;

s32 ResetGraph(s32 mode) {
    (void)mode;
    return 0;
}

s32 SetGraphDebug(s32 level) {
    (void)level;
    return 0;
}

void SetDispMask(s32 mask) {
    gpu_display.blank = !mask;
}

s32 DrawSync(s32 mode) {
    (void)mode;
    return 0;
}

void *DrawSyncCallback(void (*fn)(void)) {
    void *old = (void *)drawsync_callback;
    drawsync_callback = fn;
    return old;
}

s32 LoadImage(RECT *r, u32 *p) {
    gpu_load_image(r->x, r->y, r->w, r->h, (const u16 *)p);
    return 0;
}

s32 StoreImage(RECT *r, u32 *p) {
    gpu_store_image(r->x, r->y, r->w, r->h, (u16 *)p);
    return 0;
}

s32 MoveImage(RECT *r, s32 x, s32 y) {
    gpu_move_image(r->x, r->y, r->w, r->h, x, y);
    return 0;
}

s32 ClearImage(RECT *r, u8 red, u8 g, u8 b) {
    gpu_fill_rect(r->x, r->y, r->w, r->h, (red >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10));
    return 0;
}

/* ordering tables: linked lists of packets, 24-bit addresses, 0xFFFFFF ends the list */
u32 *ClearOTag(u32 *ot, s32 n) {
    s32 i;
    for (i = 0; i < n - 1; i++)
        ot[i] = (u32)&ot[i + 1] & 0xFFFFFF;
    ot[n - 1] = 0xFFFFFF;
    return ot;
}

u32 *ClearOTagR(u32 *ot, s32 n) {
    s32 i;
    for (i = n - 1; i > 0; i--)
        ot[i] = (u32)&ot[i - 1] & 0xFFFFFF;
    ot[0] = 0xFFFFFF;
    return ot;
}

void DrawOTag(u32 *p) {
    u32 n = 0;
    while (p && n++ < 0x100000) {
        u32 tag = *p, len = tag >> 24;
        if (len)
            gpu_gp0(p + 1, len);
        if ((tag & 0xFFFFFF) == 0xFFFFFF)
            break;
        p = (u32 *)(0x80000000u | (tag & 0xFFFFFF));
    }
    if (drawsync_callback)
        drawsync_callback();
}

void DrawPrim(u32 *p) {
    gpu_gp0(p + 1, *p >> 24);
}

DRAWENV *PutDrawEnv(DRAWENV *env) {
    gpu_set_draw_area(env->clip.x, env->clip.y, env->clip.x + env->clip.w - 1, env->clip.y + env->clip.h - 1);
    gpu_set_draw_offset(env->ofs[0], env->ofs[1]);
    gpu_set_texwindow((env->tw.w ? (~(env->tw.w - 1) & 0xFF) >> 3 : 0), (env->tw.h ? (~(env->tw.h - 1) & 0xFF) >> 3 : 0),
                      (env->tw.x >> 3) & 31, (env->tw.y >> 3) & 31);
    gpu_set_texpage(env->tpage, env->dtd, env->dfe);
    if (env->isbg)
        gpu_fill_rect(env->clip.x, env->clip.y, env->clip.w, env->clip.h,
                      (env->r0 >> 3) | ((env->g0 >> 3) << 5) | ((env->b0 >> 3) << 10));
    cur_drawenv = *env;
    return env;
}

DISPENV *PutDispEnv(DISPENV *env) {
    gpu_display.x = env->disp.x;
    gpu_display.y = env->disp.y;
    gpu_display.w = env->disp.w;
    gpu_display.h = env->disp.h;
    gpu_display.rgb24 = env->isrgb24;
    cur_dispenv = *env;
    return env;
}

DRAWENV *GetDrawEnv(DRAWENV *env) {
    *env = cur_drawenv;
    return env;
}

DISPENV *GetDispEnv(DISPENV *env) {
    *env = cur_dispenv;
    return env;
}
