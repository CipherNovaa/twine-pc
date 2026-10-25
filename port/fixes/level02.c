/* Courier (LEVEL02.EXE, mission 1) on the PC: what the native build changes in its code (port/mods.py builds this
 * into the level's module). Names are the level's own.
 *
 * MOD_REPLACES_NATIVE: func_8006BC94
 */
#include "common.h"

void func_8006BC94_orig(void);
void port_endless_logf(s32 frame, const char *fmt, ...);
extern int port_endless;

/* The bank's alarm going off (an alarm zone touched: "ALARM TRIGGERED!", the siren, the call for help): not in
 * endless mode, whose rooms are a fight anyway */
void func_8006BC94(void) {
    if (port_endless) {
        static int said;
        if (!said++)
            port_endless_logf(-1, "story: the bank's alarm kept off");
        return;
    }
    func_8006BC94_orig();
}
