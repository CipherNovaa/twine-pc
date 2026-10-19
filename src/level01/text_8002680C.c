#include "twine.h"
s32 EnableEvent(s32);
s32 OpenEvent(s32, s32, s32, void *);
M2C_UNK func_8007EE34(s32);
extern s32 D_8009C180;
extern s8 D_800A6EB0;
extern M2C_UNK func_80026914;
void func_8002680C(void);

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

INCLUDE_ASM("asm/level01/nonmatchings/text_8002680C", func_80026914);
