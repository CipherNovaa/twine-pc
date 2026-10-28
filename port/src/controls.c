/* The keys and buttons of the game's actions: what video.c reads for each, from twine.ini's [controls] block
 * (or the TWINE_<KEY> variables it stands for, as every setting), changed on the front end's CONTROLS page
 * (fixes/shell.c) and listed by F1 (video.c).
 *
 *   fire = Mouse Left
 *   use = E, F
 *   pad_fire = Right Trigger
 *
 * Keys are read by their place on the keyboard (SDL scancodes) and named as the keyboard's layout names them
 * (SDL_GetKeyFromName, then the place that key is at): on a French keyboard "Z" is the key marked Z, where a US
 * keyboard has W. The defaults are places (W A S D wherever the layout puts its letters), so the block written
 * into twine.ini shows them commented out, by the names this keyboard gives those places; a line uncommented is
 * read by its names. Several keys or buttons an action, separated by commas; "none" for none. A name not known is
 * logged with its line and left out (an action left with nothing it knows keeps its default); a key in [controls]
 * that is no action is logged too. Never fatal. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "port.h"
#include "controls.h"

void port_setting_save(const char *key, const char *value);       /* main.c */
void port_setting_unset(const char *key, const char *shown);      /* main.c */
void port_settings_add_block(const char *block);                  /* main.c */
int port_setting_line(const char *key);                           /* main.c */
void port_settings_each(void (*fn)(const char *key, int line, int controls)); /* main.c */

Bind port_binds[CTL_COUNT][MAX_BINDS] PORT_NOSAVE;

#define K(s) {BIND_KEY, SDL_SCANCODE_##s}
#define M(b) {BIND_MOUSE, b}
#define WH(d) {BIND_WHEEL, d}
#define PB(b) {BIND_PAD, SDL_GAMEPAD_BUTTON_##b}
#define TR(t) {BIND_TRIGGER, t}

static const struct {
    const char *key, *label, *does; /* (does: the F1 overlay's words; NULL: on the line before's) */
    Bind dflt[MAX_BINDS];
} info[CTL_COUNT] = {
    {"fire", "fire", "fire", {M(SDL_BUTTON_LEFT)}},
    {"aim", "aim", "aim", {M(SDL_BUTTON_RIGHT)}},
    {"use", "use", "use", {K(E), K(F)}},
    {"reload", "reload", "reload", {K(R)}},
    {"punch", "punch", "punch", {K(SPACE), M(SDL_BUTTON_X1)}},
    {"grenade", "grenade", "grenade", {K(G)}},
    {"crouch", "crouch", "crouch", {K(C), K(LCTRL)}},
    {"sprint", "sprint", "sprint", {K(LSHIFT), K(RSHIFT)}},
    {"prev_weapon", "prev weapon", "weapon back", {K(Z), WH(0)}},
    {"next_weapon", "next weapon", "weapon on", {K(X), WH(1)}},
    {"last_weapon", "last weapon", "last weapon", {K(Q)}},
    {"forward", "forward", "forward", {K(W)}},
    {"back", "back", "back", {K(S)}},
    {"strafe_left", "step left", "strafe left", {K(A)}},
    {"strafe_right", "step right", "strafe right", {K(D)}},
    {"turn_left", "turn left", "turn left", {K(J)}},
    {"turn_right", "turn right", "turn right", {K(L)}},
    {"look_up", "look up", "look up", {K(I)}},
    {"look_down", "look down", "look down", {K(K)}},
    {"messages", "messages", "messages", {K(TAB)}},
    {"pad_fire", "pad fire", "fire", {TR(1)}},
    {"pad_aim", "pad aim", "aim", {TR(0)}},
    {"pad_use", "pad use", "use", {PB(NORTH)}},
    {"pad_reload", "pad reload", "reload", {PB(WEST)}},
    {"pad_punch", "pad punch", "punch", {PB(RIGHT_STICK)}},
    {"pad_grenade", "pad grenade", "grenade", {PB(RIGHT_SHOULDER)}},
    {"pad_crouch", "pad crouch", "crouch", {PB(EAST)}},
    {"pad_sprint", "pad sprint", "sprint (on / off)", {PB(LEFT_STICK)}},
    {"pad_prev_weapon", "pad prev", "weapon back", {PB(DPAD_LEFT)}},
    {"pad_next_weapon", "pad next", "weapon on", {PB(DPAD_RIGHT)}},
    {"pad_last_weapon", "pad last", "last weapon", {PB(LEFT_SHOULDER)}},
};

const char *port_control_key(int c) {
    return info[c].key;
}

const char *port_control_label(int c) {
    return info[c].label;
}

int port_control_known(const char *key) {
    int c;
    for (c = 0; c < CTL_COUNT; c++)
        if (!strcasecmp(key, info[c].key))
            return 1;
    return 0;
}

/* ---- names ---- */

/* a gamepad's buttons by SDL's names for their places, with the PlayStation's and Xbox's as others; [3] the game's
 * font's (the PlayStation's) */
static const struct {
    int kind, code;
    const char *name, *ps, *xbox;
} pad_names[] = {
    {BIND_PAD, SDL_GAMEPAD_BUTTON_SOUTH, "South", "Cross", "A"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_EAST, "East", "Circle", "B"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_WEST, "West", "Square", "X"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_NORTH, "North", "Triangle", "Y"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_BACK, "Back", "Select", "View"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_GUIDE, "Guide", "PS", "Xbox"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_START, "Start", "Start", "Menu"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_LEFT_STICK, "Left Stick", "L3", "LS"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_RIGHT_STICK, "Right Stick", "R3", "RS"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, "Left Shoulder", "L1", "LB"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, "Right Shoulder", "R1", "RB"},
    {BIND_TRIGGER, 0, "Left Trigger", "L2", "LT"},
    {BIND_TRIGGER, 1, "Right Trigger", "R2", "RT"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_DPAD_UP, "DPad Up", "Dpad Up", "Dpad Up"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_DPAD_DOWN, "DPad Down", "Dpad Down", "Dpad Down"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_DPAD_LEFT, "DPad Left", "Dpad Left", "Dpad Left"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_DPAD_RIGHT, "DPad Right", "Dpad Right", "Dpad Right"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_MISC1, "Misc 1", "Mic", "Share"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1, "Right Paddle 1", "P1", "P1"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_LEFT_PADDLE1, "Left Paddle 1", "P3", "P3"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2, "Right Paddle 2", "P2", "P2"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_LEFT_PADDLE2, "Left Paddle 2", "P4", "P4"},
    {BIND_PAD, SDL_GAMEPAD_BUTTON_TOUCHPAD, "Touchpad", "Touchpad", "Touchpad"},
};

static const char *const mouse_names[] = {"", "Mouse Left", "Mouse Middle", "Mouse Right", "Mouse 4", "Mouse 5"};

/* names compared without case, spaces, '-' and '_' */
static int same_name(const char *a, const char *b) {
    for (;;) {
        while (*a == ' ' || *a == '-' || *a == '_')
            a++;
        while (*b == ' ' || *b == '-' || *b == '_')
            b++;
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return 0;
        if (!*a)
            return 1;
        a++;
        b++;
    }
}

static int ascii(const char *s) {
    for (; *s; s++)
        if ((unsigned char)*s >= 0x7F || (unsigned char)*s < 0x20)
            return 0;
    return 1;
}

/* a key's name as this keyboard's layout gives it (that name read back is the same key), else SDL's name of its
 * place; `plain`: ASCII only (the overlay's font and the game's), else the place's */
static const char *key_name(SDL_Scancode sc, int plain) {
    static char out[64];
    Bind back;
    const char *n = SDL_GetKeyName(SDL_GetKeyFromScancode(sc, SDL_KMOD_NONE, false));
    if (sc == SDL_SCANCODE_COMMA && (!n || !strcmp(n, ",")))
        return "Comma";
    if (n && *n && !strchr(n, ',') && (!plain || ascii(n)) && port_bind_parse(n, 0, &back, 1) == 1 &&
        back.kind == BIND_KEY && back.code == sc) {
        snprintf(out, sizeof(out), "%s", n);
        return out;
    }
    n = SDL_GetScancodeName(sc);
    snprintf(out, sizeof(out), "%s", n && *n ? n : "?");
    return out;
}

const char *port_bind_name(Bind b, int style) {
    size_t i;
    switch (b.kind) {
    case BIND_KEY:
        return key_name((SDL_Scancode)b.code, style != 0);
    case BIND_MOUSE:
        return b.code >= 1 && b.code <= 5 ? mouse_names[b.code] : "?";
    case BIND_WHEEL:
        return b.code ? "Wheel Down" : "Wheel Up";
    case BIND_PAD:
    case BIND_TRIGGER:
        for (i = 0; i < sizeof(pad_names) / sizeof(pad_names[0]); i++)
            if (pad_names[i].kind == b.kind && pad_names[i].code == b.code) {
                static char out[48];
                if (style == 0)
                    return pad_names[i].name;
                if (style == 2)
                    return pad_names[i].ps;
                if (!strcmp(pad_names[i].ps, pad_names[i].xbox))
                    return pad_names[i].ps;
                snprintf(out, sizeof(out), "%s / %s", pad_names[i].ps, pad_names[i].xbox);
                return out;
            }
        return "?";
    }
    return "none";
}

/* one name as the binds it stands for (Shift, Ctrl, Alt: both keys), how many (0: not known) */
int port_bind_parse(const char *name, int pad, Bind *out, int max) {
    static const struct {
        const char *name;
        SDL_Scancode a, b;
    } both[] = {{"Shift", SDL_SCANCODE_LSHIFT, SDL_SCANCODE_RSHIFT}, {"Ctrl", SDL_SCANCODE_LCTRL, SDL_SCANCODE_RCTRL},
                {"Control", SDL_SCANCODE_LCTRL, SDL_SCANCODE_RCTRL}, {"Alt", SDL_SCANCODE_LALT, SDL_SCANCODE_RALT},
                {"Enter", SDL_SCANCODE_RETURN, 0}, {"Esc", SDL_SCANCODE_ESCAPE, 0},
                {"Comma", SDL_SCANCODE_COMMA, 0}, {"Del", SDL_SCANCODE_DELETE, 0},
                {"Spacebar", SDL_SCANCODE_SPACE, 0}};
    size_t i;
    if (max < 1 || !*name)
        return 0;
    if (pad) {
        SDL_GamepadButton b;
        for (i = 0; i < sizeof(pad_names) / sizeof(pad_names[0]); i++)
            if (same_name(name, pad_names[i].name) || same_name(name, pad_names[i].ps) ||
                same_name(name, pad_names[i].xbox)) {
                out->kind = (unsigned char)pad_names[i].kind;
                out->code = (unsigned short)pad_names[i].code;
                return 1;
            }
        if (same_name(name, "lefttrigger") || same_name(name, "righttrigger")) { /* (SDL's own names) */
            out->kind = BIND_TRIGGER;
            out->code = same_name(name, "righttrigger");
            return 1;
        }
        if ((b = SDL_GetGamepadButtonFromString(name)) != SDL_GAMEPAD_BUTTON_INVALID) {
            out->kind = BIND_PAD;
            out->code = (unsigned short)b;
            return 1;
        }
        return 0;
    }
    for (i = 1; i <= 5; i++)
        if (same_name(name, mouse_names[i])) {
            out->kind = BIND_MOUSE;
            out->code = (unsigned short)i;
            return 1;
        }
    {
        static const struct {
            const char *name;
            int b;
        } mouse_more[] = {{"Mouse 1", 1}, {"Mouse 2", 3}, {"Mouse 3", 2}, {"Left Button", 1}, {"Right Button", 3},
                          {"Middle Button", 2}, {"Mouse Back", 4}, {"Mouse Forward", 5}};
        for (i = 0; i < sizeof(mouse_more) / sizeof(mouse_more[0]); i++)
            if (same_name(name, mouse_more[i].name)) {
                out->kind = BIND_MOUSE;
                out->code = (unsigned short)mouse_more[i].b;
                return 1;
            }
    }
    if (same_name(name, "Wheel Up") || same_name(name, "Wheel Down") || same_name(name, "Mouse Wheel Up") ||
        same_name(name, "Mouse Wheel Down")) {
        out->kind = BIND_WHEEL;
        out->code = same_name(name, "Wheel Down") || same_name(name, "Mouse Wheel Down");
        return 1;
    }
    for (i = 0; i < sizeof(both) / sizeof(both[0]); i++)
        if (same_name(name, both[i].name)) {
            out[0].kind = BIND_KEY;
            out[0].code = both[i].a;
            if (!both[i].b || max < 2)
                return 1;
            out[1].kind = BIND_KEY;
            out[1].code = both[i].b;
            return 2;
        }
    {
        SDL_Keycode k = SDL_GetKeyFromName(name);
        SDL_Keymod mod = SDL_KMOD_NONE;
        SDL_Scancode sc = k != SDLK_UNKNOWN ? SDL_GetScancodeFromKey(k, &mod) : SDL_SCANCODE_UNKNOWN;
        if (sc == SDL_SCANCODE_UNKNOWN)
            sc = SDL_GetScancodeFromName(name);
        if (sc != SDL_SCANCODE_UNKNOWN) {
            out->kind = BIND_KEY;
            out->code = (unsigned short)sc;
            return 1;
        }
    }
    return 0;
}

/* what the port keeps for itself: Esc and Enter (pause, menus), the F keys, the number keys (the weapons), Page
 * Up/Down (the debug menu); Start, Back, Guide, the touchpad, Cross and the d-pad's up and down (the prompts) */
int port_bind_reserved(Bind b) {
    if (b.kind == BIND_KEY)
        return b.code == SDL_SCANCODE_ESCAPE || b.code == SDL_SCANCODE_RETURN ||
               (b.code >= SDL_SCANCODE_F1 && b.code <= SDL_SCANCODE_F12) ||
               (b.code >= SDL_SCANCODE_1 && b.code <= SDL_SCANCODE_9) || b.code == SDL_SCANCODE_PAGEUP ||
               b.code == SDL_SCANCODE_PAGEDOWN;
    if (b.kind == BIND_PAD)
        return b.code == SDL_GAMEPAD_BUTTON_START || b.code == SDL_GAMEPAD_BUTTON_BACK ||
               b.code == SDL_GAMEPAD_BUTTON_GUIDE || b.code == SDL_GAMEPAD_BUTTON_TOUCHPAD ||
               b.code == SDL_GAMEPAD_BUTTON_SOUTH || b.code == SDL_GAMEPAD_BUTTON_DPAD_UP ||
               b.code == SDL_GAMEPAD_BUTTON_DPAD_DOWN;
    return 0;
}

static int same_bind(Bind a, Bind b) {
    return a.kind == b.kind && a.code == b.code;
}

static int same_list(const Bind *a, const Bind *b) {
    int i;
    for (i = 0; i < MAX_BINDS; i++)
        if (a[i].kind != b[i].kind || (a[i].kind != BIND_NONE && a[i].code != b[i].code))
            return 0;
    return 1;
}

static int count_binds(const Bind *b) {
    int n = 0;
    while (n < MAX_BINDS && b[n].kind != BIND_NONE)
        n++;
    return n;
}

/* the list as text: Left and Right Shift (Ctrl, Alt) as Shift */
static void binds_text(const Bind *b, int style, char *out, size_t size) {
    int i, j, n = count_binds(b), used[MAX_BINDS] = {0};
    size_t len = 0;
    out[0] = 0;
    if (!n) {
        snprintf(out, size, "none");
        return;
    }
    for (i = 0; i < n; i++) {
        const char *name = NULL;
        if (used[i])
            continue;
        if (b[i].kind == BIND_KEY && (b[i].code == SDL_SCANCODE_LSHIFT || b[i].code == SDL_SCANCODE_LCTRL ||
                                      b[i].code == SDL_SCANCODE_LALT))
            for (j = 0; j < n; j++)
                if (b[j].kind == BIND_KEY && b[j].code == b[i].code + 4) {
                    used[j] = 1;
                    name = b[i].code == SDL_SCANCODE_LSHIFT ? "Shift" : b[i].code == SDL_SCANCODE_LCTRL ? "Ctrl" : "Alt";
                }
        if (!name)
            name = port_bind_name(b[i], style);
        len += snprintf(out + len, len < size ? size - len : 0, "%s%s", len ? (style == 2 ? " / " : ", ") : "", name);
    }
}

void port_control_text(int c, int style, char *out, size_t n) {
    binds_text(port_binds[c], style, out, n);
}

/* ---- twine.ini ---- */

static void where(const char *key, char *out, size_t n) {
    int line = port_setting_line(key);
    if (line)
        snprintf(out, n, "twine.ini line %d", line);
    else {
        size_t i, k = (size_t)snprintf(out, n, "TWINE_");
        for (i = 0; key[i] && k < n - 1; i++)
            out[k++] = (char)toupper((unsigned char)key[i]);
        out[k] = 0;
    }
}

/* an action's value: its binds (the default's when nothing in it is known), problems logged */
static void read_control(int c) {
    char env[64] = "TWINE_", value[256], at[64], *item, *save;
    const char *v;
    Bind got[MAX_BINDS];
    int n = 0, bad = 0, none = 0;
    size_t i, k = 6;
    for (i = 0; info[c].key[i] && k < sizeof(env) - 1; i++)
        env[k++] = (char)toupper((unsigned char)info[c].key[i]);
    env[k] = 0;
    memcpy(port_binds[c], info[c].dflt, sizeof(port_binds[c]));
    if (!(v = getenv(env)))
        return;
    where(info[c].key, at, sizeof(at));
    snprintf(value, sizeof(value), "%s", v);
    memset(got, 0, sizeof(got));
    for (item = value; item; item = save) {
        Bind b[2];
        char *e;
        int m, j;
        if ((save = strchr(item, ',')))
            *save++ = 0;
        while (*item == ' ' || *item == '\t')
            item++;
        for (e = item + strlen(item); e > item && (e[-1] == ' ' || e[-1] == '\t'); e--)
            ;
        *e = 0;
        if (!*item)
            continue;
        if (!strcasecmp(item, "none")) {
            none = 1;
            continue;
        }
        m = port_bind_parse(item, c >= CTL_FIRST_PAD, b, 2);
        if (!m) {
            Bind other;
            int wrong = port_bind_parse(item, c < CTL_FIRST_PAD, &other, 1); /* (a pad's name for a key, or ...) */
            port_log("controls: %s: %s = %s: %s \"%s\" (left out)\n", at, info[c].key, v,
                     wrong ? (c >= CTL_FIRST_PAD ? "a key or mouse button, not the gamepad's:"
                                                  : "the gamepad's, not a key or mouse button:")
                           : (c >= CTL_FIRST_PAD ? "no gamepad button called" : "no key, mouse button or wheel called"),
                     item);
            bad++;
            continue;
        }
        for (j = 0; j < m; j++) {
            int seen = 0, x;
            for (x = 0; x < n; x++)
                seen |= same_bind(got[x], b[j]);
            if (seen)
                continue;
            if (n == MAX_BINDS) {
                port_log("controls: %s: %s = %s: more than %d (\"%s\" left out)\n", at, info[c].key, v, MAX_BINDS, item);
                bad++;
                break;
            }
            got[n++] = b[j];
        }
    }
    if (!n && !none) {
        char d[128];
        binds_text(info[c].dflt, 0, d, sizeof(d));
        port_log("controls: %s: %s = %s: nothing known, so its default: %s\n", at, info[c].key, v, d);
        return;
    }
    memcpy(port_binds[c], got, sizeof(got));
    {
        char t[192];
        binds_text(port_binds[c], 0, t, sizeof(t));
        port_log("controls: %s = %s (%s)%s\n", info[c].key, t, at, bad ? ", the rest left out" : "");
    }
}

static void unknown_key(const char *key, int line, int controls) {
    if (controls && !port_control_known(key))
        port_log("controls: twine.ini line %d: \"%s\" is no action (the actions: fire, aim, use, reload, punch, "
                 "grenade, crouch, sprint, prev_weapon, next_weapon, last_weapon, forward, back, strafe_left, "
                 "strafe_right, turn_left, turn_right, look_up, look_down, messages, and pad_ before each but the "
                 "moving and looking: pad_fire, ...)\n", line, key);
}

/* the [controls] block twine.ini is given (commented: the defaults, by the names this keyboard gives them) */
static void add_block(void) {
    static char block[6000];
    size_t n = 0;
    int c;
#define ADD(...) (n += (size_t)snprintf(block + n, n < sizeof(block) ? sizeof(block) - n : 0, __VA_ARGS__))
    ADD("[controls]\n"
        "# Keys and buttons. Take the # off a line to change it (a line left with # keeps the default it shows).\n"
        "# Several keys for an action: separated by commas. none: no key. The game's OPTIONS > PORT SETTINGS >\n"
        "# controls page changes them too, and F1 in the game lists them.\n"
        "#\n"
        "# Keyboard and mouse: keys by the names your keyboard layout gives them (W, Left Shift, Space, Keypad 5,\n"
        "# Comma, ...; Shift, Ctrl and Alt are both of them), Mouse Left, Mouse Right, Mouse Middle, Mouse 4,\n"
        "# Mouse 5, Wheel Up, Wheel Down. The mouse's buttons count in the missions only. Always as they are: the\n"
        "# arrows, Enter, Esc, and Space and Backspace in the menus, 1-9, F1-F11.\n");
    for (c = 0; c < CTL_FIRST_PAD; c++) {
        char t[192];
        binds_text(info[c].dflt, 0, t, sizeof(t));
        ADD("# %s = %s\n", info[c].key, t);
    }
    ADD("#\n"
        "# Gamepad, in the modern pad layout (controls = original keeps the PS1 pad's own buttons): buttons by\n"
        "# their place, as SDL names them: South, East, West, North (PlayStation: Cross, Circle, Square, Triangle;\n"
        "# Xbox: A, B, X, Y), Left Shoulder, Right Shoulder, Left Trigger, Right Trigger, Left Stick, Right Stick\n"
        "# (pressed in), DPad Left, DPad Right, Misc 1, Left Paddle 1, ... (L1, R1, L2, R2, L3, R3 and LB, RB, LT, RT,\n"
        "# LS, RS too). Always as they are: the sticks, Start, Back, the touchpad, South and the d-pad's up and down.\n");
    for (c = CTL_FIRST_PAD; c < CTL_COUNT; c++) {
        char t[192];
        binds_text(info[c].dflt, 0, t, sizeof(t));
        ADD("# %s = %s\n", info[c].key, t);
    }
#undef ADD
    port_settings_add_block(block);
}

void port_controls_init(void) {
    int c, changed = 0;
    for (c = 0; c < CTL_COUNT; c++) {
        read_control(c);
        changed += !same_list(port_binds[c], info[c].dflt);
    }
    port_settings_each(unknown_key);
    port_log("controls: %d of %d actions changed from their defaults\n", changed, CTL_COUNT);
    if (!getenv("TWINE_NO_CONTROLS_BLOCK"))
        add_block();
}

/* ---- the CONTROLS page's changes ---- */

static void save_control(int c) {
    char t[192];
    binds_text(port_binds[c], 0, t, sizeof(t));
    if (same_list(port_binds[c], info[c].dflt)) {
        char d[192];
        binds_text(info[c].dflt, 0, d, sizeof(d));
        port_setting_unset(info[c].key, d);
    } else
        port_setting_save(info[c].key, t);
    port_log("controls: %s = %s (the CONTROLS page)\n", info[c].key, t);
}

void port_control_set(int c, Bind b, int add) {
    int o, i, n = count_binds(port_binds[c]);
    for (o = c < CTL_FIRST_PAD ? 0 : CTL_FIRST_PAD; o < (c < CTL_FIRST_PAD ? CTL_FIRST_PAD : CTL_COUNT); o++) {
        int had = 0;
        if (o == c)
            continue;
        for (i = 0; i < MAX_BINDS; i++)
            if (same_bind(port_binds[o][i], b)) {
                memmove(&port_binds[o][i], &port_binds[o][i + 1], sizeof(Bind) * (MAX_BINDS - 1 - i));
                port_binds[o][MAX_BINDS - 1].kind = BIND_NONE;
                port_binds[o][MAX_BINDS - 1].code = 0;
                had = 1;
                i--;
            }
        if (had)
            save_control(o);
    }
    if (add) {
        for (i = 0; i < n; i++)
            if (same_bind(port_binds[c][i], b))
                return;
        if (n == MAX_BINDS)
            n--;
        port_binds[c][n] = b;
    } else {
        memset(port_binds[c], 0, sizeof(port_binds[c]));
        port_binds[c][0] = b;
    }
    save_control(c);
}

void port_control_default(int c) {
    memcpy(port_binds[c], info[c].dflt, sizeof(port_binds[c]));
    save_control(c);
}

void port_controls_reset(void) {
    int c;
    for (c = 0; c < CTL_COUNT; c++)
        if (!same_list(port_binds[c], info[c].dflt))
            port_control_default(c);
    port_log("controls: all back to their defaults\n");
}

int port_control_count(void) {
    return CTL_COUNT;
}

int port_control_first_pad(void) {
    return CTL_FIRST_PAD;
}
