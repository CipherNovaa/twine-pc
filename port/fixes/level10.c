/* Meltdown (LEVEL10.EXE, mission 11) on the PC: what the native build changes in its code (port/mods.py builds this
 * into the level's module). Names are the level's own.
 *
 * MOD_REPLACES_NATIVE: func_8006DC24
 */
#include "common.h"

void func_8006DC24_orig(void);
void port_endless_logf(s32 frame, const char *fmt, ...);
extern int port_endless;
#define RADIATION (*(s32 *)0x800A309Cu) /* the reactor's dose: it builds while Bond is in its hall, and at its top
                                         * hurts him 0x800 a frame, past his armour */

/* The mission's frame (the reactor, its timers and its messages among it): in endless mode, the reactor's hall is a
 * room to fight in, and its dose never builds */
void func_8006DC24(void) {
    if (port_endless) {
        static int said;
        if (RADIATION && !said++)
            port_endless_logf(-1, "story: the reactor's radiation kept off");
        RADIATION = 0;
    }
    func_8006DC24_orig();
    if (port_endless)
        RADIATION = 0;
}
