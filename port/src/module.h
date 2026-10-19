#ifndef MODULE_H
#define MODULE_H

/* The game's executables in the native build: the boot stub (SLUS_012.72), the movie player, the front end
 * and the eleven levels. Each is linked privately; port/gen_native.py writes its descriptor. Like on the
 * console, one runs at a time in the same RAM: the stub loads the next one from the disc and starts it with
 * BIOS Exec (port_exec in main.c). */
#include "port.h"

typedef struct {
    u32 addr;
    void *fn;
} PortFunc;

typedef struct {
    u32 addr;
    void (*fn)(void *);
} RcEntry;

typedef struct {
    const char *name;  /* boot, movie, shell, level01 ... level11 */
    const char *path;  /* the executable on the disc */
    int level;         /* 1-11 for a level */
    const PortFunc *funcs; /* PS1 address -> native function, sorted (trampolines, calls by address) */
    int func_count;
    const RcEntry *rc_funcs; /* recompiled functions by address, sorted */
    const int *rc_func_count;
    s32 (*main)(s32, void *); /* NULL for the boot stub, whose entry the runtime does (main.c) */
    u32 pc0, text_size;       /* identify the executable the stub has loaded */
    u32 bss_start, bss_end;   /* what its entry point clears */
} PortModule;

extern const PortModule *const port_modules[];
extern const int port_module_count;
extern const PortModule *port_module; /* the executable running now */
extern const PortModule *port_boot;   /* the stub, resident below the others */

#endif
