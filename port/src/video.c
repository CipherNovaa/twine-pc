/* Window, display and input (SDL3). Each game frame (VSync) the displayed VRAM rectangle is shown scaled
 * to the window; keyboard, mouse and gamepad fill the pad state libpad.c hands the game every vblank.
 * In a level the mouse is captured: it looks around (port_mouse_look, taken by the twin-stick mod,
 * src/mods/twinstick.c), and its buttons and some keys do the actions of the game's button map, so they follow
 * the options menu. TWINE_MOUSE_SENS (default 1) scales the look, TWINE_MOUSE_INVERT=1 inverts it vertically.
 * Frames between the game's ticks come from interp.c; the window's title shows the frame rate, and
 * TWINE_VSYNC=0 shows frames as fast as they come instead of at the display's rate. F11 or Alt+Enter switches
 * between full screen and a window; TWINE_FULLSCREEN=1 starts in full screen, and TWINE_DISPLAY=<number, or part
 * of its name> on that display (the log lists them). The picture keeps the TV's 4:3 shape; TWINE_ASPECT=fill
 * stretches it over the whole window or screen (for a 4:3 CRT driven at 1280x1024). TWINE_FILTER=sharp
 * (default), nearest or linear: how it's scaled (draw_picture).
 * TWINE_RENDERER=hw draws with the PC's GPU (gpu_hw.c; Vulkan, through SDL's GPU renderer, which then shows
 * everything here too) at TWINE_SCALE times VRAM's resolution, and shows that picture instead of VRAM's (but for
 * 24-bit movie frames, which come from VRAM as before); a picture larger than the window is averaged down to it
 * (supersampling) before the filter scales it the rest of the way. The default is TWINE_RENDERER=sw. With it,
 * TWINE_PRESENT=vsync|mailbox|immediate picks how frames reach the display (port_video_present_mode).
 * TWINE_HEADLESS=1 runs without a window; TWINE_SHOT=<frame>[,<frame>...] dumps frames as PPM (with the hardware
 * renderer its picture, at its full resolution, and also screen_NNNN.ppm: what a 1280x960 window would show,
 * drawn by SDL's GPU renderer into a texture the way it draws the window);
 * TWINE_TEST_LOOK=<yaw>,<pitch> stands in for the mouse there (angle units each game frame), and
 * TWINE_TEST_PAD=<frame>:<hex>[,...] for the pad (from each frame on, the pad bits below are held) and
 * TWINE_TEST_STICK=<rx>,<ry>,<lx>,<ly> (hex) for its sticks. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "port.h"
#include "gpu.h"
#include "gpu_hw.h"
#include "intro.h"
#include "module.h"

extern u16 port_pad_buttons[2];
extern int port_smooth_shown;
extern u8 port_pad_sticks[2][4];
void port_pad_motors(u8 *small, u8 *large);

static u32 frames;
static int headless;
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static SDL_Texture *screen;      /* headless with the hardware renderer: the window's stand-in */
static const char *screen_shot;  /* the PPM the next picture of it goes to */
#define MAX_PADS 8
static SDL_Gamepad *gamepad;          /* the one read */
static SDL_Gamepad *pads[MAX_PADS];   /* all connected */
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
    {SDL_SCANCODE_RIGHT, P_RIGHT},
    {SDL_SCANCODE_TAB, P_SELECT}, {SDL_SCANCODE_SPACE, P_CROSS}, {SDL_SCANCODE_BACKSPACE, P_TRI},
};

/* the game's actions (g_Args->buttonMap); reload is also the button that uses things (doors, people) */
enum { A_FIRE, A_PREV_WEAPON, A_NEXT_WEAPON, A_RELOAD, A_CROUCH, A_AIM, A_STRAFE_LEFT, A_STRAFE_RIGHT };

static const struct {
    SDL_Scancode key;
    int action;
} actionkeys[] = {
    {SDL_SCANCODE_F, A_RELOAD}, {SDL_SCANCODE_Q, A_PREV_WEAPON}, {SDL_SCANCODE_E, A_NEXT_WEAPON},
    {SDL_SCANCODE_R, A_RELOAD}, {SDL_SCANCODE_C, A_CROUCH}, {SDL_SCANCODE_LCTRL, A_CROUCH},
    {SDL_SCANCODE_A, A_STRAFE_LEFT}, {SDL_SCANCODE_D, A_STRAFE_RIGHT},
};

static int mouse_captured, wheel, wheel_pulse;
static int help_shown; /* F1 */
static Uint64 mouse_moved; /* when the mouse last moved (SDL ticks) */
static Uint64 mouse_taken; /* when the game last took its movement (port_mouse_look) */
static u16 wheel_bits; /* the weapon button a wheel step holds until the game's next tick */
static float look_yaw, look_pitch; /* angle units not yet taken */
static float taken_yaw, taken_pitch; /* (TWINE_TEST_LOOK) taken so far */

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
    {SDL_GAMEPAD_BUTTON_RIGHT_STICK, P_R3}, {SDL_GAMEPAD_BUTTON_TOUCHPAD, P_SELECT},
};

/* The gamepad while Bond has the controls, in the modern layout (TWINE_CONTROLS, the default; "original"
 * keeps the PS1 pad's buttons and the game's own settings for them): the buttons are actions, whichever PS1
 * buttons the game's settings give them. The triggers fire and aim. Cross and the d-pad stay the pad's, for
 * the prompts a mission puts up, and Start, Select and the touchpad too. */
static const struct {
    SDL_GamepadButton b;
    int action;
} modern_actions[] = {
    {SDL_GAMEPAD_BUTTON_WEST, A_RELOAD},           {SDL_GAMEPAD_BUTTON_EAST, A_CROUCH},
    {SDL_GAMEPAD_BUTTON_RIGHT_STICK, A_CROUCH},    {SDL_GAMEPAD_BUTTON_NORTH, A_NEXT_WEAPON},
    {SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, A_NEXT_WEAPON}, {SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, A_PREV_WEAPON},
};
static const struct {
    SDL_GamepadButton b;
    u16 bit;
} modern_pad[] = {
    {SDL_GAMEPAD_BUTTON_DPAD_UP, P_UP},     {SDL_GAMEPAD_BUTTON_DPAD_DOWN, P_DOWN},
    {SDL_GAMEPAD_BUTTON_DPAD_LEFT, P_LEFT}, {SDL_GAMEPAD_BUTTON_DPAD_RIGHT, P_RIGHT},
    {SDL_GAMEPAD_BUTTON_START, P_START},    {SDL_GAMEPAD_BUTTON_BACK, P_SELECT},
    {SDL_GAMEPAD_BUTTON_TOUCHPAD, P_SELECT}, {SDL_GAMEPAD_BUTTON_SOUTH, P_CROSS},
};

/* Who has the controls in a mission: the twin-stick mod says every game frame (1 Bond, 0 a menu, 2 a cut
 * scene); when it hasn't for a while (the game's frames stopped), a menu. */
static int controls_state;
static u32 controls_at;

void port_controls_state(int state) {
    controls_state = state;
    controls_at = port_vblank_count();
}

static int controls_now(void) {
    return port_vblank_count() - controls_at < 10 ? controls_state : 0;
}

static int in_gameplay(void) {
    return port_module && port_module->level && controls_now() == 1;
}

/* a menu inside a mission (the phone's pause menu, ...) */
static int in_mission_menu(void) {
    return port_module && port_module->level && controls_now() == 0;
}

/* the display TWINE_DISPLAY names (its number from 1, or part of its name), else 0 */
static SDL_DisplayID chosen_display(void) {
    const char *want = getenv("TWINE_DISPLAY");
    SDL_DisplayID *ids, id = 0;
    int i, n = 0;
    ids = SDL_GetDisplays(&n);
    for (i = 0; ids && i < n; i++) {
        const SDL_DisplayMode *m = SDL_GetDesktopDisplayMode(ids[i]);
        const char *name = SDL_GetDisplayName(ids[i]);
        port_log("video: display %d: %s, %dx%d at %.2f Hz\n", i + 1, name ? name : "?", m ? m->w : 0, m ? m->h : 0,
                 m ? m->refresh_rate : 0.0f);
        if (want && !id && (atoi(want) == i + 1 || (name && SDL_strcasestr(name, want))))
            id = ids[i];
    }
    SDL_free(ids);
    if (want && !id)
        port_log("video: no display matches TWINE_DISPLAY=%s\n", want);
    return id;
}

/* TWINE_PRESENT (hardware renderer, SDL's GPU renderer): vsync (FIFO: presenting waits while the frames before
 * still wait for their refresh), mailbox (the newest frame replaces one still waiting; presenting doesn't wait)
 * or immediate (shown at once, tearing). Unset, TWINE_VSYNC decides as for the other renderers. Again after
 * the title card, which sets vsync for itself. */
void port_video_present_mode(void) {
    const char *p = getenv("TWINE_PRESENT");
    SDL_GPUDevice *dev = gpu_hw_device();
    SDL_GPUPresentMode mode = SDL_GPU_PRESENTMODE_VSYNC;
    if (!gpu_hw_on || !window || !p || !dev)
        return;
    if (!SDL_strcasecmp(p, "mailbox"))
        mode = SDL_GPU_PRESENTMODE_MAILBOX;
    else if (!SDL_strcasecmp(p, "immediate"))
        mode = SDL_GPU_PRESENTMODE_IMMEDIATE;
    if (!SDL_WindowSupportsGPUPresentMode(dev, window, mode))
        port_log("video: present mode %s not supported here\n", p);
    else if (!SDL_SetGPUSwapchainParameters(dev, window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, mode))
        port_log("video: present mode %s: %s\n", p, SDL_GetError());
    else
        port_log("video: present mode %s\n", p);
}

/* full screen on the window's display, or back to the window; the pointer hides in full screen */
static void set_fullscreen(int on) {
    SDL_SetWindowFullscreen(window, on);
    if (on)
        SDL_HideCursor();
    else
        SDL_ShowCursor();
}

/* TWINE_RENDERER=hw without a window: the GPU device on SDL's offscreen video driver (and Ctrl+C still ends
 * the program: nothing here reads SDL's quit event) */
static void headless_hw(void) {
    RT_ENTER();
    if (!getenv("SDL_VIDEODRIVER"))
        SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "offscreen");
    SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
    if (!SDL_Init(SDL_INIT_VIDEO) || !gpu_hw_init())
        port_log("video: no hardware renderer (%s); drawing in software\n", SDL_GetError());
    else if (!(renderer = SDL_CreateGPURenderer(gpu_hw_device(), NULL)) ||
             !(screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XBGR8888, SDL_TEXTUREACCESS_TARGET, 1280, 960)) ||
             !(texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XBGR8888, SDL_TEXTUREACCESS_STREAMING, 640, 512)))
        port_log("video: no GPU renderer to stand in for the window: %s\n", SDL_GetError());
    else
        SDL_SetRenderTarget(renderer, screen);
    RT_LEAVE();
}

void port_video_init(void) {
    SDL_PropertiesID props;
    SDL_DisplayID display;
    int full;
    headless = getenv("TWINE_HEADLESS") != NULL;
    if (headless) {
        if (gpu_hw_wanted())
            headless_hw();
        return;
    }
    RT_ENTER();
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
        port_fatal("SDL_Init: %s\n", SDL_GetError());
    display = chosen_display();
    full = getenv("TWINE_FULLSCREEN") && atoi(getenv("TWINE_FULLSCREEN"));
    props = SDL_CreateProperties();
    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, "TWINE");
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, 1280);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, 960);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, full);
    if (display) {
        SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, SDL_WINDOWPOS_CENTERED_DISPLAY(display));
        SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, SDL_WINDOWPOS_CENTERED_DISPLAY(display));
    }
    window = SDL_CreateWindowWithProperties(props);
    SDL_DestroyProperties(props);
    if (!window)
        port_fatal("SDL window: %s\n", SDL_GetError());
    /* the hardware renderer's device shows the window too (SDL's GPU renderer on it) */
    if (gpu_hw_wanted() && gpu_hw_init() && !(renderer = SDL_CreateGPURenderer(gpu_hw_device(), window))) {
        port_log("video: no GPU renderer for the hardware renderer (%s); drawing in software\n", SDL_GetError());
        gpu_hw_on = 0;
    }
    if (!renderer && !(renderer = SDL_CreateRenderer(window, NULL)))
        port_fatal("SDL renderer: %s\n", SDL_GetError());
    if (full)
        SDL_HideCursor();
    {
        int want = getenv("TWINE_VSYNC") && !atoi(getenv("TWINE_VSYNC")) ? 0 : 1, vsync = -2;
        int ok = SDL_SetRenderVSync(renderer, want);
        SDL_GetRenderVSync(renderer, &vsync);
        port_log("video: SDL %s renderer, %s video driver, vsync %d (asked %d%s)\n", SDL_GetRendererName(renderer),
                 SDL_GetCurrentVideoDriver(), vsync, want, ok ? "" : ", refused");
        port_video_present_mode();
    }
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XBGR8888, SDL_TEXTUREACCESS_STREAMING, 640, 512);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    RT_LEAVE();
}

/* One line of the VRAM rectangle at (rx, ry) as 0x00BBGGRR pixels: 15-bit VRAM pixels, or in 24-bit mode
 * (movies) 3 bytes a pixel from the rectangle's left edge on (w counts real pixels in both). */
static void vram_line(s32 rx, s32 ry, int rgb24, s32 y, s32 w, u32 *out) {
    const u16 *row = &gpu_vram[((ry + y) & 511) * 1024];
    s32 x;
    if (rgb24) {
        for (x = 0; x < w; x++) {
            u32 b = (rx & 1023) * 2 + x * 3, v = 0, k;
            for (k = 0; k < 3; k++, b++) /* bytes of a line wrap at its 2048-byte end */
                v |= (u32)((row[(b >> 1) & 1023] >> ((b & 1) * 8)) & 0xFF) << (k * 8);
            out[x] = v;
        }
    } else {
        for (x = 0; x < w; x++) {
            u16 c = row[(rx + x) & 1023];
            out[x] = ((c & 31) << 3) | (((c >> 5) & 31) << 11) | (((c >> 10) & 31) << 19);
        }
    }
}

static void dump_rect(const char *path, s32 rx, s32 ry, int rgb24) {
    FILE *f;
    s32 x, y, w = gpu_display.w, h = gpu_display.h;
    gpu_sync(0, 0, 1024, 512);
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
        vram_line(rx, ry, rgb24, y, w, pixels);
        for (x = 0; x < w; x++) {
            u8 rgb[3] = {(u8)pixels[x], (u8)(pixels[x] >> 8), (u8)(pixels[x] >> 16)};
            fwrite(rgb, 1, 3, f);
        }
    }
    fclose(f);
}

static void dump_display(const char *path) {
    dump_rect(path, gpu_display.x, gpu_display.y, gpu_display.rgb24);
}

/* this frame is one TWINE_SHOT names */
static int shot_wanted(void) {
    const char *shots = getenv("TWINE_SHOT");
    char want[16], list[256], *s;
    if (!shots)
        return 0;
    snprintf(want, sizeof(want), "%u", frames);
    strncpy(list, shots, sizeof(list) - 1);
    list[sizeof(list) - 1] = 0;
    for (s = strtok(list, ","); s; s = strtok(NULL, ","))
        if (!strcmp(s, want))
            return 1;
    return 0;
}

/* The frame's shot; with the hardware renderer, also its comparison and statistics (gpu_hw_frame). */
static void maybe_shot(void) {
    char path[64];
    int shot = shot_wanted(), hw = gpu_hw_on && !gpu_display.rgb24;
    snprintf(path, sizeof(path), "shot_%04u.ppm", frames);
    if (gpu_hw_on)
        gpu_hw_frame(frames, gpu_display.x, gpu_display.y, hw ? gpu_display.w : 0, gpu_display.h,
                     hw && shot ? path : NULL);
    if (shot && screen && !port_smooth_shown) { /* (else the drawings have been shown, with their own) */
        static char sp[64];
        snprintf(sp, sizeof(sp), "screen_%04u.ppm", frames);
        screen_shot = sp;
    }
    if (!shot)
        return;
    if (!hw)
        dump_display(path);
    port_log("frame %u: wrote %s (%dx%d at %d,%d)\n", frames, path, gpu_display.w, gpu_display.h, gpu_display.x,
             gpu_display.y);
}

static u8 axis(Sint16 v) {
    return (u8)((v + 32768) >> 8);
}

/* A gamepad stick as the pad's two axes, with a dead zone around the middle (TWINE_DEADZONE, in percent of the
 * way out, 12 by default; the rest of the travel is spread over the whole range): a stick at rest is never
 * quite centred, and the game turns and looks by the smallest offsets. 0 while the stick is inside it, so
 * the keyboard's WASD and IJKL stand. */
static int stick(SDL_GamepadAxis ax, SDL_GamepadAxis ay, u8 *x, u8 *y) {
    static float dead = -1;
    float fx = SDL_GetGamepadAxis(gamepad, ax) / 32767.0f, fy = SDL_GetGamepadAxis(gamepad, ay) / 32767.0f, m, s;
    if (dead < 0) {
        const char *d = getenv("TWINE_DEADZONE");
        dead = d ? (float)atof(d) / 100 : 0.12f;
        dead = dead < 0 ? 0 : dead > 0.9f ? 0.9f : dead;
    }
    m = sqrtf(fx * fx + fy * fy);
    if (m <= dead)
        return 0;
    s = ((m > 1 ? 1 : m) - dead) / (1 - dead) / m;
    *x = axis((Sint16)SDL_clamp(fx * s * 32767, -32768, 32767));
    *y = axis((Sint16)SDL_clamp(fy * s * 32767, -32768, 32767));
    return 1;
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

/* TWINE_TEST_LOOK=<yaw>,<pitch>: a mouse moving that much every 50 ms (a game tick), steadily (in a window
 * too, for measuring how smoothly the view turns: TWINE_YAWLOG) */
static int test_look(float *yaw, float *pitch) {
    static Uint64 start;
    const char *test = getenv("TWINE_TEST_LOOK");
    float ticks;
    int y = 0, p = 0;
    if (!test)
        return 0;
    if (!start)
        start = SDL_GetTicksNS();
    sscanf(test, "%d,%d", &y, &p);
    ticks = (float)(SDL_GetTicksNS() - start) / 50e6f;
    *yaw = y * ticks;
    *pitch = p * ticks;
    return 1;
}

/* the mouse's movement not taken yet (interp.c turns the camera of frames between ticks by it) */
void port_mouse_pending(s32 *yaw, s32 *pitch) {
    float ty, tp;
    if (test_look(&ty, &tp)) {
        look_yaw = ty - taken_yaw;
        look_pitch = tp - taken_pitch;
    }
    *yaw = (s32)look_yaw;
    *pitch = (s32)look_pitch;
}

/* The game takes the mouse's movement every tick while Bond can look around (the twin-stick mod). When it
 * hasn't for a while (a cut scene, the pause menu), movement is dropped rather than saved up for later. */
static int mouse_in_use(void) {
    return mouse_taken && SDL_GetTicks() - mouse_taken < 200;
}

/* the mouse has moved in the last second (looking around with it; interp.c then turns the camera of every
 * frame by its latest movement) */
int port_mouse_recent(void) {
    if (getenv("TWINE_TEST_LOOK"))
        return 1;
    if (headless)
        return 0;
    return mouse_captured && mouse_in_use() && mouse_moved && SDL_GetTicks() - mouse_moved < 1000;
}

/* the mouse's movement since the last call, in angle units (4096 = 360 degrees; pitch positive is up) */
void port_mouse_look(s32 *yaw, s32 *pitch) {
    float ty, tp;
    if (test_look(&ty, &tp)) {
        look_yaw = ty - taken_yaw;
        look_pitch = tp - taken_pitch;
    }
    *yaw = (s32)look_yaw;
    *pitch = (s32)look_pitch;
    mouse_taken = SDL_GetTicks();
    look_yaw -= *yaw;
    look_pitch -= *pitch;
    taken_yaw += *yaw;
    taken_pitch += *pitch;
}

static void mouse_event(const SDL_Event *e) {
    static float scale;
    if (!mouse_captured)
        return;
    if (!scale) {
        const char *sens = getenv("TWINE_MOUSE_SENS");
        scale = 0.5f * (sens && atof(sens) > 0 ? (float)atof(sens) : 1.0f);
    }
    if (e->type == SDL_EVENT_MOUSE_MOTION && mouse_in_use()) {
        static int log = -1;
        /* TWINE_MOUSELOG=1: each movement as it comes (event time, x, y), next to TWINE_YAWLOG's frames */
        if (log < 0)
            log = getenv("TWINE_MOUSELOG") && atoi(getenv("TWINE_MOUSELOG"));
        if (log)
            port_log("mouse %llu %.2f %.2f\n", (unsigned long long)e->motion.timestamp, e->motion.xrel, e->motion.yrel);
        mouse_moved = SDL_GetTicks();
        look_yaw += e->motion.xrel * scale;
        look_pitch -= e->motion.yrel * scale * (getenv("TWINE_MOUSE_INVERT") && atoi(getenv("TWINE_MOUSE_INVERT")) ? -1 : 1);
    }
    if (e->type == SDL_EVENT_MOUSE_WHEEL && e->wheel.y != 0)
        wheel += e->wheel.y > 0 ? -1 : 1; /* up: the previous weapon */
}

/* Every gamepad connected is opened, and the one last used is read: Steam, for one, can add a virtual
 * gamepad of its own beside the real one, which stays silent while the game isn't run through Steam. */
static void pad_added(SDL_JoystickID id) {
    SDL_Gamepad *g;
    int i;
    if (SDL_GetGamepadFromID(id))
        return;
    for (i = 0; i < MAX_PADS && pads[i]; i++)
        ;
    if (i == MAX_PADS || !(g = SDL_OpenGamepad(id)))
        return;
    pads[i] = g;
    port_log("video: gamepad %s\n", SDL_GetGamepadName(g));
    if (!gamepad)
        gamepad = g;
}

static void pad_removed(SDL_JoystickID id) {
    int i;
    for (i = 0; i < MAX_PADS; i++) {
        if (pads[i] && SDL_GetGamepadID(pads[i]) == id) {
            if (gamepad == pads[i])
                gamepad = NULL;
            SDL_CloseGamepad(pads[i]);
            pads[i] = NULL;
        }
    }
    for (i = 0; i < MAX_PADS && !gamepad; i++)
        gamepad = pads[i];
}

static void pad_used(SDL_JoystickID id) {
    SDL_Gamepad *g = SDL_GetGamepadFromID(id);
    if (g && g != gamepad) {
        gamepad = g;
        port_log("video: reading gamepad %s\n", SDL_GetGamepadName(g));
    }
}

static void window_event(const SDL_Event *e) {
    if (e->type == SDL_EVENT_QUIT)
        exit(0);
    /* F11 or Alt+Enter: full screen or a window */
    if (e->type == SDL_EVENT_KEY_DOWN && !e->key.repeat && e->key.scancode == SDL_SCANCODE_F1)
        help_shown = !help_shown;
    if (e->type == SDL_EVENT_KEY_DOWN && !e->key.repeat &&
        (e->key.scancode == SDL_SCANCODE_F11 || (e->key.scancode == SDL_SCANCODE_RETURN && (e->key.mod & SDL_KMOD_ALT))))
        set_fullscreen(!(SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN));
    mouse_event(e);
    if (e->type == SDL_EVENT_GAMEPAD_ADDED)
        pad_added(e->gdevice.which);
    if (e->type == SDL_EVENT_GAMEPAD_REMOVED)
        pad_removed(e->gdevice.which);
    if (e->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN)
        pad_used(e->gbutton.which);
    if (e->type == SDL_EVENT_GAMEPAD_AXIS_MOTION && (e->gaxis.value > 16000 || e->gaxis.value < -16000))
        pad_used(e->gaxis.which);
}

/* The game's motor values on the gamepad: the small motor (on or off) as its high-frequency rumble, the large
 * one (0-255) as the low-frequency one. Renewed every game frame while on, so they stop when the game stops
 * asking. TWINE_RUMBLE=0 turns it off. */
static void rumble(void) {
    static u8 was_small, was_large;
    static Uint64 renewed;
    static int off = -1;
    u8 small, large;
    Uint64 now = SDL_GetTicks();
    if (off < 0)
        off = getenv("TWINE_RUMBLE") && !atoi(getenv("TWINE_RUMBLE"));
    port_pad_motors(&small, &large);
    if (off || (small == was_small && large == was_large && (!(small | large) || now - renewed < 100)))
        return;
    SDL_RumbleGamepad(gamepad, (Uint16)(large * 257), small ? 0xC000 : 0, small | large ? 250 : 0);
    if (getenv("TWINE_RUMBLE_LOG"))
        port_log("rumble: small %u large %u\n", small, large);
    was_small = small;
    was_large = large;
    renewed = now;
}

/* `tick`: at the game's frame (VSync), not a frame drawn between ticks */
static void poll_input(int tick) {
    SDL_Event e;
    const bool *keys;
    u16 down = 0;
    u8 lx = 0x80, ly = 0x80, rx = 0x80, ry = 0x80;
    SDL_MouseButtonFlags mouse;
    int capture;
    size_t i;
    /* the mouse looks around in the levels, while the window has the focus (TWINE_MOUSE=0: never, for playing
     * with a gamepad only) */
    capture = port_module && port_module->level && (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS) &&
              !(getenv("TWINE_MOUSE") && !atoi(getenv("TWINE_MOUSE")));
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
        if (keys[keymap[i].key] && !(keymap[i].key == SDL_SCANCODE_RETURN && (SDL_GetModState() & SDL_KMOD_ALT)) &&
            !(keymap[i].key == SDL_SCANCODE_SPACE && in_gameplay()))
            down |= keymap[i].bit;
    /* Space chooses in the menus and uses things (doors, people) while Bond has the controls */
    if (keys[SDL_SCANCODE_SPACE] && in_gameplay())
        down |= action_bits(A_RELOAD);
    /* Esc goes back in menus (Triangle; from the top of the phone's pause menu that closes it) and pauses or
     * skips everywhere else (Start); Enter chooses in a mission's menus (Cross) and is Start elsewhere. Esc is
     * the key the keyboard layout calls Escape (e.g. Caps Lock's with caps:swapescape, where the key marked Esc
     * is Caps Lock), where the other keys are where they are on the keyboard. */
    if (keys[SDL_GetScancodeFromKey(SDLK_ESCAPE, NULL)])
        down |= (port_module && !strcmp(port_module->name, "shell")) || in_mission_menu() ? P_TRI : P_START;
    if (keys[SDL_SCANCODE_RETURN] && !(SDL_GetModState() & SDL_KMOD_ALT))
        down |= in_mission_menu() ? P_CROSS : P_START;
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
        if (tick) {
            wheel_bits = 0;
            if (wheel_pulse)
                wheel_pulse = 0;
            else if (wheel) {
                wheel_bits = action_bits(wheel < 0 ? A_PREV_WEAPON : A_NEXT_WEAPON);
                wheel += wheel < 0 ? 1 : -1;
                wheel_pulse = 1;
            }
        }
        down |= wheel_bits;
    }
    /* In the missions the pad is in analog mode, where the game walks and turns by the sticks alone: the arrows
     * walk (the left stick) and turn (the right one) there. */
    if (port_module && port_module->level) {
        if (keys[SDL_SCANCODE_UP])
            ly = 0x00;
        if (keys[SDL_SCANCODE_DOWN])
            ly = 0xFF;
        if (keys[SDL_SCANCODE_LEFT])
            rx = 0x00;
        if (keys[SDL_SCANCODE_RIGHT])
            rx = 0xFF;
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
        static int modern = -1;
        int l2 = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) > 16000;
        int r2 = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) > 16000;
        if (modern < 0)
            modern = !(getenv("TWINE_CONTROLS") && !SDL_strcasecmp(getenv("TWINE_CONTROLS"), "original"));
        stick(SDL_GAMEPAD_AXIS_LEFTX, SDL_GAMEPAD_AXIS_LEFTY, &lx, &ly);
        stick(SDL_GAMEPAD_AXIS_RIGHTX, SDL_GAMEPAD_AXIS_RIGHTY, &rx, &ry);
        if (modern && in_gameplay()) {
            for (i = 0; i < sizeof(modern_actions) / sizeof(modern_actions[0]); i++)
                if (SDL_GetGamepadButton(gamepad, modern_actions[i].b))
                    down |= action_bits(modern_actions[i].action);
            for (i = 0; i < sizeof(modern_pad) / sizeof(modern_pad[0]); i++)
                if (SDL_GetGamepadButton(gamepad, modern_pad[i].b))
                    down |= modern_pad[i].bit;
            if (r2)
                down |= action_bits(A_FIRE);
            /* aiming, the game leans with the strafe buttons (as A/D do with the mouse's aim): the stick does */
            if (l2) {
                down |= action_bits(A_AIM);
                if (lx < 0x50)
                    down |= action_bits(A_STRAFE_LEFT);
                else if (lx > 0xB0)
                    down |= action_bits(A_STRAFE_RIGHT);
            }
        } else {
            for (i = 0; i < sizeof(padmap) / sizeof(padmap[0]); i++)
                if (SDL_GetGamepadButton(gamepad, padmap[i].b))
                    down |= padmap[i].bit;
            if (l2)
                down |= P_L2;
            if (r2)
                down |= P_R2;
        }
    }
    port_pad_buttons[0] = (u16)~down;
    if (gamepad && tick)
        rumble();
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
    s = getenv("TWINE_TEST_STICK"); /* <rx>,<ry>,<lx>,<ly> in hex: held throughout */
    if (s) {
        unsigned a[4] = {0x80, 0x80, 0x80, 0x80};
        sscanf(s, "%x,%x,%x,%x", &a[0], &a[1], &a[2], &a[3]);
        for (bits = 0; bits < 4; bits++)
            port_pad_sticks[0][bits] = (u8)a[bits];
    }
}

/* the frame rate in the window's title, once a second; with TWINE_FPSLOG=1 also in the log every ten seconds,
 * with the longest gap between two frames, how many gaps were over 20 ms (a missed refresh at 75 Hz) and how
 * long presenting a frame took (waiting for the display's refresh) */
static Uint64 present_ns; /* time spent in SDL_RenderPresent since the last log line */
int port_shown_yaw; /* the camera's yaw in the frame being shown (smooth.c), for TWINE_YAWLOG */

static void count_frame(void) {
    static Uint64 since, last, log_since, worst;
    static int n, log_n, slow, fpslog = -1;
    Uint64 now = SDL_GetTicksNS();
    if (fpslog < 0)
        fpslog = getenv("TWINE_FPSLOG") && atoi(getenv("TWINE_FPSLOG"));
    if (last) {
        if (now - last > worst)
            worst = now - last;
        if (now - last > 20000000)
            slow++;
    }
    last = now;
    n++;
    log_n++;
    if (now - since >= 1000000000) {
        char title[64];
        snprintf(title, sizeof(title), "TWINE - %d fps", (int)(n * 1000000000ull / (now - since)));
        SDL_SetWindowTitle(window, title);
        since = now;
        n = 0;
    }
    if (fpslog && now - log_since >= 10000000000ull) {
        if (log_since)
            port_log("fps: %.1f, longest gap %.1f ms, %d gaps over 20 ms, presenting %.1f ms a frame (frame %u)\n",
                     log_n * 1e9 / (double)(now - log_since), worst / 1e6, slow, present_ns / 1e6 / log_n, frames);
        present_ns = 0;
        log_since = now;
        log_n = slow = 0;
        worst = 0;
    }
}

/* Where the picture goes in the window: as large as fits at the TV's 4:3 (whatever the PS1's resolution, its
 * pixels fill a 4:3 screen), or with TWINE_ASPECT=fill the whole window (a CRT shown a 1280x1024 mode is
 * still a 4:3 tube). */
static SDL_FRect picture_rect(void) {
    static int fill = -1;
    int ww = 0, wh = 0;
    SDL_FRect r;
    if (fill < 0)
        fill = getenv("TWINE_ASPECT") && !SDL_strcasecmp(getenv("TWINE_ASPECT"), "fill");
    SDL_GetCurrentRenderOutputSize(renderer, &ww, &wh);
    r.x = r.y = 0;
    r.w = (float)ww;
    r.h = (float)wh;
    if (!fill && ww * 3 > wh * 4) {
        r.w = wh * 4.0f / 3.0f;
        r.x = (ww - r.w) / 2;
    } else if (!fill) {
        r.h = ww * 3.0f / 4.0f;
        r.y = (wh - r.h) / 2;
    }
    return r;
}

/* The picture (src of the frame texture) scaled into dst. TWINE_FILTER: "sharp" (default) scales it up by
 * whole multiples with square pixels, then smoothly the rest of the way, so every PS1 pixel comes out the same
 * size without blurring them; "nearest" uses whole screen pixels only (rows of uneven height when the scale
 * isn't whole), "linear" blurs. */
static int filter = -1; /* TWINE_FILTER: 0 nearest, 1 linear, 2 sharp */

static void set_filter(SDL_Texture *t) {
    if (filter < 0) {
        const char *f = getenv("TWINE_FILTER");
        filter = !f || !SDL_strcasecmp(f, "sharp") ? 2 : !SDL_strcasecmp(f, "linear") ? 1 : 0;
    }
    SDL_SetTextureScaleMode(t, filter == 1 ? SDL_SCALEMODE_LINEAR : SDL_SCALEMODE_NEAREST);
}

static void draw_picture(SDL_Texture *texture, const SDL_FRect *src, const SDL_FRect *dst) {
    static SDL_Texture *big;
    static int big_w, big_h;
    int sx, sy;
    set_filter(texture);
    sx = (int)(dst->w / src->w);
    sy = (int)(dst->h / src->h);
    if (filter != 2 || sx < 1 || sy < 1) {
        SDL_RenderTexture(renderer, texture, src, dst);
        return;
    }
    if (!big || big_w != (int)src->w * sx || big_h != (int)src->h * sy) {
        if (big)
            SDL_DestroyTexture(big);
        big_w = (int)src->w * sx;
        big_h = (int)src->h * sy;
        big = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XBGR8888, SDL_TEXTUREACCESS_TARGET, big_w, big_h);
        if (!big) {
            filter = 0;
            SDL_RenderTexture(renderer, texture, src, dst);
            return;
        }
        SDL_SetTextureScaleMode(big, SDL_SCALEMODE_LINEAR);
    }
    SDL_SetRenderTarget(renderer, big);
    SDL_RenderTexture(renderer, texture, src, NULL);
    SDL_SetRenderTarget(renderer, screen); /* (NULL: the window) */
    SDL_RenderTexture(renderer, big, NULL, dst);
}

/* F1: the controls, over the picture; "F1: controls" in a corner for a while the first time the front end and a
 * mission show */
static const char *const help_lines[] = {
    "CONTROLS                         F1 closes",
    "",
    "Mouse ............ look and turn",
    "W / S ............ walk forward / back",
    "A / D ............ strafe",
    "Arrows ........... walk and turn (menus: move)",
    "Left button ...... fire",
    "Right button ..... aim (hold)",
    "Wheel, Q / E ..... change weapon",
    "F, R ............. use (doors, people), reload",
    "C, Ctrl .......... crouch",
    "Enter ............ pause; in menus: choose",
    "Esc .............. pause; in menus: back",
    "Space ............ use; in menus: choose (Cross)",
    "Backspace ........ back (Triangle)",
    "Tab .............. Select (messages)",
    "F11, Alt+Enter ... full screen or window",
    "",
    "Gamepad: left stick walks and strafes, right",
    "stick turns and looks, R2 fire, L2 aim, Square",
    "use / reload, Circle crouch, Triangle / R1 next",
    "and L1 previous weapon, Options pause, touchpad",
    "messages; in menus the pad's own buttons.",
};

static void draw_help(void) {
    static const PortModule *hinted_shell, *hinted_level;
    static Uint64 hint_until;
    int ww = 0, wh = 0, cols = 0, n = sizeof(help_lines) / sizeof(help_lines[0]), i;
    float scale, x, y, w, h;
    SDL_FRect r;
    if (port_module && !strcmp(port_module->name, "shell") && !hinted_shell) {
        hinted_shell = port_module;
        hint_until = SDL_GetTicks() + 8000;
    }
    if (port_module && port_module->level && !hinted_level) {
        hinted_level = port_module;
        hint_until = SDL_GetTicks() + 8000;
    }
    SDL_GetCurrentRenderOutputSize(renderer, &ww, &wh);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (!help_shown) {
        if (SDL_GetTicks() >= hint_until)
            return;
        scale = (float)(wh / 360 > 1 ? wh / 360 : 1);
        SDL_SetRenderScale(renderer, scale, scale);
        r.x = 4;
        r.y = 4;
        r.w = 12 * 8 + 8;
        r.h = 16;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 170);
        SDL_RenderFillRect(renderer, &r);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDebugText(renderer, 8, 8, "F1: controls");
        SDL_SetRenderScale(renderer, 1, 1);
        return;
    }
    for (i = 0; i < n; i++)
        if ((int)strlen(help_lines[i]) > cols)
            cols = (int)strlen(help_lines[i]);
    /* whole multiples of the 8x8 font, as large as fits */
    scale = (float)SDL_min(ww * 9 / 10 / ((cols + 4) * 8), wh * 9 / 10 / ((n + 2) * 12));
    if (scale < 1)
        scale = 1;
    w = (float)(cols + 4) * 8;
    h = (float)(n + 2) * 12;
    x = (ww / scale - w) / 2;
    y = (wh / scale - h) / 2;
    SDL_SetRenderScale(renderer, scale, scale);
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 215);
    SDL_RenderFillRect(renderer, &r);
    for (i = 0; i < n; i++) {
        if (i == 0)
            SDL_SetRenderDrawColor(renderer, 255, 210, 80, 255);
        else
            SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
        SDL_RenderDebugText(renderer, x + 16, y + 12 + i * 12, help_lines[i]);
    }
    SDL_SetRenderScale(renderer, 1, 1);
}

/* The hardware renderer's picture of the VRAM rectangle (rx, ry, w, h) in a texture of the renderer: the size
 * it is shown at (dst), but no larger than drawn (scale times VRAM's pixels) and no smaller than VRAM's;
 * `src` is the part of the texture it fills. */
static SDL_Texture *hw_picture(s32 rx, s32 ry, s32 w, s32 h, const SDL_FRect *dst, SDL_FRect *src) {
    static SDL_Texture *pic;
    static int pic_w, pic_h;
    int s = gpu_hw_scale(), pw = SDL_min(w * s, SDL_max((int)dst->w, w)), ph = SDL_min(h * s, SDL_max((int)dst->h, h));
    if (!pic || pic_w < pw || pic_h < ph) {
        if (pic)
            SDL_DestroyTexture(pic);
        pic_w = SDL_max(pw, pic_w);
        pic_h = SDL_max(ph, pic_h);
        pic = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, pic_w, pic_h);
        if (!pic)
            port_fatal("video: picture texture %dx%d: %s\n", pic_w, pic_h, SDL_GetError());
        SDL_SetTextureBlendMode(pic, SDL_BLENDMODE_NONE);
    }
    gpu_hw_picture(SDL_GetPointerProperty(SDL_GetTextureProperties(pic), SDL_PROP_TEXTURE_GPU_TEXTURE_POINTER, NULL),
                   pw, ph, rx, ry, w, h);
    src->x = src->y = 0;
    src->w = (float)pw;
    src->h = (float)ph;
    return pic;
}

/* shows the VRAM rectangle at (rx, ry) the size of the display */
static void present_rect(s32 rx, s32 ry, int rgb24) {
    s32 y, w = gpu_display.w, h = gpu_display.h;
    SDL_FRect src, dst = picture_rect();
    SDL_Texture *pic = texture;
    if (w <= 0 || h <= 0 || w > 640 || h > 512)
        return;
    src.x = 0;
    src.y = 0;
    src.w = (float)w;
    src.h = (float)h;
    if (gpu_hw_on && !rgb24)
        pic = hw_picture(rx, ry, w, h, &dst, &src);
    else {
        gpu_sync(rx, ry, rgb24 ? (w * 3 + 1) / 2 : w, h);
        for (y = 0; y < h; y++)
            vram_line(rx, ry, rgb24, y, w, &pixels[y * 640]);
        SDL_UpdateTexture(texture, NULL, pixels, 640 * 4);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if (!gpu_display.blank)
        draw_picture(pic, &src, &dst);
    draw_help();
    if (screen_shot) {
        SDL_Surface *s = SDL_RenderReadPixels(renderer, NULL), *rgb = s ? SDL_ConvertSurface(s, SDL_PIXELFORMAT_RGB24) : NULL;
        FILE *f = rgb ? fopen(screen_shot, "wb") : NULL;
        if (f) {
            int y;
            fprintf(f, "P6\n%d %d\n255\n", rgb->w, rgb->h);
            for (y = 0; y < rgb->h; y++)
                fwrite((u8 *)rgb->pixels + y * rgb->pitch, 1, rgb->w * 3, f);
            fclose(f);
        }
        SDL_DestroySurface(rgb);
        SDL_DestroySurface(s);
        screen_shot = NULL;
    }
    {
        static int yawlog = -1;
        Uint64 t = SDL_GetTicksNS();
        SDL_RenderPresent(renderer);
        present_ns += SDL_GetTicksNS() - t;
        /* TWINE_YAWLOG=1: when each frame was shown and the camera yaw in it */
        if (yawlog < 0)
            yawlog = getenv("TWINE_YAWLOG") && atoi(getenv("TWINE_YAWLOG"));
        if (yawlog)
            port_log("yaw %llu %d %u\n", (unsigned long long)SDL_GetTicksNS(), port_shown_yaw, frames);
    }
    count_frame();
}

static void present(void) {
    present_rect(gpu_display.x, gpu_display.y, gpu_display.rgb24);
}

/* A frame interp.c had drawn (the `index`th of this tick) at (x, y): shown at once. */
void port_video_present_draw(int index, s32 x, s32 y) {
    const char *shots = getenv("TWINE_SHOT");
    RT_ENTER();
    if (headless && (shots || gpu_hw_on)) {
        char path[64];
        int shot = shots && shot_wanted();
        snprintf(path, sizeof(path), "shot_%04u_%d.ppm", frames, index);
        if (gpu_hw_on)
            gpu_hw_frame(frames, x, y, gpu_display.w, gpu_display.h, shot ? path : NULL);
        else if (shot)
            dump_rect(path, x, y, 0);
        if (screen) {
            static char sp[64];
            snprintf(sp, sizeof(sp), "screen_%04u_%d.ppm", frames, index);
            screen_shot = shot ? sp : NULL;
            present_rect(x, y, 0);
        }
    }
    if (!headless) {
        poll_input(0);
        present_rect(x, y, 0);
    }
    RT_LEAVE();
}

/* ---- the intro (intro.c), before the game: its frames don't count as the game's ---- */

/* the refresh rate of the display the window is on (0: no window, or not known) */
float port_video_refresh(void) {
    const SDL_DisplayMode *m;
    if (!window)
        return 0;
    m = SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(window));
    return m ? m->refresh_rate : 0;
}

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
            ((e.key.scancode == SDL_SCANCODE_RETURN && !(e.key.mod & SDL_KMOD_ALT)) || e.key.key == SDLK_ESCAPE ||
             e.key.scancode == SDL_SCANCODE_SPACE || e.key.scancode == SDL_SCANCODE_Z))
            skip = 1;
        if (e.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN &&
            (e.gbutton.button == SDL_GAMEPAD_BUTTON_START || e.gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH))
            skip = 1;
    }
    RT_LEAVE();
    return skip;
}

/* Whether a key or button that skips (port_video_poll_skip) is still held down */
int port_video_skip_held(void) {
    const bool *k = SDL_GetKeyboardState(NULL);
    int i, held = k[SDL_SCANCODE_RETURN] || k[SDL_SCANCODE_ESCAPE] || k[SDL_SCANCODE_SPACE] || k[SDL_SCANCODE_Z];
    for (i = 0; i < MAX_PADS && !held; i++)
        held = pads[i] && (SDL_GetGamepadButton(pads[i], SDL_GAMEPAD_BUTTON_START) ||
                           SDL_GetGamepadButton(pads[i], SDL_GAMEPAD_BUTTON_SOUTH));
    return held;
}

extern u32 gpu_stats[8];
extern int gpu_poly_log;

/* The controls between frames: once a vblank while the game waits (irq.c), so how long a key is held reaches
 * the game as it would from a pad (the front end shows a frame only 15 times a second). */
void port_poll_input(void) {
    if (headless)
        return;
    RT_ENTER();
    poll_input(0);
    RT_LEAVE();
}

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
        poll_input(1);
        if (!port_smooth_shown) /* else interp.c has shown it */
            present();
    } else if (screen && !port_smooth_shown)
        present();
    if (getenv("TWINE_TEST_PAD") || getenv("TWINE_TEST_STICK"))
        test_pad();
    port_smooth_shown = 0;
    RT_LEAVE();
}
