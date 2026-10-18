/* Start every level with invincibility on (the developer menu's INVINCIBILTY option).
 *
 * MOD_REPLACES: func_8002E5C4 */
#include "twine.h"

void func_8002E5C4_orig(void);

/* Level init: sets up the player, pad mode and level state. */
void func_8002E5C4(void) {
    func_8002E5C4_orig();
    g_Invincible = 1;
}
