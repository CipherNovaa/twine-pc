/* The movie player (MOVIE.EXE, which plays a mission's ending) on the PC: what the native build changes in its
 * code (port/mods.py builds this into the movie module). Names are the movie player's own.
 *
 * MOD_REPLACES_NATIVE: func_8002FC6C
 */
#include "common.h"

u32 func_8002FC6C_orig(u32 **addr, u32 **header);
void port_spin(u32 ns);

/* libcd's StGetNext, as in the front end (fixes/shell.c): a call that finds no frame takes as long as a pass of
 * the player's waiting loop does on the console. */
u32 func_8002FC6C(u32 **addr, u32 **header) {
    u32 r = func_8002FC6C_orig(addr, header);
    if (r)
        port_spin(1300);
    return r;
}
