/* Window, display and input (SDL3). Each game frame (VSync) the displayed VRAM rectangle is shown scaled
 * to the window; keyboard and gamepad fill the pad state libpad.c hands the game every vblank.
 * TWINE_HEADLESS=1 runs without a window; TWINE_SHOT=<frame>[,<frame>...] dumps frames as PPM. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "port.h"
#include "gpu.h"

extern u16 port_pad_buttons[2];
extern u8 port_pad_sticks[2][4];

static u32 frames;
static int headless;
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static SDL_Gamepad *gamepad;
static u32 pixels[640 * 512];

/* pad bits as libpad delivers them (active low): low byte SELECT L3 R3 START UP RIGHT DOWN LEFT,
 * high byte L2 R2 L1 R1 TRIANGLE CIRCLE CROSS SQUARE */
enum {
    P_SELECT = 0x1, P_L3 = 0x2, P_R3 = 0x4, P_START = 0x8, P_UP = 0x10, P_RIGHT = 0x20, P_DOWN = 0x40,
    P_LEFT = 0x80, P_L2 = 0x100, P_R2 = 0x200, P_L1 = 0x400, P_R1 = 0x800, P_TRI = 0x1000, P_CIRC = 0x2000,
    P_CROSS = 0x4000, P_SQUARE = 0x8000
};

static const struct {
    SDL_Scancode key;
    u16 bit;
} keymap[] = {
    {SDL_SCANCODE_UP, P_UP}, {SDL_SCANCODE_DOWN, P_DOWN}, {SDL_SCANCODE_LEFT, P_LEFT},
    {SDL_SCANCODE_RIGHT, P_RIGHT}, {SDL_SCANCODE_RETURN, P_START}, {SDL_SCANCODE_RSHIFT, P_SELECT},
    {SDL_SCANCODE_BACKSPACE, P_SELECT}, {SDL_SCANCODE_SPACE, P_CROSS}, {SDL_SCANCODE_Z, P_CROSS},
    {SDL_SCANCODE_X, P_CIRC}, {SDL_SCANCODE_C, P_SQUARE}, {SDL_SCANCODE_V, P_TRI}, {SDL_SCANCODE_Q, P_L1},
    {SDL_SCANCODE_E, P_R1}, {SDL_SCANCODE_1, P_L2}, {SDL_SCANCODE_3, P_R2},
};

static const struct {
    SDL_GamepadButton b;
    u16 bit;
} padmap[] = {
    {SDL_GAMEPAD_BUTTON_DPAD_UP, P_UP}, {SDL_GAMEPAD_BUTTON_DPAD_DOWN, P_DOWN},
    {SDL_GAMEPAD_BUTTON_DPAD_LEFT, P_LEFT}, {SDL_GAMEPAD_BUTTON_DPAD_RIGHT, P_RIGHT},
    {SDL_GAMEPAD_BUTTON_START, P_START}, {SDL_GAMEPAD_BUTTON_BACK, P_SELECT},
    {SDL_GAMEPAD_BUTTON_SOUTH, P_CROSS}, {SDL_GAMEPAD_BUTTON_EAST, P_CIRC}, {SDL_GAMEPAD_BUTTON_WEST, P_SQUARE},
    {SDL_GAMEPAD_BUTTON_NORTH, P_TRI}, {SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, P_L1},
    {SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, P_R1}, {SDL_GAMEPAD_BUTTON_LEFT_STICK, P_L3},
    {SDL_GAMEPAD_BUTTON_RIGHT_STICK, P_R3},
};

void port_video_init(void) {
    headless = getenv("TWINE_HEADLESS") != NULL;
    if (headless)
        return;
    RT_ENTER();
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
        port_fatal("SDL_Init: %s\n", SDL_GetError());
    if (!SDL_CreateWindowAndRenderer("TWINE", 1280, 960, SDL_WINDOW_RESIZABLE, &window, &renderer))
        port_fatal("SDL window: %s\n", SDL_GetError());
    SDL_SetRenderVSync(renderer, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XBGR8888, SDL_TEXTUREACCESS_STREAMING, 640, 512);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    RT_LEAVE();
}

static void dump_display(const char *path) {
    FILE *f;
    s32 x, y, w = gpu_display.w, h = gpu_display.h;
    if (getenv("TWINE_SHOT_VRAM")) {
        GpuDisplay all = {0, 0, 1024, 512, 0, 0}, keep = gpu_display;
        gpu_display = all;
        w = 1024;
        h = 512;
        (void)keep;
        f = fopen(path, "wb");
        if (!f)
            return;
        fprintf(f, "P6\n%d %d\n255\n", w, h);
        for (y = 0; y < h; y++)
            for (x = 0; x < w; x++) {
                u16 c = gpu_vram[y * 1024 + x];
                u8 rgb[3] = {(u8)((c & 31) << 3), (u8)(((c >> 5) & 31) << 3), (u8)(((c >> 10) & 31) << 3)};
                fwrite(rgb, 1, 3, f);
            }
        fclose(f);
        gpu_display = keep;
        return;
    }
    if (w <= 0 || h <= 0 || w > 1024 || h > 512)
        return;
    f = fopen(path, "wb");
    if (!f)
        return;
    fprintf(f, "P6\n%d %d\n255\n", w, h);
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            u16 c = gpu_vram[((gpu_display.y + y) & 511) * 1024 + ((gpu_display.x + x) & 1023)];
            u8 rgb[3] = {(u8)((c & 31) << 3), (u8)(((c >> 5) & 31) << 3), (u8)(((c >> 10) & 31) << 3)};
            fwrite(rgb, 1, 3, f);
        }
    }
    fclose(f);
}

static void maybe_shot(void) {
    const char *shots = getenv("TWINE_SHOT");
    char want[16], list[256], *s;
    if (!shots)
        return;
    snprintf(want, sizeof(want), "%u", frames);
    strncpy(list, shots, sizeof(list) - 1);
    list[sizeof(list) - 1] = 0;
    for (s = strtok(list, ","); s; s = strtok(NULL, ","))
        if (!strcmp(s, want)) {
            char path[64];
            snprintf(path, sizeof(path), "shot_%04u.ppm", frames);
            dump_display(path);
            port_log("frame %u: wrote %s (%dx%d at %d,%d)\n", frames, path, gpu_display.w, gpu_display.h,
                     gpu_display.x, gpu_display.y);
        }
}

static u8 axis(Sint16 v) {
    return (u8)((v + 32768) >> 8);
}

static void poll_input(void) {
    SDL_Event e;
    const bool *keys;
    u16 down = 0;
    u8 lx = 0x80, ly = 0x80, rx = 0x80, ry = 0x80;
    size_t i;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT)
            exit(0);
        if (e.type == SDL_EVENT_GAMEPAD_ADDED && !gamepad)
            gamepad = SDL_OpenGamepad(e.gdevice.which);
        if (e.type == SDL_EVENT_GAMEPAD_REMOVED && gamepad && e.gdevice.which == SDL_GetGamepadID(gamepad)) {
            SDL_CloseGamepad(gamepad);
            gamepad = NULL;
        }
    }
    keys = SDL_GetKeyboardState(NULL);
    for (i = 0; i < sizeof(keymap) / sizeof(keymap[0]); i++)
        if (keys[keymap[i].key])
            down |= keymap[i].bit;
    if (keys[SDL_SCANCODE_W])
        ly = 0x00;
    if (keys[SDL_SCANCODE_S])
        ly = 0xFF;
    if (keys[SDL_SCANCODE_A])
        lx = 0x00;
    if (keys[SDL_SCANCODE_D])
        lx = 0xFF;
    if (keys[SDL_SCANCODE_I])
        ry = 0x00;
    if (keys[SDL_SCANCODE_K])
        ry = 0xFF;
    if (keys[SDL_SCANCODE_J])
        rx = 0x00;
    if (keys[SDL_SCANCODE_L])
        rx = 0xFF;
    if (gamepad) {
        for (i = 0; i < sizeof(padmap) / sizeof(padmap[0]); i++)
            if (SDL_GetGamepadButton(gamepad, padmap[i].b))
                down |= padmap[i].bit;
        if (SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) > 16000)
            down |= P_L2;
        if (SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) > 16000)
            down |= P_R2;
        lx = axis(SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX));
        ly = axis(SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY));
        rx = axis(SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTX));
        ry = axis(SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTY));
    }
    port_pad_buttons[0] = (u16)~down;
    port_pad_sticks[0][0] = rx;
    port_pad_sticks[0][1] = ry;
    port_pad_sticks[0][2] = lx;
    port_pad_sticks[0][3] = ly;
}

static void present(void) {
    s32 x, y, w = gpu_display.w, h = gpu_display.h;
    SDL_FRect src;
    if (w <= 0 || h <= 0 || w > 640 || h > 512)
        return;
    for (y = 0; y < h; y++) {
        const u16 *row = &gpu_vram[((gpu_display.y + y) & 511) * 1024];
        u32 *out = &pixels[y * 640];
        if (gpu_display.rgb24) {
            const u8 *b = (const u8 *)row + gpu_display.x * 2;
            for (x = 0; x < w; x++)
                out[x] = b[x * 3] | (b[x * 3 + 1] << 8) | (b[x * 3 + 2] << 16);
        } else {
            for (x = 0; x < w; x++) {
                u16 c = row[(gpu_display.x + x) & 1023];
                out[x] = ((c & 31) << 3) | (((c >> 5) & 31) << 11) | (((c >> 10) & 31) << 19);
            }
        }
    }
    SDL_UpdateTexture(texture, NULL, pixels, 640 * 4);
    src.x = 0;
    src.y = 0;
    src.w = (float)w;
    src.h = (float)h;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if (!gpu_display.blank)
        SDL_RenderTexture(renderer, texture, &src, NULL);
    SDL_RenderPresent(renderer);
}

extern u32 gpu_stats[8];
extern int gpu_poly_log;

void port_frame_end(void) {
    RT_ENTER();
    frames++;
    if (getenv("TWINE_GPUSTATS") && frames % 50 == 0)
        port_log("frame %u: polys %u (skipped %u) lines %u rects %u env %u other %u pixels %u transparent %u\n",
                 frames, gpu_stats[0], gpu_stats[1], gpu_stats[2], gpu_stats[3], gpu_stats[5], gpu_stats[6],
                 gpu_stats[7], gpu_stats[4]);
    memset(gpu_stats, 0, sizeof(gpu_stats));
    if (getenv("TWINE_POLYLOG") && frames == (u32)atoi(getenv("TWINE_POLYLOG")))
        gpu_poly_log = 40;
    maybe_shot();
    if (!headless) {
        poll_input();
        present();
    }
    RT_LEAVE();
}
