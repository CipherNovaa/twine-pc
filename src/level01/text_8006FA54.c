#include "twine.h"

s32 func_80071638(void *, s32, s32, s32, void *, s32);
s32 func_80072190(void *, s32, s32, s32, void *);
s32 Vec_Dot(register s32 *row, register SVECTOR *v);


s32 func_8006FB04(void *, void *, void *);
s32 func_8006FED4(void *, void *, void *);


void func_8006FA54(s32 arg0);


extern M2C_UNK D_800A97B0;


void func_8006FA54(s32 arg0) {
    M2C_FIELD(arg0, s32 *, 0x18) = arg0 + M2C_FIELD(arg0, s32 *, 0x18);
    M2C_FIELD(arg0, s32 *, 0x1C) = arg0 + M2C_FIELD(arg0, s32 *, 0x1C);
    M2C_FIELD(arg0, s32 *, 0x20) = arg0 + M2C_FIELD(arg0, s32 *, 0x20);
    M2C_FIELD(arg0, s32 *, 0x24) = arg0 + M2C_FIELD(arg0, s32 *, 0x24);
    M2C_FIELD(arg0, s32 *, 0x14) = arg0 + M2C_FIELD(arg0, s32 *, 0x14);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_8006FB04);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_8006FED4);

s32 func_80070070(void *arg0, SVECTOR *arg1, u8 *arg2, s16 *arg3, s16 arg4, s32 *arg5) {
    s32 h;
    s32 d;
    s32 bestH;
    s32 bestD;
    s32 bestIdx;
    s16 *p;

    bestH = -0x7FFF;
    bestD = 0x7FFF;
    bestIdx = -1;
    for (p = arg3; p < arg3 + arg4; p++) {
        if (M2C_FIELD(M2C_FIELD(arg0, u8 **, 0x20) + M2C_FIELD(arg2 + *p * 12, u16 *, 6) * 8, s16 *, 2) < 0 && func_8006FB04(arg0, arg1, arg2 + *p * 12)) {
            h = func_8006FED4(arg0, arg2 + *p * 12, arg1);
            d = (h - arg1->vy) < 0 ? -(h - arg1->vy) : (h - arg1->vy);
            if (d < bestD) {
                bestD = d;
                bestH = h;
                bestIdx = *p;
            }
        }
    }
    *arg5 = bestIdx;
    return bestH;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_800702CC);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_800705A8);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_800706F0);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_80071638);

INCLUDE_ASM("asm/level01/nonmatchings/text_8006FA54", func_80072190);

s32 func_8007240C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 (*arg4)[3], s16 *arg5, s16 arg6, s32 *arg7, s32 arg8, s32 *arg9, s32 arg10, s32 arg11) {
    s32 i;
    s32 j;
    s32 start;
    s32 (*face)[3];
    SVECTOR *nrm;

    start = arg11;
    for (i = 0; i < arg6; i++) {
        face = arg4 + arg5[i];
        nrm = M2C_FIELD(arg0, SVECTOR **, 0x20) + M2C_FIELD(face, u16 *, 6);
        if (func_80071638(arg0, arg1, arg2, arg3, face, arg8) && Vec_Dot(arg7, nrm) < 0 && func_80072190(arg0, arg1, arg2, arg3, face)) {
            for (j = 0; j < arg11; j++) {
                if (arg9[j] == arg5[i]) {
                    break;
                }
            }
            if (j >= arg11) {
                arg9[arg11++] = arg5[i];
                if (arg11 >= arg10) {
                    return arg11 - start;
                }
            }
        }
    }
    return arg11 - start;
}

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
