/* Windows: the CRT's time.h, and POSIX clocks, sleeps and timers on the platform layer (platform_win.c):
 * CLOCK_MONOTONIC on the performance counter, sleeps on high-resolution waitable timers (a signal for the thread
 * ends them early, EINTR, as on Linux), timer_create's signal from a timer thread of its own. */
#include_next <time.h>

#ifndef PLAT_WIN_TIME_H
#define PLAT_WIN_TIME_H
#include <sys/types.h>

typedef struct plat_timer *timer_t;
struct sigevent;
struct itimerspec;

#undef clock_gettime
#undef clock_nanosleep
#undef nanosleep
#undef localtime_r
#undef gmtime_r
#define clock_gettime plat_clock_gettime
#define clock_nanosleep plat_clock_nanosleep
#define nanosleep plat_nanosleep
#define localtime_r plat_localtime_r
#define gmtime_r plat_gmtime_r
int plat_clock_gettime(clockid_t clock, struct timespec *ts);
int plat_clock_nanosleep(clockid_t clock, int flags, const struct timespec *t, struct timespec *left);
int plat_nanosleep(const struct timespec *t, struct timespec *left);
struct tm *plat_localtime_r(const time_t *t, struct tm *out);
struct tm *plat_gmtime_r(const time_t *t, struct tm *out);
int timer_create(clockid_t clock, struct sigevent *ev, timer_t *out);
int timer_settime(timer_t t, int flags, const struct itimerspec *value, struct itimerspec *old);
int timer_delete(timer_t t);

#endif
