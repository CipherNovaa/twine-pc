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
 * own entries. With 6 slots they do exactly what the game's do: TWINE_PEOPLE_VERIFY=1 runs each call twice, the
 * game's own then this one from the same state, and logs any difference (none, over all 11 levels and every endless
 * pool room, 2 MB and 8 MB; workbench/research/NOTES_people.md). The render list (D_800DDF08 the instances drawn,
 * D_800DDCCC their matrices, D_800DDDEC their owners, D_800DDF24 the view matrices) stays the game's while it holds
 * six at most; past six it is this mod's, handed to the game's code six at a time.
 *
 * More than six (step 6b): with RAM past the console's 2 MB an endless run may have more (func_8002E228 below). The
 * extra slots' instances are copies of the game's slots' (n_8005B508), the extra pool entries in the ext heap.
 *
 * MOD_REPLACES_NATIVE: func_8005B508 func_8005B584 func_8005B5AC func_8003C88C func_8005AEF8 func_8005B96C func_8005BA30 func_8005BAE8 func_800393E4 func_80033090 func_8005D5C4 func_800540CC func_80054118 func_8002E228 func_8005ED88 func_8003A71C
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
void *memcpy(void *d, const void *s, unsigned n);

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
    u8 rec[EXT][0x20]; /* (each its own record: a copy of the game's slot it copies, its type and texture chunk) */
    u8 *list_inst[MAX_SLOTS + 1];
    u8 list_mat[MAX_SLOTS][0x20];
    u8 *list_owner[MAX_SLOTS];
    u8 list_view[MAX_SLOTS][0x20];
} ext;
/* the pools' extra entries and the extra slots' model instances: in PS1 RAM past the console's 2 MB (the ext heap,
 * port_ext_alloc), where the game's code may treat them as its own (save states and smooth motion keep them too);
 * the ext heap starts afresh with each executable, so they're taken again for each (pools_gen) */
typedef struct {
    u8 anim[EXT][ANIM_SIZE];
    u8 ai[EXT][AI_SIZE];
} Pools;
static Pools *pools;
static int pools_gen = -1, clones_gen = -1;
static u8 *clone_buf[EXT];
static u32 clone_size[EXT];
static u8 *raw_buf[GAME_SLOTS]; /* (the game's slots' instances as the GUY file has them, before relocation) */
static u32 raw_size[GAME_SLOTS], raw_most;
static s8 bound[EXT];           /* (each extra slot: the game's slot it is a copy of) */
static u8 guard_type[16];       /* (people_guard_types: the kinds the extra slots go to) */
static int guard_types;
static int slots = GAME_SLOTS; /* the slots in use (the pools: as many more past their own) */
static int kept;
static int dry, would_spawn; /* (TWINE_PEOPLE_VERIFY: func_80033090 run to see whether it would send a guard) */
static int trace = -1;       /* TWINE_PEOPLE_TRACE=1: the slots' instances, clones, counts logged */
extern u32 port_ram_size;
extern int port_endless;
extern int port_subframe;
extern s32 D_8009CDA8;
int port_endless_gen(void);
void *port_ext_alloc(u32 size);
void port_ext_free(void *p);
void port_log(const char *fmt, ...);
char *getenv(const char *name);
int atoi(const char *s);
#define TRACE (trace < 0 ? (trace = getenv("TWINE_PEOPLE_TRACE") && *getenv("TWINE_PEOPLE_TRACE") == '1') : trace)

/* ---- for other mods ---- */
int people_slots(void) {
    return slots;
}

/* n slots from now on (6..MAX_SLOTS; more than 6 only with RAM past the console's 2 MB); the extra ones start free */
void people_set_slots(int n) {
    if (n < GAME_SLOTS)
        n = GAME_SLOTS;
    if (n > MAX_SLOTS)
        n = MAX_SLOTS;
    if (n > GAME_SLOTS && pools_gen != port_endless_gen()) {
        pools = port_ext_alloc(sizeof(Pools));
        pools_gen = port_endless_gen();
        if (pools)
            memset(pools, 0, sizeof(Pools));
    }
    if (n > GAME_SLOTS && !pools)
        n = GAME_SLOTS;
    if (n != slots && TRACE)
        port_log("people: %d slots (pools at %08X)\n", n, (u32)pools);
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

static int depth_trace(void) { /* (TWINE_PEOPLE_VERIFY's dry runs aside) */
    return dry;
}
int snprintf(char *s, unsigned n, const char *fmt, ...);

/* ---- the slots ---- */

/* how long the game's slot i's model instance is: up to the next one in the section's GUY file (the instances lie
 * one after the other in it), the last as long as the longest other */
static u32 inst_size(int i) {
    u32 at = (u32)INST(i), next = 0, most = 0, k, sz;
    for (k = 0; k < GAME_SLOTS; k++) {
        u32 o = (u32)INST(k);
        if (o > at && (!next || o < next))
            next = o;
    }
    if (next)
        return next - at;
    for (k = 0; k < GAME_SLOTS; k++)
        if (k != i && (sz = inst_size(k)) > most && (u32)INST(k) < at)
            most = sz;
    return most ? most : 0x8000;
}

/* extra slot i made a copy of the game's slot s: its instance as loaded, relocated where it is, its record (type,
 * texture chunk); only while free */
static void bind(int i, int s) {
    int j = i - GAME_SLOTS;
    if (BUSY(i) || !clone_buf[j] || !raw_buf[s] || bound[j] == s)
        return;
    memcpy(clone_buf[j], raw_buf[s], raw_size[s]);
    memcpy(ext.rec[j], REC(s), 0x20);
    func_8007CE30((u32)clone_buf[j], REC(s)[1]);
    ext.inst[j] = clone_buf[j];
    bound[j] = s;
}

/* the free extra slots shared out between the game's slots of the kinds of guards a mod asked for (endless.c: the
 * room's), as they are bound now; a game slot's type follows into its copies */
static void rebind(void) {
    int src[GAME_SLOTS], n = 0, s, k, i;
    for (s = 0; s < GAME_SLOTS; s++)
        for (k = 0; k < guard_types; k++)
            if (HU(REC(s), 0x1C) == guard_type[k]) {
                src[n++] = s;
                break;
            }
    for (i = GAME_SLOTS; i < slots; i++) {
        int j = i - GAME_SLOTS;
        if (n)
            bind(i, src[j % n]);
        if (bound[j] >= 0 && !BUSY(i))
            H(ext.rec[j], 0x1C) = H(REC(bound[j]), 0x1C);
    }
    if (TRACE) {
        char at[200];
        int m = 0;
        for (i = GAME_SLOTS; i < slots && m < 190; i++)
            m += snprintf(at + m, sizeof(at) - m, " %d:%d(type %d)", i, bound[i - GAME_SLOTS], HU(REC(i), 0x1C));
        at[m] = 0;
        port_log("people: extra slots (slot:copy of):%s\n", at);
    }
}

/* for other mods: the kinds of guards the extra slots should go to (the room's) */
void people_guard_types(const u8 *types, int n) {
    int k;
    guard_types = n < 16 ? n : 16;
    for (k = 0; k < guard_types; k++)
        guard_type[k] = types[k];
    if (slots > GAME_SLOTS)
        rebind();
}

/* each slot's instance relocated and bound to its texture chunk; all free (after a section's GUY file loads). The
 * extra slots (6 and up) get copies of the game's slots' instances (slot j the copy of j mod 6, as it shares its
 * record), made before those are relocated in place, in the ext heap */
static void n_8005B508(void) {
    s32 i;
    if (TRACE)
        for (i = 0; i < GAME_SLOTS; i++) {
            u32 *h = (u32 *)INST(i);
            port_log("people: slot %d inst %08X (%X bytes) chunk %d type %d: %08X n %d base %08X +C %X +10 %X +14 %X "
                     "+18 %X +1C %X +54 %X\n", i, (u32)h, inst_size(i), REC(i)[1], HU(REC(i), 0x1C), h[0], HU(h, 4),
                     h[2], h[3], h[4], h[5], h[6], h[7], h[0x15]);
        }
    if (slots > GAME_SLOTS) { /* (the game's instances kept as loaded, then the extra slots made copies of them) */
        if (clones_gen != port_endless_gen()) { /* (a new executable: the ext heap afresh) */
            memset(clone_buf, 0, sizeof(clone_buf));
            memset(clone_size, 0, sizeof(clone_size));
            memset(raw_buf, 0, sizeof(raw_buf));
            memset(raw_size, 0, sizeof(raw_size));
            clones_gen = port_endless_gen();
        }
        raw_most = 0;
        for (i = 0; i < GAME_SLOTS; i++) {
            u32 sz = inst_size(i);
            if (raw_size[i] < sz) {
                if (raw_buf[i])
                    port_ext_free(raw_buf[i]);
                raw_buf[i] = port_ext_alloc(sz);
                raw_size[i] = raw_buf[i] ? sz : 0;
            }
            if (raw_buf[i])
                memcpy(raw_buf[i], INST(i), sz);
            raw_size[i] = raw_buf[i] ? sz : 0;
            if (raw_size[i] > raw_most)
                raw_most = raw_size[i];
        }
        for (i = GAME_SLOTS; i < slots; i++) {
            int j = i - GAME_SLOTS;
            if (clone_size[j] < raw_most) {
                if (clone_buf[j])
                    port_ext_free(clone_buf[j]);
                clone_buf[j] = port_ext_alloc(raw_most);
                clone_size[j] = clone_buf[j] ? raw_most : 0;
            }
            ext.inst[j] = NULL;
            bound[j] = -1;
            BUSY(i) = 0;
            bind(i, i % GAME_SLOTS); /* (the default: the game's slots twice over) */
        }
    }
    for (i = 0; i < GAME_SLOTS; i++) {
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
    if (TRACE && !depth_trace()) {
        char at[200];
        int m = 0;
        for (i = 0; i < slots && m < 190; i++)
            m += snprintf(at + m, sizeof(at) - m, " %d:%d/%d", i, HU(REC(i), 0x1C), BUSY(i));
        at[m] = 0;
        port_log("people: no free slot of type %d (slot:type/taken%s)\n", obj[6], at);
    }
    return 0;
}

/* slot bound to an object type */
static s32 n_8003C88C(s32 slot, s32 type) {
    H(REC(slot), 0x1C) = (s16)type;
    if (slots > GAME_SLOTS && slot < GAME_SLOTS)
        rebind();
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
    if (first != D_800B05B0 || last != D_800B05B0 + (GAME_ANIMS - 1) * ANIM_SIZE) {
        if (TRACE && !depth_trace())
            port_log("people: no free animation record in %08X-%08X\n", (u32)first, (u32)last);
        return 0;
    }
    n = slots - GAME_SLOTS;
    for (i = 0; i < n; i++)
        if (!(W(p = pools->anim[i], 0) & 1))
            goto found;
    if (TRACE && !depth_trace())
        port_log("people: no free animation record (%d)\n", GAME_ANIMS + n);
    return 0;
found:
    func_8005D630((u32)p);
    W(p, 0) |= 1;
    return (s32)p;
}

static u8 *ai_ctx(s32 i) {
    return i < GAME_AIS ? D_800D9278 + i * AI_SIZE : pools->ai[i - GAME_AIS];
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
    if (TRACE && !depth_trace())
        port_log("people: no free AI context (%d)\n", n);
    return NULL;
}

/* all AI contexts free */
static void n_80054118(void) {
    s32 i, n = GAME_AIS + slots - GAME_SLOTS;
    for (i = 0; i < n; i++)
        *(u32 *)(ai_ctx(i) + 0x1CC) &= ~2;
}

/* ---- the people's texture animations ---- */

/* A person whose model animates its textures (its type's def +3: blinking, a face) takes a record of the level's
 * table of them (D_800D8FF8, 0x48 bytes each, D_8009D368 of them, set up by the mission for its own people: a few
 * per type), or isn't made at all (func_8003A71C). Past the game's six people a type can run out: then the person
 * shares one of his type's records (obj+0xA9), its owner kept (func_80039520 lets a record go only for its owner),
 * so they animate together; never a record that takes its object out when its time is up (flags 8). */
#define TEX_ANIM_SIZE 0x48
extern u8 D_800D8FF8[];
extern s32 D_8009D368;
u8 *func_8003A71C_orig(u8 **obj);
u8 *func_8003A71C(u8 **obj) {
    u8 *r = func_8003A71C_orig(obj);
    s32 i;
    if (r || slots <= GAME_SLOTS || !(*obj)[3])
        return r;
    for (i = 0; i < D_8009D368; i++) {
        u8 *e = D_800D8FF8 + i * TEX_ANIM_SIZE;
        if (e[0] && e[1] == (*obj)[3] && !(HU(e, 0x34) & 8)) {
            ((u8 *)obj)[0xA9] = i;
            if (TRACE)
                port_log("people: a person (texture animation type %d) shares record %d\n", (*obj)[3], i);
            return e;
        }
    }
    if (TRACE) {
        char at[300];
        int m = 0;
        for (i = 0; i < D_8009D368 && m < 280; i++)
            m += snprintf(at + m, sizeof(at) - m, " %d:t%d/a%d/f%X", i, D_800D8FF8[i * TEX_ANIM_SIZE + 1],
                          D_800D8FF8[i * TEX_ANIM_SIZE], HU(D_800D8FF8 + i * TEX_ANIM_SIZE, 0x34));
        at[m] = 0;
        port_log("people: no texture animation record for type %d (%d:%s)\n", (*obj)[3], D_8009D368, at);
    }
    return NULL;
}

/* ---- how many ---- */

/* With RAM past the console's 2 MB (the ram_8mb setting) an endless run has 12 slots (TWINE_PEOPLE_SLOTS=n: n, 6-24,
 * for tests); otherwise the game's 6. Decided as the mission sets its object count (func_8002E228: the people's
 * objects, D_800B0868, then everything's), which grows by as many: the people objects 1..D_800B0868 then reach
 * the slots. */
#define WANT_SLOTS 12
void func_8002E228_orig(void);
void func_8005ED88_orig(void);
static int wanted(void) {
    const char *e = getenv("TWINE_PEOPLE_SLOTS");
    if (port_ram_size <= 0x200000 || !port_endless)
        return GAME_SLOTS;
    return e && *e ? atoi(e) : WANT_SLOTS;
}
/* The held things' objects (a guard's gun among them: func_80039A34 makes it, or the person isn't made) are two
 * ranges (D_800B0AC8 of one kind, D_800B0AE4 of the other) inside the range of D_800B008C objects that follows the
 * people's, which they fill: each grows by the extra slots, and the range around them by both. */
extern s16 D_800B0AC8, D_800B0AE4, D_800B008C;
static s16 counts_set[4]; /* (what this left: a mission sets its own before each call) */
void func_8002E228(void) {
    s16 *v[4] = {&D_800B0868, &D_800B0AC8, &D_800B0AE4, &D_800B008C}, own[4];
    int k, extra;
    for (k = 0; k < 4; k++) { /* (called again without the mission setting its own: the mission's back) */
        int was = (slots - GAME_SLOTS) * (k == 3 ? 2 : 1);
        own[k] = *v[k] - (counts_set[0] && *v[k] == counts_set[k] ? was : 0);
    }
    people_set_slots(wanted());
    extra = slots - GAME_SLOTS;
    for (k = 0; k < 4; k++)
        counts_set[k] = *v[k] = own[k] + extra * (k == 3 ? 2 : 1);
    if (TRACE)
        port_log("people: %d slots: %d people objects, held things %d+%d in %d (the mission's %d, %d+%d in %d)\n",
                 slots, D_800B0868, D_800B0AC8, D_800B0AE4, D_800B008C, own[0], own[1], own[2], own[3]);
    func_8002E228_orig();
}

/* the animation records all free (as a mission starts): the game's, and the extra ones */
void func_8005ED88(void) {
    int i;
    func_8005ED88_orig();
    if (pools && pools_gen == port_endless_gen())
        for (i = 0; i < EXT; i++)
            W(pools->anim[i], 0) &= ~1;
}

/* ---- drawing ---- */

/* What the game's render step calls, each by its PS1 address as the game's own (recompiled) code calls it: some of
 * them have C beside their recompiled code that doesn't do quite the same (func_80033F38's and func_8005C33C's: the
 * people drawn a pixel or two off, TWINE_PEOPLE_VERIFY), so they're called the way the game calls them, the
 * recompiled code where there is some (port_rc_call). Per level, from each level's recompiled func_8005AEF8:
 * TransposeMatrix, ApplyMatrix, func_8005C33C, func_80033F38, func_800AACFC (the people's skinned models drawn),
 * func_8004A398. */
static const u32 draw_calls[12][6] = {
    {0},
    {0x8008C3E4, 0x8008C424, 0x8005C33C, 0x80033F38, 0x800AACFC, 0x8004A398},
    {0x8009058C, 0x800905CC, 0x8005C43C, 0x800348FC, 0x800B0714, 0x8004B030},
    {0x8008DFB8, 0x8008DFF8, 0x8005C058, 0x8003440C, 0x800AD614, 0x8004ADE0},
    {0x8008CE44, 0x8008CE84, 0x8005B930, 0x80034400, 0x800AC7F4, 0x8004A6B8},
    {0x80090A10, 0x80090A50, 0x8005C0E8, 0x80034630, 0x800AFE0C, 0x8004AE8C},
    {0x8008E3C4, 0x8008E404, 0x8005C4B0, 0x800347D8, 0x800ADDC4, 0x8004AFB4},
    {0x8009137C, 0x800913BC, 0x8005C938, 0x80034968, 0x800B149C, 0x8004B558},
    {0x8008E7A8, 0x8008E7E8, 0x8005C5FC, 0x8003466C, 0x800ADF6C, 0x8004B0A8},
    {0x80091080, 0x800910C0, 0x8005C72C, 0x80034B0C, 0x800B084C, 0x8004B220},
    {0x80090594, 0x800905D4, 0x8005EB68, 0x8003466C, 0x800AFCFC, 0x8004B7B0},
    {0x8008B9F8, 0x8008BA38, 0x80059780, 0x80033A20, 0x800AA68C, 0x80048650},
};
enum { C_TRANSPOSE, C_APPLY, C_LIGHTS, C_F38, C_DRAW, C_A398 };
typedef struct {
    u32 r[32];
    u32 hi, lo;
    u32 cop0[16];
} RcCtx;
void port_rc_call(RcCtx *c, u32 addr);
u32 port_rc_enter(void);
void port_rc_leave(u32 sp);
static u32 ps1(int which, u32 a0, u32 a1, u32 a2, u32 a3) {
    RcCtx c;
    u32 sp;
    memset(&c, 0, sizeof(c));
    sp = port_rc_enter();
    c.r[29] = sp;
    c.r[4] = a0, c.r[5] = a1, c.r[6] = a2, c.r[7] = a3;
    port_rc_call(&c, draw_calls[g_Args->level][which]);
    port_rc_leave(sp);
    return c.r[2];
}

/* more than six drawn: func_8005C33C (the people against the light beams, D_800F2FBC: points it turns into the view
 * first, then each drawn person's bones through func_8007EC78, which finds their owners in the game's render list by
 * a fixed address) runs on six at a time from the game's list, the beams and the GTE as they were before each */
extern u8 D_800F2FBC[];
extern s32 D_800F3B48;
void port_gte_get(u32 *regs);
void port_gte_put(const u32 *regs);
static u8 beams[0x8C4];
static void to_game_list(u8 **inst, u8 *mat, u8 **owner, u8 *view, s32 n) {
    s32 k;
    for (k = 0; k < n; k++) {
        D_800DDF08[k] = inst[k];
        memcpy(D_800DDCCC + k * 0x20, mat + k * 0x20, 0x20);
        D_800DDDEC[k] = owner[k];
        memcpy(D_800DDF24 + k * 0x20, view + k * 0x20, 0x20);
    }
    D_8009D8E8 = n;
}

/* the people drawn: the slots taken and shown into the render list, each one's view matrix, then drawn */
static void n_8005AEF8(void) {
    u8 **list_inst, **list_owner, *list_mat, *list_view;
    s32 a, i, k, n, b, cnt;
    u8 *e, *o, *m;
    s32 *s, *d;
    u32 ot;

    if (D_8009D8E4 == -1)
        return;
    a = D_8009D924 + D_8009D928;
    D_8009D924 = a;
    if (a == 0x1000 || a == 0)
        D_8009D928 = -D_8009D928;
    ps1(C_TRANSPOSE, (u32)D_800B0018, (u32)D_8009D91C, 0, 0);
    D_8009D918[0] = -HU(D_800B0018, 0x14);
    D_8009D918[1] = -HU(D_800B0018, 0x18);
    D_8009D918[2] = -HU(D_800B0018, 0x1C);
    ps1(C_APPLY, (u32)D_8009D91C, (u32)D_8009D918, (u32)((u8 *)D_8009D91C + 0x14), 0);
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
    D_8009D8E8 = n < 0x100 ? n : 0xFF;
    if (TRACE && !port_subframe) { /* (the most drawn at once, every 2 s of frames) */
        static int most, at;
        if (n > most)
            most = n;
        if (D_8009CDA8 / 40 != at) {
            at = D_8009CDA8 / 40;
            port_log("people: drawn at once (2 s): %d of %d slots\n", most, slots);
            most = 0;
        }
    }

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
    if (list_inst == D_800DDF08) {
        ps1(C_LIGHTS, 0, 0, 0, 0);
    } else { /* (six at a time, each from the same beams and GTE) */
        u32 gte[64];
        s32 nb = D_800F3B48 > 0 ? (D_800F3B48 < 33 ? D_800F3B48 : 33) * 0x44 : 0;
        port_gte_get(gte);
        memcpy(beams, D_800F2FBC, nb);
        for (b = 0; b == 0 || b < n; b += GAME_SLOTS) {
            cnt = n - b < GAME_SLOTS ? n - b : GAME_SLOTS;
            if (b) {
                port_gte_put(gte);
                memcpy(D_800F2FBC, beams, nb);
            }
            to_game_list(list_inst + b, list_mat + b * 0x20, list_owner + b, list_view + b * 0x20, cnt);
            ps1(C_LIGHTS, 0, 0, 0, 0);
        }
    }
    ps1(C_F38, 0, 0, 0, 0);
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
    if (list_inst == D_800DDF08)
        ps1(C_DRAW, (u32)list_inst, (u32)list_view, n, ot);
    else
        for (b = 0; b < n; b += GAME_SLOTS) /* (the game's list left with the last six, as the lights had it) */
            ps1(C_DRAW, (u32)(list_inst + b), (u32)(list_view + b * 0x20), n - b < GAME_SLOTS ? n - b : GAME_SLOTS,
                ot);
    if (D_8009D8F0 > 0) {
        ps1(C_A398, 0, 0, 0, 0);
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
extern volatile int port_rt_depth;
void port_rt_leave(void);
void port_gte_get(u32 *regs);
void port_gte_put(const u32 *regs);
void port_log(const char *fmt, ...);
char *getenv(const char *name);
void *malloc(unsigned n);
void *memcpy(void *d, const void *s, unsigned n);
int memcmp(const void *a, const void *b, unsigned n);
int snprintf(char *s, unsigned n, const char *fmt, ...);

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
static u32 calls, diffs, own_calls, own_diffs;
static const char *v_name[16];
static u32 v_count[16];

#define RCSTACK ((u8 *)0x1E000000u) /* (recompiled code's stack, recomp_rt.c: what a callee finds there unset) */
#define RCSTACK_SIZE 0x100000u
static u8 *rcs_pre, *rcs_post;
static void snap(u8 *ram, u8 *scr, u32 *gte, u8 *e) {
    if (verify == 3)
        memcpy(ram == ram_pre ? rcs_pre : rcs_post, RCSTACK, RCSTACK_SIZE);
    memcpy(ram, RAM, port_ram_size);
    memcpy(scr, SCRATCH, 0x1000);
    port_gte_get(gte);
    memcpy(e, &ext, sizeof(ext));
}
static void put(u8 *ram, u8 *scr, u32 *gte, u8 *e) {
    if (verify == 3)
        memcpy(RCSTACK, ram == ram_pre ? rcs_pre : rcs_post, RCSTACK_SIZE);
    memcpy(RAM, ram, port_ram_size);
    memcpy(SCRATCH, scr, 0x1000);
    port_gte_put(gte);
    memcpy(&ext, e, sizeof(ext));
}

/* this call is checked: the state before it kept */
static int v_begin(void) {
    if (verify < 0) {
        const char *e = getenv("TWINE_PEOPLE_VERIFY");
        verify = e && *e >= '1' && *e <= '3' ? *e - '0' : 0; /* (3: as 2, the recompiled code's stack kept too) */
        if (verify) {
            ram_pre = malloc(port_ram_size);
            ram_post = malloc(port_ram_size);
            if (verify == 3)
                rcs_pre = malloc(RCSTACK_SIZE), rcs_post = malloc(RCSTACK_SIZE);
            if (!ram_pre || !ram_post || (verify == 3 && (!rcs_pre || !rcs_post)))
                verify = 0;
        }
    }
    if (!verify || depth || port_subframe)
        return 0;
    port_rt_depth++; /* (no vblank in between: its handlers would change RAM under one run and not the other) */
    snap(ram_pre, scr_pre, gte_pre, ext_pre);
    return 1;
}
/* the game's has run: what it left kept, the state before put back */
static void v_mid(void) {
    snap(ram_post, scr_post, gte_post, ext_post);
    put(ram_pre, scr_pre, gte_pre, ext_pre);
}
/* where the game's run left what is at addr now (RAM or the scratchpad) */
static u8 *post_of(u32 addr) {
    return addr - 0x1F800000u < 0x1000 ? scr_post + (addr - 0x1F800000u) : ram_post + (addr & 0x7FFFFF);
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
    if (name[4] == 'A' && name[5] == 'E') { /* func_8005AEF8: what it works out itself (the render list, the view
                                             * matrices), apart from what its callees draw (its own differs there
                                             * as often, run twice: TWINE_PEOPLE_VERIFY=2) */
        static const struct {
            void *at;
            u32 n;
        } own[] = {{D_800DDF08, 24}, {D_800DDCCC, 0xC0}, {D_800DDDEC, 24}, {D_800DDF24, 0xC0}, {&D_8009D8E8, 1},
                   {&D_8009D924, 4}, {&D_8009D928, 4}, {D_800DDE10, 0x24}};
        u32 k, o = 0;
        for (k = 0; k < sizeof(own) / sizeof(own[0]); k++)
            o |= (memcmp(post_of((u32)own[k].at), own[k].at, own[k].n) != 0) << k;
        o |= (memcmp(post_of((u32)D_8009D91C), D_8009D91C, 0x24) != 0) << 8;
        o |= (memcmp(post_of((u32)D_8009D918), D_8009D918, 8) != 0) << 9;
        o |= (memcmp(post_of((u32)D_8009D920), D_8009D920, 0x14) != 0) << 10;
        own_calls++;
        if (o) {
            own_diffs++;
            port_log("people verify: f%d 8005AEF8's own outputs differ (mask %X)\n", D_8009CDA8, o);
        }
        if (own_calls % 500 == 0)
            port_log("people verify: 8005AEF8's own outputs: %u calls, %u differ\n", own_calls, own_diffs);
    }
    port_gte_get(gte);
    scr = memcmp(scr_post, SCRATCH, 0x1000) != 0;
    g = memcmp(gte_post, gte, sizeof(gte)) != 0;
    e = memcmp(ext_post, &ext, sizeof(ext)) != 0;
    calls++;
    if (n || r_orig != r_new || scr || g || e) {
        char at[160];
        int k = 0, m = 0;
        for (i = 0; i < port_ram_size && k < 6; i += 4)
            if (*(u32 *)(ram_post + i) != *(u32 *)(RAM + i))
                m += snprintf(at + m, sizeof(at) - m, " %08X:%X/%X", 0x80000000u + i, *(u32 *)(ram_post + i),
                              *(u32 *)(RAM + i)), k++;
        at[m] = 0;
        diffs++;
        port_log("people verify: f%d %s at%s\n", D_8009CDA8, name, at);
        port_log("people verify: f%d %s differs: %u RAM words (%08X..%08X: %08X, ours %08X), return %08X ours %08X, "
                 "scratchpad %d, GTE %d, ext %d\n", D_8009CDA8, name, n, 0x80000000u + first, 0x80000000u + last,
                 n ? *(u32 *)(ram_post + first) : 0, n ? *(u32 *)(RAM + first) : 0, r_orig, r_new, scr, g, e);
    }
    for (i = 0; i < 16 && v_name[i] && v_name[i] != name; i++)
        ;
    if (i < 16)
        v_name[i] = name, v_count[i]++;
    if (calls % 2000 == 0) {
        char at[400];
        int m = 0;
        for (i = 0; i < 16 && v_name[i]; i++)
            m += snprintf(at + m, sizeof(at) - m, " %s %u", v_name[i], v_count[i]);
        at[m] = 0;
        port_log("people verify: %u calls checked, %u differ (by function:%s)\n", calls, diffs, at);
    }
    port_rt_leave();
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
        if (chk && verify >= 2) /* (the game's twice: what the check itself can't hold the same) */          \
            ret_new func_##name##_orig args;                                                                  \
        else                                                                                                  \
            ret_new n_##name args;                                                                            \
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
        if (chk && verify >= 2)                                                                               \
            func_##name##_orig args;                                                                          \
        else                                                                                                  \
            n_##name args;                                                                                    \
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
        if (!v)
            port_rt_leave();
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
