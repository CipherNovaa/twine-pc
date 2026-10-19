#include "twine.h"

void func_8003B808(s32);
unsigned long func_80027248(u8 id, s32 pos, s32 flags);


typedef struct { u8 prog; u8 tone; u8 note; u8 voice; u8 vol; u8 pad5[3]; } SfxEnt;
s32 func_80026D08(u32 id, s32 flags);


s32 func_80026B44(u8 arg0);
s16 func_80026C38(int mask);
s32 func_80026ED0(void);
s32 SsVoKeyOn(s32, s32, u16, u16);
s16 func_800815A0(s16, s16, s16, s16, s16, s16, s16);
extern s32 D_8009C1E8;
typedef struct { u8 prog; u8 tone; u8 note; u8 voice; u8 vol; u8 rnd; u8 pad[2]; } SfxDef;
s32 func_80026F30(u32 id, s32 flags);


short func_800814CC(s16, s32, s32);
void func_80026C90(s32 arg0);


s32 rand();


extern char D_800A6ED0[];
extern char D_800A6EF8[];
typedef struct { u16 w; u8 pad2[0xA]; u16 x; u16 y; char text[0x50]; } TextEnt;
typedef struct { u16 a; s16 len; u16 c; s16 d; s8 e; s8 f; s8 g; u8 pad[5]; char txt[0x50]; } TxtEnt;
typedef struct { u16 first; u8 p0[2]; u16 last; u8 p1[0x5A]; } Rec60;
typedef struct { u8 p0[0xE]; s16 a; u8 p1[0x12]; s16 b; u8 p2[4]; } Rec28;
typedef struct { u8 a, b, c, d; s16 e, f; } TP8;
typedef struct { u32 tag; u8 r0, g0, b0, code; s16 x0, y0; u8 u0, v0; u16 clut; s16 w, h; } SPRT;
extern char D_800A6F34[];
void func_80029504(s16 *c);
void func_8002967C(void);
void func_80028678(void);
extern s16 D_800AB604;
extern s16 D_800AB608;
extern s16 D_800AC4E0;
extern s16 D_800AC4E4;
extern u16 D_800AC4E8;
void func_80028370(void);
void func_800274F4();
void func_80081D14(void *);
s16 GetClut(s32, s32);
s32 GetTPage(M2C_UNK, s32, M2C_UNK, M2C_UNK);
void func_8002847C(void);
extern s16 D_800AB5E8;
extern s16 D_800AB5EC;
extern s16 D_800AB5F0;
extern s16 D_800AB5F4;
extern s16 D_800AB5F8;
extern s16 D_800AB5FC;
extern s16 D_800AB60C;
extern s16 D_800AB610;
extern TP8 D_800AC4EC[];
extern Rec28 D_800AC550[];
extern s8 D_800A6F00;
extern char D_800A6F1C[];
void func_800272A4(s16 arg0, s32 arg1, s32 arg2);
void func_80081A08(u8 *);
void func_80027394(s32 pos, s16 voice, s32 id, s32 flags);
extern u8 D_800AB754[];
extern u16 D_800AB5E0;
extern u16 D_800AB5E4;
s32 func_80028958(char *text);
void func_800291B0(char *s, s16 x, s16 y, s32 *n, s32 arg4, s32 pal);
void func_800289C0(s16 arg0, char *text);


extern s32 D_8009CFB4;
extern char D_800AB618[];
void func_800286EC(s8 arg0);
void func_80028B58(s32 arg0, s32 arg1, s32 arg2);


extern s8 D_8009C1EC;
extern char D_800A6F4C[];
void Menu_ResetState(void);
void func_8006B230(s32 arg0, M2C_UNK arg1);
char *strcpy(char *dst, char *src);


extern u8 D_800AE940[];
extern u16 D_800AEDD4;


extern u8 D_800AB744[];
int toupper(int c);
void func_80029064(s32 arg0, char *arg1, s32 arg2, s32 arg3);
void func_800294B4(char *text, s32 x, s32 y, s32 idx);


int strlen(char *s);


extern M2C_UNK D_800AC4C8;
extern M2C_UNK D_800AC4D0;
extern u16 D_800AEDA0;
extern u16 D_800AEDA4;
extern u16 D_800AEDAC;
extern u16 D_800AEDB0;
extern u16 D_800AEDB4;
extern u16 D_800AEDB8;
extern u16 D_800AC4D8;
extern u16 D_800AC4DC;


void func_80028858(char *text, s16 *x, s16 *w);
s32 func_8002872C(char *text, s32 x, s32 y);
s32 func_80028810(char *text, s32 y);


extern M2C_UNK D_800AC284;
void func_80029868(char *text, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 mode);


M2C_UNK SsSetSerialAttr(M2C_UNK, M2C_UNK, M2C_UNK); /* extern */
s32 SsSetSerialVol(M2C_UNK, s32, s32);              /* extern */
s32 func_80026B00(s32 arg0);


M2C_UNK func_80081458(s16);                         /* extern */
M2C_UNK func_8006B1DC(M2C_UNK *);                   /* extern */
extern s8 D_8009C1ED;
extern M2C_UNK D_800A6EC0;


extern int D_8009C188[];
extern int D_800AEDA8;
extern int D_800AC54C;
extern s16 D_800AB614;
extern s8 D_800AB600;
extern s8 D_8009C1EE;

s32 func_80026B00(s32 arg0) {
  SsSetSerialAttr(0, 0, 1);
  arg0 = arg0;
  arg0 = arg0 & 0xFF;
  return SsSetSerialVol(0, arg0, arg0);
}

s32 func_80026B44(u8 arg0) {
    switch (arg0) {
    case 1:
        return (rand() & 3) - 2;
    case 2:
        return ((rand() & 0xFFF) % 6) - 3;
    case 3:
        return (rand() & 7) - 4;
    case 4:
        return rand() & 0;
    }
    return 0;
}

void func_80026C14(s16 arg0) {
    func_80081458(arg0);
}

s16 func_80026C38(int mask) {
    s16 i;
    for (i = 0; i < 24; i++) {
        if (mask & D_8009C188[i]) {
            return i;
        }
    }
    return -1;
}

void func_80026C90(s32 arg0) {
    s16 i;
    for (i = 0; i < 24; i++) {
        if (i != arg0) {
            func_800814CC(i, 0, 0);
            func_80081458(i);
        }
    }
}

s32 func_80026D08(u32 id, s32 flags) {
    u8 vol;
    s16 l;
    s16 r;
    s16 note;
    s32 prog;
    s32 tone;
    s32 ret;

    if (id >= 0x69) {
        return -1;
    }
    if (flags & 2) {
        vol = (*(u8 *)((u8 *)g_Args + 0x138) * *(s16 *)((u8 *)g_Args + 0x144)) / 127;
    } else {
        vol = *(u8 *)((u8 *)g_Args + 0x144);
    }
    vol = (vol * (*(SfxEnt **)((u8 *)g_Args + 0x130))[id].vol) / 127;
    l = vol;
    r = vol;
    l = l < 0 ? 0 : l < 0x80 ? l : 0x7F;
    r = r < 0 ? 0 : r < 0x80 ? r : 0x7F;
    note = (*(SfxEnt **)((u8 *)g_Args + 0x130))[id].note;
    prog = (*(SfxEnt **)((u8 *)g_Args + 0x130))[id].prog;
    tone = (*(SfxEnt **)((u8 *)g_Args + 0x130))[id].tone;
    note = note < 0 ? 0 : note < 0x80 ? note : 0x7F;
    if ((*(SfxEnt **)((u8 *)g_Args + 0x130))[id].voice != 0) {
        ret = ((s32 (*)(s32))func_80026C38)(SsVoKeyOn(prog, (*(SfxEnt **)((u8 *)g_Args + 0x130))[id].note << 8, l, r));
    } else {
        ret = func_800815A0(0, prog, tone, note, 0, l, r);
    }
    return ret;
}

s32 func_80026ED0(void) {
    u8 buf[0x18];
    s32 n = 0;
    s32 i;
    func_80081A08(buf);
    for (i = 0; i < 0x18; i++) {
        if (buf[i] == 1) n++;
    }
    return n;
}

s32 func_80026F30(u32 id, s32 flags) {
    u8 vol;
    s16 l;
    s16 r;
    s16 pitch;
    s32 prog;
    s32 tone;

    if (id < 0x69) {
        if (func_80026ED0() >= 0x14) {
            D_8009C1E8++;
            return -1;
        }
        if ((g_CutScene == 2 || g_CutScene == 3) && id != 1 && id != 2) {
            return -1;
        }
        if (flags & 2) {
            vol = (M2C_FIELD(g_Args, u8 *, 0x138) * M2C_FIELD(g_Args, s16 *, 0x13A)) / 127;
        } else {
            vol = M2C_FIELD(g_Args, s16 *, 0x13A);
        }
        vol = (vol * M2C_FIELD(g_Args, SfxDef **, 0x130)[id].vol) / 127;
        if (flags & 8) {
            r = (vol * M2C_FIELD(g_Args, u8 *, 0x136)) / 127;
            l = (vol * M2C_FIELD(g_Args, u8 *, 0x137)) / 127;
        } else {
            r = l = vol;
        }
        l = (l < 0) ? 0 : (l >= 0x80) ? 0x7F : l;
        r = (r < 0) ? 0 : (r >= 0x80) ? 0x7F : r;
        pitch = M2C_FIELD(g_Args, SfxDef **, 0x130)[id].note;
        prog = M2C_FIELD(g_Args, SfxDef **, 0x130)[id].prog;
        tone = M2C_FIELD(g_Args, SfxDef **, 0x130)[id].tone;
        if (flags & 4) {
            pitch += M2C_FIELD(g_Args, u16 *, 0x134);
        } else if (M2C_FIELD(g_Args, SfxDef **, 0x130)[id].rnd != 0) {
            pitch += func_80026B44(M2C_FIELD(g_Args, SfxDef **, 0x130)[id].rnd);
        }
        pitch = (pitch < 0) ? 0 : (pitch >= 0x80) ? 0x7F : pitch;
        if (l == 0 && r == 0) {
            return -1;
        }
        if (M2C_FIELD(g_Args, SfxDef **, 0x130)[id].voice != 0) {
            return ((s32 (*)(int))func_80026C38)(SsVoKeyOn(prog, M2C_FIELD(g_Args, SfxDef **, 0x130)[id].note << 8, l, r));
        }
        return func_800815A0(0, prog, tone, pitch, 0, l, r);
    }
    return -1;
}

unsigned long func_80027248(u8 id, s32 pos, s32 flags) {
    u8 f;

    f = flags & 2;
    if (flags & 4) {
        f |= 4;
    }
    if (pos != 0) {
        func_8003B808(pos);
        f |= 8;
    }
    return func_80026F30(id, f);
}

void func_800272A4(s16 arg0, s32 arg1, s32 arg2) {
    s32 vol;
    s32 v;
    s16 a, b;
    if (arg2 & 2) {
        vol = (g_Args->pad12E[0xA] * M2C_FIELD(g_Args, s16 *, 0x144)) / 80;
    } else {
        vol = M2C_FIELD(g_Args, s16 *, 0x144);
    }
    v = (vol * M2C_FIELD(((arg1 * 8) + M2C_FIELD(g_Args, s32 *, 0x130)), u8 *, 4)) >> 7;
    if (v >= 0) { a = 0x7F; if (v < 0x80) a = v; } else a = 0;
    if (v >= 0) { b = 0x7F; if (v < 0x80) b = v; } else b = 0;
    func_800814CC(arg0, a, b);
}

void func_80027394(s32 pos, s16 voice, s32 id, s32 flags) {
  s32 vol;
  s32 l;
  s32 r;
  u8 f;
  f = flags;
  if (pos != 0)
  {
    func_8003B808(pos);
    f = flags | 8;
  }
  if (f & 2)
  {
    vol = (((struct 
    {
      char pad[(0x138 > 0) ? (0x138) : (0)];
      typeof(*((u8 *) 0)) f;
    } *) (((char *) g_Args) + ((0x138 < 0) ? (0x138) : (0))))->f * ((struct 
    {
      char pad[(0x13A > 0) ? (0x13A) : (0)];
      typeof(*((s16 *) 0)) f;
    } *) (((char *) g_Args) + ((0x13A < 0) ? (0x13A) : (0))))->f) / 80;
  }
  else
  {
    vol = ((struct 
    {
      char pad[(0x13A > 0) ? (0x13A) : (0)];
      typeof(*((s16 *) 0)) f;
    } *) (((char *) g_Args) + ((0x13A < 0) ? (0x13A) : (0))))->f;
  }
  vol = (vol * ((struct 
  {
    char pad[(0x130 > 0) ? (0x130) : (0)];
    typeof(*((SfxDef **) 0)) f;
  } *) (((char *) g_Args) + ((0x130 < 0) ? (0x130) : (0))))->f[id].vol) >> 7;
  if (f & 8)
  {
    r = (vol * ((struct 
    {
      char pad[(0x136 > 0) ? (0x136) : (0)];
      typeof(*((u8 *) 0)) f;
    } *) (((char *) g_Args) + ((0x136 < 0) ? (0x136) : (0))))->f) >> 7;
    l = (vol * ((struct 
    {
      char pad[(0x137 > 0) ? (0x137) : (0)];
      typeof(*((u8 *) 0)) f;
    } *) (((char *) g_Args) + ((0x137 < 0) ? (0x137) : (0))))->f) >> 7;
  }
  else
  {
    r = (l = vol);
  }
  l = (l < 0) ? (0) : ((l >= 0x80) ? (0x7F) : (l));
  r = (r < 0) ? (0) : ((r >= 0x80) ? (0x7F) : (r));
  func_800814CC(voice, (s16) l, (s16) r);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_800274F4);

#ifdef NON_MATCHING
void func_80028370(void) {
    register TP8 *p REG("s0");
    s32 i = 0;
    p = D_800AC4EC;
    D_800AB5F0 = 0x33E;
    D_800AB5F4 = 0x180;
    D_800AB5F8 = 0x2C0;
    D_800AB5FC = 0x1E0;
    D_800AB5E8 = 0xF8;
    D_800AB5EC = 0x80;
    D_800AB60C = 6;
    D_800AB610 = 7;
    do {
        s32 t = GetTPage(0, 0, D_800AB5F0, D_800AB5F4);
        p->a = D_800AB5E8;
        p->e = t;
        p->f = 0;
        p->b = (u8)D_800AB5EC + D_800AB610 * i;
        i++;
        p->c = D_800AB60C;
        p++;
    } while (i < 12);
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80028370);
#endif

#ifdef NON_MATCHING
void func_8002847C(void) {
    u8 *var_a0;
    u8 *var_s1;
    s32 var_s2;
    s32 var_s3;
    s32 var_s4;
    s32 var_s5;
    u8 *temp_s0;
    s32 cur;
    u8 *base;

    D_800AEDD4 = 2;
    func_80028370();
    D_800AB604 = 0x80;
    D_800AB608 = 0;
    D_800AC4E0 = 0x360;
    D_800AC4E4 = 0x100;
    D_800AC4D8 = 0x210;
    D_800AC4DC = 0x1E1;
    D_800AEDA0 = 0x210;
    D_800AEDA4 = 0x1E0;
    D_800AEDB4 = 0x210;
    D_800AEDB8 = 0x1E0;
    D_800AEDAC = 0x210;
    D_800AEDB0 = 0x1F2;
    D_800AC4E8 = GetTPage(0, 0, 0x360, 0x100);
    Menu_ResetState();
    var_s5 = 0;
    base = D_800AC550;
    D_8009C1EE = 0;
    var_s4 = 0;
    do {
        var_s3 = 0;
        cur = var_s4;
        var_s2 = var_s4;
        var_s1 = base;
        do {
            func_80081D14((void *)(cur + (s32)var_s1));
            temp_s0 = (u8 *)(var_s2 + (s32)base);
            var_s2 += 0x14;
            var_s1 += 0x14;
            temp_s0[4] = 0x80;
            temp_s0[5] = 0x80;
            temp_s0[6] = 0x80;
            var_s3 += 1;
            *(s16 *)(temp_s0 + 0xE) = GetClut((s16)D_800AC4D8, (s16)D_800AC4DC);
            *(s16 *)(temp_s0 + 0x10) = 0x11;
            *(s16 *)(temp_s0 + 0x12) = 8;
        } while (var_s3 < 2);
        var_s5 += 1;
        var_s4 += 0x28;
    } while (var_s5 < 0xE6);
    var_s5 = 0;
    var_a0 = D_800AE940;
    do {
        *(s16 *)(var_a0 + 6) = 0;
        *(u16 *)(var_a0 + 4) = D_800AC4E8;
        var_a0[2] = 0x11;
        var_a0[0] = D_800AB604;
        var_a0[3] = 0;
        var_a0[1] = D_800AB608;
        var_s5 += 1;
        var_a0 += 8;
    } while (var_s5 < 0x8C);
    func_800274F4();
    func_80028678();
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_8002847C);
#endif

void func_80028678(void) {
    M2C_FIELD(&D_800AC4C8, u16 *, 0) = (u16) D_800AC4D8;
    M2C_FIELD(&D_800AC4D0, u16 *, 0) = (u16) D_800AC4DC;
    M2C_FIELD(&D_800AC4C8, u16 *, 2) = (u16) D_800AEDA0;
    M2C_FIELD(&D_800AC4D0, u16 *, 2) = (u16) D_800AEDA4;
    M2C_FIELD(&D_800AC4C8, u16 *, 4) = (u16) D_800AEDB4;
    M2C_FIELD(&D_800AC4D0, u16 *, 4) = (u16) D_800AEDB8;
    M2C_FIELD(&D_800AC4C8, u16 *, 6) = (u16) D_800AEDAC;
    M2C_FIELD(&D_800AC4D0, u16 *, 6) = (u16) D_800AEDB0;
}

void func_800286EC(s8 arg0) {
    if ((u32) (arg0 & 0xFF) >= 2U) {
        func_8006B1DC(&D_800A6EC0);
        return;
    }
    D_8009C1ED = arg0;
}

s32 func_8002872C(char *text, s32 x, s32 y) {
    s32 i;
    s32 len;
    char *p;

    if (*(u8 *)&D_8009C1ED == 0) {
        len = strlen(text);
        p = text;
        for (i = 0; i < len; i++) {
            *p = toupper(*(u8 *)p);
            p++;
        }
    }
    func_80029064(D_800AC54C, text, 0, 0);
    M2C_FIELD(D_800AB744 + D_800AC54C * 0x60, s16 *, 0xC) = x;
    M2C_FIELD(D_800AB744 + D_800AC54C * 0x60, s16 *, 0xE) = y;
    func_800294B4(text, x & 0xFFFF, y & 0xFFFF, D_800AC54C);
    return ++D_800AC54C;
}

s32 func_80028810(char *text, s32 y) {
    s16 w;
    s16 h;

    func_80028858(text, &w, &h);
    return func_8002872C(text, w, y);
}

void func_80028858(char *text, s16 *x, s16 *w) {
    s16 i;
    s16 len;
    s32 c;
    u8 *font;

    i = 0;
    font = D_800AE940;
    *w = 0;
    len = strlen(text);
    for (; i < len; i++) {
        c = *(u8 *)text - 0x20;
        if (c != 0x3F) {
            *w += font[c * 8 + 2] + D_800AEDD4;
        }
        text++;
    }
    *w -= D_800AEDD4;
    *x = ((s16)g_Args->displayW >> 1) - (*w >> 1);
}

s32 func_80028958(char *text) {
    s32 len;

    len = strlen(text);
    while (*text != 0) {
        if (*text - 0x20 == 0x3F) {
            len--;
        }
        text++;
    }
    return len;
}

void func_800289C0(s16 arg0, char *text) {
    char buf[32];
    s32 sp38;
    s32 maxLen;
    s32 len;
    s32 vis;
    s32 i;
    s32 end;
    TextEnt *te;
    u16 x;
    u16 y;

    if (arg0 >= D_800AC54C) {
        printf(D_800A6ED0, D_800A6EF8, 0x190);
        exit(1);
    }
    maxLen = strlen((char *)(D_800AB754 + arg0 * 0x60));
    len = strlen(text);
    vis = func_80028958(text);
    if (maxLen < vis) {
        printf(D_800A6ED0, D_800A6EF8, 0x199);
        exit(1);
    }
    strcpy(buf, text);
    if (vis < maxLen) {
        i = len;
        end = maxLen + (len - vis);
        for (; i < end; i++) {
            buf[i] = ' ';
        }
        buf[i] = 0;
    }
    te = (TextEnt *)D_800AB744;
    x = te[arg0].x;
    y = te[arg0].y;
    D_800AB5E0 = x;
    D_800AB5E4 = y;
    sp38 = te[arg0].w;
    func_800291B0(buf, x, y, &sp38, 1, -1);
}

void func_80028B58(s32 arg0, s32 arg1, s32 arg2) {
    s16 v;
    s32 i;
    Rec60 *rec;
    Rec60 *r2;
    u8 *base;
    if (arg0 >= D_800AC54C) {
        func_8006B1DC((M2C_UNK *)&D_800A6F00);
        return;
    }
    base = D_800AB744;
    rec = (Rec60 *)(arg0 * 0x60 + base);
    i = rec->first;
    if (rec->last >= i) {
        v = (arg2 << 6) | ((arg1 >> 4) & 0x3F);
        r2 = rec;
        do {
            D_800AC550[i].a = v;
            D_800AC550[i].b = v;
            i++;
        } while (r2->last >= i);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80028C08);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80028DBC);

void Menu_ResetState(void) {
    D_800AEDA8 = 0;
    D_800AC54C = 0;
    D_800AB614 = 0;
    D_800AB600 = 0;
    D_8009C1EE = 0;
}

void func_80029064(s32 arg0, char *arg1, s32 arg2, s32 arg3) {
    s32 n;
    s32 i;
    s32 cnt;
    if (arg0 >= 0x1E) {
        func_8006B230(0, D_800A6F1C);
    }
    if (arg0 == 0) {
        ((TxtEnt *)D_800AB744)[0].a = 0;
    } else {
        u8 *b = D_800AB744;
        u8 *cur = arg0 * 0x60 + b;
        s32 prev = arg0 - 1;
        *(u16 *)cur = *(u16 *)(prev * 0x60 + b + 4) + 1;
    }
    n = strlen(arg1);
    cnt = n;
    for (i = 0; i < cnt; i++) {
        s32 c = (u8)arg1[i] - 0x20;
        if (c == 0 || c == 0x3F) {
            n--;
        }
    }
    ((TxtEnt *)D_800AB744)[arg0].len = n;
    ((TxtEnt *)D_800AB744)[arg0].e = arg2;
    ((TxtEnt *)D_800AB744)[arg0].f = arg3;
    ((TxtEnt *)D_800AB744)[arg0].c = ((TxtEnt *)D_800AB744)[arg0].a + n - 1;
    strcpy(((TxtEnt *)D_800AB744)[arg0].txt, arg1);
    ((TxtEnt *)D_800AB744)[arg0].g = 1;
    ((TxtEnt *)D_800AB744)[arg0].d = 0;
}

#ifdef NON_MATCHING
void func_800291B0(char *s, s16 x, s16 y, s32 *n, s32 arg4, s32 pal) {
    s16 c;
    u8 h;
    s32 dx;
    s32 quote;
    u8 w;
    s32 j;
    TP8 *font;

    font = (TP8 *)D_800AE940;
    dx = 0;
    quote = 0;
    while (*(u8 *)s != 0) {
        if (*n >= 230) {
            func_8006B230(0, (s32)D_800A6F34);
        }
        c = *(u8 *)s - 0x20;
        if (c == 0 && arg4 == 0) {
            w = font[0].c;
            s++;
        } else if (c == 0x3F) {
            s++;
            quote = 1;
            continue;
        } else {
            h = (c >= 0x41 && c <= 0x5A) ? 7 : 8;
            if (quote) {
                ((void (*)(s16 *))func_8002967C)(&c);
            } else if (*(u8 *)&D_8009C1ED) {
                ((void (*)(s16 *, u8 *))func_80029504)(&c, &h);
            }
            w = font[c].c;
            if (font[c].d) {
                h = font[c].d;
            }
            for (j = 0; j < 2; j++) {
                (*(SPRT *)((u8 *)D_800AC550 + j * 0x14 + *n * 0x28)).w = w;
                (*(SPRT *)((u8 *)D_800AC550 + j * 0x14 + *n * 0x28)).h = h;
                (*(SPRT *)((u8 *)D_800AC550 + j * 0x14 + *n * 0x28)).u0 = font[c].a;
                (*(SPRT *)((u8 *)D_800AC550 + j * 0x14 + *n * 0x28)).v0 = font[c].b;
                (*(SPRT *)((u8 *)D_800AC550 + j * 0x14 + *n * 0x28)).x0 = x + dx;
                if (pal >= 0) {
                    (*(SPRT *)((u8 *)D_800AC550 + j * 0x14 + *n * 0x28)).clut = (((u16 *)&D_800AC4D0)[pal] << 6) | ((((u16 *)&D_800AC4C8)[pal] >> 4) & 0x3F);
                }
                (*(SPRT *)((u8 *)D_800AC550 + j * 0x14 + *n * 0x28)).y0 = y + font[c].f;
            }
            (*n)++;
            s++;
        }
        quote = 0;
        dx += D_800AEDD4 + w;
    }
}
#else
INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_800291B0);
#endif

void func_800294B4(char *text, s32 x, s32 y, s32 idx) {
    s32 sp18;

    sp18 = D_800AEDA8;
    func_800291B0(text, x, y, &sp18, 0, 0);
    D_800AEDA8 = sp18;
}

void func_80029504(s16 *c) {
    if (*c == 16) *c = 0x5F;
    else if (*c == 17) *c = 0x60;
    else if (*c == 18) *c = 0x61;
    else if (*c == 19) *c = 0x62;
    else if (*c == 20) *c = 0x63;
    else if (*c == 21) *c = 0x64;
    else if (*c == 22) *c = 0x65;
    else if (*c == 23) *c = 0x66;
    else if (*c == 24) *c = 0x67;
    else if (*c == 25) *c = 0x68;
    else if (*c == 14) *c = 0x69;
    else if (*c == 26) *c = 0x6A;
    else if (*c == 12) *c = 0x6B;
    else if (*c == 15) *c = 0x6C;
    else if (*c == 13) *c = 0x6D;
    else if (*c == 31) *c = 0x6E;
    else if (*c == 7) *c = 0x6F;
    else if (*c == 5) *c = 0x71;
}

void func_8002967C(void) {
}

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80029684);

void func_80029868(char *text, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 mode) {
    u8 *msg = (u8 *)&D_800AC284;
    char *buf;

    if ((M2C_FIELD(msg, u8 *, 0x108) == 0 || mode == 3) && ((g_Args->flags2 >> 1) & 1 || mode != 0)) {
        D_8009C1EC = 0;
        if (strlen(text) >= 300) {
            func_8006B230(0, (s32)D_800A6F4C);
        }
        buf = (char *)msg + 0x109;
        strcpy(buf, text);
        M2C_FIELD(msg, u8 *, 0x108) = 1;
        M2C_FIELD(msg, char **, 0x238) = buf;
        M2C_FIELD(msg, s32 *, 0x100) = 0;
        M2C_FIELD(msg, s16 *, 0xF0) = 0;
        M2C_FIELD(msg, s16 *, 0xF2) = 0;
        M2C_FIELD(msg, s16 *, 0xF4) = arg3;
        M2C_FIELD(msg, s16 *, 0xF6) = arg4;
        M2C_FIELD(msg, s16 *, 0xF8) = arg1;
        M2C_FIELD(msg, s16 *, 0xFA) = arg2;
        if (mode == 2) {
            M2C_FIELD(msg, s32 *, 0x104) = 1;
            M2C_FIELD(msg, u8 *, 0x23C) = 1;
        } else {
            M2C_FIELD(msg, s32 *, 0x104) = mode;
            M2C_FIELD(msg, u8 *, 0x23C) = 0;
        }
        M2C_FIELD(&D_800AC284, s16 *, 0xFC) = 0x1E;
        M2C_FIELD(&D_800AC284, u8 *, 0x242) = 0;
        M2C_FIELD(&D_800AC284, u16 *, 0x23E) = D_800AC4D8;
        M2C_FIELD(&D_800AC284, u16 *, 0x240) = D_800AC4DC;
        Menu_ResetState();
    }
}

void func_800299CC(s32 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s32 arg6, s32 arg7) {
  M2C_UNK *new_var;
  func_80029868(arg0, (s32) arg1, (s32) arg2, (s32) arg3, (s32) arg4, arg5);
  new_var = &D_800AC284;
  *((s16 *) (((s8 *) new_var) + 0x23E)) = (s16) arg6;
  *((s16 *) (((s8 *) (&D_800AC284)) + 0x240)) = (s16) arg7;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80029A34);

s32 func_8002A3C8(u8 centered) {
    s16 x;
    s16 w;
    s16 i;
    s16 n;
    s16 y;
    s16 x0;
    s16 cx;
    s32 len;
    u8 c;

    i = 0;
    len = strlen(M2C_FIELD(&D_800AC284, char **, 0x238));
    n = 0;
    D_800AB618[0] = 0;
    x0 = M2C_FIELD(&D_800AC284, s16 *, 0xF8) + 9;
    y = M2C_FIELD(&D_800AC284, s16 *, 0xFA) + 4;
    for (; i < len; i++) {
        c = M2C_FIELD(&D_800AC284, char **, 0x238)[i];
        if (c == '#') {
            c = ' ';
        }
        if (c == '\n' || i == len - 1 || c == '\t') {
            if (i == len - 1) {
                D_800AB618[n++] = c;
            }
            D_800AB618[n] = 0;
            if (centered) {
                cx = M2C_FIELD(&D_800AC284, s16 *, 0xF8) + (M2C_FIELD(&D_800AC284, s16 *, 0xF4) >> 1);
                func_80028858(D_800AB618, &x, &w);
                func_8002872C(D_800AB618, cx - (w >> 1), y);
            } else {
                if (D_8009CFB4 != 0) {
                    func_800286EC(1);
                }
                func_8002872C(D_800AB618, x0, y);
                if (D_8009CFB4 != 0) {
                    func_800286EC(0);
                }
            }
            func_80028B58(D_800AC54C - 1, M2C_FIELD(&D_800AC284, s16 *, 0x23E), M2C_FIELD(&D_800AC284, s16 *, 0x240));
            if (c == '\t') {
                M2C_FIELD(&D_800AC284, char **, 0x238) += i + 1;
                return 0;
            }
            y += 10;
            n = 0;
            D_800AB618[0] = 0;
        } else {
            D_800AB618[n++] = c;
        }
    }
    return 1;
}
