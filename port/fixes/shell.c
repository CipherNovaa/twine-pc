/* The front end (SHELL.EXE) on the PC: what the native build changes in its code (port/mods.py builds this into
 * the shell's module). Names are the shell's own.
 *
 * MOD_REPLACES_NATIVE: func_8006CDA8
 */
#include "common.h"

u32 func_8006CDA8_orig(u32 **addr, u32 **header);
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
