/* Check the hardware renderer (port/src/gpu_hw.c) against the software GPU (port/src/gpu.c) on random GP0
 * commands, including what the game itself never uses: VRAM copies, the mask bit (set and checked), texture
 * windows, 15-bit textures, flipped sprites, lines and polylines. Each scene starts from a reset VRAM with random
 * textures and CLUTs, runs a few thousand random commands and compares all of VRAM (scale 1, 15-bit colour).
 *
 *   make -C port hwtest   (-> build/port/hwtest; needs a Vulkan GPU, draws offscreen)
 *   hwtest [SEED] [COMMANDS]
 *
 * Scenes: opaque primitives; semi-transparency; texture windows and flips; mask set and checked (with
 * semi-transparency modes 1-3); mask checked with mode 0 (approximate in the hardware renderer); copies, fills
 * and images among them. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "port.h"
#include "gpu.h"
#include "gpu_hw.h"

volatile int port_rt_depth;

void port_rt_leave(void) {
    port_rt_depth--;
}

void port_log(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}

void port_fatal(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(1);
}

static u32 seed = 1;
static int mode0; /* semi-transparency mode 0 only (1), never (-1) */

static u32 rnd(u32 n) {
    seed = seed * 1103515245u + 12345u;
    return ((seed >> 8) & 0xFFFFFF) % n;
}

static u32 xy(s32 x, s32 y) {
    return (u32)(x & 0xFFFF) | ((u32)y << 16);
}

static u32 rgb(void) {
    return rnd(256) | (rnd(256) << 8) | (rnd(256) << 16);
}

/* A texpage: 4/8/15-bit (never the reserved 3), semi-transparency mode, a page in the texture area (x >= 512)
 * that doesn't reach past VRAM's right edge (it would wrap around into what is being drawn, which the hardware
 * renderer can only follow from batch to batch, not within a primitive). */
static u32 tpage(void) {
    u32 depth = rnd(3);
    return (8 + rnd(depth == 2 ? 5 : depth == 1 ? 7 : 8)) | (rnd(2) << 4) |
           ((mode0 > 0 ? 0 : mode0 < 0 ? 1 + rnd(3) : rnd(4)) << 5) | (depth << 7);
}

static u32 clut(void) {
    return ((512 + rnd(32) * 16) >> 4) | ((480 + rnd(32)) << 6);
}

static void cmd(const u32 *w, int n) {
    gpu_gp0(w, n);
}

static void environment(int windows, int masks) {
    u32 w;
    s32 x1 = rnd(64), y1 = rnd(64);
    w = 0xE3000000 | x1 | (y1 << 10);
    cmd(&w, 1);
    w = 0xE4000000 | (x1 + 200 + rnd(240)) | ((y1 + 150 + rnd(200)) << 10); /* (left of the textures) */
    cmd(&w, 1);
    w = 0xE5000000 | (rnd(64) & 0x7FF) | ((rnd(64) & 0x7FF) << 11);
    cmd(&w, 1);
    w = 0xE1000000 | tpage() | (rnd(2) << 9) | (windows ? rnd(4) << 12 : 0);
    cmd(&w, 1);
    w = 0xE2000000 | (windows && rnd(2) ? rnd(32) | (rnd(32) << 5) | (rnd(32) << 10) | (rnd(32) << 15) : 0);
    cmd(&w, 1);
    w = 0xE6000000 | (masks ? rnd(4) : 0);
    cmd(&w, 1);
}

static void polygon(int semi) {
    u32 w[16], c = 0x20 | (rnd(2) << 4) | (rnd(2) << 3) | (rnd(2) << 2) | (semi ? rnd(2) << 1 : 0) | rnd(2);
    int gouraud = (c >> 4) & 1, quad = (c >> 3) & 1, textured = (c >> 2) & 1, n = quad ? 4 : 3, i, k = 0;
    s32 cx = rnd(400), cy = rnd(300), size = 4 + rnd(rnd(4) ? 60 : 300);
    for (i = 0; i < n; i++) {
        if (i == 0 || gouraud)
            w[k++] = (i == 0 ? c << 24 : 0) | rgb();
        w[k++] = xy(cx + (s32)rnd(size) - size / 4, cy + (s32)rnd(size) - size / 4);
        if (textured)
            w[k++] = rnd(256) | (rnd(256) << 8) | (i == 0 ? clut() << 16 : i == 1 ? tpage() << 16 : 0);
    }
    cmd(w, k);
}

static void line(int semi) {
    u32 w[32], c = 0x40 | (rnd(2) << 4) | (rnd(2) << 3) | (semi ? rnd(2) << 1 : 0);
    int gouraud = (c >> 4) & 1, poly = (c >> 3) & 1, n = poly ? 2 + rnd(5) : 2, i, k = 0;
    s32 x = rnd(500), y = rnd(400);
    for (i = 0; i < n; i++) {
        if (i == 0 || gouraud)
            w[k++] = (i == 0 ? c << 24 : 0) | rgb();
        w[k++] = xy(x, y);
        x += (s32)rnd(160) - 80, y += (s32)rnd(120) - 60;
    }
    if (poly)
        w[k++] = 0x55555555;
    cmd(w, k);
}

static void sprite(int semi) {
    u32 w[4], c = 0x60 | (rnd(4) << 3) | (rnd(2) << 2) | (semi ? rnd(2) << 1 : 0) | rnd(2);
    int k = 0;
    w[k++] = (c << 24) | rgb();
    w[k++] = xy(rnd(500), rnd(400));
    if (c & 4)
        w[k++] = rnd(256) | (rnd(256) << 8) | (clut() << 16);
    if (((c >> 3) & 3) == 0)
        w[k++] = (1 + rnd(120)) | ((1 + rnd(100)) << 16);
    cmd(w, k);
}

static void vram_op(void) {
    u32 w[3 + 64 * 64 / 2];
    s32 i, wd, ht;
    switch (rnd(3)) {
    case 0: /* fill */
        w[0] = 0x02000000 | rgb();
        w[1] = xy(rnd(64) * 16, rnd(512));
        w[2] = (16 + rnd(200)) | ((1 + rnd(200)) << 16);
        cmd(w, 3);
        break;
    case 1: /* copy within VRAM */
        w[0] = 0x80000000;
        w[1] = xy(rnd(1024), rnd(512));
        w[2] = xy(rnd(1024), rnd(512));
        w[3] = (1 + rnd(200)) | ((1 + rnd(200)) << 16);
        cmd(w, 4);
        break;
    default: /* an image from the CPU */
        wd = 1 + rnd(64), ht = 1 + rnd(64);
        w[0] = 0xA0000000;
        w[1] = xy(rnd(1024), rnd(512));
        w[2] = wd | (ht << 16);
        for (i = 0; i < (wd * ht + 1) / 2; i++)
            w[3 + i] = rnd(0x10000) | (rnd(0x10000) << 16);
        cmd(w, 3 + (wd * ht + 1) / 2);
        break;
    }
}

/* random textures and CLUTs in x 512-1023 (some texels with bit 15, some 0 = transparent) */
static void textures(void) {
    static u16 img[512 * 512];
    int i;
    for (i = 0; i < 512 * 512; i++) {
        u32 r = rnd(16);
        img[i] = r == 0 ? 0 : (u16)(rnd(0x8000) | (r < 5 ? 0x8000 : 0));
    }
    gpu_load_image(512, 0, 512, 512, img);
}

int main(int argc, char **argv) {
    static const char *const names[] = {"opaque", "semi-transparent", "texture windows, flips",
                                        "mask bit (semi-transparency modes 1-3)",
                                        "mask check with mode 0 (approximate)", "copies, fills, images"};
    int commands = argc > 2 ? atoi(argv[2]) : 3000, scene, i;
    seed = argc > 1 ? (u32)atoi(argv[1]) : 1;
    setenv("TWINE_HW_COMPARE", "1", 1);
    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "offscreen");
    if (!SDL_Init(SDL_INIT_VIDEO) || !gpu_hw_init())
        port_fatal("hwtest: no hardware renderer: %s\n", SDL_GetError());
    for (scene = 0; scene < 6; scene++) {
        int semi = scene == 1 || scene >= 3, windows = scene == 2 || scene == 5, masks = scene == 3 || scene == 4;
        printf("scene %d: %s\n", scene + 1, names[scene]);
        mode0 = scene == 4 ? 1 : scene == 3 ? -1 : 0;
        gpu_reset();
        textures();
        environment(windows, masks);
        for (i = 0; i < commands; i++) {
            u32 r = rnd(100);
            if (r < 3)
                environment(windows, masks);
            if (scene == 5 && r < 10)
                vram_op();
            else if (r < 60)
                polygon(semi);
            else if (r < 75)
                line(semi);
            else
                sprite(semi);
        }
        gpu_hw_frame(scene + 1, 0, 0, 1024, 512, NULL);
    }
    return 0;
}
