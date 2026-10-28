#ifndef PLAT_WIN_PTHREAD_H
#define PLAT_WIN_PTHREAD_H
/* Windows: the few POSIX threads calls the runtime makes, on Windows threads (platform_win.c), in place of
 * MinGW's winpthreads (never linked): a thread a signal can be sent to (pthread_kill), its stack (pthread_getattr_np),
 * its signal mask. */
#include <stddef.h>
#include <signal.h>
#include <time.h>

typedef struct plat_thread *pthread_t;
typedef struct {
    void *stackaddr;
    size_t stacksize;
} pthread_attr_t;

pthread_t pthread_self(void);
int pthread_create(pthread_t *t, const pthread_attr_t *attr, void *(*fn)(void *), void *arg);
int pthread_kill(pthread_t t, int sig);
int pthread_getattr_np(pthread_t t, pthread_attr_t *attr);
int pthread_attr_getstack(const pthread_attr_t *attr, void **addr, size_t *size);
int pthread_attr_destroy(pthread_attr_t *attr);
/* a thread's CPU time as a clock for clock_gettime (GetThreadTimes: user and kernel time, in 100 ns, updated at the
 * scheduler's tick) */
int pthread_getcpuclockid(pthread_t t, clockid_t *clock);

#endif
