/* The front end (SHELL.EXE) on the PC: what the native build changes in its code (port/mods.py builds this into
 * the shell's module). Names are the shell's own.
 *
 * MOD_REPLACES_NATIVE: func_8006CDA8 func_8002ABF0 func_80026998 func_8002A400 func_80029B1C func_800293C0 func_8003277C
 */
#include <stdlib.h>
#include <string.h>
#include "common.h"

u32 func_8006CDA8_orig(u32 **addr, u32 **header);
void func_8002ABF0_orig(void);

s32 func_8004977C(s32 code);
void func_80049938(s32 code);
void port_spin(u32 ns);

/* libcd's StGetNext: the next whole movie frame in the stream ring, or 1 if none has come in yet. The movie
 * players wait for a frame by calling it a fixed number of times (2000 x 2000 for the first frame), some
 * five seconds on the console but a few milliseconds here, so a call that finds nothing takes as long as
 * such a pass of their loops does on the console. */
u32 func_8006CDA8(u32 **addr, u32 **header) {
    u32 r = func_8006CDA8_orig(addr, header);
    if (r)
        port_spin(1300);
    return r;
}

/* The front end's own developer codes, which nothing calls in the retail game (the loop at 0x800496F8, left
 * after the end of func_800495B0): each frame, every code that has just been completed takes effect. With the
 * devcheats setting they work again. All start with SELECT SELECT CIRCLE CIRCLE, then:
 *   L2 L2 R2 R2                 the sprite position editor (D_80073E28; it prints positions)
 *   L1 L1 L1 L1 L1 L1 L1        every movie in the Movies menu (D_80073E2C)
 *   L1 L1 R1 R1                 any mission can be chosen (D_80073E30)
 *   SELECT R1 L2, five times    D_80073E34, which skips ahead in the front end
 * func_8002ABF0 is the front end's frame (drawing, then waiting for the vblank's buffer swap). */
void func_8002ABF0(void) {
    s32 i;
    if (port_dev_cheats)
        for (i = 0; i < 4; i++)
            if (func_8004977C(i))
                func_80049938(i);
    func_8002ABF0_orig();
}


void func_80026998_orig(s32 movie);
int port_hd_movie(const char *name);
#define D_80070E4C ((char *)0x80070E4Cu) /* the movies' names, 16 bytes each */

/* Plays movie number `movie`, as the movie player's func_80026C2C does (fixes/movie.c): the film's own footage
 * instead when the user has a clip of it (hdmovie.c). */
void func_80026998(s32 movie) {
    if (!port_hd_movie(D_80070E4C + (s16)movie * 16))
        func_80026998_orig(movie);
}

void func_8002A400_orig(void);
s32 func_80043CAC(void);
void func_8004DABC(void);
s32 card_open(const char *name, u32 mode);
s32 card_lseek(s32 fd, s32 off, s32 whence);
s32 card_rw(s32 fd, u8 *buf, s32 len, int write);
s32 card_close(s32 fd);
void port_log(const char *fmt, ...);
#define G_ARGS (*(u8 **)0x80161040u)
#define SAVE_DATA ((u8 *)0x80153880u) /* what the load unpacks (func_80043CAC) */

/* The autoload setting (TWINE_AUTOLOAD: 1, the default, or 2, a card slot; off): the game's save on that card,
 * loaded after the title screen as the memory card menu's LOAD would (func_80042B94, there over libmcrd frame by
 * frame): the save's file, BASLUS-01272 (one a card), the second 0x200 bytes of its first 0x400 unpacked into
 * g_Args (func_80043CAC: the missions open, the settings, ...), the difficulty's flag, and the missions counted
 * again (func_8004DABC). A card without the save, or a save that won't unpack, leaves the game as it was. */
/* the setting: 0 slot 1, 1 slot 2, -1 off */
static int autoload_slot(void) {
    const char *s = getenv("TWINE_AUTOLOAD");
    return !s ? 0 : !strcmp(s, "1") ? 0 : !strcmp(s, "2") ? 1 : -1;
}

static void autoload(void) {
    int slot = autoload_slot();
    u8 *a = G_ARGS, kept[0xDB * 4];
    s32 fd, n;
    if (slot < 0 || !a || (fd = card_open(slot ? "bu10:BASLUS-01272" : "bu00:BASLUS-01272", 1)) < 0)
        return;
    card_lseek(fd, 0x200, 0);
    n = card_rw(fd, SAVE_DATA, 0x200, 0);
    card_close(fd);
    memcpy(kept, a, sizeof(kept));
    if (n != 0x200 || SAVE_DATA[0] != 4 || !func_80043CAC() || (a[0x12A] != 1 && a[0x12A] != 2)) {
        memcpy(a, kept, sizeof(kept));
        port_log("autoload: no save on the card in slot %d\n", slot + 1);
        return;
    }
    if (a[0x12A] == 1)
        *(u32 *)(a + 4) |= 0x10;
    else
        *(u32 *)(a + 4) &= ~0x10u;
    *(u32 *)(a + 4) |= 0x40;
    func_8004DABC();
    port_log("autoload: loaded the save on the card in slot %d\n", slot + 1);
}

/* The title screen; after it, the main menu */
void func_8002A400(void) {
    func_8002A400_orig();
    autoload();
}

/* PORT SETTINGS on the OPTIONS screen: a fourth item under CONTROLLER, MEMORY CARD and CHEATS, which Cross turns
 * into a page of the port's settings (Triangle goes back): Cross or right steps one on, left back, each kept in
 * twine.ini at once (port_setting_save, main.c) and used from then on.
 *
 * The screen (func_800293C0) makes its items as text (func_8005133C: left edge, line; each item's glyphs in a
 * table of 20, 0x60 bytes each, glyphs from a pool whose next free one is D_800DFEEC) and draws them in two
 * palettes (func_80051768: the item, the palette's place; D_80098C12/14 normal, D_80153E08/0A chosen). The
 * cursor (D_801628D8; D_80150C3C the one before) goes over D_80154228 items, set from the count made
 * (D_801510B4). The menus' step (func_8003277C) moves it and says 2 for Cross, 3 for Triangle (which leave the
 * screen); D_80151A1C has this frame's buttons, D_800DFDE0 the last's. A page is made by making the items
 * again over the first one's glyphs. */
s32 func_8005133C(char *text, s32 x, s32 y);
void func_80051768(s32 item, s32 cx, s32 cy);
void func_80029B1C_orig(void);
s32 func_800293C0_orig(void);
s32 func_8003277C_orig(void);
void port_setting_save(const char *key, const char *value);
#define CURSOR (*(s32 *)0x801628D8u)
#define CURSOR_WAS (*(s32 *)0x80150C3Cu)
#define CURSOR_END (*(s32 *)0x80154228u)
#define ITEMS (*(s32 *)0x801510B4u)
#define NEXT_GLYPH (*(s32 *)0x800DFEECu)
#define ITEM_X (*(s32 *)0x800DFDF4u)
#define FIRST_GLYPH (*(u16 *)0x800DF0B0u) /* item 0's */
static int in_options, on_page;
static char item_text[16][32];

static int env_on(const char *name, int dflt) {
    const char *s = getenv(name);
    return s && *s ? atoi(s) != 0 : dflt;
}

static int mouse_steps(void) { /* mouse speed in quarters, 1-12 */
    const char *s = getenv("TWINE_MOUSE_SENS");
    int n = s && atof(s) > 0 ? (int)(atof(s) * 4 + 0.5) : 4;
    return n < 1 ? 1 : n > 12 ? 12 : n;
}

#define SETTINGS 8

/* the page's line i, as its label and value, and stepped by d (1 on, -1 back) when d */
static void setting(int i, int d, char *label, char *value) {
    static const char *const onoff[] = {"off", "on"};
    char v[8];
    int on;
    switch (i) {
    case 0: {
        int n = mouse_steps() + d;
        n = n < 1 ? 12 : n > 12 ? 1 : n;
        if (d) {
            snprintf(v, sizeof(v), "%g", n / 4.0);
            port_setting_save("mouse_sens", v);
        }
        strcpy(label, "mouse speed");
        sprintf(value, "%d", n);
        return;
    }
    case 1:
        on = env_on("TWINE_MOUSE_INVERT", 0) ^ (d != 0);
        if (d)
            port_setting_save("mouse_invert", on ? "1" : "0");
        strcpy(label, "invert mouse");
        strcpy(value, onoff[on]);
        return;
    case 2:
        on = !(getenv("TWINE_CONTROLS") && !strcasecmp(getenv("TWINE_CONTROLS"), "original")) ^ (d != 0);
        if (d)
            port_setting_save("controls", on ? "modern" : "original");
        strcpy(label, "pad layout");
        strcpy(value, on ? "modern" : "classic");
        return;
    case 3:
        on = env_on("TWINE_SMOOTH", 1) ^ (d != 0);
        if (d)
            port_setting_save("smooth", on ? "1" : "0");
        strcpy(label, "smooth motion");
        strcpy(value, onoff[on]);
        return;
    case 4:
        on = env_on("TWINE_HD", 1) ^ (d != 0);
        if (d)
            port_setting_save("hd", on ? "1" : "0");
        strcpy(label, "hd movies");
        strcpy(value, onoff[on]);
        return;
    case 5:
        on = env_on("TWINE_RUMBLE", 1) ^ (d != 0);
        if (d)
            port_setting_save("rumble", on ? "1" : "0");
        strcpy(label, "vibration");
        strcpy(value, onoff[on]);
        return;
    case 6:
        on = env_on("TWINE_INFINITE_GRENADES", 0) ^ (d != 0);
        if (d)
            port_setting_save("infinite_grenades", on ? "1" : "0");
        strcpy(label, "infinite grenades");
        strcpy(value, onoff[on]);
        return;
    default: {
        static const char *const values[] = {"1", "2", "off"};
        int s = autoload_slot();
        s = s < 0 ? 2 : s;
        if (d) {
            s = (s + (d > 0 ? 1 : 2)) % 3;
            port_setting_save("autoload", values[s]);
        }
        strcpy(label, "autoload");
        strcpy(value, s == 2 ? "off" : s ? "slot 2" : "slot 1");
        return;
    }
    }
}

/* the items coloured: the chosen one (on the page, its line's label and value) in the chosen palette */
static void colour_items(int n, int at) {
    int i;
    for (i = 0; i < n; i++)
        if (on_page ? i / 2 == at / 2 : i == at)
            func_80051768(i, *(s16 *)0x80153E08u, *(s16 *)0x80153E0Au);
        else
            func_80051768(i, *(s16 *)0x80098C12u, *(s16 *)0x80098C14u);
}

/* The page as the CHEATS screen lays its lines out (func_80046DC4): the text left in lower case (the flag
 * D_80073F05, which func_8005133C otherwise capitalizes with), which draws as the small capitals; each line a
 * label at x 0x94 and its value 0x140 further on, 0x10 apart from y 0x45; the cursor on the labels (items 2i).
 * page = 0: the OPTIONS screen's own items again and PORT SETTINGS. `at` is the cursor's item. */
static void make_items(int page, int at) {
    static char *const own[] = {(char *)0x8008E0FCu, (char *)0x8008E108u, (char *)0x8008E114u};
    int i, n = page ? SETTINGS : 4;
    ITEMS = 0;
    NEXT_GLYPH = FIRST_GLYPH;
    on_page = page;
    if (page) {
        *(u8 *)0x80073F05u = 1;
        for (i = 0; i < n; i++) {
            setting(i, 0, item_text[2 * i], item_text[2 * i + 1]);
            func_8005133C(item_text[2 * i], 0x94, 0x45 + i * 0x10);
            func_8005133C(item_text[2 * i + 1], 0x94 + 0x140, 0x45 + i * 0x10);
        }
        *(u8 *)0x80073F05u = 0;
        n *= 2;
    } else
        for (i = 0; i < n; i++) {
            strcpy(item_text[i], i < 3 ? own[i] : "PORT SETTINGS");
            func_8005133C(item_text[i], ITEM_X, 0x46 + i * 0x1C);
        }
    colour_items(n, at);
    CURSOR_END = n;
    CURSOR = CURSOR_WAS = at;
}

/* The OPTIONS screen's items: PORT SETTINGS after the others */
void func_80029B1C(void) {
    func_80029B1C_orig();
    strcpy(item_text[3], "PORT SETTINGS");
    func_8005133C(item_text[3], ITEM_X, 0x9A);
    func_80051768(3, *(s16 *)0x80098C12u, *(s16 *)0x80098C14u);
}

s32 func_800293C0(void) {
    s32 r;
    in_options = 1;
    on_page = 0;
    r = func_800293C0_orig();
    in_options = on_page = 0;
    return r;
}

/* The menus' step, on the OPTIONS screen: Cross on PORT SETTINGS opens the page; on the page the cursor goes a
 * line (two items) at a time, Cross, left and right change the line chosen and Triangle goes back, none of them
 * leaving the screen */
s32 func_8003277C(void) {
    s32 at = CURSOR, r = func_8003277C_orig(), d = 0, c;
    u32 now = *(u32 *)0x80151A1Cu, was = *(u32 *)0x800DFDE0u;
    if (!in_options)
        return r;
    if (!on_page) {
        if (CURSOR == 3 && r == 2) {
            make_items(1, 0);
            return 1;
        }
        return r;
    }
    if (r == 3) {
        make_items(0, 3);
        return 1;
    }
    c = CURSOR;
    if (c != at) { /* moved one item: on to the next line's label (wrapping) */
        int n = SETTINGS * 2, up = (c == at - 1) || (at == 0 && c == n - 1);
        c = up ? ((at / 2 + SETTINGS - 1) % SETTINGS) * 2 : ((at / 2 + 1) % SETTINGS) * 2;
        CURSOR = CURSOR_WAS = c;
        colour_items(n, c);
        return r == 2 ? 1 : r;
    }
    if (r == 2 || ((now & 0x2000) && !(was & 0x2000)))
        d = 1;
    else if ((now & 0x8000) && !(was & 0x8000))
        d = -1;
    if (d) {
        char l[32], v[32];
        setting(at / 2, d, l, v);
        make_items(1, at);
    }
    return r == 2 ? 1 : r;
}
