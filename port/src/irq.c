/* Interrupts and events. On the PS1 the vertical blank interrupt (60 Hz) runs the game's handlers between
 * any two instructions; here a 60 Hz timer signal does the same on the game's thread, so the game's busy
 * waits on its vblank counter and its own Enter/ExitCriticalSection protection work as written.
 *
 * Runtime code that isn't safe to interrupt (stdio, SDL, ...) runs inside RT_ENTER/RT_LEAVE: a vblank that
 * arrives meanwhile is held and delivered when the runtime returns to game code. */
#define _GNU_SOURCE
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
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

void port_rt_leave(void) {
    if (--port_rt_depth == 0 && irq_pending)
        deliver_pending();
}

void port_irq_start(void) {
    struct sigaction sa;
    timer_t t;
    struct sigevent sev;
    struct itimerspec its;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = on_timer;
    sa.sa_flags = SA_RESTART;
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
        else
            usleep(200);
    }
    poll_input_once();
}

/* Code that waits in a loop of fixed length (a timeout counted in passes) runs far faster here than on the
 * console. A pass of such a loop can charge what it takes on the console (ns), slept in 1 ms steps; time the
 * loop spends on other work counts towards it. */
void port_spin(u32 ns) {
    static u64 owed, since;
    struct timespec ts;
    u64 now, due;
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

/* ---- libetc ---- */

static u32 vsync_last;
extern int port_subframe;
void port_smooth_tick(u32 vblanks);

/* VSync(0): wait for the next vblank; VSync(n > 1): wait until n vblanks have passed since the last call;
 * VSync(1): don't wait, return the horizontal line count (approximated); VSync(-1): vblank counter. */
s32 VSync(s32 mode) {
    u32 target;
    if (mode < 0)
        return vblank_count;
    if (mode == 1)
        return 0;
    if (port_subframe) /* an extra drawing between ticks (interp.c): nothing to wait for or show */
        return 0;
    target = vsync_last + (mode > 1 ? mode : 1);
    if (mode == 0)
        target = vblank_count + 1;
    port_frame_end();
    while ((s32)(vblank_count - target) < 0)
        port_wait_vblank();
    vsync_last = vblank_count;
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
