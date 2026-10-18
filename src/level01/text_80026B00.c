#include "twine.h"

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


void func_80028858(char *, s16 *, s16 *);
s32 func_8002872C(char *, s32, s32);
s32 func_80028810(char *text, s32 y);


extern M2C_UNK D_800AC284;
M2C_UNK func_80029868(s32, M2C_UNK, M2C_UNK, M2C_UNK, s32, s32);


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

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80026B44);

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

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80026C90);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80026D08);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80026ED0);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80026F30);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80027248);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_800272A4);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80027394);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_800274F4);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80028370);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_8002847C);

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

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_8002872C);

s32 func_80028810(char *text, s32 y) {
    s16 w;
    s16 h;

    func_80028858(text, &w, &h);
    return func_8002872C(text, w, y);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80028858);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80028958);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_800289C0);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80028B58);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80028C08);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80028DBC);

void Menu_ResetState(void) {
    D_800AEDA8 = 0;
    D_800AC54C = 0;
    D_800AB614 = 0;
    D_800AB600 = 0;
    D_8009C1EE = 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80029064);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_800291B0);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_800294B4);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80029504);

void func_8002967C(void) {
}

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80029684);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80029868);

void func_800299CC(s32 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s32 arg6, s32 arg7) {
  M2C_UNK *new_var;
  func_80029868(arg0, (s32) arg1, (s32) arg2, (s32) arg3, (s32) arg4, arg5);
  new_var = &D_800AC284;
  *((s16 *) (((s8 *) new_var) + 0x23E)) = (s16) arg6;
  *((s16 *) (((s8 *) (&D_800AC284)) + 0x240)) = (s16) arg7;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_80029A34);

INCLUDE_ASM("asm/level01/nonmatchings/text_80026B00", func_8002A3C8);
