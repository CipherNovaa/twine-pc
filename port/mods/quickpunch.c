/* Quick punch (native PC build only): Space or the mouse's back button, or R3 on a gamepad, throws a punch with Bond's
 * free hand while he keeps the weapon he holds, as the unarmed punch does it: the same blow, sound and animation (mirrored for the left).
 *
 * The weapon in Bond's hands is an object of its own (D_8009CD98, sub type 10, made by func_80079F84 for the
 * weapon's first-person model g_Weapons[]+0x5B); its model instance (+0x114) carries the animation and, at +0x14,
 * the coordinate system it hangs from (at the camera). The weapon's state machine (func_800788E0, its state in
 * D_800F11F0) plays the animations; firing unarmed (func_80038954) sets state 0x2F, which plays the punch
 * (g_Weapons[]+0x7C), lands the blow seven frames in (func_80038AA4: an invisible projectile from the camera with
 * the weapon's damage) and goes back to idle. The render step draws the object through its own view (D_800F11D0,
 * from the weapon's +0x64 offset and +0x6C angles, func_8007A040), with the weapon's projection (+0xAA).
 *
 * First-person models are loaded from the disc into one buffer (D_800E2F5C) when the weapon changes, and the
 * level's heap has no room for another, so the fists' model (<level>/FIST.HMD, in every level's table, also where
 * Bond has no fists to switch to) is loaded here once a mission into a buffer of this mod's (drawing animates it in
 * place: smooth motion keeps it as it keeps PS1 RAM), with an instance of its own from the game's two (D_800F0F80;
 * the weapon only ever has one, the old one freed before the new one is made). For a punch, the hand plays the
 * punch on that instance, as an object copied from the weapon's every frame (so lit as it is), lands the same blow
 * (the unarmed weapon's numbers lent to the weapon in hand for it: as_fists) and is drawn after the weapon through
 * the fists' view with x turned around: the right fist on the left, its triangles' winding turned too
 * (port_gte_mirror). An item held in the left hand (the disruptor, key cards) leaves the right to punch, as the
 * fists do. With the fists already up, it is the ordinary punch.
 *
 * MOD_REPLACES: func_800788E0 func_8007545C
 *
 * Names are LEVEL01's; port/mods.py maps them onto each level. */
#include "twine.h"
#include "gte.h"
#include <string.h>

extern u8 *D_8009CD98;      /* the weapon in Bond's hands (an object) */
extern s16 D_800B0DD4;      /* its slot in g_Weapons */
extern u32 D_800F11F0;      /* the weapon's state */
extern s32 D_800F11D0[8];   /* its view (GsRVIEW2) */
extern s32 D_800B0C10[];    /* the camera's view */
extern s32 g_CamPosX[3];
extern u8 *D_800AFFC8;      /* the objects */
extern void *g_Player;
extern s16 D_8009CF48;      /* frames since a shot */
extern s32 D_8009CD4C;      /* aiming */
extern s32 D_8009CA9C;
extern s32 D_8009CAA0;
extern u8 g_CutScene;
extern s32 D_800B8F24;
extern u8 D_800D2AFC[][0x14];
extern s32 D_800B0808[];
extern u8 D_800F0F80[];     /* the first-person models' instances (two, 0x94 bytes each) */
extern s32 D_800F0F5C;      /* a first-person model is being loaded */
extern char *D_8009DB50[];  /* the first-person models' files */
extern u8 *D_8009D0FC;      /* the first-person models' animation tables (0x70 bytes each) */
extern s32 D_8009CDA8;      /* the frame counter */
extern int port_gte_mirror;
int port_quick_melee(void);
void port_smooth_keep(void *at, u32 size);
void port_log(const char *fmt, ...);
int port_trace(const char *what);
extern int port_subframe;
static int trace_punch(void) {
    static int on = -1;
    if (on < 0)
        on = port_trace("punch");
    return on;
}

void port_heap3_own(int on, int fresh);
void func_800788E0_orig(void);
void func_8007545C_orig(s32 first, s32 last);
s32 func_8002C168(char *name, s32 buf);
void func_80075920(void *inst);
void func_80075E9C(void *inst);
void func_8007A040(s32 slot);
void func_800757C0(u8 *inst, s32 anim, s32 arg2);
void func_800758F0(u8 *coord);
void func_800755BC(u8 *inst);
void func_80026F30(s32 sound, s32 arg1);
void func_80038AA4(void);
void func_8007A17C(s32 state);
void func_8008D2EC(void *view);
void GsSetRefView2(void *view);
void GsGetLsUnit(void *, void *);
void GsGetLwUnit(void *, void *);
void GsSortUnit(void *, void *, void *);
void func_80077764(void *obj);
void func_8007A554(void *);
void quick_grenade(void);
void grenades_draw(void);
void func_8007A580(void *);

#define W(w, off, type) (*(type *)((u8 *)(w) + (off)))

static u8 *hand;          /* the left hand's object while it punches (record), else NULL */
static u8 record[0x124];  /* the hand's object: a copy of the weapon's with the hand's model */
static s32 view[8];       /* its view */
static int mirrored;      /* the left hand punches (the weapon is in the right) */
static int sided;         /* mirrored has been decided for this punch (at its first drawing) */
static u8 *fist;          /* the unarmed model, loaded for this mission (into fist_data) */
static u32 fist_data[0xE000 / 4]; /* as big as the game's buffer (the level's heap has no room for it) */
static u8 *fist_inst;     /* its instance, made once (the model is mapped where it lies) */
static s32 fist_frame;    /* the frame counter last frame: a smaller one is a new mission (a new heap) */

/* the unarmed weapon's numbers, the same in every mission that has it but for the damage (some have no unarmed
 * weapon: the fists are in their model tables all the same): the projection, offset and angles the fist is seen
 * with, its punch, and the blow's */
#define FIST_PROJECTION 1140
#define FIST_PUNCH 2
/* how long the blow flies (ticks; its speed is the +0xB2's): the game's 4 has Bond very close to punch, a little
 * further for the fists too (the unarmed weapon's own, every tick) */
#define FIST_REACH 6

/* the swing's sound, the punch's own in each level's state machine (the levels' sound tables differ) */
const u8 swing_sound[11] = {0x62, 0x76, 0x6F, 0x64, 0x5B, 0x71, 0x74, 0x6B, 0x6C, 0x71, 0x43};

static s16 fist_damage; /* the level's own unarmed weapon's (2000 in some, 700 in others), else 700 */

static void as_fists(Weapon *w) {
    W(w, 0x38, u32) = 0x08001FC0;
    W(w, 0x5A, s8) = -1;   /* no sound for the blow (the swing has one) */
    W(w, 0x5C, u32) = 0x64; /* no rumble */
    W(w, 0x64, s16) = 365;
    W(w, 0x66, s16) = 330;
    W(w, 0x68, s16) = 785;
    W(w, 0x6C, s16) = 0;
    W(w, 0x6E, s16) = -30;
    W(w, 0x70, s16) = 0;
    W(w, 0xAC, s16) = fist_damage;
    W(w, 0xB2, u16) = 1000;
    W(w, 0xB4, s32) = FIST_REACH;
}

/* the fists' damage: the level's unarmed weapon's, where it has one */
static void fists_reach(void) {
    s32 i;
    for (i = 0; i < 6; i++)
        if (W(&g_Weapons[i], 0xB4, s32) == 4 && !strcmp((char *)&g_Weapons[i], "UNARMED"))
            W(&g_Weapons[i], 0xB4, s32) = FIST_REACH;
}

static void find_fist_damage(void) {
    s32 i;
    fist_damage = 700;
    for (i = 0; i < 6; i++)
        if (!strcmp((char *)&g_Weapons[i], "UNARMED"))
            fist_damage = W(&g_Weapons[i], 0xAC, s16);
}

/* the weapon in hand is the fists */
static int fists_up(void) {
    return !strcmp((char *)&g_Weapons[D_800B0DD4], "UNARMED");
}

/* the fists' first-person model: <level>/FIST.HMD */
static s32 fist_model(void) {
    s32 i;
    for (i = 1; i < 32; i++) {
        u32 p = (u32)D_8009DB50[i];
        if (p >= 0x80000000u && p < 0x80200000u && strstr((char *)p, "/FIST.HMD"))
            return i;
    }
    return -1;
}

/* the fist's model and instance, as func_800752E8 makes the weapon's but from a model of its own: the weapon in
 * hand has the one buffer the game loads first-person models into (D_800E2F5C) */
static u8 *fist_ready(void) {
    s32 model = fist_model();
    u8 *p = D_800F0F80;
    s32 i, used;
    if (fist_inst && (fist_inst[0x92] != 1 || W(fist_inst, 0, u8 *) != fist))
        fist = fist_inst = hand = NULL;
    /* its animation table moved (a section loaded, the level's tables made again): the instance made again */
    if (fist_inst && model >= 0 && (W(fist_inst, 0x74, s32) != W(D_8009D0FC + model * 0x70, 0x68, s32) ||
                                    W(fist_inst, 0x90, u16) != W(D_8009D0FC + model * 0x70, 0x6C, u16))) {
        port_log("quick punch: the fists' animations moved (%08x to %08x): made again\n", W(fist_inst, 0x74, s32),
                 W(D_8009D0FC + model * 0x70, 0x68, s32));
        fist_inst[0x92] = 0;
        fist_inst = hand = NULL;
    }
    if (fist_inst)
        return fist_inst;
    if (model < 0)
        return NULL;
    if (!fist) {
        if (D_800F0F5C != 0) /* the game is loading the weapon's */
            return NULL;
        if (func_8002C168(D_8009DB50[model], (s32)fist_data) <= 0)
            return NULL;
        /* drawing animates it: smooth motion's extra drawings put it back after them */
        port_smooth_keep(fist_data, sizeof(fist_data));
        fist = (u8 *)fist_data;
    }
    for (i = 0; i < 2; i++, p += 0x94) {
        if (p[0x92] == 0) {
            p[0x92] = 1;
            W(p, 0, u8 *) = fist;
            W(p, 0x90, u16) = W(D_8009D0FC + model * 0x70, 0x6C, u16);
            W(p, 0x74, s32) = W(D_8009D0FC + model * 0x70, 0x68, s32);
            /* its parts' and animation's memory from the runtime's own heap: the level's has no room to spare
             * (and the heap's count in g_Args left as it was) */
            used = W(g_Args, 0x1F4, s32);
            port_heap3_own(1, 1);
            func_80075920(p);
            port_heap3_own(0, 0);
            W(g_Args, 0x1F4, s32) = used;
            func_80075E9C(p);
            return fist_inst = p;
        }
    }
    return NULL;
}

static void hand_away(void) {
    hand = NULL;
}

/* the hand's view: the fists', from the hand's coordinate system (func_8007A040 makes it from the weapon in hand's
 * numbers, lent the fists' meanwhile) */
static void hand_view(void) {
    Weapon held = g_Weapons[D_800B0DD4];
    s32 kept[8];
    s32 i;
    for (i = 0; i < 8; i++)
        kept[i] = D_800F11D0[i];
    as_fists(&g_Weapons[D_800B0DD4]);
    func_8007A040(D_800B0DD4);
    g_Weapons[D_800B0DD4] = held;
    for (i = 0; i < 8; i++) {
        view[i] = D_800F11D0[i];
        D_800F11D0[i] = kept[i];
    }
    view[7] = (s32)(W(hand, 0x114, u8 *) + 0x14);
}

static void punch(void) {
    u8 *inst = fist_ready();
    find_fist_damage();
    if (!inst)
        return;
    memcpy(record, D_8009CD98, sizeof(record));
    W(record, 0x114, u8 *) = inst;
    hand = record;
    sided = 0;
    hand_view();
    func_800757C0(inst, FIST_PUNCH, 1);
    port_log("quick punch: frame %d level %d: %d frames to play (animations %08x)\n", D_8009CDA8, g_Args->level,
             W(inst, 0x80, s32), W(inst, 0x74, s32));
}

/* the blow: the punch's own (func_80038AA4), from the hand as the weapon in hand with the fists' numbers, the
 * weapon's state and muzzle flash left as they were */
static void blow(void) {
    u8 *weapon = D_8009CD98;
    Weapon held = g_Weapons[D_800B0DD4];
    s16 shot = D_8009CF48;
    u32 state = D_800F11F0;
    D_8009CD98 = hand;
    as_fists(&g_Weapons[D_800B0DD4]);
    func_80038AA4();
    g_Weapons[D_800B0DD4] = held;
    D_8009CD98 = weapon;
    D_8009CF48 = shot;
    D_800F11F0 = state;
}

void func_800788E0(void) {
    int pressed = port_quick_melee();
    u8 *inst, *coord;
    s32 left;
    /* a new mission (or a room of endless mode's): the model and instance were in the last one's memory. Watched
     * every frame, not only as he punches: a first punch late in the next mission would miss the count going back */
    if (D_8009CDA8 < fist_frame) {
        if (fist)
            port_log("quick punch: a new mission (frame %d after %d): the fists loaded again\n", D_8009CDA8, fist_frame);
        fist = fist_inst = hand = NULL;
    }
    fist_frame = D_8009CDA8;
    fists_reach();
    func_800788E0_orig();
#ifdef MOD_QUICKGRENADE
    if (D_8009CD98 && g_CutScene == 0)
        quick_grenade();
#endif
    /* not in a cut scene, nor aiming (through a scope, a camera, or down the sights: the view zoomed) */
    if (!D_8009CD98 || g_CutScene != 0 || D_8009CD4C != 0) {
        hand_away();
        return;
    }
    /* nor with the grappling hook watch on the wrist (its hand is the one that would punch) */
    if (!strcmp((char *)&g_Weapons[D_800B0DD4], "GRAPPLING HOOK WATCH")) {
        hand_away();
        return;
    }
    if (pressed) {
        if (fists_up()) {
            /* as fire does, where it can (idle, or the blow landed) */
            if (D_800F11F0 == 1 || D_800F11F0 == 2 || D_800F11F0 == 8 || D_800F11F0 == 9)
                func_8007A17C(0x2F);
        } else if (!hand)
            punch();
        else {
            /* as the fists do: again once the blow has landed */
            inst = W(hand, 0x114, u8 *);
            if (W(inst, 0x74, u8 *)[W(inst, 0x78, s32)] - W(inst, 0x80, s32) > 7)
                func_800757C0(inst, FIST_PUNCH, 1);
        }
    }
    if (!hand)
        return;
    inst = W(hand, 0x114, u8 *);
    /* lit as the weapon is */
    memcpy(record, D_8009CD98, sizeof(record));
    W(record, 0x114, u8 *) = inst;
    /* as the weapon's: at the camera, square to the world (the view does the rest) */
    coord = inst + 0x14;
    W(coord, 0x18, s32) = g_CamPosX[0];
    W(coord, 0x1C, s32) = g_CamPosX[1];
    W(coord, 0x20, s32) = g_CamPosX[2];
    W(coord, 0x44, s16) = W(coord, 0x46, s16) = W(coord, 0x48, s16) = 0;
    func_800758F0(coord);
    left = W(inst, 0x74, u8 *)[W(inst, 0x78, s32)] - W(inst, 0x80, s32);
    if (trace_punch()) { /* (each frame of a punch: its frames left, its first track's playhead) */
        s32 tracks = W(inst, 0x6C, s32);
        u8 *tr = tracks > 0 ? W(inst, 0x70, u8 **)[0] : NULL;
        port_log("trace: L%d f%d punch: anim %d left %d, track playhead %d len %d speed %d, %s hand\n", g_Args->level,
                 D_8009CDA8, W(inst, 0x78, s32), W(inst, 0x80, s32), tr ? W(tr, 0x16, u16) : -1, tr ? tr[0xC] : -1,
                 tr ? (s8)tr[0xD] : -1, !sided ? "undecided" : mirrored ? "left" : "right");
    }
    if (W(inst, 0x80, s32) == 0x14)
        if ((u32)(g_Args->level - 1) < 11)
            func_80026F30(swing_sound[g_Args->level - 1], 0);
    if (left == 7)
        blow();
    else if (W(inst, 0x80, s32) < 2) {
        hand_away();
        return;
    }
    func_800755BC(inst);
}

/* Which hand holds the weapon: where its parts (the hand, the arm, the thing held) are seen, right or left of the
 * middle, in its own view (set). Its numbers don't say: the flash bang and the disruptor are held in the left. */
static s32 held_right_sum; /* (the trace's: how far right the weapon's parts are seen) */
static int held_right(void) {
    u8 *model = W(D_8009CD98, 0x114, u8 *);
    s32 *unit = W(model, 4, s32 *);
    s32 right = 0;
    u32 i;
    for (i = 0; i < W(model, 0xC, u32); i++, unit += 2) {
        if (unit[0] == 0 || unit[1] == 0)
            continue;
        GsGetLsUnit((void *)unit[0], D_800B0808);
        if (D_800B0808[7] > 0)
            right += D_800B0808[5] * 256 / D_800B0808[7];
    }
    held_right_sum = right;
    return right > 0;
}

/* the hand's parts as func_8007567C draws an object's, for the left hand with x turned around */
static void draw_hand(void) {
    u8 *model = W(hand, 0x114, u8 *);
    s32 *unit = W(model, 4, s32 *);
    u8 *flags = W(model, 8, u8 *);
    u32 i;
    func_80077764(hand);
    port_gte_mirror = mirrored;
    for (i = 0; i < W(model, 0xC, u32); i++, unit += 2, flags++) {
        if (unit[1] == 0 || (i == 0 && (D_8009CA9C != 0 || D_8009CAA0 != 0)))
            continue;
        if (unit[0] != 0) {
            s16 *m = (s16 *)D_800B0808;
            if (*flags == 1)
                continue;
            GsGetLwUnit((void *)unit[0], D_800B0808);
            func_8007A580(D_800B0808);
            GsGetLsUnit((void *)unit[0], D_800B0808);
            if (mirrored) {
                m[0] = -m[0];
                m[1] = -m[1];
                m[2] = -m[2];
                D_800B0808[5] = -D_800B0808[5];
            }
            func_8007A554(D_800B0808);
        }
        GsSortUnit(unit, D_800D2AFC[D_800B8F24], (void *)0x1F800000);
    }
    port_gte_mirror = 0;
}

void func_8007545C(s32 first, s32 last) {
    long h;
    s32 at;
    func_8007545C_orig(first, last);
#ifdef MOD_QUICKGRENADE
    grenades_draw();
#endif
    /* with the weapon, when it is drawn */
    if (!hand || !D_8009CD98 || !D_8009CD98[4] || W(D_8009CD98, 0x7C, s32) < 0)
        return;
    at = (D_8009CD98 - D_800AFFC8) / 0x124;
    if (at < first || at > last)
        return;
    gte_ReadGeomScreen(&h);
    if (!sided) {
        /* the free hand punches */
        func_8008D2EC(D_800F11D0);
        mirrored = held_right();
        sided = 1;
        if (trace_punch())
            port_log("trace: L%d f%d punch: the %s hand punches (the weapon's parts seen %d right of the middle; %s%s)\n",
                     g_Args->level, D_8009CDA8, mirrored ? "left" : "right", held_right_sum,
                     (char *)&g_Weapons[D_800B0DD4], port_subframe ? ", decided in an extra drawing" : "");
    }
    func_8008D2EC(view);
    gte_SetGeomScreen(FIST_PROJECTION);
    draw_hand();
    GsSetRefView2(D_800B0C10);
    gte_SetGeomScreen((u16)h);
}
