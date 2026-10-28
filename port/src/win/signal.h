/* Windows: the CRT's signal.h, and the POSIX signals the runtime uses (platform_win.c): sigaction with
 * SA_SIGINFO / SA_ONSTACK / SA_NODEFER, masks, sigaltstack, pthread_kill, timers' signals (SIGEV_THREAD_ID). A
 * signal sent to a thread interrupts it as on Linux, between two instructions of the program's own code (the
 * thread suspended, its registers read, the handler run on it, then the registers it left put back); while the
 * thread is in a system DLL's code it comes as soon as the thread is back. SIGSEGV is an access violation in the
 * program's own code. */
#include_next <signal.h>

#ifndef PLAT_WIN_SIGNAL_H
#define PLAT_WIN_SIGNAL_H
#include <stddef.h>
#include <sys/types.h>

#ifndef SIGALRM
#define SIGALRM 14
#endif
#define SIGPROF 27
#define SIGUSR1 30
#define SIGUSR2 31
#define PLAT_NSIG 32

typedef unsigned long plat_sigset_t;
#define sigset_t plat_sigset_t

typedef struct {
    int si_signo, si_errno, si_code;
    void *si_addr;
} siginfo_t;

#define SA_SIGINFO 0x00000004
#define SA_ONSTACK 0x08000000
#define SA_RESTART 0x10000000
#define SA_NODEFER 0x40000000
#define SA_RESETHAND 0x80000000

/* (struct sigaction and sigaction() both become plat_sigaction) */
#define sigaction plat_sigaction
struct plat_sigaction {
    union {
        void (*sa_handler)(int);
        void (*sa_sigaction)(int, siginfo_t *, void *);
    };
    sigset_t sa_mask;
    int sa_flags;
};
int plat_sigaction(int sig, const struct plat_sigaction *act, struct plat_sigaction *old);
#undef signal
#define signal plat_signal
void (*plat_signal(int sig, void (*fn)(int)))(int);

typedef struct {
    void *ss_sp;
    int ss_flags;
    size_t ss_size;
} stack_t;
#define SS_ONSTACK 1
#define SS_DISABLE 2
int sigaltstack(const stack_t *ss, stack_t *old);

#define SIG_BLOCK 0
#define SIG_UNBLOCK 1
#define SIG_SETMASK 2
static __inline__ int sigemptyset(sigset_t *s) { *s = 0; return 0; }
static __inline__ int sigfillset(sigset_t *s) { *s = ~0ul; return 0; }
static __inline__ int sigaddset(sigset_t *s, int n) { *s |= 1ul << (n & 31); return 0; }
static __inline__ int sigdelset(sigset_t *s, int n) { *s &= ~(1ul << (n & 31)); return 0; }
static __inline__ int sigismember(const sigset_t *s, int n) { return (*s >> (n & 31)) & 1; }
#undef pthread_sigmask
int pthread_sigmask(int how, const sigset_t *set, sigset_t *old);
int sigprocmask(int how, const sigset_t *set, sigset_t *old);

union sigval {
    int sival_int;
    void *sival_ptr;
};
#define SIGEV_SIGNAL 0
#define SIGEV_NONE 1
#define SIGEV_THREAD_ID 4
struct sigevent {
    union sigval sigev_value;
    int sigev_signo;
    int sigev_notify;
    union {
        int _tid; /* SIGEV_THREAD_ID: the thread (gettid) */
    } _sigev_un;
};

#endif
