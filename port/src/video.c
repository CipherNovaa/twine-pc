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
 * TWINE_HEADLESS=1 runs without a window; TWINE_SHOT=<frame>[,<frame>...] (or <first>-<last>) dumps frames as PPM (with the hardware
 * renderer its picture, at its full resolution, and also screen_NNNN.ppm: what a 1280x960 window would show,
 * drawn by SDL's GPU renderer into a texture the way it draws the window);
 * TWINE_TEST_LOOK=<yaw>,<pitch> stands in for the mouse there (angle units each game frame), and
 * TWINE_TEST_PAD=<frame>:<hex>[,...] for the pad (from each frame on, the pad bits below are held) and
 * TWINE_TEST_STICK=<rx>,<ry>,<lx>,<ly> (hex) for its sticks; TWINE_TEST_KEYS=<frame>:<name>[:<frames>][,...] presses
 * keys, mouse buttons, wheel steps and gamepad buttons by their twine.ini names (held 3 frames, or as many as
 * given), read through the controls' bindings (controls.c) as a player's are (at the blackjack table, through the
 * table's own keys, table_bits; F1 toggles the help).
 * Which keys and buttons do what: controls.c (twine.ini's [controls] block, the front end's CONTROLS page). */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL3/SDL.h>
#include "port.h"
#include "gpu.h"
#include "gpu_hw.h"
#include "intro.h"
#include "module.h"
#include "controls.h"

extern u16 port_pad_buttons[2];
void port_notice(const char *key, const char *fmt, ...); /* setup.c */
extern int port_smooth_shown;
extern u8 port_pad_sticks[2][4];
void port_pad_motors(u8 *small, u8 *large);

static u32 frames;
static int headless;

/* ms on the game's clock (irq.c: the host's, or game time with TWINE_TURBO), for what is drawn over the picture
 * for a while (hints, messages, the debug text, a flash bang's white): the screen shots then show them as long */
static Uint64 ui_ms(void) {
    return port_clock_ns() / 1000000;
}
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static SDL_Texture *screen;      /* headless with the hardware renderer: the window's stand-in */
static const char *screen_shot;  /* the PPM the next picture of it goes to */
#define MAX_PADS 8
static SDL_Gamepad *gamepad;          /* the one read */
static SDL_Gamepad *pads[MAX_PADS];   /* all connected */
static u32 pixels[640 * 512];

int port_debug_key; /* F9 was pressed, for the debug menu */
int port_debug_step; /* Page Up / Page Down: the debug menu's COLLBOX INFO steps (mods/debugmenu.c) */

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
    {SDL_SCANCODE_SPACE, P_CROSS}, {SDL_SCANCODE_BACKSPACE, P_TRI}, /* (Tab, messages: controls.c's) */
};

/* Typing on the front end's initials entry (fixes/shell.c sets port_text_entry while it shows): Shift and a letter
 * enter the letter, Shift and Backspace take one back (by scancode: any layout's letters). Without Shift the keys
 * are the menus' as ever. */
int port_text_entry;
static char typed[8];
static int typed_n;

int port_text_take(void) {
    static int test_done;
    int c, i;
    if (!test_done && getenv("TWINE_TEST_TYPE")) { /* (headless tests: these letters typed, '-' a Backspace) */
        const char *t = getenv("TWINE_TEST_TYPE");
        for (test_done = 1; *t && typed_n < (int)sizeof(typed); t++)
            typed[typed_n++] = *t == '-' ? '\b' : *t;
    }
    if (!typed_n)
        return 0;
    c = typed[0];
    for (i = 1; i < typed_n; i++)
        typed[i - 1] = typed[i];
    typed_n--;
    return c;
}

/* The casino mission's blackjack table (fixes/level11.c says when Bond is at it, every frame of the mission:
 * port_blackjack_table; the front end's BLACKJACK, port_blackjack, is that mission at its table). There the keys
 * are the table's alone, the pad's buttons its prompts show (func_800730A4 the bet, func_80072288 the hand,
 * func_80073380 insurance): Cross bets (DONE) and hits, Square stays, Circle doubles down, Triangle leaves (EXIT);
 * R1 / L1 raise and lower the bet by the table's minimum (1000 at the high table) and are SPLIT / PASS in a hand
 * and INSURANCE? Y / N; R2 / L2 change the bet by a tenth of it. Z / X answer insurance only while it asks. Esc and
 * Enter are the table's too (below, in poll_input), Alt+Enter still the full screen. */
extern int port_blackjack;
static const struct {
    SDL_Scancode key;
    u16 bit;
} bj_keys[] = {
    {SDL_SCANCODE_W, P_R1},     {SDL_SCANCODE_UP, P_R1},      {SDL_SCANCODE_S, P_L1},      {SDL_SCANCODE_DOWN, P_L1},
    {SDL_SCANCODE_D, P_R2},     {SDL_SCANCODE_RIGHT, P_R2},   {SDL_SCANCODE_A, P_L2},      {SDL_SCANCODE_LEFT, P_L2},
    {SDL_SCANCODE_SPACE, P_CROSS}, {SDL_SCANCODE_E, P_SQUARE}, {SDL_SCANCODE_Q, P_CIRC},
    {SDL_SCANCODE_BACKSPACE, P_TRI},
};
static const char *const bj_help[] = {
    "BLACKJACK",
    "",
    "Space, Enter ..... bet / hit",
    "@E ................ stay",
    "@Q ................ double down",
    "@Z / @X ............ insurance: yes / no",
    "@W / @S, Up/Down .... bet +/- 1000; split / pass",
    "@D / @A, Right/Left .. bet +/- 100",
    "Esc, Backspace ... leave the table",
    "Tab .............. how to play (MI6's message)",
    "F5, F6 ........... save state, load state",
    "",
    "GAMEPAD",
    "",
    "Cross ............ bet / hit",
    "Square ........... stay",
    "Circle ........... double down",
    "R1 / L1 .......... bet +/- 1000; split / pass;",
    "                   insurance: yes / no",
    "R2 / L2 .......... bet +/- 100",
    "Triangle ......... leave the table",
};

static int table_state;
static u32 table_at;

/* fixes/level11.c, each frame of the casino mission: 1 while Bond is at the blackjack table (sitting down, seated
 * or getting up), 2 while its INSURANCE? waits for an answer, else 0 */
void port_blackjack_table(int state) {
    table_state = state;
    table_at = port_vblank_count();
}

static int test_keys_on(void);

/* the pad bits the table's keys press (keys: the keyboard's state, by scancode: key_state's) */
static u16 table_bits(const bool *keys, int state) {
    u16 down = 0;
    size_t i;
    SDL_Scancode esc = test_keys_on() ? SDL_SCANCODE_ESCAPE : SDL_GetScancodeFromKey(SDLK_ESCAPE, NULL);
    for (i = 0; i < sizeof(bj_keys) / sizeof(bj_keys[0]); i++)
        if (keys[bj_keys[i].key])
            down |= bj_keys[i].bit;
    if (keys[esc != SDL_SCANCODE_UNKNOWN ? esc : SDL_SCANCODE_ESCAPE])
        down |= P_TRI;
    if (keys[SDL_SCANCODE_RETURN] && !(SDL_GetModState() & SDL_KMOD_ALT))
        down |= P_CROSS;
    if (state == 2 && keys[SDL_SCANCODE_Z])
        down |= P_R1; /* INSURANCE? Y */
    if (state == 2 && keys[SDL_SCANCODE_X])
        down |= P_L1; /* N */
    return down;
}

/* the game's actions (g_Args->buttonMap); reload is also the button that uses things (doors, people) */
enum { A_FIRE, A_PREV_WEAPON, A_NEXT_WEAPON, A_RELOAD, A_CROUCH, A_AIM, A_STRAFE_LEFT, A_STRAFE_RIGHT };

/* the controls (controls.c) that press one of the game's actions: the keyboard's and mouse's, and the gamepad's in
 * the modern layout (use presses the game's reload button, which uses things; reload itself: port_reload) */
static const struct {
    int ctl, action;
} ctl_actions[] = {
    {CTL_FIRE, A_FIRE}, {CTL_AIM, A_AIM}, {CTL_USE, A_RELOAD}, {CTL_PREV_WEAPON, A_PREV_WEAPON},
    {CTL_NEXT_WEAPON, A_NEXT_WEAPON}, {CTL_CROUCH, A_CROUCH}, {CTL_STRAFE_LEFT, A_STRAFE_LEFT},
    {CTL_STRAFE_RIGHT, A_STRAFE_RIGHT},
}, pad_actions[] = {
    {CTL_PAD_FIRE, A_FIRE}, {CTL_PAD_CROUCH, A_CROUCH}, {CTL_PAD_USE, A_RELOAD},
    {CTL_PAD_NEXT_WEAPON, A_NEXT_WEAPON}, {CTL_PAD_PREV_WEAPON, A_PREV_WEAPON},
};

static int mouse_captured, wheel, wheel_pulse;
static int help_shown; /* F1 */
static char message[64]; /* port_video_message's, until message_until */
static Uint64 message_until;
void port_state_request(int load);
static Uint64 mouse_moved; /* when the mouse last moved (SDL ticks) */
static Uint64 mouse_taken; /* when the game last took its movement (port_mouse_look) */
static int wheel_held; /* the wheel step the game's next tick has: 1 up, 2 down (what it is bound to held) */
static int jump_on = -1; /* TWINE_JUMP: Space jumps (else it punches) */
static int melee_held, melee_pressed; /* Space, the mouse's back button or R3 (the quick punch mod, port_quick_melee) */
static int grenade_held, grenade_pressed; /* G or R1 (the quick grenade mod, port_quick_grenade) */
static int reload_tap; /* R or Square seen since the game last asked (port_reload) */
static int sprint_held, sprint_on; /* L3 switches sprinting on until Bond stops (port_sprint) */
static int jump_held, jump_pressed; /* Space (the jump mod, port_jump) */
static int weapon_key; /* 1-9: the number key last pressed, -1 Q (the weapon keys mod, port_weapon_key) */
static int last_held; /* the last weapon's key or button (Q, L1) */
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
 * keeps the PS1 pad's buttons and the game's own settings for them): the buttons are actions (controls.c's
 * pad_ ones), whichever PS1 buttons the game's settings give them. By default the triggers fire and aim, the
 * d-pad's left and right change weapons and L1 goes back to the last one (as Q). Cross and the d-pad's up and
 * down stay the pad's, for the prompts a mission puts up, and Start, Select and the touchpad too. */
static const struct {
    SDL_GamepadButton b;
    u16 bit;
} modern_pad[] = {
    {SDL_GAMEPAD_BUTTON_DPAD_UP, P_UP},     {SDL_GAMEPAD_BUTTON_DPAD_DOWN, P_DOWN},
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

static int at_table(void) {
    return port_vblank_count() - table_at < 10 && !in_mission_menu() && !in_gameplay() ? table_state : 0;
}

static int modern_layout(void) {
    return !(getenv("TWINE_CONTROLS") && !SDL_strcasecmp(getenv("TWINE_CONTROLS"), "original"));
}

/* The input as it is now: SDL's, or headless TWINE_TEST_KEYS's (test_keys) */
static int testing_keys = -1;
static bool fake_keys[SDL_SCANCODE_COUNT];
static u32 fake_mouse;
static u8 fake_pad[SDL_GAMEPAD_BUTTON_COUNT], fake_trigger[2];

static int test_keys_on(void) {
    if (testing_keys < 0)
        testing_keys = headless && getenv("TWINE_TEST_KEYS") != NULL;
    return testing_keys;
}

static const bool *key_state(void) {
    return test_keys_on() ? fake_keys : SDL_GetKeyboardState(NULL);
}

static int pad_button(SDL_GamepadButton b) {
    return test_keys_on() ? fake_pad[b] : gamepad && SDL_GetGamepadButton(gamepad, b);
}

static int pad_trigger(int right) {
    return test_keys_on() ? fake_trigger[right]
                          : gamepad && SDL_GetGamepadAxis(gamepad, right ? SDL_GAMEPAD_AXIS_RIGHT_TRIGGER
                                                                         : SDL_GAMEPAD_AXIS_LEFT_TRIGGER) > 16000;
}

/* one of the controls held (controls.c's bindings): `what` 1 its keys, 2 its mouse buttons and wheel steps (in a
 * mission, the mouse captured), 4 its gamepad buttons */
static int held_in(int c, int what) {
    const bool *keys = key_state();
    u32 mouse = what & 2 && mouse_captured ? (test_keys_on() ? fake_mouse : SDL_GetMouseState(NULL, NULL)) : 0;
    int i;
    for (i = 0; i < MAX_BINDS; i++) {
        Bind b = port_binds[c][i];
        switch (b.kind) {
        case BIND_KEY:
            if (what & 1 && keys && b.code < SDL_SCANCODE_COUNT && keys[b.code])
                return 1;
            break;
        case BIND_MOUSE:
            if (mouse & SDL_BUTTON_MASK(b.code))
                return 1;
            break;
        case BIND_WHEEL:
            if (what & 2 && mouse_captured && wheel_held == b.code + 1)
                return 1;
            break;
        case BIND_PAD:
            if (what & 4 && b.code < SDL_GAMEPAD_BUTTON_COUNT && pad_button((SDL_GamepadButton)b.code))
                return 1;
            break;
        case BIND_TRIGGER:
            if (what & 4 && pad_trigger(b.code))
                return 1;
            break;
        }
    }
    return 0;
}
#define held(c) held_in(c, 3) /* (a keyboard and mouse control) */

/* The CONTROLS page (fixes/shell.c) waiting for a key or button to bind (port_control_set) to control
 * listen_ctl; meanwhile, and until everything is let go after, the game gets nothing pressed. Esc gives up
 * (and Start, for a gamepad's), as does 10 s of nothing. */
static int listen_ctl = -1, listen_add, listen_result, input_quiet;
static u32 listen_since;
int port_controls_page; /* (the page shows: F1 lists its keys) */

void port_controls_listen(int c, int add) {
    listen_ctl = c;
    listen_add = add;
    listen_result = 0;
    listen_since = port_vblank_count();
    port_log("controls: waiting for a %s for %s\n", c >= CTL_FIRST_PAD ? "gamepad button" : "key",
             port_control_key(c));
}

/* 0 still waiting, 1 bound, -1 given up */
int port_controls_heard(void) {
    if (listen_ctl >= 0 && port_vblank_count() - listen_since > 600) {
        port_log("controls: nothing pressed for %s\n", port_control_key(listen_ctl));
        listen_ctl = -1;
        listen_result = -1;
        input_quiet = 1;
    }
    return listen_ctl >= 0 ? 0 : listen_result;
}

static void heard(Bind b) {
    int pad = listen_ctl >= CTL_FIRST_PAD;
    if (listen_ctl < 0)
        return;
    if ((b.kind == BIND_KEY && b.code == SDL_GetScancodeFromKey(SDLK_ESCAPE, NULL)) ||
        (b.kind == BIND_KEY && b.code == SDL_SCANCODE_ESCAPE) ||
        (pad && b.kind == BIND_PAD && b.code == SDL_GAMEPAD_BUTTON_START)) {
        port_log("controls: %s left as it was\n", port_control_key(listen_ctl));
        listen_ctl = -1;
        listen_result = -1;
        input_quiet = 1;
        return;
    }
    if (pad != (b.kind == BIND_PAD || b.kind == BIND_TRIGGER) || port_bind_reserved(b))
        return; /* (not one for this control: still waiting) */
    port_control_set(listen_ctl, b, listen_add);
    listen_ctl = -1;
    listen_result = 1;
    input_quiet = 1;
}

/* anything held (the CONTROLS page waits for all of it let go after a key is bound) */
static int anything_held(void) {
    const bool *keys = key_state();
    int i;
    for (i = 0; keys && i < SDL_SCANCODE_COUNT; i++)
        if (keys[i])
            return 1;
    if (test_keys_on() ? fake_mouse : SDL_GetMouseState(NULL, NULL))
        return 1;
    for (i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++)
        if (pad_button((SDL_GamepadButton)i))
            return 1;
    return pad_trigger(0) || pad_trigger(1);
}

/* headless: TWINE_TEST_KEYS's keys and buttons held at this frame (pressed: as their events would, the wheel's
 * steps and the CONTROLS page's waiting) */
static void test_keys(void) {
    const char *s = getenv("TWINE_TEST_KEYS");
    memset(fake_keys, 0, sizeof(fake_keys));
    memset(fake_pad, 0, sizeof(fake_pad));
    memset(fake_trigger, 0, sizeof(fake_trigger));
    fake_mouse = 0;
    while (s && *s) {
        char *end, name[48];
        u32 at = (u32)strtoul(s, &end, 10), len = 3;
        int k = 0, n;
        Bind b[2];
        if (*end == ':')
            for (end++; *end && *end != ',' && *end != ':' && k < 47; end++)
                name[k++] = *end;
        name[k] = 0;
        if (*end == ':')
            len = (u32)strtoul(end + 1, &end, 10);
        while (*end && *end != ',')
            end++;
        s = *end ? end + 1 : end;
        if (!(n = port_bind_parse(name, 0, b, 2)) && !(n = port_bind_parse(name, 1, b, 1))) {
            if (frames == at)
                port_log("test keys: no key or button called \"%s\"\n", name);
            continue;
        }
        if (frames < at || frames >= at + len)
            continue;
        if (frames == at) {
            port_log("test keys: %s pressed at frame %u (table %d)\n", port_bind_name(b[0], 0), frames, at_table());
            heard(b[0]);
            if (b[0].kind == BIND_KEY && b[0].code == SDL_SCANCODE_F1) /* (as its key-down event: window_event) */
                help_shown = !help_shown;
        }
        for (k = 0; k < n; k++)
            switch (b[k].kind) {
            case BIND_KEY:
                fake_keys[b[k].code] = 1;
                break;
            case BIND_MOUSE:
                fake_mouse |= SDL_BUTTON_MASK(b[k].code);
                break;
            case BIND_WHEEL:
                if (frames == at && mouse_captured)
                    wheel += b[k].code ? 1 : -1;
                break;
            case BIND_PAD:
                fake_pad[b[k].code] = 1;
                break;
            case BIND_TRIGGER:
                fake_trigger[b[k].code] = 1;
                break;
            }
    }
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
    if (!SDL_Init(SDL_INIT_VIDEO) || !gpu_hw_init()) {
        port_log("video: no hardware renderer (%s); drawing in software\n", SDL_GetError());
        port_notice("renderer", "The hardware renderer (renderer = hw) couldn't start: no usable Vulkan driver "
                                "for this 32-bit program. The game draws with the software renderer instead. (%s)",
                    SDL_GetError());
    }
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
        /* (a headless run never opens the computer's sound: SDL's dummy driver for anything that asks, the HD films'
         * player included; the game's sound goes to TWINE_WAV when asked) */
        SDL_SetHint(SDL_HINT_AUDIO_DRIVER, "dummy");
        if (gpu_hw_wanted())
            headless_hw();
        return;
    }
    RT_ENTER();
    /* more gamepads than SDL knows: a gamecontrollerdb.txt (SDL's mapping format) next to twine.ini */
    {
        const char *xdg = getenv("XDG_CONFIG_HOME"), *home = getenv("HOME");
        char db[1024];
        if (xdg && *xdg)
            snprintf(db, sizeof(db), "%s/twine/gamecontrollerdb.txt", xdg);
        else
            snprintf(db, sizeof(db), "%s/.config/twine/gamecontrollerdb.txt", home ? home : ".");
        if (access(db, R_OK) == 0)
            SDL_SetHint(SDL_HINT_GAMECONTROLLERCONFIG_FILE, db);
    }
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
        port_fatal("No display to open a window on (%s). Start TWINE from a desktop session.\n", SDL_GetError());
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
        port_fatal("Couldn't open the game's window: %s\n", SDL_GetError());
    /* the hardware renderer's device shows the window too (SDL's GPU renderer on it) */
    if (gpu_hw_wanted() && gpu_hw_init() && !(renderer = SDL_CreateGPURenderer(gpu_hw_device(), window))) {
        port_log("video: no GPU renderer for the hardware renderer (%s); drawing in software\n", SDL_GetError());
        gpu_hw_on = 0;
    }
    if (gpu_hw_wanted() && !gpu_hw_on) /* (the setup screen shows it before the game: setup.c) */
        port_notice("renderer", "The hardware renderer (renderer = hw) couldn't start: no usable Vulkan driver "
                                "for this 32-bit program. The game draws with the software renderer instead. (%s)",
                    SDL_GetError());
    if (!renderer && !(renderer = SDL_CreateRenderer(window, NULL)) &&
        !(renderer = SDL_CreateRenderer(window, SDL_SOFTWARE_RENDERER)))
        port_fatal("Couldn't draw in the game's window: %s\n", SDL_GetError());
    if (!SDL_strcmp(SDL_GetRendererName(renderer), SDL_SOFTWARE_RENDERER))
        port_notice("software", "No graphics acceleration (OpenGL or Vulkan) was found for this 32-bit program, so "
                                "the picture is scaled on the CPU: it may run slowly in a large window. Installing "
                                "your graphics driver's 32-bit libraries (as Steam needs) fixes it.");
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
int port_shot_now; /* (tests: a shot of the next frame shown, asked for by the code: fixes/shell.c's) */
static int shot_wanted(void) {
    const char *shots = getenv("TWINE_SHOT");
    if (port_shot_now) {
        port_shot_now = 0;
        return 1;
    }
    char want[16], list[256], *s;
    if (!shots)
        return 0;
    snprintf(want, sizeof(want), "%u", frames);
    strncpy(list, shots, sizeof(list) - 1);
    list[sizeof(list) - 1] = 0;
    for (s = strtok(list, ","); s; s = strtok(NULL, ",")) {
        unsigned a, b;
        if (sscanf(s, "%u-%u", &a, &b) == 2 ? frames >= a && frames <= b : !strcmp(s, want))
            return 1;
    }
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
        start = port_clock_ns();
    sscanf(test, "%d,%d", &y, &p);
    ticks = (float)(port_clock_ns() - start) / 50e6f;
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
    const char *sens = getenv("TWINE_MOUSE_SENS"); /* (read each time: the front end's PORT SETTINGS change it) */
    float scale = 0.5f * (sens && atof(sens) > 0 ? (float)atof(sens) : 1.0f);
    if (!mouse_captured)
        return;
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
    if (e->type == SDL_EVENT_KEY_DOWN && !e->key.repeat && port_text_entry && (e->key.mod & SDL_KMOD_SHIFT) &&
        typed_n < (int)sizeof(typed) &&
        ((e->key.scancode >= SDL_SCANCODE_A && e->key.scancode <= SDL_SCANCODE_Z) || e->key.scancode == SDL_SCANCODE_BACKSPACE))
        typed[typed_n++] = e->key.scancode == SDL_SCANCODE_BACKSPACE ? '\b' : 'a' + (e->key.scancode - SDL_SCANCODE_A);
    if (e->type == SDL_EVENT_KEY_DOWN && !e->key.repeat && e->key.scancode == SDL_SCANCODE_F1)
        help_shown = !help_shown;
    /* F5 saves a state, F6 loads it (savestate.c) */
    if (e->type == SDL_EVENT_KEY_DOWN && !e->key.repeat &&
        (e->key.scancode == SDL_SCANCODE_F5 || e->key.scancode == SDL_SCANCODE_F6))
        port_state_request(e->key.scancode == SDL_SCANCODE_F6);
    /* F9 opens the developers' debug menu (mods/savestate.c), with PORT SETTINGS' debug menu on */
    if (e->type == SDL_EVENT_KEY_DOWN && !e->key.repeat && e->key.scancode == SDL_SCANCODE_F9 &&
        getenv("TWINE_DEBUG_MENU") && atoi(getenv("TWINE_DEBUG_MENU")))
        port_debug_key = 1;
    if (e->type == SDL_EVENT_KEY_DOWN && (e->key.scancode == SDL_SCANCODE_PAGEUP || e->key.scancode == SDL_SCANCODE_PAGEDOWN))
        port_debug_step += e->key.scancode == SDL_SCANCODE_PAGEDOWN ? 1 : -1;
    if (e->type == SDL_EVENT_KEY_DOWN && !e->key.repeat && e->key.scancode >= SDL_SCANCODE_1 &&
        e->key.scancode <= SDL_SCANCODE_9 && in_gameplay())
        weapon_key = e->key.scancode - SDL_SCANCODE_1 + 1;
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
    /* the CONTROLS page waiting for a key or button */
    if (listen_ctl >= 0) {
        Bind b = {BIND_NONE, 0};
        if (e->type == SDL_EVENT_KEY_DOWN && !e->key.repeat)
            b = (Bind){BIND_KEY, (unsigned short)(e->key.key == SDLK_ESCAPE ? SDL_SCANCODE_ESCAPE : e->key.scancode)};
        else if (e->type == SDL_EVENT_MOUSE_BUTTON_DOWN && e->button.button >= 1 && e->button.button <= 5)
            b = (Bind){BIND_MOUSE, e->button.button};
        else if (e->type == SDL_EVENT_MOUSE_WHEEL && e->wheel.y != 0)
            b = (Bind){BIND_WHEEL, (unsigned short)(e->wheel.y > 0 ? 0 : 1)};
        else if (e->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN)
            b = (Bind){BIND_PAD, e->gbutton.button};
        else if (e->type == SDL_EVENT_GAMEPAD_AXIS_MOTION && e->gaxis.value > 16000 &&
                 (e->gaxis.axis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER || e->gaxis.axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER))
            b = (Bind){BIND_TRIGGER, (unsigned short)(e->gaxis.axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER)};
        if (b.kind != BIND_NONE)
            heard(b);
    }
}

/* The game's motor values on the gamepad: the small motor (on or off) as its high-frequency rumble, the large
 * one (0-255) as the low-frequency one. Renewed every game frame while on, so they stop when the game stops
 * asking. TWINE_RUMBLE=0 turns it off. */
static void rumble(void) {
    static u8 was_small, was_large;
    static Uint64 renewed;
    int off = getenv("TWINE_RUMBLE") && !atoi(getenv("TWINE_RUMBLE"));
    u8 small, large;
    Uint64 now = SDL_GetTicks();
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
    int capture, modern = modern_layout(), table = at_table();
    size_t i;
    /* the mouse looks around in the levels, while the window has the focus (TWINE_MOUSE=0: never, for playing
     * with a gamepad only); headless, TWINE_TEST_KEYS's mouse is in a level */
    if (test_keys_on())
        capture = port_module && port_module->level;
    else
        capture = port_module && port_module->level && (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS) &&
                  !(getenv("TWINE_MOUSE") && !atoi(getenv("TWINE_MOUSE")));
    if (capture != mouse_captured) {
        if (!test_keys_on())
            SDL_SetWindowRelativeMouseMode(window, capture);
        mouse_captured = capture;
        look_yaw = look_pitch = 0;
        wheel = 0;
    }
    if (test_keys_on())
        test_keys();
    else
        while (SDL_PollEvent(&e))
            window_event(&e);
    keys = key_state();
    /* each wheel step holds what it is bound to (the weapon buttons) for one game frame, then lets go for one */
    if (!mouse_captured)
        wheel_held = 0;
    else if (tick) {
        wheel_held = 0;
        if (wheel_pulse)
            wheel_pulse = 0;
        else if (wheel) {
            wheel_held = wheel < 0 ? 1 : 2;
            wheel += wheel < 0 ? 1 : -1;
            wheel_pulse = 1;
        }
    }
    for (i = 0; i < sizeof(keymap) / sizeof(keymap[0]); i++)
        if (keys[keymap[i].key] && !(keymap[i].key == SDL_SCANCODE_RETURN && (SDL_GetModState() & SDL_KMOD_ALT)) &&
            !(keymap[i].key == SDL_SCANCODE_SPACE && in_gameplay()) &&
            !(keymap[i].key == SDL_SCANCODE_BACKSPACE && port_text_entry && (SDL_GetModState() & SDL_KMOD_SHIFT)))
            down |= keymap[i].bit;
    if (held(CTL_MESSAGES))
        down |= P_SELECT;
    /* at the blackjack table: its keys alone, the pad's buttons its prompts show (no arrows' d-pad, no bindings:
     * no use, weapon, crouch or strafe buttons, no mouse buttons, no keys' sticks) */
    if (table) /* (its messages key too: the MI6 message on how to play) */
        down = table_bits(keys, table) | (down & P_SELECT);
    /* The punch's keys (Space) choose in the menus (Space: keymap) and punch (below) while Bond has the controls;
     * with TWINE_JUMP=1 (and the jump mod built in, PORT_MODS=... JUMP) they jump instead, kept until the game's
     * next tick takes it */
    if (jump_on < 0)
        jump_on = getenv("TWINE_JUMP") && atoi(getenv("TWINE_JUMP"));
    {
        int space = held_in(CTL_PUNCH, 1) && in_gameplay();
        if (jump_on && space && !jump_held)
            jump_pressed = 1;
        jump_held = jump_on && space;
    }
    /* Esc goes back in menus (Triangle; from the top of the phone's pause menu that closes it) and pauses or
     * skips everywhere else (Start); Enter chooses in a mission's menus (Cross) and is Start elsewhere. Esc is
     * the key the keyboard layout calls Escape (e.g. Caps Lock's with caps:swapescape, where the key marked Esc
     * is Caps Lock), where the other keys are where they are on the keyboard. */
    {
        /* (at the blackjack table Esc is its EXIT and Enter its X, not the pause) */
        if (keys[test_keys_on() ? SDL_SCANCODE_ESCAPE : SDL_GetScancodeFromKey(SDLK_ESCAPE, NULL)])
            down |= (port_module && !strcmp(port_module->name, "shell")) || in_mission_menu() || table ? P_TRI : P_START;
        if (keys[SDL_SCANCODE_RETURN] && !(SDL_GetModState() & SDL_KMOD_ALT))
            down |= in_mission_menu() || table ? P_CROSS : P_START;
    }
    for (i = 0; i < sizeof(ctl_actions) / sizeof(ctl_actions[0]); i++)
        if (!table && held(ctl_actions[i].ctl))
            down |= action_bits(ctl_actions[i].action);
    if (!table && (held(CTL_RELOAD) || (modern && held_in(CTL_PAD_RELOAD, 4))))
        reload_tap = 1;
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
    if (held(CTL_FORWARD))
        ly = 0x00;
    if (held(CTL_BACK))
        ly = 0xFF;
    if (held(CTL_STRAFE_LEFT))
        lx = 0x00;
    if (held(CTL_STRAFE_RIGHT))
        lx = 0xFF;
    if (held(CTL_LOOK_UP))
        ry = 0x00;
    if (held(CTL_LOOK_DOWN))
        ry = 0xFF;
    if (held(CTL_TURN_LEFT))
        rx = 0x00;
    if (held(CTL_TURN_RIGHT))
        rx = 0xFF;
    if (table) /* (the table's keys move nothing) */
        lx = ly = rx = ry = 0x80;
    if (gamepad || test_keys_on()) {
        if (gamepad) {
            stick(SDL_GAMEPAD_AXIS_LEFTX, SDL_GAMEPAD_AXIS_LEFTY, &lx, &ly);
            stick(SDL_GAMEPAD_AXIS_RIGHTX, SDL_GAMEPAD_AXIS_RIGHTY, &rx, &ry);
        }
        if (modern && in_gameplay()) {
            for (i = 0; i < sizeof(pad_actions) / sizeof(pad_actions[0]); i++)
                if (held_in(pad_actions[i].ctl, 4))
                    down |= action_bits(pad_actions[i].action);
            for (i = 0; i < sizeof(modern_pad) / sizeof(modern_pad[0]); i++)
                if (pad_button(modern_pad[i].b))
                    down |= modern_pad[i].bit;
            /* aiming, the game leans with the strafe buttons (as A/D do with the mouse's aim): the stick does */
            if (held_in(CTL_PAD_AIM, 4)) {
                down |= action_bits(A_AIM);
                if (lx < 0x50)
                    down |= action_bits(A_STRAFE_LEFT);
                else if (lx > 0xB0)
                    down |= action_bits(A_STRAFE_RIGHT);
            }
        } else {
            for (i = 0; i < sizeof(padmap) / sizeof(padmap[0]); i++)
                if (pad_button(padmap[i].b))
                    down |= padmap[i].bit;
            if (pad_trigger(0))
                down |= P_L2;
            if (pad_trigger(1))
                down |= P_R2;
        }
    }
    /* the punch (Space but for the jump, the mouse's back button; R3 in the modern layout): a quick punch, kept until
     * the game's next tick takes it */
    {
        int melee = in_gameplay() && ((held_in(CTL_PUNCH, 1) && !jump_on) || held_in(CTL_PUNCH, 2) ||
                                      (modern && held_in(CTL_PAD_PUNCH, 4)));
        if (melee && !melee_held)
            melee_pressed = 1;
        melee_held = melee;
    }
    /* the grenade (G, or R1 in the modern layout): a quick grenade, once for each press */
    {
        int grenade = in_gameplay() && (held(CTL_GRENADE) || (modern && held_in(CTL_PAD_GRENADE, 4)));
        if (grenade && !grenade_held)
            grenade_pressed = 1;
        grenade_held = grenade;
    }
    /* the last weapon (Q, or L1 in the modern layout): back to the weapon held before, once for each press */
    {
        int last = in_gameplay() && (held(CTL_LAST_WEAPON) || (modern && held_in(CTL_PAD_LAST_WEAPON, 4)));
        if (last && !last_held)
            weapon_key = -1;
        last_held = last;
    }
    /* the pad's sprint (L3 in the modern layout) switches sprinting on; it goes off when the left stick lets go (or
     * Bond hasn't the controls) */
    {
        int l3 = in_gameplay() && modern && held_in(CTL_PAD_SPRINT, 4);
        if (l3 && !sprint_held)
            sprint_on = !sprint_on;
        sprint_held = l3;
        if (!in_gameplay() || (abs(lx - 0x80) < 0x30 && abs(ly - 0x80) < 0x30))
            sprint_on = 0;
    }
    /* the CONTROLS page waiting for a key, and until all is let go after: nothing for the game */
    if (listen_ctl >= 0 || input_quiet) {
        if (listen_ctl < 0 && !anything_held())
            input_quiet = 0;
        else
            down = 0;
    }
    port_pad_buttons[0] = (u16)~down;
    if (gamepad && tick)
        rumble();
    port_pad_sticks[0][0] = rx;
    port_pad_sticks[0][1] = ry;
    port_pad_sticks[0][2] = lx;
    port_pad_sticks[0][3] = ly;
}

/* 1 while the quick punch is held, and once for a press shorter than a game tick (port/mods/quickpunch.c asks
 * every tick; held, it punches again whenever it can, as the fists do with fire held) */
int port_quick_melee(void) {
    int p = melee_pressed || melee_held;
    melee_pressed = 0;
    return p;
}

/* The game's reload button both reloads and uses things (doors, people, alarms: whatever reads it). Here R and,
 * in the modern layout, Square reload without pressing it (the twin-stick mod sets the game's reload flags);
 * E, F and Triangle press it and the mod clears those flags. Bit 0: a reload key is down (or was tapped since
 * the last call, once a tick); bit 1: a use key is. TWINE_TEST_RELOAD=frame (a tap) for headless tests. */
int port_reload(void) {
    int r = 0, modern = modern_layout();
    if (headless && !test_keys_on())
        return getenv("TWINE_TEST_RELOAD") && frames >= (u32)atoi(getenv("TWINE_TEST_RELOAD")) &&
               frames < (u32)atoi(getenv("TWINE_TEST_RELOAD")) + 2;
    if (listen_ctl >= 0 || input_quiet || at_table()) /* (at the blackjack table: its keys alone) */
        return 0;
    if (held(CTL_RELOAD) || (modern && held_in(CTL_PAD_RELOAD, 4)))
        r |= 1;
    if (held(CTL_USE) || (modern && held_in(CTL_PAD_USE, 4)))
        r |= 2;
    r |= reload_tap;
    reload_tap = 0;
    return r;
}

/* 1 while Bond should sprint: the sprint keys held (Shift), or the pad's sprint on (src/mods/twinstick.c asks every
 * tick) */
int port_sprint(void) {
    if (headless && !test_keys_on()) /* tests: TWINE_TEST_SPRINT=1 holds it */
        return getenv("TWINE_TEST_SPRINT") != NULL;
    return in_gameplay() && (held(CTL_SPRINT) || sprint_on);
}

/* 1 once for each press of the quick grenade (port/mods/quickgrenade.c asks every tick) */
int port_quick_grenade(void) {
    int p = grenade_pressed;
    grenade_pressed = 0;
    return p;
}

/* 1 once for each press of the jump key (port/mods/jump.c asks every tick) */
int port_jump(void) {
    int p = jump_pressed;
    jump_pressed = 0;
    return p;
}

/* the number key pressed since the game last asked (port/mods/weaponkeys.c, every frame Bond has the controls),
 * -1 for Q, else 0 */
int port_weapon_key(void) {
    int k = weapon_key;
    weapon_key = 0;
    return k;
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
    /* <frame>:<input>[,...]: the PC's weapon inputs as they reach the game: wheelup / wheeldown (a mouse wheel step:
     * the weapon button for a game tick), z / x (the keys, held as long), dleft / dright (the modern layout's d-pad),
     * q, l1 (the modern layout's: as Q) and 1-9 (the number keys) */
    s = getenv("TWINE_TEST_INPUT");
    while (s && *s) {
        char *end, name[16];
        u32 at = (u32)strtoul(s, &end, 10);
        int k = 0;
        if (*end == ':')
            for (end++; *end && *end != ',' && k < 15; end++)
                name[k++] = *end;
        name[k] = 0;
        if (frames >= at && frames < at + 3) {
            if (!strcmp(name, "wheelup") || !strcmp(name, "z") || !strcmp(name, "dleft"))
                bits |= action_bits(A_PREV_WEAPON);
            else if (!strcmp(name, "wheeldown") || !strcmp(name, "x") || !strcmp(name, "dright"))
                bits |= action_bits(A_NEXT_WEAPON);
        }
        if (frames == at && (!strcmp(name, "q") || !strcmp(name, "l1")))
            weapon_key = -1;
        else if (frames == at && name[0] >= '1' && name[0] <= '9' && !name[1])
            weapon_key = name[0] - '0';
        s = *end ? end + 1 : end;
    }
    if (test_keys_on()) /* (with TWINE_TEST_KEYS's, poll_input's) */
        port_pad_buttons[0] &= (u16)~bits;
    else
        port_pad_buttons[0] = (u16)~bits;
    s = getenv("TWINE_TEST_MELEE"); /* <frame>[,...]: the quick punch pressed at these frames */
    while (s && *s) {
        char *end;
        if ((u32)strtoul(s, &end, 10) == frames)
            melee_pressed = 1;
        s = *end ? end + 1 : end;
    }
    s = getenv("TWINE_TEST_GRENADE"); /* <frame>[,...]: the quick grenade pressed at these frames */
    while (s && *s) {
        char *end;
        if ((u32)strtoul(s, &end, 10) == frames)
            grenade_pressed = 1;
        s = *end ? end + 1 : end;
    }
    s = getenv("TWINE_TEST_JUMP"); /* <frame>[,...]: jump at these frames */
    while (s && *s) {
        char *end;
        if ((u32)strtoul(s, &end, 10) == frames)
            jump_pressed = 1;
        s = *end ? end + 1 : end;
    }
    s = getenv("TWINE_TEST_WEAPON"); /* <frame>:<n>[,...]: number key n (-1: Q) pressed at these frames */
    while (s && *s) {
        char *end;
        u32 at = (u32)strtoul(s, &end, 10);
        int n = *end == ':' ? (int)strtol(end + 1, &end, 10) : 0;
        if (at == frames)
            weapon_key = n;
        s = *end ? end + 1 : end;
    }
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

int port_shown_fps; /* frames shown in the last second (the debug menu's FRAMERATE INFO) */

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
        port_shown_fps = (int)(n * 1000000000ull / (now - since));
        snprintf(title, sizeof(title), "TWINE - %d fps", port_shown_fps);
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
/* The F1 list, from the bindings (controls.c) as they are, the keys by the names this keyboard's layout gives them
 * (made again every half second: the CONTROLS page changes them) */
static char help_text[56][96];
static const char *help_lines[56];
static int help_n;

static void help_add(const char *keys, const char *does) {
    if (help_n < 56) {
        if (does)
            snprintf(help_text[help_n], sizeof(help_text[0]), "%s .. %s", keys, does);
        else
            snprintf(help_text[help_n], sizeof(help_text[0]), "%s", keys);
        help_lines[help_n] = help_text[help_n];
        help_n++;
    }
}

static void help_ctl(int c, const char *does) {
    char t[80];
    port_control_text(c, 1, t, sizeof(t));
    help_add(t, does);
}

/* the first key of each of four controls, as "W A S D" */
static void help_four(int a, int b, int c, int d, const char *does) {
    int list[4] = {a, b, c, d}, i;
    char t[80] = "";
    for (i = 0; i < 4; i++) {
        size_t n = strlen(t);
        Bind first = port_binds[list[i]][0];
        snprintf(t + n, sizeof(t) - n, "%s%s", i ? " " : "", first.kind ? port_bind_name(first, 1) : "-");
    }
    help_add(t, does);
}

static void make_help(void) {
    char t[160], u[80];
    help_n = 0;
    help_add("CONTROLS", NULL);
    help_add("", NULL);
    help_add("Mouse", "look");
    help_four(CTL_FORWARD, CTL_STRAFE_LEFT, CTL_BACK, CTL_STRAFE_RIGHT, "move");
    help_four(CTL_TURN_LEFT, CTL_TURN_RIGHT, CTL_LOOK_UP, CTL_LOOK_DOWN, "turn, look");
    help_add("Arrows", "move and turn; menus: move");
    help_ctl(CTL_FIRE, "fire");
    help_ctl(CTL_AIM, "aim");
    port_control_text(CTL_PREV_WEAPON, 1, t, 80);
    port_control_text(CTL_NEXT_WEAPON, 1, u, sizeof(u));
    snprintf(t + strlen(t), sizeof(t) - strlen(t), " / %s", u);
    help_add(t, "change weapon");
    help_add("1 - 9", "pick a weapon");
    help_ctl(CTL_LAST_WEAPON, "last weapon");
    help_ctl(CTL_USE, "use");
    help_ctl(CTL_RELOAD, "reload");
    help_ctl(CTL_PUNCH, "punch");
    help_ctl(CTL_GRENADE, "grenade");
    help_ctl(CTL_SPRINT, "sprint");
    help_ctl(CTL_CROUCH, "crouch");
    help_add("Enter", "pause; menus: choose");
    help_add("Esc", "pause; menus: back");
    help_add("Space", "menus: choose");
    help_add("Backspace", "menus: back");
    help_ctl(CTL_MESSAGES, "messages");
    help_add("F5, F6", "save state, load state");
    help_add("F11, Alt+Enter", "full screen");
    help_add("", NULL);
    help_add("GAMEPAD", NULL);
    help_add("", NULL);
    help_add("Left stick", "move");
    help_add("Right stick", "look");
    if (!modern_layout())
        help_add("(classic layout: the PS1 pad's buttons)", NULL);
    else {
        int c;
        for (c = CTL_PAD_FIRE; c < CTL_COUNT; c++)
            if (c == CTL_PAD_PREV_WEAPON) {
                port_control_text(CTL_PAD_PREV_WEAPON, 1, t, 80);
                port_control_text(CTL_PAD_NEXT_WEAPON, 1, u, sizeof(u));
                snprintf(t + strlen(t), sizeof(t) - strlen(t), " / %s", u);
                help_add(t, "change weapon");
            } else if (c != CTL_PAD_NEXT_WEAPON)
                help_ctl(c, c == CTL_PAD_FIRE ? "fire" : c == CTL_PAD_AIM ? "aim" : c == CTL_PAD_USE ? "use" :
                            c == CTL_PAD_RELOAD ? "reload" : c == CTL_PAD_PUNCH ? "punch" :
                            c == CTL_PAD_GRENADE ? "grenade" : c == CTL_PAD_CROUCH ? "crouch" :
                            c == CTL_PAD_SPRINT ? "sprint (on / off)" : "last weapon");
    }
    help_add("Start", "pause");
    help_add("Back, touchpad", "messages");
}

/* on the front end's CONTROLS page: its keys */
static const char *const page_help[] = {
    "CONTROLS PAGE",
    "",
    "Up, Down ......... choose",
    "Enter, Space, X .. change (then press it)",
    "Right ............ add one more",
    "Left ............. back to the default",
    "Esc, Triangle .... back",
    "",
    "Esc while waiting: leave it as it was",
    "Keys: twine.ini's [controls] too",
};

/* A help line as this keyboard's layout names its keys: "@W" is the key in W's place (keys are read by their
 * place), and the dots between the keys and what they do are fitted to the names. */
static const char *const *help_list(int *n) {
    if (port_blackjack == 1 || at_table()) {
        *n = sizeof(bj_help) / sizeof(bj_help[0]);
        return bj_help;
    }
    if (port_controls_page) {
        *n = sizeof(page_help) / sizeof(page_help[0]);
        return page_help;
    }
    {
        static Uint64 made_at;
        if (!help_n || ui_ms() - made_at > 500 || ui_ms() < made_at) {
            make_help();
            made_at = ui_ms();
        }
    }
    *n = help_n;
    return help_lines;
}

static const char *help_line(int i) {
    static char out[96];
    int count;
    const char *s = help_list(&count)[i], *dots = strstr(s, " ..");
    char keys[64];
    int n = 0;
    if (!dots)
        return s;
    for (; s < dots && n < (int)sizeof(keys) - 8; s++)
        if (*s == '@' && s[1] >= 'A' && s[1] <= 'Z') {
            const char *name = SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode)(SDL_SCANCODE_A + s[1] - 'A'),
                                                                     SDL_KMOD_NONE, false));
            if (!name || !*name || strlen(name) > 6)
                name = (char[2]){s[1], 0};
            n += snprintf(keys + n, sizeof(keys) - n, "%s", name);
            s++;
        } else
            keys[n++] = *s;
    keys[n] = 0;
    while (*dots == ' ' || *dots == '.')
        dots++;
    n = snprintf(out, sizeof(out), "%s ", keys);
    while (n < 18 && n < (int)sizeof(out) - 2)
        out[n++] = '.';
    snprintf(out + n, sizeof(out) - n, " %s", dots);
    return out;
}

/* The game's debug-font text (FntPrint, FntFlush: mods/savestate.c), drawn here instead of with its font */
static char dbg_text[2048], dbg_shown[2048];
static Uint64 dbg_until;

void port_fnt_print(const char *text) {
    size_t n = strlen(dbg_text);
    for (; *text && n < sizeof(dbg_text) - 1; text++)
        if (*text == '~' && text[1] == 'c' && text[2] && text[3] && text[4])
            text += 4; /* a colour, ~cRGB */
        else
            dbg_text[n++] = *text;
    dbg_text[n] = 0;
}

static int dbg_flushed;

void port_fnt_flush(void) {
    if (!dbg_text[0])
        return;
    memcpy(dbg_shown, dbg_text, sizeof(dbg_shown));
    dbg_text[0] = 0;
    dbg_until = ui_ms() + 1000;
    dbg_flushed = 1;
    if (getenv("TWINE_TEST_DEBUG_PAGE")) { /* headless tests: the page's text in the log, now and then */
        static int n;
        if (n++ % 100 == 50)
            port_log("debug text:\n%s", dbg_shown);
    }
}

/* A game frame starts (mods/savestate.c): the text goes with the frame that stopped printing it, as the game's
 * box does. */
void port_fnt_frame(void) {
    if (!dbg_flushed)
        dbg_until = 0;
    dbg_flushed = 0;
}

/* where the game's FntOpen puts it (func_8004BDE4: x 32, y 24 of the PS1's screen), in the game's own debug font
 * (FntLoad's: 4-bit glyphs at VRAM 832,256, 16 a row from the space, its colours at 832,384) */
static void draw_debug_text(void) {
    static SDL_Texture *font;
    static Uint64 made_for;
    SDL_FRect dst = picture_rect();
    const char *p = dbg_shown;
    float sx, sy;
    int col = 0, row = 0;
    if (ui_ms() >= dbg_until || gpu_display.w <= 0 || gpu_display.h <= 0)
        return;
    if (!font || made_for + 1000 < ui_ms()) { /* again when the menu opens again */
        static u32 px[128 * 32];
        u16 clut[16];
        int x, y;
        gpu_sync(832, 256, 32, 32);
        gpu_sync(832, 384, 16, 1);
        memcpy(clut, &gpu_vram[384 * 1024 + 832], sizeof(clut));
        for (y = 0; y < 32; y++)
            for (x = 0; x < 128; x++) {
                u16 c = clut[(gpu_vram[(256 + y) * 1024 + 832 + x / 4] >> ((x & 3) * 4)) & 15];
                px[y * 128 + x] = !c ? 0 : 0xFF000000u | (u32)((c & 31) << 3) << 16 | (u32)((c >> 5 & 31) << 3) << 8 |
                                               (u32)((c >> 10 & 31) << 3);
            }
        if (!font) {
            font = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 128, 32);
            SDL_SetTextureBlendMode(font, SDL_BLENDMODE_BLEND);
            SDL_SetTextureScaleMode(font, SDL_SCALEMODE_NEAREST);
        }
        SDL_UpdateTexture(font, NULL, px, 128 * 4);
    }
    made_for = ui_ms();
    sx = dst.w / gpu_display.w;
    sy = dst.h / gpu_display.h;
    for (; *p; p++) {
        int c = SDL_toupper((unsigned char)*p) - 0x20;
        if (*p == '\n') {
            col = 0;
            row++;
            continue;
        }
        if (c > 0 && c < 64) {
            SDL_FRect s = {(float)(c % 16 * 8), (float)(c / 16 * 8), 8, 8};
            SDL_FRect d = {dst.x + (32 + col * 8) * sx, dst.y + (24 + row * 8) * sy, 8 * sx, 8 * sy};
            SDL_RenderTexture(renderer, font, &s, &d);
        }
        col++;
    }
}

/* tests: TWINE_TEST_HELP=1 logs the F1 list once a level shows, and shows it (the hw renderer's screen shots) */
static void test_help(void) {
    static int done;
    int n, i;
    if (done || !port_module || !port_module->level || !getenv("TWINE_TEST_HELP"))
        return;
    done = help_shown = 1;
    help_list(&n);
    for (i = 0; i < n; i++)
        port_log("help: %s\n", help_line(i));
}

static void draw_help(void) {
    static const PortModule *hinted_shell, *hinted_level;
    static Uint64 hint_until;
    int ww = 0, wh = 0, cols = 0, n, i;
    const char *const *list;
    test_help();
    list = help_list(&n);
    float scale, x, y, w, h;
    SDL_FRect r;
    if (port_module && !strcmp(port_module->name, "shell") && !hinted_shell) {
        hinted_shell = port_module;
        hint_until = ui_ms() + 8000;
    }
    if (port_module && port_module->level && !hinted_level) {
        hinted_level = port_module;
        hint_until = ui_ms() + 8000;
    }
    SDL_GetCurrentRenderOutputSize(renderer, &ww, &wh);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (!help_shown) {
        const char *text = ui_ms() < message_until ? message : "F1: controls";
        if ((ui_ms() >= hint_until || getenv("TWINE_NO_HINT")) && text != message) /* (TWINE_NO_HINT: captures) */
            return;
        scale = (float)(wh / 360 > 1 ? wh / 360 : 1);
        SDL_SetRenderScale(renderer, scale, scale);
        r.x = 4;
        r.y = 4;
        r.w = (float)strlen(text) * 8 + 8;
        r.h = 16;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 170);
        SDL_RenderFillRect(renderer, &r);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDebugText(renderer, 8, 8, text);
        SDL_SetRenderScale(renderer, 1, 1);
        return;
    }
    for (i = 0; i < n; i++)
        if ((int)strlen(help_line(i)) > cols)
            cols = (int)strlen(help_line(i));
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
        if (i == 0 || !strcmp(list[i], "GAMEPAD"))
            SDL_SetRenderDrawColor(renderer, 255, 210, 80, 255);
        else
            SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
        SDL_RenderDebugText(renderer, x + 16, y + 12 + i * 12, help_line(i));
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
int port_video_covered(void); /* endless.c */
static Uint64 flash_until, flash_len; /* port_video_flash's */

/* a flash bang's: the picture whited out, fading over ms (endless mode's lent flash bangs) */
void port_video_flash(int ms) {
    flash_len = (Uint64)ms;
    flash_until = ui_ms() + flash_len;
}

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
    if (port_video_covered()) { /* (endless's first start: its survey of the missions, not shown) */
        static const char text[] = "PREPARING ENDLESS MODE";
        int ww = 0, wh = 0;
        float scale;
        SDL_GetCurrentRenderOutputSize(renderer, &ww, &wh);
        scale = (float)(wh / 240 > 1 ? wh / 240 : 1);
        SDL_SetRenderScale(renderer, scale, scale);
        SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
        SDL_RenderDebugText(renderer, (ww / scale - (float)(sizeof(text) - 1) * 8) / 2, (wh / scale - 8) / 2, text);
        SDL_SetRenderScale(renderer, 1, 1);
    } else if (!gpu_display.blank)
        draw_picture(pic, &src, &dst);
    if (ui_ms() < flash_until && flash_len) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, (Uint8)(255 * (flash_until - ui_ms()) / flash_len));
        SDL_RenderFillRect(renderer, &dst);
    }
    draw_debug_text();
    if (!port_video_covered())
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
    u64 pt = PT_BEGIN();
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
    PT_END(PT_PRESENT, pt);
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

void *port_video_window(void) {
    return window;
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

u32 port_video_frames(void) {
    return frames;
}

/* a line at the window's top left for a few seconds (the save states' "State saved", ...) */
void port_video_message(const char *text) {
    snprintf(message, sizeof(message), "%s", text);
    message_until = ui_ms() + 2500;
    port_log("message: %s\n", text);
}

void port_frame_end(void) {
    u64 pt = PT_BEGIN();
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
    } else {
        if (test_keys_on())
            poll_input(1);
        if (screen && !port_smooth_shown)
            present();
    }
    if (getenv("TWINE_TEST_PAD") || getenv("TWINE_TEST_STICK") || getenv("TWINE_TEST_MELEE") || getenv("TWINE_TEST_GRENADE") ||
        getenv("TWINE_TEST_WEAPON") || getenv("TWINE_TEST_JUMP") || getenv("TWINE_TEST_INPUT") || getenv("TWINE_TEST_KEYS"))
        test_pad();
    port_smooth_shown = 0;
    RT_LEAVE();
    PT_END(PT_PRESENT, pt);
}
