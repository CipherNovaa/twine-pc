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

static int lent_trace(void);
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
    if (in && lent_trace()) { /* (each set of images once) */
        static u32 sum_seen;
        u32 sum = lent_imgs;
        for (i = 0; i < lent_imgs; i++)
            sum = sum * 31 + (u32)lent_img[i].r.x * 7 + (u32)lent_img[i].r.y * 3 + (u32)lent_img[i].r.w + (u32)lent_img[i].r.h;
        if (sum != sum_seen)
            for (sum_seen = sum, i = 0; i < lent_imgs; i++)
                port_log("trace: lentprim: lent image %d at %d,%d %dx%d\n", i, lent_img[i].r.x, lent_img[i].r.y,
                         lent_img[i].r.w, lent_img[i].r.h);
    }
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

/* TWINE_TRACE=lentprim: the lent weapon's textured polygons whose texture page or colours lie outside its own images
 * (what the mission's VRAM has there is drawn: another mission's model reading its home mission's VRAM), each
 * (texture page, colours) once */
static int lent_trace(void) {
    static int on = -1;
    if (on < 0)
        on = port_trace("lentprim");
    return on;
}
static int lent_covers(s32 x, s32 y, s32 w, s32 h) {
    int i;
    for (i = 0; i < lent_imgs; i++)
        if (x >= lent_img[i].r.x && y >= lent_img[i].r.y && x + w <= lent_img[i].r.x + lent_img[i].r.w &&
            y + h <= lent_img[i].r.y + lent_img[i].r.h)
            return 1;
    return 0;
}
static void lent_prim(const u32 *w, u32 len) {
    static u32 seen[256];
    static int nseen;
    u32 cmd = w[0] >> 24, clut = 0, tpage = 0, key;
    int k, quad, gour, n, i, pos = 1, umin = 255, umax = 0, vmin = 255, vmax = 0, depth;
    s32 cx, cy, tx, ty, div;
    if ((cmd & 0xE0) == 0x20 && !(cmd & 4) && getenv("TWINE_TRACE_LENTPRIM_ALL")) { /* (an untextured one: where) */
        int st = 1 + (cmd >> 4 & 1), nv = cmd >> 3 & 1 ? 4 : 3, x0 = 9999, x1 = -9999, y0 = 9999, y1 = -9999;
        for (i = 0; i < nv && (u32)(1 + i * st) < len; i++) {
            s16 x = (s16)(w[1 + i * st] & 0xFFFF), y = (s16)(w[1 + i * st] >> 16);
            x0 = x < x0 ? x : x0, x1 = x > x1 ? x : x1, y0 = y < y0 ? y : y0, y1 = y > y1 ? y : y1;
        }
        if ((x1 - x0) * (y1 - y0) > 400)
            port_log("trace: lentprim: untextured cmd %02X colour %06X at %d..%d, %d..%d\n", cmd, w[0] & 0xFFFFFF, x0, x1,
                     y0, y1);
        return;
    }
    if ((cmd & 0xE0) != 0x20 || !(cmd & 4)) /* (textured polygons) */
        return;
    quad = cmd >> 3 & 1, gour = cmd >> 4 & 1, n = quad ? 4 : 3;
    if (len < (u32)(1 + n * 2 + (gour ? n - 1 : 0)))
        return;
    for (i = 0; i < n; i++) {
        u32 uv;
        if (gour && i)
            pos++; /* (its colour) */
        uv = w[pos + 1];
        if (i == 0)
            clut = uv >> 16;
        if (i == 1)
            tpage = uv >> 16;
        if ((int)(uv & 0xFF) < umin) umin = uv & 0xFF;
        if ((int)(uv & 0xFF) > umax) umax = uv & 0xFF;
        if ((int)(uv >> 8 & 0xFF) < vmin) vmin = uv >> 8 & 0xFF;
        if ((int)(uv >> 8 & 0xFF) > vmax) vmax = uv >> 8 & 0xFF;
        pos += 2;
    }
    depth = tpage >> 7 & 3, div = depth == 0 ? 4 : depth == 1 ? 2 : 1;
    tx = (tpage & 15) * 64, ty = (tpage >> 4 & 1) * 256, cx = (clut & 0x3F) * 16, cy = clut >> 6 & 0x1FF;
    key = (u32)tx << 20 ^ (u32)(ty + vmin) << 10 ^ (u32)umin ^ clut << 12;
    for (k = 0; k < nseen && seen[k] != key; k++)
        ;
    if (k < nseen)
        return;
    if (nseen < 256)
        seen[nseen++] = key;
    {
        int in_t = lent_covers(tx + umin / div, ty + vmin, (umax - umin) / div + 1, vmax - vmin + 1);
        int in_c = depth == 2 || lent_covers(cx, cy, depth == 0 ? 16 : 256, 1);
        if (in_t && in_c && !getenv("TWINE_TRACE_LENTPRIM_ALL"))
            return;
        port_log("trace: lentprim: cmd %02X texels %d,%d %dx%d (page %d,%d, %d bits%s) colours %d,%d: %s%s\n", cmd,
                 tx + umin / div, ty + vmin, (umax - umin) / div + 1, vmax - vmin + 1, tx, ty, 16 / div,
                 cmd & 2 ? ", semi-transparent" : "", cx, cy, in_t ? "texels in the lent images" : "texels NOT in them",
                 in_c ? "" : ", colours NOT in them");
    }
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
        if (len && lent_in && lent_trace())
            lent_prim(p + 1, len);
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
