/* The CD drive's XA-ADPCM player, written from psx-spx ("CDROM XA Audio ADPCM Compression", "CDROM XA
 * Subheader, File, Channel, Interleave", "CDROM Controller I/O Ports"): the audio sectors the read head accepts
 * (cdrom.c) are decoded (4/8-bit, mono/stereo, 37800/18900 Hz) into a FIFO, and the drive's output stage
 * takes them from there at 44100 Hz: the 37800 -> 44100 Hz zigzag interpolation (seven 29-tap tables, one
 * extra sample after every six; 18900 Hz samples go in twice), the CdMix volume matrix and the mute.
 *
 * The output runs on host time, like the head: its clock starts when the first sector of a run has been
 * read, so each sector's audio is due exactly when the interleave delivers the next one, whenever the CD
 * interrupt gets to run. What it produces goes into the SPU's CD input ring (spu.c), kept short: after a
 * stall (the game holding interrupts off) the overdue audio is dropped instead of adding latency. */
#include <string.h>
#include "port.h"
#include "spu.h"
#include "xa.h"

#define FIFO_SIZE 32768u        /* decoded 37800 Hz stereo samples (a sector holds at most 8064) */
#define LEAD_NS 2000000u        /* the output starts this long after the first sector has been read */

static s16 fifo[FIFO_SIZE][2];
static u32 fifo_rd, fifo_wr;
static s32 hist[2][2];          /* ADPCM old/older, left (mono) and right */

/* zigzag interpolation: 32 most recent 37800 Hz samples per channel, the six-step counter, and the seven
 * 44100 Hz samples of the last step not yet output */
static s16 zz_ring[2][32];
static u32 zz_p;
static int zz_six = 6;
static s16 zz_out[7][2];
static int zz_have, zz_pos;

/* output stage */
static u8 mix[4] = {0x80, 0, 0x80, 0}; /* L->L, L->R, R->R, R->L */
static int muted;
static int out_on;              /* playing; else waiting for decoded audio */
static u64 out_t0, out_frames;  /* host time of output frame 0, frames done since */
static u8 last_ci = 0xFF;       /* coding info of the last sector, for the log */

/* psx-spx "25-point Zigzag Interpolation": Table1..Table7, taps for the samples 1..29 back */
static const s16 zigzag[7][29] = {
    {0, 0, 0, 0, 0, -0x0002, 0x000A, -0x0022, 0x0041, -0x0054, 0x0034, 0x0009, -0x010A, 0x0400, -0x0A78,
     0x234C, 0x6794, -0x1780, 0x0BCD, -0x0623, 0x0350, -0x016D, 0x006B, 0x000A, -0x0010, 0x0011, -0x0008,
     0x0003, -0x0001},
    {0, 0, 0, -0x0002, 0, 0x0003, -0x0013, 0x003C, -0x004B, 0x00A2, -0x00E3, 0x0132, -0x0043, -0x0267, 0x0C9D,
     0x74BB, -0x11B4, 0x09B8, -0x05BF, 0x0372, -0x01A8, 0x00A6, -0x001B, 0x0005, 0x0006, -0x0008, 0x0003,
     -0x0001, 0},
    {0, 0, -0x0001, 0x0003, -0x0002, -0x0005, 0x001F, -0x004A, 0x00B3, -0x0192, 0x02B1, -0x039E, 0x04F8, -0x05A6,
     0x7939, -0x05A6, 0x04F8, -0x039E, 0x02B1, -0x0192, 0x00B3, -0x004A, 0x001F, -0x0005, -0x0002, 0x0003,
     -0x0001, 0, 0},
    {0, -0x0001, 0x0003, -0x0008, 0x0006, 0x0005, -0x001B, 0x00A6, -0x01A8, 0x0372, -0x05BF, 0x09B8, -0x11B4,
     0x74BB, 0x0C9D, -0x0267, -0x0043, 0x0132, -0x00E3, 0x00A2, -0x004B, 0x003C, -0x0013, 0x0003, 0, -0x0002,
     0, 0, 0},
    {-0x0001, 0x0003, -0x0008, 0x0011, -0x0010, 0x000A, 0x006B, -0x016D, 0x0350, -0x0623, 0x0BCD, -0x1780,
     0x6794, 0x234C, -0x0A78, 0x0400, -0x010A, 0x0009, 0x0034, -0x0054, 0x0041, -0x0022, 0x000A, -0x0001, 0,
     0x0001, 0, 0, 0},
    {0x0002, -0x0008, 0x0010, -0x0023, 0x002B, 0x001A, -0x00EB, 0x027B, -0x0548, 0x0AFA, -0x16FA, 0x53E0,
     0x3C07, -0x1249, 0x080E, -0x0347, 0x015B, -0x0044, -0x0017, 0x0046, -0x0023, 0x0011, -0x0005, 0, 0, 0, 0,
     0, 0},
    {-0x0005, 0x0011, -0x0023, 0x0046, -0x0017, -0x0044, 0x015B, -0x0347, 0x080E, -0x1249, 0x3C07, 0x53E0,
     -0x16FA, 0x0AFA, -0x0548, 0x027B, -0x00EB, 0x001A, 0x002B, -0x0023, 0x0010, -0x0008, 0x0002, 0, 0, 0, 0,
     0, 0},
};

static inline s32 clamp16(s32 v) {
    return v < -0x8000 ? -0x8000 : v > 0x7FFF ? 0x7FFF : v;
}

/* ---- decoder ---- */

/* One sound unit (block) of a 128-byte sound group: 28 samples. Header: shift (13..15 act as 9) and one of
 * the four filters. 4-bit units are nibbles of the data words (unit u in byte u/2, low nibble for even u),
 * 8-bit units bytes; either is expanded to 16 bits and shifted right. */
static void decode_unit(const u8 *g, int u, int bits8, s32 *h, s16 *dst, int stride) {
    u8 head = g[4 + u];
    int shift = head & 0xF, filter = (head >> 4) & 3, j;
    s32 f0 = adpcm_filter_pos[filter], f1 = adpcm_filter_neg[filter], old = h[0], older = h[1];
    if (shift > 12)
        shift = 9;
    for (j = 0; j < 28; j++) {
        s32 t, s;
        if (bits8)
            t = (s16)(g[16 + j * 4 + u] << 8);
        else
            t = (s16)(((g[16 + j * 4 + u / 2] >> ((u & 1) * 4)) & 0xF) << 12);
        s = clamp16((t >> shift) + ((old * f0 + older * f1 + 32) >> 6));
        dst[j * stride] = (s16)s;
        older = old;
        old = s;
    }
    h[0] = old;
    h[1] = older;
}

int xa_decode(const u8 *raw, s16 *out, s32 h[2][2]) {
    u8 ci = raw[19];
    int stereo = (ci & 3) == 1, bits8 = ((ci >> 4) & 3) == 1, units = bits8 ? 4 : 8, n = 0, grp, u;
    const u8 *g = raw + 24;
    for (grp = 0; grp < 18; grp++, g += 128) {
        for (u = 0; u < units; u++) {
            if (stereo) /* even units left, odd right */
                decode_unit(g, u, bits8, h[u & 1], out + (n + (u >> 1) * 28) * 2 + (u & 1), 2);
            else
                decode_unit(g, u, bits8, h[0], out + n + u * 28, 1);
        }
        n += (stereo ? units / 2 : units) * 28;
    }
    return n;
}

static void fifo_put(s16 l, s16 r) {
    if (fifo_wr - fifo_rd >= FIFO_SIZE)
        return; /* the output has fallen far behind the head: lose the newest */
    fifo[fifo_wr % FIFO_SIZE][0] = l;
    fifo[fifo_wr % FIFO_SIZE][1] = r;
    fifo_wr++;
}

void xa_sector(const u8 *raw, u64 t_ns) {
    static s16 pcm[4032 * 2];
    u8 ci = raw[19];
    int stereo = (ci & 3) == 1, half = (ci >> 2) & 1, n, i;
    if (ci != last_ci) { /* the first sector of a run, or a new format */
        last_ci = ci;
        port_log("xa: file %u channel %u: %s %u Hz %d-bit%s\n", raw[16], raw[17], stereo ? "stereo" : "mono",
                 half ? 18900 : 37800, ((ci >> 4) & 3) == 1 ? 8 : 4, ci & 0x40 ? ", emphasis (not emulated)" : "");
    }
    if (raw[18] & 0x80)
        port_log("xa: file %u channel %u: end of file\n", raw[16], raw[17]);
    n = xa_decode(raw, pcm, hist);
    for (i = 0; i < n; i++) {
        s16 l = stereo ? pcm[i * 2] : pcm[i], r = stereo ? pcm[i * 2 + 1] : l;
        fifo_put(l, r);
        if (half) /* 18900 Hz: every sample twice at 37800 Hz */
            fifo_put(l, r);
    }
    if (!out_on) {
        out_on = 1;
        out_t0 = t_ns + LEAD_NS;
        out_frames = 0;
        spu_cd_idle(0);
    }
}

/* ---- output stage ---- */

/* The next 44100 Hz output sample, from the zigzag interpolation; 0 when the decoded audio has run out. */
static int zigzag_next(s16 *lr) {
    while (zz_pos == zz_have) {
        u32 p;
        int t, c, i;
        if (fifo_rd == fifo_wr)
            return 0;
        zz_ring[0][zz_p & 31] = fifo[fifo_rd % FIFO_SIZE][0];
        zz_ring[1][zz_p & 31] = fifo[fifo_rd % FIFO_SIZE][1];
        fifo_rd++;
        zz_p++;
        if (--zz_six)
            continue;
        zz_six = 6;
        p = zz_p;
        for (t = 0; t < 7; t++) {
            for (c = 0; c < 2; c++) {
                s32 sum = 0;
                for (i = 1; i <= 29; i++)
                    sum += (zz_ring[c][(p - i) & 31] * zigzag[t][i - 1]) >> 15;
                zz_out[t][c] = (s16)clamp16(sum);
            }
        }
        zz_have = 7;
        zz_pos = 0;
    }
    lr[0] = zz_out[zz_pos][0];
    lr[1] = zz_out[zz_pos][1];
    if (++zz_pos == zz_have)
        zz_pos = zz_have = 0;
    return 1;
}

void xa_run(u64 now_ns) {
    static s16 buf[512 * 2];
    u64 due;
    s32 room;
    int n = 0;
    if (!out_on || now_ns <= out_t0)
        return;
    spu_sync(); /* without an audio device, let the SPU take what is due first */
    due = (now_ns - out_t0) * 44100 / 1000000000ull;
    room = SPU_CD_TARGET - spu_cd_queued();
    while (out_frames < due) {
        s16 lr[2];
        if (!zigzag_next(lr)) { /* run out: wait for the next sector, the SPU plays out what it has */
            out_on = 0;
            spu_cd_idle(1);
            break;
        }
        out_frames++;
        if (room <= 0)
            continue; /* overdue after a stall: dropped, to stay in time */
        room--;
        if (muted) {
            buf[n * 2] = buf[n * 2 + 1] = 0;
        } else { /* the CdMix matrix; it saturates (fine up to double volume) */
            buf[n * 2] = (s16)clamp16((lr[0] * mix[0] + lr[1] * mix[3]) >> 7);
            buf[n * 2 + 1] = (s16)clamp16((lr[1] * mix[2] + lr[0] * mix[1]) >> 7);
        }
        if (++n == 512) {
            spu_cd_push(buf, n);
            n = 0;
        }
    }
    if (n)
        spu_cd_push(buf, n);
}

int xa_playing(void) {
    return out_on;
}

void xa_flush(void) {
    fifo_rd = fifo_wr;
    zz_have = zz_pos = 0;
    zz_six = 6;
    memset(zz_ring, 0, sizeof(zz_ring));
    memset(hist, 0, sizeof(hist));
    last_ci = 0xFF;
    if (out_on) {
        out_on = 0;
        spu_cd_idle(1);
    }
}

void xa_set_mute(int on) {
    muted = on;
}

void xa_set_mix(const u8 *atv) {
    memcpy(mix, atv, 4);
}
