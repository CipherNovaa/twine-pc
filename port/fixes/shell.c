/* The front end (SHELL.EXE) on the PC: what the native build changes in its code (port/mods.py builds this into
 * the shell's module). Names are the shell's own.
 *
 * MOD_REPLACES_NATIVE: func_8006F30C func_8006F0B4 func_80042450 func_80042B94 func_8006CDA8 func_8002ABF0 func_80026998 func_8002A400 func_80029B1C func_800293C0 func_8003277C func_8003A7A8 func_80051768 func_80028644 func_8005133C func_8003C14C func_80033818 func_80051824 func_800519D8 func_800439E0 func_800443D8 func_8003EA00 func_800385C4
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
static void shell_keys(void);

void func_8002ABF0(void) {
    s32 i;
    shell_keys();
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
 * instead when the user has a clip of it (hdmovie.c). Not as endless mode starts: its rooms are no mission's. */
extern int port_endless;

void func_80026998(s32 movie) {
    if (port_endless) /* (endless mode: no mission's film before its rooms) */
        return;
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
static void endless_card_read(int slot, const char *why);
/* the setting: 0 slot 1, 1 slot 2, -1 off */
static int autoload_slot(void) {
    const char *s = getenv("TWINE_AUTOLOAD");
    return !s ? 0 : !strcmp(s, "1") ? 0 : !strcmp(s, "2") ? 1 : -1;
}

static void autoload(void) {
    int slot = autoload_slot();
    u8 *a = G_ARGS, kept[0xDB * 4];
    s32 fd, n;
    if (slot >= 0)
        endless_card_read(slot, "autoload");
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

/* Endless mode's high scores on the memory card: a save of their own beside the campaign's, BASLUS-01272ENDLESS
 * (one block: the title "TWINE ENDLESS" and the campaign save's icon in its header, the table after it:
 * port/src/endless.c packs it), through the BIOS's file functions as the game's own saves go. It is written to a
 * card whenever the game saves to it (its save screen, func_80042450, writing the campaign's file with libmcrd's
 * MemCardWriteFile, func_8006F30C) and after a ranked run (to the card autoload reads, else slot 1), and read
 * whenever the game reads a card (autoload; the load screen, func_80042B94, reading with MemCardReadFile,
 * func_8006F0B4). The campaign's save is never touched: a card that hasn't it keeps a block free for it. */
s32 card_firstfile(const char *pattern, u32 dir);
s32 card_nextfile(u32 dir);
int port_endless_table_pack(u8 *out, int len);
int port_endless_table_unpack(const u8 *in, int len);
void port_endless_table_import(void);
#define ENDLESS_SAVE "BASLUS-01272ENDLESS"
#define CAMPAIGN_SAVE "BASLUS-01272"

static void card_name(int slot, const char *file, char *out) {
    sprintf(out, "bu%d0:%s", slot, file);
}

/* the blocks in use on a card (-1: no card) */
static int card_used(int slot) {
    static u8 dir[40];
    char pat[16];
    int n = 0;
    u32 d;
    card_name(slot, "*", pat);
    for (d = card_firstfile(pat, (u32)dir); d; d = card_nextfile((u32)dir))
        n += (*(s32 *)(dir + 0x18) + 0x1FFF) / 0x2000;
    return n;
}

static int card_has(int slot, const char *file) {
    char name[32];
    s32 fd;
    card_name(slot, file, name);
    if ((fd = card_open(name, 1)) < 0)
        return 0;
    card_close(fd);
    return 1;
}

/* the table written to the card in slot (0/1): 1 done */
static int endless_card_write(int slot, const char *why) {
    static u8 block[0x2000];
    static const char title[] = "TWINE ENDLESS";
    char name[32];
    s32 fd;
    int i, at;
    memset(block, 0, sizeof(block));
    card_name(slot, CAMPAIGN_SAVE, name);
    if ((fd = card_open(name, 1)) >= 0) { /* (the campaign save's header: its icon's palette and picture) */
        card_rw(fd, block, 0x100, 0);
        card_close(fd);
    } else /* (none: a plain icon, white on the palette's colour 1) */
    {
        memset(block + 0x80, 0x11, 0x80);
        *(u16 *)(block + 0x62) = 0x7FFF;
    }
    block[0] = 'S', block[1] = 'C', block[2] = 0x11, block[3] = 1;
    memset(block + 4, 0, 0x5C);
    for (i = 0, at = 4; title[i]; i++, at += 2) { /* (the title in Shift-JIS, as card managers show it) */
        u16 c = title[i] == ' ' ? 0x8140 : title[i] >= 'A' && title[i] <= 'Z' ? 0x8260 + title[i] - 'A' : 0x8140;
        block[at] = (u8)(c >> 8), block[at + 1] = (u8)c;
    }
    if (!port_endless_table_pack(block + 0x200, sizeof(block) - 0x200))
        return 0;
    card_name(slot, ENDLESS_SAVE, name);
    if ((fd = card_open(name, 2)) < 0) {
        int used = card_used(slot);
        if (used < 0 || used + 1 + !card_has(slot, CAMPAIGN_SAVE) > 15) {
            port_endless_logf(-1, "scores: no room on the card in slot %d (%d blocks used%s): kept in endless.txt only",
                              slot + 1, used, card_has(slot, CAMPAIGN_SAVE) ? "" : ", one kept for the game's save");
            return 0;
        }
        if ((fd = card_open(name, 0x200 | 1 << 16)) < 0) {
            port_endless_logf(-1, "scores: no card in slot %d (or it can't take the save): kept in endless.txt only",
                              slot + 1);
            return 0;
        }
    }
    i = card_rw(fd, block, sizeof(block), 1);
    card_close(fd);
    port_endless_logf(-1, "scores: the table written to the card in slot %d (%s)%s", slot + 1, why,
                      i == (int)sizeof(block) ? "" : ": the write failed");
    return i == (int)sizeof(block);
}

/* the table read from the card in slot: its own, or endless.txt's for a card without it */
static void endless_card_read(int slot, const char *why) {
    static u8 block[0x2000];
    char name[32];
    s32 fd;
    int n;
    card_name(slot, ENDLESS_SAVE, name);
    if ((fd = card_open(name, 1)) < 0) {
        port_endless_table_import();
        return;
    }
    n = card_rw(fd, block, sizeof(block), 0);
    card_close(fd);
    if (n != (int)sizeof(block) || block[0] != 'S' || block[1] != 'C' ||
        !port_endless_table_unpack(block + 0x200, sizeof(block) - 0x200)) {
        port_endless_logf(-1, "scores: the card in slot %d has a damaged table (%s): left as it was", slot + 1, why);
        return;
    }
    port_endless_logf(-1, "scores: read from the card in slot %d (%s)", slot + 1, why);
}

/* libmcrd's MemCardWriteFile and MemCardReadFile: which card the campaign's save went to or came from */
static s32 saved_chan = -1, loaded_chan = -1;
s32 func_8006F30C_orig(s32 chan, char *file, u32 *buf, s32 ofs, s32 bytes);
s32 func_8006F30C(s32 chan, char *file, u32 *buf, s32 ofs, s32 bytes) {
    s32 r;
    if (file && !strcmp(file, CAMPAIGN_SAVE))
        saved_chan = chan;
    r = func_8006F30C_orig(chan, file, buf, ofs, bytes);
    port_endless_logf(-1, "card: the game writes %s to channel %X (%d bytes): %d", file ? file : "?", chan, bytes, r);
    return r;
}

s32 func_8006F0B4_orig(s32 chan, char *file, u32 *buf, s32 ofs, s32 bytes);
s32 func_8006F0B4(s32 chan, char *file, u32 *buf, s32 ofs, s32 bytes) {
    if (file && !strcmp(file, CAMPAIGN_SAVE))
        loaded_chan = chan;
    return func_8006F0B4_orig(chan, file, buf, ofs, bytes);
}

/* the save screen and the load screen: the table with the campaign */
s32 func_80042450_orig(s32 arg);
s32 func_80042450(s32 arg) { /* (arg: the screen's mode, as its callers give it) */
    s32 r;
    saved_chan = -1;
    r = func_80042450_orig(arg);
    port_endless_logf(-1, "card: the save screen left (channel %d)", saved_chan);
    if (saved_chan >= 0 && card_has(saved_chan >> 4 & 1, CAMPAIGN_SAVE))
        endless_card_write(saved_chan >> 4 & 1, "the game saved");
    return r;
}

s32 func_80042B94_orig(void);
s32 func_80042B94(void) {
    s32 r;
    loaded_chan = -1;
    r = func_80042B94_orig();
    if (loaded_chan >= 0)
        endless_card_read(loaded_chan >> 4 & 1, "the game loaded");
    return r;
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
s32 func_8005133C_orig(char *text, s32 x, s32 y);
void func_80051768(s32 item, s32 cx, s32 cy);
void func_80051768_orig(s32 item, s32 cx, s32 cy);
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
static int in_options, on_page, in_main, main_made;
static char item_text[20][32];

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
    default: /* F9 opens the developers' debug menu in a mission (mods/savestate.c) */
        on = env_on("TWINE_DEBUG_MENU", 0) ^ (d != 0);
        if (d)
            port_setting_save("debug_menu", on ? "1" : "0");
        strcpy(label, "debug menu");
        strcpy(value, onoff[on]);
        return;
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
/* AUTOLOAD on the OPTIONS screen, a fifth item under PORT SETTINGS: Cross, left or right steps it */
static char *autoload_name(void) {
    static char *const names[] = {"AUTOLOAD SLOT 1", "AUTOLOAD SLOT 2", "AUTOLOAD OFF"};
    int v = autoload_slot();
    return names[v < 0 ? 2 : v];
}

static void make_items(int page, int at) {
    static char *const own[] = {(char *)0x8008E0FCu, (char *)0x8008E108u, (char *)0x8008E114u};
    int i, n = page ? SETTINGS : 5;
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
            strcpy(item_text[i], i < 3 ? own[i] : i == 3 ? "PORT SETTINGS" : autoload_name());
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
    strcpy(item_text[4], autoload_name());
    func_8005133C(item_text[4], ITEM_X, 0x9A + 0x1C);
    func_80051768(4, *(s16 *)0x80098C12u, *(s16 *)0x80098C14u);
}

/* BLACKJACK on the main menu, after MOVIES: the casino mission (folder 11, mission 3) started at its blackjack
 * table (port_blackjack: port/fixes/level11.c seats Bond). The main menu (func_8003A7A8) makes its four items
 * (colouring each as it goes: func_80051768) and then slides its panel in, so the item is made as MOVIES is
 * coloured; choosing it (in the menu step) leaves the front end
 * at once (port_exe_leave) with g_Args naming the mission, and the boot stub starts it. */
s32 func_8003A7A8_orig(void);
void port_exe_leave(void);
void port_endless_logf(s32 frame, const char *fmt, ...);
extern int port_blackjack;
static char blackjack_text[] = "BLACKJACK";

s32 func_8003A7A8(void) {
    s32 r;
    port_endless_logf(-1, "shell: main menu");
    port_blackjack = 0;
    in_main = 1;
    main_made = 0;
    r = func_8003A7A8_orig();
    in_main = 0;
    return r;
}

void func_80051768(s32 item, s32 cx, s32 cy) {
    func_80051768_orig(item, cx, cy);
    if (in_main && !main_made && item == 3) {
        main_made = 1;
        func_8005133C(blackjack_text, ITEM_X, *(s32 *)0x800DFE20u + 4 * *(s32 *)0x80174F68u);
        func_80051768_orig(4, *(s16 *)0x80098C12u, *(s16 *)0x80098C14u);
        CURSOR_END = 5;
    }
}

static void main_step(s32 r) {
    u8 *a = G_ARGS;
    if (CURSOR == 4 && r == 2 && a) {
        *(s16 *)(a + 0x188) = 11;
        *(s16 *)(a + 0x18A) = 3;
        port_blackjack = 1;
        port_log("blackjack: the casino mission at its table\n");
        port_exe_leave();
    }
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
    if (in_main && !in_options) {
        main_step(r);
        return r;
    }
    if (!in_options)
        return r;
    if (!on_page) {
        if (CURSOR == 3 && r == 2) {
            make_items(1, 0);
            return 1;
        }
        if (CURSOR == 4 && (r == 2 || ((now & 0xA000) & ~was))) {
            static const char *const values[] = {"1", "2", "off"};
            int v = autoload_slot();
            v = ((v < 0 ? 2 : v) + ((now & 0x8000) & ~was ? 2 : 1)) % 3;
            port_setting_save("autoload", values[v]);
            make_items(0, 4);
            return r == 2 ? 1 : r;
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

/* Endless mode's end (port/src/endless.c): the run's stats on the mission stats screen, as a mission completed's
 * (func_80028644, after a mission: its debriefing, the stats (func_800350F4) and the hi-score table). The
 * campaign is left as it was: its progress and hi-scores in g_Args are put back afterwards (what the screen makes
 * of a mission completed: the mission done, the next one open, the table's new entry). Meanwhile the table of the
 * mission named (the last room's) holds endless mode's best three, and its new entry is kept (port_endless_keep).
 * A run made unranked (a save state loaded) goes in as a cheated mission's does: the cheat flag 9 set, which keeps
 * it off the table.
 *
 * The stats' lines (made as text: func_8005133C) say what the run did: rooms cleared and waves in place of
 * efficiency and health, the enemies eliminated in full, and the score endless mode gives instead of the
 * mission's (D_80160FE8, which the table then takes). */
extern int port_endless_stats, port_endless_unranked;
extern char port_endless_unranked_why[32];
typedef struct {
    int rooms, kills, headshots, shots, hits, time, score, unranked;
} EndlessRunStats; /* (port/src/endless.h's EndlessRun) */
extern EndlessRunStats port_endless_run;
void port_endless_best(char names[3][4], u32 scores[3]);
void port_endless_keep(const char *name);
void func_80028644_orig(void);
#define MISSION_SCORE (*(s32 *)0x80160FE8u)
static int in_endless_stats, after_stats; /* (after_stats: frames of the front end to log after a run's stats) */

s32 func_8005133C(char *text, s32 x, s32 y) {
    static char line[48];
    EndlessRunStats *r = &port_endless_run;
    if (in_endless_stats && text) {
        if (getenv("TWINE_ENDLESS_DUMP"))
            port_endless_logf(-1, "stats text \"%s\" at %d %d (unranked %d)", text, x, y, port_endless_unranked);
        if (y == 56 && x > 89 && *text >= 'A' && *text <= 'Z') { /* the mission's name, centred over the lines */
            x += ((s32)strlen(text) - 7) * 15 / 4;
            strcpy(text = line, "ENDLESS"); /* (in a buffer: the text is written over, capitalized) */
        } else if (!strncmp(text, "enemies eliminated", 18))
            snprintf(text = line, sizeof(line), "enemies eliminated...%d", r->kills);
        else if (!strncmp(text, "efficiency", 10))
            snprintf(text = line, sizeof(line), "rooms cleared.%d", r->rooms);
        else if (!strncmp(text, "health", 6))
            snprintf(text = line, sizeof(line), "waves.........%d", r->rooms + 1);
        else if (port_endless_unranked && strstr(text, "hi-score")) { /* (why it takes no place in the table) */
            s32 was = (s32)strlen(text);
            snprintf(text = line, sizeof(line), "unranked: %s", port_endless_unranked_why);
            x -= ((s32)strlen(line) - was) * 15 / 4; /* (centred as the heading was) */
        } else if (!strncmp(text, "mission score:", 14)) {
            MISSION_SCORE = r->score;
            snprintf(text = line, sizeof(line), "endless score:%d", r->score);
        }
    }
    return func_8005133C_orig(text, x, y);
}

static u8 kept[0xDB * 4]; /* the campaign's g_Args, while the run's stats show */
void func_800439E0_orig(void);

/* A save made from the stats screen (SAVE GAME: func_800439E0 packs g_Args for the card) saves the campaign as it
 * was, not the run's stand-in mission */
void func_800439E0(void) {
    static u8 now[0xDB * 4];
    u8 *a = G_ARGS;
    if (!in_endless_stats || !a) {
        func_800439E0_orig();
        return;
    }
    memcpy(now, a, sizeof(now));
    memcpy(a, kept, sizeof(kept));
    func_800439E0_orig();
    memcpy(a, now, sizeof(now));
}

void func_80028644(void) {
    u8 *a = G_ARGS, *table;
    char names[3][4], name[4] = "";
    u32 scores[3];
    int i, m;
    if (!port_endless_stats || !a) {
        func_80028644_orig();
        return;
    }
    port_endless_stats = 0;
    memcpy(kept, a, sizeof(kept));
    m = *(s16 *)(a + 0x18A);
    table = a + 0xC + m * 24; /* the mission's three: initials (4 bytes), score */
    port_endless_best(names, scores);
    for (i = 0; i < 3; i++) {
        memcpy(table + i * 8, names[i], 4);
        *(u32 *)(table + i * 8 + 4) = scores[i];
    }
    if (port_endless_unranked)
        *(u32 *)(a + 4) |= 1 << 9;
    a[0x114] = 1; /* the stats as a mission completed's */
    a[0x115] = 0;
    in_endless_stats = 1;
    port_endless_logf(-1, "the run's stats screen opens");
    func_80028644_orig();
    in_endless_stats = 0;
    for (i = 0; i < 3; i++) /* the run's entry, if it made the table */
        if (port_endless_run.score > 0 && *(u32 *)(table + i * 8 + 4) == (u32)port_endless_run.score &&
            memcmp(table + i * 8, names[i], 4)) {
            memcpy(name, table + i * 8, 3);
            break;
        }
    port_endless_keep(name);
    if (!port_endless_unranked) /* (a ranked run: the table to the card the campaign autoloads from, else slot 1) */
        endless_card_write(autoload_slot() < 0 ? 0 : autoload_slot(), "a ranked run");
    {
        char diff[600];
        int n = 0;
        diff[0] = 0;
        for (i = 0; i < (int)sizeof(kept) && n < 560; i++)
            if (a[i] != kept[i])
                n += snprintf(diff + n, sizeof(diff) - n, "%X:%02X>%02X ", i, kept[i], a[i]);
        port_endless_logf(-1, "stats: g_Args changed: %s", diff);
    }
    /* the campaign put back, but not the front end's running state kept in g_Args beside it (0x116-0x12F: the
     * drawing buffer at 0x118 above all; put back, the screens after drew into the buffer not shown, and showed
     * nothing of their tiles and text) */
    memcpy(a, kept, 0x116);
    memcpy(a + 0x130, kept + 0x130, sizeof(kept) - 0x130);
    /* (a save made from the stats leaves the front end's "go on to the next mission" flag set, which then keeps the
     * main menu from making its items when mission select is left with BACK: the run has no next mission) */
    *(u8 *)0x80173998u = 0;
    if (getenv("TWINE_TEST_UNLOCK")) /* (headless tests: every mission open on mission select) */
        memset(a + 0x170, 1, 11);
    port_endless_logf(-1, "the run's stats shown (initials \"%s\")", name);
    after_stats = 20 * 60;
}

/* ENDLESS on mission select: a twelfth tile at the end of the middle row, which becomes four like the others
 * (4/4/4: the row's three missions move left under the rows' columns). Choosing it starts endless mode
 * (port/src/endless.c), the front end left at once as for the BLACKJACK item.
 *
 * Mission select is func_8003C14C, here in C with the twelfth tile: each mission's tile is a POLY_FT4 per buffer
 * (D_801516A8[11][2]) of its thumbnail (one of TIM99.BIN's, put in VRAM when the screen opens), made from tables
 * of 12 (the last never used: D_80073C9C its x on the screen, D_80073CB4 its y; the thumbnail's place in VRAM, its
 * CLUT's). Over the chosen one the highlight (D_800DDF18[2]) pulses; the chosen mission's name is the text item
 * under the tiles (func_80051420, from the names D_80073A94). The tiles are drawn by the frame (func_8002ABF0:
 * the eleven added to the ordering table one after the other); ENDLESS's own tile goes in the chain right after
 * the last of them, as the frame draws the text items (func_80051824 or func_800519D8, while the screen's text
 * shows: D_80073960). Its prims are in PS1 memory: the MIPS code of func_80028644 (replaced above) past the jump
 * that leads to its native code. The cursor (func_80033818, here in C too) goes over the twelve as a 4x3 grid.
 *
 * ENDLESS's thumbnail is a collage of six missions' thumbnails, in slanting strips, made in 15-bit colour from
 * their TIMs in RAM and put in VRAM (640, 208), free on this screen. */
s32 func_80055244(s32 pad);
void func_80031754(void);
void func_800548B0(char *name, u32 to);
void func_800316D4(u8 a, u8 b, u8 c);
void func_80039DE8(void *panel, s32 a, s32 b, s32 c);
void func_8003A2C4(void *panel, s32 a, s32 b, s32 c);
void func_8004A574(u32 tim);
void func_80051420(char *text, s32 y);
void func_80051C34(void);
void func_8005550C(s32 sound, s32 arg1);
s32 func_8004E55C(void);
void func_80049B2C(void);
s32 func_80031B54(void);
s32 func_80031C2C(void);
s32 func_80031EAC(void);
s32 func_80031F74(void);
void func_80031A4C(void);
void func_8002A66C(void);
void func_80051824_orig(u32 ot, s32 buf);
void func_800519D8_orig(u32 ot, s32 buf);
void gpu_load_image(s32 x, s32 y, s32 w, s32 h, const u16 *src);
void port_endless_begin(u8 *args);

#define U8(a) (*(u8 *)(a))
#define S16(a) (*(s16 *)(a))
#define U16(a) (*(u16 *)(a))
#define S32(a) (*(s32 *)(a))
#define TILES ((u8 *)0x801516A8u)     /* [11][2] POLY_FT4, 0x28 bytes each */
#define HIGHLIGHT ((u8 *)0x800DDF18u) /* [2] POLY_FT4 */
#define PANEL_TOP ((u8 *)0x80154368u) /* the screen's two title bars ([2] each) */
#define PANEL_BAR ((u8 *)0x801543B8u)
#define TAB_X ((s16 *)0x80073C9Cu)    /* the tiles' tables, 12 each */
#define TAB_Y ((s16 *)0x80073CB4u)
#define TAB_U ((s16 *)0x80073C6Cu)
#define TAB_V ((s16 *)0x80073C84u)
#define TAB_VX ((s16 *)0x80073CCCu)
#define TAB_VY ((s16 *)0x80073CE4u)
#define TAB_CX ((s16 *)0x80073CFCu)
#define TAB_CY ((s16 *)0x80073D14u)
#define NAMES ((char **)0x80073A94u)
#define LEVEL_OF ((u8 *)0x800710D0u)  /* each mission's level */
#define STATE (*(s32 *)0x80073E8Cu)   /* 1 choosing, 2 chosen, 3 back, 5 options */
#define NAME_Y (*(s32 *)0x800DFE20u)
#define PAD_NOW (*(s32 *)0x80151A1Cu)
#define PAD_WAS (*(s32 *)0x800DFDE0u)
#define ANY_MISSION (*(s32 *)0x80073E30u) /* the developers' code: every mission may be chosen */
#define SINE ((s32 *)0x800893BCu)     /* (the high half of each word) */
#define REPEAT (*(u32 *)0x800747B0u)  /* frames a direction has been held */
#define ENDLESS 11
#define OWN ((u8 *)0x80028650u)       /* ENDLESS's tile, [2] POLY_FT4 (func_80028644's dead MIPS code) */
#define COLLAGE_X 640
#define COLLAGE_Y 208

static int in_mission_select, endless_briefing;
static char endless_name[] = "ENDLESS";

static u8 *tile(int m, int buf) {
    return m == ENDLESS ? OWN + buf * 0x28 : TILES + m * 0x50 + buf * 0x28;
}

static int open_mission(int m) {
    return m == ENDLESS || G_ARGS[0x170 + m] || ANY_MISSION;
}

/* a POLY_FT4 over (x, y), 116x40, of the texture at (u, v) */
static void quad(u8 *p, s32 x, s32 y, u32 u, u32 v, u16 clut, u16 tpage) {
    U8(p + 3) = 9;
    U8(p + 7) = 0x2C;
    U8(p + 4) = U8(p + 5) = U8(p + 6) = 0x80;
    U16(p + 0x16) = tpage;
    U16(p + 0xE) = clut;
    U8(p + 0xC) = u, U8(p + 0xD) = v;
    U8(p + 0x14) = u + 0x74, U8(p + 0x15) = v;
    U8(p + 0x1C) = u, U8(p + 0x1D) = v + 0x28;
    U8(p + 0x24) = u + 0x74, U8(p + 0x25) = v + 0x28;
    S16(p + 8) = x, S16(p + 0xA) = y;
    S16(p + 0x10) = x + 0x74, S16(p + 0x12) = y;
    S16(p + 0x18) = x, S16(p + 0x1A) = y + 0x28;
    S16(p + 0x20) = x + 0x74, S16(p + 0x22) = y + 0x28;
}

/* the collage: six thumbnails' strips (their TIMs at 0x8017ECD0, 0x1440 bytes each: CLUT, then 116x40 at 8 bits) */
static void make_collage(void) {
    static const int from[6] = {0, 1, 4, 5, 8, 10};
    static u16 img[116 * 40];
    int x, y;
    for (y = 0; y < 40; y++)
        for (x = 0; x < 116; x++) {
            int at = x + (39 - y) / 3, k = at * 6 / (116 + 13), edge = (at * 6) % (116 + 13);
            u8 *tim = (u8 *)0x8017ECD0u + from[k] * 0x1440;
            u16 *clut = (u16 *)(tim + 0x14);
            u8 *pix = tim + 8 + S32(tim + 8) + 12;
            img[y * 116 + x] = k && edge < 6 ? 0x8000 | (6 << 10) | (20 << 5) | 25 : clut[pix[y * 116 + x]];
        }
    gpu_load_image(COLLAGE_X, COLLAGE_Y, 116, 40, img);
}

/* the frame's text items, drawn after the tiles: ENDLESS's tile chained after the last mission's */
static void link_own(s32 buf) {
    u8 *last = TILES + 10 * 0x50 + buf * 0x28, *own = OWN + buf * 0x28;
    if (!in_mission_select)
        return;
    S32(own) = (S32(own) & 0xFF000000) | (S32(last) & 0xFFFFFF);
    S32(last) = (S32(last) & 0xFF000000) | ((u32)own & 0xFFFFFF);
}

void func_80051824(u32 ot, s32 buf) {
    link_own(buf);
    func_80051824_orig(ot, buf);
}

void func_800519D8(u32 ot, s32 buf) {
    link_own(buf);
    func_800519D8_orig(ot, buf);
}

/* the chosen tile's corners for the highlight (buffer buf) */
static void highlight(int buf) {
    u8 *h = HIGHLIGHT + buf * 0x28, *t = tile(CURSOR, buf);
    S32(h + 8) = S32(t + 8);
    S32(h + 0x10) = S32(t + 0x10);
    S32(h + 0x18) = S32(t + 0x18);
    S32(h + 0x20) = S32(t + 0x20);
}

/* one of the title bars' two halves: w wide from texel u, 20 high */
static void panel(u8 *p, s32 x, s32 y, s32 w, u32 u, u16 tpage) {
    quad(p, x, y, u, 0, 0x7968, tpage);
    U8(p + 0x14) = U8(p + 0x24) = u + w;
    U8(p + 0x1D) = U8(p + 0x25) = 0x14;
    S16(p + 0x10) = S16(p + 0x20) = x + w;
    S16(p + 0x1A) = S16(p + 0x22) = y + 0x14;
}

static void shade(int buf) {
    int m;
    for (m = 0; m <= ENDLESS; m++) {
        u8 *t = tile(m, buf), c = !open_mission(m) ? 0x32 : m == CURSOR ? 0xC0 : 0xA0;
        U8(t + 4) = U8(t + 5) = U8(t + 6) = c;
    }
}

static void show_name(void) {
    func_80051420(CURSOR == ENDLESS ? endless_name : NAMES[CURSOR], NAME_Y);
    func_80051768(0, *(s16 *)0x80153E08u, *(s16 *)0x80153E0Au);
}

void func_8003C14C_orig(void);
void func_8003C14C(void) {
    u8 *a = G_ARGS;
    s32 b, m, frame = 0;
    if (getenv("TWINE_TEST_ORIG_MS")) {
        func_8003C14C_orig();
        return;
    }
    port_endless_logf(-1, "shell: mission select opens (state %d, cursor %d, args %08X)", STATE, CURSOR, S32(a));
    func_8002A66C();
    port_endless_logf(-1, "shell: mission select: loading its picture");
    func_800548B0((char *)0x8008E344u, 0x801A9A44u); /* MSSNSEL.TIM */
    port_endless_logf(-1, "shell: mission select: picture loaded");
    func_80031754();
    *(s32 *)0x80073924u = 1;
    PAD_WAS = func_80055244(0);
    PAD_NOW = PAD_WAS;
    if (*(s16 *)0x80073E68u > 0)
        CURSOR = *(s16 *)0x80073E68u;
    CURSOR = (a[0] & 1) ? *(s16 *)0x80073E60u : 0;
    if (getenv("TWINE_TEST_MS_CURSOR")) /* (headless tests: the cursor starts here) */
        CURSOR = atoi(getenv("TWINE_TEST_MS_CURSOR"));
    a[0x170] = 1;
    if (CURSOR < ENDLESS)
        a[0x170 + CURSOR] = 1;
    CURSOR_END = ENDLESS + 1;
    CURSOR_WAS = CURSOR;
    /* the middle row under the others' columns, and ENDLESS at its end */
    TAB_X[4] = TAB_X[0], TAB_X[5] = TAB_X[1], TAB_X[6] = TAB_X[2];
    for (b = 0; b < 2; b++) {
        s32 y0 = *(s16 *)(a + 0x18E) * (1 - b); /* (the display's offset in the buffer) */
        panel(PANEL_TOP + b * 0x28, 0x32, y0 + 0x17, 0x7F, 0x80, 0x1C);
        panel(PANEL_BAR + b * 0x28, 0xB1, y0 + 0x17, 0x73, 0, 0x1D);
        for (m = 0; m < ENDLESS; m++) {
            u8 *t = tile(m, b);
            quad(t, TAB_X[m], TAB_Y[m] + y0, (u8)TAB_U[m], (u8)TAB_V[m], (TAB_CY[m] << 6) | ((TAB_CX[m] >> 4) & 0x3F),
                 ((TAB_VY[m] & 0x100) >> 4) | ((TAB_VX[m] & 0x3FF) >> 6) | 0x80 | ((TAB_VY[m] & 0x200) * 4));
            if (m & 1) /* (as the screen makes them: the odd ones a texel narrower) */
                U8(t + 0x14) -= 1, U8(t + 0x24) -= 1;
        }
        quad(tile(ENDLESS, b), TAB_X[3], TAB_Y[4] + y0, 0, COLLAGE_Y & 0xFF, 0, (2 << 7) | (COLLAGE_X >> 6));
        quad(HIGHLIGHT + b * 0x28, 0, 0, 0, 0x16, 0x7AE8, 0x1C);
        U8(HIGHLIGHT + b * 0x28 + 0x14) = U8(HIGHLIGHT + b * 0x28 + 0x24) = 0x78;
        U8(HIGHLIGHT + b * 0x28 + 0x1D) = U8(HIGHLIGHT + b * 0x28 + 0x25) = 0x40;
        highlight(b);
    }
    func_8004A574(0x801789D8u);
    *(s32 *)0x80073984u = 1;
    for (m = 0; m < ENDLESS; m++) /* the thumbnails */
        func_8004A574(0x8017ECD0u + m * 0x1440);
    make_collage();
    *(s32 *)0x800739D4u = 1;
    func_8004A574(0x801793A0u);
    *(s32 *)0x800739D8u = 1;
    func_8004A574(0x8018CCD0u);
    func_8004A574(0x8018EA30u);
    func_8004A574(0x8018CB90u);
    func_8004A574(0x8018EB74u);
    func_8004A574(0x801987C0u);
    func_8004A574(0x80198900u);
    *(s32 *)0x800739F4u = 1;
    *(s32 *)0x800739FCu = 1;
    *(s32 *)0x800739F8u = 1;
    *(s32 *)0x8017399Cu = 0;
    NAME_Y = 0xBE;
    *(s32 *)0x80174F68u = 0x16;
    show_name();
    *(s32 *)0x80073960u = 1;
    *(s32 *)0x800739DCu = 2;
    *(s32 *)0x800739E0u = 1;
    STATE = 1;
    port_endless_logf(-1, "shell: mission select: tiles made (TIMs %08X %08X %08X %08X)", S32(0x8017ECD0u), S32(0x801789D8u),
                      S32(0x8018CCD0u), S32(0x801793A0u));
    func_800316D4(0, 0, 0);
    *(s32 *)0x80073914u = 0;
    shade(0);
    shade(1);
    in_mission_select = 1;
    func_80039DE8(PANEL_TOP, 2, 0x32, 0xF3);
    port_endless_logf(-1, "shell: mission select ready (cursor %d)", CURSOR);
    *(u8 *)0x80073E3Fu = 1;
    while (STATE == 1) {
        s32 buf = *(s32 *)(a + 0x118), pulse;
        u8 *h = HIGHLIGHT + buf * 0x28;
        func_8002ABF0();
        func_80032048();
        STATE = func_80033818();
        if (CURSOR != CURSOR_WAS) {
            func_80051C34();
            show_name();
            frame = 0;
        }
        highlight(buf);
        pulse = (((s16)(SINE[(frame << 6) & 0xFFF] >> 16) * 100) >> 12) + 100;
        U8(h + 4) = U8(h + 5) = U8(h + 6) = pulse;
        shade(buf);
        if (STATE == 2) {
            if (!open_mission(CURSOR))
                STATE = 1;
            else if (CURSOR != ENDLESS) {
                *(s16 *)(a + 0x188) = LEVEL_OF[CURSOR];
                *(s16 *)(a + 0x18A) = CURSOR;
            }
        }
        frame++;
    }
    *(u8 *)0x80073E3Fu = 0;
    highlight(0);
    highlight(1);
    shade(0);
    shade(1);
    func_8003A2C4(PANEL_TOP, 2, 0x32, 0xF3);
    func_80031754();
    in_mission_select = 0;
    port_endless_logf(-1, "shell: mission select left (state %d, cursor %d)", STATE, CURSOR);
    if (STATE == 2 && CURSOR == ENDLESS) { /* endless mode: its first room's mission, after its briefing */
        port_endless_begin(a);
        endless_briefing = 1;
        port_endless_logf(-1, "chosen on mission select");
    }
}

/* The cursor on mission select: as the screen's own step (its sounds, Cross or Start to choose, Triangle back,
 * Square options; a direction held repeats), over the 4x3 grid with ENDLESS at the middle row's end */
static void step(int dir) {
    /* the grid: rows 0 1 2 3 / 4 5 6 11 / 7 8 9 10 */
    static const s8 next[4][12] = {
        {1, 2, 3, 4, 5, 6, 11, 8, 9, 10, 0, 7},  /* right */
        {10, 0, 1, 2, 3, 4, 5, 11, 7, 8, 9, 6},  /* left */
        {4, 5, 6, 11, 7, 8, 9, 0, 1, 2, 3, 10},  /* down */
        {7, 8, 9, 10, 0, 1, 2, 4, 5, 6, 11, 3},  /* up */
    };
    int i;
    REPEAT = 0;
    for (i = 0; i < 12; i++) { /* on to the next open one that way */
        CURSOR = next[dir][CURSOR];
        if (open_mission(CURSOR))
            break;
    }
}

s32 func_80033818(void) {
    if (func_8004E55C())
        return 4;
    if (*(s32 *)0x80073E28u) {
        func_80049B2C();
        return 1;
    }
    CURSOR_WAS = CURSOR;
    if (func_80031B54())
        return 3;
    if (func_80031C2C() || func_80031EAC())
        return 2;
    if (func_80031F74())
        return 5;
    func_80031A4C();
    {
        u32 held = (u32)(S32(G_ARGS + 0x11C) >> 2) < REPEAT; /* held long enough to repeat */
        if (*(s16 *)0x80160FB2u && (held || !*(s16 *)0x800DEBF4u))
            step(0);
        if (*(s16 *)0x80153CFCu && (held || !*(s16 *)0x80097E44u))
            step(1);
        if (*(s16 *)0x80153D62u && (held || !*(s16 *)0x80098C10u))
            step(2);
        if (*(s16 *)0x800DFDF8u && (held || !*(s16 *)0x80160FE0u))
            step(3);
    }
    if (CURSOR != CURSOR_WAS)
        func_8005550C(1, 0);
    if (CURSOR < 0)
        CURSOR = 0;
    if (CURSOR > ENDLESS)
        CURSOR = ENDLESS;
    return 1;
}

/* Endless mode's briefing, after ENDLESS is chosen: the mission briefing screen (func_800443D8) with the mode's
 * rules, in C: its title bar and the three portraits, each a page (the cursor goes over them: func_80032CA0) with
 * a heading and lines of text (made as text items, as the screen's own: func_8005133C), but no film of M and no
 * gadgets. NEXT starts the run (the front end leaves for its first room's mission, as for a mission briefed);
 * BACK gives it up and goes back to mission select. */
void func_800443D8_orig(void);
void func_800512FC(s32 on);
void func_8002A6C0(void);
void func_800540DC(s32 arg0);
void func_800540A8(void);
s32 func_80032CA0(void);
void port_endless_cancel(void);
#define TEXT_X (*(s32 *)0x800DFDF4u)
#define TEXT_Y (*(s32 *)0x800DFE20u)
#define LINE_H (*(s32 *)0x80174F68u)

static char brief_text[3][6][40];
static const char *const brief_src[3][6] = {
    {"m's briefing", "eliminate enemies", "rooms from all missions", "fight to open the exit",
     "they keep coming", 0},
    {"q's gadgets", "weapons carry over", "guards drop armor", "and ammo too", "take the heavy armor", 0},
    {"moneypenny's advice", "keep moving james", "early kills score more", "quick rooms pay more",
     "aim well. it counts", 0},
};

static void brief_item(char *text, s32 x, s32 y) {
    func_8005133C(text, x, y);
    func_80051768(ITEMS - 1, *(s16 *)0x80153E08u, *(s16 *)0x80153E0Au);
}

static void brief_page(int page) {
    static char name[] = "ENDLESS", dash[5][2];
    int i;
    func_80051C34();
    func_8005133C(name, TEXT_X - 0x18, TEXT_Y);
    func_80051768(0, *(s16 *)0x80153E08u, *(s16 *)0x80153E0Au);
    *(s32 *)0x800739A4u = page == 0;
    *(s32 *)0x800739A8u = page == 1;
    *(s32 *)0x800739ACu = page == 2;
    *(s32 *)0x800739B0u = 0;
    strcpy(brief_text[page][0], brief_src[page][0]);
    brief_item(brief_text[page][0], TEXT_X - 0x54, TEXT_Y + 0x10);
    for (i = 1; brief_src[page][i]; i++) {
        s32 y = i * LINE_H + TEXT_Y + 0x46;
        strcpy(dash[i - 1], "-");
        brief_item(dash[i - 1], TEXT_X - 0x60, y);
        strcpy(brief_text[page][i], brief_src[page][i]);
        brief_item(brief_text[page][i], TEXT_X - 0x54, y);
    }
}

/* a POLY_FT4 of the screen's: texture (u0, v0)-(u1, v1), on the screen (x0, y0)-(x1, y1) */
static void brief_quad(u8 *p, u16 tpage, u16 clut, u32 u0, u32 v0, u32 u1, u32 v1, s32 x0, s32 y0, s32 x1, s32 y1) {
    quad(p, x0, y0, u0, v0, clut, tpage);
    U8(p + 0x14) = U8(p + 0x24) = u1;
    U8(p + 0x1D) = U8(p + 0x25) = v1;
    S16(p + 0x10) = S16(p + 0x20) = x1;
    S16(p + 0x1A) = S16(p + 0x22) = y1;
}

void func_800443D8(void) {
    u8 *a = G_ARGS;
    s32 b, y0;
    if (!endless_briefing) {
        func_800443D8_orig();
        return;
    }
    endless_briefing = 0;
    func_800548B0((char *)0x8008EA10u, 0x801A9A44u); /* BRIEFBG.TIM */
    func_80031754();
    *(s32 *)0x80073924u = 1;
    PAD_WAS = func_80055244(0);
    PAD_NOW = PAD_WAS;
    CURSOR = 0;
    CURSOR_END = 3;
    CURSOR_WAS = CURSOR;
    func_800512FC(1);
    for (b = 0; b < 2; b++) {
        y0 = S16(a + 0x18E) * (1 - b);
        brief_quad((u8 *)0x80097DF0u + b * 0x28, 0x1D, 0x796C, 0x40, 0xA6, 0xDC, 0xBA, 0x32, y0 + 0x17, 0xCE, y0 + 0x2B);
        brief_quad((u8 *)0x80097E48u + b * 0x28, 0x9F, 0x7C28, 0, 0, 0x58, 0x32, 0x63, y0 + 0x35, 0xBB, y0 + 0x67);
        brief_quad((u8 *)0x800DEC00u + b * 0x28, 0x9F, 0x7CE8, 0, 0x96, 0x58, 0xC8, 0x63, y0 + 0x35, 0xBB, y0 + 0x67);
        brief_quad((u8 *)0x80098BB8u + b * 0x28, 0x9F, 0x7C68, 0, 0x32, 0x58, 0x64, 0x63, y0 + 0x6A, 0xBB, y0 + 0x9C);
        brief_quad((u8 *)0x800DEFA0u + b * 0x28, 0x9F, 0x7CA8, 0, 0x64, 0x58, 0x96, 0x63, y0 + 0x9F, 0xBB, y0 + 0xD1);
    }
    func_8004A574(0x8019ADBCu);
    *(s32 *)0x800739A0u = 1;
    *(s32 *)0x800739DCu = 2;
    *(s32 *)0x800739E0u = 1;
    *(s32 *)0x800739F4u = 1;
    *(s32 *)0x800739FCu = 1;
    *(s32 *)0x8017399Cu = 0;
    TEXT_X = 0x12E;
    TEXT_Y = 0x39;
    LINE_H = 0xD;
    *(s32 *)0x80073960u = 1;
    STATE = 1;
    *(s32 *)0x800747ACu = 0;
    func_8004A574(0x8019BB5Cu); /* the portraits */
    func_8004A574(0x8019CEACu);
    func_8004A574(0x8019F54Cu);
    brief_page(0);
    func_80039DE8((void *)0x80097DF0u, 1, 0x32, 0x9C);
    func_8002A6C0();
    func_800540DC(*(s32 *)0x80074774u);
    while (STATE == 1) {
        func_8002ABF0();
        func_80032048();
        STATE = func_80032CA0();
        if (CURSOR != CURSOR_WAS)
            brief_page(CURSOR);
    }
    func_800540A8();
    func_8003A2C4((void *)0x80097DF0u, 1, 0x32, 0x84);
    func_800512FC(0);
    func_80031754();
    if (STATE == 3) { /* BACK: no run */
        port_endless_cancel();
        port_endless_logf(-1, "briefing left: no run");
    }
}

/* The memory card screens (arg: 2 the save after a mission's stats) */
void func_8003EA00_orig(u8 mode);
void func_8003EA00(u8 mode) {
    port_endless_logf(-1, "shell: memory card screen %d (stats %d)", mode, in_endless_stats);
    func_8003EA00_orig(mode);
    port_endless_logf(-1, "shell: memory card screen done (state %d)", STATE);
}

/* (endless mode's log: the keys pressed on its screens: mission select, briefing, stats) */
static void shell_keys(void) {
    static u32 was;
    extern int port_text_entry;
    if (port_text_entry > 0)
        port_text_entry--; /* (the initials entry sets it while it shows) */
    u32 now = *(u32 *)0x80151A1Cu;
    /* (and no attract mode from these screens: the front end's idle count, which after a while leaves a screen for
     * the intro film, left the run's stats and mission select broken behind it) */
    if (in_mission_select || in_endless_stats || endless_briefing)
        *(s32 *)0x800747ACu = 0;
    if (after_stats > 0 && --after_stats % 100 == 0) /* (what the front end is doing, for a while after the stats) */
        port_endless_logf(-1, "shell: after the stats: state %d, cursor %d, mission select %d, text %d, screen %d %d",
                          STATE, CURSOR, in_mission_select, S32(0x80073960u), S32(0x80073984u), S32(0x800739D4u));
    if ((in_mission_select || in_endless_stats || endless_briefing || after_stats > 0) && (now & ~was))
        port_endless_logf(-1, "shell: keys %04X on %s", now & ~was,
                          in_endless_stats ? "the run's stats" : in_mission_select ? "mission select" :
                          endless_briefing ? "the briefing" : "the front end (after the stats)");
    was = now;
}

/* The hi-score table's initials entry (func_800385C4, each frame it shows: up and down choose the letter, left and
 * right the place, its letters D_800738E8[3], the letter chosen D_80173A40 at the place D_80174E68, the cursor's x
 * D_800738E0 from the letters' widths D_800738D4[3]): letters typed with Shift held go in as if chosen, one place on
 * after each, and Shift+Backspace goes a place back (port/src/video.c) */
s32 func_800385C4_orig(s16 item);
int port_text_take(void);
extern int port_text_entry;
#define LETTER (*(s32 *)0x80173A40u)
#define PLACE (*(s32 *)0x80174E68u)
#define LETTERS ((s32 *)0x800738E8u)
#define WIDTHS ((s32 *)0x800738D4u)
#define CURSOR_X (*(s32 *)0x800738E0u)

static void initials_place(int place) {
    PLACE = place;
    LETTER = LETTERS[place];
    CURSOR_X = place == 1 ? WIDTHS[0] + 3 : place == 2 ? WIDTHS[0] + WIDTHS[1] + 6 : 0;
    func_8005550C(0, 0);
}

s32 func_800385C4(s16 item) {
    int c = port_text_take();
    s32 r;
    port_text_entry = 3;
    if (c == '\b') {
        if (PLACE > 0)
            initials_place(PLACE - 1);
    } else if (c >= 'a' && c <= 'z')
        LETTER = c - 'a';
    r = func_800385C4_orig(item);
    if (c >= 'a' && c <= 'z') {
        port_endless_logf(-1, "shell: initial '%c' typed at place %d", c, PLACE);
        if (PLACE < 2)
            initials_place(PLACE + 1);
    }
    return r;
}
