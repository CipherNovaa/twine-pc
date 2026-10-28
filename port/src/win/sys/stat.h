/* Windows: the CRT's sys/stat.h, and POSIX's mkdir(path, mode) (the CRT's takes no mode) */
#include_next <sys/stat.h>

#ifndef PLAT_WIN_SYS_STAT_H
#define PLAT_WIN_SYS_STAT_H
/* (the CRT's one-argument declarations first, or the macro would meet them later) */
#include <io.h>
#include <direct.h>
#undef mkdir
#define mkdir(path, mode) _mkdir(path)

#endif
