/* Smooth motion between the game's 20 Hz ticks (with the smooth mod, port/mods/smooth.c). While the game
 * waits out a tick, its render step draws the frame again as often as there is time for, each time with the
 * camera interpolated between the last two ticks. Such an extra drawing runs the game's own code on the
 * machine as it is, and everything it changes is put back afterwards: PS1 RAM and the scratchpad, the GTE, the
 * GPU's drawing state and libgpu's environments. Meanwhile the vblank interrupt is held back, writes to the
 * sound chip are dropped and VSync doesn't wait. Every frame is shown as soon as it is drawn (the tick's own
 * one too), at the display's rate (SDL vsync) or as fast as they come with TWINE_VSYNC=0.
 *
 * TWINE_SMOOTH=0 turns it off. Headless runs only do it with TWINE_SMOOTH=1; TWINE_SHOT frames then dump each
 * drawing of the tick as shot_NNNN_K.ppm. TWINE_SMOOTH_LOG=1 logs the timing behind each decision. */
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

int port_subframe;        /* while an extra drawing runs */
int port_smooth_shown;    /* the render step has shown this tick's frame (video.c then doesn't at VSync) */
static int enabled = -1;
static u64 tick_start;    /* host time the game's last VSync returned */
static u32 tick_vblanks = 3;
static u64 cost = 4000000; /* ns per drawing, a moving average */
static u64 began;
static int drawings;       /* extra drawings this tick */
static u8 ram_copy[PS1_RAM_SIZE], scratch_copy[PS1_SCRATCH_SIZE];

#define VBLANK_NS 16683333ull

static u64 now_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (u64)ts.tv_sec * 1000000000ull + (u64)ts.tv_nsec;
}

static int smooth_enabled(void) {
    if (enabled < 0) {
        const char *s = getenv("TWINE_SMOOTH");
        enabled = s ? atoi(s) != 0 : !port_video_headless();
    }
    return enabled;
}

/* irq.c: the game's VSync returned; its next tick lasts `vblanks` */
void port_smooth_tick(u32 vblanks) {
    tick_start = now_ns();
    tick_vblanks = vblanks;
    drawings = 0;
}

/* Before an extra drawing (smooth.c): 1 if there is time for one before the tick ends, keeping room for
 * `room` drawings in all (this one and those the tick still has to make; -1: draw whatever the time), with how
 * far into the tick it is (0-4096) and the mouse's movement the game hasn't taken yet; the machine is saved. */
int port_smooth_begin(s32 *t, s32 *yaw, s32 *pitch, int room) {
    u64 now, end;
    if (port_subframe || !smooth_enabled() || !tick_start)
        return 0;
    now = now_ns();
    end = tick_start + tick_vblanks * VBLANK_NS;
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
    memcpy(ram_copy, (void *)PS1_RAM_BASE, PS1_RAM_SIZE);
    memcpy(scratch_copy, (void *)PS1_SCRATCH_BASE, PS1_SCRATCH_SIZE);
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
    cost = (cost * 3 + (now - began)) / 4;
    gpu_draw_origin(&x, &y);
    memcpy((void *)PS1_RAM_BASE, ram_copy, PS1_RAM_SIZE);
    memcpy((void *)PS1_SCRATCH_BASE, scratch_copy, PS1_SCRATCH_SIZE);
    GTE_restore();
    gpu_restore();
    libgpu_restore();
    port_subframe = 0;
    RT_LEAVE();
    port_video_present_draw(drawings++, x, y);
    port_smooth_shown = 1;
}

/* After the tick's own drawing: show it at once (when smoothing; else VSync shows frames as usual). */
void port_smooth_present(void) {
    s32 x, y;
    if (!smooth_enabled())
        return;
    gpu_draw_origin(&x, &y);
    port_video_present_draw(drawings++, x, y);
    port_smooth_shown = 1;
}
