/* Interrupts and events. On the PS1 the vertical blank interrupt (60 Hz) runs the game's handlers between
 * any two instructions; here a 60 Hz timer signal does the same on the game's thread, so the game's busy
 * waits on its vblank counter and its own Enter/ExitCriticalSection protection work as written.
 *
 * Runtime code that isn't safe to interrupt (stdio, SDL, ...) runs inside RT_ENTER/RT_LEAVE: a vblank that
 * arrives meanwhile is held and delivered when the runtime returns to game code. */
#define _GNU_SOURCE
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <ucontext.h>
#include <unistd.h>
#include "port.h"
#include "module.h"

volatile int port_rt_depth;            /* > 0 while runtime code runs */
static volatile int irq_masked;        /* EnterCriticalSection */
static volatile int irq_pending;       /* vblanks held back */
static volatile u32 vblank_count;
static volatile int in_irq;

typedef s32 (*EventFn)(void);
typedef struct {
    u32 cls, spec, mode;
    EventFn fn;
    int open, enabled, delivered;
    const PortModule *owner; /* the executable that opened it */
} Event;
#define EVENTS 32
static Event events[EVENTS];
static u32 rcnt_running; /* root counters started with StartRCnt (bit n = counter n) */
static void (*vsync_callback)(void);
static void (*vsync_callbacks[8])(void); /* VSyncCallbacks' channels */

static void run_vblank(void) {
    int i;
    in_irq = 1;
    vblank_count++;
    /* RCntCNT3 = the vblank root counter; EvMdINTR (0x1000) = call the handler from the interrupt. Only the
     * running executable's: on the console the boot stub's vblank event (opened again before each executable
     * it starts, never closed) doesn't run while the front end or a level does (checked in DuckStation:
     * LEVEL02's handler runs every vblank, the stub's never). Here it did, once for each one it opened, and
     * the game, counting vblanks with both, thought it ran at 5-7 frames a second instead of 20, so every
     * speed it scales by that (movement, enemies, timers) was 3-4 times too great. */
    for (i = 0; i < EVENTS; i++) {
        Event *e = &events[i];
        if (e->open && e->enabled && e->cls == 0xF2000003 && (rcnt_running & 8) && e->fn && (e->mode & 0x1000) &&
            e->owner == port_module)
            e->fn();
    }
    if (vsync_callback)
        vsync_callback();
    for (i = 0; i < 8; i++)
        if (vsync_callbacks[i])
            vsync_callbacks[i]();
    port_pad_vblank();
    port_cd_irq();
    port_dma_irq();
    in_irq = 0;
}

static void deliver_pending(void) {
    while (irq_pending && !irq_masked && !port_rt_depth && !in_irq) {
        irq_pending--;
        run_vblank();
    }
}

static void on_timer(int sig) {
    (void)sig;
    if (irq_masked || port_rt_depth || in_irq) {
        if (irq_pending < 8)
            irq_pending++;
        return;
    }
    run_vblank();
}

/* ---- the game's clock: the host's, or game time (TWINE_TURBO) ----
 * With TWINE_TURBO=1 (headless runs only: tests) no timer gives vblanks. The clock stands still while the game works and
 * moves on only where it waits: to the next vblank in port_wait_vblank, by a wait loop's pass in port_spin, by an
 * extra drawing's fixed cost and to its due time in smooth motion (interp.c). Each vblank whose time the clock
 * passes comes as the timer's would (held while the game masks it or runtime code runs, as many as 8). So the
 * game sees the frames, vblanks, controls and sound it would on an infinitely fast machine, as fast as this one
 * can run them. The sound chip's samples (spu.c), the CD drive's and the root counters' time (cdrom.c, io.c) and
 * the port's on-screen messages (video.c) all keep to this clock.
 * TWINE_TEST_VBLANKS=<n> (either clock): the run ends (exit 0) at the first wait once n vblanks have come, its
 * last log line "test: the run's <n> vblanks are over". */
#define VBLANK_PERIOD_NS 16683333ull /* NTSC: 59.94 Hz */
int port_turbo PORT_NOSAVE;
static int irq_started PORT_NOSAVE;
static u64 virt_ns PORT_NOSAVE, virt_vblank_due PORT_NOSAVE;
static u32 polls PORT_NOSAVE, poll_moves PORT_NOSAVE;
static u32 end_vblanks PORT_NOSAVE;

__attribute__((constructor)) static void clock_init(void) {
    const char *t = getenv("TWINE_TURBO"), *e = getenv("TWINE_TEST_VBLANKS");
    port_turbo = t && atoi(t) && getenv("TWINE_HEADLESS");
    end_vblanks = e ? (u32)strtoul(e, NULL, 10) : 0;
}

static u64 host_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (u64)ts.tv_sec * 1000000000ull + (u64)ts.tv_nsec;
}

u64 port_clock_ns(void) {
    return port_turbo ? virt_ns : host_ns();
}

/* turbo: the clock on to `to`, the vblanks due on the way delivered (or held) as the timer's signal would */
static volatile int in_advance PORT_NOSAVE;
static volatile u32 progress PORT_NOSAVE, spins PORT_NOSAVE, spin_at PORT_NOSAVE, kicks PORT_NOSAVE, kick_at PORT_NOSAVE;
static u32 spins_told PORT_NOSAVE, kicks_told PORT_NOSAVE;

static void turbo_advance(u64 to) {
    polls = 0;
    in_advance = 1;
    progress++;
    while (irq_started && virt_vblank_due <= to) {
        virt_ns = virt_vblank_due;
        virt_vblank_due += VBLANK_PERIOD_NS;
        in_advance = 0;
        on_timer(0);
        in_advance = 1;
    }
    if (to > virt_ns)
        virt_ns = to;
    in_advance = 0;
    if (spins != spins_told || kicks != kicks_told) {
        if (spins != spins_told)
            port_log("turbo: %u vblanks so far to busy waits of the game's own (the last at %08X)\n", spins, spin_at);
        if (kicks != kicks_told)
            port_log("turbo: %u vblanks so far given on the host's time, the game neither waiting nor spinning "
                     "(the last at %08X): from there on the run is the host's timing, as when paced\n", kicks, kick_at);
        spins_told = spins;
        kicks_told = kicks;
    }
}

/* turbo: a busy wait of the game's own (decompiled C at -O0 spinning on a variable its vblank handler changes, as
 * LEVEL07's func_8002A61C does on g_Args->unk2DC before a CD search) gets no vblank from a clock that only moves
 * when told. A timer looks at the game's thread every 1 ms of host time (TWINE_TURBO_LOOK_NS) while the clock
 * stands still: when its registers, all of them, and the 4 KB of stack above are found again as they were at one of
 * the last 8 looks, three looks running, in the program's own code (not a system call's; not in RT_ENTER), it is a
 * loop that changes nothing: the vblank due comes there (whenever the host gets to see it, the game is in the same
 * state: the run stays the same). As a last resort a still clock gets its vblank after 200 ms all the same
 * (logged: the run is then the host's timing; never seen in the suite). */
#define LOOKS 8
static u32 look_stack_top PORT_NOSAVE; /* (the game's thread's stack's end) */
static int look_fallback PORT_NOSAVE = 200; /* (looks in 200 ms) */
static void on_turbo_check(int sig, siginfo_t *si, void *ctx) {
    static u32 seen;
    static int still, n, at, repeats;
    static greg_t look[LOOKS][11];
    extern char __executable_start[], etext[];
    greg_t *g = ((ucontext_t *)ctx)->uc_mcontext.gregs, cur[11];
    int i, again = 0;
    (void)sig;
    (void)si;
    if (in_advance || progress != seen || port_rt_depth || in_irq) {
        seen = progress;
        still = n = repeats = 0;
        return;
    }
    cur[0] = g[REG_EDI], cur[1] = g[REG_ESI], cur[2] = g[REG_EBP], cur[3] = g[REG_ESP], cur[4] = g[REG_EBX];
    cur[5] = g[REG_EDX], cur[6] = g[REG_ECX], cur[7] = g[REG_EAX], cur[8] = g[REG_EIP], cur[9] = g[REG_EFL];
    { /* (and the stack above, 4 KB of it: a loop that works (its counters at -O0 there, the recompiled code's
       * registers in its wrapper's frame) comes back to the same registers now and then (GTE_cmd's end, called
       * over and over), a busy wait leaves all of it as it was) */
        const u32 *p = (const u32 *)cur[3], *end = (const u32 *)((u32)cur[3] + 4096);
        u32 h = 2166136261u;
        if ((u32)end > look_stack_top)
            end = (const u32 *)look_stack_top;
        for (; p < end; p++)
            h = (h ^ *p) * 16777619u;
        cur[10] = (greg_t)h;
    }
    if ((char *)cur[8] >= __executable_start && (char *)cur[8] < etext && !irq_masked)
        for (i = 0; i < n && !again; i++)
            again = !memcmp(look[i], cur, sizeof(cur));
    memcpy(look[at], cur, sizeof(cur));
    at = (at + 1) % LOOKS;
    if (n < LOOKS)
        n++;
    repeats = again ? repeats + 1 : 0;
    if (repeats >= 3) {
        spins++;
        spin_at = (u32)cur[8];
    } else if (++still >= look_fallback) {
        kicks++;
        kick_at = (u32)cur[8];
    } else
        return;
    still = n = repeats = 0;
    virt_ns = virt_vblank_due;
    virt_vblank_due += VBLANK_PERIOD_NS;
    on_timer(0);
}

u64 port_clock_poll(void) {
    if (port_turbo && ++polls >= 4096) { /* (polled 4096 times and still: a wait of its own; 50 us on) */
        if (!poll_moves++)
            port_log("turbo: the clock moved on by polling (vblank %u)\n", vblank_count);
        turbo_advance(virt_ns + 50000);
    }
    return port_clock_ns();
}

void port_clock_sleep_until(u64 ns) {
    struct timespec ts;
    if (port_turbo) {
        turbo_advance(ns);
        return;
    }
    ts.tv_sec = (time_t)(ns / 1000000000ull);
    ts.tv_nsec = (long)(ns % 1000000000ull);
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL); /* (a vblank's signal may end it early) */
}

void port_clock_spend(u64 ns) {
    if (port_turbo)
        turbo_advance(virt_ns + ns);
}

u32 port_video_frames(void);

/* TWINE_TEST_VBLANKS: the run over (from the game's thread, not in an interrupt or runtime code) */
static void check_end(void) {
    if (end_vblanks && vblank_count >= end_vblanks && !in_irq && !port_rt_depth) {
        port_log("test: the run's %u vblanks are over (frame %u%s)\n", end_vblanks, port_video_frames(),
                 port_turbo ? ", turbo" : "");
        _exit(0);
    }
}

void port_rt_leave(void) {
    if (--port_rt_depth == 0 && irq_pending)
        deliver_pending();
}

void port_irq_start(void) {
    struct sigaction sa;
    timer_t t;
    struct sigevent sev;
    struct itimerspec its;
    irq_started = 1;
    if (port_turbo) { /* (the vblanks come as the clock passes their time; the timer only checks it moves) */
        virt_vblank_due = virt_ns + VBLANK_PERIOD_NS;
        port_log("turbo: game time (TWINE_TURBO)\n");
    }
    memset(&sa, 0, sizeof(sa));
    if (port_turbo) {
        /* (the look runs on a stack of its own: on the game's, its frame would be left below the game's stack
         * pointer at whatever moment the host's timer chose, and some of the game's code reads locals it never set
         * (a run's outcome changes with where the stack is: tools/endless_check.py runs them with the addresses
         * fixed, setarch -R); there, what it found would be the host's timing) */
        stack_t ss;
        pthread_attr_t at;
        void *base;
        size_t size;
        extern u32 port_game_stack_top; /* (main.c: the game's stack, at a fixed place) */
        if (port_game_stack_top)
            look_stack_top = port_game_stack_top;
        else if (!pthread_getattr_np(pthread_self(), &at)) {
            if (!pthread_attr_getstack(&at, &base, &size))
                look_stack_top = (u32)base + (u32)size;
            pthread_attr_destroy(&at);
        }
        if (!look_stack_top) /* (unknown: no further than this frame's 64 KB above) */
            look_stack_top = (u32)&at + 65536;
        ss.ss_size = 256 * 1024;
        ss.ss_sp = malloc(ss.ss_size);
        ss.ss_flags = 0;
        sigaltstack(&ss, NULL);
        sa.sa_sigaction = on_turbo_check;
        sa.sa_flags = SA_RESTART | SA_SIGINFO | SA_ONSTACK;
    } else {
        sa.sa_handler = on_timer;
        sa.sa_flags = SA_RESTART;
    }
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);
    memset(&sev, 0, sizeof(sev));
    /* to this (the game's) thread only: SDL's threads must never run game code */
    sev.sigev_notify = SIGEV_THREAD_ID;
    sev._sigev_un._tid = gettid();
    sev.sigev_signo = SIGALRM;
    timer_create(CLOCK_MONOTONIC, &sev, &t);
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 16683333; /* NTSC: 59.94 Hz */
    if (port_turbo) /* (the look at the game's thread; TWINE_TURBO_LOOK_NS: tests) */
    {
        its.it_value.tv_nsec = getenv("TWINE_TURBO_LOOK_NS") ? atol(getenv("TWINE_TURBO_LOOK_NS")) : 1000000;
        if (!its.it_value.tv_nsec) /* (0: no look, for tests: a busy wait of the game's then waits for ever) */
            return;
        look_fallback = (int)(200000000 / its.it_value.tv_nsec);
    }
    its.it_interval = its.it_value;
    timer_settime(t, 0, &its, NULL);
}

u32 port_vblank_count(void) {
    return vblank_count;
}

void port_poll_input(void);

/* the controls, read once a vblank while the game waits for one (video.c) */
static void poll_input_once(void) {
    static u32 polled;
    if (polled != vblank_count) {
        polled = vblank_count;
        port_poll_input();
    }
}

/* Sleep until the next vblank has been delivered. */
void port_wait_vblank(void) {
    u32 v = vblank_count;
    while (vblank_count == v) {
        if (irq_pending && !irq_masked)
            deliver_pending();
        else if (port_turbo)
            turbo_advance(virt_vblank_due);
        else
            usleep(200);
    }
    poll_input_once();
    check_end();
}

/* Code that waits in a loop of fixed length (a timeout counted in passes) runs far faster here than on the
 * console. A pass of such a loop can charge what it takes on the console (ns), slept in 1 ms steps; time the
 * loop spends on other work counts towards it. */
void port_spin(u32 ns) {
    static u64 owed, since;
    struct timespec ts;
    u64 now, due;
    if (port_turbo) { /* (the pass takes its time on the clock of game time; the work around it none) */
        turbo_advance(virt_ns + ns);
        check_end();
        return;
    }
    clock_gettime(CLOCK_MONOTONIC, &ts);
    now = (u64)ts.tv_sec * 1000000000ull + ts.tv_nsec;
    if (!owed)
        since = now;
    owed += ns;
    if (owed < 1000000)
        return;
    due = since + owed;
    owed = 0;
    ts.tv_sec = due / 1000000000ull;
    ts.tv_nsec = due % 1000000000ull;
    while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL) == EINTR)
        ;
    check_end();
}

/* ---- libapi / BIOS ---- */

s32 EnterCriticalSection(void) {
    irq_masked = 1;
    return 1;
}

void ExitCriticalSection(void) {
    irq_masked = 0;
    deliver_pending();
}

/* An event like one already open (the boot stub opens its vblank event again before each executable it
 * starts, and libspu its SPU event at each SpuInit, never closing them) is that one: on the console they
 * don't pile up, here they filled the table after a few missions, and the front end, starting again, got no
 * vblank event of its own and waited for one forever. */
s32 OpenEvent(u32 cls, s32 spec, s32 mode, void *fn) {
    int i;
    for (i = 0; i < EVENTS; i++)
        if (events[i].open && events[i].cls == cls && events[i].spec == (u32)spec && events[i].mode == (u32)mode &&
            events[i].fn == (EventFn)fn) {
            events[i].owner = port_module;
            return 0xF1000000 | i;
        }
    for (i = 0; i < EVENTS; i++) {
        if (!events[i].open) {
            events[i].cls = cls;
            events[i].spec = spec;
            events[i].mode = mode;
            events[i].fn = (EventFn)fn;
            events[i].enabled = events[i].delivered = 0;
            events[i].owner = port_module;
            events[i].open = 1;
            return 0xF1000000 | i;
        }
    }
    return -1;
}

static Event *event_of(u32 h) {
    return (h & 0xFFFF0000) == 0xF1000000 && (h & 0xFFFF) < EVENTS ? &events[h & 0xFFFF] : NULL;
}

s32 EnableEvent(u32 h) {
    Event *e = event_of(h);
    if (e)
        e->enabled = 1;
    return e != NULL;
}

s32 DisableEvent(u32 h) {
    Event *e = event_of(h);
    if (e)
        e->enabled = 0;
    return e != NULL;
}

/* DeliverEvent (B0:07, and the memory card driver, memcard.c): each open, enabled event of this class and spec
 * gets it: an EvMdINTR one (1000h) has its handler called, the others TestEvent reports. */
void port_deliver_event(u32 cls, u32 spec) {
    int i;
    for (i = 0; i < EVENTS; i++) {
        Event *e = &events[i];
        if (!e->open || !e->enabled || e->cls != cls || (u32)e->spec != spec)
            continue;
        if ((e->mode & 0x1000) && e->fn)
            e->fn();
        else
            e->delivered = 1;
    }
}

/* TestEvent: the memory card events (SwCARD, HwCARD) when they have been delivered; transfers and other
 * hardware work complete at once here, so their events have always been delivered; the vblank counter's has
 * once per vblank. */
s32 TestEvent(u32 h) {
    static u32 seen[16];
    Event *e = event_of(h);
    if (!e)
        return 0;
    if (e->cls == 0xF4000001 || e->cls == 0xF0000011) {
        s32 r = e->delivered;
        e->delivered = 0;
        return r;
    }
    if (e->cls == 0xF2000003) {
        u32 i = h & 0xFFFF;
        if (seen[i] == vblank_count)
            return 0;
        seen[i] = vblank_count;
    }
    return 1;
}

s32 CloseEvent(u32 h) {
    Event *e = event_of(h);
    if (e)
        e->open = e->enabled = 0;
    return e != NULL;
}

/* StartRCnt / StopRCnt (spec = RCntCNTn = 0xF2000000 | n) */
s32 StartRCnt(u32 spec) {
    rcnt_running |= 1u << (spec & 3);
    return (spec & 0xFFFF) < 3;
}

s32 StopRCnt(u32 spec) {
    rcnt_running &= ~(1u << (spec & 3));
    return 1;
}

/* Save states (savestate.c): the runtime's own place now, which a load keeps (put = 1 puts v back) */
void port_irq_now(int *v, int put) {
    if (put) {
        port_rt_depth = v[0], irq_masked = v[1], irq_pending = v[2], in_irq = v[3];
        return;
    }
    v[0] = port_rt_depth, v[1] = irq_masked, v[2] = irq_pending, v[3] = in_irq;
}

/* An executable left without its own ending (main.c, port_exe_leave): the callbacks into its code forgotten
 * and its events closed */
void port_irq_forget(const PortModule *m) {
    int i;
    vsync_callback = NULL;
    for (i = 0; i < 8; i++)
        vsync_callbacks[i] = NULL;
    for (i = 0; i < EVENTS; i++)
        if (events[i].owner == m)
            events[i].open = events[i].enabled = 0;
}

/* ---- libetc ---- */

static u32 vsync_last;
extern int port_subframe;
void port_smooth_tick(u32 vblanks);
void port_state_frame(void);

/* VSync(0): wait for the next vblank; VSync(n > 1): wait until n vblanks have passed since the last call;
 * VSync(1): don't wait, return the horizontal line count (approximated); VSync(-1): vblank counter. */
s32 VSync(s32 mode) {
    u32 target;
    if (mode < 0) {
        if (port_turbo) /* (a loop of the game's own waiting on the count) */
            port_clock_poll();
        return vblank_count;
    }
    if (mode == 1)
        return 0;
    if (port_subframe) /* an extra drawing between ticks (interp.c): nothing to wait for or show */
        return 0;
    target = vsync_last + (mode > 1 ? mode : 1);
    if (mode == 0)
        target = vblank_count + 1;
    port_state_frame();
    port_frame_end();
    {
        u64 t = PT_BEGIN();
        while ((s32)(vblank_count - target) < 0)
            port_wait_vblank();
        PT_END(PT_WAIT, t);
    }
    vsync_last = vblank_count;
    port_timing_tick();
    port_smooth_tick(mode > 1 ? mode : 1);
    return 0;
}

/* A pass of a wait loop in recompiled code (recomp.py wait_loop): it takes its time on the console, so the
 * loop sleeps rather than spins and a delay counted in passes lasts as long as it should. A program that
 * waits for the vblank its own way, without VSync (the front end flips its buffers from its vblank handler),
 * gets its frames shown and the controls read here, once per vblank while VSync has gone uncalled for a while
 * (a game tick takes several). Not from an interrupt handler or runtime code. */
void port_idle(u32 ns) {
    static u32 shown;
    if (in_irq || port_rt_depth)
        return;
    port_spin(ns);
    if (!port_subframe && vblank_count != shown && vblank_count - vsync_last >= 8) {
        shown = vblank_count;
        port_state_frame();
        port_frame_end();
    } else if (!port_subframe) {
        poll_input_once();
    }
}

void *VSyncCallback(void (*fn)(void)) {
    void *old = (void *)vsync_callback;
    vsync_callback = fn;
    return old;
}

/* VSyncCallbacks: more functions to call every vblank, one per channel (0-7) */
void *VSyncCallbacks(s32 ch, void (*fn)(void)) {
    void *old = (void *)vsync_callbacks[ch & 7];
    vsync_callbacks[ch & 7] = fn;
    return old;
}
