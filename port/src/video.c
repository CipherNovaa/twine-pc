/* Window, display and input (SDL3). Each game frame (VSync) the displayed VRAM rectangle is shown scaled
 * to the window; keyboard, mouse and gamepad fill the pad state libpad.c hands the game every vblank.
 * In a level the mouse is captured: it looks around (port_mouse_look, taken by the twin-stick mod,
 * src/mods/twinstick.c), and its buttons and some keys do the actions of the game's button map, so they follow
 * the options menu. TWINE_MOUSE_SENS (default 1) scales the look, TWINE_MOUSE_INVERT=1 inverts it vertically.
 * TWINE_HEADLESS=1 runs without a window; TWINE_SHOT=<frame>[,<frame>...] dumps frames as PPM;
 * TWINE_TEST_LOOK=<yaw>,<pitch> stands in for the mouse there (angle units each game frame), and
 * TWINE_TEST_PAD=<frame>:<hex>[,...] for the pad (from each frame on, the pad bits below are held). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "port.h"
#include "gpu.h"
#include "intro.h"
#include "module.h"

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

/* menus and the pad's own buttons */
static const struct {
    SDL_Scancode key;
    u16 bit;
} keymap[] = {
    {SDL_SCANCODE_UP, P_UP}, {SDL_SCANCODE_DOWN, P_DOWN}, {SDL_SCANCODE_LEFT, P_LEFT},
    {SDL_SCANCODE_RIGHT, P_RIGHT}, {SDL_SCANCODE_RETURN, P_START}, {SDL_SCANCODE_ESCAPE, P_START},
    {SDL_SCANCODE_TAB, P_SELECT}, {SDL_SCANCODE_SPACE, P_CROSS}, {SDL_SCANCODE_BACKSPACE, P_TRI},
};

/* the game's actions (g_Args->buttonMap) */
enum { A_FIRE, A_PREV_WEAPON, A_NEXT_WEAPON, A_RELOAD, A_CROUCH, A_AIM, A_STRAFE_LEFT, A_STRAFE_RIGHT };

static const struct {
    SDL_Scancode key;
    int action;
} actionkeys[] = {
    {SDL_SCANCODE_F, A_FIRE}, {SDL_SCANCODE_Q, A_PREV_WEAPON}, {SDL_SCANCODE_E, A_NEXT_WEAPON},
    {SDL_SCANCODE_R, A_RELOAD}, {SDL_SCANCODE_C, A_CROUCH}, {SDL_SCANCODE_LCTRL, A_CROUCH},
    {SDL_SCANCODE_A, A_STRAFE_LEFT}, {SDL_SCANCODE_D, A_STRAFE_RIGHT},
};

static int mouse_captured, wheel, wheel_pulse;
static float look_yaw, look_pitch; /* angle units not yet taken */

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

/* The pad bits of one of the game's actions: g_Args->buttonMap holds them in the game's bit order, which is
 * libpad's with the bytes swapped. g_Args is the boot stub's (0x8001F158). */
static u16 action_bits(int action) {
    u32 args = *(u32 *)0x8001F158;
    u16 b;
    if (args < 0x80000000u || args >= 0x80200000u)
        return 0;
    b = *(u16 *)(args + 0x148 + action * 2);
    return (u16)((b << 8) | (b >> 8));
}

/* the mouse's movement since the last call, in angle units (4096 = 360 degrees; pitch positive is up) */
void port_mouse_look(s32 *yaw, s32 *pitch) {
    const char *test = getenv("TWINE_TEST_LOOK");
    if (headless && test) {
        *yaw = 0;
        *pitch = 0;
        sscanf(test, "%d,%d", yaw, pitch);
        return;
    }
    *yaw = (s32)look_yaw;
    *pitch = (s32)look_pitch;
    look_yaw -= *yaw;
    look_pitch -= *pitch;
}

static void mouse_event(const SDL_Event *e) {
    static float scale;
    if (!mouse_captured)
        return;
    if (!scale) {
        const char *sens = getenv("TWINE_MOUSE_SENS");
        scale = 0.5f * (sens && atof(sens) > 0 ? (float)atof(sens) : 1.0f);
    }
    if (e->type == SDL_EVENT_MOUSE_MOTION) {
        look_yaw += e->motion.xrel * scale;
        look_pitch -= e->motion.yrel * scale * (getenv("TWINE_MOUSE_INVERT") ? -1 : 1);
    }
    if (e->type == SDL_EVENT_MOUSE_WHEEL && e->wheel.y != 0)
        wheel += e->wheel.y > 0 ? -1 : 1; /* up: the previous weapon */
}

static void window_event(const SDL_Event *e) {
    if (e->type == SDL_EVENT_QUIT)
        exit(0);
    mouse_event(e);
    if (e->type == SDL_EVENT_GAMEPAD_ADDED && !gamepad)
        gamepad = SDL_OpenGamepad(e->gdevice.which);
    if (e->type == SDL_EVENT_GAMEPAD_REMOVED && gamepad && e->gdevice.which == SDL_GetGamepadID(gamepad)) {
        SDL_CloseGamepad(gamepad);
        gamepad = NULL;
    }
}

static void poll_input(void) {
    SDL_Event e;
    const bool *keys;
    u16 down = 0;
    u8 lx = 0x80, ly = 0x80, rx = 0x80, ry = 0x80;
    SDL_MouseButtonFlags mouse;
    int capture;
    size_t i;
    /* the mouse looks around in the levels, while the window has the focus */
    capture = port_module && port_module->level && (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS);
    if (capture != mouse_captured) {
        SDL_SetWindowRelativeMouseMode(window, capture);
        mouse_captured = capture;
        look_yaw = look_pitch = 0;
        wheel = 0;
    }
    while (SDL_PollEvent(&e))
        window_event(&e);
    keys = SDL_GetKeyboardState(NULL);
    for (i = 0; i < sizeof(keymap) / sizeof(keymap[0]); i++)
        if (keys[keymap[i].key])
            down |= keymap[i].bit;
    for (i = 0; i < sizeof(actionkeys) / sizeof(actionkeys[0]); i++)
        if (keys[actionkeys[i].key])
            down |= action_bits(actionkeys[i].action);
    if (mouse_captured) {
        mouse = SDL_GetMouseState(NULL, NULL);
        if (mouse & SDL_BUTTON_LMASK)
            down |= action_bits(A_FIRE);
        if (mouse & SDL_BUTTON_RMASK)
            down |= action_bits(A_AIM);
        /* each wheel step presses the weapon button for one game frame */
        if (wheel_pulse)
            wheel_pulse = 0;
        else if (wheel) {
            down |= action_bits(wheel < 0 ? A_PREV_WEAPON : A_NEXT_WEAPON);
            wheel += wheel < 0 ? 1 : -1;
            wheel_pulse = 1;
        }
    }
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

/* headless: the pad bits TWINE_TEST_PAD holds at this frame */
static void test_pad(void) {
    const char *s = getenv("TWINE_TEST_PAD");
    u32 bits = 0;
    while (s && *s) {
        char *end;
        u32 from = (u32)strtoul(s, &end, 10), b = 0;
        if (*end == ':')
            b = (u32)strtoul(end + 1, &end, 16);
        if (from <= frames)
            bits = b;
        s = *end ? end + 1 : end;
    }
    port_pad_buttons[0] = (u16)~bits;
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

/* ---- the intro (intro.c), before the game: its frames don't count as the game's ---- */

int port_video_headless(void) {
    return headless;
}

void *port_video_renderer(void) {
    return renderer;
}

void port_video_show_gpu(void) {
    if (headless)
        return;
    RT_ENTER();
    present();
    RT_LEAVE();
}

void port_video_dump_gpu(const char *path) {
    RT_ENTER();
    dump_display(path);
    RT_LEAVE();
}

/* Pump window events; 1 when Start, Cross, Enter or Esc has been pressed. */
int port_video_poll_skip(void) {
    SDL_Event e;
    int skip = 0;
    if (headless)
        return 0;
    RT_ENTER();
    while (SDL_PollEvent(&e)) {
        window_event(&e);
        if (e.type == SDL_EVENT_KEY_DOWN && !e.key.repeat &&
            (e.key.scancode == SDL_SCANCODE_RETURN || e.key.scancode == SDL_SCANCODE_ESCAPE ||
             e.key.scancode == SDL_SCANCODE_SPACE || e.key.scancode == SDL_SCANCODE_Z))
            skip = 1;
        if (e.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN &&
            (e.gbutton.button == SDL_GAMEPAD_BUTTON_START || e.gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH))
            skip = 1;
    }
    RT_LEAVE();
    return skip;
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
    } else if (getenv("TWINE_TEST_PAD"))
        test_pad();
    RT_LEAVE();
}
