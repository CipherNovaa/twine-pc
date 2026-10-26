/* Software GTE (PS1 geometry transformation engine), written from the psx-spx documentation
 * ("Geometry Transformation Engine (GTE)"). The game's gte_* macros (include/gte_native.h) move
 * registers with GTE_mtc2/mfc2/ctc2/cfc2 and run commands with GTE_cmd, so behaviour follows the
 * hardware: register truncation and extension, 44-bit MAC overflow flags, IR/colour/SZ/SXY saturation,
 * the UNR division used by RTPS/RTPT, and the FIFOs. */
#include <string.h>
#include "port.h"

static u32 D[32]; /* data registers as stored (16-bit ones already truncated/extended for reads) */
int port_gte_mirror; /* NCLIP gives the other sign: a model drawn mirrored (the quick punch's left hand) */
static u32 C[32]; /* control registers */
#define FLAG C[31]

/* field accessors */
#define VX(n) ((s16)D[(n) * 2])
#define VY(n) ((s16)(D[(n) * 2] >> 16))
#define VZ(n) ((s16)D[(n) * 2 + 1])
#define IR(n) ((s32)D[8 + (n)])
#define MAC(n) ((s32)D[24 + (n)])
#define SX(n) ((s16)D[12 + (n)])
#define SY(n) ((s16)(D[12 + (n)] >> 16))
#define SZ(n) ((s32)(D[16 + (n)] & 0xFFFF))
#define RGBC_R ((s32)(D[6] & 0xFF))
#define RGBC_G ((s32)((D[6] >> 8) & 0xFF))
#define RGBC_B ((s32)((D[6] >> 16) & 0xFF))
#define RGBC_CODE (D[6] & 0xFF000000)
/* a 3x3 matrix of s16 stored in five control registers from `base` */
#define M(base, i) ((s16)(C[(base) + (i) / 2] >> (((i) & 1) * 16)))
#define TRX ((s32)C[5])
#define TRY ((s32)C[6])
#define TRZ ((s32)C[7])
#define OFX ((s32)C[24])
#define OFY ((s32)C[25])
#define H ((u32)(C[26] & 0xFFFF))
#define DQA ((s32)(s16)C[27])
#define DQB ((s32)C[28])
#define ZSF3 ((s32)(s16)C[29])
#define ZSF4 ((s32)(s16)C[30])

/* The registers, saved and put back around an extra drawing (interp.c). */
static u32 saved_d[32], saved_c[32];

void GTE_save(void) {
    memcpy(saved_d, D, sizeof(D));
    memcpy(saved_c, C, sizeof(C));
}

void GTE_restore(void) {
    memcpy(D, saved_d, sizeof(D));
    memcpy(C, saved_c, sizeof(C));
}

/* All 64 registers as stored (data then control), out and back in: for a check that runs code twice from the same
 * state (port/mods/people.c's TWINE_PEOPLE_VERIFY). */
void port_gte_get(u32 *regs) {
    memcpy(regs, D, sizeof(D));
    memcpy(regs + 32, C, sizeof(C));
}

void port_gte_put(const u32 *regs) {
    memcpy(D, regs, sizeof(D));
    memcpy(C, regs + 32, sizeof(C));
}

/* Back to the power-on state (all registers zero), after the BIOS boot animation (intro.c). */
void GTE_reset(void) {
    int i;
    for (i = 0; i < 32; i++)
        D[i] = C[i] = 0;
}

static u32 flag_with_error(void) {
    return (FLAG & 0x7FFFF000) | ((FLAG & 0x7F87E000) ? 0x80000000 : 0);
}

/* ---- register access ---- */

u32 GTE_mfc2(int r) {
    switch (r) {
    case 15:
        return D[14];
    case 28:
    case 29: {
        s32 c[3], i;
        for (i = 0; i < 3; i++) {
            c[i] = IR(i + 1) >> 7;
            c[i] = c[i] < 0 ? 0 : c[i] > 0x1F ? 0x1F : c[i];
        }
        return c[0] | (c[1] << 5) | (c[2] << 10);
    }
    default:
        return D[r];
    }
}

void GTE_mtc2(int r, u32 v) {
    switch (r) {
    case 1: case 3: case 5: case 8: case 9: case 10: case 11:
        D[r] = (s32)(s16)v;
        break;
    case 7: case 16: case 17: case 18: case 19:
        D[r] = v & 0xFFFF;
        break;
    case 15:
        D[12] = D[13];
        D[13] = D[14];
        D[14] = v;
        D[15] = v;
        break;
    case 28:
        D[28] = v & 0x7FFF;
        D[9] = (v & 0x1F) << 7;
        D[10] = ((v >> 5) & 0x1F) << 7;
        D[11] = ((v >> 10) & 0x1F) << 7;
        break;
    case 29:
    case 31:
        break;
    case 30: {
        u32 x = (s32)v < 0 ? ~v : v;
        s32 n = 0;
        while (n < 32 && !(x & 0x80000000u)) {
            x <<= 1;
            n++;
        }
        D[30] = v;
        D[31] = n;
        break;
    }
    default:
        D[r] = v;
        break;
    }
}

u32 GTE_cfc2(int r) {
    if (r == 31)
        return flag_with_error();
    if (r == 26) /* H reads sign-extended (hardware quirk) */
        return (s32)(s16)C[26];
    return C[r];
}

void GTE_ctc2(int r, u32 v) {
    switch (r) {
    case 4: case 12: case 20: case 27: case 29: case 30:
        C[r] = (s32)(s16)v;
        break;
    case 26:
        C[r] = v & 0xFFFF;
        break;
    case 31:
        C[r] = v & 0x7FFFF000;
        break;
    default:
        C[r] = v;
        break;
    }
}

/* ---- arithmetic with flags ---- */

/* MACn (n = 1..3) accumulates in 44 bits; overflow sets FLAG 30-28 (positive) / 27-25 (negative). */
static s64 mac_chk(int n, s64 v) {
    if (v > 0x7FFFFFFFFFFLL)
        FLAG |= 1u << (31 - n);
    else if (v < -0x80000000000LL)
        FLAG |= 1u << (28 - n);
    return (s64)((u64)v << 20) >> 20;
}

static s64 mac0_chk(s64 v) {
    if (v > 0x7FFFFFFFLL)
        FLAG |= 1u << 16;
    else if (v < -0x80000000LL)
        FLAG |= 1u << 15;
    return v;
}

static s32 ir_sat(int n, s64 v, int lm) {
    s32 lo = lm ? 0 : -0x8000;
    if (v < lo) {
        FLAG |= 1u << (25 - n);
        return lo;
    }
    if (v > 0x7FFF) {
        FLAG |= 1u << (25 - n);
        return 0x7FFF;
    }
    return (s32)v;
}

static void set_mac_ir(int n, s64 v, int shift, int lm) {
    v = v >> shift;
    D[24 + n] = (u32)(s32)v;
    D[8 + n] = (u32)ir_sat(n, (s32)v, lm);
}

static s32 col_sat(int n, s32 v) {
    if (v < 0) {
        FLAG |= 1u << (22 - n);
        return 0;
    }
    if (v > 0xFF) {
        FLAG |= 1u << (22 - n);
        return 0xFF;
    }
    return v;
}

static void push_color(void) {
    D[20] = D[21];
    D[21] = D[22];
    D[22] = (u32)col_sat(1, MAC(1) >> 4) | ((u32)col_sat(2, MAC(2) >> 4) << 8) |
            ((u32)col_sat(3, MAC(3) >> 4) << 16) | RGBC_CODE;
}

static void push_sz(s64 v) {
    if (v < 0) {
        FLAG |= 1u << 18;
        v = 0;
    } else if (v > 0xFFFF) {
        FLAG |= 1u << 18;
        v = 0xFFFF;
    }
    D[16] = D[17];
    D[17] = D[18];
    D[18] = D[19];
    D[19] = (u32)v;
}

static s32 sxy_sat(int bit, s64 v) {
    if (v < -0x400) {
        FLAG |= 1u << bit;
        return -0x400;
    }
    if (v > 0x3FF) {
        FLAG |= 1u << bit;
        return 0x3FF;
    }
    return (s32)v;
}

/* (sum of three products + t * 0x1000), checked after every addition like the hardware */
static s64 mac3(int n, s64 t, s64 a, s64 b, s64 c) {
    s64 v = mac_chk(n, t * 0x1000 + a);
    v = mac_chk(n, v + b);
    return mac_chk(n, v + c);
}

/* UNR division table: unr[i] = max(0, (0x40000 / (i + 0x100) + 1) / 2 - 0x101), i = 0..0x100 */
static u8 unr[0x101];
static int unr_ready;

static u32 gte_divide(u32 h, u32 sz3) {
    s32 z;
    u32 n, d, u;
    if (!unr_ready) {
        s32 i, x;
        for (i = 0; i <= 0x100; i++) {
            x = (0x40000 / (i + 0x100) + 1) / 2 - 0x101;
            unr[i] = x < 0 ? 0 : x;
        }
        unr_ready = 1;
    }
    if (h >= sz3 * 2) {
        FLAG |= 1u << 17;
        return 0x1FFFF;
    }
    z = __builtin_clz(sz3) - 16; /* sz3 > 0 here, 16-bit */
    n = h << z;
    d = sz3 << z;
    u = unr[(d - 0x7FC0) >> 7] + 0x101;
    d = (0x2000080 - d * u) >> 8;
    d = (0x0000080 + d * u) >> 8;
    {
        u64 q = (((u64)n * d) + 0x8000) >> 16;
        return q > 0x1FFFF ? 0x1FFFF : (u32)q;
    }
}

/* ---- commands ---- */

/* RTPS/RTPT on vertex v; `last` also computes IR0 (depth cue) */
static void rtp(int v, int sf, int lm, int last) {
    int shift = sf * 12;
    s64 m1, m2, m3;
    u32 q;
    s64 x, y;
    m1 = mac3(1, TRX, (s64)M(0, 0) * VX(v), (s64)M(0, 1) * VY(v), (s64)M(0, 2) * VZ(v));
    m2 = mac3(2, TRY, (s64)M(0, 3) * VX(v), (s64)M(0, 4) * VY(v), (s64)M(0, 5) * VZ(v));
    m3 = mac3(3, TRZ, (s64)M(0, 6) * VX(v), (s64)M(0, 7) * VY(v), (s64)M(0, 8) * VZ(v));
    set_mac_ir(1, m1, shift, lm);
    set_mac_ir(2, m2, shift, lm);
    /* IR3: saturated from MAC3, but the flag follows MAC3 >> 12 when sf = 0 */
    D[27] = (u32)(s32)(m3 >> shift);
    {
        s32 mac3v = MAC(3), lo = lm ? 0 : -0x8000;
        s32 f = (s32)(m3 >> 12);
        D[11] = (u32)(mac3v < lo ? lo : mac3v > 0x7FFF ? 0x7FFF : mac3v);
        if (f < -0x8000 || f > 0x7FFF)
            FLAG |= 1u << 22;
    }
    push_sz(m3 >> 12);
    q = gte_divide(H, SZ(3));
    x = mac0_chk((s64)q * IR(1) + OFX);
    y = mac0_chk((s64)q * IR(2) + OFY);
    D[12] = D[13];
    D[13] = D[14];
    D[14] = ((u32)sxy_sat(14, x >> 16) & 0xFFFF) | ((u32)sxy_sat(13, y >> 16) << 16);
    D[15] = D[14];
    D[24] = (u32)(s32)y;
    if (last) {
        s64 p = mac0_chk((s64)q * DQA + DQB);
        D[24] = (u32)(s32)p;
        p >>= 12;
        if (p < 0) {
            FLAG |= 1u << 12;
            p = 0;
        } else if (p > 0x1000) {
            FLAG |= 1u << 12;
            p = 0x1000;
        }
        D[8] = (u32)p;
    }
}

/* matrix (rt=0, llm=1, lcm=2, garbage=3) x vector -> MAC/IR, plus translation t (scaled by 0x1000) */
static void mat_vec(int mx, const s32 vec[3], const s32 t[3], int shift, int lm) {
    s32 m[9];
    int i;
    if (mx == 3) {
        m[0] = -(RGBC_R << 4); m[1] = RGBC_R << 4; m[2] = IR(0);
        m[3] = m[4] = m[5] = M(0, 2);
        m[6] = m[7] = m[8] = M(0, 4);
    } else {
        for (i = 0; i < 9; i++)
            m[i] = M(mx * 8, i);
    }
    for (i = 0; i < 3; i++) {
        s64 v = mac3(i + 1, t[i], (s64)m[i * 3] * vec[0], (s64)m[i * 3 + 1] * vec[1], (s64)m[i * 3 + 2] * vec[2]);
        set_mac_ir(i + 1, v, shift, lm);
    }
}

static void bk(s32 t[3]) {
    t[0] = (s32)C[13];
    t[1] = (s32)C[14];
    t[2] = (s32)C[15];
}

static void ir_vec(s32 v[3]) {
    v[0] = IR(1);
    v[1] = IR(2);
    v[2] = IR(3);
}

/* normal * light matrix, then light colour matrix with background colour */
static void light(int v, int shift, int lm) {
    static const s32 zero[3];
    s32 vec[3], t[3];
    vec[0] = VX(v);
    vec[1] = VY(v);
    vec[2] = VZ(v);
    mat_vec(1, vec, zero, shift, lm);
    ir_vec(vec);
    bk(t);
    mat_vec(2, vec, t, shift, lm);
}

/* MAC = MAC + (FC - MAC) * IR0, from pre-shift values m[] (44-bit domain) */
static void depth_cue(s64 m[3], int shift) {
    int i;
    for (i = 0; i < 3; i++) {
        s64 fc = (s64)(s32)C[21 + i] << 12;
        s32 ir = ir_sat(i + 1, mac_chk(i + 1, fc - m[i]) >> shift, 0);
        m[i] = mac_chk(i + 1, (s64)ir * IR(0) + m[i]);
    }
}

static void finish_color(s64 m[3], int shift, int lm) {
    int i;
    for (i = 0; i < 3; i++)
        set_mac_ir(i + 1, m[i], shift, lm);
    push_color();
}

static void color_mul(s64 m[3]) {
    m[0] = mac_chk(1, ((s64)RGBC_R << 4) * IR(1));
    m[1] = mac_chk(2, ((s64)RGBC_G << 4) * IR(2));
    m[2] = mac_chk(3, ((s64)RGBC_B << 4) * IR(3));
}

void GTE_cmd(u32 op) {
    int sf = (op >> 19) & 1, shift = sf * 12, lm = (op >> 10) & 1;
    int i;
    s64 m[3];
    FLAG = 0;
    switch (op & 0x3F) {
    case 0x01: /* RTPS */
        rtp(0, sf, lm, 1);
        break;
    case 0x30: /* RTPT */
        rtp(0, sf, lm, 0);
        rtp(1, sf, lm, 0);
        rtp(2, sf, lm, 1);
        break;
    case 0x06: /* NCLIP (mirrored: the other way round, port_gte_mirror) */
        D[24] = (u32)(s32)mac0_chk((port_gte_mirror ? -1 : 1) * ((s64)SX(0) * SY(1) + (s64)SX(1) * SY(2) + (s64)SX(2) * SY(0) -
                                   (s64)SX(0) * SY(2) - (s64)SX(1) * SY(0) - (s64)SX(2) * SY(1)));
        break;
    case 0x0C: { /* OP: cross product of IR with the RT diagonal, all three from IR as it was before */
        s32 d1 = M(0, 0), d2 = M(0, 4), d3 = M(0, 8);
        s64 ir1 = IR(1), ir2 = IR(2), ir3 = IR(3);
        s64 m1 = mac_chk(1, ir3 * d2 - ir2 * d3);
        s64 m2 = mac_chk(2, ir1 * d3 - ir3 * d1);
        s64 m3 = mac_chk(3, ir2 * d1 - ir1 * d2);
        set_mac_ir(1, m1, shift, lm);
        set_mac_ir(2, m2, shift, lm);
        set_mac_ir(3, m3, shift, lm);
        break;
    }
    case 0x12: { /* MVMVA */
        int mx = (op >> 17) & 3, vi = (op >> 15) & 3, cv = (op >> 13) & 3;
        s32 vec[3], t[3] = {0, 0, 0};
        if (vi == 3)
            ir_vec(vec);
        else {
            vec[0] = VX(vi);
            vec[1] = VY(vi);
            vec[2] = VZ(vi);
        }
        if (cv == 0) {
            t[0] = TRX;
            t[1] = TRY;
            t[2] = TRZ;
        } else if (cv == 1)
            bk(t);
        if (cv == 2) {
            /* hardware bug: FC + first column only set flags; the result is the other two columns */
            s32 mm[9];
            for (i = 0; i < 9; i++)
                mm[i] = mx == 3 ? 0 : M(mx * 8, i);
            for (i = 0; i < 3; i++) {
                s64 f = mac_chk(i + 1, ((s64)(s32)C[21 + i] << 12) + (s64)mm[i * 3] * vec[0]);
                ir_sat(i + 1, f >> shift, 0);
                set_mac_ir(i + 1, mac_chk(i + 1, mac_chk(i + 1, (s64)mm[i * 3 + 1] * vec[1]) + (s64)mm[i * 3 + 2] * vec[2]),
                           shift, lm);
            }
        } else
            mat_vec(mx, vec, t, shift, lm);
        break;
    }
    case 0x28: /* SQR */
        for (i = 1; i <= 3; i++)
            set_mac_ir(i, mac_chk(i, (s64)IR(i) * IR(i)), shift, lm);
        break;
    case 0x2D: /* AVSZ3 */
    case 0x2E: { /* AVSZ4 */
        s64 v = (op & 0x3F) == 0x2D ? mac0_chk((s64)ZSF3 * (SZ(1) + SZ(2) + SZ(3)))
                                     : mac0_chk((s64)ZSF4 * (SZ(0) + SZ(1) + SZ(2) + SZ(3)));
        D[24] = (u32)(s32)v;
        v >>= 12;
        if (v < 0) {
            FLAG |= 1u << 18;
            v = 0;
        } else if (v > 0xFFFF) {
            FLAG |= 1u << 18;
            v = 0xFFFF;
        }
        D[7] = (u32)v;
        break;
    }
    case 0x3D: /* GPF */
        for (i = 1; i <= 3; i++)
            m[i - 1] = mac_chk(i, (s64)IR(0) * IR(i));
        finish_color(m, shift, lm);
        break;
    case 0x3E: /* GPL */
        for (i = 1; i <= 3; i++)
            m[i - 1] = mac_chk(i, ((s64)MAC(i) << shift) + (s64)IR(0) * IR(i));
        finish_color(m, shift, lm);
        break;
    case 0x1E: /* NCS */
    case 0x20: /* NCT */
        for (i = 0; i < ((op & 0x3F) == 0x20 ? 3 : 1); i++) {
            light(i, shift, lm);
            push_color();
        }
        break;
    case 0x1B: /* NCCS */
    case 0x3F: /* NCCT */
        for (i = 0; i < ((op & 0x3F) == 0x3F ? 3 : 1); i++) {
            light(i, shift, lm);
            color_mul(m);
            finish_color(m, shift, lm);
        }
        break;
    case 0x13: /* NCDS */
    case 0x16: /* NCDT */
        for (i = 0; i < ((op & 0x3F) == 0x16 ? 3 : 1); i++) {
            light(i, shift, lm);
            color_mul(m);
            depth_cue(m, shift);
            finish_color(m, shift, lm);
        }
        break;
    case 0x1C: /* CC */
    case 0x14: { /* CDP */
        s32 vec[3], t[3];
        ir_vec(vec);
        bk(t);
        mat_vec(2, vec, t, shift, lm);
        color_mul(m);
        if ((op & 0x3F) == 0x14)
            depth_cue(m, shift);
        finish_color(m, shift, lm);
        break;
    }
    case 0x10: /* DPCS */
    case 0x2A: /* DPCT */
        for (i = 0; i < ((op & 0x3F) == 0x2A ? 3 : 1); i++) {
            u32 c = (op & 0x3F) == 0x2A ? D[20] : D[6];
            m[0] = (s64)(c & 0xFF) << 16;
            m[1] = (s64)((c >> 8) & 0xFF) << 16;
            m[2] = (s64)((c >> 16) & 0xFF) << 16;
            depth_cue(m, shift);
            finish_color(m, shift, lm);
        }
        break;
    case 0x29: /* DCPL */
        color_mul(m);
        depth_cue(m, shift);
        finish_color(m, shift, lm);
        break;
    case 0x11: /* INTPL */
        for (i = 0; i < 3; i++)
            m[i] = (s64)IR(i + 1) << 12;
        depth_cue(m, shift);
        finish_color(m, shift, lm);
        break;
    default:
        port_log("GTE: unknown command %08X\n", op);
        break;
    }
}
