#include "twine.h"

typedef struct { char *name; s32 unk4; s32 unk8; } CdFileEnt;
typedef struct { u8 p[0x142]; u16 v; } ArgsX;
typedef struct { u8 pad[0x340]; s32 f[11]; } ArgsFiles;
typedef struct { u8 p[0x1A]; u16 v; } Rec;
extern s8 D_800A7060;
extern s8 D_800A70D4;
extern char D_800AF624[];
extern char D_800A70F8[];
s32 func_8002AC90(u8 *a, u8 *b);
extern char D_800A72E8[];
extern u8 D_800AF6A8[];
extern u8 *D_800AEE10;
s32 func_80082D00(s32, s32);
void func_8002ADF0(void);
extern char D_800A7048[];
extern char D_800A7190[];
extern s16 D_8009CA56;
extern s32 D_800AEDE0;
extern s32 D_800AEDE4;
extern CdFileEnt D_8009C204[];


s16 func_8002AAFC(s16 *arg0, s16 *arg1, u8 *arg2);
M2C_UNK func_80082E04(M2C_UNK);
M2C_UNK func_80082B64(s32, s8 *, s32);
s32 VSync(s32);
extern char D_800A7260[];
extern char D_800A7290[];


char *strcpy(char *dst, char *src);
char *strcat(char *dst, char *src);
int strlen(char *s);
extern char D_800A71EC[];
extern char D_800A71F0[];
extern char D_800A71F4[];
extern char D_800A71F8[];
extern char D_800AFEC8[];


s32 func_8002A854(u8 *name, s32 arg1);
s32 func_8002BE24(char *arg0, s32 arg1);
s32 func_8002BF34(void *arg0, char *arg1);
M2C_UNK func_8006B1DC(M2C_UNK *);
u8 func_8002AAC4(void);
extern char D_800A7214[];
extern u8 D_800AFF2C[];
s32 func_8002C168(char *name, s32 arg1);


extern s32 D_8009CA74;
void func_8002B0A0(void);


s32 CdSearchFile(void *, char *);
s32 func_800834D0(void *);


void func_8002BB68(u8 arg0);
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
void func_8002A720(s32 arg0);
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

void func_8002A720(s32 arg0) {
    s32 r;
    s32 tries;
    u8 b[3];
    u8 buf[24];
    tries = 4;
    func_800833CC(arg0, buf);
loop1:
    func_80082F8C(2, buf, 0);
    func_80082B64(1, D_800AF6A8, 0x80);
loop2:
    r = func_80082D00(1, 0);
    if (r == 0) goto done;
    if (r == -1) {
        if (--tries != -1) {
            b[2] = 0;
            b[1] = 0;
            b[0] = 0;
            func_800831FC(0x16, b, 0);
            goto loop1;
        } else {
            func_8002ADF0();
        }
    }
    goto loop2;
done:
    D_800AEE10 = D_800AF6A8;
    func_8002AA94(1);
}

s32 func_8002A854(u8 *name, s32 arg1) {
    s16 sp10;
    s16 sp12;
    s8 buf[8];
    u8 mode;
    s32 start;
    s32 r;
    s32 tries;
    s8 b[3];
    s32 len;
    tries = 4;
    len = strlen((char *)name);
    if (len >= 0x1B) {
        printf(&D_800A7060, name, 0x1A);
        printf(&D_800A70A0, &D_800A70C8, 0x1C2);
        exit(1);
    }
    mode = 0x80;
    D_800AF620 = 0x80;
    func_80082F8C(0xE, &D_800AF620, 0);
    start = g_Args->unk2DC;
    if (func_8002AAFC(&sp10, &sp12, name) == 0) {
        printf(&D_800A70D4, name);
        exit(1);
    }
    func_800833CC(sp10 + D_800AEDD8, buf);
loop1:
    func_80082F8C(2, buf, 0);
    func_80082B64(sp12, arg1, mode);
loop2:
    r = func_80082D00(1, 0);
    if (r == 0) goto done;
    if (r == -1) {
        if (--tries != -1) {
            b[2] = 0;
            b[1] = 0;
            b[0] = 0;
            func_800831FC(0x16, b, 0);
            goto loop1;
        } else {
            func_8002ADF0();
        }
    }
    goto loop2;
done:
    return sp12 * 2048;
}

void func_8002AA94(int arg0) {
  D_8009C200 = arg0;
}

u8 func_8002AAC4(void) {
    return D_8009C200;
}

s16 func_8002AAFC(s16 *arg0, s16 *arg1, u8 *arg2) {
    s32 i = 0;
    for (;;) {
        if (func_8002AC90(arg2, (u8 *)&((Rec *)D_800AEE10)[i]) == 0) {
            break;
        }
        if (++i >= 0x48) {
            sprintf(D_800AF624, D_800A70F8, i, 0x48);
            func_8006B1DC(D_800AF624);
            return 0;
        }
    }
    *arg0 = ((Rec *)D_800AEE10)[i].v;
    *arg1 = ((Rec *)D_800AEE10)[i + 1].v - ((Rec *)D_800AEE10)[i].v;
    return 1;
}

s32 func_8002AC90(u8 *a, u8 *b) {
    a++;
    for (;;) {
        if (*a == ';') {
            return 0;
        } else if (*a == 0) {
            return 0;
        } else if (*a == *b) {
            a++;
            b++;
        } else if (*a == '\\' && *b == '/') {
            a++;
            b++;
        } else {
            return *a - *b;
        }
    }
}

void func_8002ADF0(void) {
    printf(&D_800A7120);
    exit(0);
}

void func_8002AE34(u8 arg0) {
    u8 sp18[8];
    if (D_8009CA56 != -1) {
        ((ArgsX *)g_Args)->v = D_8009CA56;
        *(u16 *)&D_8009CA5C = D_8009CA56;
        D_8009CA56 = -1;
    }
    func_800833CC(D_800AEDE4, (s8 *)sp18);
    func_80082F8C(arg0, sp18, 0);
    D_800AEDDC = D_800AEDE4;
    D_800AEDE0 = D_800AEDE0 + 1;
}

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

void func_8002AFF8(s32 idx) {
    s32 file[6];

    if (CdSearchFile(file, D_8009C204[idx].name) == 0) {
        return;
    }
}

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

void func_8002BB68(u8 arg0) {
    if (*(s16 *)&g_CutScene == 2 || *(s16 *)&g_CutScene == 3) {
        func_80026B00(g_Args->pad143[1]);
    } else {
        func_80026B00((u8)(g_Args->pad12E[0xC] << 1));
    }
}

INCLUDE_ASM("asm/level01/nonmatchings/text_8002A61C", func_8002BC18);

void func_8002BDE4(void) {
    D_800AFEBC = -1;
    D_800AFEB6 = 0;
    D_800AFEB5 = 0x7F;
}

s32 func_8002BE24(char *arg0, s32 arg1) {
    s32 fd;
    s32 size;
    s32 got;
    fd = PCopen(arg0, 0, 0);
    if (fd == -1) {
        printf(D_800A7048, arg0);
        exit(1);
    }
    size = PClseek(fd, 0, 2);
    PClseek(fd, 0, 0);
    got = PCread(fd, arg1, size);
    if (got != size) {
        printf(D_800A7190, arg0);
        exit(1);
    }
    func_80083F14(fd);
    return size;
}

s32 func_8002BF34(void *arg0, char *arg1) {
    u8 *s;
    u8 *d;

    s = arg1;
    if (arg1 == NULL) {
        printf(&D_800A70A0, &D_800A70C8, 0x59E);
        exit(1);
    }
    strcpy(arg0, D_800A71EC);
    if (*s != '\\') {
        strcat(arg0, D_800A71F0);
    }
    d = (u8 *)arg0 + strlen(arg0);
    while (*s != 0) {
        if (*s >= 0x61) {
            *d = *s - 0x20;
        } else {
            *d = *s;
        }
        if (*s == '/') {
            *d = '\\';
        }
        d++;
        s++;
    }
    *d = 0;
    strcat(arg0, D_800A71F4);
    if (strlen(arg0) >= 0x1B) {
        sprintf(D_800AFEC8, D_800A71F8, arg0);
        func_8006B1DC((M2C_UNK *)D_800AFEC8);
        printf(&D_800A70A0, &D_800A70C8, 0x5CA);
        exit(1);
    }
    return (s32)arg0;
}

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

s32 func_8002C490(char *name, s8 *buf, void (*cb)(s32)) {
    u8 mode;
    s16 sector;
    s16 count;
    s8 loc[4];

    if (((s32 (*)(void))func_8002AAC4)() == 0 && g_Args->cdArchive) {
        func_8006B1DC((M2C_UNK *)D_800A7260);
        printf(&D_800A70A0, &D_800A70C8, 0x650);
        exit(1);
    } else {
    }
    func_8002BF34(D_800AFF2C, name);
    if (func_8002AAFC(&sector, &count, D_800AFF2C) == 0) {
        sprintf(g_Args->exePath, D_800A7290, D_800AFF2C);
        func_8006B1DC((M2C_UNK *)g_Args->exePath);
        return 0;
    }
    func_800833CC(sector + D_800AEDD8, loc);
    func_80082F8C(2, (u8 *)loc, 0);
    mode = 0x80;
    func_800831FC(0xE, (s8 *)&mode, NULL);
    VSync(3);
    func_80082E04((M2C_UNK)cb);
    func_80082B64(count, buf, 0x80);
    return 1;
}

void func_8002C664(void) {
    func_80082F78(0);
    D_800AF620 = 0x80;
    func_80082F8C(0xE, &D_800AF620, 0);
}

void func_8002C6BC(s32 arg0) {
    if (arg0 != 0x63) {
        if (arg0 >= 0 && arg0 < 0xB) {
        } else {
            printf(&D_800A70A0, &D_800A70C8, 0x6DF);
            exit(1);
        }
    }
    if (((ArgsFiles *)g_Args)->f[arg0] != -1) {
        return;
    }
    sprintf(g_Args->exePath, D_800A72E8, g_Args->level, arg0 + 1);
    func_8002A61C(g_Args->exePath, &((ArgsFiles *)g_Args)->f[arg0]);
}
