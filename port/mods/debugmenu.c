/* The developers' debug menu (native PC build only; F9 opens it: mods/savestate.c), with what the retail game
 * leaves out of it.
 *
 * Its text (FntPrint, FntFlush) goes to the port's overlay (video.c), which draws it in the game's own debug font:
 * the hardware renderer draws the font's sprites before the frame's clear, which wipes them. FRAMERATE INFO also
 * says how many frames the port shows a second.
 *
 * DISPLAY gets the pages of the August 2000 beta that retail dropped (its DebugScreen_DrawPage's cases 5, 6, 9):
 * COLLBOX INFO (Page Up / Page Down step through the boxes), TERRAIN INFO and BG COLOR INFO, after retail's ten.
 * The beta's code is rewritten here with retail's variables, paired with the beta's by matching the two
 * executables' functions. Left out: the beta's TERRAIN INFO far clip line (its variable isn't paired yet) and its
 * POLYGON INFO (nearly all "NOT CALCULATED!").
 *
 * MOD_REPLACES_NATIVE: FntPrint FntFlush DebugScreen_DrawPage
 *
 * Names are LEVEL01's; port/mods.py maps them onto each level. */
#include "twine.h"
#include <stdarg.h>

typedef struct { s32 active; s32 cursor; s32 count[12]; s32 vals[12]; } DevMenuState;
extern s32 g_DevMenu[];
#define MENU ((DevMenuState *)g_DevMenu)
extern s16 g_DebugTextX, g_DebugTextY; /* the page's columns and lines (its box) */
extern u8 *D_8009CF7C;                   /* the collision boxes, 0x44 bytes each */
extern s16 D_8009CF80;                   /* how many */
extern s32 *D_800D2FC0;                  /* the loaded area's polygon lists */
extern s32 D_8009E9B4, D_8009E9B8;       /* subdivision: recursion, distance */
extern u8 D_800B8FB0[];                  /* the background colour, R G B */
extern int port_shown_fps, port_debug_step;

int vsnprintf(char *s, unsigned n, const char *fmt, va_list ap);
int snprintf(char *s, unsigned n, const char *fmt, ...);
int strncmp(const char *a, const char *b, unsigned n);
void port_fnt_print(const char *text);
void port_fnt_flush(void);
void DebugScreen_DrawPage_orig(s32 page);
void func_80032E4C(s32 lines, s32 cols); /* the page's box */

#define RETAIL_PAGES 10
static const char *const pages[] = {"COLLBOX INFO", "TERRAIN INFO", "BG COLOR INFO", "WEAPON STATE"};
#define OUR_PAGES 4

static void print(const char *fmt, ...) {
    char buf[256];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    port_fnt_print(buf);
}

int FntPrint(char *fmt, ...) {
    char buf[256];
    va_list ap;
    va_start(ap, fmt);
    MENU->count[0] = RETAIL_PAGES + OUR_PAGES;
    if (!strncmp(fmt, "FR %2d - FR CAP %2d", 19)) { /* FRAMERATE INFO: the game's rate, and the frames shown */
        s32 rate = va_arg(ap, s32), cap = va_arg(ap, s32);
        va_end(ap);
        g_DebugTextY = 2;
        print("FR %2d - FR CAP %2d\nSHOWN %d FPS\n", rate, cap, port_shown_fps);
        return 0;
    }
    if (!strncmp(fmt, "%s%s\n", 6)) { /* a menu line: DISPLAY's value past retail's names is ours */
        char *label = va_arg(ap, char *), *value = va_arg(ap, char *);
        va_end(ap);
        if (!strncmp(label, "DISPLAY:", 8) && MENU->vals[0] >= RETAIL_PAGES)
            value = (char *)pages[MENU->vals[0] - RETAIL_PAGES];
        print("%s%s\n", label, value);
        return 0;
    }
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    port_fnt_print(buf);
    return 0;
}

int FntFlush(int id) {
    port_fnt_flush();
    return 0;
}

static void collbox_page(void) {
    static s32 at;
    s32 i, n = 0;
    u8 *box;
    for (i = 0; D_8009CF7C && i < D_8009CF80; i++)
        n += D_8009CF7C[i * 0x44] != 0;
    g_DebugTextX = 0x19;
    g_DebugTextY = 6;
    print("Num CollBoxes %2d\n", n);
    if (!D_8009CF7C || D_8009CF80 <= 0)
        return;
    at = (at + port_debug_step + D_8009CF80) % D_8009CF80;
    port_debug_step = 0;
    box = D_8009CF7C + at * 0x44;
    print("CollBox       %2d\n", at);
    print("(%6d, %6d, %6d)\n", *(s32 *)(box + 0x10), *(s32 *)(box + 0x14), *(s32 *)(box + 0x18));
    print("Load Area     %d\n", *(s32 *)(*(u8 **)(box + 0x1C)) >> 27);
    print("Type          %d\n", box[1]);
    if (box[1] == 1)
        print("Heading       %d\n", *(s16 *)(box + 0x30));
}

static void terrain_page(void) {
    s32 *t = D_800D2FC0, *p;
    g_DebugTextX = 0x24;
    g_DebugTextY = 9;
    if (!t)
        return;
    print("NUM TSP POLYS:          %5d\n", t[0] ? ((s32 *)t[0])[4] : 0);
    p = (s32 *)t[1];
    print("NUM COLOR CYCLES:       %5d\n", p ? p[0] : 0);
    print("NUM TEXTURE CYCLES:     %5d\n", p ? p[1] : 0);
    print("NUM CONTOUR POLYS:      %5d\n", t[2] ? *(s16 *)((u8 *)t[2] + 0x10) : 0);
    print("NUM WPN CONTOUR POLYS:  %5d\n", t[3] ? *(s16 *)((u8 *)t[3] + 0x10) : 0);
    print("NUM BACKDROP POLYS:     %5d\n", t[4] ? ((s32 *)t[4])[1] * ((s32 *)t[4])[2] : 0);
    print("\n");
    print("SUBDIVISION RECURSION:  %5d\n", D_8009E9B4);
    print("SUBDIVISION DISTANCE:   %5d\n", D_8009E9B8);
}

static void bgcolor_page(void) {
    g_DebugTextX = 0x11;
    g_DebugTextY = 3;
    print("R = %3d\n", D_800B8FB0[0]);
    print("G = %3d\n", D_800B8FB0[1]);
    print("B = %3d\n", D_800B8FB0[2]);
}

/* WEAPON STATE: Bond's weapon and its first-person model as the trace mod sees them (mods/trace.c), live: a line of
 * the trace a field a line */
extern char trace_weapon_line[] __attribute__((weak));
void trace_weapon_state(void) __attribute__((weak));
static void weapon_page(void) {
    char line[64];
    const char *p;
    int n = 0;
    g_DebugTextX = 0x26;
    g_DebugTextY = 12;
    if (!trace_weapon_state) {
        print("(the TRACE mod is not built in)\n");
        return;
    }
    trace_weapon_state();
    for (p = trace_weapon_line; *p; p++) {
        if ((p[0] == ',' && p[1] == ' ') || n == (int)sizeof(line) - 1) {
            line[n] = 0;
            print("%s\n", line);
            n = 0;
            if (p[0] == ',')
                p++;
            continue;
        }
        line[n++] = *p >= 'a' && *p <= 'z' ? *p - 32 : *p; /* (the debug font's capitals) */
    }
    line[n] = 0;
    print("%s\n", line);
}

/* the page DISPLAY chose: retail's, or one of ours drawn as retail draws its own (box, title, page) */
void DebugScreen_DrawPage(s32 page) {
    if (page < RETAIL_PAGES) {
        DebugScreen_DrawPage_orig(page);
        return;
    }
    func_80032E4C(g_DebugTextY + 1, g_DebugTextX);
    print("%s:\n", pages[page - RETAIL_PAGES]);
    if (page == RETAIL_PAGES)
        collbox_page();
    else if (page == RETAIL_PAGES + 1)
        terrain_page();
    else if (page == RETAIL_PAGES + 2)
        bgcolor_page();
    else
        weapon_page();
    port_fnt_flush();
}
