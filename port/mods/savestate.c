/* Save states (native PC build only): F5 saves, F6 loads (port/src/savestate.c). The runtime takes and puts back
 * a state where the mission's frame loop (func_8002C844) reads the controller at its top, before it does: nothing
 * of a frame is under way there.
 *
 * Pad_Update is called from one other place, a dialog's (func_80029A34, deeper in the frame); the loop's call is
 * the one from the shallowest native frame, which runs first in every frame (the runtime tells them apart).
 *
 * MOD_REPLACES_NATIVE: Pad_Update FntPrint FntFlush
 *
 * Names are LEVEL01's; port/mods.py maps them onto each level. */
#include "twine.h"

s32 Pad_Update_orig(void);
void port_state_point(void *frame);
void func_80026F30(s32 page, s32 arg1);
extern s32 g_DevMenu;
extern int port_debug_key;

/* F9 (video.c) opens the developers' debug menu, as the game's own menu code does (g_DevMenu, then its page). */
s32 Pad_Update(void) {
    static int calls;
    port_state_point(__builtin_frame_address(0));
    if (getenv("TWINE_TEST_DEBUG_MENU") && ++calls == atoi(getenv("TWINE_TEST_DEBUG_MENU")))
        port_debug_key = 1;
    if (port_debug_key) {
        port_debug_key = 0;
        g_DevMenu = 1;
        func_80026F30(1, 0);
    }
    return Pad_Update_orig();
}

/* The debug menu's text (FntPrint, FntFlush) goes to the port's overlay (video.c), which draws it in the game's
 * own debug font: the hardware renderer draws the font's sprites before the frame's clear, which wipes them. */
#include <stdarg.h>
int vsnprintf(char *s, unsigned n, const char *fmt, va_list ap);
void port_fnt_print(const char *text);
void port_fnt_flush(void);

extern int port_shown_fps;
extern s16 g_DebugTextY; /* the page's lines (its box: func_80033010) */
int strncmp(const char *a, const char *b, unsigned n);
int snprintf(char *s, unsigned n, const char *fmt, ...);

int FntPrint(char *fmt, ...) {
    char buf[256];
    va_list ap;
    va_start(ap, fmt);
    if (!strncmp(fmt, "FR %2d - FR CAP %2d", 19)) { /* FRAMERATE INFO: the game's rate, and the frames shown */
        s32 rate = va_arg(ap, s32), cap = va_arg(ap, s32);
        va_end(ap);
        g_DebugTextY = 2;
        snprintf(buf, sizeof(buf), "FR %2d - FR CAP %2d\nSHOWN %d FPS\n", rate, cap, port_shown_fps);
        port_fnt_print(buf);
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
