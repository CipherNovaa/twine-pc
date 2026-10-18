#include "twine.h"

s32 func_8002A854(s32, s32);
s32 func_8002BE24(char *, s32);
s32 func_8002BF34(void *, char *);
M2C_UNK func_8006B1DC(M2C_UNK *);
u8 func_8002AAC4(void);
extern char D_800A7214[];
extern u8 D_800AFF2C[];
s32 func_8002C168(char *name, s32 arg1);


extern s32 D_8009CA74;
void func_8002B0A0(void);


s32 CdSearchFile(void *, char *);
s32 func_800834D0(void *);


M2C_UNK func_8002BB68(u8);                          /* extern */
extern s8 D_8009CA58;
extern s8 D_8009CA59;
extern u8 D_8009CA5C;
extern s32 D_800AEDDC;
void func_8002AA94(int arg0);
void CdBin_Disable(void);
void CdBin_Enable(void);
extern s8 D_800A70A0;
extern M2C_UNK D_800A70C8;


void func_8002A61C(char *name, s32 *result);
M2C_UNK func_8002A720(s32);                         /* extern */
extern s8 D_800A7230;
extern s8 D_800A7244;
extern s32 D_800AEDD8;


void func_8002BB34(void);
s32 func_80082F24(M2C_UNK, M2C_UNK);                /* extern */
M2C_UNK func_800831FC(M2C_UNK, s8 *, M2C_UNK *);    /* extern */
M2C_UNK func_800833CC(s32, s8 *);                   /* extern */
extern s8 D_8009CA5A;
extern s8 D_800AF6A4;
extern u8 D_800AF6A5;
extern s32 D_800AFEA8;
extern u8 D_800AFEB7;
extern M2C_UNK D_800AFF20;


M2C_UNK func_80082F78(M2C_UNK);                     /* extern */
M2C_UNK func_80082F8C(M2C_UNK, u8 *, M2C_UNK);      /* extern */
extern u8 D_800AF620;


M2C_UNK exit(M2C_UNK);                              /* extern */
extern M2C_UNK D_800A7120;
M2C_UNK func_8002B29C(s32, M2C_UNK);                /* extern */
M2C_UNK func_80026B00(M2C_UNK);                     /* extern */


extern u8 D_8009C200;
extern s8 D_800AFEB5;
extern s8 D_800AFEB6;
extern s32 D_800AFEBC;


void func_8002A61C(char *name, s32 *result) {
    u8 mode;
    s32 start;
    s32 file[6];

    func_80082F78(0);
    mode = 0x80;
    D_800AF620 = 0x80;
    func_80082F8C(0xE, &D_800AF620, 0);
    start = g_Args->unk2DC;
    while ((u32)(g_Args->unk2DC - start) < 3) {
    }
    if (CdSearchFile(file, name) == 0) {
        *result = 2;
    } else {
        *result = func_800834D0(file);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002A61C", func_8002A720);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002A61C", func_8002A854);

void func_8002AA94(int arg0) {
  D_8009C200 = arg0;
}

u8 func_8002AAC4(void) {
    return D_8009C200;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002A61C", func_8002AAFC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002A61C", func_8002AC90);

void func_8002ADF0(void) {
    printf(&D_800A7120);
    exit(0);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002A61C", func_8002AE34);

s32 func_8002AF08(void) {
    s8 sp10;

    if (func_80082F24(1, 0) == 0) {
        return -1;
    }
    D_800AF620 = 0xC8;
    func_80082F8C(0xE, &D_800AF620, 0);
    D_800AF6A4 = 1;
    D_800AF6A5 = D_800AFEB7;
    func_8002BB34();
    func_800831FC(0xD, &D_800AF6A4, &D_800AFF20);
    func_800833CC(D_800AFEA8, &sp10);
    func_80082F8C(2, &sp10, 0);
    D_8009CA5A = 1;
    return 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002A61C", func_8002AFF8);

void func_8002B064(s32 arg0) {
    func_8002B29C(arg0, -2);
}

void func_8002B0A0(void) {
    do {
        D_8009CA58 = 9;
        if (func_80082F8C(9, NULL, 0) != 0) break;
        D_8009CA74++;
    } while (D_8009CA74 < 1000);
    D_8009CA74 = 0;
    {
        register Args *args = g_Args;
        args->flags &= ~8;
    }
    D_800AFEB6 = 0;
    D_800AFEBC = -1;
    func_8002BB34();
}

void func_8002B17C(void) {
    s8 sp10;

    if ((u8) D_800AFEB6 == 3) {
        D_8009CA58 = 6;
        func_80082F8C(6, NULL, 0);
        func_8002BB68(D_8009CA5C);
        return;
    }
    if ((u8) D_800AFEB6 == 6) {
        D_800AFEB6 = 2;
        return;
    }
    if (((u32) g_Args->flags >> 3) & 1) {
        D_8009CA59 = 1;
        D_800AF620 = 0xC8;
        func_800831FC(0xE, (s8 *) &D_800AF620, NULL);
        func_800833CC(D_800AEDDC, &sp10);
        func_80082F8C(0x15, (u8 *) &sp10, 0);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002A61C", func_8002B29C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002A61C", func_8002B69C);

void func_8002BB34(void) {
    func_80026B00(0);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002A61C", func_8002BB68);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002A61C", func_8002BC18);

void func_8002BDE4(void) {
    D_800AFEBC = -1;
    D_800AFEB6 = 0;
    D_800AFEB5 = 0x7F;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002A61C", func_8002BE24);

INCLUDE_ASM("asm/level01/nonmatchings/text_8002A61C", func_8002BF34);

s32 func_8002C168(char *name, s32 arg1) {
    s32 len;
    s32 idx;

    len = -1;
    if (g_Args->cdArchive) {
        if (((s32 (*)(void))func_8002AAC4)()) {
            idx = func_8002BF34(D_800AFF2C, name);
            len = func_8002A854(idx, arg1);
        } else {
            func_8006B1DC((M2C_UNK *)D_800A7214);
        }
    } else {
        len = func_8002BE24(name, arg1);
    }
    return len;
}

void CdBin_Disable(void) {
    func_8002AA94(0);
    g_Args->cdArchive = 0;
}

void CdBin_Enable(void) {
    g_Args->cdArchive = 1;
    g_Args->unk2DC = 0;
    func_8002AA94(1);
}

void func_8002C2D8(s32 arg0) {
    if (arg0 == 0x63) {
        sprintf((s8 *) (g_Args->exePath), &D_800A7230);
    } else {
        sprintf((s8 *) (g_Args->exePath), &D_800A7244, arg0, arg0);
    }
    func_8002A61C(g_Args->exePath, &D_800AEDD8);
    func_8002A720(D_800AEDD8);
}

void CdBin_SetLevel(s32 arg0, s32 arg1) {
    if (arg0 != 0x63) {
        if ((arg0 > 0) && (arg0 < 0xC)) {

        } else {
            printf(&D_800A70A0, &D_800A70C8, 0x61F);
            exit(1);
        }
    }
    if (arg1 != 0) {
        func_8002C2D8(arg0);
        CdBin_Enable();
        CdBin_Disable();
        return;
    }
    if ((s8) g_Args->unk1BE == 1) {
        func_8002C2D8(arg0);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002A61C", func_8002C490);

void func_8002C664(void) {
    func_80082F78(0);
    D_800AF620 = 0x80;
    func_80082F8C(0xE, &D_800AF620, 0);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002A61C", func_8002C6BC);
