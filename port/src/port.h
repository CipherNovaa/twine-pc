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

#endif
