/* libgpu packet helpers (pure functions on primitive structs; same results as PsyQ's). */
#include "port.h"

typedef struct {
    s16 x, y, w, h;
} RECT;

#define SETLEN_CODE(p, len, code) (((u8 *)(p))[3] = (len), ((u8 *)(p))[7] = (code))

void SetPolyF3(void *p) { SETLEN_CODE(p, 4, 0x20); }
void SetPolyFT3(void *p) { SETLEN_CODE(p, 7, 0x24); }
void SetPolyF4(void *p) { SETLEN_CODE(p, 5, 0x28); }
void SetPolyFT4(void *p) { SETLEN_CODE(p, 9, 0x2C); }
void SetPolyG3(void *p) { SETLEN_CODE(p, 6, 0x30); }
void SetPolyGT3(void *p) { SETLEN_CODE(p, 9, 0x34); }
void SetPolyG4(void *p) { SETLEN_CODE(p, 8, 0x38); }
void SetPolyGT4(void *p) { SETLEN_CODE(p, 12, 0x3C); }
void SetLineF2(void *p) { SETLEN_CODE(p, 3, 0x40); }
void SetLineG2(void *p) { SETLEN_CODE(p, 4, 0x50); }
void SetSprt(void *p) { SETLEN_CODE(p, 4, 0x64); }
void SetSprt16(void *p) { SETLEN_CODE(p, 3, 0x7C); }
void SetSprt8(void *p) { SETLEN_CODE(p, 3, 0x74); }
void SetTile(void *p) { SETLEN_CODE(p, 3, 0x60); }
void SetTile1(void *p) { SETLEN_CODE(p, 2, 0x68); }
void SetTile8(void *p) { SETLEN_CODE(p, 2, 0x70); }
void SetTile16(void *p) { SETLEN_CODE(p, 2, 0x78); }

void SetLineF3(void *p) {
    SETLEN_CODE(p, 5, 0x48);
    ((u32 *)p)[4] = 0x55555555;
}

void SetLineF4(void *p) {
    SETLEN_CODE(p, 6, 0x4C);
    ((u32 *)p)[6] = 0x55555555;
}

void SetLineG3(void *p) {
    SETLEN_CODE(p, 7, 0x58);
    ((u32 *)p)[7] = 0x55555555;
}

void SetLineG4(void *p) {
    SETLEN_CODE(p, 9, 0x5C);
    ((u32 *)p)[9] = 0x55555555;
}

void SetSemiTrans(void *p, s32 abe) {
    if (abe)
        ((u8 *)p)[7] |= 2;
    else
        ((u8 *)p)[7] &= ~2;
}

void SetShadeTex(void *p, s32 tge) {
    if (tge)
        ((u8 *)p)[7] |= 1;
    else
        ((u8 *)p)[7] &= ~1;
}

void SetDrawMode(void *p, s32 dfe, s32 dtd, s32 tpage, RECT *tw) {
    u32 *w = (u32 *)p;
    ((u8 *)p)[3] = 2;
    w[1] = 0xE1000000 | (dtd ? 0x200 : 0) | (dfe ? 0x400 : 0) | (tpage & 0x9FF);
    if (tw)
        w[2] = 0xE2000000 | (((u8)tw->y >> 3) << 15) | (((u8)tw->x >> 3) << 10) | ((-tw->h << 2) & 0x3E0) |
               ((-tw->w & 0xFF) >> 3);
    else
        w[2] = 0;
}

u16 GetTPage(s32 tp, s32 abr, s32 x, s32 y) {
    return ((tp & 3) << 7) | ((abr & 3) << 5) | ((y & 0x100) >> 4) | ((x & 0x3FF) >> 6) | ((y & 0x200) << 2);
}

u16 GetClut(s32 x, s32 y) {
    return (u16)((y << 6) | ((x >> 4) & 0x3F));
}

void AddPrim(u32 *ot, u32 *p) {
    *p = (*p & 0xFF000000) | (*ot & 0xFFFFFF);
    *ot = (*ot & 0xFF000000) | ((u32)p & 0xFFFFFF);
}

void AddPrims(u32 *ot, u32 *first, u32 *last) {
    *last = (*last & 0xFF000000) | (*ot & 0xFFFFFF);
    *ot = (*ot & 0xFF000000) | ((u32)first & 0xFFFFFF);
}
