/* TWINE_TIMING=1: where the time of each tick (VSync to VSync) goes, summed over 2 s of host time and logged as
 * "timing:" lines, in ms per tick: the tick, the time VSync waited (to spare), smooth motion's extra drawings and
 * their machine copies, the GPU's command lists (DrawOTag), a lent weapon's VRAM swaps, sound banks read and
 * decoded, frames shown; with counts (extra drawings, VRAM read back under lent images, sound banks read) and the
 * longest tick. Off by default: then each timed point costs one test of port_timing_on. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "port.h"

int port_timing_on;
static u64 sum[PT_IDS], tick_start, period_start, longest;
static u32 ticks;

static const char *const names[PT_SECTIONS] = {"tick", "waiting", "extra drawings", "their copies", "DrawOTag",
                                               "lent swaps", "sound loads", "showing"};
static const char *const counted[PT_IDS - PT_SECTIONS] = {"lent reads", "extras", "bank reads"};

u64 port_timing_now(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (u64)ts.tv_sec * 1000000000ull + (u64)ts.tv_nsec;
}

void port_timing_add(int id, u64 since) {
    if (id >= 0 && id < PT_SECTIONS && since)
        sum[id] += port_timing_now() - since;
}

void port_timing_count(int id) {
    if (port_timing_on && id >= PT_SECTIONS && id < PT_IDS)
        sum[id]++;
}

__attribute__((constructor)) static void timing_init(void) {
    const char *s = getenv("TWINE_TIMING");
    port_timing_on = s && atoi(s);
}

void port_timing_tick(void) {
    u64 now;
    if (!port_timing_on)
        return;
    now = port_timing_now();
    if (tick_start) {
        u64 t = now - tick_start;
        sum[PT_TICK] += t;
        if (t > longest)
            longest = t;
        ticks++;
    }
    tick_start = now;
    if (!period_start)
        period_start = now;
    if (now - period_start >= 2000000000ull && ticks) {
        char line[512];
        int n = 0, i;
        for (i = 0; i < PT_SECTIONS && n < (int)sizeof(line) - 64; i++)
            n += snprintf(line + n, sizeof(line) - n, "%s%s %.2f", i ? ", " : "", names[i], sum[i] / 1e6 / ticks);
        for (i = PT_SECTIONS; i < PT_IDS && n < (int)sizeof(line) - 64; i++)
            n += snprintf(line + n, sizeof(line) - n, "; %s %.2f", counted[i - PT_SECTIONS], (double)sum[i] / ticks);
        port_log("timing: ms a tick over %u ticks: %s; longest tick %.1f ms\n", ticks, line, longest / 1e6);
        for (i = 0; i < PT_IDS; i++)
            sum[i] = 0;
        ticks = 0;
        longest = 0;
        period_start = now;
    }
}

/* TWINE_TRACE: the tracing asked for, a list of names (weapon, change, sound, ... or all): 1 if `what` is in it
 * ("trace:" lines; port/mods/trace.c) */
int port_trace(const char *what) {
    const char *s = getenv("TWINE_TRACE"), *p;
    size_t n = strlen(what);
    if (!s || !*s)
        return 0;
    if (!strcmp(s, "all") || !strcmp(s, "1"))
        return 1;
    for (p = s; (p = strstr(p, what)); p += n)
        if ((p == s || p[-1] == ',') && (p[n] == 0 || p[n] == ','))
            return 1;
    return 0;
}
