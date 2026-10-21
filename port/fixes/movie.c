/* The movie player (MOVIE.EXE, which plays a mission's ending) on the PC: what the native build changes in its
 * code (port/mods.py builds this into the movie module). Names are the movie player's own.
 *
 * MOD_REPLACES_NATIVE: func_8002FC6C func_80026C2C
 */
#include "common.h"

u32 func_8002FC6C_orig(u32 **addr, u32 **header);
void port_spin(u32 ns);
void func_80026C2C_orig(s32 movie);
int port_hd_movie(const char *name);
#define D_80038460 ((char *)0x80038460u) /* the movies' names, 16 bytes each (MGMLOGO first) */

/* libcd's StGetNext, as in the front end (fixes/shell.c): a call that finds no frame takes as long as a pass of
 * the player's waiting loop does on the console. */
u32 func_8002FC6C(u32 **addr, u32 **header) {
    u32 r = func_8002FC6C_orig(addr, header);
    if (r)
        port_spin(1300);
    return r;
}

/* Plays movie number `movie` (its name looked up, the stream opened, frames decoded and shown until the end or a
 * button): the film's own footage instead when the user has a clip of it (hdmovie.c, TWINE_HD_MOVIES). */
void func_80026C2C(s32 movie) {
    if (!port_hd_movie(D_80038460 + (s16)movie * 16))
        func_80026C2C_orig(movie);
}
