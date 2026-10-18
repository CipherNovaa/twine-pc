#include "twine.h"

extern M2C_UNK D_800F0F80;


extern void *D_8009CD98;
void func_80076184(s32 idx, u32 val);


s32 func_800752C0(void);
void func_800758CC(void);
void func_80075F0C(void);


M2C_UNK func_8006B230(M2C_UNK, s32);                /* extern */
s32 func_80075FE4(int arg0);
extern M2C_UNK D_800A97D0;
extern M2C_UNK D_800E2F5C;


M2C_UNK func_80030670(s32);                         /* extern */
M2C_UNK func_8008D2EC(M2C_UNK *);                   /* extern */
extern M2C_UNK D_800B0C10;
M2C_UNK func_80082D00(M2C_UNK, M2C_UNK);            /* extern */
M2C_UNK func_8006B1DC(M2C_UNK *);                   /* extern */
M2C_UNK func_80082E04(M2C_UNK);                     /* extern */
extern M2C_UNK D_800A97F8;
M2C_UNK func_800395E8(void *);                      /* extern */
M2C_UNK func_80065614(void *, M2C_UNK, M2C_UNK);    /* extern */
M2C_UNK func_800697C8(void *);                      /* extern */
M2C_UNK func_80076A8C();                            /* extern */


extern s8 D_800E2F58;
extern s32 D_800F0F5C;


void func_80075290(void) {
    func_800752C0();
    func_800758CC();
    func_80075F0C();
}

s32 func_800752C0(void) {
  s32 var_v1;
  void *var_v0;
  var_v1 = 1;
  var_v0 = &D_800F0F80;
  var_v0 = var_v0 + 0x94;
  do
  {
    ((struct 
    {
      char pad[(0x92 > 0) ? (0x92) : (0)];
      typeof(*((s8 *) 0)) f;
    } *) (((char *) var_v0) + ((0x92 < 0) ? (0x92) : (0))))->f = 0;
    var_v1 -= 1;
    var_v0 -= 0x94;
  }
  while (var_v1 >= 0);
  return (s32) var_v0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_800752E8);

void func_800753A4(void *arg0) {
    if (arg0 != NULL) {
        func_80030670(M2C_FIELD(arg0, s32 *, 4));
        func_80030670(M2C_FIELD(arg0, s32 *, 0x70));
        func_80030670(M2C_FIELD(arg0, s32 *, 8));
        M2C_FIELD(arg0, s8 *, 0x92) = 0;
    }
}

M2C_UNK *func_800753F0(s32 arg0) {
    s32 temp_s1;

    temp_s1 = arg0 & 0xFF;
    if (func_80075FE4(temp_s1) == 0) {
        sprintf(g_Args->exePath, &D_800A97D0, temp_s1);
        func_8006B230(0, g_Args->exePath);
    }
    return &D_800E2F5C;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_8007545C);

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_800755BC);

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_8007567C);

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_800757C0);

s32 func_800758A0(void *arg0, s32 arg1) {
    return M2C_FIELD(arg0, s32 *, 0x78) == arg1;
}

s32 func_800758B4(void *arg0) {
    return M2C_FIELD(arg0, s32 *, 0x80) < 2;
}

void func_800758C0(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0x78) = -1;
}

void func_800758CC(void) {
    func_8008D2EC(&D_800B0C10);
}

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_800758F0);

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_80075920);

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_80075D48);

void func_80075E9C(void *arg0) {
    s32 i;
    void *item;

    if (M2C_FIELD(arg0, s32 *, 0x6C) > 0) {
        for (i = 0; i < M2C_FIELD(arg0, s32 *, 0x6C); i++) {
            item = M2C_FIELD(arg0, void ***, 0x70)[i];
            M2C_FIELD(item, u16 *, 8) = 0xFFFF;
            M2C_FIELD(item, s16 *, 0xA) = 0;
            M2C_FIELD(item, s8 *, 0xD) = 0x10;
            M2C_FIELD(item, u16 *, 0x16) = M2C_FIELD(item, u16 *, 0x18);
            M2C_FIELD(item, u8 *, 0xC) = M2C_FIELD(item, u8 *, 0x1A);
        }
    }
}

void func_80075F0C(void) {
    D_800F0F5C = 0;
    D_800E2F58 = 0;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_80075F20);

s32 func_80075FA0(void) {
    s32 temp_v0;

    temp_v0 = D_800F0F5C == 1;
    if (temp_v0 != 0) {
        func_80082D00(1, 0);
    }
    return temp_v0;
}

s32 func_80075FE4(int arg0) {
  unsigned char new_var;
  u8 new_var2;
  s32 var_a1;
  new_var = 0xFF & arg0;
  var_a1 = 0;
  if (D_800F0F5C == 0)
  {
 do { new_var2 = (u8) D_800E2F58; var_a1 = new_var2 == new_var; } while (0);
  }
  return var_a1;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_80076014);

void func_800760B4(s32 arg0) {
    if (arg0 == 2) {
        D_800F0F5C = 0;
        func_80082E04(0);
        return;
    }
    func_8006B1DC(&D_800A97F8);
    D_800F0F5C = 2;
}

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_80076104);

void func_80076184(s32 idx, u32 val) {
    void *t;

    if (val < 2 && idx > 0) {
        t = M2C_FIELD(D_8009CD98, void **, 0x114);
        if (*(s32 *)(M2C_FIELD(t, u8 **, 4) + idx * 8) != 0) {
            *(M2C_FIELD(t, u8 **, 8) + idx) = val;
        }
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_800761DC);

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_8007632C);

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_80076A0C);

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_80076A8C);

void func_80076B9C(void *arg0) {
    func_80076A8C();
    if (M2C_FIELD(M2C_FIELD(arg0, void **, 0x90), s32 *, 0x2C) >= 0x2AF9) {
        func_80065614(arg0, 1, 5);
        func_800697C8(arg0);
        func_800395E8(arg0);
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_80076BF8);

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_80076CDC);

INCLUDE_ASM("asm/level01/nonmatchings/text_80075290", func_80076F24);
