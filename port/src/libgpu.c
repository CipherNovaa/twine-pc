/* libgpu on the host: the game's ordering tables, primitives, drawing/display environments and image
 * transfers go to the software GPU (gpu.c). Drawing completes immediately. Signatures follow PsyQ's
 * libgpu.h. */
#include <stdio.h>
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

/* VRAM OVERLAYS: textures of the port's own (port/mods/quickgrenade.c's grenade, endless.c's lent launcher round)
 * that no mission keeps a place free for. Their pixels are never left in VRAM: they are put there only while
 * DrawOTag draws their own primitives, and what was there put back before anything else is drawn that reads or
 * writes there, and when the walk ends. Their primitives are known by their colours: the overlay's CLUT word with
 * bit 15 set (the GPU ignores that bit: the CLUT's y has 9 bits, 6-14), which their TMDs carry
 * (PORT_OVERLAY_MARK). So whatever a room of any mission keeps at those places is never overwritten: a fixed
 * "free" place had been chosen from a survey of VRAM writes (wt/scope), which no later room could be trusted to
 * keep to. With the hardware renderer the swaps go to the card's VRAM alone (as the lent weapon's do): gpu.c's
 * copy keeps the mission's pixels, read back for each swap (gpu_store_image: what the card has there).
 * TWINE_TRACE=overlay logs each overlay registered and, every 600 video frames, the swaps and what made them. */
#define OVERLAYS 4
#define PORT_OVERLAY_MARK 0x8000
u32 port_video_frames(void);
#define OVERLAY_RECTS 2
static struct {
    u16 clut;                       /* (its CLUT word, without the mark) */
    int n, in;
    RECT r[OVERLAY_RECTS];
    u16 *pix[OVERLAY_RECTS], *saved[OVERLAY_RECTS];
} overlay[OVERLAYS];
static int overlays, overlays_in;
static u32 overlay_swaps, overlay_outs_for[4]; /* (why swapped out: something else reads, a VRAM transfer, a lent
                                                * weapon's swap, the walk's end) */
static int plant_at(void);
static void overlay_plant_one(int i);
s32 LoadImage(RECT *r, u32 *p);
static int overlay_trace(void) {
    static int on = -1;
    if (on < 0)
        on = port_trace("overlay");
    return on;
}

/* (quickgrenade.c, endless.c) an overlay: its picture (tw x th at tx, ty) and colours (cw x 1 at cx, cy; cw 0: none),
 * for the primitives whose CLUT word is clut | PORT_OVERLAY_MARK. Again with the same clut: the pixels replaced. */
void port_vram_overlay(u16 clut, s32 tx, s32 ty, s32 tw, s32 th, const u16 *tex, s32 cx, s32 cy, s32 cw,
                       const u16 *col) {
    int i, k;
    clut &= 0x7FFF;
    for (i = 0; i < overlays && overlay[i].clut != clut; i++)
        ;
    if (i == OVERLAYS)
        return;
    if (i == overlays) {
        overlays++;
        overlay[i].clut = clut;
    }
    for (k = 0; k < overlay[i].n; k++)
        free(overlay[i].pix[k]), free(overlay[i].saved[k]);
    overlay[i].n = 0;
    for (k = 0; k < 2; k++) {
        s32 x = k ? cx : tx, y = k ? cy : ty, w = k ? cw : tw, h = k ? 1 : th;
        const u16 *src = k ? col : tex;
        int n = overlay[i].n;
        if (w <= 0 || h <= 0 || !src)
            continue;
        overlay[i].r[n].x = (s16)x, overlay[i].r[n].y = (s16)y, overlay[i].r[n].w = (s16)w, overlay[i].r[n].h = (s16)h;
        overlay[i].pix[n] = malloc((size_t)w * h * 2);
        overlay[i].saved[n] = malloc((size_t)w * h * 2);
        if (!overlay[i].pix[n] || !overlay[i].saved[n]) {
            free(overlay[i].pix[n]), free(overlay[i].saved[n]);
            continue;
        }
        memcpy(overlay[i].pix[n], src, (size_t)w * h * 2);
        overlay[i].n++;
    }
    if (overlay_trace())
        port_log("trace: overlay: %d (colours %04X): %dx%d at %d,%d, colours %dx1 at %d,%d\n", i, clut, tw, th, tx, ty,
                 cw, cx, cy);
    if (plant_at() >= 0 && port_video_frames() >= (u32)plant_at())
        overlay_plant_one(i); /* (tests: registered after the frame the pattern was planted at) */
}

static void overlay_load(s32 x, s32 y, s32 w, s32 h, const u16 *p) {
    if (gpu_hw_on)
        gpu_hw_load_image(x, y, w, h, p);
    else
        gpu_load_image(x, y, w, h, p);
}

static void overlay_swap(int i, int in, int why) {
    int k;
    static int off = -1; /* (TWINE_TEST_OVERLAY_OFF=1, tests: as before, the pixels put in VRAM for good as they are
                          * first drawn, what was there lost) */
    if (off < 0)
        off = getenv("TWINE_TEST_OVERLAY_OFF") != NULL;
    if (off) {
        for (k = 0; in && k < overlay[i].n; k++)
            gpu_load_image(overlay[i].r[k].x, overlay[i].r[k].y, overlay[i].r[k].w, overlay[i].r[k].h,
                           overlay[i].pix[k]);
        return;
    }
    if (overlay[i].in == in)
        return;
    for (k = 0; k < overlay[i].n; k++) {
        RECT *r = &overlay[i].r[k];
        if (in)
            gpu_store_image(r->x, r->y, r->w, r->h, overlay[i].saved[k]);
        overlay_load(r->x, r->y, r->w, r->h, in ? overlay[i].pix[k] : overlay[i].saved[k]);
    }
    overlay[i].in = in;
    overlays_in += in ? 1 : -1;
    if (!in)
        overlay_outs_for[why]++;
    else if (!overlay_swaps++ && overlay_trace())
        port_log("trace: overlay: %d first put in at v%u\n", i, port_video_frames());
}

static void overlays_out(int why) {
    int i;
    for (i = 0; overlays_in && i < overlays; i++)
        overlay_swap(i, 0, why);
}

static int overlay_meets(int i, s32 x, s32 y, s32 w, s32 h) {
    int k;
    for (k = 0; k < overlay[i].n; k++) {
        const RECT *r = &overlay[i].r[k];
        if (x < r->x + r->w && r->x < x + w && y < r->y + r->h && r->y < y + h)
            return 1;
    }
    return 0;
}

/* a textured command's texels (u, v ranges in its page; through a texture window: the page's whole range) and
 * colours: do they meet overlay i's places */
static int overlay_reads(int i, u32 tpage, u32 clut, int umin, int umax, int vmin, int vmax, u32 mx, u32 my) {
    s32 tx = (tpage & 15) * 64, ty = (tpage >> 4 & 1) * 256, depth = tpage >> 7 & 3, sh = depth == 0 ? 2 : depth == 1;
    if (mx)
        umin = 0, umax = 255;
    if (my)
        vmin = 0, vmax = 255;
    umin = umin > 0 ? umin - 1 : 0, umax = umax < 255 ? umax + 1 : 255; /* (the steps' rounding reaches past) */
    vmin = vmin > 0 ? vmin - 1 : 0, vmax = vmax < 255 ? vmax + 1 : 255;
    if (overlay_meets(i, tx + (umin >> sh), ty + vmin, ((umax - umin) >> sh) + 2, vmax - vmin + 1))
        return 1;
    if (tx + (umax >> sh) > 1023 && overlay_meets(i, 0, ty + vmin, ((tx + (umax >> sh)) & 1023) + 1, vmax - vmin + 1))
        return 1; /* (a page at VRAM's right edge goes on at its left) */
    return depth < 2 && overlay_meets(i, (clut & 63) * 16, clut >> 6 & 511, depth ? 256 : 16, 1);
}

/* Before a packet of the ordering table is drawn: the overlays its primitives are drawn from put in; those that
 * something else in it reads or writes put out first */
static void overlay_packet(const u32 *w, u32 len) {
    u32 k = 0, tp, mx, my;
    int i;
    gpu_texture_state(&tp, &mx, &my);
    while (k < len) {
        u32 cmd = w[k] >> 24, n = 1;
        if (cmd >= 0x20 && cmd < 0x40) { /* polygons */
            int tex = cmd >> 2 & 1, gour = cmd >> 4 & 1, nv = cmd & 8 ? 4 : 3, v, pos = 1;
            int umin = 255, umax = 0, vmin = 255, vmax = 0;
            u32 clut = 0, page = tp;
            n = 1 + nv * (1 + tex) + (gour ? nv - 1 : 0);
            if (tex && k + n <= len) {
                for (v = 0; v < nv; v++) {
                    u32 uv;
                    if (gour && v)
                        pos++;
                    uv = w[k + pos + 1];
                    if (v == 0)
                        clut = uv >> 16;
                    if (v == 1)
                        page = (tp & ~0x1FFu) | (uv >> 16 & 0x1FF);
                    umin = (int)(uv & 255) < umin ? (int)(uv & 255) : umin;
                    umax = (int)(uv & 255) > umax ? (int)(uv & 255) : umax;
                    vmin = (int)(uv >> 8 & 255) < vmin ? (int)(uv >> 8 & 255) : vmin;
                    vmax = (int)(uv >> 8 & 255) > vmax ? (int)(uv >> 8 & 255) : vmax;
                    pos += 2;
                }
                tp = page; /* (a textured polygon sets the GPU's page) */
                for (i = 0; i < overlays; i++)
                    if ((clut & PORT_OVERLAY_MARK) && (clut & 0x7FFF) == overlay[i].clut)
                        overlay_swap(i, 1, 0);
                    else if (overlay[i].in && overlay_reads(i, page, clut, umin, umax, vmin, vmax, mx, my)) {
                        static int told;
                        if (overlay_trace() && told < 20 && ++told)
                            port_log("trace: overlay: %d out at v%u for cmd %02X page %04X colours %04X u %d-%d v %d-%d "
                                     "window %d,%d\n", i, port_video_frames(), cmd, page, clut, umin, umax, vmin, vmax, mx, my);
                        overlay_swap(i, 0, 0);
                    }
            }
        } else if (cmd >= 0x40 && cmd < 0x60) { /* lines: untextured (a poly-line runs to its end marker) */
            if (!(cmd & 8))
                n = cmd & 0x10 ? 4 : 3;
            else {
                for (n = cmd & 0x10 ? 4 : 3; k + n < len && (w[k + n] & 0xF000F000) != 0x50005000; n += cmd & 0x10 ? 2 : 1)
                    ;
                n++;
            }
        } else if (cmd >= 0x60 && cmd < 0x80) { /* rectangles */
            int tex = cmd >> 2 & 1, size = cmd >> 3 & 3;
            n = 2 + tex + (size == 0);
            if (tex && overlays_in && k + n <= len) {
                u32 uv = w[k + 2];
                int rw = size == 1 ? 1 : size == 2 ? 8 : size == 3 ? 16 : (int)(w[k + 3] & 0x3FF);
                int rh = size == 1 ? 1 : size == 2 ? 8 : size == 3 ? 16 : (int)(w[k + 3] >> 16 & 0x1FF);
                int u0 = uv & 255, v0 = uv >> 8 & 255;
                for (i = 0; i < overlays; i++)
                    if (overlay[i].in &&
                        overlay_reads(i, tp, uv >> 16, u0, u0 + rw - 1 > 255 ? 255 : u0 + rw - 1, v0,
                                      v0 + rh - 1 > 255 ? 255 : v0 + rh - 1, 1, 1))
                        overlay_swap(i, 0, 0); /* (flipped or wrapped: the page's whole range, as through a window) */
            }
        } else if (cmd == 0x02 || (cmd >= 0x80 && cmd < 0xE0)) { /* fills and transfers: none with an overlay in */
            overlays_out(1);
            n = cmd == 0x02 || cmd >= 0xC0 ? 3 : cmd < 0xA0 ? 4 : k + 2 < len ? 3 + ((((w[k + 2] - 1) & 1023) + 1) *
                                                                                      ((((w[k + 2] >> 16) - 1) & 511) + 1) + 1) / 2 : len;
        } else if (cmd == 0xE1)
            tp = w[k] & 0x3FFF;
        else if (cmd == 0xE2)
            mx = w[k] & 31, my = w[k] >> 5 & 31;
        k += n;
    }
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

/* TWINE_TRACE_PRIMAT=x0,y0,x1,y1,f0,f1 (research): each polygon drawn in video frames f0..f1 whose corners' box
 * overlaps that box (drawing coordinates) and covers more than 200 square pixels: its command, colour, box, texture
 * page and colours, its packet's address and place in the ordering table's walk (which list, which code made it) */
u32 port_video_frames(void);
static int primat_box[6];
static int primat_on(void) {
    static int on = -1;
    if (on < 0) {
        const char *e = getenv("TWINE_TRACE_PRIMAT");
        on = e && sscanf(e, "%d,%d,%d,%d,%d,%d", &primat_box[0], &primat_box[1], &primat_box[2], &primat_box[3],
                         &primat_box[4], &primat_box[5]) == 6;
    }
    return on && (int)port_video_frames() >= primat_box[4] && (int)port_video_frames() <= primat_box[5];
}
static void primat(const u32 *pk, u32 len, u32 nth) {
    const u32 *w = pk + 1;
    u32 cmd = w[0] >> 24;
    int tex, gour, nv, st, i, x0 = 9999, x1 = -9999, y0 = 9999, y1 = -9999;
    if ((cmd & 0xE0) != 0x20)
        return;
    tex = cmd >> 2 & 1, gour = cmd >> 4 & 1, nv = cmd >> 3 & 1 ? 4 : 3, st = 1 + tex + gour;
    for (i = 0; i < nv; i++) {
        u32 at = 1 + i * st;
        s16 x, y;
        if (at >= len)
            return;
        x = (s16)(w[at] & 0xFFFF), y = (s16)(w[at] >> 16);
        x0 = x < x0 ? x : x0, x1 = x > x1 ? x : x1, y0 = y < y0 ? y : y0, y1 = y > y1 ? y : y1;
    }
    if (x1 < primat_box[0] || x0 > primat_box[2] || y1 < primat_box[1] || y0 > primat_box[3] ||
        (x1 - x0) * (y1 - y0) <= 200)
        return;
    port_log("trace: primat: v%u #%u packet %08X cmd %02X colour %06X box %d..%d %d..%d%s%s", port_video_frames(), nth,
             (u32)(long)pk, cmd, w[0] & 0xFFFFFF, x0, x1, y0, y1, tex ? " textured" : "", lent_in ? " (lent weapon's)" : "");
    if (tex)
        port_log(" clut %04X tpage %04X", w[2] >> 16, w[2 + (gour ? 2 : 1) + 1] >> 16);
    port_log("\n");
}

static u32 overlay_logged;
/* (tests: TWINE_TEST_VRAM_PLANT=N: from video frame N a pattern written where the overlays go (those registered then,
 * each one registered later as it is), through the game's own LoadImage, as a room that kept its own textures there
 * would have them: check w looks for it after a grenade) */
static int plant_at(void) {
    static int at = -2;
    if (at == -2)
        at = getenv("TWINE_TEST_VRAM_PLANT") ? atoi(getenv("TWINE_TEST_VRAM_PLANT")) : -1;
    return at;
}
static void overlay_plant_one(int i) {
    static u16 pat[64 * 64];
    int k, j;
    for (k = 0; k < overlay[i].n; k++) {
        RECT r = overlay[i].r[k];
        if (r.w * r.h > 64 * 64)
            continue;
        for (j = 0; j < r.w * r.h; j++)
            pat[j] = (u16)(((r.x + j % r.w) * 37 + (r.y + j / r.w) * 101) & 0x7FFF) | 0x0421;
        LoadImage(&r, (u32 *)pat);
        port_log("test: VRAM planted at %d,%d %dx%d (overlay %d)\n", r.x, r.y, r.w, r.h, i);
    }
}
static void overlay_plant(void) {
    static int done;
    int i;
    if (done || plant_at() < 0 || port_video_frames() < (u32)plant_at())
        return;
    done = 1;
    for (i = 0; i < overlays; i++)
        overlay_plant_one(i);
}

void DrawOTag(u32 *p) {
    u32 n = 0;
    if (overlays)
        overlay_plant();
    u64 t = PT_BEGIN();
    while (p && n++ < 0x100000) {
        u32 tag = *p, len = tag >> 24;
        if (lent_imgs) {
            if (!lent_in && ((p >= lent_ot_lo[0] && p < lent_ot_hi[0]) || (p >= lent_ot_lo[1] && p < lent_ot_hi[1])))
                overlays_out(2), lent_swap(1);
            else if (lent_in && ((p >= lent_ot_lo[2] && p < lent_ot_hi[2]) || (p >= lent_ot_lo[3] && p < lent_ot_hi[3])))
                overlays_out(2), lent_swap(0);
        }
        if (len && overlays)
            overlay_packet(p + 1, len);
        if (len && lent_in && lent_trace())
            lent_prim(p + 1, len);
        if (len && primat_on())
            primat(p, len, n);
        if (len)
            gpu_gp0(p + 1, len);
        if ((tag & 0xFFFFFF) == 0xFFFFFF)
            break;
        p = (u32 *)(0x80000000u | (tag & 0xFFFFFF));
    }
    overlays_out(3);
    if (lent_in)
        lent_swap(0);
    if (overlays && overlay_trace() && port_video_frames() / 600 != overlay_logged) {
        overlay_logged = port_video_frames() / 600;
        port_log("trace: overlay: v%u: %u put in; out for something reading there %u, a transfer %u, a lent weapon %u, "
                 "the walk's end %u\n", port_video_frames(), overlay_swaps, overlay_outs_for[0], overlay_outs_for[1],
                 overlay_outs_for[2], overlay_outs_for[3]);
    }
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
