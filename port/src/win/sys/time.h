/* Windows: the CRT's sys/time.h, and setitimer (ITIMER_REAL: SIGALRM, ITIMER_PROF: SIGPROF, both on the host's
 * clock) on the platform layer (platform_win.c) */
#include_next <sys/time.h>

#ifndef PLAT_WIN_SYS_TIME_H
#define PLAT_WIN_SYS_TIME_H
#include <time.h>

#define ITIMER_REAL 0
#define ITIMER_VIRTUAL 1
#define ITIMER_PROF 2
struct itimerval {
    struct timeval it_interval, it_value;
};
int setitimer(int which, const struct itimerval *value, struct itimerval *old);

#endif
