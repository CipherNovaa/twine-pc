#ifndef SPU_H
#define SPU_H

/* SPU emulation (spu.c) and its audio output (audio.c). The game reaches the SPU's registers through io.c,
 * which calls spu_read/spu_write (io.h). */
#include "port.h"

/* What advances the SPU's 44100 Hz clock:
 *   SPU_CLOCK_MANUAL  only explicit spu_render calls (the default; tests)
 *   SPU_CLOCK_DEVICE  an audio device thread calling spu_render; register access then takes a mutex
 *   SPU_CLOCK_HOST    host time: each register access first runs the samples due since the last one, so
 *                     voices still play and end (ENVX, ENDX) without an audio device (TWINE_HEADLESS) */
enum { SPU_CLOCK_MANUAL, SPU_CLOCK_DEVICE, SPU_CLOCK_HOST };
int spu_set_clock(int mode); /* 0 if the mode can't be used (no mutex for SPU_CLOCK_DEVICE) */

/* Run the SPU for `frames` 44100 Hz ticks, writing interleaved stereo (L, R) to out (NULL: discard). */
void spu_render(s16 *out, int frames);
void spu_sync(void); /* SPU_CLOCK_HOST: run the samples due by now (else nothing) */
int spu_take_peak(void); /* the loudest output sample (absolute) run since the last call */

/* ADPCM prediction filters, shared by SPU-ADPCM and XA-ADPCM (which uses the first four) */
extern const s32 adpcm_filter_pos[5], adpcm_filter_neg[5];

/* CD audio input: the drive's 44100 Hz output (xa.c) comes in through a lock-free ring from the game thread
 * to whichever thread runs the SPU. After running dry the SPU waits for a little to be queued, so the
 * game thread's bursts (once per vblank) don't break up the sound. */
#define SPU_CD_TARGET 4096                  /* frames the drive keeps queued at most */
int spu_cd_queued(void);                    /* frames waiting */
int spu_cd_push(const s16 *lr, int frames); /* interleaved L, R; returns the frames that fit */
void spu_cd_idle(int idle);                 /* the drive has stopped sending: play out what's left */

void spu_reset(void);       /* power-on state (after the intro) */
void spu_set_sink(void (*fn)(const s16 *out, int frames)); /* SPU_CLOCK_HOST: where its samples go */

void port_audio_init(void); /* audio.c: open the audio device (not with TWINE_HEADLESS) */
void port_audio_wav(const s16 *samples, int frames); /* audio.c: into TWINE_WAV (headless), if set */

#endif
