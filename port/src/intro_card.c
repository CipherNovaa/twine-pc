/* The port's title card: "PORT BY" over "CipherNovaa", on black, after the BIOS boot animation and before
 * the game. It is drawn on the CPU at the window's own pixel size from the signed distance fields in
 * intro_font.h (port/tools/bake_intro_font.py), so the lettering is sharp at any resolution: smooth edges,
 * a soft bloom, a hairline rule. Light is mixed as light (gamma 2), faded evenly on screen and dithered
 * so the fades and the bloom don't band. Only the box around the text is drawn; it goes to the window 1:1
 * through video.c's SDL_Renderer.
 *
 * Timeline (s): the label fades in from 0.3 while its letter-spacing settles, the name from 0.55, the rule
 * draws out from 0.9; everything holds, fades out from 3.65 and the card ends at 4.9; a very slow push-in
 * runs throughout. Start, Cross, Enter or Esc fades it out at once (a second press cuts it).
 *
 * Headless (TWINE_INTRO=1) it runs on a 60 Hz clock at TWINE_INTRO_SIZE=<w>x<h> (default 1280x960) and
 * only renders, writing the frames TWINE_INTRO_SHOT lists. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "port.h"
#include "intro.h"
#include "intro_font.h"

#define CARD_END 4.9f
#define FADE_OUT 3.65f
#define LIN(v) (((v) / 255.0f) * ((v) / 255.0f)) /* display value -> light (gamma 2, see encode) */

typedef struct {
    float r, g, b;
} Rgb;

static u8 *fields;   /* decoded distance fields */
static float *lum;   /* the text box: light of the lettering, linear, relative to the ink colour */
static u32 *pix;     /* the box's pixels */
static int lum_cap;
static int box_x, box_y, box_w, box_h, out_w, out_h;
static float noise[64 * 64];
static Rgb ink, halo; /* the lettering's colour, the bloom's tint times its strength */

/* the bloom, at quarter resolution, and where each box column samples it; encode()'s rows */
static float *bl, *bl_tmp, *col_f, *brow, *nrow;
static int bl_w, bl_h, bl_f, bl_cap, col_cap, *col_i, row_cap;

static void init_tables(void) {
    static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const char *s = intro_font_b64;
    u32 bits = 0, n = 0, i = 0, h = 0x1234567;
    int k;
    fields = malloc(INTRO_FONT_BYTES + 4);
    for (; *s && *s != '='; s++) {
        const char *p = strchr(alphabet, *s);
        if (!p)
            continue;
        bits = (bits << 6) | (u32)(p - alphabet);
        n += 6;
        if (n >= 8) {
            n -= 8;
            if (i < INTRO_FONT_BYTES)
                fields[i++] = (u8)(bits >> n);
        }
    }
    for (k = 0; k < 64 * 64; k++) { /* dither: a tile of white noise in (-0.5, 0.5) */
        h ^= h << 13;
        h ^= h >> 17;
        h ^= h << 5;
        noise[k] = (h & 0xFFFF) / 65536.0f - 0.5f;
    }
}

static float clamp01(float x) {
    return x < 0 ? 0 : x > 1 ? 1 : x;
}

static float ease_in_out(float x) {
    x = clamp01(x);
    return x < 0.5f ? 4 * x * x * x : 1 - powf(-2 * x + 2, 3) / 2;
}

static float ease_out(float x) {
    x = clamp01(x);
    return 1 - (1 - x) * (1 - x) * (1 - x);
}

/* a sample coordinate (pixel centres at .5) -> the lower of the two texels it lies between (0..n-2) and
 * the weight of the upper one; outside it clamps to the border */
static int texel(float u, int n, float *frac) {
    int i;
    if (u <= 0) {
        *frac = 0;
        return 0;
    }
    i = (int)u;
    if (i >= n - 1) {
        *frac = 1;
        return n - 2;
    }
    *frac = u - i;
    return i;
}

/* One glyph's field with its top-left corner at (x0, y0), k screen pixels per field pixel, at light
 * `level`. Overlapping glyph cells keep the brighter value. */
static void draw_glyph(const IntroGlyph *g, float spread, float x0, float y0, float k, float level) {
    const u8 *f = fields + g->offset;
    /* coverage = distance in screen pixels + 1/2 = byte * scale - bias */
    float inv = 1 / k, scale = 2 * spread * k / 255.0f, bias = spread * k - 0.5f;
    int sx0 = (int)floorf(x0), sy0 = (int)floorf(y0), sx1 = (int)ceilf(x0 + g->w * k);
    int sy1 = (int)ceilf(y0 + g->h * k), sx, sy, *ui;
    float *uf;
    sx0 = sx0 < box_x ? box_x : sx0;
    sy0 = sy0 < box_y ? box_y : sy0;
    sx1 = sx1 > box_x + box_w ? box_x + box_w : sx1;
    sy1 = sy1 > box_y + box_h ? box_y + box_h : sy1;
    if (sx1 <= sx0 || sy1 <= sy0)
        return;
    uf = malloc((sx1 - sx0) * (sizeof(float) + sizeof(int)));
    ui = (int *)(uf + (sx1 - sx0));
    for (sx = sx0; sx < sx1; sx++)
        ui[sx - sx0] = texel((sx + 0.5f - x0) * inv - 0.5f, g->w, &uf[sx - sx0]);
    for (sy = sy0; sy < sy1; sy++) {
        float fy, *p = lum + (sy - box_y) * box_w + (sx0 - box_x);
        const u8 *r0 = f + texel((sy + 0.5f - y0) * inv - 0.5f, g->h, &fy) * g->w, *r1 = r0 + g->w;
        for (sx = 0; sx < sx1 - sx0; sx++) {
            int i = ui[sx];
            float fx = uf[sx], top = r0[i] + (r0[i + 1] - r0[i]) * fx, bot = r1[i] + (r1[i + 1] - r1[i]) * fx;
            float cover = (top + (bot - top) * fy) * scale - bias, v;
            if (cover <= 0)
                continue;
            v = (cover > 1 ? 1 : cover) * level;
            if (v > p[sx])
                p[sx] = v;
        }
    }
    free(uf);
}

/* width of a line with extra spacing `track` (em) after each character but the last */
static float line_width(const IntroLine *L, float em, float track) {
    return (L->advance + track * (float)(strlen(L->text) - 1)) * em;
}

/* One line of text: pen at x on the baseline, `em` screen pixels per em, extra spacing `track` (em per
 * character). */
static void draw_line(const IntroLine *L, float x, float baseline, float em, float track, float level) {
    float k = em / L->em;
    int i, chr = 0;
    for (i = 0; i < L->n; i++) {
        const IntroGlyph *g = &intro_glyphs[L->glyph[i]];
        while (L->text[chr] == ' ')
            chr++;
        draw_glyph(g, L->spread, x + (L->pen[i] + track * chr) * em + g->left * k, baseline + g->top * k, k, level);
        chr++;
    }
}

/* A hairline centred on (cx, y), half-width hw, brightest in the middle and fading towards its ends. */
static void draw_rule(float cx, float y, float hw, float thick, float level) {
    int sx, sy;
    if (hw < 1)
        return;
    for (sy = (int)floorf(y - thick / 2); sy <= (int)ceilf(y + thick / 2); sy++) {
        float cover = clamp01(fminf(sy + 1, y + thick / 2) - fmaxf(sy, y - thick / 2));
        if (cover <= 0 || sy < box_y || sy >= box_y + box_h)
            continue;
        for (sx = (int)floorf(cx - hw); sx <= (int)ceilf(cx + hw); sx++) {
            float a = 1 - clamp01((fabsf(sx + 0.5f - cx) / hw - 0.15f) / 0.85f), *p;
            if (sx < box_x || sx >= box_x + box_w)
                continue;
            p = lum + (sy - box_y) * box_w + (sx - box_x);
            a = a * a * (3 - 2 * a) * cover * level;
            if (a > *p)
                *p = a;
        }
    }
}

/* The bloom around the lettering: its light at 1/4 resolution or less (whatever keeps the blur short),
 * blurred (Gaussian, `radius` screen pixels at about two sigma). encode() adds it back. */
static void bloom(float radius) {
    int x, y, i, r, f = 4;
    float sigma, w[48], sum = 0;
    while (radius / (2 * f) > 6 && f < 16)
        f *= 2;
    sigma = radius / (2 * f);
    bl_f = f;
    bl_w = (box_w + f - 1) / f;
    bl_h = (box_h + f - 1) / f;
    if (bl_w * bl_h > bl_cap) {
        bl_cap = bl_w * bl_h;
        bl = realloc(bl, bl_cap * sizeof(float));
        bl_tmp = realloc(bl_tmp, bl_cap * sizeof(float));
    }
    if (box_w > col_cap) {
        col_cap = box_w;
        col_i = realloc(col_i, col_cap * sizeof(int));
        col_f = realloc(col_f, col_cap * sizeof(float));
    }
    if (bl_w < 2 || bl_h < 2 || sigma < 0.5f) {
        bl_w = 0;
        return;
    }
    r = (int)ceilf(sigma * 3);
    r = r > 47 ? 47 : r;
    for (i = 0; i <= r; i++)
        sum += (w[i] = expf(-(float)(i * i) / (2 * sigma * sigma))) * (i ? 2 : 1);
    for (i = 0; i <= r; i++)
        w[i] /= sum;
    for (y = 0; y < bl_h; y++)
        for (x = 0; x < bl_w; x++) {
            float s = 0;
            int yy, xx;
            for (yy = y * f; yy < y * f + f && yy < box_h; yy++)
                for (xx = x * f; xx < x * f + f && xx < box_w; xx++)
                    s += lum[yy * box_w + xx];
            bl[y * bl_w + x] = s / (f * f);
        }
    for (y = 0; y < bl_h; y++)
        for (x = 0; x < bl_w; x++) {
            const float *row = bl + y * bl_w;
            float s = row[x] * w[0];
            for (i = 1; i <= r; i++)
                s += w[i] * ((x - i >= 0 ? row[x - i] : 0) + (x + i < bl_w ? row[x + i] : 0));
            bl_tmp[y * bl_w + x] = s;
        }
    for (y = 0; y < bl_h; y++)
        for (x = 0; x < bl_w; x++) {
            float s = bl_tmp[y * bl_w + x] * w[0];
            for (i = 1; i <= r; i++)
                s += w[i] * ((y - i >= 0 ? bl_tmp[(y - i) * bl_w + x] : 0) +
                             (y + i < bl_h ? bl_tmp[(y + i) * bl_w + x] : 0));
            bl[y * bl_w + x] = s;
        }
    for (x = 0; x < box_w; x++)
        col_i[x] = texel((x + 0.5f) / f - 0.5f, bl_w, &col_f[x]);
}

/* Draw the card at time t (s) into the text box; `fade` is the skip fade-out (1 = none). */
static void render(float t, float fade) {
    float u = fminf((float)out_w, out_h * 16.0f / 9.0f) / 1920.0f;
    float push = 1 + 0.03f * t / CARD_END; /* slow push-in */
    float out = (1 - ease_in_out((t - FADE_OUT) / 1.0f)) * fade;
    float a_label = ease_in_out((t - 0.30f) / 1.0f) * out, a_title = ease_in_out((t - 0.55f) / 1.1f) * out;
    float grow = ease_out((t - 0.9f) / 1.5f);
    float title_em = 132 * u * push, label_em = 24 * u * push;
    float track = 0.42f + 0.24f * (1 - ease_out((t - 0.3f) / 2.8f)), title_track = 0.012f;
    float title_w = line_width(&intro_title, title_em, title_track);
    float label_w = line_width(&intro_label, label_em, track);
    float cap_t = intro_title.cap_height * title_em, cap_l = intro_label.cap_height * label_em;
    float gap = 0.44f * title_em, rule_gap = 0.50f * title_em;
    float group = cap_l + gap + cap_t + rule_gap;
    float cx = out_w / 2.0f, top = out_h / 2.0f - group / 2 - 0.02f * out_h;
    float label_base = top + cap_l, title_base = label_base + gap + cap_t, rule_y = title_base + rule_gap;
    float glow_r = 0.30f * title_em, margin = glow_r * 1.6f + 4;
    float wide = fmaxf(title_w, label_w);
    int need;
    /* ivory lettering; the label and the rule are dimmer: (168/240)^2 and (150/240)^2 */
    ink.r = LIN(240), ink.g = LIN(236), ink.b = LIN(228);
    halo.r = LIN(255) * 0.11f, halo.g = LIN(238) * 0.11f, halo.b = LIN(214) * 0.11f;

    box_x = (int)floorf(cx - wide / 2 - margin);
    box_y = (int)floorf(label_base - cap_l - margin);
    box_w = (int)ceilf(cx + wide / 2 + margin) - box_x;
    box_h = (int)ceilf(rule_y + margin) - box_y;
    if (box_x < 0)
        box_w += box_x, box_x = 0;
    if (box_y < 0)
        box_h += box_y, box_y = 0;
    if (box_x + box_w > out_w)
        box_w = out_w - box_x;
    if (box_y + box_h > out_h)
        box_h = out_h - box_y;
    if (box_w <= 0 || box_h <= 0) {
        box_w = box_h = 0;
        return;
    }
    need = box_w * box_h;
    if (need > lum_cap) {
        lum = realloc(lum, need * sizeof(float));
        pix = realloc(pix, need * sizeof(u32));
        lum_cap = need;
    }
    memset(lum, 0, need * sizeof(float));
    /* fades are applied as light (squared), so they look even on screen */
    draw_line(&intro_label, cx - label_w / 2, label_base, label_em, track, 0.49f * a_label * a_label);
    draw_line(&intro_title, cx - title_w / 2, title_base, title_em, title_track, a_title * a_title);
    draw_rule(cx, rule_y, title_w * 0.30f * grow, fmaxf(1.0f, 1.4f * u), 0.39f * grow * out * grow * out);
    bloom(glow_r);
}

/* Lettering and bloom -> display pixels (gamma 2: a square root), with a sub-LSB noise added before
 * rounding so the gradients don't band. The last loop is plain arithmetic the compiler vectorizes. */
static void encode(void) {
    int x, y;
    if (box_w > row_cap) {
        row_cap = box_w;
        brow = realloc(brow, row_cap * sizeof(float));
        nrow = realloc(nrow, row_cap * sizeof(float));
    }
    for (y = 0; y < box_h; y++) {
        const float *l = lum + y * box_w, *n = noise + ((box_y + y) & 63) * 64;
        u32 *o = pix + y * box_w;
        float ir = ink.r, ig = ink.g, ib = ink.b, hr = halo.r, hg = halo.g, hb = halo.b;
        if (bl_w) {
            float fy;
            const float *r0 = bl + texel((y + 0.5f) / bl_f - 0.5f, bl_h, &fy) * bl_w, *r1 = r0 + bl_w;
            for (x = 0; x < box_w; x++) {
                int j = col_i[x];
                float fx = col_f[x], a = r0[j] + (r0[j + 1] - r0[j]) * fx, c = r1[j] + (r1[j + 1] - r1[j]) * fx;
                brow[x] = a + (c - a) * fy;
            }
        } else
            memset(brow, 0, box_w * sizeof(float));
        for (x = 0; x < box_w; x++)
            nrow[x] = n[(box_x + x) & 63] + 0.5f;
        for (x = 0; x < box_w; x++) {
            float t = l[x], b = brow[x], e = nrow[x];
            float r = sqrtf(t * ir + b * hr) * 255 + e, g = sqrtf(t * ig + b * hg) * 255 + e;
            float u = sqrtf(t * ib + b * hb) * 255 + e;
            r = r > 255 ? 255 : r;
            g = g > 255 ? 255 : g;
            u = u > 255 ? 255 : u;
            o[x] = (u32)(int)r << 16 | (u32)(int)g << 8 | (u32)(int)u;
        }
    }
}

static void dump(const char *path) {
    FILE *f = fopen(path, "wb");
    u8 *row;
    int x, y;
    if (!f)
        return;
    row = calloc(out_w, 3);
    fprintf(f, "P6\n%d %d\n255\n", out_w, out_h);
    for (y = 0; y < out_h; y++) {
        memset(row, 0, out_w * 3);
        if (y >= box_y && y < box_y + box_h)
            for (x = 0; x < box_w; x++) {
                u32 c = pix[(y - box_y) * box_w + x];
                u8 *o = row + (box_x + x) * 3;
                o[0] = (u8)(c >> 16);
                o[1] = (u8)(c >> 8);
                o[2] = (u8)c;
            }
        fwrite(row, 1, out_w * 3, f);
    }
    free(row);
    fclose(f);
}

void intro_card_run(void) {
    SDL_Renderer *r = port_video_renderer();
    SDL_Texture *tex = NULL;
    int tex_w = 0, tex_h = 0, headless = port_video_headless(), frame = 0;
    float skip_at = -1;
    Uint64 start = 0, last = 0, host_start = SDL_GetTicksNS();
    init_tables();
    if (headless) {
        const char *size = getenv("TWINE_INTRO_SIZE");
        out_w = 1280;
        out_h = 960;
        if (size && (sscanf(size, "%dx%d", &out_w, &out_h) != 2 || out_w < 16 || out_h < 16))
            out_w = 1280, out_h = 960;
    } else {
        SDL_SetRenderVSync(r, 1);
        start = SDL_GetTicksNS();
    }
    for (;; frame++) {
        float t = headless ? frame / 60.0f : (SDL_GetTicksNS() - start) / 1e9f;
        float fade = skip_at < 0 ? 1 : 1 - ease_in_out((t - skip_at) / 0.35f);
        u32 n = intro_next_frame();
        if (t >= CARD_END || (skip_at >= 0 && t >= skip_at + 0.4f))
            break;
        if (!headless)
            SDL_GetCurrentRenderOutputSize(r, &out_w, &out_h);
        render(t, fade);
        encode();
        if (intro_shot_wanted(n)) {
            char path[64];
            snprintf(path, sizeof(path), "intro_%04u.ppm", n);
            dump(path);
            port_log("intro frame %u (card, %.2f s): wrote %s (%dx%d)\n", n, t, path, out_w, out_h);
        }
        if (headless)
            continue;
        if (port_video_poll_skip()) {
            if (skip_at >= 0)
                break;
            skip_at = t;
        }
        if (box_w > tex_w || box_h > tex_h) {
            if (tex)
                SDL_DestroyTexture(tex);
            tex_w = box_w > tex_w ? box_w : tex_w;
            tex_h = box_h > tex_h ? box_h : tex_h;
            tex = SDL_CreateTexture(r, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, tex_w, tex_h);
            SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
        }
        SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
        SDL_RenderClear(r);
        if (tex && box_w > 0 && box_h > 0) {
            SDL_Rect area = {0, 0, box_w, box_h};
            SDL_FRect src = {0, 0, (float)box_w, (float)box_h};
            SDL_FRect dst = {(float)box_x, (float)box_y, (float)box_w, (float)box_h};
            SDL_UpdateTexture(tex, &area, pix, box_w * 4);
            SDL_RenderTexture(r, tex, &src, &dst);
        }
        SDL_RenderPresent(r);
        if (SDL_GetTicksNS() - last < 4000000) /* no vsync after all: don't spin faster than 250 Hz */
            SDL_DelayNS(4000000 - (SDL_GetTicksNS() - last));
        last = SDL_GetTicksNS();
    }
    if (!headless) {
        SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
        SDL_RenderClear(r);
        SDL_RenderPresent(r);
        SDL_SetRenderVSync(r, 0);
        if (tex)
            SDL_DestroyTexture(tex);
    }
    port_log("intro: title card %s after %d frames at %dx%d (%.2f s on the host)\n", skip_at >= 0 ? "skipped" : "done",
             frame, out_w, out_h, (SDL_GetTicksNS() - host_start) / 1e9);
    free(lum);
    free(pix);
    free(fields);
    free(bl);
    free(bl_tmp);
    free(col_i);
    free(col_f);
    free(brow);
    free(nrow);
    lum = bl = bl_tmp = col_f = brow = nrow = NULL;
    pix = NULL;
    fields = NULL;
    col_i = NULL;
    lum_cap = bl_cap = col_cap = row_cap = 0;
}
