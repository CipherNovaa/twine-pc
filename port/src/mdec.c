/* The MDEC (motion decoder), which turns the run-length codes of the game's movie frames into pixels,
 * written from the psx-spx documentation ("Macroblock Decoder (MDEC)"). Commands come in through MDEC0
 * (usually just the command word, its parameters by DMA0): 1 decodes macroblocks, 2 sets the quant tables,
 * 3 the IDCT scale table. A macroblock is decoded as soon as all of its input is in, into an output FIFO
 * the CPU or DMA1 drain (io.c); when the FIFO is full, decoding waits for it to drain, like the hardware.
 *
 * Decoding follows psx-spx's description: rl_decode_block (DC with the block's quant scale, AC run/level
 * codes dequantised along the zigzag and saturated to 11 bits), the IDCT as two matrix passes with the scale
 * table's upper 13 bits, then YUV to RGB with the 8x8 Cr/Cb blocks stretched over the 16x16 macroblock
 * (Cr, Cb, Y1..Y4 order; Y1 Y2 above Y3 Y4), or Y alone for the monochrome depths. Colored macroblocks come
 * out as 16 rows of 16 pixels, the way DMA1 stores them (the hardware's FIFO holds 8x8 blocks and DMA1
 * reorders them; a CPU reading MDEC0 gets the DMA order here). */
#include <string.h>
#include "mdec.h"

void port_log(const char *fmt, ...);

#define IN_MAX 0x20000u    /* halfwords: MDEC(1) takes up to FFFFh parameter words */
#define OUT_MAX 0x100000u  /* bytes of decoded pixels waiting for DMA1 (a 640x480 24-bit frame fits) */

static u16 in_buf[IN_MAX];
static u32 in_len, in_pos;  /* MDEC(1) parameter halfwords received / decoded */
static u32 cmd;             /* the last command word */
static u32 words_left;      /* parameter words the command still expects */
static int decoding;        /* MDEC(1) has macroblocks left to decode (its input may still be coming) */
static u16 count;           /* status bits 0-15 */
static int req_in, req_out; /* DMA request enables (MDEC1 control bits 30, 29) */

static u8 table[128];       /* MDEC(2)/(3) parameters as they arrive */
static u32 table_len;
static u8 iq_y[64], iq_uv[64];
static s16 scale[64];

static u8 out_buf[OUT_MAX];
static u32 out_r, out_w;

/* raster position of each zigzag index (psx-spx's zagzig: the inverse of its zigzag table) */
static const u8 zagzig[64] = {
    0,  1,  8,  16, 9,  2,  3,  10, 17, 24, 32, 25, 18, 11, 4,  5,  12, 19, 26, 33, 40, 48,
    41, 34, 27, 20, 13, 6,  7,  14, 21, 28, 35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23,
    30, 37, 44, 51, 58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63,
};

static s32 sext10(u32 n) {
    return (s32)((n & 0x3FF) ^ 0x200) - 0x200;
}

static s32 clamp(s32 v, s32 lo, s32 hi) {
    return v < lo ? lo : v > hi ? hi : v;
}

static u32 depth(void) {
    return (cmd >> 27) & 3; /* 0 = 4-bit, 1 = 8-bit, 2 = 24-bit, 3 = 15-bit */
}

/* output bytes per macroblock */
static u32 mb_size(void) {
    static const u32 sizes[4] = {32, 64, 768, 512};
    return sizes[depth()];
}

/* ---- decoding ---- */

/* One block's codes (psx-spx rl_decode_block): the coefficients in raster order. 0 if the input ends
 * first (nothing is consumed then). */
static int rl_block(u32 *pos, const u8 *qt, s32 *blk) {
    u32 p = *pos, n;
    s32 q, k = 0, val;
    memset(blk, 0, 64 * sizeof(*blk));
    do { /* FE00h before a block is padding */
        if (p >= in_len)
            return 0;
        n = in_buf[p++];
    } while (n == 0xFE00);
    q = n >> 10;
    val = sext10(n) * qt[0];
    for (;;) {
        if (!q) /* quant scale 0: no quant table, no zigzag */
            val = sext10(n) * 2;
        val = clamp(val, -0x400, 0x3FF);
        blk[q ? zagzig[k] : k] = val;
        if (p >= in_len)
            return 0;
        n = in_buf[p++];
        k += (n >> 10) + 1; /* zero run; FE00h (end of block) runs past 63 */
        if (k > 63)
            break;
        val = (sext10(n) * qt[k] * q + 4) >> 3;
    }
    *pos = p;
    return 1;
}

/* psx-spx real_idct_core: dst = src (columns) x scale table, twice */
static void idct_pass(const s32 *src, s32 *dst) {
    int x, y, z;
    for (y = 0; y < 8; y++) {
        int any = 0;
        for (z = 0; z < 8; z++)
            any |= src[y + z * 8];
        if (!any) {
            for (x = 0; x < 8; x++)
                dst[x + y * 8] = 0;
            continue;
        }
        for (x = 0; x < 8; x++) {
            s32 sum = 0;
            for (z = 0; z < 8; z++)
                sum += src[y + z * 8] * (scale[x + z * 8] >> 3);
            dst[x + y * 8] = (sum + 0xFFF) >> 13;
        }
    }
}

static void idct(s32 *blk) {
    s32 tmp[64];
    idct_pass(blk, tmp);
    idct_pass(tmp, blk);
}

/* Append n output bytes. */
static u8 *out_put(u32 n) {
    u8 *p = &out_buf[out_w];
    out_w += n;
    return p;
}

/* psx-spx yuv_to_rgb for the whole macroblock, then 15- or 24-bit pixels in 16 rows of 16 */
static void put_colored(const s32 *cr, const s32 *cb, s32 y_blk[4][64]) {
    int sgn = (cmd >> 26) & 1, d24 = depth() == 2;
    u16 bit15 = (cmd >> 25) & 1 ? 0x8000 : 0;
    u8 *o = out_put(mb_size());
    int px, py;
    for (py = 0; py < 16; py++) {
        for (px = 0; px < 16; px++) {
            s32 c = (px >> 1) + (py >> 1) * 8, r = cr[c], b = cb[c];
            s32 y = y_blk[(py >> 3) * 2 + (px >> 3)][(px & 7) + (py & 7) * 8];
            /* R = 1.402 Cr, G = -0.3437 Cb - 0.7143 Cr, B = 1.772 Cb (12-bit fixed point) */
            s32 rr = clamp(y + ((r * 5743 + 2048) >> 12), -128, 127);
            s32 gg = clamp(y + ((b * -1408 + r * -2926 + 2048) >> 12), -128, 127);
            s32 bb = clamp(y + ((b * 7258 + 2048) >> 12), -128, 127);
            u8 R = (u8)(rr ^ (sgn ? 0 : 0x80)), G = (u8)(gg ^ (sgn ? 0 : 0x80)), B = (u8)(bb ^ (sgn ? 0 : 0x80));
            if (d24) {
                o[0] = R;
                o[1] = G;
                o[2] = B;
                o += 3;
            } else {
                u16 v = (R >> 3) | ((G >> 3) << 5) | ((B >> 3) << 10) | bit15;
                o[0] = (u8)v;
                o[1] = (u8)(v >> 8);
                o += 2;
            }
        }
    }
}

/* psx-spx y_to_mono: 8x8 pixels, 8-bit or 4-bit (first pixel in the low nibble) */
static void put_mono(const s32 *y) {
    int sgn = (cmd >> 26) & 1, d8 = depth() == 1, i;
    u8 *o = out_put(mb_size());
    for (i = 0; i < 64; i++) {
        s32 v = (s32)((y[i] & 0x1FF) ^ 0x100) - 0x100; /* signed 9 bits */
        u8 m = (u8)(clamp(v, -128, 127) ^ (sgn ? 0 : 0x80));
        if (d8)
            o[i] = m;
        else if (i & 1)
            o[i >> 1] |= (m >> 4) << 4;
        else
            o[i >> 1] = m >> 4;
    }
}

/* Decode the next macroblock if all of its input is in. */
static int decode_macroblock(void) {
    s32 cr[64], cb[64], y[4][64];
    u32 p = in_pos;
    int i;
    if (depth() < 2) {
        if (!rl_block(&p, iq_y, y[0]))
            return 0;
        idct(y[0]);
        put_mono(y[0]);
    } else {
        if (!rl_block(&p, iq_uv, cr) || !rl_block(&p, iq_uv, cb))
            return 0;
        for (i = 0; i < 4; i++)
            if (!rl_block(&p, iq_y, y[i]))
                return 0;
        idct(cr);
        idct(cb);
        for (i = 0; i < 4; i++)
            idct(y[i]);
        put_colored(cr, cb, y);
    }
    in_pos = p;
    return 1;
}

/* Decode what the input allows and the output FIFO has room for. MDEC(1) ends when its last parameter
 * word is in and no complete macroblock is left (trailing FE00h padding). */
static void run(void) {
    while (decoding) {
        if (out_w + mb_size() > OUT_MAX) {
            if (!out_r)
                return; /* full: wait for DMA1 */
            memmove(out_buf, out_buf + out_r, out_w - out_r);
            out_w -= out_r;
            out_r = 0;
        }
        if (!decode_macroblock()) {
            if (!words_left) {
                decoding = 0;
                in_len = in_pos = 0;
            }
            return;
        }
    }
}

/* ---- the command interface ---- */

static void start_command(u32 v) {
    static int warned;
    if (decoding && !warned) {
        warned = 1;
        port_log("mdec: command %08X while the last one still has macroblocks to decode\n", v);
    }
    cmd = v;
    decoding = 0;
    in_len = in_pos = table_len = 0;
    switch (v >> 29) {
    case 1: /* decode macroblocks */
        words_left = v & 0xFFFF;
        out_r = out_w = 0;
        decoding = 1;
        break;
    case 2: /* quant tables: luma, and with bit 0 chroma too */
        words_left = v & 1 ? 32 : 16;
        break;
    case 3: /* scale table */
        words_left = 32;
        break;
    default: /* no function; the status shows the parameter count field as is */
        words_left = 0;
        count = (u16)v;
        return;
    }
    count = (u16)(words_left - 1);
    if (decoding && !words_left) /* MDEC(1) without parameters: done at once */
        run();
}

static void command_done(void) {
    switch (cmd >> 29) {
    case 2:
        memcpy(iq_y, table, 64);
        if (cmd & 1)
            memcpy(iq_uv, table + 64, 64);
        break;
    case 3: {
        int i;
        for (i = 0; i < 64; i++)
            scale[i] = (s16)(table[i * 2] | (table[i * 2 + 1] << 8));
        break;
    }
    }
}

void mdec_write(u32 v) {
    if (!words_left) {
        start_command(v);
        return;
    }
    words_left--;
    count--;
    if ((cmd >> 29) == 1) { /* decoded when output is wanted, or once all of it is in */
        in_buf[in_len++] = (u16)v;
        in_buf[in_len++] = (u16)(v >> 16);
        if (!words_left)
            run();
        return;
    }
    memcpy(&table[table_len], &v, 4);
    table_len += 4;
    if (!words_left)
        command_done();
}

int mdec_out_ready(void) {
    if (out_r == out_w)
        run();
    return out_r != out_w;
}

u32 mdec_read(void) {
    u32 v;
    if (!mdec_out_ready())
        return 0;
    memcpy(&v, &out_buf[out_r], 4);
    out_r += 4;
    if (out_r >= out_w) {
        out_r = out_w = 0;
        run();
    }
    return v;
}

/* Current block (bits 16-18): the output's (Y1 for colored macroblocks here) when there is output, else
 * the one being decoded (Cr first); always 4 = Y for monochrome. */
u32 mdec_status(void) {
    int empty = out_r == out_w;
    u32 s = (u32)count | ((empty || depth() < 2 ? 4u : 0u) << 16) | (((cmd >> 25) & 15) << 23);
    if (empty)
        s |= 1u << 31;
    if (decoding && !words_left)
        s |= 1u << 30; /* last word received */
    if (decoding || words_left)
        s |= 1u << 29; /* busy */
    if (req_in && (words_left || !decoding))
        s |= 1u << 28;
    if (req_out && !empty)
        s |= 1u << 27;
    return s;
}

void mdec_control(u32 v) {
    if (v & 0x80000000) { /* reset: abort the command, status 80040000h */
        cmd = 0;
        words_left = 0;
        decoding = 0;
        in_len = in_pos = table_len = 0;
        out_r = out_w = 0;
        count = 0;
    }
    req_in = (v >> 30) & 1;
    req_out = (v >> 29) & 1;
}
