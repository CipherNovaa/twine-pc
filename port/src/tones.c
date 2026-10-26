/* Another mission's sounds: a tone of a level's sound bank (SOUNDFX/LnFX.VH and .VB in Lnn/LVLnnCD.BIN), decoded
 * from its ADPCM once and played by the runtime's own mixer (spu.c's port_sfx_play) at the pitch a note gives it,
 * looping as the bank's sample loops. The banks fill the SPU's RAM, so a mission can't hold another's samples:
 * endless mode's lent weapons fire with their own missions' sounds this way. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "port.h"

int port_sfx_play(const s16 *pcm, u32 n, u32 rate, s32 vol, s32 loop_at);
int port_tone_play_pcm(int level, int program, int tone, int note, s32 vol, int loop);

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
    *centre = atr[4] * 128 - atr[5]; /* (the fine tune raises the pitch: the sound library's, as measured: a key on at home) */
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

#define TONES 64
static struct {
    s8 level, program, tone;
    s16 *pcm;
    u32 len;
    s32 loop, centre;
} tones[TONES];

/* Each level's sound bank, read from the disc once: the archive's directory (entries of 0x1C bytes: the name, the
 * first sector at +26; a file runs to the next one's first sector), then its VH and VB alone (reading the whole
 * archive, megabytes, at the first sound of a level stalled play) */
static struct {
    u8 *vh, *vb;
    int tried;
} banks[16];

int port_sound_bank(int level, const u8 **vh, const u8 **vb) {
    char name[32];
    u8 *dir, *e, *f;
    u32 n, vh_at = 0, vb_at = 0, vh_end = 0, vb_end = 0, at, size;
    u64 t;
    if (level < 0 || level >= 16)
        return 0;
    if (!banks[level].tried) {
        banks[level].tried = 1;
        t = PT_BEGIN();
        snprintf(name, sizeof(name), "L%02d/LVL%02dCD.BIN", level, level);
        if ((n = port_disc_part(name, 0, 0x4000, &dir))) {
            for (e = dir; e + 0x1C <= dir + n && *e; e += 0x1C) {
                at = RD16(e, 26) * 0x800;
                if (!strncmp((char *)e, "SOUNDFX/", 8) && strstr((char *)e, ".VH"))
                    vh_at = at;
                else if (!strncmp((char *)e, "SOUNDFX/", 8) && strstr((char *)e, ".VB"))
                    vb_at = at;
            }
            for (e = dir; e + 0x1C <= dir + n && *e; e += 0x1C) { /* (each file's end: the next file's start) */
                at = RD16(e, 26) * 0x800;
                if (vh_at && at > vh_at && (!vh_end || at < vh_end))
                    vh_end = at;
                if (vb_at && at > vb_at && (!vb_end || at < vb_end))
                    vb_end = at;
            }
            free(dir);
            if (vh_at && vb_at) {
                size = vh_end ? vh_end - vh_at : 0x10000;
                if (port_disc_part(name, vh_at, size, &f))
                    banks[level].vh = f;
                size = vb_end ? vb_end - vb_at : 0x100000; /* (the last file: to the archive's end) */
                if (banks[level].vh && port_disc_part(name, vb_at, size, &f))
                    banks[level].vb = f;
            }
        }
        port_timing_count(PT_BANK_READS);
        PT_END(PT_SOUND_LOAD, t);
        port_log("tones: level %d's sound bank %s\n", level, banks[level].vb ? "read" : "not found");
    }
    if (!banks[level].vb)
        return 0;
    *vh = banks[level].vh, *vb = banks[level].vb;
    return 1;
}

/* a tone, decoded once (NULL: no such tone) */
static int find(int level, int program, int tone) {
    int k;
    const u8 *vh, *vb;
    u64 t;
    for (k = 0; k < TONES && tones[k].pcm; k++)
        if (tones[k].level == level && tones[k].program == program && tones[k].tone == tone)
            return k;
    if (k == TONES)
        return -1;
    if (!port_sound_bank(level, &vh, &vb))
        return -1;
    t = PT_BEGIN();
    if ((tones[k].pcm = malloc(2 * 65536))) {
        tones[k].len = decode(vh, vb, (u32)program, (u32)tone, tones[k].pcm, 65536, &tones[k].loop, &tones[k].centre);
        tones[k].level = (s8)level, tones[k].program = (s8)program, tones[k].tone = (s8)tone;
        port_log("tones: level %d program %d tone %d: %u samples, loop %d, centre %d/128\n", level, program, tone,
                 tones[k].len, tones[k].loop, tones[k].centre);
    }
    PT_END(PT_SOUND_LOAD, t);
    return tones[k].pcm && tones[k].len ? k : -1;
}

int port_xvoice_play(const u8 *adpcm, u32 size, u16 pitch, u16 adsr1, u16 adsr2, u16 voll, u16 volr, int reverb);

/* play it as the game's sound library keys a voice on (spu.c's port_xvoice_play: a voice as the SPU's own, from the
 * bank's ADPCM), as measured against its key-ons at home (TWINE_TRACE=sound,spu): the pitch the note gives at the
 * tone's centre less its fine tune (4096 at the centre), the tone's ADSR, the left and right volumes as given (0..127,
 * the game's own for the sound: x 0x3FFF/127), reverb by the tone's mode. note < 0: 11025 Hz (the voices' rate). loop:
 * ignored (the ADPCM's own loop flags loop it, as on the SPU). A handle (spu.c's), 0 none. */
int port_tone_play(int level, int program, int tone, int note, s32 l, s32 r, int loop) {
    const u8 *vh, *vb, *atr, *sizes;
    u32 programs, i, off = 0, size, vag, pitch;
    s32 centre;
    (void)loop;
    if (getenv("TWINE_TEST_TONES_PCM")) /* (tests: the old way, raw PCM, to compare) */
        return port_tone_play_pcm(level, program, tone, note, (l > r ? l : r) * 0x7F, loop);
    if (!port_sound_bank(level, &vh, &vb) || RD32(vh, 0) != 0x56414270 || (u32)program >= RD16(vh, 0x12))
        return 0;
    programs = RD16(vh, 0x12);
    atr = vh + 0x20 + 0x800 + (program * 16 + tone) * 32;
    sizes = vh + 0x20 + 0x800 + programs * 16 * 32;
    vag = RD16(atr, 0x16);
    if (!vag)
        return 0;
    for (i = 1; i < vag; i++)
        off += RD16(sizes, i * 2) * 8;
    size = RD16(sizes, vag * 2) * 8;
    centre = atr[4] * 128 - atr[5]; /* (the fine tune raises the pitch: the sound library's, as measured: a key on at home) */
    pitch = note < 0 ? 0x400 : (u32)(4096.0 * pow(2.0, (note * 128 - centre) / (12.0 * 128)) + 0.5);
    if (pitch > 0x3FFF)
        pitch = 0x3FFF;
    l = l < 0 ? 0 : l > 127 ? 127 : l; /* (the voice's volumes: the game's left and right as given, as the sound
                                         * library keys a sound on: neither the tone's nor the program's volume) */
    r = r < 0 ? 0 : r > 127 ? 127 : r;
    {
        int h = port_xvoice_play(vb + off, size, (u16)pitch, (u16)RD16(atr, 0x10), (u16)RD16(atr, 0x12),
                                 (u16)(l * 0x3FFF / 127), (u16)(r * 0x3FFF / 127), (atr[1] & 4) != 0);
        static int on = -1;
        if (on < 0)
            on = port_trace("sound");
        if (on)
            port_log("trace: sound: tone level %d program %d tone %d note %d: a voice, pitch %04X adsr %04X %04X "
                     "volume %d %d%s: handle %d\n", level, program, tone, note, pitch, RD16(atr, 0x10), RD16(atr, 0x12),
                     l, r, (atr[1] & 4) ? ", reverb" : "", h);
        return h;
    }
}

/* (the old way: decoded to PCM, mixed raw, nearest sample: TWINE_TEST_TONES_PCM) */
int port_tone_play_pcm(int level, int program, int tone, int note, s32 vol, int loop) {
    int k = find(level, program, tone);
    u32 rate;
    if (k < 0)
        return 0;
    rate = note < 0 ? 11025 : (u32)(44100.0 * pow(2.0, (note * 128 - tones[k].centre) / (12.0 * 128)));
    if (rate < 1000)
        rate = 1000;
    if (rate > 176400)
        rate = 176400;
    {
        int h = port_sfx_play(tones[k].pcm, tones[k].len, rate, vol, loop ? tones[k].loop : -1);
        static int on = -1;
        if (on < 0)
            on = port_trace("sound");
        if (on)
            port_log("trace: sound: tone level %d program %d tone %d note %d at %u Hz, volume %d%s: handle %d\n", level,
                     program, tone, note, rate, vol, loop ? " looping" : "", h);
        return h;
    }
}
