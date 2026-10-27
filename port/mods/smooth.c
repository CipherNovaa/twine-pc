/* Smooth motion at the display's frame rate (native PC build only). The game moves in 20 Hz ticks and draws
 * once per tick; while it waits for the next tick, the runtime has its render step draw the frame again as
 * often as there is time for (port/src/interp.c), with the camera and everything that moves between where they
 * were at the last two ticks, and the camera turned by mouse movement the game hasn't taken yet. Those drawings
 * change nothing (the runtime puts the machine back after each); the tick's own drawing comes last, as always.
 *
 * The camera is a coordinate system (g_Camera+0x30) placed at g_Camera.pos and turned by the angles at
 * g_Camera+0x80 (pitch, yaw, roll); the view (D_800B0C10) looks along its z axis. A drawing in between rebuilds
 * the camera's matrices from interpolated angles and position as the camera update does (func_8004D978).
 *
 * Objects (characters, props, vehicles, casings, projectiles...) are 0x124-byte records at D_800AFFC8,
 * D_800B0AA8 of them: +4 in use, +0x2C the tick it was made in, +0x90 its parts, +0xB how many. A part is 0x8C
 * bytes: a GsDOBJ2, then its coordinate system (GsCOORDINATE2) at +0x10 with the matrix at +0x14 and the
 * position at +0x28, then the angles the game builds the matrix from at +0x60. The first part places the
 * object in the world, the others hang from it (or from each other). The render step draws a part through its
 * coordinate system (func_800785A8: GsGetLs). Characters are drawn instead from a copy of their first part's
 * world matrix, taken during the tick (func_8005BA30), one per character slot (D_800DE004, owner D_800DDEF0).
 * Effects made of a model (muzzle flashes: 0x34-byte records at D_800B0AB4, D_800B0C84 of them; +1 in use,
 * +0x18 the tick it was made in, +0x1C its one part) are drawn like a part (func_800781B4).
 * Each tick, the pose (matrix and position) of all of these is kept; a drawing in between puts each one that
 * was there at the last tick too between its two poses: positions on a straight line, the matrix's axes turned
 * part of the way (scaled back to their length). One that jumped, turned a lot or is not the same thing anymore
 * (the slot freed and taken again) stays where the tick has it. The characters' animations and the sprites
 * (smoke, fire) go between their last two ticks the same way (Anim, Sprite below).
 *
 * MOD_REPLACES: func_8002F7CC
 *
 * Names are LEVEL01's; port/mods.py maps them onto each level. */
#include "twine.h"

extern s32 D_800B0C10[]; /* the view (GsRVIEW2): viewpoint, reference point, roll, coordinate system */
extern s32 D_800D3238[]; /* the world's coordinate system */
extern s32 D_800B0808[]; /* its local-screen matrix */
extern s32 D_8009CD30;   /* the view's heading */
extern s32 D_8009CD00;
extern u8 *D_800AFFC8;   /* the objects */
extern s32 D_800B0AA8;   /* how many */
extern u8 D_800DE004[];  /* the world matrix of the character in each slot, as drawn */
extern u8 *D_800DDEF0[]; /* the character in each slot */
extern u8 *D_800B0AB4;   /* the model effects */
extern s16 D_800B0C84;   /* how many */
extern u8 D_800DB884[];  /* the sprites */
void func_8002F7CC_orig(void);
void func_8003BC98(void *rot, void *coord, s32 part);
void GsSetRefView2(void *view);
void GsGetLs(void *coord, void *m);
s32 ratan2(s32 y, s32 x);
int port_smooth_begin(s32 *t, s32 *yaw, s32 *pitch, int room);
int port_mouse_recent(void);
void port_smooth_end(void);
void port_smooth_present(void);
extern int port_shown_yaw;

#define CAM_ROT ((s16 *)((u8 *)&g_Camera + 0x80))
#define CAM_POS (&g_Camera.posX)

typedef struct {
    s16 rot[3];
    s32 pos[3];
} View;

static View s_Prev;
static s32 s_HavePrev = 0;

/* signed difference of two angles (4096 = 360 degrees) */
static s32 angle_diff(s32 a, s32 b) {
    s32 d = (b - a) & 0xFFF;
    return d >= 0x800 ? d - 0x1000 : d;
}

/* What moves. Every pose is the matrix of a coordinate system (a MATRIX: 3x3 s16, then the position, 3 s32),
 * kept with where it lives and what it belongs to. */
#define MAX_POSES 2048
#define CHAR_SLOTS 24 /* (the game's 6; more with port/mods/people.c's slots: people_slots()) */
extern u32 port_ram_size;
#define IN_RAM(p) ((u32)(p) - 0x80000000u < port_ram_size)
int people_slots(void) __attribute__((weak));
u8 **people_owner(int i) __attribute__((weak));
u8 *people_matrix(int i) __attribute__((weak));
static int char_slots(void) {
    int n = people_slots ? people_slots() : 6;
    return n < CHAR_SLOTS ? n : CHAR_SLOTS;
}

typedef struct {
    s16 m[9];
    s32 t[3];
} Pose;

typedef struct {
    u8 *at;    /* the matrix in the game's memory */
    u8 *owner; /* the object (or effect, character) it belongs to */
    s32 made;  /* the tick that was made in */
    u8 *parts; /* its parts, and how many (their coordinate systems are recomputed when one moves) */
    s32 nparts;
    Pose pose;
} Moving;

/* A character's animation, as its instance (obj+0xC0, one per character slot) has it for the drawing
 * (func_8005B96C): two frames (+0x24/+0x28 the animations' frames, +0x2C/+0x30 which) and how far it is from
 * the first to the second (+0x44, 0-4096), each tick a step further; past the second, the second is the first
 * of the next pair (func_8005C8C4). The skinning works out the bones from them at drawing time. */
typedef struct {
    u8 *inst;
    u8 *owner;
    s32 made;
    s32 w[9]; /* +0x24..+0x44 */
} Anim;

#define A_ANIM0 0
#define A_ANIM1 1
#define A_FRAME0 2
#define A_FRAME1 3
#define A_BLEND 8

/* A sprite (smoke, fire, flashes: 30 records of 0xA0 bytes at D_800DB884; +0x9B in use, +0x94 the tick it was
 * made in, +0x50 what it is) is placed on the screen at drawing time (func_80062394) from its position (+0x54,
 * 3 s16), size (+0x78, +0x7A) and turn (+0x9C); the render step then moves it on (func_80063B20). */
#define SPRITES 30

typedef struct {
    s32 made;
    u8 *what;
    s16 pos[3];
    s16 size[2];
    s16 turn;
} Sprite;

typedef struct {
    s32 n;
    Moving m[MAX_POSES];
    s16 find[MAX_POSES * 2]; /* by where the matrix lives (hashed): which one, +1 */
    Anim anim[CHAR_SLOTS];
    Sprite sprite[SPRITES]; /* `what` 0: none */
} Poses;

static Poses s_Poses[2];
static Poses *s_Old = &s_Poses[0], *s_New = &s_Poses[1];

static u32 hash(const u8 *at) {
    return ((u32)at >> 2) * 2654435761u >> 20 & (MAX_POSES * 2 - 1);
}

static void keep(Poses *p, u8 *at, u8 *owner, s32 made, u8 *parts, s32 nparts) {
    Moving *m;
    u32 h;
    s32 i;

    if (p->n >= MAX_POSES)
        return;
    for (h = hash(at); p->find[h] != 0; h = (h + 1) & (MAX_POSES * 2 - 1))
        ;
    p->find[h] = p->n + 1;
    m = &p->m[p->n++];
    m->at = at;
    m->owner = owner;
    m->made = made;
    m->parts = parts;
    m->nparts = nparts;
    for (i = 0; i < 9; i++)
        m->pose.m[i] = ((s16 *)at)[i];
    for (i = 0; i < 3; i++)
        m->pose.t[i] = ((s32 *)(at + 0x14))[i];
}

/* This tick's poses: every part of every object, the characters as drawn, the model effects. */
static void keep_poses(Poses *p) {
    s32 i, k, n;
    u8 *obj, *parts;

    p->n = 0;
    for (i = 0; i < MAX_POSES * 2; i++)
        p->find[i] = 0;
    if (D_800AFFC8 != NULL) {
        for (i = 0; i < D_800B0AA8; i++) {
            obj = D_800AFFC8 + i * 0x124;
            parts = *(u8 **)(obj + 0x90);
            if (obj[4] == 0 || parts == NULL)
                continue;
            n = obj[0xB];
            for (k = 0; k < n; k++)
                keep(p, parts + k * 0x8C + 0x14, obj, *(s32 *)(obj + 0x2C), parts, n);
        }
        /* (a free slot can keep its last owner) */
        for (i = 0; i < CHAR_SLOTS; i++)
            p->anim[i].inst = NULL;
        for (i = 0; i < char_slots(); i++) {
            obj = people_owner ? *people_owner(i) : D_800DDEF0[i];
            if (!IN_RAM(obj) || obj[4] == 0)
                continue;
            keep(p, people_matrix ? people_matrix(i) : D_800DE004 + i * 0x20, obj, *(s32 *)(obj + 0x2C), NULL, 0);
            if (IN_RAM(*(u8 **)(obj + 0xC0))) {
                p->anim[i].inst = *(u8 **)(obj + 0xC0);
                p->anim[i].owner = obj;
                p->anim[i].made = *(s32 *)(obj + 0x2C);
                for (k = 0; k < 9; k++)
                    p->anim[i].w[k] = ((s32 *)(p->anim[i].inst + 0x24))[k];
            }
        }
    }
    if (D_800B0AB4 != NULL) {
        for (i = 0; i < D_800B0C84; i++) {
            obj = D_800B0AB4 + i * 0x34;
            parts = *(u8 **)(obj + 0x1C);
            if (obj[1] != 0 && parts != NULL)
                keep(p, parts + 0x14, obj, *(s32 *)(obj + 0x18), parts, 1);
        }
    }
    for (i = 0; i < SPRITES; i++) {
        obj = D_800DB884 + i * 0xA0;
        p->sprite[i].what = obj[0x9B] != 0 ? *(u8 **)(obj + 0x50) : NULL;
        p->sprite[i].made = *(s32 *)(obj + 0x94);
        for (k = 0; k < 3; k++)
            p->sprite[i].pos[k] = ((s16 *)(obj + 0x54))[k];
        p->sprite[i].size[0] = *(s16 *)(obj + 0x78);
        p->sprite[i].size[1] = *(s16 *)(obj + 0x7A);
        p->sprite[i].turn = *(s16 *)(obj + 0x9C);
    }
}

static u32 isqrt(u32 v) {
    u32 r = 0, b = 1u << 30;

    while (b > v)
        b >>= 2;
    while (b != 0) {
        if (v >= r + b) {
            v -= r + b;
            r = (r >> 1) + b;
        } else {
            r >>= 1;
        }
        b >>= 2;
    }
    return r;
}

/* the lengths of a matrix's axes (its columns) */
static void axis_lengths(const s16 *m, s32 *len) {
    s32 i, j;
    u32 l;

    for (j = 0; j < 3; j++) {
        l = 0;
        for (i = 0; i < 3; i++)
            l += m[i * 3 + j] * m[i * 3 + j];
        len[j] = isqrt(l);
    }
}

/* `t` (0-4096) of the way from pose a to pose b, into the matrix at `at`; 0 when it jumped (it stays b). */
static s32 blend(const Pose *a, const Pose *b, s32 t, u8 *at) {
    s16 *m = (s16 *)at;
    s32 *pos = (s32 *)(at + 0x14);
    s32 i, j, l, la[3], lb[3], v[3];
    s32 dot;
    u32 lv;

    for (i = 0; i < 3; i++) {
        if (abs(b->t[i] - a->t[i]) > 0x1000)
            return 0;
    }
    for (i = 0; i < 3; i++)
        pos[i] = a->t[i] + (b->t[i] - a->t[i]) * t / 4096;
    for (i = 0; i < 9; i++) {
        if (a->m[i] != b->m[i])
            break;
    }
    if (i == 9)
        return 1;
    /* Each axis turns from a's to b's, and is kept as long as they are; not when one turns by more than 60
     * degrees in a tick (spinning). */
    axis_lengths(a->m, la);
    axis_lengths(b->m, lb);
    for (j = 0; j < 3; j++) {
        dot = 0;
        for (i = 0; i < 3; i++)
            dot += a->m[i * 3 + j] * b->m[i * 3 + j] >> 4;
        if (dot < (la[j] >> 2) * (lb[j] >> 2) / 2)
            return 1;
    }
    for (j = 0; j < 3; j++) {
        lv = 0;
        for (i = 0; i < 3; i++) {
            v[i] = a->m[i * 3 + j] + (b->m[i * 3 + j] - a->m[i * 3 + j]) * t / 4096;
            lv += v[i] * v[i];
        }
        lv = isqrt(lv);
        l = la[j] + (lb[j] - la[j]) * t / 4096;
        for (i = 0; i < 3; i++)
            m[i * 3 + j] = lv != 0 ? v[i] * l / (s32)lv : v[i];
    }
    return 1;
}

/* A character's animation `t` of the way from a to b: within one pair of frames, or into the next pair. Only
 * when both play one animation (not changing from one to another) and the same one. */
static void blend_anim(const Anim *a, const Anim *b, s32 t) {
    s32 *w = (s32 *)(b->inst + 0x24);
    s32 k, s;

    if (a->inst != b->inst || a->owner != b->owner || a->made != b->made)
        return;
    if (a->w[A_ANIM0] != a->w[A_ANIM1] || b->w[A_ANIM0] != b->w[A_ANIM1] || a->w[A_ANIM0] != b->w[A_ANIM0])
        return;
    for (k = 4; k < 8; k++) {
        if (a->w[k] != b->w[k])
            return;
    }
    if (a->w[A_FRAME0] == b->w[A_FRAME0] && a->w[A_FRAME1] == b->w[A_FRAME1] && a->w[A_BLEND] <= b->w[A_BLEND]) {
        w[A_BLEND] = a->w[A_BLEND] + (b->w[A_BLEND] - a->w[A_BLEND]) * t / 4096;
    } else if (a->w[A_FRAME1] == b->w[A_FRAME0]) {
        /* (a one-frame loop goes from its frame to itself) */
        s = a->w[A_BLEND] + (4096 - a->w[A_BLEND] + b->w[A_BLEND]) * t / 4096;
        if (s < 4096) {
            w[A_FRAME0] = a->w[A_FRAME0];
            w[A_FRAME1] = a->w[A_FRAME1];
            w[A_BLEND] = s;
        } else {
            w[A_BLEND] = s - 4096;
        }
    }
}

/* A sprite `t` of the way from a to b, into the record at p. */
static void blend_sprite(const Sprite *a, const Sprite *b, s32 t, u8 *p) {
    s32 k;

    if (a->what != b->what || a->made != b->made)
        return;
    for (k = 0; k < 3; k++) {
        if (abs(b->pos[k] - a->pos[k]) > 0x1000)
            return;
    }
    for (k = 0; k < 3; k++)
        ((s16 *)(p + 0x54))[k] = a->pos[k] + (b->pos[k] - a->pos[k]) * t / 4096;
    *(s16 *)(p + 0x78) = a->size[0] + (b->size[0] - a->size[0]) * t / 4096;
    *(s16 *)(p + 0x7A) = a->size[1] + (b->size[1] - a->size[1]) * t / 4096;
    /* (0: not turned, drawn upright) */
    if (a->turn != 0 && b->turn != 0)
        *(s16 *)(p + 0x9C) = a->turn + angle_diff(a->turn, b->turn) * t / 4096;
}

/* Everything that was there at the last tick too (the same thing), `t` of the way from there to here. */
static void place_between(s32 t) {
    const Moving *a, *b;
    u32 h;
    s32 k;

    for (k = 0; k < SPRITES; k++) {
        if (s_Old->sprite[k].what != NULL)
            blend_sprite(&s_Old->sprite[k], &s_New->sprite[k], t, D_800DB884 + k * 0xA0);
    }
    for (k = 0; k < CHAR_SLOTS; k++) {
        if (s_Old->anim[k].inst != NULL && s_New->anim[k].inst != NULL)
            blend_anim(&s_Old->anim[k], &s_New->anim[k], t);
    }
    for (b = s_New->m; b < s_New->m + s_New->n; b++) {
        for (h = hash(b->at); s_Old->find[h] != 0; h = (h + 1) & (MAX_POSES * 2 - 1)) {
            a = &s_Old->m[s_Old->find[h] - 1];
            if (a->at == b->at)
                break;
        }
        if (s_Old->find[h] == 0 || a->owner != b->owner || a->made != b->made || a->parts != b->parts ||
            a->nparts != b->nparts)
            continue;
        if (!blend(&a->pose, &b->pose, t, b->at))
            continue;
        /* its parts' world matrices are worked out again (flag 0: changed) */
        for (k = 0; k < b->nparts; k++)
            *(s32 *)(b->parts + k * 0x8C + 0x10) = 0;
    }
}

/* One drawing between ticks, `t` (0-4096) of the way from the last tick's camera to this one's. While the
 * mouse is in use, the camera's angles are this tick's turned by what the mouse has moved since the game took
 * it, rather than interpolated: looking around then lags nothing. */
static void draw_between(const View *cur, s32 t, s32 yaw, s32 pitch, s32 mouse) {
    s32 p0, p, i;

    for (i = 0; i < 3; i++) {
        CAM_POS[i] = s_Prev.pos[i] + (cur->pos[i] - s_Prev.pos[i]) * t / 4096;
        CAM_ROT[i] = mouse ? cur->rot[i] : s_Prev.rot[i] + angle_diff(s_Prev.rot[i], cur->rot[i]) * t / 4096;
    }
    /* the mouse turns the player (and the camera with it) at the next tick (twinstick.c) */
    if (mouse) {
        CAM_ROT[1] += yaw;
        /* within the game's limits for looking up and down */
        p0 = ((CAM_ROT[0] & 0xFFF) ^ 0x800) - 0x800;
        p = p0 + pitch;
        if (pitch > 0 && p > 0x320) {
            p = p0 > 0x320 ? p0 : 0x320;
        } else if (pitch < 0 && p < -0x280) {
            p = p0 < -0x280 ? p0 : -0x280;
        }
        CAM_ROT[0] = p;
    }
    func_8003BC98(CAM_ROT, (u8 *)&g_Camera + 0x30, 0);
    GsSetRefView2(D_800B0C10);
    GsGetLs(D_800D3238, D_800B0808);
    gte_SetRotMatrix(D_800B0808);
    gte_SetTransMatrix(D_800B0808);
    D_8009CD30 = (CAM_ROT[1] + ratan2(D_800B0C10[3] - D_800B0C10[0], D_800B0C10[5] - D_800B0C10[2])) & 0xFFF;
    if (t < 4096)
        place_between(t);
    port_shown_yaw = CAM_ROT[1];
    func_8002F7CC_orig();
    port_smooth_end();
}

void func_8002F7CC(void) {
    View cur;
    Poses *p;
    s32 t, yaw, pitch, mouse, i;

    for (i = 0; i < 3; i++) {
        cur.rot[i] = CAM_ROT[i];
        cur.pos[i] = CAM_POS[i];
    }
    /* a cut, not motion: nothing in between */
    for (i = 0; i < 3; i++) {
        if (!s_HavePrev || abs(cur.pos[i] - s_Prev.pos[i]) > 0x800 || abs(angle_diff(s_Prev.rot[i], cur.rot[i])) > 0x400) {
            s_Prev = cur;
            break;
        }
    }
    keep_poses(s_New);
    mouse = port_mouse_recent() && g_CutScene == 0 && D_8009CD00 == 0;
    /* the drawings in between, leaving time for the tick's own frame (and with the mouse, its turned copy) */
    while (port_smooth_begin(&t, &yaw, &pitch, mouse ? 3 : 2)) {
        draw_between(&cur, t, yaw, pitch, mouse);
    }
    s_Prev = cur;
    s_HavePrev = 1;
    port_shown_yaw = CAM_ROT[1];
    func_8002F7CC_orig();
    /* The tick's own frame. With the mouse in use it lags the drawings before it, which were turned by what
     * the mouse has moved since the game took it (showing it would step back 20 times a second): one turned
     * the same way is drawn and shown instead, whatever the time, so that the display gets a frame here (the
     * next comes only after the game's next tick; left out, it often missed a refresh while turning). */
    if (mouse && port_smooth_begin(&t, &yaw, &pitch, -1)) {
        draw_between(&cur, 4096, yaw, pitch, mouse);
    } else {
        port_smooth_present();
    }
    p = s_Old;
    s_Old = s_New;
    s_New = p;
}
