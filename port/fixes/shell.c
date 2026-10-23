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

/* The autoload setting on the OPTIONS screen, a fourth item under CONTROLLER, MEMORY CARD and CHEATS:
 * "AUTOLOAD SLOT 1" / "SLOT 2" / "OFF", changed with Cross or left and right, kept in twine.ini.
 *
 * The screen (func_800293C0) makes its items as text (func_8005133C: left edge, line; each item's glyphs in a
 * table of 20, 0x60 bytes each, glyphs from a pool whose next free one is D_800DFEEC) and draws them in two
 * palettes (func_80051768: the item, the palette's place; D_80098C12/14 normal, D_80153E08/0A chosen). The
 * cursor (D_801628D8) goes over as many items as were made (D_801510B4), and Cross on one past the third stays
 * on the screen, so the item only has to be made and answer its keys (in the menus' step, func_8003277C:
 * D_80151A1C this frame's buttons, D_800DFDE0 the last's). */
s32 func_8005133C(char *text, s32 x, s32 y);
void func_80051768(s32 item, s32 cx, s32 cy);
void func_80029B1C_orig(void);
s32 func_800293C0_orig(void);
s32 func_8003277C_orig(void);
void port_setting_save(const char *key, const char *value);
#define CURSOR (*(s32 *)0x801628D8u)
#define ITEMS (*(s32 *)0x801510B4u)
#define NEXT_GLYPH (*(s32 *)0x800DFEECu)
#define ITEM_X (*(s32 *)0x800DFDF4u)
#define AUTOLOAD_Y 0x9A
static int in_options;
static char autoload_text[32];

static void autoload_item(void) {
    static const char *const names[] = {"AUTOLOAD SLOT 1", "AUTOLOAD SLOT 2", "AUTOLOAD OFF"};
    int v = autoload_slot();
    strcpy(autoload_text, names[v < 0 ? 2 : v]);
    func_8005133C(autoload_text, ITEM_X, AUTOLOAD_Y);
}

/* The OPTIONS screen's items: the fourth after the others */
void func_80029B1C(void) {
    func_80029B1C_orig();
    autoload_item();
    func_80051768(3, *(s16 *)0x80098C12u, *(s16 *)0x80098C14u);
}

s32 func_800293C0(void) {
    s32 r;
    in_options = 1;
    r = func_800293C0_orig();
    in_options = 0;
    return r;
}

/* The menus' step: on the OPTIONS screen, Cross, left or right on the autoload item changes it (made again
 * over its own glyphs, in the chosen palette) */
s32 func_8003277C(void) {
    s32 r = func_8003277C_orig(), step = 0, v;
    u32 now = *(u32 *)0x80151A1Cu, was = *(u32 *)0x800DFDE0u;
    static const char *const values[] = {"1", "2", "off"};
    if (!in_options || CURSOR != 3)
        return r;
    if (r == 2 || ((now & 0x2000) && !(was & 0x2000)))
        step = 1;
    else if ((now & 0x8000) && !(was & 0x8000))
        step = 2;
    if (!step)
        return r;
    v = autoload_slot();
    v = ((v < 0 ? 2 : v) + step) % 3;
    port_setting_save("autoload", values[v]);
    ITEMS = 3;
    NEXT_GLYPH = *(u16 *)(0x800DF0B0u + 3 * 0x60);
    autoload_item();
    func_80051768(3, *(s16 *)0x80153E08u, *(s16 *)0x80153E0Au);
    return r;
}
