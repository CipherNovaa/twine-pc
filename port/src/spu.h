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

void port_audio_init(void); /* audio.c: open the audio device (not with TWINE_HEADLESS) */

#endif
