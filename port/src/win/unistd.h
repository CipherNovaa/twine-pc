/* Windows: the CRT's unistd.h, and what the runtime uses of POSIX's on the platform layer (platform_win.c):
 * readlink("/proc/self/exe") (the program's path, with /), usleep and sleep (ended early by a signal for the
 * thread), gettid, ftruncate of a memfd_create'd file */
#include_next <unistd.h>

#ifndef PLAT_WIN_UNISTD_H
#define PLAT_WIN_UNISTD_H
#include <sys/types.h>
#include <sys/stat.h>

#undef usleep
#undef sleep
#undef ftruncate
#define usleep plat_usleep
#define sleep plat_sleep
#define ftruncate plat_ftruncate
int plat_usleep(unsigned int us);
unsigned int plat_sleep(unsigned int s);
int plat_ftruncate(int fd, long long size);
ssize_t readlink(const char *path, char *buf, size_t size);
pid_t gettid(void);

#endif
