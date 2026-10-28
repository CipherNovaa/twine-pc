#ifndef INTRO_H
#define INTRO_H

/* Startup sequence before the game (intro.c): the PS1's own boot animation from the user's BIOS
 * (bios_boot.c, with the CD-ROM controller in bios_cdrom.c), then the port's title card (intro_card.c). */
#include "port.h"

void port_intro(void); /* main.c, after port_video_init/port_audio_init and before anything is loaded */
int port_intro_bios(const char *path, char *desc, size_t len); /* setup.c: a BIOS file's rank (0: not one), or the found one's */

/* intro.c */
int intro_shot_wanted(u32 frame); /* TWINE_INTRO_SHOT lists this intro frame */
u32 intro_next_frame(void);       /* number the next intro frame (BIOS vblanks, then card frames) */
void intro_audio(int frames);     /* headless: run the SPU for `frames` samples (into TWINE_WAV) */

/* bios_boot.c: run the BIOS from reset until it jumps to stop_pc (the boot executable's entry point).
 * 1 = reached, 0 = gave up, skipped or no disc; the caller then restores the machine. */
int bios_boot_run(const u8 *rom, u32 stop_pc);
extern u64 bios_boot_now;          /* CPU cycles since reset */
void bios_boot_irq(int n);         /* raise I_STAT bit n */
void bios_boot_schedule(u64 when); /* a device has something due at `when` */

/* bios_cdrom.c */
int cdc_open(void);           /* 0 if there is no disc image */
void cdc_close(void);
u32 cdc_boot_entry(void);     /* entry point of the executable SYSTEM.CNF boots (0: none) */
u8 cdc_read(u32 reg);         /* 0x1F801800 + reg */
void cdc_write(u32 reg, u8 v);
u32 cdc_dma_word(void);       /* DMA3: the next word of the data FIFO */
void cdc_events(void);        /* run what is due at bios_boot_now */
extern u64 cdc_next;          /* when the controller next needs cdc_events() */

/* intro_card.c: "Port by CipherNovaa" at the window's resolution. */
void intro_card_run(void);

/* video.c */
int port_video_headless(void);
int port_video_poll_skip(void);   /* pump window events; 1 = a skip key or button was pressed */
void port_video_show_gpu(void);   /* present the GPU's display (as the game's frames are) */
void port_video_dump_gpu(const char *path);
void *port_video_renderer(void);  /* SDL_Renderer (NULL when headless) */
void port_video_present_mode(void); /* TWINE_PRESENT again, after changing vsync */

#endif
