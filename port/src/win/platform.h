#ifndef PLAT_WIN_PLATFORM_H
#define PLAT_WIN_PLATFORM_H
/* The Windows build's platform layer (port/src/platform_win.c): the POSIX the runtime is written against,
 * given by the headers in this folder (port/src/win/, on the include path of the Windows build only, before the
 * MinGW ones they extend with #include_next). Nothing here is seen by the Linux build.
 * workbench/research/NOTES_windows.md has the design. */
#include <stddef.h>

/* the platform's set-up, done before anything else (a constructor before main.c's) */
void plat_init(void);
/* the program's memory a save state holds (savestate.c): the data and BSS of the port's own objects, not of the C
 * runtime linked in with them; n ranges at most, returns how many */
int plat_state_ranges(char **at, unsigned *size, int n);
/* the game's code (the image's .text): what the turbo look and the signals count as the program's own */
extern char __executable_start[], etext[];

#endif
