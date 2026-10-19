/* libgs functions written for the host (the rest of libgs is decompiled C, port/psyq). */
#include "port.h"
#include "psyq.h"

/* GsGetTimInfo: header of a TIM image (`tim` points at its flag word, after the 0x10 id). */
void GsGetTimInfo(u32 *tim, GsIMAGE *im) {
    u32 *p = tim;
    im->pmode = *p;
    if ((*p >> 3) & 1) {
        u32 *clut = p + 1;
        u32 *pix = (u32 *)((u8 *)clut + (*clut & ~3u));
        im->cx = ((u16 *)clut)[2];
        im->cy = ((u16 *)clut)[3];
        im->cw = ((u16 *)clut)[4];
        im->ch = ((u16 *)clut)[5];
        im->clut = clut + 3;
        p = pix;
    } else {
        p = p + 1;
    }
    im->px = ((u16 *)p)[2];
    im->py = ((u16 *)p)[3];
    im->pw = ((u16 *)p)[4];
    im->ph = ((u16 *)p)[5];
    im->pixel = p + 3;
}

/* GsMapModelingData: turn a TMD's object-table offsets (vertices, normals, primitives) into addresses.
 * `p` points at the TMD flag word; bit 0 records that it's done. */
void GsMapModelingData(u32 *p) {
    u32 *obj, n, i;
    if (*p & 1)
        return;
    *p |= 1;
    n = p[1];
    obj = p + 2;
    for (i = 0; i < n; i++) {
        u32 *o = obj + i * 7;
        o[0] += (u32)obj;
        o[4] += (u32)obj;
        o[2] += (u32)obj;
    }
}
