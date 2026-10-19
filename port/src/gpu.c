/* PS1 GPU in software, written from the psx-spx documentation ("Graphics Processing Unit (GPU)"): GP0
 * command packets (polygons, lines, rectangles, fills, VRAM transfers, drawing environment) rasterized
 * into a 1024x512 15-bit VRAM with the hardware's texture/CLUT lookup, colour modulation,
 * semi-transparency, dithering, mask bit and drawing area. libgpu (libgpu.c) feeds it the game's
 * ordering tables, and the display shows a rectangle of this VRAM like the real video output. */
#include <string.h>
#include "port.h"
#include "gpu.h"

u16 gpu_vram[512 * 1024];
GpuDisplay gpu_display = {0, 0, 320, 240, 0, 1};

/* drawing environment */
static s32 area_x1, area_y1, area_x2 = 1023, area_y2 = 511;
static s32 off_x, off_y;
static u32 texpage;                 /* E1 bits 0-11 (+ rect flip bits 12-13) */
static u32 tw_mask_x, tw_mask_y, tw_off_x, tw_off_y;
static int mask_set, mask_check;
static int dither_on;

#define VRAM(x, y) gpu_vram[((y) & 511) * 1024 + ((x) & 1023)]

static const s8 dither_table[4][4] = {
    {-4, 0, -3, 1}, {2, -2, 3, -1}, {-3, 1, -4, 0}, {3, -1, 2, -2}};

static s32 sext11(u32 v) {
    return ((s32)(v << 21)) >> 21;
}

/* ---- pixel pipeline ---- */

typedef struct {
    int textured, raw, semi, gouraud;
    u32 tpage;      /* texpage bits for this primitive */
    u32 clut_x, clut_y;
    int dither;
} Prim;

static u16 tex_fetch(const Prim *p, u32 u, u32 v) {
    u32 tx = (p->tpage & 15) * 64, ty = ((p->tpage >> 4) & 1) * 256;
    u32 depth = (p->tpage >> 7) & 3;
    u = (u & ~(tw_mask_x * 8)) | ((tw_off_x & tw_mask_x) * 8);
    v = (v & ~(tw_mask_y * 8)) | ((tw_off_y & tw_mask_y) * 8);
    u &= 255;
    v &= 255;
    if (depth == 0) {
        u16 w = VRAM(tx + u / 4, ty + v);
        u32 i = (w >> ((u & 3) * 4)) & 15;
        return VRAM(p->clut_x + i, p->clut_y);
    }
    if (depth == 1) {
        u16 w = VRAM(tx + u / 2, ty + v);
        u32 i = (w >> ((u & 1) * 8)) & 255;
        return VRAM(p->clut_x + i, p->clut_y);
    }
    return VRAM(tx + u, ty + v);
}

/* Write one pixel: colour (r, g, b 8-bit, 0x80 = neutral for texture modulation), texel if textured. */
static void plot(const Prim *p, s32 x, s32 y, s32 r, s32 g, s32 b, u32 u, u32 v) {
    u16 *dst, out;
    s32 cr, cg, cb;
    int semi = p->semi;
    if (x < area_x1 || x > area_x2 || y < area_y1 || y > area_y2)
        return;
    dst = &VRAM(x, y);
    if (mask_check && (*dst & 0x8000))
        return;
    if (p->textured) {
        u16 t = tex_fetch(p, u, v);
        if (t == 0)
            return;
        semi = semi && (t & 0x8000);
        cr = t & 31;
        cg = (t >> 5) & 31;
        cb = (t >> 10) & 31;
        if (!p->raw) {
            /* modulate in 8-bit precision so dithering can apply */
            cr = cr * 8 * r / 128;
            cg = cg * 8 * g / 128;
            cb = cb * 8 * b / 128;
        } else {
            cr *= 8;
            cg *= 8;
            cb *= 8;
        }
        out = t & 0x8000;
    } else {
        cr = r;
        cg = g;
        cb = b;
        out = 0;
    }
    if (p->dither && !(p->textured && p->raw)) {
        s32 d = dither_table[y & 3][x & 3];
        cr += d;
        cg += d;
        cb += d;
    }
    cr = cr < 0 ? 0 : cr > 255 ? 31 : cr >> 3;
    cg = cg < 0 ? 0 : cg > 255 ? 31 : cg >> 3;
    cb = cb < 0 ? 0 : cb > 255 ? 31 : cb >> 3;
    if (semi) {
        s32 br = *dst & 31, bg = (*dst >> 5) & 31, bb = (*dst >> 10) & 31;
        switch ((p->tpage >> 5) & 3) {
        case 0:
            cr = (br + cr) >> 1;
            cg = (bg + cg) >> 1;
            cb = (bb + cb) >> 1;
            break;
        case 1:
            cr = br + cr;
            cg = bg + cg;
            cb = bb + cb;
            break;
        case 2:
            cr = br - cr;
            cg = bg - cg;
            cb = bb - cb;
            break;
        case 3:
            cr = br + cr / 4;
            cg = bg + cg / 4;
            cb = bb + cb / 4;
            break;
        }
        cr = cr < 0 ? 0 : cr > 31 ? 31 : cr;
        cg = cg < 0 ? 0 : cg > 31 ? 31 : cg;
        cb = cb < 0 ? 0 : cb > 31 ? 31 : cb;
    }
    *dst = out | (mask_set ? 0x8000 : 0) | cr | (cg << 5) | (cb << 10);
}

/* ---- triangles ---- */

typedef struct {
    s32 x, y;
    s32 r, g, b;
    s32 u, v;
} Vtx;

static s64 edge(const Vtx *a, const Vtx *b, s32 x, s32 y) {
    return (s64)(b->x - a->x) * (y - a->y) - (s64)(b->y - a->y) * (x - a->x);
}

/* Is the edge a->b a top or left edge of a triangle with positive area (pixels on it are drawn)? */
static int top_left(const Vtx *a, const Vtx *b) {
    s32 dx = b->x - a->x, dy = b->y - a->y;
    return dy < 0 || (dy == 0 && dx > 0);
}

static void triangle(const Prim *p, Vtx v0, Vtx v1, Vtx v2) {
    s64 area = edge(&v0, &v1, v2.x, v2.y);
    s32 minx, maxx, miny, maxy, x, y;
    int b0, b1, b2;
    if (area == 0)
        return;
    if (area < 0) {
        Vtx t = v1;
        v1 = v2;
        v2 = t;
        area = -area;
    }
    minx = v0.x < v1.x ? v0.x : v1.x;
    minx = minx < v2.x ? minx : v2.x;
    maxx = v0.x > v1.x ? v0.x : v1.x;
    maxx = maxx > v2.x ? maxx : v2.x;
    miny = v0.y < v1.y ? v0.y : v1.y;
    miny = miny < v2.y ? miny : v2.y;
    maxy = v0.y > v1.y ? v0.y : v1.y;
    maxy = maxy > v2.y ? maxy : v2.y;
    /* the GPU skips polygons with an edge longer than 1023x511 */
    if (maxx - minx > 1023 || maxy - miny > 511)
        return;
    if (minx < area_x1)
        minx = area_x1;
    if (maxx > area_x2)
        maxx = area_x2;
    if (miny < area_y1)
        miny = area_y1;
    if (maxy > area_y2)
        maxy = area_y2;
    b0 = top_left(&v1, &v2) ? 0 : -1;
    b1 = top_left(&v2, &v0) ? 0 : -1;
    b2 = top_left(&v0, &v1) ? 0 : -1;
    for (y = miny; y <= maxy; y++) {
        for (x = minx; x <= maxx; x++) {
            /* edge functions are negative inside for this winding: flip */
            s64 w0 = -edge(&v1, &v2, x, y), w1 = -edge(&v2, &v0, x, y), w2 = -edge(&v0, &v1, x, y);
            s32 r, g, b, u = 0, v = 0;
            if (w0 + b0 < 0 || w1 + b1 < 0 || w2 + b2 < 0)
                continue;
            if (p->gouraud) {
                r = (s32)((w0 * v0.r + w1 * v1.r + w2 * v2.r) / area);
                g = (s32)((w0 * v0.g + w1 * v1.g + w2 * v2.g) / area);
                b = (s32)((w0 * v0.b + w1 * v1.b + w2 * v2.b) / area);
            } else {
                r = v0.r;
                g = v0.g;
                b = v0.b;
            }
            if (p->textured) {
                u = (s32)((w0 * v0.u + w1 * v1.u + w2 * v2.u) / area);
                v = (s32)((w0 * v0.v + w1 * v1.v + w2 * v2.v) / area);
            }
            plot(p, x, y, r, g, b, u, v);
        }
    }
}

/* ---- lines ---- */

static void line(const Prim *p, Vtx a, Vtx b) {
    s32 dx = b.x - a.x, dy = b.y - a.y, n, i;
    s32 adx = dx < 0 ? -dx : dx, ady = dy < 0 ? -dy : dy;
    if (adx > 1023 || ady > 511)
        return;
    n = adx > ady ? adx : ady;
    for (i = 0; i <= n; i++) {
        s32 x = n ? a.x + (dx * i + (dx >= 0 ? n / 2 : -(n / 2))) / n : a.x;
        s32 y = n ? a.y + (dy * i + (dy >= 0 ? n / 2 : -(n / 2))) / n : a.y;
        s32 r = p->gouraud && n ? a.r + (b.r - a.r) * i / n : a.r;
        s32 g = p->gouraud && n ? a.g + (b.g - a.g) * i / n : a.g;
        s32 bl = p->gouraud && n ? a.b + (b.b - a.b) * i / n : a.b;
        plot(p, x, y, r, g, bl, 0, 0);
    }
}

/* ---- command packets ---- */

static void color_of(u32 w, Vtx *v) {
    v->r = w & 255;
    v->g = (w >> 8) & 255;
    v->b = (w >> 16) & 255;
}

static void xy_of(u32 w, Vtx *v) {
    v->x = sext11(w) + off_x;
    v->y = sext11(w >> 16) + off_y;
}

static int poly(const u32 *w) {
    u32 cmd = w[0] >> 24;
    int quad = (cmd >> 3) & 1, n = quad ? 4 : 3, i, k = 0;
    Prim p;
    Vtx v[4];
    p.gouraud = (cmd >> 4) & 1;
    p.textured = (cmd >> 2) & 1;
    p.raw = cmd & 1;
    p.semi = (cmd >> 1) & 1;
    p.tpage = texpage;
    p.clut_x = p.clut_y = 0;
    for (i = 0; i < n; i++) {
        if (i == 0 || p.gouraud)
            color_of(w[k++], &v[i]);
        else
            v[i].r = v[0].r, v[i].g = v[0].g, v[i].b = v[0].b;
        xy_of(w[k++], &v[i]);
        if (p.textured) {
            u32 t = w[k++];
            v[i].u = t & 255;
            v[i].v = (t >> 8) & 255;
            if (i == 0) {
                p.clut_x = ((t >> 16) & 63) * 16;
                p.clut_y = (t >> 22) & 511;
            } else if (i == 1) {
                /* the polygon's texpage replaces the drawing mode's (bits 0-8, 11) */
                p.tpage = (texpage & ~0x9FFu) | ((t >> 16) & 0x9FF);
                texpage = p.tpage;
            }
        }
    }
    p.dither = dither_on && (p.gouraud || (p.textured && !p.raw));
    triangle(&p, v[0], v[1], v[2]);
    if (quad)
        triangle(&p, v[1], v[2], v[3]);
    return k;
}

static int lines(const u32 *w, int avail) {
    u32 cmd = w[0] >> 24;
    int gouraud = (cmd >> 4) & 1, poly_line = (cmd >> 3) & 1, k = 0;
    Prim p;
    Vtx a, b;
    memset(&p, 0, sizeof(p));
    p.gouraud = gouraud;
    p.semi = (cmd >> 1) & 1;
    p.tpage = texpage;
    p.dither = dither_on && gouraud;
    color_of(w[k++], &a);
    xy_of(w[k++], &a);
    for (;;) {
        if (k >= avail)
            break;
        if (poly_line && (w[k] & 0xF000F000) == 0x50005000)
            return k + 1;
        if (gouraud)
            color_of(w[k++], &b);
        else
            b.r = a.r, b.g = a.g, b.b = a.b;
        xy_of(w[k++], &b);
        line(&p, a, b);
        a = b;
        if (!poly_line)
            break;
    }
    return k;
}

static int rect(const u32 *w) {
    u32 cmd = w[0] >> 24;
    int size = (cmd >> 3) & 3, k = 0;
    s32 x, y, wd, ht, i, j, u0 = 0, v0 = 0;
    Prim p;
    Vtx o;
    memset(&p, 0, sizeof(p));
    p.textured = (cmd >> 2) & 1;
    p.raw = cmd & 1;
    p.semi = (cmd >> 1) & 1;
    p.tpage = texpage;
    color_of(w[k++], &o);
    xy_of(w[k++], &o);
    if (p.textured) {
        u32 t = w[k++];
        u0 = t & 255;
        v0 = (t >> 8) & 255;
        p.clut_x = ((t >> 16) & 63) * 16;
        p.clut_y = (t >> 22) & 511;
    }
    if (size == 0) {
        wd = w[k] & 0x3FF;
        ht = (w[k] >> 16) & 0x1FF;
        k++;
    } else {
        wd = ht = size == 1 ? 1 : size == 2 ? 8 : 16;
    }
    x = o.x;
    y = o.y;
    for (j = 0; j < ht; j++) {
        for (i = 0; i < wd; i++) {
            s32 u = (texpage >> 12) & 1 ? u0 - i : u0 + i;
            s32 v = (texpage >> 13) & 1 ? v0 - j : v0 + j;
            plot(&p, x + i, y + j, o.r, o.g, o.b, u, v);
        }
    }
    return k;
}

static void fill(const u32 *w) {
    u16 c = ((w[0] >> 3) & 31) | (((w[0] >> 11) & 31) << 5) | (((w[0] >> 19) & 31) << 10);
    s32 x = w[1] & 0x3F0, y = (w[1] >> 16) & 0x1FF;
    s32 wd = ((w[2] & 0x3FF) + 15) & ~15, ht = (w[2] >> 16) & 0x1FF, i, j;
    for (j = 0; j < ht; j++)
        for (i = 0; i < wd; i++)
            VRAM(x + i, y + j) = c;
}

void gpu_load_image(s32 x, s32 y, s32 w, s32 h, const u16 *src) {
    s32 i, j;
    for (j = 0; j < h; j++)
        for (i = 0; i < w; i++)
            VRAM(x + i, y + j) = *src++;
}

void gpu_store_image(s32 x, s32 y, s32 w, s32 h, u16 *dst) {
    s32 i, j;
    for (j = 0; j < h; j++)
        for (i = 0; i < w; i++)
            *dst++ = VRAM(x + i, y + j);
}

void gpu_move_image(s32 sx, s32 sy, s32 w, s32 h, s32 dx, s32 dy) {
    static u16 tmp[512 * 1024];
    gpu_store_image(sx, sy, w, h, tmp);
    gpu_load_image(dx, dy, w, h, tmp);
}

void gpu_fill_rect(s32 x, s32 y, s32 w, s32 h, u16 c) {
    s32 i, j;
    for (j = 0; j < h; j++)
        for (i = 0; i < w; i++)
            VRAM(x + i, y + j) = c;
}

static void env(u32 w) {
    switch (w >> 24) {
    case 0xE1:
        texpage = w & 0x3FFF;
        dither_on = (w >> 9) & 1;
        break;
    case 0xE2:
        tw_mask_x = w & 31;
        tw_mask_y = (w >> 5) & 31;
        tw_off_x = (w >> 10) & 31;
        tw_off_y = (w >> 15) & 31;
        break;
    case 0xE3:
        area_x1 = w & 1023;
        area_y1 = (w >> 10) & 511;
        break;
    case 0xE4:
        area_x2 = w & 1023;
        area_y2 = (w >> 10) & 511;
        break;
    case 0xE5:
        off_x = sext11(w);
        off_y = sext11(w >> 11);
        break;
    case 0xE6:
        mask_set = w & 1;
        mask_check = (w >> 1) & 1;
        break;
    }
}

/* Run `n` GP0 words (one packet's payload; may hold several commands). */
void gpu_gp0(const u32 *w, int n) {
    int k = 0;
    while (k < n) {
        u32 cmd = w[k] >> 24;
        int used = 1;
        if (cmd >= 0x20 && cmd < 0x40)
            used = poly(w + k);
        else if (cmd >= 0x40 && cmd < 0x60)
            used = lines(w + k, n - k);
        else if (cmd >= 0x60 && cmd < 0x80)
            used = rect(w + k);
        else if (cmd == 0x02) {
            fill(w + k);
            used = 3;
        } else if (cmd >= 0x80 && cmd < 0xA0) {
            gpu_move_image(w[k + 1] & 1023, (w[k + 1] >> 16) & 511, ((w[k + 3] - 1) & 1023) + 1,
                           (((w[k + 3] >> 16) - 1) & 511) + 1, w[k + 2] & 1023, (w[k + 2] >> 16) & 511);
            used = 4;
        } else if (cmd >= 0xA0 && cmd < 0xC0) {
            s32 x = w[k + 1] & 1023, y = (w[k + 1] >> 16) & 511;
            s32 wd = ((w[k + 2] - 1) & 1023) + 1, ht = (((w[k + 2] >> 16) - 1) & 511) + 1;
            gpu_load_image(x, y, wd, ht, (const u16 *)&w[k + 3]);
            used = 3 + (wd * ht + 1) / 2;
        } else if (cmd >= 0xE1 && cmd <= 0xE6)
            env(w[k]);
        k += used;
    }
}

/* drawing environment from libgpu's PutDrawEnv */
void gpu_set_draw_area(s32 x1, s32 y1, s32 x2, s32 y2) {
    area_x1 = x1;
    area_y1 = y1;
    area_x2 = x2;
    area_y2 = y2;
}

void gpu_set_draw_offset(s32 x, s32 y) {
    off_x = x;
    off_y = y;
}

void gpu_set_texpage(u32 tp, int dither, int draw_on_display) {
    texpage = (tp & 0x9FF) | (dither ? 0x200 : 0) | (draw_on_display ? 0x400 : 0);
    dither_on = dither;
}

void gpu_set_texwindow(u32 mx, u32 my, u32 ox, u32 oy) {
    tw_mask_x = mx;
    tw_mask_y = my;
    tw_off_x = ox;
    tw_off_y = oy;
}
