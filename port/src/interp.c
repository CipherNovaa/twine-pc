/* Smooth motion between the game's 20 Hz ticks (with the smooth mod, port/mods/smooth.c). While the game
 * waits out a tick, its render step draws the frame again as often as there is time for, each time with the
 * camera interpolated between the last two ticks. Such an extra drawing runs the game's own code on the
 * machine as it is, and everything it changes is put back afterwards: PS1 RAM and the scratchpad, the GTE, the
 * GPU's drawing state and libgpu's environments. Meanwhile the vblank interrupt is held back, writes to the
 * sound chip are dropped and VSync doesn't wait. Every frame is shown as soon as it is drawn (the tick's own
 * one too), at the display's rate (SDL vsync) or as fast as they come with TWINE_VSYNC=0.
 *
 * TWINE_SMOOTH=0 turns it off. Headless runs only do it with TWINE_SMOOTH=1; TWINE_SHOT frames then dump each
 * drawing of the tick as shot_NNNN_K.ppm. TWINE_SMOOTH_LOG=1 logs the timing behind each decision.
 * TWINE_FPS_MAX: drawings at most that many a second (0: as many as there is time for); by default three times
 * the refresh rate of the window's display, so that each refresh gets one drawn at most a third of a refresh
 * before it, with room to spare around the ticks (more would be drawn and never shown). The tick's own frame is
 * never held back. */
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "port.h"
#include "gpu.h"

void GTE_save(void);
void GTE_restore(void);
void libgpu_save(void);
void libgpu_restore(void);
void port_mouse_pending(s32 *yaw, s32 *pitch);
int port_video_headless(void);
void port_video_present_draw(int index, s32 x, s32 y);
float port_video_refresh(void);

int port_subframe;        /* while an extra drawing runs */
int port_smooth_shown;    /* the render step has shown this tick's frame (video.c then doesn't at VSync) */
static int enabled = -1;
static u64 tick_start;    /* host time the game's last VSync returned */
static u32 tick_vblanks = 3;
static u64 cost = 4000000; /* ns per drawing, a moving average */
static u64 began;
static int drawings;       /* extra drawings this tick */
extern u32 port_ext_low;
static u8 *ram_copy PORT_NOSAVE, scratch_copy[PS1_SCRATCH_SIZE]; /* (RAM's copy: as big as RAM) */
static u64 shown_at;       /* host time the last drawing was shown */
static u64 spacing;        /* TWINE_FPS_MAX's time between drawings (0: none) */

#define VBLANK_NS 16683333ull

/* memory of the game's outside PS1 RAM (a mod's), put back after each extra drawing as RAM is */
static struct {
    void *at;
    u32 size;
    void *copy;
} kept[4];

void port_smooth_keep(void *at, u32 size) {
    int i;
    for (i = 0; i < 4; i++)
        if (kept[i].at == at || !kept[i].at) {
            if (kept[i].at != at)
                kept[i].copy = malloc(size);
            kept[i].at = at;
            kept[i].size = size;
            return;
        }
}

static u64 now_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (u64)ts.tv_sec * 1000000000ull + (u64)ts.tv_nsec;
}

static int smooth_enabled(void) {
    const char *s = getenv("TWINE_SMOOTH"); /* (read each time: the front end's PORT SETTINGS change it) */
    enabled = s ? atoi(s) != 0 : !port_video_headless();
    return enabled;
}

static u64 fps_spacing(void) {
    static int fps_max = -2;
    float hz;
    if (fps_max == -2) {
        const char *s = getenv("TWINE_FPS_MAX");
        fps_max = s ? atoi(s) : -1;
    }
    if (fps_max == 0)
        return 0;
    hz = fps_max > 0 ? (float)fps_max : 3 * port_video_refresh();
    return hz > 0 ? (u64)(1e9f / hz) : 0;
}

/* irq.c: the game's VSync returned; its next tick lasts `vblanks` */
void port_smooth_tick(u32 vblanks) {
    tick_start = now_ns();
    tick_vblanks = vblanks;
    drawings = 0;
    spacing = fps_spacing(); /* (the window may have gone to another display) */
}

/* Before an extra drawing (smooth.c): 1 if there is time for one before the tick ends, keeping room for
 * `room` drawings in all (this one and those the tick still has to make; -1: draw whatever the time), with how
 * far into the tick it is (0-4096) and the mouse's movement the game hasn't taken yet; the machine is saved. */
int port_smooth_begin(s32 *t, s32 *yaw, s32 *pitch, int room) {
    u64 now, end;
    int i;
    if (port_subframe || !smooth_enabled() || !tick_start)
        return 0;
    now = now_ns();
    end = tick_start + tick_vblanks * VBLANK_NS;
    /* TWINE_FPS_MAX: not before its time, so that it is shown `spacing` after the one before (if that leaves
     * room for the rest) */
    if (room >= 0 && spacing && shown_at && now + (cost < spacing ? cost : spacing) < shown_at + spacing) {
        u64 at = shown_at + spacing - (cost < spacing ? cost : spacing);
        struct timespec ts;
        if (at + room * cost + 1000000 > end)
            return 0;
        ts.tv_sec = (time_t)(at / 1000000000ull);
        ts.tv_nsec = (long)(at % 1000000000ull);
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL);
        now = now_ns();
    }
    if (getenv("TWINE_SMOOTH_LOG"))
        port_log("smooth: %.2f ms into the tick of %.2f ms, drawing %.2f ms, %d drawn\n", (now - tick_start) / 1e6,
                 (end - tick_start) / 1e6, cost / 1e6, drawings);
    if (room >= 0 && now + room * cost + 1000000 > end)
        return 0;
    if (now > end)
        now = end;
    *t = (s32)((now - tick_start) * 4096 / (end - tick_start));
    port_mouse_pending(yaw, pitch);
    RT_ENTER(); /* holds the vblank interrupt */
    if (!ram_copy && !(ram_copy = malloc(PS1_RAM_SIZE)))
        port_fatal("out of memory\n");
    memcpy(ram_copy, (void *)PS1_RAM_BASE, PS1_RAM_CONSOLE);
    if (PS1_RAM_SIZE > PS1_RAM_CONSOLE && port_ext_low) { /* (8 MB: of the ext heap, its first free block's header
                                                           * and what it has handed out) */
        memcpy(ram_copy + PS1_RAM_CONSOLE, (void *)(PS1_RAM_BASE + PS1_RAM_CONSOLE), 16);
        memcpy(ram_copy + (port_ext_low - PS1_RAM_BASE), (void *)port_ext_low, PS1_RAM_BASE + PS1_RAM_SIZE - port_ext_low);
    }
    memcpy(scratch_copy, (void *)PS1_SCRATCH_BASE, PS1_SCRATCH_SIZE);
    for (i = 0; i < 4 && kept[i].at; i++)
        memcpy(kept[i].copy, kept[i].at, kept[i].size);
    GTE_save();
    gpu_save();
    libgpu_save();
    port_subframe = 1;
    began = now;
    return 1;
}

/* After an extra drawing: put the machine back, and show the drawing. */
void port_smooth_end(void) {
    u64 now = now_ns();
    s32 x, y;
    int i;
    cost = (cost * 3 + (now - began)) / 4;
    gpu_draw_origin(&x, &y);
    memcpy((void *)PS1_RAM_BASE, ram_copy, PS1_RAM_CONSOLE);
    if (PS1_RAM_SIZE > PS1_RAM_CONSOLE && port_ext_low) {
        memcpy((void *)(PS1_RAM_BASE + PS1_RAM_CONSOLE), ram_copy + PS1_RAM_CONSOLE, 16);
        memcpy((void *)port_ext_low, ram_copy + (port_ext_low - PS1_RAM_BASE), PS1_RAM_BASE + PS1_RAM_SIZE - port_ext_low);
    }
    memcpy((void *)PS1_SCRATCH_BASE, scratch_copy, PS1_SCRATCH_SIZE);
    for (i = 0; i < 4 && kept[i].at; i++)
        memcpy(kept[i].at, kept[i].copy, kept[i].size);
    GTE_restore();
    gpu_restore();
    libgpu_restore();
    port_subframe = 0;
    RT_LEAVE();
    port_video_present_draw(drawings++, x, y);
    port_smooth_shown = 1;
    shown_at = now_ns();
}

/* After the tick's own drawing: show it at once (when smoothing; else VSync shows frames as usual). */
void port_smooth_present(void) {
    s32 x, y;
    if (!smooth_enabled())
        return;
    gpu_draw_origin(&x, &y);
    port_video_present_draw(drawings++, x, y);
    port_smooth_shown = 1;
    shown_at = now_ns();
}
