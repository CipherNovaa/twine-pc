/* Windows: the CRT's stdlib.h, and POSIX's setenv/unsetenv (platform_win.c) */
#include_next <stdlib.h>

#ifndef PLAT_WIN_STDLIB_H
#define PLAT_WIN_STDLIB_H
int setenv(const char *name, const char *value, int overwrite);
int unsetenv(const char *name);
#endif
