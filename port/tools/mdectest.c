/* Check the MDEC model (port/src/mdec.c) on its own: take one frame of a movie straight from the disc
 * image, turn its BS bitstream into MDEC run-length codes (the work the game's DecDCTvlc does on the CPU),
 * run them through the MDEC with the standard quant and scale tables, and write the picture as a PPM.
 * Written from psx-spx ("CDROM File Video Streaming STR (Sony)", "CDROM File Video BS Compression ...").
 *
 *   make -C port mdectest   (-> build/port/mdectest)
 *   mdectest [-15] [-all] DISC.bin '\MOVIE\EAGAMES.STR' FRAME OUT.ppm
 *
 * DISC.bin is the raw (2352-byte sector) image. -15 decodes to 15-bit pixels instead of 24-bit; -all also
 * decodes every frame of the movie and reports any whose codes don't decode cleanly. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "mdec.h"

void port_log(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}

static FILE *disc;

static int read_raw(u32 lba, u8 *raw) {
    return !fseek(disc, (long)lba * 2352, SEEK_SET) && fread(raw, 1, 2352, disc) == 2352;
}

static u32 le32(const u8 *p) {
    return p[0] | (p[1] << 8) | (p[2] << 16) | ((u32)p[3] << 24);
}

static u16 le16(const u8 *p) {
    return (u16)(p[0] | (p[1] << 8));
}

/* ISO 9660: find "\DIR\FILE;1" from the root directory */
static int find_file(const char *path, u32 *lba, u32 *size) {
    u8 raw[2352], *d = raw + 24;
    char name[64];
    const char *s = path;
    if (!read_raw(16, raw))
        return 0;
    *lba = le32(d + 156 + 2);
    *size = le32(d + 156 + 10);
    while (*s == '\\')
        s++;
    while (*s) {
        const char *e = strchr(s, '\\');
        size_t n = e ? (size_t)(e - s) : strlen(s);
        u32 off, found = 0;
        if (n >= sizeof(name))
            return 0;
        memcpy(name, s, n);
        name[n] = 0;
        if (!e && !strchr(name, ';'))
            strcat(name, ";1");
        n = strlen(name);
        for (off = 0; off < *size && !found; off += 2048) {
            u32 p = 0;
            if (!read_raw(*lba + off / 2048, raw))
                return 0;
            while (p < 2048 && d[p]) {
                if (d[p + 32] == n && !strncasecmp((char *)d + p + 33, name, n)) {
                    *lba = le32(d + p + 2);
                    *size = le32(d + p + 10);
                    found = 1;
                    break;
                }
                p += d[p];
            }
        }
        if (!found)
            return 0;
        s = e ? e + 1 : s + strlen(s);
    }
    return 1;
}

/* ---- STR: one frame's BS data from its sectors' 7E0h-byte payloads ---- */

typedef struct {
    u8 *data;
    u32 size, width, height;
} Frame;

static int read_frame(u32 lba, u32 sectors, u32 frame_no, Frame *f) {
    u8 raw[2352];
    u32 i, count = 0, have = 0;
    free(f->data);
    memset(f, 0, sizeof(*f));
    for (i = 0; i < sectors; i++) {
        const u8 *h = raw + 24;
        u32 idx, n;
        if (!read_raw(lba + i, raw))
            break;
        /* form 1 sectors (submode bit 5 clear) with the STR header 0160h/8001h; the submode's video bit
         * isn't always set */
        if ((raw[18] & 0x20) || le16(h) != 0x0160 || le16(h + 2) != 0x8001 || le32(h + 8) != frame_no)
            continue;
        idx = le16(h + 4);
        n = le16(h + 6);
        if (!f->data) {
            count = n;
            f->data = calloc(n, 0x7E0);
            f->size = le32(h + 12);
            f->width = le16(h + 16);
            f->height = le16(h + 18);
        }
        if (idx < count && !(have & (1u << idx))) {
            memcpy(f->data + idx * 0x7E0, h + 0x20, 0x7E0);
            have |= 1u << idx;
        }
        if (count && have == (1u << count) - 1)
            return 1;
    }
    return 0;
}

/* ---- BS v2/v3 bitstream -> MDEC codes ---- */

typedef struct {
    const u8 *p, *end;
    u32 buf; /* the next bits, MSB first */
    int n;
} Bits;

static void fill(Bits *b) {
    while (b->n <= 16) {
        u32 h = b->p + 1 < b->end ? le16(b->p) : 0;
        b->p += 2;
        b->buf |= h << (16 - b->n);
        b->n += 16;
    }
}

static u32 peek(Bits *b, int n) {
    fill(b);
    return b->buf >> (32 - n);
}

static void skip(Bits *b, int n) {
    b->buf <<= n;
    b->n -= n;
}

static u32 get(Bits *b, int n) {
    u32 v = peek(b, n);
    skip(b, n);
    return v;
}

static int clz16(u32 v) {
    int z = 0;
    while (z < 16 && !(v & (0x8000 >> z)))
        z++;
    return z;
}

/* AC codes (psx-spx "Huffman codes for AC values BS v1/v2/v3/iki"): FE00h for end of block */
static const u16 ac_z2[8] = {0x3401, 0x0006, 0x3001, 0x2C01, 0x0C02, 0x0403, 0x0005, 0x2801};
static const u16 ac_z3[4] = {0x1C01, 0x1801, 0x0402, 0x1401};
static const u16 ac_z4[4] = {0x0802, 0x2401, 0x0004, 0x2001};
static const u16 ac_z6[8] = {0x4001, 0x1402, 0x0007, 0x0803, 0x0404, 0x3C01, 0x3801, 0x1002};
static const u16 ac_z7[16] = {0x000B, 0x2002, 0x1003, 0x000A, 0x0804, 0x1C02, 0x5401, 0x5001,
                              0x0009, 0x4C01, 0x4801, 0x0405, 0x0C03, 0x0008, 0x1802, 0x4401};
static const u16 ac_z8[16] = {0x2802, 0x2402, 0x1403, 0x0C04, 0x0805, 0x0407, 0x0406, 0x000F,
                              0x000E, 0x000D, 0x000C, 0x6801, 0x6401, 0x6001, 0x5C01, 0x5801};
static const u16 ac_z9[16] = {0x001F, 0x001E, 0x001D, 0x001C, 0x001B, 0x001A, 0x0019, 0x0018,
                              0x0017, 0x0016, 0x0015, 0x0014, 0x0013, 0x0012, 0x0011, 0x0010};
static const u16 ac_z10[16] = {0x0028, 0x0027, 0x0026, 0x0025, 0x0024, 0x0023, 0x0022, 0x0021,
                               0x0020, 0x040E, 0x040D, 0x040C, 0x040B, 0x040A, 0x0409, 0x0408};
static const u16 ac_z11[16] = {0x0412, 0x0411, 0x0410, 0x040F, 0x1803, 0x4002, 0x3C02, 0x3802,
                               0x3402, 0x3002, 0x2C02, 0x7C01, 0x7801, 0x7401, 0x7001, 0x6C01};

/* one AC code -> MDEC value; -1 on an unused code */
static s32 ac_code(Bits *b) {
    u32 v = peek(b, 16), code, x;
    int z = clz16(v), len, xbits;
    const u16 *tab;
    switch (z) {
    case 0:
        if (!(v & 0x4000)) {
            skip(b, 2);
            return 0xFE00;
        }
        code = 0x0001, len = 3;
        break;
    case 1:
        if (v & 0x2000)
            code = 0x0401, len = 4;
        else
            code = (v & 0x1000) ? 0x0801 : 0x0002, len = 5;
        break;
    case 2:
        if (v & 0x1000)
            code = (v & 0x0800) ? 0x0C01 : 0x1001, len = 6;
        else if (v & 0x0800)
            code = 0x0003, len = 6;
        else
            code = ac_z2[(v >> 8) & 7], len = 9;
        break;
    case 5:
        skip(b, 6);
        return get(b, 16);
    default:
        switch (z) {
        case 3: tab = ac_z3, xbits = 2; break;
        case 4: tab = ac_z4, xbits = 2; break;
        case 6: tab = ac_z6, xbits = 3; break;
        case 7: tab = ac_z7, xbits = 4; break;
        case 8: tab = ac_z8, xbits = 4; break;
        case 9: tab = ac_z9, xbits = 4; break;
        case 10: tab = ac_z10, xbits = 4; break;
        case 11: tab = ac_z11, xbits = 4; break;
        default: return -1;
        }
        /* z zeroes, a one, the index, the sign: 17 bits at most, more than one peek */
        skip(b, z + 1);
        x = get(b, xbits);
        code = tab[x];
        len = 1;
        if (get(b, 1))
            code = (code & 0xFC00) | ((-(s32)(code & 0x3FF)) & 0x3FF);
        return code;
    }
    skip(b, len - 1);
    if (get(b, 1)) /* s: negative level */
        code = (code & 0xFC00) | ((-(s32)(code & 0x3FF)) & 0x3FF);
    return code;
}

/* v3 DC: size by a prefix code, then size bits (top bit 0 = negative), times 4 */
static int dc_v3(Bits *b, int chroma, s32 *diff) {
    static const struct {
        u16 code;
        u8 len, size;
    } y_codes[] = {{0x4, 3, 0}, {0x0, 2, 1}, {0x1, 2, 2}, {0x5, 3, 3}, {0x6, 3, 4}, {0xE, 4, 5}, {0x1E, 5, 6},
                   {0x3E, 6, 7}, {0x7E, 7, 8}},
      c_codes[] = {{0x0, 2, 0}, {0x1, 2, 1}, {0x2, 2, 2}, {0x6, 3, 3}, {0xE, 4, 4}, {0x1E, 5, 5}, {0x3E, 6, 6},
                   {0x7E, 7, 7}, {0xFE, 8, 8}};
    const typeof(y_codes[0]) *t = chroma ? c_codes : y_codes;
    int i;
    for (i = 0; i < 9; i++) {
        if (peek(b, t[i].len) == t[i].code) {
            s32 v;
            skip(b, t[i].len);
            if (!t[i].size) {
                *diff = 0;
                return 1;
            }
            v = get(b, t[i].size);
            if (!(v >> (t[i].size - 1)))
                v -= (1 << t[i].size) - 1;
            *diff = v * 4;
            return 1;
        }
    }
    return 0;
}

/* Returns the number of MDEC halfwords written to out (after the 2-halfword command), or -1. */
static s32 bs_decode(const u8 *bs, u32 size, u16 *out, u32 max, u32 *mdec_words) {
    Bits b = {bs + 8, bs + size, 0, 0};
    u32 quant = le16(bs + 4), version = le16(bs + 6), n = 0;
    s32 old_dc[3] = {0, 0, 0};
    *mdec_words = le16(bs);
    if (le16(bs + 2) != 0x3800 || (version != 2 && version != 3)) {
        fprintf(stderr, "not a BS v2/v3 frame (id %04X version %u)\n", le16(bs + 2), version);
        return -1;
    }
    for (;;) {
        int blk;
        for (blk = 0; blk < 6; blk++) {
            s32 dc;
            if (version == 2) {
                dc = (s32)peek(&b, 10);
                if (blk == 0 && dc == 0x1FF)
                    goto done; /* end of frame */
                skip(&b, 10);
            } else {
                s32 diff, ch = blk < 2 ? blk : 2;
                if (peek(&b, 10) == 0x3FF)
                    goto done;
                if (!dc_v3(&b, blk < 2, &diff))
                    return -1;
                dc = old_dc[ch] = old_dc[ch] + diff;
            }
            if (n + 64 > max)
                return -1;
            out[n++] = (u16)((quant << 10) | (dc & 0x3FF));
            for (;;) {
                s32 c = ac_code(&b);
                if (c < 0)
                    return -1;
                out[n++] = (u16)c;
                if (c == 0xFE00)
                    break;
                if (n >= max)
                    return -1;
            }
        }
        if (b.p > b.end + 4)
            return -1;
    }
done:
    return n;
}

/* ---- MDEC ---- */

static const u8 quant_table[64] = {
    0x02, 0x10, 0x10, 0x13, 0x10, 0x13, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x1A, 0x18, 0x1A, 0x1B,
    0x1B, 0x1B, 0x1A, 0x1A, 0x1A, 0x1A, 0x1B, 0x1B, 0x1B, 0x1D, 0x1D, 0x1D, 0x22, 0x22, 0x22, 0x1D,
    0x1D, 0x1D, 0x1B, 0x1B, 0x1D, 0x1D, 0x20, 0x20, 0x22, 0x22, 0x25, 0x26, 0x25, 0x23, 0x23, 0x22,
    0x23, 0x26, 0x26, 0x28, 0x28, 0x28, 0x30, 0x30, 0x2E, 0x2E, 0x38, 0x38, 0x3A, 0x45, 0x45, 0x53,
};

static const u16 scale_table[64] = {
    0x5A82, 0x5A82, 0x5A82, 0x5A82, 0x5A82, 0x5A82, 0x5A82, 0x5A82, 0x7D8A, 0x6A6D, 0x471C, 0x18F8, 0xE707,
    0xB8E3, 0x9592, 0x8275, 0x7641, 0x30FB, 0xCF04, 0x89BE, 0x89BE, 0xCF04, 0x30FB, 0x7641, 0x6A6D, 0xE707,
    0x8275, 0xB8E3, 0x471C, 0x7D8A, 0x18F8, 0x9592, 0x5A82, 0xA57D, 0xA57D, 0x5A82, 0x5A82, 0xA57D, 0xA57D,
    0x5A82, 0x471C, 0x8275, 0x18F8, 0x6A6D, 0x9592, 0xE707, 0x7D8A, 0xB8E3, 0x30FB, 0x89BE, 0x7641, 0xCF04,
    0xCF04, 0x7641, 0x89BE, 0x30FB, 0x18F8, 0xB8E3, 0x6A6D, 0x8275, 0x7D8A, 0x9592, 0x471C, 0xE707,
};

static void mdec_setup(void) {
    u32 i, w;
    mdec_control(0x80000000);
    mdec_control(0x60000000);
    mdec_write(0x40000001); /* both quant tables */
    for (i = 0; i < 128; i += 4) {
        memcpy(&w, &quant_table[i & 63], 4);
        mdec_write(w);
    }
    mdec_write(0x60000000);
    for (i = 0; i < 64; i += 2)
        mdec_write(scale_table[i] | ((u32)scale_table[i + 1] << 16));
}

/* Decode one frame's codes to RGB (w x h, 3 bytes a pixel). Returns 0 if the MDEC output is short. */
static int mdec_frame(const u16 *codes, u32 words, int d15, u32 w, u32 h, u8 *rgb) {
    u32 mb_w = (w + 15) / 16, mb_h = (h + 15) / 16, mb, mb_bytes = d15 ? 512 : 768, i, st;
    u8 *buf = malloc(mb_w * mb_h * mb_bytes);
    u32 got = 0;
    /* MDEC(1): 15-bit (with bit 15 set, as the game asks) or 24-bit, unsigned */
    mdec_write((d15 ? 0x3A000000u : 0x30000000u) | words);
    for (i = 0; i < words; i++)
        mdec_write(codes[i * 2] | ((u32)codes[i * 2 + 1] << 16));
    while (got < mb_w * mb_h * mb_bytes && mdec_out_ready()) {
        u32 v = mdec_read();
        memcpy(buf + got, &v, 4);
        got += 4;
    }
    st = mdec_status();
    if (got != mb_w * mb_h * mb_bytes || mdec_out_ready() || (st & 0x20000000)) {
        fprintf(stderr, "MDEC output %u of %u bytes, more %d, status %08X\n", got, mb_w * mb_h * mb_bytes,
                mdec_out_ready(), st);
        free(buf);
        return 0;
    }
    /* macroblocks go down the columns, then across */
    for (mb = 0; mb < mb_w * mb_h; mb++) {
        u32 mx = mb / mb_h * 16, my = mb % mb_h * 16, px, py;
        const u8 *m = buf + mb * mb_bytes;
        for (py = 0; py < 16; py++)
            for (px = 0; px < 16; px++) {
                u8 *o;
                if (mx + px >= w || my + py >= h)
                    continue;
                o = rgb + ((my + py) * w + mx + px) * 3;
                if (d15) {
                    u16 c = le16(m + (py * 16 + px) * 2);
                    o[0] = (u8)((c & 31) << 3);
                    o[1] = (u8)(((c >> 5) & 31) << 3);
                    o[2] = (u8)(((c >> 10) & 31) << 3);
                } else
                    memcpy(o, m + (py * 16 + px) * 3, 3);
            }
    }
    free(buf);
    return 1;
}

static int decode(const Frame *f, int d15, u8 *rgb, int verbose) {
    static u16 codes[0x20000 + 64];
    u32 words, i;
    s32 n = bs_decode(f->data, f->size, codes, 0x20000, &words);
    if (n < 0 || (u32)n > words * 2) {
        fprintf(stderr, "bad bitstream (%d halfwords for %u words)\n", n, words);
        return 0;
    }
    for (i = n; i < words * 2; i++)
        codes[i] = 0xFE00;
    if (verbose)
        printf("%ux%u, BS v%u quant %u: %d MDEC halfwords (%u words with padding)\n", f->width, f->height,
               le16(f->data + 6), le16(f->data + 4), n, words);
    return mdec_frame(codes, words, d15, f->width, f->height, rgb);
}

int main(int argc, char **argv) {
    int d15 = 0, all = 0;
    u32 lba, size, frame_no, sectors;
    Frame f = {0};
    u8 *rgb;
    FILE *out;
    while (argc > 1 && argv[1][0] == '-') {
        if (!strcmp(argv[1], "-15"))
            d15 = 1;
        else if (!strcmp(argv[1], "-all"))
            all = 1;
        argc--, argv++;
    }
    if (argc != 5) {
        fprintf(stderr, "usage: mdectest [-15] [-all] DISC.bin '\\MOVIE\\X.STR' FRAME OUT.ppm\n");
        return 2;
    }
    disc = fopen(argv[1], "rb");
    if (!disc || !find_file(argv[2], &lba, &size)) {
        fprintf(stderr, "%s: no %s\n", argv[1], argv[2]);
        return 1;
    }
    sectors = (size + 2047) / 2048;
    printf("%s: sector %u, %u sectors\n", argv[2], lba, sectors);
    mdec_setup();
    if (all) {
        u32 n, bad = 0;
        for (n = 1; read_frame(lba, sectors, n, &f); n++) {
            rgb = malloc(f.width * f.height * 3);
            if (!decode(&f, d15, rgb, 0)) {
                printf("frame %u: failed\n", n);
                bad++;
            }
            free(rgb);
        }
        printf("%u frames, %u failed\n", n - 1, bad);
    }
    frame_no = (u32)atoi(argv[3]);
    if (!read_frame(lba, sectors, frame_no, &f)) {
        fprintf(stderr, "frame %u not found\n", frame_no);
        return 1;
    }
    rgb = calloc(f.width * f.height, 3);
    if (!decode(&f, d15, rgb, 1))
        return 1;
    out = fopen(argv[4], "wb");
    if (!out)
        return 1;
    fprintf(out, "P6\n%u %u\n255\n", f.width, f.height);
    fwrite(rgb, 3, f.width * f.height, out);
    fclose(out);
    printf("frame %u -> %s\n", frame_no, argv[4]);
    return 0;
}
