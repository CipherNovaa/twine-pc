/* PARKED (step 6a, not built: not in port/Makefile PORT_MODS; add PEOPLE there to build it). Unproven: run with
 * TWINE_PEOPLE_VERIFY=1 across all levels before enabling. */
/* People (native PC build only): the people's slots, more than the game's six when a mod asks (people_set_slots).
 *
 * A person on the map (class 6) is drawn from a GUY slot: a model instance (D_800DDFEC), bound to its texture chunk
 * and to an object type by the section's INI record (D_800D2E84, 0x20 bytes: +1 the chunk, +0x1C the type), taken
 * (D_800DE0C8: 0 free, 1 drawn, 2 held but not drawn) by its owner (D_800DDEF0), with the world matrix it is drawn
 * from (D_800DE004, 0x20 bytes; +0x14 the position). The person's animation record (pool D_800B05B0, 8 of 0x38
 * bytes; +0xA the slot) and AI context (pool D_800D9278, 7 of 0x248 bytes) come from pools beside them. All of it is
 * six (or 8, 7) fixed entries of the level's memory, and about a dozen functions walk them (the same code in every
 * level: workbench/research/SLOT_FUNCS.txt).
 *
 * Here those functions are the game's own made native, each loop going to people_slots() (6 unless a mod raised it),
 * slot i below 6 the game's own memory, 6 and up this mod's (ext below); the pools grow the same way past their
 * own entries. With 6 slots they do exactly what the game's do (checked by identical runs, screenshots and logs).
 * The render list (D_800DDF08 the instances drawn, D_800DDCCC their matrices, D_800DDDEC their owners, D_800DDF24
 * the view matrices) stays the game's while it holds six at most.
 *
 * MOD_REPLACES_NATIVE: func_8005B508 func_8005B584 func_8005B5AC func_8003C88C func_8005AEF8 func_8005B96C func_8005BA30 func_8005BAE8 func_800393E4 func_80033090 func_8005D5C4 func_800540CC func_80054118
 *
 * Names are LEVEL01's; port/mods.py maps them onto each level. */
#include "twine.h"

/* the game's slots */
extern u8 *D_800DDFEC[];    /* each slot's model instance */
extern u8 D_800DE004[];     /* each slot's world matrix (0x20 bytes) */
extern u8 D_800DE0C8[];     /* each slot taken: 0 free, 1 drawn, 2 not drawn */
extern u8 *D_800DDEF0[];    /* each slot's owner */
extern u8 D_800D2E84[];     /* each slot's INI record (0x20 bytes: +1 texture chunk, +0x1C object type) */
/* the render list */
extern u8 *D_800DDF08[];    /* the instances drawn */
extern u8 D_800DDCCC[];     /* their world matrices */
extern u8 *D_800DDDEC[];    /* their owners */
extern u8 D_800DDF24[];     /* their view matrices (0x20 bytes) */
extern u8 D_8009D8E8;       /* how many */
extern s32 D_8009D8E4;      /* slots hidden (bit per slot; -1 none drawn) */
/* the pools */
extern u8 D_800B05B0[];     /* the animation records (8 of 0x38 bytes) */
extern u8 D_800D9278[];     /* the AI contexts (7 of 0x248 bytes; +0x1CC flags, bit 1 taken) */
/* the render step's own */
extern s32 D_8009D924, D_8009D928;
extern u8 D_800B0018[];
extern s32 *D_8009D91C;
extern s16 *D_8009D918;
extern u16 *D_8009D920;
extern s32 D_800DDE10[];
extern u8 D_800B8F88[];
extern s32 D_800B8F24;
extern s16 D_8009D8F0;
extern s32 D_800DDDA8;
extern u8 D_800B0848[];
extern u8 **D_800DDEE4;
extern u8 *D_800DDEE0;
/* the people objects, the reinforcements */
extern u8 *D_800AFFC8;
extern s16 D_800B0DA0;
extern s16 D_800B0868;
extern s32 D_8009CF74, D_8009CF10, D_8009CF2C;
extern s32 D_8009CF14[];

u32 func_8007CE30(u32 inst, u32 chunk);
u32 func_8007D260(u32 inst);
u32 func_8005D630(u32 rec);
u32 TransposeMatrix(u32 a, u32 b);
u32 ApplyMatrix(u32 m, u32 v, u32 out);
u32 func_80033F38(void);
u32 func_800AACFC(u32 insts, u32 views, u32 n, u32 ot);
u32 func_8004A398(void);
u32 GsGetLs(u32 coord, u32 m);
u32 func_8005B7BC(u32 obj, u32 inst);
u32 func_800753A4(u32 p);
void func_80081458(s32 voice);
u32 func_80039520(u32 obj);
u32 func_80032EFC(u32 rec, u32 type);
void func_8005C33C(void);
u32 GTE_mfc2(int reg);
void GTE_mtc2(int reg, u32 value);
void GTE_ctc2(int reg, u32 value);
void GTE_cmd(u32 op);
void port_smooth_keep(void *at, u32 size);
s32 func_8005B584(s32 a0, u16 v, s16 i);
void func_8005BA30(u8 *obj);
void *memset(void *d, int c, unsigned n);

#define GAME_SLOTS 6
#define MAX_SLOTS 24
#define GAME_ANIMS 8
#define GAME_AIS 7
#define EXT (MAX_SLOTS - GAME_SLOTS)
#define ANIM_SIZE 0x38
#define AI_SIZE 0x248

static struct {
    u8 *inst[EXT];
    u8 busy[EXT];
    u8 *owner[EXT];
    u8 mat[EXT][0x20];
    u8 rec[EXT][0x20];
    u8 anim[EXT][ANIM_SIZE];
    u8 ai[EXT][AI_SIZE];
    u8 *list_inst[MAX_SLOTS + 1];
    u8 list_mat[MAX_SLOTS][0x20];
    u8 *list_owner[MAX_SLOTS];
    u8 list_view[MAX_SLOTS][0x20];
} ext;
static int slots = GAME_SLOTS; /* the slots in use (the pools: as many more past their own) */
static int kept;
static int dry, would_spawn; /* (TWINE_PEOPLE_VERIFY: func_80033090 run to see whether it would send a guard) */

/* ---- for other mods ---- */
int people_slots(void) {
    return slots;
}

/* n slots from now on (6..MAX_SLOTS); the extra ones start free */
void people_set_slots(int n) {
    if (n < GAME_SLOTS)
        n = GAME_SLOTS;
    if (n > MAX_SLOTS)
        n = MAX_SLOTS;
    slots = n;
    if (!kept++)
        port_smooth_keep(&ext, sizeof(ext)); /* (an extra drawing puts it back as it does RAM) */
}

u8 **people_inst(int i) {
    return i < GAME_SLOTS ? &D_800DDFEC[i] : &ext.inst[i - GAME_SLOTS];
}
u8 *people_busy(int i) {
    return i < GAME_SLOTS ? &D_800DE0C8[i] : &ext.busy[i - GAME_SLOTS];
}
u8 **people_owner(int i) {
    return i < GAME_SLOTS ? &D_800DDEF0[i] : &ext.owner[i - GAME_SLOTS];
}
u8 *people_matrix(int i) {
    return i < GAME_SLOTS ? D_800DE004 + i * 0x20 : ext.mat[i - GAME_SLOTS];
}
u8 *people_record(int i) {
    return i < GAME_SLOTS ? D_800D2E84 + i * 0x20 : ext.rec[i - GAME_SLOTS];
}

#define INST(i) (*people_inst(i))
#define BUSY(i) (*people_busy(i))
#define OWNER(i) (*people_owner(i))
#define MAT(i) people_matrix(i)
#define REC(i) people_record(i)
#define W(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define HU(p, o) (*(u16 *)((u8 *)(p) + (o)))

/* ---- the slots ---- */

/* each slot's instance relocated and bound to its texture chunk; all free (after a section's GUY file loads) */
static void n_8005B508(void) {
    s32 i;
    for (i = 0; i < slots; i++) {
        func_8007CE30((u32)INST(i), REC(i)[1]);
        BUSY(i) = 0;
    }
}

/* slot i is bound to type v */
static s32 n_8005B584(s32 a0, u16 v, s16 i) {
    return HU(REC(i), 0x1C) == v;
}

/* a free slot of obj's type taken for it: its instance (p walks the game's instances from the first) */
static s32 n_8005B5AC(u8 *obj, s32 *p) {
    u16 i;
    u8 *sub;
    for (i = 0; i < slots; i++, p++) {
        if (BUSY(i) == 0 && func_8005B584(obj == g_Player, obj[6], i) != 0) {
            func_8007D260((u32)INST(i));
            sub = *(u8 **)(obj + 0x94);
            sub[0xA] = i;
            W(sub, 0x24) &= ~0x20000;
            BUSY(i) = 1;
            OWNER(i) = obj;
            return i < GAME_SLOTS ? *p : (s32)INST(i);
        }
    }
    return 0;
}

/* slot bound to an object type */
static s32 n_8003C88C(s32 slot, s32 type) {
    H(REC(slot), 0x1C) = (s16)type;
    return (s32)D_800D2E84;
}

/* the person's world matrix kept for drawing */
static void n_8005BA30(u8 *obj) {
    u8 *c = *(u8 **)(obj + 0x90);
    s32 *dst = (s32 *)MAT((*(u8 **)(obj + 0x94))[0xA]);
    s32 k;
    W(c, 0x10) = 0;
    GsGetLs((u32)(c + 0x10), (u32)D_800B0848);
    W(c, 0x10) = 2;
    for (k = 0; k < 8; k++)
        dst[k] = W(c, 0x34 + k * 4);
}

/* the person's animation frames into its instance, its position into its slot's matrix */
static void n_8005B96C(u8 *obj, s32 pos, u8 *anim) {
    s32 idx = anim[0xA];
    s32 *p = (s32 *)(*(u8 **)(obj + 0xC0) + 0x24);
    s32 *q;
    p[0] = (s32)D_800DDEE4[H(anim, 0x10)];
    p[1] = (s32)D_800DDEE4[H(anim, 0x12)];
    p[2] = H(anim, 0x14);
    p[3] = H(anim, 0x16);
    p[8] = H(anim, 0x18);
    q = (s32 *)(MAT(idx) + 0x14);
    q[0] = W(pos, 0x28);
    q[1] = W(pos, 0x2C);
    q[2] = W(pos, 0x30);
    func_8005BA30(obj);
    func_8005B7BC((u32)obj, W(obj, 0xC0));
}

/* the animation's root motion; the slot drawn or not */
static void n_8005BAE8(u8 *obj, u8 *anim) {
    s16 *p;
    s16 x1, y1, x2, y2, z2;
    s32 dx, dz;
    s16 f;
    s16 z1;

    p = ((s16 **)D_800DDEE0)[H(anim, 0x10)];
    p += anim[0x14] * 3;
    x1 = p[0];
    y1 = p[1];
    z1 = p[2];
    p = ((s16 **)D_800DDEE0)[H(anim, 0x12)];
    p += anim[0x16] * 3;
    x2 = p[0];
    y2 = p[1];
    z2 = p[2];
    if (W(anim, 0) & 2) {
        H(obj, 0x98) = 0;
        H(obj, 0x9C) = 0;
    } else if (anim[3] == 0) {
        if (H(anim, 0x18) != 0) {
            f = H(anim, 0x1E);
            dx = (x2 * f) >> 12;
            H(obj, 0x98) = dx;
            dz = (z2 * f) >> 12;
            H(obj, 0x9C) = dz;
            W(anim, 0x28) += dx;
            W(anim, 0x30) += dz;
        } else {
            H(obj, 0x98) = x1 - W(anim, 0x28);
            H(obj, 0x9C) = z1 - W(anim, 0x30);
            W(anim, 0x28) = 0;
            W(anim, 0x30) = 0;
        }
    }
    H(*(u8 **)(obj + 0x94), 0xE) = y1 + (((y2 - y1) * H(anim, 0x18)) >> 12);
    BUSY(anim[0xA]) = obj[0xA6] != 0 ? 1 : 2;
}

/* an object taken out of the game: a person's slot and animation record let go */
static s32 n_800393E4(u8 *obj) {
    u8 *a;
    if (W(obj, 0x114) != 0) {
        func_800753A4(W(obj, 0x114));
        W(obj, 0x114) = 0;
    }
    if (*(u8 **)(obj + 0x10C) != NULL) {
        (*(u8 **)(obj + 0x10C))[0x65] = 0;
        *(u8 **)(obj + 0x10C) = NULL;
    }
    if (obj[5] == 6) {
        int go = 1;
        if (W(obj, 0xC0) != 0) {
            a = *(u8 **)(obj + 0x94);
            W(obj, 0xC0) = 0;
            if (a != NULL)
                BUSY(a[0xA]) = 0;
            else
                go = 0;
        }
        if (go && (a = *(u8 **)(obj + 0x94)) != NULL) {
            W(a, 0) &= ~1;
            if (H(*(u8 **)(obj + 0x94), 4) >= 0)
                func_80081458(H(*(u8 **)(obj + 0x94), 4));
            if (W(obj, 0xC0) != 0) {
                BUSY((*(u8 **)(obj + 0x94))[0xA]) = 0;
                W(obj, 0xC0) = 0;
            }
            *(u8 **)(obj + 0x94) = NULL;
        }
    }
    if ((*(u8 **)obj)[3] != 0)
        func_80039520((u32)obj);
    H(obj, 0x11C) = 0;
    H(obj, 0x11E) = 0;
    H(obj, 0x120) = 0;
    H(obj, 0x122) = 0;
    return 0;
}

/* the game's reinforcements: a spawn point's guard sent while fewer than 5 people are about and a slot of his type
 * is free */
static void n_80033090(void) {
    s32 s0, s1, s2, a1;
    u8 *s5, *a0;
    u32 f;
    if (D_8009CF74 != 0 || D_8009D8E4 != 0 || D_8009CF10 == 0)
        return;
    if (D_8009CF10 > 0)
        D_8009CF10 -= 1;
    if (D_8009CF2C > 0)
        D_8009CF2C -= 1;
    a1 = 0;
    a0 = D_800AFFC8 + D_800B0DA0 * 0x124;
    for (s2 = 0; s2 < D_800B0868; s2++, a0 += 0x124)
        if (a0[4] != 0)
            a1 += 1;
    if (a1 >= 5)
        return;
    s5 = (u8 *)D_8009CF14[1];
    for (s2 = 0; s2 < D_8009CF14[0]; s2++, s5 += 0x14) {
        f = W(s5, 0x10);
        if ((f & 0x4001) != 0x4001)
            continue;
        if (W(s5, 0xC) != 0) {
            W(s5, 0xC) -= 1;
            continue;
        }
        if (D_8009CF2C != 0)
            continue;
        s1 = ((f >> 1) & 3) == 0 ? D_8009CF14[4] : D_8009CF14[5];
        for (s0 = 0; s0 < slots; s0++) {
            if (BUSY(s0) == 0 && func_8005B584(0, (s16)s1, s0) != 0) {
                if (dry)
                    would_spawn = 1;
                else
                    func_80032EFC((u32)s5, s1);
                break;
            }
        }
    }
}

/* ---- the pools ---- */

/* a free animation record from first..last (the game's pool: then this mod's, past its own) */
static s32 n_8005D5C4(u8 *first, u8 *last) {
    u8 *p;
    s32 i, n;
    for (p = first; (u32)last >= (u32)p; p += ANIM_SIZE)
        if (!(W(p, 0) & 1))
            goto found;
    if (first != D_800B05B0 || last != D_800B05B0 + (GAME_ANIMS - 1) * ANIM_SIZE)
        return 0;
    n = slots - GAME_SLOTS;
    for (i = 0; i < n; i++)
        if (!(W(p = ext.anim[i], 0) & 1))
            goto found;
    return 0;
found:
    func_8005D630((u32)p);
    W(p, 0) |= 1;
    return (s32)p;
}

static u8 *ai_ctx(s32 i) {
    return i < GAME_AIS ? D_800D9278 + i * AI_SIZE : ext.ai[i - GAME_AIS];
}

/* a free AI context */
static u8 *n_800540CC(void) {
    s32 i, n = GAME_AIS + slots - GAME_SLOTS;
    u8 *a;
    for (i = 0; i < n; i++) {
        a = ai_ctx(i);
        if (!((*(u32 *)(a + 0x1CC) >> 1) & 1)) {
            *(u32 *)(a + 0x1CC) |= 2;
            return a;
        }
    }
    return NULL;
}

/* all AI contexts free */
static void n_80054118(void) {
    s32 i, n = GAME_AIS + slots - GAME_SLOTS;
    for (i = 0; i < n; i++)
        *(u32 *)(ai_ctx(i) + 0x1CC) &= ~2;
}

/* ---- drawing ---- */

/* the people drawn: the slots taken and shown into the render list, each one's view matrix, then drawn */
static void n_8005AEF8(void) {
    u8 **list_inst, **list_owner, *list_mat, *list_view;
    s32 a, i, k, n;
    u8 *e, *o, *m;
    s32 *s, *d;
    u32 ot;

    if (D_8009D8E4 == -1)
        return;
    a = D_8009D924 + D_8009D928;
    D_8009D924 = a;
    if (a == 0x1000 || a == 0)
        D_8009D928 = -D_8009D928;
    TransposeMatrix((u32)D_800B0018, (u32)D_8009D91C);
    D_8009D918[0] = -HU(D_800B0018, 0x14);
    D_8009D918[1] = -HU(D_800B0018, 0x18);
    D_8009D918[2] = -HU(D_800B0018, 0x1C);
    ApplyMatrix((u32)D_8009D91C, (u32)D_8009D918, (u32)((u8 *)D_8009D91C + 0x14));
    m = (u8 *)D_8009D91C;
    for (k = 0; k < 8; k++)
        GTE_ctc2(k, W(m, k * 4));
    s = D_8009D91C + 8;
    d = D_800DDE10 + 8;
    for (k = 8; k != 0; k--)
        *d-- = *s--;

    /* the render list: the game's while six can be in it */
    if (slots <= GAME_SLOTS) {
        list_inst = D_800DDF08, list_owner = D_800DDDEC, list_mat = D_800DDCCC, list_view = D_800DDF24;
    } else {
        list_inst = ext.list_inst, list_owner = ext.list_owner, list_mat = ext.list_mat[0], list_view = ext.list_view[0];
    }
    n = 0;
    for (i = 0; i < slots; i++) {
        if (BUSY(i) == 1 && !(i < 32 && (D_8009D8E4 & (1 << i))) && W(OWNER(i), 0x7C) >= 0) {
            list_inst[n] = INST(i);
            for (k = 0; k < 8; k++)
                W(list_mat + n * 0x20, k * 4) = W(MAT(i), k * 4);
            list_owner[n] = OWNER(i);
            n++;
        }
    }
    D_8009D8E8 = n; /* (more than six: past the game's byte, see people_set_slots) */

    for (i = 0; i < n; i++) {
        e = list_mat + i * 0x20;
        o = list_view + i * 0x20;
        HU(D_8009D918, 0) = HU(e, 0x14);
        HU(D_8009D918, 2) = HU(e, 0x18);
        HU(D_8009D918, 4) = HU(e, 0x1C);
        GTE_mtc2(0, W(D_8009D918, 0));
        GTE_mtc2(1, W(D_8009D918, 4));
        GTE_cmd(0x4A480012);
        W(o, 0x14) = GTE_mfc2(25);
        W(o, 0x18) = GTE_mfc2(26);
        W(o, 0x1C) = GTE_mfc2(27);
        for (k = 0; k < 3; k++) {
            GTE_mtc2(9, HU(e, k * 2));
            GTE_mtc2(10, HU(e, k * 2 + 6));
            GTE_mtc2(11, HU(e, k * 2 + 0xC));
            GTE_cmd(0x4A49E012);
            HU(D_8009D920, k * 2) = GTE_mfc2(9);
            HU(D_8009D920, k * 2 + 6) = GTE_mfc2(10);
            HU(D_8009D920, k * 2 + 0xC) = GTE_mfc2(11);
        }
        HU(o, 0) = HU(D_8009D920, 0);
        HU(o, 2) = HU(D_8009D920, 4);
        HU(o, 4) = -HU(D_8009D920, 2);
        HU(o, 6) = HU(D_8009D920, 6);
        HU(o, 8) = HU(D_8009D920, 0xA);
        HU(o, 0xA) = -HU(D_8009D920, 8);
        HU(o, 0xC) = HU(D_8009D920, 0xC);
        HU(o, 0xE) = HU(D_8009D920, 0x10);
        HU(o, 0x10) = -HU(D_8009D920, 0xE);
    }
    func_8005C33C();
    func_80033F38();
    for (i = 0; i < n; i++) {
        s32 w, x, y, z;
        o = list_view + i * 0x20;
        w = (s32)((u32)W(o, 0x18) << 12) / 0x1999;
        x = (s32)((u32)(s32)H(o, 6) << 12) / 0x1999;
        y = (s32)((u32)(s32)H(o, 8) << 12) / 0x1999;
        z = (s32)((u32)(s32)H(o, 0xA) << 12) / 0x1999;
        W(o, 0x18) = w;
        H(o, 6) = x;
        H(o, 8) = y;
        H(o, 0xA) = z;
    }
    ot = *(u32 *)(D_800B8F88 + D_800B8F24 * 20 + 4);
    func_800AACFC((u32)list_inst, (u32)list_view, n, ot);
    if (D_8009D8F0 > 0) {
        func_8004A398();
        D_8009D8F0 = D_8009D8F0 - 1;
        if (D_8009D8F0 <= 0)
            D_800DDDA8 = 0;
    }
}

/* ---- TWINE_PEOPLE_VERIFY=1: each call (outside a call of another of these) runs the game's own function first, then
 * this one from the same state (RAM, scratchpad, GTE, the extra slots), and logs any difference in what they leave
 * or return ("people verify:" lines; a count every 2000 calls). Not where the game's own would act outside that
 * state: func_800393E4 freeing heap memory (+0x114), func_80033090 sending a guard (a dry run of this one says). ---- */
extern u32 port_ram_size;
extern s32 D_8009CDA8;
extern int port_subframe;
void port_gte_get(u32 *regs);
void port_gte_put(const u32 *regs);
void port_log(const char *fmt, ...);
char *getenv(const char *name);
void *malloc(unsigned n);
void *memcpy(void *d, const void *s, unsigned n);
int memcmp(const void *a, const void *b, unsigned n);

void func_8005B508_orig(void);
s32 func_8005B584_orig(s32 a0, u32 v, s32 i);
s32 func_8005B5AC_orig(u8 *obj, s32 *p);
s32 func_8003C88C_orig(s32 slot, s32 type);
void func_8005AEF8_orig(void);
void func_8005B96C_orig(u8 *obj, s32 pos, u8 *anim);
void func_8005BA30_orig(u8 *obj);
void func_8005BAE8_orig(u8 *obj, u8 *anim);
s32 func_800393E4_orig(u8 *obj);
void func_80033090_orig(void);
s32 func_8005D5C4_orig(u8 *first, u8 *last);
u8 *func_800540CC_orig(void);
void func_80054118_orig(void);

#define RAM ((u8 *)0x80000000u)
#define SCRATCH ((u8 *)0x1F800000u)
static int verify = -1, depth;
static u8 *ram_pre, *ram_post;
static u8 scr_pre[0x1000], scr_post[0x1000];
static u32 gte_pre[64], gte_post[64];
static u8 ext_pre[sizeof(ext)], ext_post[sizeof(ext)];
static u32 calls, diffs;

static void snap(u8 *ram, u8 *scr, u32 *gte, u8 *e) {
    memcpy(ram, RAM, port_ram_size);
    memcpy(scr, SCRATCH, 0x1000);
    port_gte_get(gte);
    memcpy(e, &ext, sizeof(ext));
}
static void put(u8 *ram, u8 *scr, u32 *gte, u8 *e) {
    memcpy(RAM, ram, port_ram_size);
    memcpy(SCRATCH, scr, 0x1000);
    port_gte_put(gte);
    memcpy(&ext, e, sizeof(ext));
}

/* this call is checked: the state before it kept */
static int v_begin(void) {
    if (verify < 0) {
        const char *e = getenv("TWINE_PEOPLE_VERIFY");
        verify = e && *e == '1';
        if (verify) {
            ram_pre = malloc(port_ram_size);
            ram_post = malloc(port_ram_size);
            if (!ram_pre || !ram_post)
                verify = 0;
        }
    }
    if (!verify || depth || port_subframe)
        return 0;
    snap(ram_pre, scr_pre, gte_pre, ext_pre);
    return 1;
}
/* the game's has run: what it left kept, the state before put back */
static void v_mid(void) {
    snap(ram_post, scr_post, gte_post, ext_post);
    put(ram_pre, scr_pre, gte_pre, ext_pre);
}
static void v_end(const char *name, u32 r_orig, u32 r_new) {
    u32 i, n = 0, first = 0, last = 0, gte[64];
    int scr, g, e;
    for (i = 0; i < port_ram_size; i += 4)
        if (*(u32 *)(ram_post + i) != *(u32 *)(RAM + i)) {
            if (!n++)
                first = i;
            last = i;
        }
    port_gte_get(gte);
    scr = memcmp(scr_post, SCRATCH, 0x1000) != 0;
    g = memcmp(gte_post, gte, sizeof(gte)) != 0;
    e = memcmp(ext_post, &ext, sizeof(ext)) != 0;
    calls++;
    if (n || r_orig != r_new || scr || g || e) {
        diffs++;
        port_log("people verify: f%d %s differs: %u RAM words (%08X..%08X: %08X, ours %08X), return %08X ours %08X, "
                 "scratchpad %d, GTE %d, ext %d\n", D_8009CDA8, name, n, 0x80000000u + first, 0x80000000u + last,
                 n ? *(u32 *)(ram_post + first) : 0, n ? *(u32 *)(RAM + first) : 0, r_orig, r_new, scr, g, e);
    }
    if (calls % 2000 == 0)
        port_log("people verify: %u calls checked, %u differ\n", calls, diffs);
}

#define CHECKED(ret, name, params, args, ret_orig, ret_new)                                                    \
    ret func_##name params {                                                                                  \
        u32 r_orig = 0, r_new;                                                                                \
        int chk = v_begin();                                                                                  \
        depth++;                                                                                              \
        if (chk) {                                                                                            \
            ret_orig func_##name##_orig args;                                                                 \
            v_mid();                                                                                          \
        }                                                                                                     \
        ret_new n_##name args;                                                                                \
        depth--;                                                                                              \
        if (chk)                                                                                              \
            v_end(#name, r_orig, r_new);                                                                      \
        return (ret)r_new;                                                                                    \
    }
#define VOID_CHECKED(name, params, args)                                                                       \
    void func_##name params {                                                                                 \
        int chk = v_begin();                                                                                  \
        depth++;                                                                                              \
        if (chk) {                                                                                            \
            func_##name##_orig args;                                                                          \
            v_mid();                                                                                          \
        }                                                                                                     \
        n_##name args;                                                                                        \
        depth--;                                                                                              \
        if (chk)                                                                                              \
            v_end(#name, 0, 0);                                                                               \
    }

VOID_CHECKED(8005B508, (void), ())
CHECKED(s32, 8005B584, (s32 a0, u16 v, s16 i), (a0, v, i), r_orig = (u32), r_new = (u32))
CHECKED(s32, 8005B5AC, (u8 *obj, s32 *p), (obj, p), r_orig = (u32), r_new = (u32))
CHECKED(s32, 8003C88C, (s32 slot, s32 type), (slot, type), r_orig = (u32), r_new = (u32))
VOID_CHECKED(8005AEF8, (void), ())
VOID_CHECKED(8005B96C, (u8 *obj, s32 pos, u8 *anim), (obj, pos, anim))
VOID_CHECKED(8005BA30, (u8 *obj), (obj))
VOID_CHECKED(8005BAE8, (u8 *obj, u8 *anim), (obj, anim))
CHECKED(s32, 8005D5C4, (u8 *first, u8 *last), (first, last), r_orig = (u32), r_new = (u32))
CHECKED(u8 *, 800540CC, (void), (), r_orig = (u32), r_new = (u32))
VOID_CHECKED(80054118, (void), ())

s32 func_800393E4(u8 *obj) {
    u32 r_orig = 0, r_new;
    int v = W(obj, 0x114) == 0 && v_begin(); /* (not when it frees heap memory) */
    depth++;
    if (v) {
        r_orig = func_800393E4_orig(obj);
        v_mid();
    }
    r_new = n_800393E4(obj);
    depth--;
    if (v)
        v_end("800393E4", r_orig, r_new);
    return r_new;
}

void func_80033090(void) {
    int v = v_begin();
    if (v) { /* (a dry run first: when this would send a guard, not checked) */
        dry = 1, would_spawn = 0;
        depth++;
        n_80033090();
        depth--;
        dry = 0;
        put(ram_pre, scr_pre, gte_pre, ext_pre);
        v = !would_spawn;
    }
    depth++;
    if (v) {
        func_80033090_orig();
        v_mid();
    }
    n_80033090();
    depth--;
    if (v)
        v_end("80033090", 0, 0);
}
