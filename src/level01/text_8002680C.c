#include "twine.h"
s32 EnableEvent(s32);
s32 OpenEvent(s32, s32, s32, void *);
M2C_UNK func_8007EE34(s32);
extern s32 D_8009C180;
extern s8 D_800A6EB0;
s32 func_80026914(void);
void func_8002680C(void);
extern s8 D_8009CDE7;
extern s16 D_8009C184;
extern s32 D_800B0B64;
extern s32 D_800AFF88;
extern s16 D_8009CF48;
extern s16 D_8009CAA4;
void func_80030118(void);
s32 func_800324C0(void);

M2C_UNK CloseEvent(s32);                            /* extern */
M2C_UNK func_8007EEBC();                            /* extern */
M2C_UNK func_80081448();                            /* extern */
extern s32 D_800AB540;


void func_8002680C(void) {
    s32 r;
    g_Args->unk2DC = 0;
    D_8009C180 = 0;
    func_80081448();
    D_800AB540 = OpenEvent(0xF2000003, 2, 0x1000, &func_80026914);
    if (D_800AB540 == -1) {
        printf(&D_800A6EB0);
    }
    r = EnableEvent(D_800AB540);
    func_8007EE34(0xF2000003);
    func_8007EEBC();
}

void func_800268CC(void) {
    func_80081448();
    CloseEvent(D_800AB540);
    func_8007EEBC();
}

s32 func_80026914(void) {

    g_Args->unk2DC++;
    if (*(u8 *)&D_8009CDE7 != 0) {
        func_80030118();
    }
    if (D_8009C184 == 1) {
        if (M2C_FIELD(g_Args, u8 *, 0x12B) != 0) {
            D_800AFF88 = g_Args->unk2DC - D_800B0B64;
            D_800B0B64 = g_Args->unk2DC;
            if (D_800AFF88 >= 0x3D) {
                g_FrameRate = 0xF;
            } else {
                g_FrameRate = 0x3C / D_800AFF88;
            }
            M2C_FIELD(g_Args, u8 *, 0x12B) = 0;
        }
        if (D_8009CF48 < 0x2710) {
            D_8009CF48 = (u16) D_8009CF48 + 1;
        }
        if ((D_8009CAA4 == 0) && (g_CutScene == 0) && (func_800324C0() != 0)) {
            D_8009C180 += 1;
        }
    }
    return 0;
}
