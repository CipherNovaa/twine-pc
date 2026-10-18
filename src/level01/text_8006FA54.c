#include "twine.h"

extern M2C_UNK D_800A97B0;


INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_8006FA54);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_8006FB04);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_8006FED4);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_80070070);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_800702CC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_800705A8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_800706F0);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_80071638);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_80072190);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_8007240C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_800726D0);

void func_80072D48(register void *arg0, register void *arg1, register void *arg2, register s32 *arg3, register s32 *arg4) {
    s32 sp0;
    s32 sp4;

    if (M2C_FIELD(arg0, s16 *, 2) < M2C_FIELD(arg1, s16 *, 2)) {
        sp0 = (s32) M2C_FIELD(arg0, s16 *, 2);
        sp4 = (s32) M2C_FIELD(arg1, s16 *, 2);
    } else {
        sp0 = (s32) M2C_FIELD(arg1, s16 *, 2);
        sp4 = (s32) M2C_FIELD(arg0, s16 *, 2);
    }
    if (M2C_FIELD(arg2, s16 *, 2) < sp0) {
        sp0 = (s32) M2C_FIELD(arg2, s16 *, 2);
    } else if (sp4 < M2C_FIELD(arg2, s16 *, 2)) {
        sp4 = (s32) M2C_FIELD(arg2, s16 *, 2);
    }
    *arg3 = sp0;
    *arg4 = sp4;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_80072E28);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_8007369C);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_800743E8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_800746D0);

void DebugPage_ContourMask(void) {
    g_DebugTextX = 0xE;
    g_DebugTextY = 1;
    FntPrint(&D_800A97B0);
}

void func_80074E38(void) {

}

s32 Vec_Dot(register s32 *row, register SVECTOR *v) {
    s32 result;

    gte_ldR11R12R13R21(row);
    gte_ldsv(v);
    gte_mvmva_rt_ir();
    gte_mfMAC1(result);
    return result;
}

void Vec_ProjectXZ(register SVECTOR *v, register s32 *rowA, register s32 *rowC, register s32 *out1, register s32 *out3) {
    s32 temp;

    gte_ldR11R12R13R21(rowA);
    gte_ldR31R32R33(rowC);
    gte_ldsv(v);
    gte_mvmva_rt_ir();
    gte_mfMAC1(temp);
    *out1 = temp;
    gte_mfMAC3(temp);
    *out3 = temp;
}

void Vec_Scale(register s32 scale, register SVECTOR *in, register SVECTOR *out) {
    gte_lddp(scale);
    gte_ldsv(in);
    gte_gpf12();
    gte_stsv(out);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_80074FE8);

s32 Math_Abs(register s32 x) {
    return abs(x);
}

s32 Math_Max(register s32 a, register s32 b) {
    return (a < b) ? b : a;
}

s32 Math_Min3(register s32 a, register s32 b, register s32 c) {
    s32 m;

    m = MIN(a, b);
    return MIN(m, c);
}

s32 Math_Max3(register s32 a, register s32 b, register s32 c) {
    s32 m;

    m = MAX(a, b);
    return MAX(m, c);
}
