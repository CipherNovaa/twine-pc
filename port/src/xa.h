#ifndef XA_H
#define XA_H

/* The CD drive's XA-ADPCM audio (xa.c): cdrom.c's read head hands it the audio sectors the drive accepts;
 * it decodes them and plays them, at the drive's 44100 Hz, into the SPU's CD audio input (spu.c). All of
 * it runs on the game thread (libcd calls and the CD interrupt). */
#include "port.h"

void xa_sector(const u8 *raw, u64 t_ns); /* a raw 2352-byte audio sector the head read at host time t_ns */
void xa_run(u64 now_ns);                 /* play the decoded audio due by now_ns into the SPU */
int xa_playing(void);                    /* decoded audio is still being played */
void xa_flush(void);                     /* a read at a new location: drop decoded audio, reset the decoder */
void xa_set_mute(int on);                /* Mute / Demute commands */
void xa_set_mix(const u8 *atv);          /* CdMix: L->L, L->R, R->R, R->L (80h = 1.0) */

/* Decoder core: one sector's 18 sound groups to 16-bit samples at the sector's own rate (stereo interleaved
 * L, R), continuing the ADPCM history in hist[channel][old, older]. Returns samples per channel. */
int xa_decode(const u8 *raw, s16 *out, s32 hist[2][2]);

#endif
