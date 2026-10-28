/* The Windows platform layer (the 32-bit MinGW build: make -C port windows). The runtime is written against
 * Linux; here is what it uses of Linux that Windows lacks, under the same names, so the runtime's and the game's code
 * is the same on both (port/src/win/ holds the headers, before the MinGW ones on the Windows build's include path):
 *
 *   memory   mmap of anonymous memory and of a memfd_create'd file (a page-file section) at fixed addresses: PS1 RAM
 *            and its mirrors, the scratchpad, the recompiled code's stack (VirtualAlloc, MapViewOfFileEx)
 *   signals  POSIX signals to a thread: a thread sent one (a timer's, pthread_kill) is suspended, and when it is in
 *            the program's own code (not a system DLL's) its registers are read, the handler run on it (on its
 *            sigaltstack with SA_ONSTACK) with them as its ucontext, and the registers it leaves put back; in a DLL's
 *            code it gets the signal as soon as it is back (its sleeps here end early for it, EINTR, as on Linux).
 *            SIGSEGV: an access violation in the program's own code (a vectored exception handler)
 *   clocks   CLOCK_MONOTONIC (the performance counter), sleeps on high-resolution waitable timers, POSIX timers and
 *            setitimer (a thread each)
 *   threads  pthread_create / self / kill / getattr_np, signal masks
 *   files    the settings, saves, cards and logs in %APPDATA%\twine (XDG_CONFIG_HOME and XDG_DATA_HOME set to
 *            %APPDATA% when unset), or with a file named "portable" next to twine.exe in a twine folder there; files
 *            are binary as on Linux (no CR/LF translation); readlink("/proc/self/exe"), setenv, mkdir(path, mode),
 *            rename over an existing file
 *
 * The Linux build compiles this file to nothing. workbench/research/NOTES_windows.md has the design. */
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif
#include <windows.h>
#include <mmsystem.h>
#include <direct.h>
#include <errno.h>
#include <fcntl.h>
#include <float.h>
#include <io.h>
#include <stdio.h>
#include <string.h>
#include <execinfo.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h>
#include <ucontext.h>
#include <unistd.h>
#include "port.h"
#include "win/platform.h"

#ifndef CREATE_WAITABLE_TIMER_HIGH_RESOLUTION
#define CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002
#endif

static LARGE_INTEGER qpc_freq;
static int plat_debug; /* TWINE_PLAT_DEBUG=1: signals' deliveries on stderr */
static int inited;

/* ---- clocks ---- */

static u64 now_ns(void) {
    LARGE_INTEGER c;
    QueryPerformanceCounter(&c);
    return (u64)(c.QuadPart / qpc_freq.QuadPart) * 1000000000ull +
           (u64)(c.QuadPart % qpc_freq.QuadPart) * 1000000000ull / (u64)qpc_freq.QuadPart;
}

int plat_clock_gettime(clockid_t clock, struct timespec *ts) {
    u64 ns;
    if (clock == CLOCK_REALTIME) {
        FILETIME ft;
        ULARGE_INTEGER u;
        GetSystemTimePreciseAsFileTime(&ft);
        u.LowPart = ft.dwLowDateTime;
        u.HighPart = ft.dwHighDateTime;
        ns = (u.QuadPart - 116444736000000000ull) * 100; /* (since 1970) */
    } else
        ns = now_ns();
    ts->tv_sec = (time_t)(ns / 1000000000ull);
    ts->tv_nsec = (long)(ns % 1000000000ull);
    return 0;
}

struct tm *plat_localtime_r(const time_t *t, struct tm *out) {
    return localtime_s(out, t) ? NULL : out;
}

struct tm *plat_gmtime_r(const time_t *t, struct tm *out) {
    return gmtime_s(out, t) ? NULL : out;
}

/* ---- threads ---- */

typedef struct plat_thread {
    DWORD id;
    HANDLE h;
    volatile LONG blocked, pending; /* signal masks (bit n: signal n) */
    volatile LONG sent; /* signals sent while it was in a DLL's code: taken by the sender's next try or its sleep */
    volatile LONG in_exception; /* in the vectored exception handler */
    u32 resumed[11]; /* where the last handler returned to (the registers), whether it has run since */
    volatile int resumed_valid;
    stack_t alt;
    volatile LONG sleeping;
    HANDLE wake;  /* set: a signal for the thread ends its sleep */
    HANDLE timer; /* its sleeps' waitable timer */
    void *(*fn)(void *);
    void *arg;
    volatile u32 resume_esp, resume_eip; /* a handler's end: where the thread goes on (its stub) */
    u8 *stub;
} plat_thread;

#define THREADS 32
static plat_thread threads[THREADS];
static volatile LONG thread_count;
static CRITICAL_SECTION threads_lock, deliver_lock;
static DWORD tls_self;
static u8 *stubs; /* a handler's last jump, per thread: mov esp, [resume_esp]; jmp [resume_eip] */

static void lazy_init(void);

static HANDLE new_sleep_timer(void) {
    HANDLE h = CreateWaitableTimerExW(NULL, NULL, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
    return h ? h : CreateWaitableTimerW(NULL, FALSE, NULL);
}

static plat_thread *thread_new(void) {
    plat_thread *t;
    LONG i;
    EnterCriticalSection(&threads_lock);
    i = thread_count;
    if (i >= THREADS) {
        LeaveCriticalSection(&threads_lock);
        port_fatal("platform: too many threads\n");
    }
    t = &threads[i];
    memset(t, 0, sizeof(*t));
    t->wake = CreateEventW(NULL, FALSE, FALSE, NULL);
    t->timer = new_sleep_timer();
    t->stub = stubs + 16 * i;
    t->stub[0] = 0x8B, t->stub[1] = 0x25; /* mov esp, [&resume_esp] */
    *(u32 *)(t->stub + 2) = (u32)&t->resume_esp;
    t->stub[6] = 0xFF, t->stub[7] = 0x25; /* jmp [&resume_eip] */
    *(u32 *)(t->stub + 8) = (u32)&t->resume_eip;
    FlushInstructionCache(GetCurrentProcess(), t->stub, 16);
    MemoryBarrier();
    thread_count = i + 1;
    LeaveCriticalSection(&threads_lock);
    return t;
}

/* the calling thread's (registered the first time) */
static plat_thread *self(void) {
    plat_thread *t;
    lazy_init();
    if ((t = TlsGetValue(tls_self)))
        return t;
    t = thread_new();
    t->id = GetCurrentThreadId();
    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &t->h, 0, FALSE,
                    DUPLICATE_SAME_ACCESS);
    TlsSetValue(tls_self, t);
    return t;
}

static plat_thread *thread_by_id(DWORD id) {
    LONG i, n = thread_count;
    for (i = 0; i < n; i++)
        if (threads[i].id == id)
            return &threads[i];
    return NULL;
}

pid_t gettid(void) {
    self();
    return (pid_t)GetCurrentThreadId();
}

pthread_t pthread_self(void) {
    return self();
}

static DWORD WINAPI thread_start(void *p) {
    plat_thread *t = p;
    TlsSetValue(tls_self, t);
    t->fn(t->arg);
    return 0;
}

int pthread_create(pthread_t *out, const pthread_attr_t *attr, void *(*fn)(void *), void *arg) {
    plat_thread *t;
    (void)attr;
    lazy_init();
    t = thread_new();
    t->fn = fn;
    t->arg = arg;
    t->h = CreateThread(NULL, 0, thread_start, t, CREATE_SUSPENDED, &t->id);
    if (!t->h)
        return EAGAIN;
    *out = t;
    ResumeThread(t->h);
    return 0;
}

/* the thread's stack: [stackaddr, stackaddr + stacksize) as Linux gives it (the TEB's StackBase is its top) */
int pthread_getattr_np(pthread_t t, pthread_attr_t *attr) {
    NT_TIB *tib = (NT_TIB *)NtCurrentTeb();
    if (t != self())
        return EINVAL;
    attr->stackaddr = *(void **)((u8 *)tib + 0xE0C); /* (DeallocationStack: the reservation's start) */
    if (!attr->stackaddr)
        attr->stackaddr = tib->StackLimit;
    attr->stacksize = (u8 *)tib->StackBase - (u8 *)attr->stackaddr;
    return 0;
}

int pthread_attr_getstack(const pthread_attr_t *attr, void **addr, size_t *size) {
    *addr = attr->stackaddr;
    *size = attr->stacksize;
    return 0;
}

int pthread_attr_destroy(pthread_attr_t *attr) {
    (void)attr;
    return 0;
}

/* ---- signals ---- */

static struct {
    struct plat_sigaction act;
    int set;
} actions[PLAT_NSIG];

static u32 text_lo, text_hi;
/* PS1 KUSEG's low 8 MB: the image's first section (port/tools/win_link.py), no access */
#define KUSEG_END 0x00800000u
extern char __kuseg_start__[];
extern void plat_sig_entry(void);

#define SIGBIT(n) (1ul << ((n) & 31))

/* What a handler is called with on the thread (on its stack, or its alternate stack): the trampoline's
 * argument, the floating point state it keeps, the registers it restores (popad's order, then the flags). */
struct sigframe {
    u32 ret; /* the interrupted instruction (a backtrace from the handler goes on through it) */
    struct sigframe *self;
    u32 pad[2];
    u8 fx[512]; /* (fxsave: 16-aligned) */
    u32 rb[9];
    int sig;
    plat_thread *t;
    siginfo_t info;
    ucontext_t uc;
};
_Static_assert(__builtin_offsetof(struct sigframe, fx) == 16, "fx");
_Static_assert(__builtin_offsetof(struct sigframe, rb) == 528, "rb");

/* The thread arrives here with its stack pointer at a sigframe (deliver_async): the handler is run, then the
 * registers put back as the ucontext has them, the stack pointer and the program counter last, by the thread's
 * stub (whose address the TEB's ArbitraryUserPointer holds: a jump through it needs no register). */
__asm__(".section .text$plat_sig_b,\"x\"\n"
        ".globl plat_sig_entry\n"
        "plat_sig_entry:\n"
        "    cld\n"
        "    movl 4(%esp), %eax\n"
        "    fxsave 16(%eax)\n"
        "    subl $12, %esp\n"
        "    pushl %eax\n"
        "    call plat_sig_dispatch\n"
        "    popl %eax\n"
        "    fxrstor 16(%eax)\n"
        "    leal 528(%eax), %esp\n"
        "    popal\n"
        "    popfl\n"
        "    jmp *%fs:0x14\n"
        "    int3\n"
        ".text\n");

/* The code of a handler's return (plat_sig_dispatch after the handler, the entry's end, the stubs): a signal mustn't
 * come there, its return would take the place of this one's. Its grouped sections lie together, in name order. */
__asm__(".section .text$plat_sig_a,\"x\"\n"
        ".globl plat_sig_code\n"
        "plat_sig_code:\n"
        ".section .text$plat_sig_c,\"x\"\n"
        ".globl plat_sig_code_end\n"
        "plat_sig_code_end:\n"
        ".text\n");
extern char plat_sig_code[], plat_sig_code_end[];

/* fn(arg) on another stack (a signal handler on the thread's alternate stack, from the thread itself) */
void plat_call_on_stack(void *sp, void (*fn)(void *), void *arg);
__asm__(".text\n"
        ".globl plat_call_on_stack\n"
        "plat_call_on_stack:\n"
        "    pushl %ebp\n"
        "    movl %esp, %ebp\n"
        "    movl 8(%ebp), %eax\n"
        "    movl 12(%ebp), %ecx\n"
        "    movl 16(%ebp), %edx\n"
        "    movl %eax, %esp\n"
        "    andl $-16, %esp\n"
        "    subl $12, %esp\n"
        "    pushl %edx\n"
        "    call *%ecx\n"
        "    movl %ebp, %esp\n"
        "    popl %ebp\n"
        "    ret\n");

static void context_to_uc(const CONTEXT *c, ucontext_t *uc) {
    greg_t *g = uc->uc_mcontext.gregs;
    memset(uc, 0, sizeof(*uc));
    g[REG_GS] = c->SegGs, g[REG_FS] = c->SegFs, g[REG_ES] = c->SegEs, g[REG_DS] = c->SegDs;
    g[REG_EDI] = c->Edi, g[REG_ESI] = c->Esi, g[REG_EBP] = c->Ebp, g[REG_ESP] = c->Esp;
    g[REG_EBX] = c->Ebx, g[REG_EDX] = c->Edx, g[REG_ECX] = c->Ecx, g[REG_EAX] = c->Eax;
    g[REG_EIP] = c->Eip, g[REG_CS] = c->SegCs, g[REG_EFL] = c->EFlags, g[REG_UESP] = c->Esp, g[REG_SS] = c->SegSs;
}

static void uc_to_context(const ucontext_t *uc, CONTEXT *c) {
    const greg_t *g = uc->uc_mcontext.gregs;
    c->Edi = g[REG_EDI], c->Esi = g[REG_ESI], c->Ebp = g[REG_EBP], c->Esp = g[REG_ESP];
    c->Ebx = g[REG_EBX], c->Edx = g[REG_EDX], c->Ecx = g[REG_ECX], c->Eax = g[REG_EAX];
    c->Eip = g[REG_EIP], c->EFlags = g[REG_EFL];
}

/* code the thread may be interrupted in: the program's own (its .text and PS1 RAM's jumps), not a system DLL's,
 * nor the last steps of a handler's return */
static int interruptible(u32 eip) {
    if ((eip >= (u32)plat_sig_code && eip < (u32)plat_sig_code_end) || (eip >= (u32)stubs && eip < (u32)stubs + 16 * THREADS))
        return 0;
    return (eip >= text_lo && eip < text_hi) || (eip >= PS1_RAM_BASE && eip < PS1_RAM_BASE + PS1_RAM_SIZE);
}

static u32 ctx_reg(const CONTEXT *c, int r) {
    switch (r & 7) {
    case 0: return c->Eax;
    case 1: return c->Ecx;
    case 2: return c->Edx;
    case 3: return c->Ebx;
    case 4: return c->Esp;
    case 5: return c->Ebp;
    case 6: return c->Esi;
    default: return c->Edi;
    }
}

/* Whether the instruction the thread is at reads or writes PS1 KUSEG (or the null page): an access violation that
 * may already be under way, which a signal mustn't meet (the exception's dispatch would go on with the registers it
 * took before the handler's). Decoded as main.c's on_segv does; anything it can't tell may be one. */
static int at_low_access(const CONTEXT *c) {
    const u8 *ip = (const u8 *)c->Eip;
    u8 op, modrm, mod, rm;
    u32 ea;
    while (*ip == 0x66 || *ip == 0x67 || *ip == 0xF2 || *ip == 0xF3 || *ip == 0x2E || *ip == 0x3E || *ip == 0x26 ||
           *ip == 0x64 || *ip == 0x65 || *ip == 0x36 || *ip == 0xF0)
        ip++;
    op = *ip++;
    if ((op >= 0xA4 && op <= 0xA7) || (op >= 0xAA && op <= 0xAF)) /* (string instructions) */
        return c->Esi < KUSEG_END || c->Edi < KUSEG_END;
    if (op == 0x0F) {
        op = *ip++;
        if (op == 0x38 || op == 0x3A)
            ip++;
    }
    modrm = *ip++;
    mod = modrm >> 6;
    rm = modrm & 7;
    if (mod == 3)
        return 0;
    if (rm == 4) {
        u8 sib = *ip++;
        ea = ((sib >> 3) & 7) != 4 ? ctx_reg(c, (sib >> 3) & 7) << (sib >> 6) : 0;
        if ((sib & 7) == 5 && mod == 0) {
            ea += *(const u32 *)ip;
            ip += 4;
        } else
            ea += ctx_reg(c, sib & 7);
    } else if (rm == 5 && mod == 0) {
        ea = *(const u32 *)ip;
        ip += 4;
    } else
        ea = ctx_reg(c, rm);
    if (mod == 1)
        ea += (u32)(s32)(s8)*ip;
    else if (mod == 2)
        ea += *(const u32 *)ip;
    return ea < KUSEG_END;
}

/* whether the thread is where the last handler left it, all its registers as they were: it hasn't run since */
static int not_run_since(const plat_thread *t, const CONTEXT *c) {
    const u32 *r = t->resumed;
    return t->resumed_valid && c->Eip == r[10] && c->Esp == r[9] && c->Edi == r[0] && c->Esi == r[1] &&
           c->Ebp == r[2] && c->Ebx == r[4] && c->Edx == r[5] && c->Ecx == r[6] && c->Eax == r[7] &&
           ((c->EFlags ^ r[8]) & 0xCD5) == 0; /* (the arithmetic flags and DF: the system's own may differ) */
}

static void set_mask(plat_thread *t, LONG mask, const ucontext_t *at);

/* the handler of sig run on this thread, the signal (and its sa_mask) masked meanwhile */
static void run_handler(plat_thread *t, int sig, siginfo_t *info, ucontext_t *uc) {
    struct plat_sigaction a = actions[sig].act;
    LONG old = t->blocked;
    if (a.sa_handler == SIG_DFL || a.sa_handler == SIG_IGN)
        return;
    InterlockedExchange(&t->blocked, old | (LONG)a.sa_mask | (a.sa_flags & SA_NODEFER ? 0 : (LONG)SIGBIT(sig)));
    if (a.sa_flags & SA_RESETHAND)
        actions[sig].act.sa_handler = SIG_DFL;
    if (a.sa_flags & SA_SIGINFO)
        a.sa_sigaction(sig, info, uc);
    else
        a.sa_handler(sig);
    /* (the same signal come again while its handler ran: it would come with the same registers, the thread not having
     * run since; it is that one, as a timer's overrun on Linux) */
    InterlockedAnd(&t->pending, ~(LONG)SIGBIT(sig));
    set_mask(t, old, uc);
}

void plat_sig_dispatch(struct sigframe *f) __attribute__((section(".text$plat_sig_b")));
void plat_sig_dispatch(struct sigframe *f) {
    plat_thread *t = f->t;
    const greg_t *g = f->uc.uc_mcontext.gregs;
    run_handler(t, f->sig, &f->info, &f->uc);
    f->rb[0] = g[REG_EDI], f->rb[1] = g[REG_ESI], f->rb[2] = g[REG_EBP], f->rb[3] = 0;
    f->rb[4] = g[REG_EBX], f->rb[5] = g[REG_EDX], f->rb[6] = g[REG_ECX], f->rb[7] = g[REG_EAX];
    f->rb[8] = g[REG_EFL];
    t->resume_esp = g[REG_ESP];
    t->resume_eip = g[REG_EIP];
    memcpy(t->resumed, f->rb, sizeof(t->resumed)); /* (popad's order and the flags, then the stack and the pc) */
    t->resumed[9] = g[REG_ESP];
    t->resumed[10] = g[REG_EIP];
    t->resumed_valid = 1;
    ((NT_TIB *)NtCurrentTeb())->ArbitraryUserPointer = t->stub;
}

static int on_alt_stack(const plat_thread *t, u32 sp) {
    return t->alt.ss_sp && !(t->alt.ss_flags & SS_DISABLE) && sp > (u32)t->alt.ss_sp &&
           sp <= (u32)t->alt.ss_sp + t->alt.ss_size;
}

/* a signal for this thread, delivered by the thread itself (a mask lifted, pthread_kill to itself): as if it came at
 * `at` (the registers the handler sees) */
struct sync_call {
    plat_thread *t;
    int sig;
    siginfo_t info;
    ucontext_t uc;
};

static void sync_call(void *p) {
    struct sync_call *c = p;
    run_handler(c->t, c->sig, &c->info, &c->uc);
}

static void deliver_sync(plat_thread *t, int sig, const ucontext_t *at) {
    struct sync_call c;
    memset(&c, 0, sizeof(c));
    c.t = t;
    c.sig = sig;
    c.info.si_signo = sig;
    if (at)
        c.uc = *at;
    else {
        CONTEXT ctx;
        memset(&ctx, 0, sizeof(ctx));
        RtlCaptureContext(&ctx);
        context_to_uc(&ctx, &c.uc);
    }
    if ((actions[sig].act.sa_flags & SA_ONSTACK) && t->alt.ss_sp && !(t->alt.ss_flags & SS_DISABLE) &&
        !on_alt_stack(t, (u32)&c))
        plat_call_on_stack((u8 *)t->alt.ss_sp + t->alt.ss_size, sync_call, &c);
    else
        sync_call(&c);
}

/* the thread's mask set (by itself); signals pending that it no longer blocks are delivered now */
static void set_mask(plat_thread *t, LONG mask, const ucontext_t *at) {
    int sig;
    InterlockedExchange(&t->blocked, mask);
    for (;;) {
        LONG ready = t->pending & ~t->blocked;
        if (!ready)
            return;
        for (sig = 1; sig < PLAT_NSIG && !(ready & SIGBIT(sig)); sig++)
            ;
        InterlockedAnd(&t->pending, ~(LONG)SIGBIT(sig));
        deliver_sync(t, sig, at);
    }
}

/* A signal for another thread: 1 delivered (or left pending under its mask), 0 not yet: it is in a DLL's code (sent:
 * taken by a later try once it is back in the program's, or by itself on waking from a sleep here, which a signal
 * ends early as on Linux). */
static int deliver_async(plat_thread *t, int sig, int *tried) {
    CONTEXT ctx;
    struct sigframe *f;
    u32 sp;
    int r = 1;
    if (t->id == GetCurrentThreadId()) {
        if (t->blocked & SIGBIT(sig))
            InterlockedOr(&t->pending, (LONG)SIGBIT(sig));
        else
            deliver_sync(t, sig, NULL);
        return 1;
    }
    EnterCriticalSection(&deliver_lock);
    if (SuspendThread(t->h) == (DWORD)-1) {
        LeaveCriticalSection(&deliver_lock);
        return 1;
    }
    memset(&ctx, 0, sizeof(ctx));
    ctx.ContextFlags = CONTEXT_FULL;
    if (!GetThreadContext(t->h, &ctx))
        r = 0;
    else if (*tried && !(InterlockedAnd(&t->sent, ~(LONG)SIGBIT(sig)) & SIGBIT(sig)))
        ; /* (an earlier try's, taken by the thread meanwhile) */
    else if (t->blocked & SIGBIT(sig))
        InterlockedOr(&t->pending, (LONG)SIGBIT(sig));
    else if (actions[sig].act.sa_handler == SIG_DFL || actions[sig].act.sa_handler == SIG_IGN)
        ;
    else if (not_run_since(t, &ctx)) {
        if (plat_debug)
            fprintf(stderr, "plat: sig %d coalesced at %08lX\n", sig, (unsigned long)ctx.Eip);
    } /* (the thread hasn't run since the last handler: this one comes with that, as a timer's overrun on Linux) */
    else if (!interruptible(ctx.Eip) || t->in_exception || at_low_access(&ctx) || on_alt_stack(t, ctx.Esp)) {
        /* (on its alternate stack: the end of a handler, its mask already lifted; on Linux the return to the
         * interrupted code and the mask are one step) */
        InterlockedOr(&t->sent, (LONG)SIGBIT(sig));
        *tried = 1;
        if (t->sleeping)
            SetEvent(t->wake);
        r = 0;
    } else {
        if ((actions[sig].act.sa_flags & SA_ONSTACK) && t->alt.ss_sp && !(t->alt.ss_flags & SS_DISABLE) &&
            !on_alt_stack(t, ctx.Esp))
            sp = (u32)t->alt.ss_sp + t->alt.ss_size;
        else
            sp = ctx.Esp;
        sp = (sp - sizeof(struct sigframe)) & ~15u;
        f = (struct sigframe *)sp;
        f->ret = ctx.Eip;
        f->self = f;
        f->sig = sig;
        f->t = t;
        memset(&f->info, 0, sizeof(f->info));
        f->info.si_signo = sig;
        context_to_uc(&ctx, &f->uc);
        ctx.Esp = sp;
        ctx.Eip = (u32)plat_sig_entry;
        if (!SetThreadContext(t->h, &ctx))
            r = 0;
        if (plat_debug) {
            const greg_t *g = f->uc.uc_mcontext.gregs;
            u32 h = 2166136261u, *q;
            for (q = (u32 *)g[REG_ESP]; q < (u32 *)(g[REG_ESP] + 4096); q++) {
                MEMORY_BASIC_INFORMATION mi;
                if (((u32)q & 0xFFF) == 0 && (!VirtualQuery(q, &mi, sizeof(mi)) || mi.State != MEM_COMMIT))
                    break;
                h = (h ^ *q) * 16777619u;
            }
            fprintf(stderr, "plat: %llu sig %d at %08X esp %08X regs %08X %08X %08X %08X %08X %08X fl %08X stack %08X\n",
                    (unsigned long long)now_ns() / 1000, sig, g[REG_EIP], g[REG_ESP], g[REG_EAX], g[REG_ECX], g[REG_EDX],
                    g[REG_EBX], g[REG_ESI], g[REG_EDI], g[REG_EFL], h);
        }
    }
    ResumeThread(t->h);
    LeaveCriticalSection(&deliver_lock);
    return r;
}

/* a short wait for a thread that can't take its signal now */
static void retry_pause(HANDLE timer) {
    LARGE_INTEGER due;
    due.QuadPart = -1000; /* 100 us */
    if (timer && SetWaitableTimer(timer, &due, 0, NULL, NULL, FALSE))
        WaitForSingleObject(timer, 10);
    else
        Sleep(0);
}

/* deliver, trying again until the thread takes it (or `until` has passed: 0 never) */
static int deliver_retry(plat_thread *t, int sig, HANDLE timer, u64 until) {
    int tried = 0;
    while (!deliver_async(t, sig, &tried)) {
        if (until && now_ns() >= until)
            return 0;
        retry_pause(timer);
    }
    return 1;
}

/* the signals sent to this thread while it was in a DLL's code, taken now (from its sleeps) */
static int take_sent(plat_thread *t) {
    LONG bits = InterlockedExchange(&t->sent, 0);
    int sig, n = 0;
    for (sig = 1; sig < PLAT_NSIG; sig++)
        if (bits & SIGBIT(sig)) {
            n++;
            if (t->blocked & SIGBIT(sig))
                InterlockedOr(&t->pending, (LONG)SIGBIT(sig));
            else
                deliver_sync(t, sig, NULL);
        }
    return n;
}

int pthread_kill(pthread_t t, int sig) {
    HANDLE timer;
    if (sig <= 0 || sig >= PLAT_NSIG)
        return sig ? EINVAL : 0;
    timer = new_sleep_timer();
    deliver_retry(t, sig, timer, now_ns() + 1000000000ull);
    CloseHandle(timer);
    return 0;
}

int pthread_sigmask(int how, const sigset_t *set, sigset_t *old) {
    plat_thread *t = self();
    LONG m = t->blocked;
    if (old)
        *old = (sigset_t)m;
    if (!set)
        return 0;
    if (how == SIG_BLOCK)
        m |= (LONG)*set;
    else if (how == SIG_UNBLOCK)
        m &= ~(LONG)*set;
    else
        m = (LONG)*set;
    set_mask(t, m, NULL);
    return 0;
}

int sigprocmask(int how, const sigset_t *set, sigset_t *old) {
    return pthread_sigmask(how, set, old);
}

int sigaltstack(const stack_t *ss, stack_t *old) {
    plat_thread *t = self();
    if (old)
        *old = t->alt;
    if (ss)
        t->alt = *ss;
    return 0;
}

#undef signal
static int crt_signal(int sig) { /* (what the C runtime raises itself: Ctrl+C, termination, abort) */
    return sig == SIGINT || sig == SIGTERM || sig == SIGBREAK || sig == SIGABRT || sig == SIGFPE || sig == SIGILL;
}

int plat_sigaction(int sig, const struct plat_sigaction *act, struct plat_sigaction *old) {
    if (sig <= 0 || sig >= PLAT_NSIG) {
        errno = EINVAL;
        return -1;
    }
    lazy_init();
    if (old)
        *old = actions[sig].act;
    if (act) {
        actions[sig].act = *act;
        actions[sig].set = 1;
        if (crt_signal(sig) && !(act->sa_flags & SA_SIGINFO))
            signal(sig, act->sa_handler);
    }
    return 0;
}

void (*plat_signal(int sig, void (*fn)(int)))(int) {
    struct plat_sigaction a, old;
    memset(&a, 0, sizeof(a));
    a.sa_handler = fn;
    a.sa_flags = SA_RESTART;
    if (plat_sigaction(sig, &a, &old))
        return SIG_ERR;
    return old.sa_handler;
}

/* SIGSEGV: an access violation in the program's own code, its handler run there and then (the registers it leaves
 * are the ones the thread goes on with); left to its default (a crash) when it puts that back */
static LONG CALLBACK on_exception(EXCEPTION_POINTERS *ep) {
    CONTEXT *c = ep->ContextRecord;
    struct plat_sigaction a = actions[SIGSEGV].act;
    siginfo_t info;
    ucontext_t uc;
    plat_thread *t;
    u32 fault = ep->ExceptionRecord->NumberParameters > 1 ? (u32)ep->ExceptionRecord->ExceptionInformation[1] : ~0u;
    /* (the program's own code, or any code's access to PS1 KUSEG, which nothing else uses: a C runtime's memcpy given
     * such a pointer by the game) */
    if (ep->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION ||
        !(interruptible(c->Eip) || fault < KUSEG_END) || a.sa_handler == SIG_DFL || a.sa_handler == SIG_IGN)
        return EXCEPTION_CONTINUE_SEARCH;
    memset(&info, 0, sizeof(info));
    info.si_signo = SIGSEGV;
    info.si_addr = (void *)fault;
    context_to_uc(c, &uc);
    t = self();
    InterlockedIncrement(&t->in_exception);
    if (a.sa_flags & SA_SIGINFO)
        a.sa_sigaction(SIGSEGV, &info, &uc);
    else
        a.sa_handler(SIGSEGV);
    InterlockedDecrement(&t->in_exception);
    if (actions[SIGSEGV].act.sa_handler == SIG_DFL)
        return EXCEPTION_CONTINUE_SEARCH;
    uc_to_context(&uc, c);
    return EXCEPTION_CONTINUE_EXECUTION;
}

/* a crash: said in the log, and the process ended (no error dialog) */
static LONG CALLBACK on_crash(EXCEPTION_POINTERS *ep) {
    port_log("crash: exception %08lX at %08lX (access to %08lX)\n", ep->ExceptionRecord->ExceptionCode,
             (unsigned long)ep->ContextRecord->Eip,
             ep->ExceptionRecord->NumberParameters > 1 ? (unsigned long)ep->ExceptionRecord->ExceptionInformation[1]
                                                       : 0ul);
    fprintf(stderr, "crash: exception %08lX at %08lX\n", ep->ExceptionRecord->ExceptionCode,
            (unsigned long)ep->ContextRecord->Eip);
    fflush(NULL);
    if (getenv("TWINE_CRASH_DUMPS")) { /* (a minidump there: the USB test kit's results, port/usbkit) */
        typedef struct {
            DWORD ThreadId;
            EXCEPTION_POINTERS *ExceptionPointers;
            BOOL ClientPointers;
        } DumpInfo;
        typedef BOOL(WINAPI * WriteDump)(HANDLE, DWORD, HANDLE, int, DumpInfo *, void *, void *);
        HMODULE dbghelp = LoadLibraryA("dbghelp.dll");
        WriteDump write = dbghelp ? (WriteDump)(void *)GetProcAddress(dbghelp, "MiniDumpWriteDump") : NULL;
        char path[MAX_PATH * 2];
        HANDLE f;
        snprintf(path, sizeof(path), "%s/twine-%lu.dmp", getenv("TWINE_CRASH_DUMPS"), GetCurrentProcessId());
        f = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (write && f != INVALID_HANDLE_VALUE) {
            DumpInfo di = {GetCurrentThreadId(), ep, FALSE};
            write(GetCurrentProcess(), GetCurrentProcessId(), f, 0x1 /* MiniDumpWithDataSegs */, &di, NULL, NULL);
        }
        if (f != INVALID_HANDLE_VALUE)
            CloseHandle(f);
    }
    TerminateProcess(GetCurrentProcess(), 3);
    return EXCEPTION_EXECUTE_HANDLER;
}

/* ---- timers ---- */

typedef struct plat_timer {
    plat_thread *target;
    int sig;
    HANDLE thread, timer, changed;
    CRITICAL_SECTION lock;
    u64 due, period; /* (ns; due 0: disarmed) */
    volatile LONG deleted;
} plat_timer;

static void wait_until(plat_timer *tm, u64 due) {
    LARGE_INTEGER d;
    u64 now = now_ns();
    HANDLE hs[2];
    if (due <= now)
        return;
    d.QuadPart = -(LONGLONG)((due - now) / 100);
    if (!d.QuadPart)
        d.QuadPart = -1;
    hs[0] = tm->timer;
    hs[1] = tm->changed;
    if (SetWaitableTimer(tm->timer, &d, 0, NULL, NULL, FALSE))
        WaitForMultipleObjects(2, hs, FALSE, INFINITE);
    else
        WaitForSingleObject(tm->changed, (DWORD)((due - now) / 1000000));
}

/* A timer's thread: its signal at each expiry, each delivered (a thread in a DLL's code takes it when it is back;
 * expiries meanwhile are owed, at most 8). */
static DWORD WINAPI timer_thread(void *p) {
    plat_timer *tm = p;
    u32 owed = 0;
    int tried = 0;
    while (!tm->deleted) {
        u64 due, period, now;
        EnterCriticalSection(&tm->lock);
        due = tm->due, period = tm->period;
        LeaveCriticalSection(&tm->lock);
        if (!due) {
            WaitForSingleObject(tm->changed, INFINITE);
            continue;
        }
        if ((now = now_ns()) < due) {
            wait_until(tm, due);
            continue;
        }
        EnterCriticalSection(&tm->lock);
        if (tm->due == due) {
            if (period)
                while (tm->due <= now) {
                    tm->due += period;
                    owed++;
                }
            else {
                tm->due = 0;
                owed++;
            }
        }
        due = tm->due;
        LeaveCriticalSection(&tm->lock);
        if (owed > 8)
            owed = 8;
        while (owed && !tm->deleted) {
            if (deliver_async(tm->target, tm->sig, &tried)) {
                owed--;
                tried = 0;
            } else if (due && now_ns() >= due)
                break;
            else
                retry_pause(tm->timer);
        }
    }
    return 0;
}

static plat_timer *timer_new(plat_thread *target, int sig) {
    plat_timer *tm = calloc(1, sizeof(*tm));
    tm->target = target;
    tm->sig = sig;
    tm->timer = new_sleep_timer();
    tm->changed = CreateEventW(NULL, FALSE, FALSE, NULL);
    InitializeCriticalSection(&tm->lock);
    tm->thread = CreateThread(NULL, 0, timer_thread, tm, 0, NULL);
    SetThreadPriority(tm->thread, THREAD_PRIORITY_TIME_CRITICAL);
    return tm;
}

static void timer_arm(plat_timer *tm, u64 value, u64 period) {
    EnterCriticalSection(&tm->lock);
    tm->due = value ? now_ns() + value : 0;
    tm->period = period;
    LeaveCriticalSection(&tm->lock);
    SetEvent(tm->changed);
}

int timer_create(clockid_t clock, struct sigevent *ev, timer_t *out) {
    plat_thread *target = NULL;
    (void)clock;
    lazy_init();
    if (ev && ev->sigev_notify == SIGEV_THREAD_ID)
        target = thread_by_id((DWORD)ev->_sigev_un._tid);
    if (!target)
        target = self();
    *out = timer_new(target, ev ? ev->sigev_signo : SIGALRM);
    return 0;
}

static u64 ts_ns(const struct timespec *ts) {
    return (u64)ts->tv_sec * 1000000000ull + (u64)ts->tv_nsec;
}

int timer_settime(timer_t t, int flags, const struct itimerspec *value, struct itimerspec *old) {
    u64 v = ts_ns(&value->it_value);
    if (old)
        memset(old, 0, sizeof(*old));
    if (flags & TIMER_ABSTIME)
        v = v > now_ns() ? v - now_ns() : 1;
    timer_arm(t, v, ts_ns(&value->it_interval));
    return 0;
}

int timer_delete(timer_t t) {
    t->deleted = 1;
    timer_arm(t, 0, 0);
    return 0;
}

/* setitimer: ITIMER_REAL SIGALRM, ITIMER_PROF SIGPROF (on the host's clock, not CPU time), to the calling thread */
int setitimer(int which, const struct itimerval *value, struct itimerval *old) {
    static plat_timer *timers[3];
    u64 v = (u64)value->it_value.tv_sec * 1000000000ull + (u64)value->it_value.tv_usec * 1000ull;
    u64 p = (u64)value->it_interval.tv_sec * 1000000000ull + (u64)value->it_interval.tv_usec * 1000ull;
    if (old)
        memset(old, 0, sizeof(*old));
    if (which < 0 || which > 2 || which == ITIMER_VIRTUAL) {
        errno = EINVAL;
        return -1;
    }
    if (!timers[which])
        timers[which] = timer_new(self(), which == ITIMER_PROF ? SIGPROF : SIGALRM);
    timer_arm(timers[which], v, p);
    return 0;
}

/* ---- sleeps (a signal for the thread ends them: EINTR) ---- */

static int sleep_until(u64 due) {
    plat_thread *t = self();
    LARGE_INTEGER d;
    HANDLE hs[2];
    DWORD r;
    u64 now = now_ns();
    if (t->sent && take_sent(t))
        return EINTR;
    if (due <= now)
        return 0;
    d.QuadPart = -(LONGLONG)((due - now) / 100);
    if (!d.QuadPart)
        d.QuadPart = -1;
    if (!SetWaitableTimer(t->timer, &d, 0, NULL, NULL, FALSE)) {
        Sleep((DWORD)((due - now) / 1000000));
        return 0;
    }
    hs[0] = t->timer;
    hs[1] = t->wake;
    InterlockedExchange(&t->sleeping, 1);
    if (t->sent) /* (sent as it came here) */
        SetEvent(t->wake);
    r = WaitForMultipleObjects(2, hs, FALSE, INFINITE);
    InterlockedExchange(&t->sleeping, 0);
    if (r == WAIT_OBJECT_0 + 1) {
        CancelWaitableTimer(t->timer);
        take_sent(t);
        return EINTR;
    }
    return take_sent(t) ? EINTR : 0;
}

int plat_clock_nanosleep(clockid_t clock, int flags, const struct timespec *ts, struct timespec *left) {
    u64 now = now_ns(), due;
    int r;
    if (flags & TIMER_ABSTIME) {
        due = ts_ns(ts);
        if (clock == CLOCK_REALTIME) {
            struct timespec rt;
            u64 rtn;
            plat_clock_gettime(CLOCK_REALTIME, &rt);
            rtn = ts_ns(&rt);
            due = due > rtn ? now + (due - rtn) : now;
        }
    } else
        due = now + ts_ns(ts);
    r = sleep_until(due);
    if (r && left && !(flags & TIMER_ABSTIME)) {
        u64 n = now_ns(), l = due > n ? due - n : 0;
        left->tv_sec = (time_t)(l / 1000000000ull);
        left->tv_nsec = (long)(l % 1000000000ull);
    }
    return r;
}

int plat_nanosleep(const struct timespec *ts, struct timespec *left) {
    int r = plat_clock_nanosleep(CLOCK_MONOTONIC, 0, ts, left);
    if (r) {
        errno = r;
        return -1;
    }
    return 0;
}

int plat_usleep(unsigned int us) {
    if (sleep_until(now_ns() + (u64)us * 1000ull)) {
        errno = EINTR;
        return -1;
    }
    return 0;
}

unsigned int plat_sleep(unsigned int s) {
    u64 due = now_ns() + (u64)s * 1000000000ull;
    if (sleep_until(due)) {
        u64 n = now_ns();
        return n < due ? (unsigned)((due - n + 999999999ull) / 1000000000ull) : 0;
    }
    return 0;
}

/* ---- memory ---- */

#define MEMFDS 8
#define MEMFD_BASE 0x4000
static HANDLE memfds[MEMFDS];

int memfd_create(const char *name, unsigned int flags) {
    int i;
    (void)name, (void)flags;
    for (i = 0; i < MEMFDS; i++)
        if (!memfds[i]) {
            memfds[i] = INVALID_HANDLE_VALUE; /* (its section comes with ftruncate) */
            return MEMFD_BASE + i;
        }
    errno = EMFILE;
    return -1;
}

static HANDLE *memfd(int fd) {
    return fd >= MEMFD_BASE && fd < MEMFD_BASE + MEMFDS && memfds[fd - MEMFD_BASE] ? &memfds[fd - MEMFD_BASE] : NULL;
}

int plat_ftruncate(int fd, long long size) {
    HANDLE *m = memfd(fd);
    if (!m)
        return _chsize_s(fd, size) ? -1 : 0;
    if (*m != INVALID_HANDLE_VALUE) {
        errno = EINVAL;
        return -1;
    }
    *m = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE | SEC_COMMIT, (DWORD)(size >> 32),
                            (DWORD)size, NULL);
    if (!*m) {
        *m = INVALID_HANDLE_VALUE;
        errno = ENOMEM;
        return -1;
    }
    return 0;
}

static DWORD page_protect(int prot) {
    if (prot & PROT_EXEC)
        return prot & PROT_WRITE ? PAGE_EXECUTE_READWRITE : prot & PROT_READ ? PAGE_EXECUTE_READ : PAGE_EXECUTE;
    return prot & PROT_WRITE ? PAGE_READWRITE : prot & PROT_READ ? PAGE_READONLY : PAGE_NOACCESS;
}

void *mmap(void *addr, size_t len, int prot, int flags, int fd, long long offset) {
    void *p;
    lazy_init();
    if (flags & MAP_ANONYMOUS) {
        p = VirtualAlloc((flags & (MAP_FIXED | MAP_FIXED_NOREPLACE)) ? addr : NULL, len, MEM_RESERVE | MEM_COMMIT,
                         page_protect(prot));
        if (!p && addr && !(flags & (MAP_FIXED | MAP_FIXED_NOREPLACE)))
            p = VirtualAlloc(NULL, len, MEM_RESERVE | MEM_COMMIT, page_protect(prot));
    } else {
        HANDLE *m = memfd(fd);
        DWORD access = (prot & PROT_WRITE ? FILE_MAP_WRITE : FILE_MAP_READ) | (prot & PROT_EXEC ? FILE_MAP_EXECUTE : 0);
        if (!m || *m == INVALID_HANDLE_VALUE) { /* (only memfd files: the runtime maps no other) */
            errno = EBADF;
            return MAP_FAILED;
        }
        p = MapViewOfFileEx(*m, access, (DWORD)(offset >> 32), (DWORD)offset, len,
                            (flags & (MAP_FIXED | MAP_FIXED_NOREPLACE)) ? addr : NULL);
    }
    if (!p) {
        errno = addr ? EEXIST : ENOMEM;
        return MAP_FAILED;
    }
    return p;
}

int munmap(void *addr, size_t len) {
    MEMORY_BASIC_INFORMATION mi;
    (void)len;
    if (!VirtualQuery(addr, &mi, sizeof(mi)))
        return -1;
    if (mi.Type == MEM_MAPPED)
        return UnmapViewOfFile(addr) ? 0 : -1;
    return VirtualFree(addr, 0, MEM_RELEASE) ? 0 : -1;
}

int mprotect(void *addr, size_t len, int prot) {
    DWORD old;
    return VirtualProtect(addr, len, page_protect(prot), &old) ? 0 : -1;
}

/* ---- files and the environment ---- */

static char exe_path[MAX_PATH * 2];

/* readlink: /proc/self/exe is the program's path (with /, as main.c and hdmovie.c look for the last one) */
ssize_t readlink(const char *path, char *buf, size_t size) {
    size_t n;
    if (strcmp(path, "/proc/self/exe")) {
        errno = EINVAL;
        return -1;
    }
    lazy_init();
    n = strlen(exe_path);
    if (n > size)
        n = size;
    memcpy(buf, exe_path, n);
    return (ssize_t)n;
}

#undef rename
int plat_rename(const char *from, const char *to) {
    if (MoveFileExA(from, to, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
        return 0;
    errno = EACCES;
    return -1;
}

int setenv(const char *name, const char *value, int overwrite) {
    if (!overwrite && getenv(name))
        return 0;
    return _putenv_s(name, value) ? -1 : 0;
}

int unsetenv(const char *name) {
    return _putenv_s(name, "") ? -1 : 0;
}

/* ---- backtraces (the frame pointers' chain) ---- */

int backtrace(void **at, int n) {
    return RtlCaptureStackBackTrace(1, n > 62 ? 62 : n, at, NULL);
}

void backtrace_symbols_fd(void *const *at, int n, int fd) {
    int i;
    for (i = 0; i < n; i++) {
        char line[160], name[MAX_PATH] = "?";
        HMODULE m = NULL;
        const char *base;
        int len;
        if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                               (const char *)at[i], &m))
            GetModuleFileNameA(m, name, sizeof(name));
        base = strrchr(name, '\\') ? strrchr(name, '\\') + 1 : name;
        len = snprintf(line, sizeof(line), "%s(+0x%lx) [0x%08lx]\n", base, (unsigned long)((u8 *)at[i] - (u8 *)m),
                       (unsigned long)at[i]);
        if (len > 0 && _write(fd, line, (unsigned)len) < 0)
            break;
    }
}

/* ---- the program's memory a save state holds ---- */

/* (port/src/win/first.c and last.c, linked before and after the port's own objects: their data and BSS lie between) */
extern char plat_data_first[], plat_data_last[], plat_bss_first[], plat_bss_last[];

int plat_state_ranges(char **at, unsigned *size, int n) {
    int k = 0;
    if (k < n) {
        at[k] = plat_data_first;
        size[k++] = (unsigned)(plat_data_last - plat_data_first);
    }
    if (k < n) {
        at[k] = plat_bss_first;
        size[k++] = (unsigned)(plat_bss_last - plat_bss_first);
    }
    return k;
}

/* ---- set-up ---- */

static int file_exists(const char *path) {
    DWORD a = GetFileAttributesA(path);
    return a != INVALID_FILE_ATTRIBUTES;
}

/* Where the player's files go: XDG_CONFIG_HOME and XDG_DATA_HOME (twine.ini; cards, save states, endless mode's
 * records) are %APPDATA% unless set (the test runs set their own; under Wine as TWINE_XDG_CONFIG_HOME etc.), so all in %APPDATA%\twine; a file named portable
 * next to twine.exe keeps them in a twine folder there instead. Started from its own folder (as from Explorer), the
 * game works in that twine folder, where its log (port.log) goes. */
static void files_init(void) {
    char dir[MAX_PATH * 2], base[MAX_PATH * 2], path[MAX_PATH * 2 + 32], cwd[MAX_PATH * 2];
    const char *appdata = getenv("APPDATA");
    char *slash;
    int portable;
    snprintf(dir, sizeof(dir), "%s", exe_path);
    if ((slash = strrchr(dir, '/')))
        *slash = 0;
    snprintf(path, sizeof(path), "%s/portable", dir);
    portable = file_exists(path);
    if (portable || !appdata || !*appdata)
        snprintf(base, sizeof(base), "%s", dir);
    else
        snprintf(base, sizeof(base), "%s", appdata);
    { /* (TWINE_XDG_*_HOME first: what tests give under Wine, which hides the host's XDG_* from Windows programs) */
        static const char *const names[] = {"XDG_CONFIG_HOME", "XDG_DATA_HOME", "XDG_STATE_HOME"};
        int i;
        for (i = 0; i < 3; i++) {
            char alt[64];
            snprintf(alt, sizeof(alt), "TWINE_%s", names[i]);
            if (getenv(alt) && *getenv(alt))
                setenv(names[i], getenv(alt), 0);
            setenv(names[i], base, 0);
        }
    }
    if (_getcwd(cwd, sizeof(cwd))) {
        char *c;
        for (c = cwd; *c; c++)
            if (*c == '\\')
                *c = '/';
        if (!_stricmp(cwd, dir) && !getenv("TWINE_HEADLESS")) {
            snprintf(path, sizeof(path), "%s/twine", base);
            _mkdir(path);
            _chdir(path);
        }
    }
}

static void lazy_init(void) {
    static volatile LONG once;
    if (inited)
        return;
    if (InterlockedCompareExchange(&once, 1, 0)) {
        while (!inited)
            Sleep(0);
        return;
    }
    QueryPerformanceFrequency(&qpc_freq);
    plat_debug = getenv("TWINE_PLAT_DEBUG") != NULL;
    InitializeCriticalSection(&threads_lock);
    InitializeCriticalSection(&deliver_lock);
    tls_self = TlsAlloc();
    stubs = VirtualAlloc(NULL, 16 * THREADS, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    text_lo = (u32)__executable_start;
    text_hi = (u32)etext;
    {
        char *c;
        DWORD n = GetModuleFileNameA(NULL, exe_path, sizeof(exe_path) - 1);
        exe_path[n] = 0;
        for (c = exe_path; *c; c++)
            if (*c == '\\')
                *c = '/';
    }
    inited = 1;
}

/* (before main.c's constructor(101), which maps PS1 memory: first of all) */
#pragma GCC diagnostic ignored "-Wprio-ctor-dtor"
__attribute__((constructor(100))) void plat_init(void) {
    static int done;
    if (done)
        return;
    done = 1;
    lazy_init();
    /* files as on Linux: no text mode's CR/LF translation */
    _set_fmode(_O_BINARY);
    _setmode(_fileno(stdout), _O_BINARY);
    _setmode(_fileno(stderr), _O_BINARY);
    timeBeginPeriod(1);
    { /* the x87 unit's precision as on Linux, 64-bit mantissas (the C runtime sets 53): float code's results the same */
        unsigned int cw;
        _controlfp_s(&cw, _PC_64, _MCW_PC);
    }
    self(); /* (the game's thread) */
    {
        DWORD old;
        if (!VirtualProtect(__kuseg_start__, KUSEG_END - (u32)__kuseg_start__, PAGE_NOACCESS, &old))
            fprintf(stderr, "platform: KUSEG left accessible (%lu)\n", GetLastError());
    }
    AddVectoredExceptionHandler(1, on_exception);
    SetUnhandledExceptionFilter(on_crash);
    if (getenv("TWINE_HEADLESS"))
        SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
    files_init();
}

/* The program has no console (a window of its own; -mwindows): the C runtime starts it here */
int main(int argc, char **argv);
int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show) {
    (void)inst, (void)prev, (void)cmd, (void)show;
    return main(__argc, __argv);
}

#endif /* _WIN32 */

