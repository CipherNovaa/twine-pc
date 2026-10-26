/* libgpu on the host: the game's ordering tables, primitives, drawing/display environments and image
 * transfers go to the software GPU (gpu.c). Drawing completes immediately. Signatures follow PsyQ's
 * libgpu.h. */
#include <stdlib.h>
#include <string.h>
#include "port.h"
#include "gpu.h"
#include "gpu_hw.h"

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

/* A weapon lent from another mission (port/mods/endless.c) has its model's textures where its own mission keeps them,
 * which this mission may use for its own (Courier's grenade lies where the AR36's scope picture goes). So the images
 * the lent model loads aren't put in VRAM as it's made: they're kept here and put there only while the weapon in hand
 * is drawn (its ordering table, sorted into the frame's: from the first of its tags DrawOTag meets to the next of the
 * frame's own), what was there put back after. What was there is read once (again only after the game writes
 * there itself: lent_written). With the hardware renderer the swaps go to the card's VRAM alone: gpu.c's copy (which
 * the hardware renderer keeps only for reading VRAM back) keeps the mission's pixels there; swapping it too made
 * gpu.c keep, and soon draw, everything of every drawing (a third of the time a drawing took). */
#define LENT_IMAGES 16
static struct {
    RECT r;
    u16 *pix, *saved;
    int have; /* (saved holds what is there: read once, then again only after the game writes there itself) */
} lent_img[LENT_IMAGES];
static int lent_imgs, lent_in;
static const u8 *lent_lo, *lent_hi;            /* (the lent model's file: its images are loaded from there) */
static const u32 *lent_ot_lo[4], *lent_ot_hi[4]; /* (the tags of the weapon in hand's two ordering tables, then the
                                                  * frame's two) */

void port_lent_images(const void *at, u32 size) {
    int i;
    for (i = 0; i < lent_imgs; i++)
        free(lent_img[i].pix), free(lent_img[i].saved);
    lent_imgs = 0;
    lent_lo = at;
    lent_hi = (const u8 *)at + size;
}

void port_lent_ot(int k, const void *org, u32 tags) {
    if (k >= 0 && k < 4)
        lent_ot_lo[k] = org, lent_ot_hi[k] = (const u32 *)org + tags;
}

static int lent_image(const RECT *r, const u32 *p) {
    int i;
    u32 n = (u32)r->w * (u32)r->h;
    if (!lent_lo || (const u8 *)p < lent_lo || (const u8 *)p >= lent_hi || r->w <= 0 || r->h <= 0)
        return 0;
    for (i = 0; i < lent_imgs; i++)
        if (!memcmp(&lent_img[i].r, r, sizeof(*r)))
            break;
    if (i == LENT_IMAGES)
        return 0;
    if (i == lent_imgs) {
        lent_img[i].r = *r;
        lent_img[i].pix = malloc(n * 2);
        lent_img[i].saved = malloc(n * 2);
        if (!lent_img[i].pix || !lent_img[i].saved) {
            free(lent_img[i].pix), free(lent_img[i].saved);
            return 0;
        }
        lent_imgs++;
    }
    lent_img[i].have = 0;
    memcpy(lent_img[i].pix, p, n * 2);
    return 1;
}

/* the game writes VRAM there itself: what the lent images cover read again at the next swap. (Reading it at each
 * swap made gpu.c draw everything it keeps for later, every drawing: the swaps' own loads are kept there.) */
static void lent_written(s32 x, s32 y, s32 w, s32 h) {
    int i;
    if (lent_in)
        return;
    for (i = 0; i < lent_imgs; i++)
        if (x < lent_img[i].r.x + lent_img[i].r.w && lent_img[i].r.x < x + w && y < lent_img[i].r.y + lent_img[i].r.h &&
            lent_img[i].r.y < y + h)
            lent_img[i].have = 0;
}

static void lent_swap(int in) {
    int i;
    u64 t = PT_BEGIN();
    static int hwonly = -1;
    static int reread = -1; /* (TWINE_TEST_LENT_REREAD=1: read back at every swap, as before: for comparing) */
    if (reread < 0)
        reread = getenv("TWINE_TEST_LENT_REREAD") != NULL;
    if (in && reread)
        for (i = 0; i < lent_imgs; i++)
            lent_img[i].have = 0;
    if (in)
        for (i = 0; i < lent_imgs; i++)
            if (!lent_img[i].have) {
                gpu_store_image(lent_img[i].r.x, lent_img[i].r.y, lent_img[i].r.w, lent_img[i].r.h, lent_img[i].saved);
                lent_img[i].have = 1;
                port_timing_count(PT_LENT_READ);
            }
    if (hwonly < 0) /* (the card's VRAM only: TWINE_TEST_LENT_SW_SWAP=1 has gpu.c's copy swapped too, as before) */
        hwonly = getenv("TWINE_TEST_LENT_SW_SWAP") == NULL;
    for (i = in ? 0 : lent_imgs - 1; in ? i < lent_imgs : i >= 0; i += in ? 1 : -1)
        if (hwonly && gpu_hw_on)
            gpu_hw_load_image(lent_img[i].r.x, lent_img[i].r.y, lent_img[i].r.w, lent_img[i].r.h,
                              in ? lent_img[i].pix : lent_img[i].saved);
        else
            gpu_load_image(lent_img[i].r.x, lent_img[i].r.y, lent_img[i].r.w, lent_img[i].r.h,
                           in ? lent_img[i].pix : lent_img[i].saved);
    lent_in = in;
    PT_END(PT_LENT_SWAP, t);
}

s32 LoadImage(RECT *r, u32 *p) {
    if (lent_image(r, p))
        return 0;
    lent_written(r->x, r->y, r->w, r->h);
    gpu_load_image(r->x, r->y, r->w, r->h, (const u16 *)p);
    return 0;
}

s32 StoreImage(RECT *r, u32 *p) {
    gpu_store_image(r->x, r->y, r->w, r->h, (u16 *)p);
    return 0;
}

s32 MoveImage(RECT *r, s32 x, s32 y) {
    lent_written(x, y, r->w, r->h);
    gpu_move_image(r->x, r->y, r->w, r->h, x, y);
    return 0;
}

s32 ClearImage(RECT *r, u8 red, u8 g, u8 b) {
    lent_written(r->x, r->y, r->w, r->h);
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
    u64 t = PT_BEGIN();
    while (p && n++ < 0x100000) {
        u32 tag = *p, len = tag >> 24;
        if (lent_imgs) {
            if (!lent_in && ((p >= lent_ot_lo[0] && p < lent_ot_hi[0]) || (p >= lent_ot_lo[1] && p < lent_ot_hi[1])))
                lent_swap(1);
            else if (lent_in && ((p >= lent_ot_lo[2] && p < lent_ot_hi[2]) || (p >= lent_ot_lo[3] && p < lent_ot_hi[3])))
                lent_swap(0);
        }
        if (len)
            gpu_gp0(p + 1, len);
        if ((tag & 0xFFFFFF) == 0xFFFFFF)
            break;
        p = (u32 *)(0x80000000u | (tag & 0xFFFFFF));
    }
    if (lent_in)
        lent_swap(0);
    PT_END(PT_DRAWOTAG, t);
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

/* saved and put back around an extra drawing (interp.c) */
static DRAWENV saved_drawenv;
static DISPENV saved_dispenv;
static void (*saved_drawsync_callback)(void);

void libgpu_save(void) {
    saved_drawenv = cur_drawenv;
    saved_dispenv = cur_dispenv;
    saved_drawsync_callback = drawsync_callback;
}

void libgpu_restore(void) {
    cur_drawenv = saved_drawenv;
    cur_dispenv = saved_dispenv;
    drawsync_callback = saved_drawsync_callback;
}

DRAWENV *GetDrawEnv(DRAWENV *env) {
    *env = cur_drawenv;
    return env;
}

DISPENV *GetDispEnv(DISPENV *env) {
    *env = cur_dispenv;
    return env;
}
