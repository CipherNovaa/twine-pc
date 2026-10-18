#include "twine.h"

M2C_UNK CloseEvent(s32);                            /* extern */
M2C_UNK func_8007EEBC();                            /* extern */
M2C_UNK func_80081448();                            /* extern */
extern s32 D_800AB540;


INCLUDE_ASM("asm/level01/nonmatchings/text_8002680C", func_8002680C);

void func_800268CC(void) {
    func_80081448();
    CloseEvent(D_800AB540);
    func_8007EEBC();
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002680C", func_80026914);
