/* libetc / libapi pieces with host behaviour: interrupt callbacks (the vblank one is run by irq.c, DMA
 * ones by io.c), root counters, the video mode and the stack switch the game does around its main loop. */
#include "port.h"
#include "io.h"

static u32 video_mode;
static u32 rcnt_target[4], rcnt_mode[4];

void *VSyncCallback(void (*fn)(void));

s32 ResetCallback(void) {
    return 0;
}

s32 StopCallback(void) {
    return 0;
}

static void *irq_callbacks[11];

void *InterruptCallback(s32 irq, void *fn) {
    void *old = NULL;
    if (irq >= 0 && irq < 11) {
        old = irq_callbacks[irq];
        irq_callbacks[irq] = fn;
    }
    if (irq == 0) /* vblank */
        VSyncCallback((void (*)(void))fn);
    return old;
}

static void *dma_callbacks[7];

void *DMACallback(s32 ch, void *fn) {
    void *old = NULL;
    if (ch >= 0 && ch < 7) {
        old = dma_callbacks[ch];
        dma_callbacks[ch] = fn;
        port_dma_callback(ch, fn);
    }
    return old;
}

s32 SetVideoMode(s32 mode) {
    s32 old = video_mode;
    video_mode = mode;
    return old;
}

s32 GetVideoMode(void) {
    return video_mode;
}

/* SetSp: the game runs on the scratchpad stack on the PS1; native code keeps the host stack. */
u32 SetSp(u32 sp) {
    (void)sp;
    return 0x801FFF00;
}

s32 SetRCnt(u32 spec, u16 target, s32 mode) {
    rcnt_target[spec & 3] = target;
    rcnt_mode[spec & 3] = mode;
    return 1;
}

s32 ResetRCnt(u32 spec) {
    (void)spec;
    return 1;
}

s32 GetRCnt(u32 spec) {
    (void)spec;
    return 0;
}

void puts_ps1(const char *s) {
    port_log("%s\n", s);
}

s32 PCwrite(s32 fd, char *buf, s32 len) {
    return -1;
}
