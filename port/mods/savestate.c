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

s32 Pad_Update(void) {
    port_state_point(__builtin_frame_address(0));
    return Pad_Update_orig();
}
