/* SPU (Sound Processing Unit), written from the psx-spx documentation ("Sound Processing Unit (SPU)"; the
 * ADPCM filters from "CDROM - XA-ADPCM", which the SPU format shares). The recompiled libspu/libsnd program
 * it through its registers (io.c forwards 0x1F801C00-0x1F801FFF here); sound RAM is filled by DMA (io.c)
 * and by the manual-write FIFO here.
 *
 * Emulated at the SPU's own rate, one tick per 44100 Hz sample: 24 voices (SPU-ADPCM blocks with their
 * loop flags and ENDX, the pitch counter with pitch modulation and 4-point Gaussian interpolation, noise,
 * the ADSR envelope readable as ENVX, fixed or sweeping left/right volume), the main volume and the reverb
 * unit (22050 Hz, its work area in sound RAM, resampled through the documented 39-tap filter).
 *
 * Key on/off take effect when written (the hardware applies them at its next tick).
 *
 * Not emulated: CD-ROM/external audio input and its volumes, the capture buffers (sound RAM 0-0xFFF), the
 * SPU interrupt (IRQA), and the transfer timing (io.c completes transfers at once). */
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
#define REG_MVOLX 0xDC  /* 0x1F801DB8 current main volume left (right follows) */
#define REG_REVERB 0xE0 /* 0x1F801DC0 reverb configuration */
#define REG_VOLX 0x100  /* 0x1F801E00 current voice volumes, left/right per voice */

#define ATTR_ENABLE 0x8000
#define ATTR_UNMUTE 0x4000
#define ATTR_REVERB 0x0080 /* reverb master enable: writes to the work area */

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
static int clock_mode;
static SDL_Mutex *mutex;
static u64 host_start_ns, host_samples;

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

/* ADPCM prediction filters (psx-spx "XA-ADPCM", Pos/neg Tables; SPU-ADPCM has five) */
static const s32 filter_pos[5] = {0, 60, 115, 98, 122};
static const s32 filter_neg[5] = {0, 0, -52, -55, -60};

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
    f0 = filter_pos[filter];
    f1 = filter_neg[filter];
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

static void key_on(int n) {
    Voice *v = &voices[n];
    v->addr = spu_regs[n * 8 + V_SSA] * 8u;
    v->counter = 0;
    v->phase = ENV_ATTACK;
    v->level = 0;
    v->env_counter = 0;
    memset(v->buf, 0, sizeof(v->buf));
    decode_block(v, n);
    endx &= ~(1u << n);
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

/* ---- mixer ---- */

static void spu_tick(s32 *out) {
    u16 attr = spu_regs[REG_ATTR];
    u32 pmon = reg32(REG_PMON), non = reg32(REG_NON), eon = reg32(REG_EON);
    s32 dry[2] = {0, 0}, wet[2] = {0, 0}, rv[2], prev = 0;
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
    reverb_tick(attr, wet, rv);
    for (c = 0; c < 2; c++) {
        main_vol[c] = volume_tick(spu_regs[REG_MVOLL + c], main_vol[c], &main_vol_counter[c]);
        out[c] = (attr & ATTR_UNMUTE) ? clamp16((clamp16(dry[c] + rv[c]) * main_vol[c]) >> 15) : 0;
    }
}

static void render(s16 *out, int frames) {
    int i;
    for (i = 0; i < frames; i++) {
        s32 o[2] = {0, 0};
        if (spu_regs[REG_ATTR] & ATTR_ENABLE)
            spu_tick(o);
        if (out) {
            out[i * 2] = (s16)o[0];
            out[i * 2 + 1] = (s16)o[1];
        }
    }
}

/* ---- clock and locking ---- */

static u64 host_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (u64)ts.tv_sec * 1000000000ull + ts.tv_nsec;
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

/* SPU_CLOCK_HOST: run the samples due since the last register access (at most half a second's worth). */
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
    render(NULL, frames);
}

void spu_render(s16 *out, int frames) {
    lock();
    render(out, frames);
    unlock();
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
    u32 bits, n;
    reg &= 0x1FF;
    RT_ENTER();
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
