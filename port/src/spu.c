/* SPU (Sound Processing Unit), written from the psx-spx documentation ("Sound Processing Unit (SPU)"; the
 * ADPCM filters from "CDROM - XA-ADPCM", which the SPU format shares). The recompiled libspu/libsnd program
 * it through its registers (io.c forwards 0x1F801C00-0x1F801FFF here); sound RAM is filled by DMA (io.c)
 * and by the manual-write FIFO here.
 *
 * Emulated at the SPU's own rate, one tick per 44100 Hz sample: 24 voices (SPU-ADPCM blocks with their
 * loop flags and ENDX, the pitch counter with pitch modulation and 4-point Gaussian interpolation, noise,
 * the ADSR envelope readable as ENVX, fixed or sweeping left/right volume), the main volume and the reverb
 * unit (22050 Hz, its work area in sound RAM, resampled through the documented 39-tap filter), and the CD
 * audio input (the drive's XA-ADPCM output, xa.c) with its volume, enable and reverb bits.
 *
 * Key on/off take effect when written (the hardware applies them at its next tick).
 *
 * Not emulated: external audio input, the capture buffers (sound RAM 0-0xFFF), the SPU interrupt (IRQA),
 * and the transfer timing (io.c completes transfers at once).
 *
 * TWINE_XA_WAV=file records the CD audio input as the SPU takes it in (44100 Hz stereo, before the CD
 * volume), TWINE_SPU_WAV=file the SPU's whole output. */
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL3/SDL.h>
#include "port.h"
#include "io.h"
#include "spu.h"

/* registers, as halfword indices from 0x1F801C00 */
#define REG_MVOLL 0xC0  /* 0x1F801D80 main volume left (right follows) */
#define REG_EVOLL 0xC2  /* 0x1F801D84 reverb output volume left (right follows) */
#define REG_KON 0xC4    /* 0x1F801D88 */
#define REG_KOFF 0xC6   /* 0x1F801D8C */
#define REG_PMON 0xC8   /* 0x1F801D90 pitch modulation */
#define REG_NON 0xCA    /* 0x1F801D94 noise */
#define REG_EON 0xCC    /* 0x1F801D98 reverb input */
#define REG_ENDX 0xCE   /* 0x1F801D9C */
#define REG_ESA 0xD1    /* 0x1F801DA2 reverb work area start */
#define REG_XADDR 0xD3  /* 0x1F801DA6 */
#define REG_FIFO 0xD4   /* 0x1F801DA8 */
#define REG_ATTR 0xD5   /* 0x1F801DAA SPUCNT */
#define REG_CDVOL 0xD8  /* 0x1F801DB0 CD audio input volume left (right follows) */
#define REG_MVOLX 0xDC  /* 0x1F801DB8 current main volume left (right follows) */
#define REG_REVERB 0xE0 /* 0x1F801DC0 reverb configuration */
#define REG_VOLX 0x100  /* 0x1F801E00 current voice volumes, left/right per voice */

#define ATTR_ENABLE 0x8000
#define ATTR_UNMUTE 0x4000
#define ATTR_REVERB 0x0080 /* reverb master enable: writes to the work area */
#define ATTR_CD_REVERB 0x0004
#define ATTR_CD 0x0001     /* CD audio enable */

/* per voice, at 0x1F801C00 + n * 16 */
enum { V_VOLL, V_VOLR, V_PITCH, V_SSA, V_ADSR1, V_ADSR2, V_ENVX, V_LSAX };

/* reverb configuration, from 0x1F801DC0 */
enum {
    dAPF1, dAPF2, vIIR, vCOMB1, vCOMB2, vCOMB3, vCOMB4, vWALL, vAPF1, vAPF2, mLSAME, mRSAME, mLCOMB1, mRCOMB1,
    mLCOMB2, mRCOMB2, dLSAME, dRSAME, mLDIFF, mRDIFF, mLCOMB3, mRCOMB3, mLCOMB4, mRCOMB4, dLDIFF, dRDIFF,
    mLAPF1, mRAPF1, mLAPF2, mRAPF2, vLIN, vRIN
};

enum { ENV_OFF, ENV_ATTACK, ENV_DECAY, ENV_SUSTAIN, ENV_RELEASE };

typedef struct {
    u32 addr;        /* ADPCM block being played (byte address in sound RAM) */
    u32 counter;     /* pitch counter: bits 12+ = sample within the block, bits 4-11 = interpolation index */
    u8 flags;        /* loop flags of that block */
    s16 buf[3 + 28]; /* the previous block's last 3 samples, then this block's 28 */
    int phase;       /* ADSR phase */
    s32 level;       /* ADSR level (ENVX) */
    u32 env_counter;
    s32 vol[2];      /* current left/right volume (VOLX) */
    u32 vol_counter[2];
} Voice;

static Voice voices[24];
static u32 exe_gen, v_gen[24]; /* (executables started; the one each voice was keyed on in) */
static u32 endx;
static s32 main_vol[2];
static u32 main_vol_counter[2];
static s32 noise_timer;
static u16 noise_level;

/* reverb */
static u32 rv_cur;                     /* current buffer address (bytes) */
static s16 rv_in[2][128], rv_out[2][128]; /* 44100 Hz filter histories, each sample stored twice (+64) */
static u32 rv_pos;
static int rv_write_on;

/* clocking and the lock that guards all of the above (and spu_regs) against the audio thread */
static int clock_mode PORT_NOSAVE;
static SDL_Mutex *mutex PORT_NOSAVE;
static u64 host_start_ns PORT_NOSAVE, host_samples PORT_NOSAVE;

/* CD audio input ring: the game thread writes cd_wr, the SPU's thread cd_rd (single producer, single
 * consumer; the indexes count frames and wrap) */
#define CD_RING 16384u
#define CD_PREBUFFER 2048u /* frames queued before the SPU starts taking them (after running dry) */
static s16 cd_ring[CD_RING][2];
static atomic_uint cd_wr, cd_rd;
static atomic_int cd_idle = 1;
static int cd_running; /* SPU side: taking frames (else waiting for CD_PREBUFFER) */

/* WAV recordings (TWINE_XA_WAV, TWINE_SPU_WAV), written by whichever thread runs the SPU */
typedef struct {
    const char *env;
    FILE *f;
    int tried;
    u32 frames, n;
    s16 buf[4096 * 2];
} Wav;
static Wav xa_wav PORT_NOSAVE = {"TWINE_XA_WAV"}, spu_wav PORT_NOSAVE = {"TWINE_SPU_WAV"};

/* 4-point Gaussian interpolation table (psx-spx "4-Point Gaussian Interpolation") */
static const s16 gauss[512] = {
    -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, -0x001,
    -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, -0x001, -0x001,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001,
    0x0001, 0x0001, 0x0001, 0x0002, 0x0002, 0x0002, 0x0003, 0x0003,
    0x0003, 0x0004, 0x0004, 0x0005, 0x0005, 0x0006, 0x0007, 0x0007,
    0x0008, 0x0009, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E,
    0x000F, 0x0010, 0x0011, 0x0012, 0x0013, 0x0015, 0x0016, 0x0018,
    0x0019, 0x001B, 0x001C, 0x001E, 0x0020, 0x0021, 0x0023, 0x0025,
    0x0027, 0x0029, 0x002C, 0x002E, 0x0030, 0x0033, 0x0035, 0x0038,
    0x003A, 0x003D, 0x0040, 0x0043, 0x0046, 0x0049, 0x004D, 0x0050,
    0x0054, 0x0057, 0x005B, 0x005F, 0x0063, 0x0067, 0x006B, 0x006F,
    0x0074, 0x0078, 0x007D, 0x0082, 0x0087, 0x008C, 0x0091, 0x0096,
    0x009C, 0x00A1, 0x00A7, 0x00AD, 0x00B3, 0x00BA, 0x00C0, 0x00C7,
    0x00CD, 0x00D4, 0x00DB, 0x00E3, 0x00EA, 0x00F2, 0x00FA, 0x0101,
    0x010A, 0x0112, 0x011B, 0x0123, 0x012C, 0x0135, 0x013F, 0x0148,
    0x0152, 0x015C, 0x0166, 0x0171, 0x017B, 0x0186, 0x0191, 0x019C,
    0x01A8, 0x01B4, 0x01C0, 0x01CC, 0x01D9, 0x01E5, 0x01F2, 0x0200,
    0x020D, 0x021B, 0x0229, 0x0237, 0x0246, 0x0255, 0x0264, 0x0273,
    0x0283, 0x0293, 0x02A3, 0x02B4, 0x02C4, 0x02D6, 0x02E7, 0x02F9,
    0x030B, 0x031D, 0x0330, 0x0343, 0x0356, 0x036A, 0x037E, 0x0392,
    0x03A7, 0x03BC, 0x03D1, 0x03E7, 0x03FC, 0x0413, 0x042A, 0x0441,
    0x0458, 0x0470, 0x0488, 0x04A0, 0x04B9, 0x04D2, 0x04EC, 0x0506,
    0x0520, 0x053B, 0x0556, 0x0572, 0x058E, 0x05AA, 0x05C7, 0x05E4,
    0x0601, 0x061F, 0x063E, 0x065C, 0x067C, 0x069B, 0x06BB, 0x06DC,
    0x06FD, 0x071E, 0x0740, 0x0762, 0x0784, 0x07A7, 0x07CB, 0x07EF,
    0x0813, 0x0838, 0x085D, 0x0883, 0x08A9, 0x08D0, 0x08F7, 0x091E,
    0x0946, 0x096F, 0x0998, 0x09C1, 0x09EB, 0x0A16, 0x0A40, 0x0A6C,
    0x0A98, 0x0AC4, 0x0AF1, 0x0B1E, 0x0B4C, 0x0B7A, 0x0BA9, 0x0BD8,
    0x0C07, 0x0C38, 0x0C68, 0x0C99, 0x0CCB, 0x0CFD, 0x0D30, 0x0D63,
    0x0D97, 0x0DCB, 0x0E00, 0x0E35, 0x0E6B, 0x0EA1, 0x0ED7, 0x0F0F,
    0x0F46, 0x0F7F, 0x0FB7, 0x0FF1, 0x102A, 0x1065, 0x109F, 0x10DB,
    0x1116, 0x1153, 0x118F, 0x11CD, 0x120B, 0x1249, 0x1288, 0x12C7,
    0x1307, 0x1347, 0x1388, 0x13C9, 0x140B, 0x144D, 0x1490, 0x14D4,
    0x1517, 0x155C, 0x15A0, 0x15E6, 0x162C, 0x1672, 0x16B9, 0x1700,
    0x1747, 0x1790, 0x17D8, 0x1821, 0x186B, 0x18B5, 0x1900, 0x194B,
    0x1996, 0x19E2, 0x1A2E, 0x1A7B, 0x1AC8, 0x1B16, 0x1B64, 0x1BB3,
    0x1C02, 0x1C51, 0x1CA1, 0x1CF1, 0x1D42, 0x1D93, 0x1DE5, 0x1E37,
    0x1E89, 0x1EDC, 0x1F2F, 0x1F82, 0x1FD6, 0x202A, 0x207F, 0x20D4,
    0x2129, 0x217F, 0x21D5, 0x222C, 0x2282, 0x22DA, 0x2331, 0x2389,
    0x23E1, 0x2439, 0x2492, 0x24EB, 0x2545, 0x259E, 0x25F8, 0x2653,
    0x26AD, 0x2708, 0x2763, 0x27BE, 0x281A, 0x2876, 0x28D2, 0x292E,
    0x298B, 0x29E7, 0x2A44, 0x2AA1, 0x2AFF, 0x2B5C, 0x2BBA, 0x2C18,
    0x2C76, 0x2CD4, 0x2D33, 0x2D91, 0x2DF0, 0x2E4F, 0x2EAE, 0x2F0D,
    0x2F6C, 0x2FCC, 0x302B, 0x308B, 0x30EA, 0x314A, 0x31AA, 0x3209,
    0x3269, 0x32C9, 0x3329, 0x3389, 0x33E9, 0x3449, 0x34A9, 0x3509,
    0x3569, 0x35C9, 0x3629, 0x3689, 0x36E8, 0x3748, 0x37A8, 0x3807,
    0x3867, 0x38C6, 0x3926, 0x3985, 0x39E4, 0x3A43, 0x3AA2, 0x3B00,
    0x3B5F, 0x3BBD, 0x3C1B, 0x3C79, 0x3CD7, 0x3D35, 0x3D92, 0x3DEF,
    0x3E4C, 0x3EA9, 0x3F05, 0x3F62, 0x3FBD, 0x4019, 0x4074, 0x40D0,
    0x412A, 0x4185, 0x41DF, 0x4239, 0x4292, 0x42EB, 0x4344, 0x439C,
    0x43F4, 0x444C, 0x44A3, 0x44FA, 0x4550, 0x45A6, 0x45FC, 0x4651,
    0x46A6, 0x46FA, 0x474E, 0x47A1, 0x47F4, 0x4846, 0x4898, 0x48E9,
    0x493A, 0x498A, 0x49D9, 0x4A29, 0x4A77, 0x4AC5, 0x4B13, 0x4B5F,
    0x4BAC, 0x4BF7, 0x4C42, 0x4C8D, 0x4CD7, 0x4D20, 0x4D68, 0x4DB0,
    0x4DF7, 0x4E3E, 0x4E84, 0x4EC9, 0x4F0E, 0x4F52, 0x4F95, 0x4FD7,
    0x5019, 0x505A, 0x509A, 0x50DA, 0x5118, 0x5156, 0x5194, 0x51D0,
    0x520C, 0x5247, 0x5281, 0x52BA, 0x52F3, 0x532A, 0x5361, 0x5397,
    0x53CC, 0x5401, 0x5434, 0x5467, 0x5499, 0x54CA, 0x54FA, 0x5529,
    0x5558, 0x5585, 0x55B2, 0x55DE, 0x5609, 0x5632, 0x565B, 0x5684,
    0x56AB, 0x56D1, 0x56F6, 0x571B, 0x573E, 0x5761, 0x5782, 0x57A3,
    0x57C3, 0x57E2, 0x57FF, 0x581C, 0x5838, 0x5853, 0x586D, 0x5886,
    0x589E, 0x58B5, 0x58CB, 0x58E0, 0x58F4, 0x5907, 0x5919, 0x592A,
    0x593A, 0x5949, 0x5958, 0x5965, 0x5971, 0x597C, 0x5986, 0x598F,
    0x5997, 0x599E, 0x59A4, 0x59A9, 0x59AD, 0x59B0, 0x59B2, 0x59B3,
};

/* ADPCM prediction filters (psx-spx "XA-ADPCM", Pos/neg Tables; SPU-ADPCM has five, XA-ADPCM four) */
const s32 adpcm_filter_pos[5] = {0, 60, 115, 98, 122};
const s32 adpcm_filter_neg[5] = {0, 0, -52, -55, -60};

/* reverb resampling filter (psx-spx "Reverb Buffer Resampling") */
static const s16 rv_fir[39] = {
    -0x0001, 0, 0x0002, 0, -0x000A, 0, 0x0023, 0, -0x0067, 0, 0x010A, 0, -0x0268, 0, 0x0534, 0,
    -0x0B90, 0, 0x2806, 0x4000, 0x2806, 0, -0x0B90, 0, 0x0534, 0, -0x0268, 0, 0x010A, 0, -0x0067, 0,
    0x0023, 0, -0x000A, 0, 0x0002, 0, -0x0001,
};

static inline s32 clamp16(s32 v) {
    return v < -0x8000 ? -0x8000 : v > 0x7FFF ? 0x7FFF : v;
}

static inline u32 reg32(int reg) {
    return spu_regs[reg] | (u32)spu_regs[reg + 1] << 16;
}

/* ---- envelopes ---- */

/* One tick of the envelope generator shared by ADSR and volume sweeps (psx-spx "Envelope Operation
 * depending on Shift/Step/Mode/Direction"). */
static s32 envelope_tick(s32 level, u32 *counter, int shift, int step, int exponential, int decrease,
                         int negative) {
    s32 adsr_step = 7 - step;
    u32 inc;
    if (decrease ^ negative)
        adsr_step = ~adsr_step; /* +7,+6,+5,+4 -> -8,-7,-6,-5 */
    adsr_step *= 1 << (shift < 11 ? 11 - shift : 0);
    inc = 0x8000u >> (shift > 11 ? shift - 11 : 0);
    if (exponential && !decrease && level > 0x6000) { /* "exponential" increase: slower above 6000h */
        if (shift < 10)
            adsr_step >>= 2;
        else if (shift >= 11)
            inc >>= 2;
        else {
            adsr_step >>= 1;
            inc >>= 1;
        }
    } else if (exponential && decrease)
        adsr_step = (adsr_step * level) >> 15;
    if ((step | (shift << 2)) != 0x7F && inc < 1)
        inc = 1; /* only the all-ones rate never steps */
    *counter += inc;
    if (!(*counter & 0x8000))
        return level;
    *counter = 0;
    level += adsr_step;
    if (!decrease)
        return clamp16(level);
    if (negative)
        return level < -0x8000 ? -0x8000 : level > 0 ? 0 : level;
    return level < 0 ? 0 : level;
}

/* Voice or main volume register: fixed (bit 15 clear: volume/2) or a sweep from the current volume. */
static s32 volume_tick(u16 reg, s32 cur, u32 *counter) {
    if (!(reg & 0x8000)) {
        *counter = 0;
        return (s16)(reg << 1);
    }
    return envelope_tick(cur, counter, (reg >> 2) & 0x1F, reg & 3, (reg >> 14) & 1, (reg >> 13) & 1,
                         (reg >> 12) & 1);
}

static void adsr_tick(Voice *v, const u16 *reg) {
    u16 a1 = reg[V_ADSR1], a2 = reg[V_ADSR2];
    switch (v->phase) {
    case ENV_ATTACK: /* increase to 7FFFh */
        v->level = envelope_tick(v->level, &v->env_counter, (a1 >> 10) & 0x1F, (a1 >> 8) & 3, a1 >> 15, 0, 0);
        if (v->level >= 0x7FFF) {
            /* sustain level Fh (8000h) is reached at once: no decay */
            v->phase = v->level <= ((a1 & 0xF) + 1) * 0x800 ? ENV_SUSTAIN : ENV_DECAY;
            v->env_counter = 0;
        }
        break;
    case ENV_DECAY: /* exponential decrease (step -8) to the sustain level */
        v->level = envelope_tick(v->level, &v->env_counter, (a1 >> 4) & 0xF, 0, 1, 1, 0);
        if (v->level <= ((a1 & 0xF) + 1) * 0x800) {
            v->phase = ENV_SUSTAIN;
            v->env_counter = 0;
        }
        break;
    case ENV_SUSTAIN: /* until key off */
        v->level = envelope_tick(v->level, &v->env_counter, (a2 >> 8) & 0x1F, (a2 >> 6) & 3, a2 >> 15,
                                 (a2 >> 14) & 1, 0);
        break;
    case ENV_RELEASE: /* decrease (step -8) to 0 */
        v->level = envelope_tick(v->level, &v->env_counter, a2 & 0x1F, 0, (a2 >> 5) & 1, 1, 0);
        if (v->level <= 0) {
            v->level = 0;
            v->phase = ENV_OFF;
        }
        break;
    }
}

/* ---- voices ---- */

/* Decode the 16-byte ADPCM block at v->addr: header (shift/filter, loop flags), then 28 4-bit samples. */
static void decode_block(Voice *v, int n) {
    u32 a = v->addr & 0x7FFF8, i;
    u8 b[16];
    int shift, filter;
    s32 old = v->buf[30], older = v->buf[29], f0, f1;
    for (i = 0; i < 16; i++)
        b[i] = spu_ram[(a + i) & 0x7FFFF];
    shift = b[0] & 0xF;
    if (shift > 12)
        shift = 9; /* reserved shift values act as 9 */
    filter = (b[0] >> 4) & 7;
    if (filter > 4)
        filter = 0; /* undocumented filter values: no prediction */
    f0 = adpcm_filter_pos[filter];
    f1 = adpcm_filter_neg[filter];
    v->buf[0] = v->buf[28];
    v->buf[1] = v->buf[29];
    v->buf[2] = v->buf[30];
    for (i = 0; i < 28; i++) {
        u32 nib = (b[2 + i / 2] >> ((i & 1) * 4)) & 0xF;
        s32 s = ((s16)(nib << 12) >> shift) + ((old * f0 + older * f1 + 32) >> 6);
        s = clamp16(s);
        v->buf[3 + i] = s;
        older = old;
        old = s;
    }
    v->flags = b[1];
    if (b[1] & 4) /* loop start: the repeat address register takes this block's address */
        spu_regs[n * 8 + V_LSAX] = a >> 3;
}

/* After the last sample of a block: loop end jumps to the repeat address (and mutes the voice unless the
 * repeat bit is set), otherwise on to the next block. */
static void next_block(Voice *v, int n) {
    if (v->flags & 1) {
        endx |= 1u << n;
        v->addr = spu_regs[n * 8 + V_LSAX] * 8u;
        if (!(v->flags & 2)) {
            v->phase = ENV_OFF;
            v->level = 0;
        }
    } else
        v->addr = (v->addr + 16) & 0x7FFFF;
    decode_block(v, n);
}

int port_trace(const char *what);
static int spu_trace_on;
/* TWINE_TRACE=spu: the first half second of each voice's output after a key on, measured (its energy, peak, and a
 * checksum of its samples: two voices playing the same the same way measure the same), for comparing a lent tone with
 * the mission's own */
static struct { u32 n; double e; s32 peak; u32 sum; } meter[24 + 8];
static void meter_put(int k, s32 smp, int which) {
    if (meter[k].n >= 22050)
        return;
    meter[k].e += (double)smp * smp;
    if (abs(smp) > meter[k].peak)
        meter[k].peak = abs(smp);
    meter[k].sum = meter[k].sum * 31 + (u32)smp;
    if (++meter[k].n == 22050)
        port_log("trace: spu: %s %d's first half second: energy %.0f, peak %d, checksum %08X\n",
                 which ? "lent voice" : "voice", which ? k - 24 : k, meter[k].e / 22050, meter[k].peak, meter[k].sum);
}
static void key_on(int n) {
    Voice *v = &voices[n];
    static int on = -1;
    if (on < 0)
        spu_trace_on = on = port_trace("spu");
    memset(&meter[n], 0, sizeof(meter[n]));
    if (on) /* (TWINE_TRACE=spu: each key on, the voice's registers as the game set them) */
        port_log("trace: spu: key on voice %d: vol %04X %04X pitch %04X start %05X adsr %04X %04X%s\n", n,
                 spu_regs[n * 8 + V_VOLL], spu_regs[n * 8 + V_VOLR], spu_regs[n * 8 + V_PITCH],
                 spu_regs[n * 8 + V_SSA] * 8u, spu_regs[n * 8 + V_ADSR1], spu_regs[n * 8 + V_ADSR2],
                 (reg32(REG_EON) >> n & 1) ? ", reverb" : "");
    v->addr = spu_regs[n * 8 + V_SSA] * 8u;
    v->counter = 0;
    v->phase = ENV_ATTACK;
    v->level = 0;
    v->env_counter = 0;
    memset(v->buf, 0, sizeof(v->buf));
    decode_block(v, n);
    endx &= ~(1u << n);
    v_gen[n] = exe_gen;
}

static void key_off(int n) {
    Voice *v = &voices[n];
    if (v->phase != ENV_OFF) {
        v->phase = ENV_RELEASE;
        v->env_counter = 0;
    }
}

static s32 interpolate(const Voice *v) {
    u32 i = (v->counter >> 4) & 0xFF;
    const s16 *p = &v->buf[v->counter >> 12]; /* oldest, older, old, new */
    return ((gauss[0x0FF - i] * p[0]) >> 15) + ((gauss[0x1FF - i] * p[1]) >> 15) +
           ((gauss[0x100 + i] * p[2]) >> 15) + ((gauss[0x000 + i] * p[3]) >> 15);
}

static void noise_tick(u16 attr) {
    int shift = (attr >> 10) & 0xF, step = ((attr >> 8) & 3) + 4;
    u32 parity = ((noise_level >> 15) ^ (noise_level >> 12) ^ (noise_level >> 11) ^ (noise_level >> 10) ^ 1) & 1;
    noise_timer -= step;
    if (noise_timer < 0) {
        noise_level = (u16)(noise_level * 2 + parity);
        noise_timer += 0x20000 >> shift;
        if (noise_timer < 0)
            noise_timer += 0x20000 >> shift;
    }
}

/* ---- reverb ---- */

/* Address in the work area relative to the current buffer address, wrapped within ESA..7FFFEh. */
static u32 rv_addr(s32 off) {
    u32 base = spu_regs[REG_ESA] * 8u, size = 0x80000 - base;
    s32 rel = ((s32)(rv_cur - base) + off) % (s32)size;
    if (rel < 0)
        rel += size;
    return (base + rel) & 0x7FFFE;
}

static s32 rv_rd(s32 off) {
    u32 a = rv_addr(off);
    return (s16)(spu_ram[a] | spu_ram[a + 1] << 8);
}

static void rv_wr(s32 off, s32 v) {
    u32 a;
    if (!rv_write_on)
        return;
    a = rv_addr(off);
    v = clamp16(v);
    spu_ram[a] = (u8)v;
    spu_ram[a + 1] = (u8)(v >> 8);
}

/* [m] = (in + [d]*vWALL - [m-2])*vIIR + [m-2] */
static void rv_reflect(s32 in, s32 m, s32 d, s32 wall, s32 iir) {
    s32 prev = rv_rd(m - 2);
    s32 t = clamp16(in + ((rv_rd(d) * wall) >> 15) - prev);
    rv_wr(m, ((t * iir) >> 15) + prev);
}

/* out = out - vAPF*[m-d], [m] = out, out = out*vAPF + [m-d] */
static s32 rv_apf(s32 x, s32 m, s32 d, s32 vol) {
    s32 delayed = rv_rd(m - d);
    x = clamp16(x - ((vol * delayed) >> 15));
    rv_wr(m, x);
    return clamp16(((x * vol) >> 15) + delayed);
}

/* One 22050 Hz step of the reverb formula (psx-spx "SPU Reverb Formula"): in = reverb input (already at
 * 22050 Hz), out = Lout/Rout before the output volume. */
static void reverb_step(const s32 *in, s32 *out) {
    const u16 *R = &spu_regs[REG_REVERB];
#define RV(r) ((s32)(s16)R[r]) /* volume */
#define RA(r) ((s32)R[r] * 8)  /* address offset in bytes */
    s32 wall = RV(vWALL), iir = RV(vIIR);
    s32 lin = (in[0] * RV(vLIN)) >> 15, rin = (in[1] * RV(vRIN)) >> 15, l, r;
    rv_reflect(lin, RA(mLSAME), RA(dLSAME), wall, iir); /* same side: L to L, R to R */
    rv_reflect(rin, RA(mRSAME), RA(dRSAME), wall, iir);
    rv_reflect(lin, RA(mLDIFF), RA(dRDIFF), wall, iir); /* different side: R to L, L to R */
    rv_reflect(rin, RA(mRDIFF), RA(dLDIFF), wall, iir);
    l = clamp16(((RV(vCOMB1) * rv_rd(RA(mLCOMB1))) >> 15) + ((RV(vCOMB2) * rv_rd(RA(mLCOMB2))) >> 15) +
                ((RV(vCOMB3) * rv_rd(RA(mLCOMB3))) >> 15) + ((RV(vCOMB4) * rv_rd(RA(mLCOMB4))) >> 15));
    r = clamp16(((RV(vCOMB1) * rv_rd(RA(mRCOMB1))) >> 15) + ((RV(vCOMB2) * rv_rd(RA(mRCOMB2))) >> 15) +
                ((RV(vCOMB3) * rv_rd(RA(mRCOMB3))) >> 15) + ((RV(vCOMB4) * rv_rd(RA(mRCOMB4))) >> 15));
    l = rv_apf(l, RA(mLAPF1), RA(dAPF1), RV(vAPF1));
    r = rv_apf(r, RA(mRAPF1), RA(dAPF1), RV(vAPF1));
    l = rv_apf(l, RA(mLAPF2), RA(dAPF2), RV(vAPF2));
    r = rv_apf(r, RA(mRAPF2), RA(dAPF2), RV(vAPF2));
    out[0] = l;
    out[1] = r;
#undef RV
#undef RA
}

static s32 fir(const s16 *h) {
    s32 sum = 0;
    int i;
    for (i = 0; i < 39; i++)
        sum += rv_fir[i] * h[i];
    return sum;
}

/* 44100 Hz side of the reverb unit: the input is filtered down to 22050 Hz, the formula runs on every
 * other tick, and its output is filtered back up (zero-stuffed, hence the gain of 2). Returns the output
 * after the reverb output volume (EVOL). */
static void reverb_tick(u16 attr, const s32 *in, s32 *out) {
    u32 p = rv_pos & 63, w = (rv_pos - 38) & 63;
    s32 y[2] = {0, 0};
    int c;
    s16 evol[2] = {(s16)spu_regs[REG_EVOLL], (s16)spu_regs[REG_EVOLL + 1]};
    rv_write_on = (attr & ATTR_REVERB) != 0;
    for (c = 0; c < 2; c++)
        rv_in[c][p] = rv_in[c][p + 64] = (s16)clamp16(in[c]);
    if (rv_pos & 1) {
        if (rv_write_on || evol[0] || evol[1]) {
            s32 x[2];
            for (c = 0; c < 2; c++)
                x[c] = clamp16(fir(&rv_in[c][w]) >> 15);
            reverb_step(x, y);
        }
        rv_cur = (rv_cur + 2) & 0x7FFFE;
        if (rv_cur < spu_regs[REG_ESA] * 8u)
            rv_cur = spu_regs[REG_ESA] * 8u;
    }
    for (c = 0; c < 2; c++) {
        rv_out[c][p] = rv_out[c][p + 64] = (s16)y[c];
        out[c] = (clamp16(fir(&rv_out[c][w]) >> 14) * evol[c]) >> 15;
    }
    rv_pos++;
}

/* ---- CD audio input ---- */

int spu_cd_queued(void) {
    return (int)(atomic_load_explicit(&cd_wr, memory_order_relaxed) -
                 atomic_load_explicit(&cd_rd, memory_order_acquire));
}

int spu_cd_push(const s16 *lr, int frames) {
    u32 wr = atomic_load_explicit(&cd_wr, memory_order_relaxed);
    u32 space = CD_RING - (wr - atomic_load_explicit(&cd_rd, memory_order_acquire)), i;
    if ((u32)frames > space)
        frames = (int)space;
    for (i = 0; i < (u32)frames; i++) {
        cd_ring[(wr + i) % CD_RING][0] = lr[i * 2];
        cd_ring[(wr + i) % CD_RING][1] = lr[i * 2 + 1];
    }
    atomic_store_explicit(&cd_wr, wr + frames, memory_order_release);
    return frames;
}

void spu_cd_idle(int idle) {
    atomic_store_explicit(&cd_idle, idle, memory_order_release);
}

/* The next CD input sample; silence while there is none, and after running dry until the drive has queued
 * CD_PREBUFFER frames again (or has stopped, leaving less). */
static void cd_input(s32 *lr) {
    u32 rd = atomic_load_explicit(&cd_rd, memory_order_relaxed);
    u32 have = atomic_load_explicit(&cd_wr, memory_order_acquire) - rd;
    lr[0] = lr[1] = 0;
    if (!cd_running && (have >= CD_PREBUFFER || (have && atomic_load_explicit(&cd_idle, memory_order_acquire))))
        cd_running = 1;
    if (!cd_running)
        return;
    if (!have) {
        cd_running = 0;
        return;
    }
    lr[0] = cd_ring[rd % CD_RING][0];
    lr[1] = cd_ring[rd % CD_RING][1];
    atomic_store_explicit(&cd_rd, rd + 1, memory_order_release);
}

/* ---- WAV recording ---- */

static void wav_header(FILE *f, u32 frames) {
    u8 h[44];
    u32 bytes = frames * 4;
    memcpy(h, "RIFF", 4);
    *(u32 *)(h + 4) = 36 + bytes;
    memcpy(h + 8, "WAVEfmt ", 8);
    *(u32 *)(h + 16) = 16;
    *(u16 *)(h + 20) = 1; /* PCM */
    *(u16 *)(h + 22) = 2;
    *(u32 *)(h + 24) = 44100;
    *(u32 *)(h + 28) = 44100 * 4;
    *(u16 *)(h + 32) = 4;
    *(u16 *)(h + 34) = 16;
    memcpy(h + 36, "data", 4);
    *(u32 *)(h + 40) = bytes;
    fseek(f, 0, SEEK_SET);
    fwrite(h, 1, sizeof(h), f);
    fseek(f, 0, SEEK_END);
}

/* Append a frame; every 4096 frames the data goes out and the header is brought up to date, so the file
 * stays valid however the process ends. */
static void wav_put(Wav *w, s32 l, s32 r) {
    if (!w->tried) {
        const char *path = getenv(w->env);
        w->tried = 1;
        if (path && (w->f = fopen(path, "wb")))
            wav_header(w->f, 0);
    }
    if (!w->f)
        return;
    w->buf[w->n * 2] = (s16)l;
    w->buf[w->n * 2 + 1] = (s16)r;
    if (++w->n < 4096)
        return;
    fwrite(w->buf, 4, w->n, w->f);
    w->frames += w->n;
    w->n = 0;
    wav_header(w->f, w->frames);
    fflush(w->f);
}

/* ---- mixer ---- */

/* A sound of the runtime's own (port_sfx): PCM mixed in like a voice, at its own rate */
#define SFX_CHANNELS 8
static struct {
    const s16 *pcm;
    u32 len, loop, pos, step; /* (pos, step: 16.16 samples; loop: where a looping sound starts over, or len) */
    s32 vol, handle;
} sfx[SFX_CHANNELS];
static s32 sfx_handles;
int port_audio_muted(void); /* endless.c: nothing heard (its first start's survey) */

/* A sound of another mission's bank (tones.c): a voice as the SPU's own in every way (ADPCM blocks decoded as it
 * plays, Gaussian interpolation, the tone's ADSR, its volumes, reverb), its samples in host memory (the banks fill
 * the SPU's RAM). Keyed off, it releases as the game's voices do. */
#define XVOICES 8
static struct {
    Voice v;
    const u8 *data;  /* (the tone's ADPCM blocks) */
    u32 size, loop;  /* (bytes; the loop's block, from its loop-start flag) */
    u16 regs[8];     /* (as a voice's registers: V_VOLL, V_VOLR, V_PITCH, -, V_ADSR1, V_ADSR2) */
    int reverb, handle;
    u32 gen;         /* (the executable it was keyed on in: port_spu_exe_started) */
} xv[XVOICES];

static void xv_decode(int k) {
    Voice *v = &xv[k].v;
    u32 a = v->addr, i;
    const u8 *b = xv[k].data + (a < xv[k].size ? a : 0);
    int shift, filter;
    s32 old = v->buf[30], older = v->buf[29], f0, f1;
    shift = b[0] & 0xF;
    if (shift > 12)
        shift = 9;
    filter = (b[0] >> 4) & 7;
    if (filter > 4)
        filter = 0;
    f0 = adpcm_filter_pos[filter];
    f1 = adpcm_filter_neg[filter];
    v->buf[0] = v->buf[28];
    v->buf[1] = v->buf[29];
    v->buf[2] = v->buf[30];
    for (i = 0; i < 28; i++) {
        u32 nib = (b[2 + i / 2] >> ((i & 1) * 4)) & 0xF;
        s32 smp = ((s16)(nib << 12) >> shift) + ((old * f0 + older * f1 + 32) >> 6);
        smp = clamp16(smp);
        v->buf[3 + i] = smp;
        older = old;
        old = smp;
    }
    v->flags = b[1];
    if (b[1] & 4)
        xv[k].loop = a;
}

static void xv_next(int k) {
    Voice *v = &xv[k].v;
    if (v->flags & 1) {
        v->addr = xv[k].loop;
        if (!(v->flags & 2)) {
            v->phase = ENV_OFF;
            v->level = 0;
        }
    } else
        v->addr += 16;
    if (v->addr + 16 > xv[k].size) { /* (past the tone's end: as the end flag) */
        v->phase = ENV_OFF;
        v->level = 0;
        v->addr = 0;
    }
    xv_decode(k);
}

static void spu_tick(s32 *out, const s32 *cd) {
    u16 attr = spu_regs[REG_ATTR];
    u32 pmon = reg32(REG_PMON), non = reg32(REG_NON), eon = reg32(REG_EON);
    s32 dry[2] = {0, 0}, wet[2] = {0, 0}, rv[2], prev = 0, cdv[2] = {0, 0};
    int n, c;
    noise_tick(attr);
    for (n = 0; n < 24; n++) {
        Voice *v = &voices[n];
        const u16 *reg = &spu_regs[n * 8];
        u32 step = reg[V_PITCH];
        s32 s = 0;
        if (n > 0 && ((pmon >> n) & 1)) { /* modulated by the previous voice's output */
            s32 st = (s16)step;
            step = (u32)((st * (prev + 0x8000)) >> 15) & 0xFFFF;
        }
        if (step > 0x3FFF)
            step = 0x4000;
        adsr_tick(v, reg);
        if (v->level) {
            s = ((non >> n) & 1) ? (s16)noise_level : interpolate(v);
            s = (s * v->level) >> 15;
        }
        prev = s;
        if (spu_trace_on)
            meter_put(n, s, 0);
        v->vol[0] = volume_tick(reg[V_VOLL], v->vol[0], &v->vol_counter[0]);
        v->vol[1] = volume_tick(reg[V_VOLR], v->vol[1], &v->vol_counter[1]);
        if (s) {
            for (c = 0; c < 2; c++) {
                s32 o = (s * v->vol[c]) >> 15;
                dry[c] += o;
                if ((eon >> n) & 1)
                    wet[c] += o;
            }
        }
        v->counter += step;
        while (v->counter >= 28u << 12) {
            v->counter -= 28u << 12;
            next_block(v, n);
        }
    }
    for (c = 0; c < SFX_CHANNELS; c++)
        if (sfx[c].pcm) {
            if ((sfx[c].pos >> 16) >= sfx[c].len && sfx[c].loop < sfx[c].len)
                sfx[c].pos = (sfx[c].loop << 16) + (sfx[c].pos & 0xFFFF);
            if ((sfx[c].pos >> 16) >= sfx[c].len)
                sfx[c].pcm = NULL;
            else {
                s32 o = (sfx[c].pcm[sfx[c].pos >> 16] * sfx[c].vol) >> 15;
                dry[0] += o;
                dry[1] += o;
                sfx[c].pos += sfx[c].step;
            }
        }
    for (c = 0; c < XVOICES; c++) /* (another mission's tones: as a voice) */
        if (xv[c].data) {
            Voice *v = &xv[c].v;
            u32 step = xv[c].regs[V_PITCH] > 0x3FFF ? 0x4000 : xv[c].regs[V_PITCH];
            s32 sm = 0;
            int k;
            adsr_tick(v, xv[c].regs);
            if (v->phase == ENV_OFF) {
                while (spu_trace_on && meter[24 + c].n && meter[24 + c].n < 22050) /* (its silence after, measured
                                                                                     * as a voice's would be) */
                    meter_put(24 + c, 0, 1);
                xv[c].data = NULL;
                continue;
            }
            if (v->level)
                sm = (interpolate(v) * v->level) >> 15;
            if (spu_trace_on)
                meter_put(24 + c, sm, 1);
            v->vol[0] = volume_tick(xv[c].regs[V_VOLL], v->vol[0], &v->vol_counter[0]);
            v->vol[1] = volume_tick(xv[c].regs[V_VOLR], v->vol[1], &v->vol_counter[1]);
            for (k = 0; k < 2 && sm; k++) {
                s32 o = (sm * v->vol[k]) >> 15;
                dry[k] += o;
                if (xv[c].reverb)
                    wet[k] += o;
            }
            v->counter += step;
            while (v->counter >= 28u << 12) {
                v->counter -= 28u << 12;
                xv_next(c);
            }
        }
    if (attr & ATTR_CD) { /* CD audio: its own volume, then like a voice (reverb if enabled) */
        for (c = 0; c < 2; c++) {
            cdv[c] = (cd[c] * (s16)spu_regs[REG_CDVOL + c]) >> 15;
            if (attr & ATTR_CD_REVERB)
                wet[c] += cdv[c];
        }
    }
    reverb_tick(attr, wet, rv);
    for (c = 0; c < 2; c++) {
        main_vol[c] = volume_tick(spu_regs[REG_MVOLL + c], main_vol[c], &main_vol_counter[c]);
        /* the mute bit silences the voices and reverb, not the CD input */
        out[c] = clamp16((clamp16(((attr & ATTR_UNMUTE) ? dry[c] + rv[c] : 0) + cdv[c]) * main_vol[c]) >> 15);
    }
    if (port_audio_muted())
        out[0] = out[1] = 0;
}

static s32 out_peak; /* the loudest sample since spu_take_peak */
static int test_no_cd = -1;

static void render(s16 *out, int frames) {
    int i, c;
    if (test_no_cd < 0)
        test_no_cd = getenv("TWINE_TEST_NO_CD") != NULL;
    for (i = 0; i < frames; i++) {
        s32 o[2] = {0, 0}, cd[2];
        u16 attr = spu_regs[REG_ATTR];
        cd_input(cd); /* the drive's output keeps time whatever the SPU does with it */
        if (test_no_cd) /* (TWINE_TEST_NO_CD: tests comparing sounds, the music left out) */
            cd[0] = cd[1] = 0;
        wav_put(&xa_wav, cd[0], cd[1]);
        if (attr & ATTR_ENABLE)
            spu_tick(o, cd);
        else if (attr & ATTR_CD) /* in reset, the CD input still plays (at the current main volume) */
            for (c = 0; c < 2; c++)
                o[c] = clamp16((clamp16((cd[c] * (s16)spu_regs[REG_CDVOL + c]) >> 15) * main_vol[c]) >> 15);
        wav_put(&spu_wav, o[0], o[1]);
        for (c = 0; c < 2; c++)
            if (abs(o[c]) > out_peak)
                out_peak = abs(o[c]);
        if (out) {
            out[i * 2] = (s16)o[0];
            out[i * 2 + 1] = (s16)o[1];
        }
    }
}

/* ---- clock and locking ---- */

static u64 host_ns(void) { /* (the game's clock: irq.c) */
    return port_clock_ns();
}

int spu_set_clock(int mode) {
    if (mode == SPU_CLOCK_DEVICE && !mutex && !(mutex = SDL_CreateMutex()))
        return 0;
    clock_mode = mode;
    host_start_ns = host_ns();
    host_samples = 0;
    return 1;
}

static void lock(void) {
    if (mutex)
        SDL_LockMutex(mutex);
}

static void unlock(void) {
    if (mutex)
        SDL_UnlockMutex(mutex);
}

/* the sound chip held still (savestate.c) */
void spu_hold(int on) {
    if (on)
        lock();
    else
        unlock();
}

/* play n samples of pcm (kept by the caller) at rate Hz and volume vol (0x7FFF full), with what else plays;
 * loop_at >= 0: from n on, again from that sample until stopped. A handle for port_sfx_stop. */
int port_sfx_play(const s16 *pcm, u32 n, u32 rate, s32 vol, s32 loop_at) {
    int c, oldest = 0;
    lock();
    for (c = 0; c < SFX_CHANNELS && sfx[c].pcm; c++)
        if (sfx[c].handle < sfx[oldest].handle)
            oldest = c;
    if (c == SFX_CHANNELS)
        c = oldest;
    sfx[c].pcm = pcm;
    sfx[c].len = n;
    sfx[c].loop = loop_at >= 0 && (u32)loop_at < n ? (u32)loop_at : n;
    sfx[c].pos = 0;
    sfx[c].step = (u32)(((u64)rate << 16) / 44100);
    sfx[c].vol = vol;
    sfx[c].handle = sfx_handles = sfx_handles % 0x3FFF + 1;
    unlock();
    return sfx[c].handle;
}

void port_sfx_stop(int handle) {
    int c;
    lock();
    for (c = 0; c < SFX_CHANNELS; c++)
        if (sfx[c].pcm && sfx[c].handle == handle)
            sfx[c].pcm = NULL;
    for (c = 0; c < XVOICES; c++) /* (another mission's tone: keyed off, its release) */
        if (xv[c].data && xv[c].handle == handle && xv[c].v.phase != ENV_OFF) {
            xv[c].v.phase = ENV_RELEASE;
            xv[c].v.env_counter = 0;
        }
    unlock();
}

/* key on another mission's tone (tones.c): its ADPCM (kept by the caller), the registers the game's sound library
 * would set (pitch, ADSR, left/right volume), reverb or not. A handle for port_sfx_stop (a release, as keyed off). */
int port_xvoice_play(const u8 *adpcm, u32 size, u16 pitch, u16 adsr1, u16 adsr2, u16 voll, u16 volr, int reverb) {
    int c, oldest = 0;
    if (!adpcm || size < 16)
        return 0;
    lock();
    for (c = 0; c < XVOICES && xv[c].data; c++)
        if (xv[c].handle < xv[oldest].handle)
            oldest = c;
    if (c == XVOICES)
        c = oldest;
    memset(&xv[c], 0, sizeof(xv[c]));
    xv[c].data = adpcm, xv[c].size = size & ~15u;
    xv[c].regs[V_PITCH] = pitch, xv[c].regs[V_ADSR1] = adsr1, xv[c].regs[V_ADSR2] = adsr2;
    xv[c].regs[V_VOLL] = voll, xv[c].regs[V_VOLR] = volr;
    xv[c].reverb = reverb;
    xv[c].gen = exe_gen;
    xv[c].v.phase = ENV_ATTACK;
    xv_decode(c);
    memset(&meter[24 + c], 0, sizeof(meter[24 + c]));
    if (port_trace("spu"))
        spu_trace_on = 1;
    xv[c].handle = sfx_handles = sfx_handles % 0x3FFF + 1; /* (one count with port_sfx_play's: port_sfx_stop either) */
    unlock();
    return xv[c].handle;
}

/* The runtime's own sounds keyed off (the extra voices: their release; port_sfx's: cut), but the one with handle
 * keep, as the game keys off all of its voices (endless.c: SsUtAllKeyOff at the mission's end, func_80026C90 as the
 * menu opens) or an executable starts. The count. */
static int runtime_off(int keep) {
    int c, n = 0;
    for (c = 0; c < XVOICES; c++)
        if (xv[c].data && xv[c].handle != keep && xv[c].v.phase != ENV_OFF && xv[c].v.phase != ENV_RELEASE) {
            xv[c].v.phase = ENV_RELEASE;
            xv[c].v.env_counter = 0;
            n++;
        }
    for (c = 0; c < SFX_CHANNELS; c++)
        if (sfx[c].pcm && sfx[c].handle != keep)
            sfx[c].pcm = NULL, n++;
    return n;
}

int port_spu_runtime_off(int keep) {
    int n;
    lock();
    n = runtime_off(keep);
    unlock();
    return n;
}

/* An executable starts (main.c run_main: a mission, the front end, a restart): the runtime's own sounds of the last
 * one are stopped. Their handles went with its memory: nothing there could stop them now -- a lent automatic
 * weapon's looping fire, held through a door, looped on for good, through RESTART MISSION too (playtest 8). The
 * game's own voices are its sound library's to stop, as on the PS1. TWINE_TEST_SOUND_KEEP=1: kept (the bug). */
void port_spu_exe_started(void) {
    int n = 0;
    lock();
    exe_gen++;
    if (!getenv("TWINE_TEST_SOUND_KEEP"))
        n = runtime_off(0);
    unlock();
    if (n && port_trace("sound"))
        port_log("trace: sound: %d of the runtime's own sounds stopped as an executable starts\n", n);
}

/* What sounds now (TWINE_TRACE=voices): the game's voices and the extra ones keyed on, and of them those keyed on
 * before this executable started (a sound left on from the last mission). Into buf; the count from before. */
int port_spu_sounding(char *buf, int size) {
    char list[512];
    int c, game = 0, lent = 0, old = 0, len = 0;
    list[0] = 0;
    lock();
    for (c = 0; c < 24; c++)
        if (voices[c].phase != ENV_OFF) {
            game++;
            if (v_gen[c] != exe_gen && len < (int)sizeof(list))
                old++, len += snprintf(list + len, sizeof(list) - len, " voice %d (%s, adsr %04X %04X)", c,
                                       voices[c].phase == ENV_RELEASE ? "releasing" : "on", spu_regs[c * 8 + V_ADSR1],
                                       spu_regs[c * 8 + V_ADSR2]);
            else if (v_gen[c] != exe_gen)
                old++;
        }
    for (c = 0; c < XVOICES; c++)
        if (xv[c].data && xv[c].v.phase != ENV_OFF) {
            lent++;
            if (xv[c].gen != exe_gen && len < (int)sizeof(list))
                old++, len += snprintf(list + len, sizeof(list) - len, " lent voice %d (handle %d, %s)", c, xv[c].handle,
                                       xv[c].v.phase == ENV_RELEASE ? "releasing" : "on");
            else if (xv[c].gen != exe_gen)
                old++;
        }
    for (c = 0; c < SFX_CHANNELS; c++)
        lent += sfx[c].pcm != NULL;
    unlock();
    snprintf(buf, size, "game voices %d, the runtime's %d; keyed on before this executable: %d%s", game, lent, old, list);
    return old;
}

void port_sfx(const s16 *pcm, u32 n, u32 rate, s32 vol) {
    port_sfx_play(pcm, n, rate, vol, -1);
}

static void (*sink)(const s16 *out, int frames);

void spu_set_sink(void (*fn)(const s16 *out, int frames)) {
    sink = fn;
}

/* SPU_CLOCK_HOST: run the samples due since the last register access (at most half a second's worth),
 * into the sink if there is one (TWINE_WAV). */
static void catch_up(void) {
    u64 due;
    s32 frames;
    if (clock_mode != SPU_CLOCK_HOST)
        return;
    due = (host_ns() - host_start_ns) * 44100 / 1000000000ull;
    if (due <= host_samples)
        return;
    if (due - host_samples > 22050)
        host_samples = due - 22050;
    frames = (s32)(due - host_samples);
    host_samples = due;
    if (!sink) {
        render(NULL, frames);
        return;
    }
    while (frames > 0) {
        s16 buf[512 * 2];
        int n = frames < 512 ? frames : 512;
        render(buf, n);
        sink(buf, n);
        frames -= n;
    }
}

/* Back to the power-on state (silent, sound RAM cleared), after the BIOS boot animation (intro.c). */
void spu_reset(void) {
    lock();
    memset(voices, 0, sizeof(voices));
    endx = 0;
    memset(main_vol, 0, sizeof(main_vol));
    memset(main_vol_counter, 0, sizeof(main_vol_counter));
    noise_timer = 0;
    noise_level = 0;
    rv_cur = rv_pos = 0;
    memset(rv_in, 0, sizeof(rv_in));
    memset(rv_out, 0, sizeof(rv_out));
    rv_write_on = 0;
    memset(spu_regs, 0, sizeof(spu_regs));
    memset(spu_ram, 0, sizeof(spu_ram));
    host_start_ns = host_ns();
    host_samples = 0;
    unlock();
}

void spu_render(s16 *out, int frames) {
    lock();
    render(out, frames);
    unlock();
}

int spu_take_peak(void) {
    s32 p;
    lock();
    p = out_peak;
    out_peak = 0;
    unlock();
    return p;
}

void spu_sync(void) {
    if (clock_mode != SPU_CLOCK_HOST)
        return;
    RT_ENTER();
    lock();
    catch_up();
    unlock();
    RT_LEAVE();
}

/* ---- registers ---- */

u16 spu_read(u32 reg) {
    u16 v;
    reg &= 0x1FF;
    RT_ENTER();
    lock();
    catch_up();
    if (reg < 0xC0 && (reg & 7) == V_ENVX)
        v = (u16)voices[reg >> 3].level;
    else if (reg == REG_ENDX)
        v = endx & 0xFFFF;
    else if (reg == REG_ENDX + 1)
        v = endx >> 16;
    else if (reg == REG_MVOLX || reg == REG_MVOLX + 1)
        v = (u16)main_vol[reg - REG_MVOLX];
    else if (reg >= REG_VOLX && reg < REG_VOLX + 48)
        v = (u16)voices[(reg - REG_VOLX) >> 1].vol[reg & 1];
    else
        v = spu_regs[reg]; /* includes the repeat address (LSAX), which loop-start blocks update */
    unlock();
    RT_LEAVE();
    return v;
}

void spu_write(u32 reg, u16 v) {
    static int log_cd = -1;
    u32 bits, n;
    reg &= 0x1FF;
    RT_ENTER();
    if (log_cd < 0)
        log_cd = getenv("TWINE_CD_LOG") != NULL;
    if (log_cd && (((reg == REG_CDVOL || reg == REG_CDVOL + 1) && v != spu_regs[reg]) ||
                   (reg == REG_ATTR && ((v ^ spu_regs[reg]) & (ATTR_CD | ATTR_CD_REVERB)))))
        port_log("spu: %s = %04X\n", reg == REG_ATTR ? "SPUCNT" : reg == REG_CDVOL ? "CD volume L" : "CD volume R", v);
    lock();
    catch_up();
    spu_regs[reg] = v;
    if (reg < 0xC0 && (reg & 7) == V_ENVX)
        voices[reg >> 3].level = (s16)v; /* the envelope continues from the written level */
    else if (reg >= REG_VOLX && reg < REG_VOLX + 48)
        voices[(reg - REG_VOLX) >> 1].vol[reg & 1] = (s16)v;
    switch (reg) {
    case REG_XADDR:
        spu_xfer_addr = (u32)v * 8;
        break;
    case REG_FIFO:
        spu_ram[spu_xfer_addr & 0x7FFFF] = v & 0xFF;
        spu_ram[(spu_xfer_addr + 1) & 0x7FFFF] = v >> 8;
        spu_xfer_addr = (spu_xfer_addr + 2) & 0x7FFFF;
        break;
    case REG_KON:
    case REG_KON + 1:
        bits = ((u32)v << (reg == REG_KON ? 0 : 16)) & 0xFFFFFF;
        for (n = 0; n < 24; n++)
            if (bits & (1u << n))
                key_on(n);
        break;
    case REG_KOFF:
    case REG_KOFF + 1:
        bits = ((u32)v << (reg == REG_KOFF ? 0 : 16)) & 0xFFFFFF;
        for (n = 0; n < 24; n++)
            if (bits & (1u << n))
                key_off(n);
        break;
    case REG_ESA:
        rv_cur = (u32)v * 8;
        break;
    case REG_MVOLX:
    case REG_MVOLX + 1:
        main_vol[reg - REG_MVOLX] = (s16)v;
        break;
    }
    unlock();
    RT_LEAVE();
}
