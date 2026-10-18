/* Wider field of view in every level: projection distance 300 -> 240 (about 81 -> 94 degrees horizontal).
 * Weapon zoom still works on top of it, because the zoom code starts from g_Projection.
 *
 * MOD_REPLACES: Gfx_Init */
#include "twine.h"

void Gfx_Init_orig(void);
void func_800876E4(s32 h); /* PsyQ: set the GTE projection distance */

void Gfx_Init(void) {
    Gfx_Init_orig();
    g_Projection = 240;
    func_800876E4(g_Projection);
}
