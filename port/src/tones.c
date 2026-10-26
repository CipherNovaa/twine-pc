/* Another mission's sounds: a tone of a level's sound bank (SOUNDFX/LnFX.VH and .VB in Lnn/LVLnnCD.BIN), decoded
 * from its ADPCM once and played by the runtime's own mixer (spu.c's port_sfx_play) at the pitch a note gives it,
 * looping as the bank's sample loops. The banks fill the SPU's RAM, so a mission can't hold another's samples:
 * endless mode's lent weapons fire with their own missions' sounds this way. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "port.h"

u32 port_disc_file(const char *name, u8 **out);
int port_sfx_play(const s16 *pcm, u32 n, u32 rate, s32 vol, s32 loop_at);

#define RD16(p, o) ((u32)(p)[o] | (u32)(p)[(o) + 1] << 8)
#define RD32(p, o) (RD16(p, o) | RD16(p, (o) + 2) << 16)

/* a tone's samples (at most max), its loop's start (-1: none) and its centre note (x128, with its fine tune) */
static u32 decode(const u8 *vh, const u8 *vb, u32 program, u32 tone, s16 *out, u32 max, s32 *loop, s32 *centre) {
    static const s32 pos[5] = {0, 60, 115, 98, 122}, neg[5] = {0, 0, -52, -55, -60};
    u32 programs = RD16(vh, 0x12), tones = 0x20 + 0x800, vag, i, off = 0, size, b, k, n = 0;
    const u8 *sizes = vh + tones + programs * 16 * 32, *atr;
    s32 s1 = 0, s2 = 0, sample, filter, shift;
    *loop = -1;
    if (RD32(vh, 0) != 0x56414270 || program >= programs) /* "pBAV" */
        return 0;
    atr = vh + tones + (program * 16 + tone) * 32;
    vag = RD16(atr, 0x16);
    *centre = atr[4] * 128 + atr[5];
    if (!vag)
        return 0;
    for (i = 1; i < vag; i++)
        off += RD16(sizes, i * 2) * 8;
    size = RD16(sizes, vag * 2) * 8;
    for (b = 0; b < size / 16 && n + 28 <= max; b++) {
        const u8 *blk = vb + off + b * 16;
        shift = blk[0] & 15;
        filter = blk[0] >> 4 > 4 ? 0 : blk[0] >> 4;
        if (blk[1] & 4)
            *loop = (s32)n;
        for (k = 0; k < 28; k++) {
            sample = (s16)((blk[2 + k / 2] >> (k & 1) * 4 & 15) << 12) >> shift;
            sample += (s1 * pos[filter] + s2 * neg[filter] + 32) >> 6;
            sample = sample > 32767 ? 32767 : sample < -32768 ? -32768 : sample;
            s2 = s1;
            s1 = sample;
            out[n++] = (s16)sample;
        }
        if (blk[1] & 1) {
            if (!(blk[1] & 2)) /* (an end without a repeat: no loop) */
                *loop = -1;
            break;
        }
    }
    return n;
}

#define TONES 24
static struct {
    s8 level, program, tone;
    s16 *pcm;
    u32 len;
    s32 loop, centre;
} tones[TONES];

/* a tone, decoded once (NULL: no such tone) */
static int find(int level, int program, int tone) {
    int k;
    u8 *arc, *e, *vh = NULL, *vb = NULL;
    u32 n;
    for (k = 0; k < TONES && tones[k].pcm; k++)
        if (tones[k].level == level && tones[k].program == program && tones[k].tone == tone)
            return k;
    if (k == TONES)
        return -1;
    {
        char name[32];
        snprintf(name, sizeof(name), "L%02d/LVL%02dCD.BIN", level, level);
        if (!(n = port_disc_file(name, &arc)))
            return -1;
    }
    for (e = arc; e + 0x1C <= arc + n && *e; e += 0x1C) {
        const char *nm = (const char *)e;
        if (!strncmp(nm, "SOUNDFX/", 8) && strstr(nm, ".VH"))
            vh = arc + RD16(e, 26) * 0x800;
        else if (!strncmp(nm, "SOUNDFX/", 8) && strstr(nm, ".VB"))
            vb = arc + RD16(e, 26) * 0x800;
    }
    if (vh && vb && (tones[k].pcm = malloc(2 * 65536))) {
        tones[k].len = decode(vh, vb, (u32)program, (u32)tone, tones[k].pcm, 65536, &tones[k].loop, &tones[k].centre);
        tones[k].level = (s8)level, tones[k].program = (s8)program, tones[k].tone = (s8)tone;
        port_log("tones: level %d program %d tone %d: %u samples, loop %d, centre %d/128\n", level, program, tone,
                 tones[k].len, tones[k].loop, tones[k].centre);
    }
    free(arc);
    return tones[k].pcm && tones[k].len ? k : -1;
}

/* play it: note < 0 at the rate the missions' voices are made at (11025 Hz); else at that note (a sample plays at
 * 44100 Hz at its centre note); vol 0..0x7FFF; looping as the sample does if loop. A handle (spu.c's), 0 none. */
int port_tone_play(int level, int program, int tone, int note, s32 vol, int loop) {
    int k = find(level, program, tone);
    u32 rate;
    if (k < 0)
        return 0;
    rate = note < 0 ? 11025 : (u32)(44100.0 * pow(2.0, (note * 128 - tones[k].centre) / (12.0 * 128)));
    if (rate < 1000)
        rate = 1000;
    if (rate > 176400)
        rate = 176400;
    return port_sfx_play(tones[k].pcm, tones[k].len, rate, vol, loop ? tones[k].loop : -1);
}
