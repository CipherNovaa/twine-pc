#include "twine.h"

typedef struct { s32 m[8]; } GsMat32;
typedef struct { u8 pad0[0x4C]; void *super; } GsCOORDUNIT;
typedef struct { GsCOORDUNIT *coord; u32 *primtop; } GsUNIT;
typedef struct { u32 type; void **ptr; } GsTYPEUNIT;
typedef struct { u32 *hmd; GsUNIT *units; u8 *flags; u32 nunits; GsCOORDUNIT *coords; u32 flg; GsMat32 coord; u8 pad38[0x28]; s32 sub; u8 pad64[8]; s32 nframes; void *seq; u8 pad74[4]; s32 unk78; s32 unk7C; s32 unk80; s32 unk84; s32 unk88; s32 unk8C; } HmdModel;
typedef struct { u32 length; u32 *org; u32 offset; u32 point; u32 *tag; } GsOT;
extern GsMat32 D_800FC6A0;
extern GsOT D_800B8F88[];
void *func_80030610(s32);
void func_80075D48(HmdModel *);
void GsMapUnit(u32 *);
s32 GsScanUnit(u32 *, GsTYPEUNIT *, GsOT *, u32 *);
GsCOORDUNIT *GsMapCoordUnit(u32 *, void **);
s32 GsLinkAnim(void *, void **);
s32 GsScanAnim(void **, s32);
void *func_8008E03C(u32);
void *func_8008DAAC(u32);
void *func_8008CCB8(u32);
void *func_8008DB68(u32);
void func_8008DBDC(void);
void func_8008E378(void);


void func_80076104(s32 idx);
void func_800760B4(s32 arg0);
void func_8002C490(char *, s8 *, void (*)(s32));


extern char *D_8009DB50[];
s32 func_8002C168(char *name, s32 arg1);


extern s32 D_1F8003E8[];
extern s32 D_1F8003D4;
extern s32 D_1F8003D8;
extern s32 D_1F8003DC;
extern s32 D_800B0018[];
extern s32 D_800B0C64;
extern u16 D_800B0C5C;
extern s32 D_8009CD4C;
extern s16 D_8009CF48;
extern char D_800A98B4[];
s32 func_80076BF8(s32 arg0, s32 arg1, s32 arg2);
void func_8003B53C(u8 *, s32, s32, s32);


unsigned long func_80027248(M2C_UNK, s32, M2C_UNK);


typedef struct { u16 start; u8 len; u8 flag; } AnimRec;
typedef struct { u8 pad0[6]; u16 count; u16 unk8; u16 tag; u8 unkC; u8 padD[3]; s16 unk10; u8 pad12[4]; u16 unk16; AnimRec recs[1]; } AnimTrack;


extern M2C_UNK D_800F11D0;
extern s32 D_800AFFC8;
void GsSetRefView2(void *);
void func_8007567C(void *arg0);


extern s32 D_800B8F24;
extern s32 D_8009CA9C;
extern s32 D_8009CAA0;
extern s32 D_800B0808[];
extern u8 D_800D2AFC[][0x14];
void GsGetLsUnit(void *, void *);
void GsGetLwUnit(void *, void *);
void GsSortUnit(void *, void *, void *);
void func_80077764();
void func_8007A554(void *);
void func_8007A580(void *);


extern s16 D_800B0DD4;
extern s32 D_800A988C;
extern s32 D_8009CDA8;
extern s16 D_800D9208;
extern s8 D_800D920C;
extern u8 D_1F8003E0[];
s32 func_80039A9C(M2C_UNK, u8, M2C_UNK);


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

void func_8007545C(s32 first, s32 last) {
    long h;
    u8 *p;
    u8 *end;
    gte_ReadGeomScreen(&h);
    func_8008D2EC(&D_800B0C10);
    p = (u8 *)D_800AFFC8 + first * 0x124;
    end = (u8 *)D_800AFFC8 + last * 0x124;
    for (; p <= end; p += 0x124) {
        if (p[4] != 0 && M2C_FIELD(p, s32 *, 0x7C) >= 0 && M2C_FIELD(p, s32 *, 0x114) != 0) {
            if (p == D_8009CD98) {
                func_8008D2EC(&D_800F11D0);
                gte_SetGeomScreen(M2C_FIELD(&g_Weapons[D_800B0DD4], u16 *, 0xAA));
            } else {
                gte_SetGeomScreen((u16)h);
            }
            func_8007567C(p);
        }
    }
    GsSetRefView2(&D_800B0C10);
    gte_SetGeomScreen((u16)h);
}

void func_800755BC(u8 *arg0) {
    s32 ok;

    ok = 0;
    if (*(s32 *)(arg0 + 0x6C) != 0 && *(s32 *)(arg0 + 0x84) == 0 && *(s32 *)(arg0 + 0x88) >= 0 && *(s32 *)(arg0 + 0x80) > 0) {
        ok = D_8009CAA0 == 0;
    }
    if (ok) {
        *(s32 *)(arg0 + 0x78) = *(s32 *)(arg0 + 0x88);
        *(s32 *)(arg0 + 0x7C) = *(s32 *)(arg0 + 0x8C);
        if (*(s32 *)(arg0 + 0x80) > 0) {
            if (--*(s32 *)(arg0 + 0x80) == 0) {
                if (*(u16 *)(**(u8 ***)(arg0 + 0x70) + 0xA) == 0xFFFF) {
                    *(s32 *)(arg0 + 0x80) = (*(u8 **)(arg0 + 0x74))[*(s32 *)(arg0 + 0x78)];
                }
            }
        }
    } else {
        *(s32 *)(arg0 + 0x80) = 0;
    }
}

void func_8007567C(void *arg0) {
    u32 i;
    u8 *flags;
    void *model;
    s32 *unit;
    s32 idx;

    if (arg0 != NULL) {
        model = M2C_FIELD(arg0, void **, 0x114);
        if (model != NULL) {
            i = 0;
            func_80077764();
            unit = M2C_FIELD(model, s32 **, 4);
            flags = M2C_FIELD(model, u8 **, 8);
            idx = D_800B8F24;
            for (; i < M2C_FIELD(model, u32 *, 0xC); i++, unit += 2, flags++) {
                if (unit[1] == 0) {
                    continue;
                }
                if (i == 0 && (D_8009CA9C != 0 || D_8009CAA0 != 0)) {
                    continue;
                }
                if (unit[0] != 0) {
                    if (*flags == 1) {
                        continue;
                    }
                    GsGetLwUnit((void *)unit[0], D_800B0808);
                    func_8007A580(D_800B0808);
                    GsGetLsUnit((void *)unit[0], D_800B0808);
                    func_8007A554(D_800B0808);
                }
                GsSortUnit(unit, D_800D2AFC[idx], (void *)0x1F800000);
            }
        }
    }
}

void func_800757C0(u8 *obj, s32 anim, s32 arg2) {
    s32 i;
    s32 tag;
    s32 v;
    AnimTrack *e;
    AnimRec *r;

    if (obj == NULL || anim < 0) {
        return;
    }
    if (*(u16 *)(obj + 0x90) - 1 < anim) {
        return;
    }
    tag = 0;
    v = (*(u8 **)(obj + 0x74))[anim];
    *(s32 *)(obj + 0x80) = v;
    if (*(s32 *)(obj + 0x84) == 0) {
        tag = 0xFFFF;
        if (arg2 != 0) {
            tag = v;
        }
    }
    for (i = 0; i < *(s32 *)(obj + 0x6C); i++) {
        e = (*(AnimTrack ***)(obj + 0x70))[i];
        e->tag = tag;
        if (anim < e->count) {
            r = &e->recs[anim];
            e->unk16 = r->start;
            e->unkC = r->len;
            e->unk10 = 0;
            e->unk8 = 0xFFFF;
            e->recs[0].flag = 0;
        }
    }
    *(s32 *)(obj + 0x88) = anim;
    *(s32 *)(obj + 0x8C) = 0;
}

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

void func_80075920(HmdModel *arg0) {
    GsTYPEUNIT ut;
    u32 *p;
    u32 n;
    u32 i;
    s32 j;
    s32 cnt;
    s32 len;
    GsOT *ot;

    ot = D_800B8F88;
    p = arg0->hmd;
    if (p == NULL) {
        return;
    }
    arg0->nframes = 0;
    arg0->coords = NULL;
    arg0->sub = 0;
    arg0->coord = D_800FC6A0;
    arg0->unk84 = 0;
    arg0->unk78 = -1;
    arg0->unk7C = 0;
    arg0->unk80 = 0;
    arg0->unk88 = -1;
    arg0->unk8C = -1;
    n = p[3];
    arg0->units = func_80030610(n * 8);
    arg0->seq = func_80030610(n * 0x1C);
    arg0->flags = func_80030610(n);
    if (p[1] != 0) {
        func_80075D48(arg0);
        return;
    }
    GsMapUnit(p);
    p += 3;
    arg0->nunits = *p++;
    for (i = 0; i < arg0->nunits; i++) {
        arg0->flags[i] = 0;
        arg0->units[i].primtop = (u32 *)*p;
        arg0->units[i].coord = NULL;
        p++;
        if (arg0->units[i].primtop != NULL) {
            GsScanUnit(arg0->units[i].primtop, NULL, NULL, NULL);
            while (GsScanUnit(NULL, &ut, ot, (u32 *)0x1F800000)) {
                if (((ut.type >> 24) < 2 || (ut.type >> 24) == 6) && (ut.type & 0x800000)) {
                    arg0->coords = GsMapCoordUnit(arg0->hmd, ut.ptr);
                    ut.type &= ~0x800000;
                    cnt = ((s32 *)arg0->coords)[-1];
                    for (j = 0; j < cnt; j++) {
                        if (arg0->coords[j].super == NULL) {
                            arg0->coords[j].super = &arg0->flg;
                        }
                    }
                }
                *ut.ptr = NULL;
                switch (ut.type >> 24) {
                case 0:
                    *ut.ptr = func_8008E03C(ut.type);
                    break;
                case 1:
                    *ut.ptr = func_8008DAAC(ut.type);
                    break;
                case 2:
                    *ut.ptr = func_8008CCB8(ut.type);
                    ((void (*)(u32 *))*ut.ptr)((u32 *)0x1F800000);
                    *ut.ptr = func_8008E378;
                    break;
                case 3:
                    *ut.ptr = func_8008DB68(ut.type);
                    if (*ut.ptr != NULL && *ut.ptr != func_8008E378) {
                        len = GsLinkAnim(arg0->seq, ut.ptr);
                        if (arg0->nframes <= 0) {
                            arg0->nframes = len;
                        }
                        if (len != arg0->nframes && arg0->nframes < len) {
                            arg0->nframes = len;
                        }
                        if (GsScanAnim(ut.ptr, 0)) {
                            func_8008DBDC();
                        }
                    }
                    break;
                }
                if (*ut.ptr == NULL) {
                    *ut.ptr = func_8008E378;
                }
            }
        }
    }
    if (arg0->coords != NULL) {
        for (i = 1; i < arg0->nunits - 1; i++) {
            arg0->units[i].coord = &arg0->coords[i - 1];
        }
    }
}

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

void func_80076014(s32 idx) {
    if (g_Args->cdArchive == 0) {
        func_80076104(idx);
        return;
    }
    D_800F0F5C = 1;
    D_800E2F58 = M2C_FIELD(&g_Weapons[idx], u8 *, 0x5B);
    func_8002C490(D_8009DB50[M2C_FIELD(&g_Weapons[idx], u8 *, 0x5B)], &D_800E2F58 + 4, func_800760B4);
}

void func_800760B4(s32 arg0) {
    if (arg0 == 2) {
        D_800F0F5C = 0;
        func_80082E04(0);
        return;
    }
    func_8006B1DC(&D_800A97F8);
    D_800F0F5C = 2;
}

void func_80076104(s32 idx) {
    func_8002C168(D_8009DB50[M2C_FIELD(&g_Weapons[idx], u8 *, 0x5B)], (s32)(&D_800E2F58 + 4));
    D_800F0F5C = 0;
    D_800E2F58 = M2C_FIELD(&g_Weapons[idx], u8 *, 0x5B);
}

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

s32 func_80076BF8(s32 arg0, s32 arg1, s32 arg2) {
    Weapon *w;
    s32 obj;

    w = &g_Weapons[D_800B0DD4];
    if (w->ammo > 0) {
        obj = func_80039A9C(arg0, arg2, 1);
        if (obj == 0) {
            func_8006B1DC(&D_800A988C);
            return 0;
        }
        M2C_FIELD(obj, u8 *, 7) = 0x2C;
        M2C_FIELD(obj, u8 *, 0xA5) = 0x18;
        M2C_FIELD(obj, s16 *, 0x6A) = 0x818;
        M2C_FIELD(obj, s32 *, 0x34) = 0x4E20;
        M2C_FIELD(obj, s16 *, 0x28) = 0x7D0;
        D_800D920C = 2;
        w->clip--;
        M2C_FIELD(obj, s32 *, 0x2C) = D_8009CDA8;
        M2C_FIELD(obj, u16 *, 0xAC) = *(u16 *)(D_1F8003E0 - 8);
        D_800D9208 = M2C_FIELD(w, u32 *, 0x5C) >> 26;
        return obj;
    }
    return 0;
}

void func_80076CDC(u8 *obj) {
    u8 *player;
    Weapon *w;
    u8 *proj;
    s32 *t;
    s32 n;
    if (obj == NULL) {
        return;
    }
    t = (s32 *)(M2C_FIELD(obj, u8 **, 0x114) + 0x38);
    D_1F8003D4 = ((struct { s16 m[3][3]; s32 t[3]; } *)t)->t[0];
    D_1F8003D8 = ((struct { s16 m[3][3]; s32 t[3]; } *)t)->t[1];
    D_1F8003DC = ((struct { s16 m[3][3]; s32 t[3]; } *)t)->t[2];
    *(SVECTOR *)D_1F8003E0 = M2C_FIELD(obj, SVECTOR *, 0x3C);
    player = g_Player;
    w = &g_Weapons[D_800B0DD4];
    gte_SetRotMatrix(D_800B0018);
    gte_ldv0(D_1F8003E0);
    gte_rtv0();
    gte_stlvnl(D_1F8003E8);
    D_1F8003D4 += D_1F8003E8[0];
    D_1F8003D8 += D_1F8003E8[1];
    D_1F8003DC += D_1F8003E8[2];
    proj = (u8 *)func_80076BF8((s32)player, (s32)obj, obj[0x3A]);
    if (proj == NULL) {
        func_8006B1DC((M2C_UNK *)D_800A98B4);
        return;
    }
    M2C_FIELD(obj, s16 *, 0x38) = 500;
    if (D_8009CD4C != 0) {
        func_8003B53C(proj, *(s32 *)&D_800B0C5C, D_800B0C64 + 0x5B, 500);
    } else {
        func_8003B53C(proj, M2C_FIELD(M2C_FIELD(player, u8 **, 0x90), s16 *, 0x62), M2C_FIELD(M2C_FIELD(player, u8 **, 0x90), s16 *, 0x60), 500);
    }
    M2C_FIELD(proj, s16 *, 0x56) = -1;
    M2C_FIELD(M2C_FIELD(proj, u8 **, 0x90), s16 *, 0x60) = 0x400;
    if (D_8009CD4C != 0) {
        M2C_FIELD(M2C_FIELD(proj, u8 **, 0x90), s16 *, 0x62) = D_800B0C5C;
    } else {
        M2C_FIELD(M2C_FIELD(proj, u8 **, 0x90), s16 *, 0x62) = M2C_FIELD(M2C_FIELD(player, u8 **, 0x90), s16 *, 0x62);
    }
    M2C_FIELD(M2C_FIELD(proj, u8 **, 0x90), s32 *, 0x10) = 2;
    D_8009CF48 = 0;
    n = ((u32)w->flags >> 15) & 0xF;
    if (n > 0) {
        func_80076184(n, 0);
    }
}

void func_80076F24(u8 *obj, s32 idx) {
    SVECTOR rot;
    SVECTOR pos;
    u8 *base;
    s16 *r;

    base = *(u8 **)(obj + 0x10C);
    if (base == NULL) {
        return;
    }
    r = (s16 *)(base + idx * 0x14);
    base[0x64] = 0;
    *(s16 *)(obj + 0x98) = 0;
    *(s16 *)(obj + 0x9A) = 0;
    *(s16 *)(obj + 0x9C) = 0;
    pos.vx = r[0];
    pos.vy = r[1];
    pos.vz = r[2];
    rot.vx = r[3];
    rot.vy = r[4];
    rot.vz = r[5];
    *(s16 *)(obj + 0x9E) = 0;
    *(s16 *)(obj + 0xA0) = 0;
    *(s16 *)(obj + 0xA2) = 0;
    func_80065614(obj, 1, 5);
    func_80027248(0x43, *(s32 *)(obj + 0x90) + 0x14, 0);
    func_800395E8(obj);
}
