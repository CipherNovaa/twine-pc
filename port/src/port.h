#ifndef PORT_H
#define PORT_H

/* Native port runtime: shared declarations of the host side (memory, logging, GTE, libraries). */
#include "common.h"

/* PS1 memory lives at its own addresses in this (32-bit) process: 2 MB main RAM at 0x80000000 and the
 * 1 KB scratchpad at 0x1F800000, so the game's pointers and absolute addresses work unchanged. */
#define PS1_RAM_BASE 0x80000000u
/* 2 MB as on the console; 8 MB (a development kit's) with the ram_8mb setting (TWINE_RAM_8MB), whose last 6 MB
 * are a heap of the runtime's own for what the levels' 2 MB has no room for (malloc3.c's ext heap) */
extern u32 port_ram_size;
#define PS1_RAM_SIZE port_ram_size
#define PS1_RAM_CONSOLE 0x00200000u
#define PS1_RAM_MASK (PS1_RAM_SIZE - 4) /* (DMA addresses: words in RAM) */
#define PS1_SCRATCH_BASE 0x1F800000u
#define PS1_SCRATCH_SIZE 0x00001000u

void port_log(const char *fmt, ...);
void port_fatal(const char *fmt, ...) __attribute__((noreturn));

/* Runtime code that must not be interrupted by the emulated vblank (irq.c) */
extern volatile int port_rt_depth;
void port_rt_leave(void);
#define RT_ENTER() (port_rt_depth++)
#define RT_LEAVE() port_rt_leave()

/* Memory a save state leaves as it is (savestate.c): what belongs to the host or to this run, like a pointer to
 * the heap. (port/Makefile puts whole objects there too.) */
#define PORT_NOSAVE __attribute__((section("nosave_data")))

void port_irq_start(void);
u32 port_vblank_count(void);
void port_wait_vblank(void);
void port_spin(u32 ns);      /* a pass of a counted wait loop takes this long, as on the console */

void port_pad_vblank(void);  /* pad.c: per-vblank controller update */
void port_cd_irq(void);      /* cdrom.c: CD completion callbacks */
void port_dma_irq(void);     /* io.c: MDEC transfer completion callbacks */
void port_video_init(void);  /* video.c: window and input */
void port_frame_end(void);   /* video.c: present a finished frame */

/* cdrom.c: a file of the disc (malloc'd: its size, 0 none), or len bytes of it from offset */
u32 port_disc_file(const char *name, u8 **data);
u32 port_disc_part(const char *name, u32 offset, u32 len, u8 **data);
/* tones.c: a level's sound bank (SOUNDFX/LnFX.VH and .VB of its Lnn/LVLnnCD.BIN), read once (0: none) */
int port_sound_bank(int level, const u8 **vh, const u8 **vb);

/* TWINE_TIMING=1 (timing.c): where each tick's time goes, logged every 2 s ("timing:" lines). Off: one test of a
 * flag at each point. Sections are timed with PT_BEGIN / PT_END, events counted with port_timing_count. */
enum {
    PT_TICK,        /* VSync to VSync: the whole tick */
    PT_WAIT,        /* VSync waiting for the vblank (time to spare) */
    PT_EXTRA,       /* smooth motion's extra drawings, all of each */
    PT_SMOOTH_COPY, /* ... their machine saved and put back (RAM, scratchpad, mods' memory) */
    PT_DRAWOTAG,    /* the game's drawings sent to the GPU (DrawOTag), the tick's and the extras' */
    PT_LENT_SWAP,   /* a lent weapon's images swapped in and out of VRAM */
    PT_SOUND_LOAD,  /* sound banks read from the disc, tones decoded */
    PT_PRESENT,     /* frames shown */
    PT_SECTIONS,
    PT_LENT_READ = PT_SECTIONS, /* (counts) VRAM read back under the lent images */
    PT_EXTRAS,                  /* extra drawings */
    PT_BANK_READS,              /* sound banks read */
    PT_IDS
};
extern int port_timing_on;
u64 port_timing_now(void);
void port_timing_add(int id, u64 since);
void port_timing_count(int id);
void port_timing_tick(void); /* irq.c, at each VSync */
int port_trace(const char *what); /* TWINE_TRACE names it (timing.c) */
#define PT_BEGIN() (port_timing_on ? port_timing_now() : 0)
#define PT_END(id, t) do { if (port_timing_on) port_timing_add((id), (t)); } while (0)

#endif
