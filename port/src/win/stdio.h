/* Windows: the CRT's stdio.h, and rename as POSIX has it: an existing destination is replaced (the CRT's fails
 * there; the settings and the memory cards are written to a .new file renamed over the old) */
#include_next <stdio.h>

#ifndef PLAT_WIN_STDIO_H
#define PLAT_WIN_STDIO_H
#undef rename
#define rename plat_rename
int plat_rename(const char *from, const char *to);
#endif
