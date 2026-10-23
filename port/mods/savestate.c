/* Save states (native PC build only): F5 saves, F6 loads (port/src/savestate.c). The runtime takes and puts back
 * a state where the mission's frame loop (func_8002C844) reads the controller at its top, before it does: nothing
 * of a frame is under way there.
 *
 * Pad_Update is called from one other place, a dialog's (func_80029A34, deeper in the frame); the loop's call is
 * the one from the shallowest native frame, which runs first in every frame (the runtime tells them apart).
 *
 * MOD_REPLACES_NATIVE: Pad_Update
 *
 * Names are LEVEL01's; port/mods.py maps them onto each level. */
#include "twine.h"

s32 Pad_Update_orig(void);
void port_state_point(void *frame);
void func_80026F30(s32 page, s32 arg1);
extern s32 g_DevMenu;
extern s32 D_8009CAC4; /* set as the menu closes */
extern int port_debug_key;
void port_fnt_frame(void);

/* F9 (video.c) opens and closes the developers' debug menu, as the game's own menu code does (g_DevMenu, then its page). */
s32 Pad_Update(void) {
    static int calls;
    port_state_point(__builtin_frame_address(0));
    port_fnt_frame();
    if (getenv("TWINE_TEST_DEBUG_PAGE")) /* headless tests: DISPLAY's page (the menu's state: vals[0]) */
        (&g_DevMenu)[14] = atoi(getenv("TWINE_TEST_DEBUG_PAGE"));
    if (getenv("TWINE_TEST_DEBUG_MENU") && ++calls == atoi(getenv("TWINE_TEST_DEBUG_MENU")))
        port_debug_key = 1;
    if (port_debug_key) { /* F9 again closes it, as its Start does (func_80032AA0) */
        port_debug_key = 0;
        if (g_DevMenu) {
            g_DevMenu = 0;
            func_80026F30(4, 0);
            D_8009CAC4 = 1;
        } else {
            g_DevMenu = 1;
            func_80026F30(1, 0);
        }
    }
    return Pad_Update_orig();
}
