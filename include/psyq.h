#ifndef PSYQ_H
#define PSYQ_H

/* PsyQ 4.4 library functions linked into every executable (kept as assembly). */

typedef struct GsIMAGE {
    u32 pmode;
    s16 px, py;
    u16 pw, ph;
    u32 *pixel;
    s16 cx, cy;
    u16 cw, ch;
    u32 *clut;
} GsIMAGE;

int printf(char *fmt, ...);
int sprintf(char *buf, char *fmt, ...);
int FntPrint(char *fmt, ...);
int FntFlush(int id);
int abs(int x);

#endif
