/* Endless mode (native PC build only): the level's part of the run (port/src/endless.c keeps the run and starts
 * each room's mission at the room's point).
 *
 * A room is a section of the mission, as a fight. Its own guards stay, but for the mission's story people: anyone of
 * a kind the mission places only once (an ally, a target, a boss) or not of the guards' sort (civilians, the one to
 * protect or to chase, the one who runs away) is taken out of the section, record and all. More guards come on top: a director spends a budget that grows with the time spent
 * in the room and with the wave (the rooms cleared, plus one), sending copies of the mission's guards (made the way
 * the section makes its own, from a record: func_8003C1BC; the record is the copy's, kept here). They come to places
 * on the floor Bond can walk to from where he is without going through a door (the section's floor polygons, flooded
 * from his, the doorways stopping it), out of his sight: far off, and further still in front of him, and further from
 * where he came in than he is. When there is no such place, they come in at a doorway. They come one by one and from
 * places apart; one left far off for long is sent back, and comes again nearer. A room with few guards of its own gets
 * some at once.
 *
 * The room's doors stay shut until the fight is over: its own guards down, or enough kills, or after a while. Then
 * any door to another section (one the mission asks the map to load: D_800D2F90 2, through the door's load point)
 * leaves the room, but for the one Bond came in by. The mission's main then returns (g_Args' flags 0x40, which its
 * frame loop's top checks) and the next room's mission starts in its place (main.c, or the boot stub), with Bond's
 * weapons, ammo, health and armour carried across: each weapon by its slot's name, which is the same in every
 * mission that has it. In a mission that flows (King's Ransom), the door's own section comes next, loaded the
 * mission's way, for a stretch of a few rooms.
 *
 * The mission's own story is left out: its objectives neither complete it (func_8002E1B0) nor fail it (func_8002D4E4,
 * but for Bond's death, GAME OVER), and their messages aren't shown (func_80048B4C); its cutscenes don't play
 * (func_80054414), nor its loading picture between rooms (func_8002E318). When Bond dies the mission ends as when
 * RESTART? is answered NO, without asking, and the front end's mission stats show the run (port/src/endless.c,
 * fixes/shell.c).
 *
 * Everything that happens is logged ("endless:" lines, port_endless_logf): rooms, spawns, pickups made and taken,
 * kills, damage, doors, cutscenes and messages skipped.
 *
 * endless_frame is called at the top of every frame of the mission (mods/savestate.c's Pad_Update).
 * TWINE_ENDLESS_DUMP=1 also logs the section's doors, its kinds of placed things and its people.
 *
 * MOD_REPLACES_NATIVE: func_8002D4E4 func_8002E1B0 RestartPrompt_Open func_8004CA5C func_8004B270 func_80054414 func_8002E318 func_80048B4C func_80055E24 func_8005E020
 *
 * Names are LEVEL01's; port/mods.py maps them onto each level. */
#include "twine.h"
#include "../src/endless.h"

char *getenv(const char *name);
int snprintf(char *s, unsigned n, const char *fmt, ...);
int atoi(const char *s);
void *memcpy(void *d, const void *s, unsigned n);
void *memset(void *d, int c, unsigned n);
char *strncpy(char *d, const char *s, unsigned n);
int strncmp(const char *a, const char *b, unsigned n);
char *strchr(const char *s, int c);
char *strstr(const char *s, const char *t);
void port_endless_logf(s32 frame, const char *fmt, ...);
extern int port_subframe; /* (interp.c: an extra drawing between ticks runs, whose memory is put back after) */
#define ELOG(...) port_endless_logf(D_8009CDA8, __VA_ARGS__)

extern s32 D_800D2F90;   /* the map's loading: 0 none, 2 a load point's section asked for, ... */
extern s32 D_800D2F8C;   /* the section loaded */
extern s16 D_8009E9C4[]; /* the start point (section, yaw, x, y, z); +0x10 the load point to go to; load points from +0x18 */
extern s32 D_8009CDA8;   /* the mission's frame counter */
extern s16 D_8009CF68;   /* the section's lists of placed things: how many lists */
extern s32 **D_8009CF70; /* each list (6 words a thing: x, y, z from the section's origin, flags, w4, w5) */
extern s32 *D_8009CF6C;  /* each list's count */
extern s32 D_800D2FB0[]; /* the section's origin */
extern s32 D_8009CFF0;   /* Bond's armour (0x20000 full) */
extern s32 D_8009CFFC;   /* the fade: 1 out, -1 in */
extern s32 D_800AFFC8;   /* the objects (0x124 bytes each) */
extern s16 D_800B0DA0, D_800B07B0; /* the first and last of the people among them */
extern s16 D_800B0DD4;   /* the weapon in hand (its slot in g_Weapons) */
extern s16 D_800B0DDC;   /* the weapon to change to (the one in hand when none is changing) */
extern s16 D_8009D104, D_8009D106; /* the object types of the light and the heavy armour */
extern s16 D_8009D108;   /* ... of the ammo box (its +0x34: whose ammo, an entry of D_8009D130) */
extern s32 D_8009D130[]; /* the weapons' pickups, 19 of 4 words: weapon slot, object type, messages */
extern s32 D_800E2F54;   /* the section's floor (its contour polygons) */
extern s16 D_8009CAA4;   /* a menu over the mission (its frame loop runs the menu alone) */
extern s32 g_DevMenu[];
extern u32 g_PadPressed; /* the buttons pressed this frame */
u8 *func_8003C1BC(s32 *rec, s32 x, s32 y, s32 z); /* makes a placed thing from its record */
u8 *func_80039A9C(s32 arg0, u8 type, s32 kind);  /* makes an object of a kind (4: a pickup) at MAKE_AT */
void func_800395E8(u8 *o);                       /* takes an object out of the game */
void func_8005E020(u8 *o);                       /* a person hit: hurt, or falls */
void func_8005E020_orig(u8 *o);
void func_80030FA8(s16 *at);                     /* puts Bond at a point (section, yaw, x, y, z) */
s32 func_800705A8(s32 floor, s16 *pos, s32 arg2, s32 arg3); /* the floor's height under pos, -0x7FFF none */
#define MAKE_AT ((s32 *)0x1F8003D4u)             /* (the scratchpad's x, y, z) */
s32 func_8002D4E4_orig(s32 text);
s32 func_8002E1B0_orig(void);
void RestartPrompt_Open_orig(void);
s32 func_8004CA5C_orig(u8 *o, u8 *who);
void func_8004B270_orig(s32 damage, s32 through_armour, s32 always);
s32 func_80054414_orig(u8 nis, u8 arg1, u8 arg2);
void func_8002E318_orig(void);
s32 func_80048B4C_orig(char *text, u8 prio, u8 flag);
s32 func_80055E24_orig(u8 *cmd, u8 *obj);
static int scripted; /* a message from the mission's script (its lines: hints, orders, dialogue) */
s32 func_80048B4C(char *text, u8 prio, u8 flag); /* a message on the HUD */
void func_8004B270(s32 damage, s32 through_armour, s32 always); /* Bond hurt */

extern EndlessCarry port_endless_carry;
int port_endless_room(s16 *entry);
int port_endless_wave(void);
void port_endless_next(void);
int port_endless_segment(int point);
void port_endless_unrank(const char *why);
int port_endless_room_started(void);
void port_endless_score(int points);
void port_endless_ungate(int level);
void quick_grenade_give(void);

#define OBJ(i) ((u8 *)D_800AFFC8 + (i) * 0x124)
#define TYPE_DEF(t) ((u8 *)g_Args->unk228 + (t) * 0x70) /* the object types: +1 their class (6 people), +2 (0x22 a door's zone) */
#define HEALTH(o) (*(s32 *)((u8 *)(o) + 0x34))
#define POS(o) ((s32 *)(*(u8 **)((u8 *)(o) + 0x90) + 0x28))
#define SEC 20                                   /* frames a second (the game's 20 Hz) */
#define SLOTS 10                                 /* g_Weapons' slots (named; the mission's own set) */

static void dump(void);
/* the HUD's messages (in writable memory: the game's own are in its data) */
static char msg_open[] = "EXIT OPEN", msg_locked[] = "EXIT LOCKED", msg_back[] = "NOT THAT WAY", msg_wave[16],
            msg_leave[] = "PRESS USE TO LEAVE";

/* The guards to copy: one record of each kind of the mission's guards (people of whom it places more than one: not
 * Bond, nor the story people). The lists hold the whole mission's things, each section's in its own part of the
 * world. */
#define KINDS 8
static s32 kinds[KINDS][6];
static int kind_count;
/* the section's floor: its polygons' middles, and which of them Bond can walk to (flooded from his) */
#define FACES 4096
#define VERTS 8192
static s32 mid[FACES][3];
static u8 reach[FACES], shut[FACES];
static u16 vfirst[VERTS + 1], vfaces[FACES * 3], queue[FACES];
static int faces;
/* its doorways (its door zones: load points' and locked doors') */
#define DOORS 48
static s32 doors[DOORS][3];
static int door_count;
/* the copies' records (the objects made from them keep pointing at them: +0x60) */
#define COPIES 64
static s32 copies[COPIES][6];
static int copy_next;
/* the copies made: the object, when, from where */
#define HUNTERS 16
static struct {
    u8 *o;
    s32 born, x, z;
} hunters[HUNTERS];

/* the room's own fixed guards (snipers, grenade throwers, posts: those of the game's standing AIs, 2-5) that fell,
 * to come back to their places half a minute later */
#define RETURNS 8
static struct {
    s32 rec[6], at[3], due;
} returns[RETURNS];

static s16 entry[5];
static s32 was_alive[64]; /* each person's health last frame (a guard falling leaves pickups) */
static s32 was_at[3], said; /* where Bond was as the last frame began; when a message was last given */
static s32 last_frame = -1, room_frames, carried, kills_at_start, kills_scored, budget, cooldown, open;
static s32 burst, segment, last_section = -1, loading, at_entry;
static u32 rng = 12345;

static u32 rnd(void) {
    rng = rng * 1103515245 + 12345;
    return rng >> 8;
}

/* sine of a (4096 a turn), times 4096 (Bhaskara's approximation, to within a percent or so) */
static s32 isin(s32 a) {
    s32 x = a & 0x7FF, r = 4 * x * (2048 - x) / 1024; /* x in half a turn */
    r = r * 4096 / (5 * 2048 * 2048 / 1024 - r);
    r = r * 4;
    return (a & 0x800) ? -r : r;
}

static s32 dist2(s32 x0, s32 z0, s32 x1, s32 z1) { /* the distance on the ground, roughly */
    s32 dx = x0 - x1, dz = z0 - z1;
    dx = dx < 0 ? -dx : dx;
    dz = dz < 0 ? -dz : dz;
    return dx > dz ? dx + dz / 2 : dz + dx / 2;
}

static s32 bond_health(void) {
    return HEALTH(g_Player);
}

static const char *weapon_name(void) {
    return D_800B0DD4 >= 0 && D_800B0DD4 < 10 ? (char *)g_Weapons[D_800B0DD4].pad0 : "?";
}

static int is_copy(u8 *o) {
    s32 *src = *(s32 **)(o + 0x60);
    int h;
    for (h = 0; h < HUNTERS; h++)
        if (hunters[h].o == o)
            return 1;
    return src >= &copies[0][0] && src < &copies[0][0] + COPIES * 6;
}

/* the people alive (but Bond), and how many of those are the section's own */
static int alive(int *own) {
    int i, n = 0;
    *own = 0;
    for (i = D_800B0DA0; i <= D_800B07B0; i++) {
        u8 *o = OBJ(i);
        if (o == g_Player || !o[4] || HEALTH(o) <= 0)
            continue;
        /* (only those about: the section's far parts' people, held in their slots, don't make a fight) */
        if (dist2(g_Args->playerX, g_Args->playerZ, POS(o)[0], POS(o)[2]) > 12000 ||
            POS(o)[1] - g_Args->playerY > 1500 || g_Args->playerY - POS(o)[1] > 1500)
            continue;
        n++;
        if (!is_copy(o))
            (*own)++;
    }
    return n;
}

/* Bond as he leaves the room, for the next (the weapons this mission doesn't have kept as they were) */
/* (the log: Bond's weapons now) */
static void log_weapons(const char *when) {
    char text[400];
    int i, n = 0;
    text[0] = 0;
    for (i = 0; i < 10; i++) {
        Weapon *w = &g_Weapons[i];
        if (w->pad0[0] >= 'A' && w->pad0[0] <= 'Z')
            n += snprintf(text + n, sizeof(text) - n, "%s%d:%.16s%s %d; ", i == D_800B0DD4 ? "*" : "", i,
                          (char *)w->pad0, (w->flags & 0x1800) == 0x1800 ? "" : "(-)", w->ammo);
    }
    ELOG("weapons %s: %s", when, text);
}

static void carry_out(void) {
    EndlessCarry *c = &port_endless_carry;
    int i, j;
    c->valid = 1;
    c->health = bond_health();
    c->armour = D_8009CFF0;
    for (i = 0; i < SLOTS; i++) {
        Weapon *w = &g_Weapons[i];
        if (w->pad0[0] < 'A' || w->pad0[0] > 'Z')
            continue;
        for (j = 0; j < c->weapons && strncmp(c->weapon[j].name, (char *)w->pad0, 23); j++)
            ;
        if (j == c->weapons) {
            if (j == ENDLESS_WEAPONS)
                continue;
            c->weapons++;
            strncpy(c->weapon[j].name, (char *)w->pad0, 23);
            c->weapon[j].name[23] = 0;
        }
        c->weapon[j].owned = (w->flags & 0x1800) == 0x1800;
        c->weapon[j].ammo = w->ammo;
        c->weapon[j].clip = w->clip;
        c->weapon[j].reserve = w->reserve;
        if (i == D_800B0DD4)
            snprintf(c->in_hand, sizeof(c->in_hand), "%s", c->weapon[j].name);
    }
}

/* ... and as he comes into this one: what he had of this mission's weapons, and more of an ammo than it gives */
static void carry_in(void) {
    EndlessCarry *c = &port_endless_carry;
    int i, j;
    if (!c->valid)
        return;
    if (c->health > 0)
        HEALTH(g_Player) = c->health;
    D_8009CFF0 = c->armour;
    for (i = 0; i < SLOTS; i++) {
        Weapon *w = &g_Weapons[i];
        for (j = 0; j < c->weapons; j++)
            if (w->pad0[0] >= 'A' && w->pad0[0] <= 'Z' && !strncmp(c->weapon[j].name, (char *)w->pad0, 23))
                break;
        if (j == c->weapons)
            continue;
        if (c->weapon[j].owned)
            w->flags |= 0x1800;
        if (c->weapon[j].ammo > w->ammo) {
            w->ammo = c->weapon[j].ammo;
            w->clip = c->weapon[j].clip;
            w->reserve = c->weapon[j].reserve;
        }
    }
}

/* the weapon Bond held (in a run's first room: his pistol), drawn as the room starts (the game changes to
 * D_800B0DDC); a mission without it leaves the one it gives */
static int draw_slot = -1; /* (a slot to change to, as soon as the game lets him) */
int weapon_change_to(int slot);
static void draw_weapon(void) {
    EndlessCarry *c = &port_endless_carry;
    const char *want = c->valid ? c->in_hand : "WOLFRAM P2K";
    int i;
    for (i = 0; i < SLOTS; i++) {
        Weapon *w = &g_Weapons[i];
        if (!strncmp((char *)w->pad0, want, 23)) {
            if (!c->valid) /* (the run's first room: the pistol his) */
                w->flags |= 0x1800;
            if ((w->flags & 0x1800) == 0x1800 && i != D_800B0DD4) {
                draw_slot = i;
                ELOG("weapon: %s to be drawn", want);
            }
            return;
        }
    }
}

/* the section's floor's height under (x, z) (-0x7FFF: none of it there) */
static s32 floor_at(s32 x, s32 y, s32 z) {
    s16 at[4];
    if (x < -0x7FFF || x > 0x7FFF || z < -0x7FFF || z > 0x7FFF)
        return -0x7FFF;
    at[0] = x, at[1] = y, at[2] = z, at[3] = 0;
    return func_800705A8(D_800E2F54, at, 0, 0);
}

/* The floor's polygons (+0x24, 8 bytes each: three corners and the normal; +0x10 how many) over its corners (+0x1C),
 * and which corners each one has; those in a doorway are shut */
static void read_floor(void) {
    u8 *mesh = (u8 *)D_800E2F54;
    int i, k, verts = 0;
    faces = 0;
    if (!mesh)
        return;
    faces = *(s16 *)(mesh + 0x10);
    if (faces > FACES)
        faces = FACES;
    memset(vfirst, 0, sizeof(vfirst));
    for (i = 0; i < faces; i++) {
        u16 *f = (u16 *)(*(u8 **)(mesh + 0x24) + i * 8);
        s32 x = 0, y = 0, z = 0;
        for (k = 0; k < 3; k++) {
            s16 *v = (s16 *)(*(u8 **)(mesh + 0x1C) + f[k] * 8);
            x += v[0], y += v[1], z += v[2];
            if (f[k] < VERTS) {
                vfirst[f[k] + 1]++;
                if (f[k] + 1 > verts)
                    verts = f[k] + 1;
            }
        }
        mid[i][0] = x / 3, mid[i][1] = y / 3, mid[i][2] = z / 3;
        shut[i] = 0;
        for (k = 0; k < door_count; k++)
            if (dist2(mid[i][0], mid[i][2], doors[k][0], doors[k][2]) < 700 && mid[i][1] - doors[k][1] < 1500 &&
                doors[k][1] - mid[i][1] < 1500)
                shut[i] = 1;
    }
    for (i = 0; i < VERTS; i++)
        vfirst[i + 1] += vfirst[i];
    {
        static u16 fill[VERTS];
        memset(fill, 0, sizeof(fill));
        for (i = 0; i < faces; i++) {
            u16 *f = (u16 *)(*(u8 **)(mesh + 0x24) + i * 8);
            for (k = 0; k < 3; k++)
                if (f[k] < VERTS)
                    vfaces[vfirst[f[k]] + fill[f[k]]++] = i;
        }
    }
}

/* the floor Bond can walk to from where he stands without a doorway: polygons sharing a corner, flooded */
static int flood(void) {
    u8 *mesh = (u8 *)D_800E2F54;
    s32 bx = g_Args->playerX, by = g_Args->playerY, bz = g_Args->playerZ, best = 0x7FFFFFFF;
    int i, k, j, start = -1, head = 0, tail = 0, n = 0;
    if (!mesh || !faces)
        return 0;
    for (i = 0; i < faces; i++) {
        s32 d = dist2(bx, bz, mid[i][0], mid[i][2]), dy = mid[i][1] - by;
        reach[i] = 0;
        if (dy < 600 && dy > -600 && d < best)
            best = d, start = i;
    }
    if (start < 0)
        return 0;
    reach[start] = 1;
    queue[tail++] = start;
    while (head < tail) {
        u16 *f = (u16 *)(*(u8 **)(mesh + 0x24) + queue[head++] * 8);
        n++;
        for (k = 0; k < 3; k++) {
            if (f[k] >= VERTS)
                continue;
            for (j = vfirst[f[k]]; j < vfirst[f[k] + 1]; j++) {
                int g = vfaces[j];
                if (!reach[g] && !shut[g]) {
                    reach[g] = 1;
                    queue[tail++] = g;
                }
            }
        }
    }
    return n;
}

/* One of the mission's story people, not a guard: a kind it places only once (an ally, a target, a boss), or not
 * of the guards' sort (the people types' +8: 0 the guards and Bond; 1 and 2 civilians, women, scientists, the
 * assassin) */
static int story_person(u32 t, s32 *counts) {
    return t && TYPE_DEF(t)[1] == 6 && t != ((u8 *)g_Player)[6] && (counts[t] < 2 || TYPE_DEF(t)[8] != 0);
}

/* The mission's story people, out: those of a kind it places only once (counts: how many of each kind it places),
 * their records too, so that its spawner doesn't make them again */
static void no_story_people(s32 *counts) {
    int i, j;
    for (i = 0; i < D_8009CF68; i++)
        for (j = 0; j < D_8009CF6C[i]; j++) {
            s32 *p = D_8009CF70[i] + j * 6;
            u32 t = p[3] & 0xFF;
            if (story_person(t, counts)) {
                ELOG("room: story person (type %d) left out of the section", t);
                p[3] = (p[3] & ~0xFF) | 0x100; /* (no kind, and made) */
            }
        }
    for (i = D_800B0DA0; i <= D_800B07B0; i++) {
        u8 *o = OBJ(i);
        if (o != g_Player && o[4] && story_person(o[6], counts) && !is_copy(o)) {
            ELOG("room: story person (type %d) taken out", o[6]);
            func_800395E8(o);
        }
    }
}

/* the room as it starts: the kinds of guards, its doorways and floor */
static void room_start(void) {
    s32 i, j, counts[256];
    room_frames = carried = budget = open = kills_scored = said = burst = at_entry = 0;
    draw_slot = -1;
    kind_count = copy_next = door_count = 0;
    memset(hunters, 0, sizeof(hunters));
    memset(returns, 0, sizeof(returns));
    cooldown = SEC * 5;
    kills_at_start = *(u16 *)((u8 *)g_Args + 0x2C4);
    rng ^= D_8009CDA8 * 2654435761u ^ entry[2] ^ D_800D2F8C;
    last_section = D_800D2F8C;
    for (i = 0; i < 256; i++)
        counts[i] = 0;
    for (i = 0; i < D_8009CF68; i++)
        for (j = 0; j < D_8009CF6C[i]; j++)
            counts[D_8009CF70[i][j * 6 + 3] & 0xFF]++;
    for (i = 0; i < D_8009CF68; i++)
        for (j = 0; j < D_8009CF6C[i]; j++) {
            s32 *p = D_8009CF70[i] + j * 6, k;
            u32 t = p[3] & 0xFF;
            if (t && TYPE_DEF(t)[2] == 0x22 && door_count < DOORS) { /* a door's zone */
                doors[door_count][0] = p[0] + D_800D2FB0[0];
                doors[door_count][1] = p[1] + D_800D2FB0[1];
                doors[door_count][2] = p[2] + D_800D2FB0[2];
                door_count++;
            }
            if (!t || TYPE_DEF(t)[1] != 6 || story_person(t, counts) || t == ((u8 *)g_Player)[6] || kind_count == KINDS)
                continue;
            for (k = 0; k < kind_count && (kinds[k][3] & 0xFF) != t; k++)
                ;
            if (k == kind_count)
                memcpy(kinds[kind_count++], p, sizeof(kinds[0]));
        }
    no_story_people(counts);
    read_floor();
    if (!segment)
        carry_in(); /* (Bond as he left the last room, before the room's first frame) */
    if (port_endless_room_started() > 1)
        port_endless_unrank("room restarted");
    ELOG("room start: level %d section %d at %d %d %d, wave %d: %d kinds of guards, %d floor polygons, %d doorways, "
         "people slots %d-%d", g_Args->level, D_800D2F8C, entry[2], entry[3], entry[4], port_endless_wave(),
         kind_count, faces, door_count, D_800B0DA0, D_800B07B0);
}

/* Where a guard may come: in at a doorway ahead of Bond (one further from where he came in than he is, not by him,
 * on the floor of his side of it), from one where none came lately; failing that, a floor polygon he can walk to,
 * out of his sight (far, further in front of him) and ahead of him. -1 none. */
static s32 recent[4][3];
static int recent_next;

/* room around a place for a guard: floor nearly all round it at the guard's reach (a wall or a drop leaves none) */
static int need_clear = 1; /* (a room whose floor shows no room anywhere: its places taken as they are) */
static int clear(int f) {
    static const s8 dir[8][2] = {{3, 0}, {2, 2}, {0, 3}, {-2, 2}, {-3, 0}, {-2, -2}, {0, -3}, {2, -2}};
    int k, ok = 0;
    for (k = 0; k < 8; k++) {
        s32 h = floor_at(mid[f][0] + dir[k][0] * 60, mid[f][1] - 400, mid[f][2] + dir[k][1] * 60);
        ok += h != -0x7FFF && h - mid[f][1] <= 400 && mid[f][1] - h <= 400;
    }
    return !need_clear || ok >= 7;
}

static int lately(s32 x, s32 z) { /* (by where one of the last few came, or by where Bond came in) */
    int k;
    if (dist2(x, z, entry[2], entry[4]) < 3500)
        return 1;
    for (k = 0; k < 4; k++)
        if ((recent[k][0] | recent[k][2]) && dist2(recent[k][0], recent[k][2], x, z) < 2500)
            return 1;
    return 0;
}

#define LEVEL_DY 1500 /* (a place on Bond's own floor: another floor's guards would stay up there and shoot down) */

static int place(int *door) {
    s32 bx = g_Args->playerX, by = g_Args->playerY, bz = g_Args->playerZ, at = dist2(bx, bz, entry[2], entry[4]);
    s32 fx = isin(gPsi), fz = isin(gPsi + 0x400); /* the way he looks */
    static u16 pick[FACES];
    int i, k, n = 0;
    *door = 1;
    for (k = 0; k < door_count; k++) {
        s32 best = 0x7FFFFFFF, b = -1, d = dist2(bx, bz, doors[k][0], doors[k][2]);
        if (d < 3500 || d > 20000 || dist2(entry[2], entry[4], doors[k][0], doors[k][2]) < at + 500)
            continue;
        for (i = 0; i < faces; i++) {
            s32 e;
            if (!reach[i] || shut[i] || (e = dist2(doors[k][0], doors[k][2], mid[i][0], mid[i][2])) > 2200 ||
                e < 800 || mid[i][1] - doors[k][1] > 1200 || doors[k][1] - mid[i][1] > 1200 ||
                mid[i][1] - by > LEVEL_DY || by - mid[i][1] > LEVEL_DY)
                continue; /* (a little inside the room, clear of the doorway) */
            if (e < best && clear(i))
                best = e, b = i;
        }
        if (b >= 0 && !lately(mid[b][0], mid[b][2]) && dist2(bx, bz, mid[b][0], mid[b][2]) > 3500)
            pick[n++] = b;
    }
    if (n && !getenv("TWINE_ENDLESS_TEST_NEAR"))
        return pick[rnd() % n];
    *door = 0;
    for (i = 0; i < faces; i++) {
        s32 dx, dz, far, on, seen, h;
        if (!reach[i] || shut[i] || mid[i][1] - by > LEVEL_DY || by - mid[i][1] > LEVEL_DY)
            continue;
        dx = mid[i][0] - bx, dz = mid[i][2] - bz, far = dist2(bx, bz, mid[i][0], mid[i][2]);
        on = dist2(entry[2], entry[4], mid[i][0], mid[i][2]);
        /* in front of him (within some 50 degrees) a place must be further off, past what he can make out */
        seen = far > 0 && (dx / 16 * fx + dz / 16 * fz) / (far / 16 + 1) > 4096 * 2 / 3;
        if (getenv("TWINE_ENDLESS_TEST_NEAR") ? far < atoi(getenv("TWINE_ENDLESS_TEST_NEAR")) || far > atoi(getenv("TWINE_ENDLESS_TEST_NEAR")) + 2500
                                              : far < (seen ? 11000 : 5000) || far > 16000 || on < at + 1000)
            continue;
        if (lately(mid[i][0], mid[i][2]) || !clear(i))
            continue;
        if ((h = floor_at(mid[i][0], mid[i][1] - 200, mid[i][2])) == -0x7FFF || h < mid[i][1] - 300)
            continue; /* (under another floor: below stairs) */
        pick[n++] = i;
    }
    if (n)
        return pick[rnd() % n];
    /* nothing ahead (a small room): a doorway anywhere else on his floor, not by him and out of his sight */
    *door = 1;
    for (k = 0; k < door_count; k++) {
        s32 dx = doors[k][0] - bx, dz = doors[k][2] - bz, d = dist2(bx, bz, doors[k][0], doors[k][2]);
        if (d < 5000 || d > 20000 || (dx / 16 * fx + dz / 16 * fz) / (d / 16 + 1) > 0)
            continue; /* (behind him or to the side only) */
        for (i = 0; i < faces; i++)
            if (reach[i] && !shut[i] && dist2(doors[k][0], doors[k][2], mid[i][0], mid[i][2]) < 2200 &&
                dist2(doors[k][0], doors[k][2], mid[i][0], mid[i][2]) > 800 && mid[i][1] - by < LEVEL_DY &&
                by - mid[i][1] < LEVEL_DY && !lately(mid[i][0], mid[i][2]) && clear(i)) {
                pick[n++] = i;
                break;
            }
    }
    if (n)
        return pick[rnd() % n];
    /* a small room: its floor's furthest clear place from him, out of his sight, but not by him */
    {
        s32 best = 2500;
        int b = -1;
        *door = 0;
        for (i = 0; i < faces; i++) {
            s32 dx = mid[i][0] - bx, dz = mid[i][2] - bz, d = dist2(bx, bz, mid[i][0], mid[i][2]);
            if (!reach[i] || shut[i] || d <= best || mid[i][1] - by > LEVEL_DY || by - mid[i][1] > LEVEL_DY ||
                (dx / 16 * fx + dz / 16 * fz) / (d / 16 + 1) > 4096 / 4 || lately(mid[i][0], mid[i][2]) || !clear(i))
                continue;
            best = d, b = i;
        }
        if (b < 0) { /* the last: any place he can walk to, not right by him (a tiny room) */
            for (i = 0; i < faces; i++) {
                s32 d = dist2(bx, bz, mid[i][0], mid[i][2]);
                if (reach[i] && !shut[i] && d > 2000 && d > best - 2500 && mid[i][1] - by <= LEVEL_DY &&
                    by - mid[i][1] <= LEVEL_DY)
                    best = d + 2500, b = i;
            }
        }
        if (b < 0) { /* (why: for the log) */
            int r = 0, lv = 0, cl = 0, la = 0;
            for (i = 0; i < faces; i++)
                if (reach[i] && !shut[i]) {
                    r++;
                    if (mid[i][1] - by <= LEVEL_DY && by - mid[i][1] <= LEVEL_DY) {
                        lv++;
                        cl += clear(i);
                        la += !lately(mid[i][0], mid[i][2]);
                    }
                }
            ELOG("place: none: %d polygons reachable, %d on his floor (y %d), %d clear, %d not by the entry", r, lv, by,
                 cl, la);
        }
        return b;
    }
}

/* A people's slot freed for a guard to come: the one furthest from Bond, of those far off (or on another floor),
 * goes; he comes back when Bond nears his place, as the section brings its own (logged: who held the slots) */
static int free_slot(u32 type) {
    int i, b = -1;
    s32 best = 9000;
    for (i = D_800B0DA0; i <= D_800B07B0; i++) {
        u8 *o = OBJ(i);
        s32 d;
        if (o == g_Player || !o[4])
            continue;
        d = dist2(g_Args->playerX, g_Args->playerZ, POS(o)[0], POS(o)[2]);
        if (POS(o)[1] - g_Args->playerY > 1500 || g_Args->playerY - POS(o)[1] > 1500)
            d += 20000;
        if (o[6] == type) /* (each kind has its own slots: the level's guy slots, KGnnn.INI) */
            d += 40000;
        if (HEALTH(o) <= 0 && d > 3000) /* (a body: it goes first) */
            d += 80000;
        ELOG("slots: %d: type %d, health %d, %d off", i, o[6], HEALTH(o), d);
        if (d > best)
            best = d, b = i;
    }
    if (b < 0)
        return 0;
    ELOG("slots: %d freed (type %d, %d off)", b, OBJ(b)[6], best);
    if (*(s32 **)(OBJ(b) + 0x60)) /* (its record: not made, so that the section makes it again when Bond nears) */
        **(s32 **)(OBJ(b) + 0x60) &= ~0x100;
    func_800395E8(OBJ(b));
    return 1;
}

/* a copy of one of the kinds of guards at a place (why: for the log) */
static void spawn(const char *why) {
    int i, h, door;
    s32 *rec;
    u8 *o;
    need_clear = 1;
    if (kind_count && (i = place(&door)) < 0) {
        need_clear = 0;
        i = place(&door);
    }
    if (!kind_count || i < 0) {
        if (kind_count)
            ELOG("spawn: no place for a guard (%s)", why);
        cooldown = SEC * 3;
        return;
    }
    rec = copies[copy_next];
    copy_next = (copy_next + 1) % COPIES;
    memcpy(rec, kinds[rnd() % kind_count], sizeof(copies[0]));
    rec[0] = mid[i][0] - D_800D2FB0[0];
    rec[1] = mid[i][1] - D_800D2FB0[1];
    rec[2] = mid[i][2] - D_800D2FB0[2];
    rec[3] &= ~0x300;           /* not made yet; not asleep */
    rec[4] &= ~(0x1F << 7);     /* no path to walk (the kind's was in its own section) */
    o = func_8003C1BC(rec, mid[i][0], mid[i][1], mid[i][2]);
    for (h = 0; !o && h < kind_count; h++) { /* (the kinds in turn, a slot freed for each) */
        rec[3] = (rec[3] & ~0x3FF) | (kinds[h][3] & 0xFF);
        if ((o = func_8003C1BC(rec, mid[i][0], mid[i][1], mid[i][2])))
            break;
        rec[3] &= ~0x300;
        if (free_slot(rec[3] & 0xFF))
            o = func_8003C1BC(rec, mid[i][0], mid[i][1], mid[i][2]);
        rec[3] &= ~0x300;
    }
    if (!o) {
        ELOG("spawn: the game made no guard (type %d: the people's six slots are full)", rec[3] & 0xFF);
        cooldown = SEC * 5;
        return;
    }
    for (h = 0; h < HUNTERS && hunters[h].o && hunters[h].o[4] && HEALTH(hunters[h].o) > 0; h++)
        ;
    if (h < HUNTERS)
        hunters[h].o = o, hunters[h].born = room_frames, hunters[h].x = mid[i][0], hunters[h].z = mid[i][2];
    recent[recent_next][0] = mid[i][0], recent[recent_next][2] = mid[i][2];
    recent_next = (recent_next + 1) % 4;
    ELOG("spawn (%s): guard type %d %s at %d %d %d, %d from Bond, budget %d", why, rec[3] & 0xFF,
         door ? "in at a doorway" : "on the floor", mid[i][0], mid[i][1], mid[i][2],
         dist2(g_Args->playerX, g_Args->playerZ, mid[i][0], mid[i][2]), budget);
}

/* Copies left far off for long, sent back (the director sends them again, nearer): the pressure stays on Bond */
static void recall(void) {
    int h;
    for (h = 0; h < HUNTERS; h++) {
        u8 *o = hunters[h].o;
        s32 d;
        if (!o)
            continue;
        if (!o[4] || HEALTH(o) <= 0) {
            hunters[h].o = 0;
            continue;
        }
        d = dist2(g_Args->playerX, g_Args->playerZ, POS(o)[0], POS(o)[2]);
        if (POS(o)[1] - g_Args->playerY > 1500 || g_Args->playerY - POS(o)[1] > 1500)
            d += 20000; /* (on another floor: as good as far) */
        if (room_frames - hunters[h].born > SEC * 20 && d > 13000) {
            ELOG("recall: guard type %d left %d off for %d s: sent back", o[6], d, (room_frames - hunters[h].born) / SEC);
            func_800395E8(o);
            hunters[h].o = 0;
            budget += SEC * 20; /* (another, soon) */
        }
    }
}

/* A pickup of this mission's at (x, y, z) (none if it has none of that type): an object of the pickups' kind (4, as
 * the game makes the gun a guard drops: func_80039A34), which Bond's touch takes (func_8004CA5C) */
static u8 *pickup(s32 type, s32 x, s32 y, s32 z, const char *what) {
    u8 *o;
    if (type <= 0 || type > 0xFF)
        return 0;
    MAKE_AT[0] = x, MAKE_AT[1] = y, MAKE_AT[2] = z;
    o = func_80039A9C(0, type, 4);
    ELOG("pickup made: %s (type %d) at %d %d %d%s", what, type, x, y, z, o ? "" : ": none (no room for it)");
    return o;
}

/* What a guard leaves as he falls: armour, ammo for one of the mission's weapons, now and then a weapon */
static s32 last_drop[3];
static void drop(u8 *o) {
    s32 x = POS(o)[0], y = POS(o)[1] - 100, z = POS(o)[2], r = rnd() % 100, i, n;
    s32 weapons[19], ammo[19], na = 0;
    u8 *box;
    for (i = n = 0; i < 19; i++) {
        if (D_8009D130[i * 4] >= 0 && D_8009D130[i * 4 + 1] > 0)
            weapons[n++] = i; /* (a weapon lying about: its own pickup) */
        if (D_8009D130[i * 4] >= 0 && D_8009D130[i * 4] < SLOTS)
            ammo[na++] = i;   /* (its ammo, in a box: any of the mission's weapons, gadgets too) */
    }
    last_drop[0] = x, last_drop[1] = y, last_drop[2] = z;
    /* a frag grenade, now and then: King's Ransom's own pickup there; elsewhere, one of the quick grenade's */
    if (rnd() % 100 < 20) {
        for (i = 0; i < 19; i++)
            if (D_8009D130[i * 4] >= 0 && D_8009D130[i * 4 + 1] > 0 &&
                !strncmp((char *)&g_Weapons[D_8009D130[i * 4]], "FRAG GRENADE", 12))
                break;
        if (i < 19)
            pickup(D_8009D130[i * 4 + 1], x - 150, y, z - 150, "frag grenade");
#ifdef MOD_QUICKGRENADE
        else {
            static char got[] = "GOT FRAG GRENADE";
            quick_grenade_give();
            func_80048B4C(got, 0x80, 0);
            ELOG("grenade: a frag grenade given (the mission has none to drop)");
        }
#endif
    }
    if (r < 8)
        pickup(D_8009D106, x, y, z, "heavy armour");
    else if (r < 35)
        pickup(D_8009D104 > 0 ? D_8009D104 : D_8009D106, x, y, z, "light armour");
    else if (r < 50 && n)
        pickup(D_8009D130[weapons[rnd() % n] * 4 + 1], x, y, z, "weapon");
    /* and ammo, mostly */
    if (rnd() % 100 < 70 && na && (box = pickup(D_8009D108, x + 150, y, z + 150, "ammo box"))) {
        *(s32 *)(box + 0x34) = ammo[rnd() % na];
        ELOG("pickup: the box holds %s's ammo", (char *)g_Weapons[D_8009D130[*(s32 *)(box + 0x34) * 4]].pad0);
    }
}

/* the room left for the next: its bonus, Bond carried across (how: for the log) */
static void leave(int wave, const char *how) {
    log_weapons("as Bond leaves");
    port_endless_score(500 + 250 * wave + (room_frames < SEC * 90 ? (SEC * 90 - room_frames) / 2 : 0));
    carry_out();
    port_endless_next();
    g_Args->flags |= 0x40;
    ELOG("room left (%s) after %d s", how, room_frames / SEC);
}

void endless_frame(void) {
    int own, n, wave, i;
    s32 kills, back;
    if (port_subframe) /* (an extra drawing's: not the mission's frame) */
        return;
    if (getenv("TWINE_ENDLESS_DUMP"))
        dump();
    if (!port_endless_room(entry) || !g_Player)
        return;
    if (D_8009CDA8 == last_frame) /* (once a frame: the controller is read again in some dialogs) */
        return;
    if (D_8009CDA8 < last_frame || last_frame < 0)
        room_start();
    last_frame = D_8009CDA8;
    port_endless_ungate(g_Args->level);
    /* a stretch of the mission (King's Ransom): the next section loaded the mission's way is the next room */
    if (loading && D_800D2F90 == 0 && D_800D2F8C != last_section) {
        loading = 0;
        port_endless_room(entry);
        room_start();
    }
    if (g_CutScene)
        return;
    if (!carried) {
        carried = 1;
        for (i = 0; i < 64; i++)
            was_alive[i] = 0;
        snprintf(msg_wave, sizeof(msg_wave), "WAVE %d", port_endless_wave());
        func_80048B4C(msg_wave, 0x80, 0);
        ELOG("room ready: health %d armour %d, weapon %s, floor Bond can reach: %d polygons", bond_health(),
             D_8009CFF0, weapon_name(), flood());
        draw_weapon();
        log_weapons("as the room starts");
        /* the first room: a heavy armour ahead of Bond */
        if (port_endless_wave() == 1 && !segment) {
            s32 x = g_Args->playerX + (isin(gPsi) * 1300 >> 12), z = g_Args->playerZ + (isin(gPsi + 0x400) * 1300 >> 12);
            s32 y = floor_at(x, g_Args->playerY - 500, z);
            pickup(D_8009D106, x, y == -0x7FFF ? g_Args->playerY : y, z, "heavy armour (the first room's)");
        }
    }
    room_frames++;
    if (room_frames % (SEC * 10) == 0)
        flood(); /* (where Bond can walk to now) */
    /* the debug menu, the game's cheats (used, or on: g_Args +4 bits 3, 9-16) or the port's take the run off the
     * high scores */
    if (g_DevMenu[0])
        port_endless_unrank("debug menu");
    if (*(u32 *)((u8 *)g_Args + 4) & 0x1FE08)
        port_endless_unrank("cheats");
    if (getenv("TWINE_INFINITE_GRENADES") && atoi(getenv("TWINE_INFINITE_GRENADES")))
        port_endless_unrank("infinite grenades");
    wave = port_endless_wave();
    kills = *(u16 *)((u8 *)g_Args + 0x2C4) - kills_at_start;
    /* the score: a kill is worth more the sooner it comes after Bond came in (twice as much at once, down to its
     * worth after a minute) */
    if (kills > kills_scored) {
        s32 bonus = room_frames < SEC * 60 ? 100 - room_frames * 100 / (SEC * 60) : 0;
        port_endless_score((kills - kills_scored) * (100 + bonus) * (10 + wave) / 10);
        kills_scored = kills;
    }
    if (getenv("TWINE_ENDLESS_TEST_KILL") && room_frames % atoi(getenv("TWINE_ENDLESS_TEST_KILL")) == 0)
        for (i = D_800B0DA0; i <= D_800B07B0; i++) /* (headless tests: the guards fall) */
            if (OBJ(i) != g_Player && OBJ(i)[4] && HEALTH(OBJ(i)) > 0) {
                HEALTH(OBJ(i)) = 0;
                func_8005E020(OBJ(i));
            }
    n = alive(&own);
    for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) {
        u8 *o = OBJ(i);
        s32 h = o[4] ? HEALTH(o) : 0;
        if (o != g_Player && was_alive[i] > 0 && h <= 0 && o[4]) {
            s32 *src = *(s32 **)(o + 0x60);
            int k;
            ELOG("kill: guard type %d (%s) at %d off, with %s", o[6], is_copy(o) ? "sent" : "the room's",
                 dist2(g_Args->playerX, g_Args->playerZ, POS(o)[0], POS(o)[2]), weapon_name());
            if (!is_copy(o) && src && o[0xA7] >= 2 && o[0xA7] <= 5)
                for (k = 0; k < RETURNS; k++)
                    if (!returns[k].due) {
                        memcpy(returns[k].rec, src - 3, sizeof(returns[k].rec));
                        returns[k].at[0] = src[-3] + D_800D2FB0[0];
                        returns[k].at[1] = src[-2] + D_800D2FB0[1];
                        returns[k].at[2] = src[-1] + D_800D2FB0[2];
                        returns[k].due = room_frames + SEC * 30;
                        ELOG("special: guard type %d (ai %d) back at his place in 30 s", o[6], o[0xA7]);
                        break;
                    }
            drop(o);
        }
        was_alive[i] = h;
    }

    /* the director: more the longer Bond stays and the further he has come, as many at once as the wave allows;
     * a room with few guards of its own gets some at once, one every second and a half */
    budget += 2 + wave;
    if (cooldown > 0)
        cooldown--;
    for (i = 0; i < RETURNS; i++) /* the room's fixed guards back first */
        if (returns[i].due && room_frames >= returns[i].due) {
            s32 *rec = copies[copy_next];
            u8 *o;
            copy_next = (copy_next + 1) % COPIES;
            memcpy(rec, returns[i].rec, sizeof(returns[i].rec));
            rec[3] &= ~0x300;
            o = func_8003C1BC(rec, returns[i].at[0], returns[i].at[1], returns[i].at[2]);
            if (!o && free_slot(rec[3] & 0xFF)) {
                rec[3] &= ~0x300;
                o = func_8003C1BC(rec, returns[i].at[0], returns[i].at[1], returns[i].at[2]);
            }
            ELOG("special: guard type %d %s at %d %d %d", rec[3] & 0xFF, o ? "back" : "not back (no slot)",
                 returns[i].at[0], returns[i].at[1], returns[i].at[2]);
            returns[i].due = o ? 0 : room_frames + SEC * 5;
        }
    i = D_800B07B0 - D_800B0DA0 + 1; /* (the people's slots but Bond's: six) */
    if (n >= i && room_frames % (SEC * 5)) /* (all taken by ones near: none can come) */
        ;
    else if (!cooldown && room_frames > SEC * 2 && burst < 3 && own + burst < 3) {
        burst++;
        spawn("few guards here");
        if (cooldown < SEC * 3 / 2)
            cooldown = SEC * 3 / 2;
    } else if (!cooldown && budget >= SEC * 20 && n < 2 + wave) {
        spawn("director");
        budget = 0;
        cooldown = SEC * 4 + rnd() % (SEC * 4);
    }
    recall();

    /* the doors: open once the room's own guards about are down (and one at least), or after kills enough, or a
     * while */
    if (!open) {
        const char *why = !own && kills && room_frames > SEC * 20 ? "its guards down" :
                          kills >= 3 + wave                        ? "kills enough" :
                          room_frames > SEC * 90                   ? "time" : 0;
        if (why) {
            open = 1;
            func_80048B4C(msg_open, 0x80, 0);
            ELOG("exit open (%s): %d kills, %d of the room's guards left", why, kills, own);
        }
    }
    if (getenv("TWINE_ENDLESS_TEST_DIE") && room_frames == atoi(getenv("TWINE_ENDLESS_TEST_DIE")) &&
        (!strchr(getenv("TWINE_ENDLESS_TEST_DIE"), '@') || wave == atoi(strchr(getenv("TWINE_ENDLESS_TEST_DIE"), '@') + 1)))
        func_8004B270(0x40000, 1, 1); /* (headless tests: Bond hit hard, at frame N or N@wave) */
    if (getenv("TWINE_ENDLESS_TEST_GRAB") && room_frames == atoi(getenv("TWINE_ENDLESS_TEST_GRAB")) && last_drop[0]) {
        s16 at[5]; /* (headless tests: Bond onto the last drop) */
        at[0] = D_800D2F8C, at[1] = gPsi, at[2] = last_drop[0], at[3] = last_drop[1], at[4] = last_drop[2];
        func_80030FA8(at);
    }
    if (getenv("TWINE_ENDLESS_TEST_ARM") && room_frames == 40) { /* (headless tests: every weapon, ammo) */
        for (i = 0; i < 10; i++)
            if (g_Weapons[i].pad0[0] >= 'A' && g_Weapons[i].pad0[0] <= 'Z')
                g_Weapons[i].flags |= 0x1800, g_Weapons[i].ammo += 20, g_Weapons[i].reserve += 20;
        log_weapons("armed (test)");
    }
    if (draw_slot >= 0 && D_800B0DD4 != draw_slot) {
#ifdef MOD_WEAPONKEYS
        if (weapon_change_to(draw_slot))
#endif
            draw_slot = -1;
    }
    if (room_frames == SEC * 5)
        log_weapons("five seconds in");
    if (getenv("TWINE_ENDLESS_TEST_OPEN") && room_frames == atoi(getenv("TWINE_ENDLESS_TEST_OPEN")))
        open = 1; /* (headless tests: the exit open) */
    if (getenv("TWINE_ENDLESS_TEST_LOCKED") && room_frames >= 100 && room_frames < 140)
        D_800D2F90 = 2; /* (headless tests: a shut door, walked into again and again) */
    if (getenv("TWINE_ENDLESS_TEST_DOOR") && room_frames == 100) { /* (headless tests: a door to load point N) */
        open = 1, room_frames = SEC * 60, D_800D2F90 = 2, D_8009CFFC = 1;
        *(s16 **)(D_8009E9C4 + 8) = D_8009E9C4 + 0xC + atoi(getenv("TWINE_ENDLESS_TEST_DOOR")) * 8;
    }
    if (getenv("TWINE_ENDLESS_TEST_EXIT") && room_frames == atoi(getenv("TWINE_ENDLESS_TEST_EXIT")))
        open = 1, D_800D2F90 = 2, room_frames = SEC * 60; /* (headless tests: as if Bond went through a door) */
    /* the way out by the entry (for a room whose doors lead nowhere: a story's, a lift's, or only for show): once the
     * exit is open, standing where he came in, Bond is told he may leave, and leaves with the use button */
    if (open && !(g_Args->flags & 0x40) && !loading &&
        dist2(g_Args->playerX, g_Args->playerZ, entry[2], entry[4]) < 1500) {
        if (!at_entry++) {
            func_80048B4C(msg_leave, 0x80, 0);
            ELOG("entry: Bond told he may leave here");
        }
        if (g_PadPressed & *(u16 *)((u8 *)g_Args + 0x14E)) {
            D_8009CFFC = 1; /* (the fade out) */
            leave(wave, "the use button at the entry");
            return;
        }
    } else
        at_entry = 0;
    if (D_800D2F90 == 2 && !(g_Args->flags & 0x40) && !loading) {
        s16 *to = *(s16 **)(D_8009E9C4 + 8);
        int point = to ? (to - (D_8009E9C4 + 0xC)) / 8 : -1;
        back = dist2(g_Args->playerX, g_Args->playerZ, entry[2], entry[4]) < 3000 && room_frames < SEC * 60;
        if (open && !back) { /* the room cleared */
            ELOG("door: open, to load point %d (section %d)", point, to ? to[0] : -1);
            if (port_endless_segment(point)) { /* the mission's next section, as the next room */
                port_endless_score(500 + 250 * wave);
                segment++;
                loading = 1;
                ELOG("room left for the mission's next section (stretch %d)", segment);
            } else {
                segment = 0;
                leave(wave, "door");
            }
        } else { /* shut: the load the door asked for undone, and Bond back out of the doorway */
            s32 *pos = POS(g_Player);
            D_800D2F90 = 0;
            D_8009CFFC = -1;
            *(s32 *)(D_8009E9C4 + 8) = 0;
            pos[0] = was_at[0], pos[1] = was_at[1], pos[2] = was_at[2];
            if (room_frames - said > SEC * 3 || room_frames < said) {
                func_80048B4C(back ? msg_back : msg_locked, 0x80, 0);
                said = room_frames;
                ELOG("door: %s (to load point %d)", back ? "the way Bond came in" : "locked", point);
            }
        }
    }
    was_at[0] = POS(g_Player)[0], was_at[1] = POS(g_Player)[1], was_at[2] = POS(g_Player)[2];
}

/* The mission failed (text: why): in a run, only by Bond's death */
s32 func_8002D4E4(s32 text) {
    s16 e[5];
    if (port_endless_room(e) && g_Player && bond_health() > 0) {
        ELOG("story: the mission's failure skipped (\"%s\")", text ? (char *)text : "");
        return 0;
    }
    return func_8002D4E4_orig(text);
}

/* The mission completed: never in a run */
s32 func_8002E1B0(void) {
    s16 e[5];
    if (port_endless_room(e)) {
        ELOG("story: the mission's completion skipped");
        return 0;
    }
    return func_8002E1B0_orig();
}

/* RESTART? after Bond's death: in a run, NO at once (the prompt's own NO: func_80042020) */
void RestartPrompt_Open(void) {
    s16 e[5];
    if (!port_endless_room(e)) {
        RestartPrompt_Open_orig();
        return;
    }
    ELOG("run end: Bond is dead");
    D_8009CAA4 = 0;
    g_Args->flags |= 0x80;
}

/* A person hit: one that is hit again and again without being hurt (a story person the game won't let die,
 * under a guard's look) is taken out of the room */
void func_8005E020(u8 *o) {
    static u8 *who[8];
    static s32 hits[8], was[8];
    s16 e[5];
    int k;
    s32 before = HEALTH(o);
    func_8005E020_orig(o);
    if (port_subframe || !port_endless_room(e) || o == g_Player || !o[4])
        return;
    for (k = 0; k < 8 && who[k] != o; k++)
        ;
    if (k == 8) {
        for (k = 0; k < 8 && who[k] && who[k][4]; k++)
            ;
        if (k == 8)
            k = 0;
        who[k] = o, hits[k] = 0, was[k] = before;
    }
    if (HEALTH(o) < was[k] - 0) {
        was[k] = HEALTH(o), hits[k] = 0;
        return;
    }
    if (++hits[k] >= 6 && HEALTH(o) > 0) {
        ELOG("person type %d hit %d times unhurt (health %d): taken out", o[6], hits[k], HEALTH(o));
        func_800395E8(o);
        who[k] = 0;
    }
}

/* A pickup touched by Bond: logged when taken */
s32 func_8004CA5C(u8 *o, u8 *who) {
    s32 type = o[6], health = HEALTH(g_Player), armour = D_8009CFF0, ammo = D_800B0DD4 >= 0 ? g_Weapons[D_800B0DD4].ammo : 0;
    s32 r = func_8004CA5C_orig(o, who);
    s16 e[5];
    if (r && !port_subframe && port_endless_room(e))
        ELOG("pickup taken: type %d: health %d -> %d, armour %d -> %d, ammo in hand %d -> %d", type, health,
             HEALTH(g_Player), armour, D_8009CFF0, ammo, D_800B0DD4 >= 0 ? g_Weapons[D_800B0DD4].ammo : 0);
    return r;
}

/* Bond hurt (or healed): logged */
void func_8004B270(s32 damage, s32 through_armour, s32 always) {
    s32 health = g_Player ? HEALTH(g_Player) : 0, armour = D_8009CFF0;
    s16 e[5];
    /* arriving: none until Bond has been in the room, with control and the picture up, six seconds (from a section's
     * far side especially, where its guards may already be facing him) */
    if (damage > 0 && port_endless_room(e) && (!carried || room_frames < SEC * 6) && !getenv("TWINE_ENDLESS_TEST_DIE")) {
        if (!port_subframe)
            ELOG("damage: %d kept off (Bond arriving)", damage);
        return;
    }
    func_8004B270_orig(damage, through_armour, always);
    if (g_Player && damage > 0 && !port_subframe && port_endless_room(e))
        ELOG("damage: %d: health %d -> %d, armour %d -> %d%s", damage, health, HEALTH(g_Player), armour, D_8009CFF0,
             HEALTH(g_Player) <= 0 ? " (dead)" : "");
}

/* A cutscene of the mission's (its NIS): none in a run */
s32 func_80054414(u8 nis, u8 arg1, u8 arg2) {
    s16 e[5];
    if (port_endless_room(e)) {
        ELOG("story: cutscene %d skipped", nis);
        return 0;
    }
    return func_80054414_orig(nis, arg1, arg2);
}

/* The loading picture as the mission ends for another (flags 0x40): not between rooms, which go on in the dark */
void func_8002E318(void) {
    s16 e[5];
    if (port_endless_room(e)) {
        ELOG("loading picture skipped");
        return;
    }
    func_8002E318_orig();
}

/* A HUD message: in a run, not the story's (objectives, failures; the game's own short notes, such as a pickup's or
 * a locked door's, stay) */
s32 func_80048B4C(char *text, u8 prio, u8 flag) {
    s16 e[5];
    s32 r;
    if (text && port_endless_room(e)) {
        int story = prio != 0x80 || scripted || strstr(text, "MISSION") || strstr(text, "FAILED") || strstr(text, "ESCAPED") ||
                    strstr(text, "OBJECTIVE") || strstr(text, "ALARM");
        if (story) {
            static char last[48];
            if (!port_subframe && strncmp(last, text, 47)) { /* (logged once, not every frame it is asked for) */
                strncpy(last, text, 47);
                ELOG("message hidden: \"%s\"", text);
            }
            return 0;
        }
    }
    r = func_80048B4C_orig(text, prio, flag);
    if (r == 1 && !port_subframe && port_endless_room(e)) /* (new on the HUD) */
        ELOG("message shown: \"%s\"", text);
    return r;
}

/* The mission's script giving a line (a hint, an order, someone speaking): not shown in a run */
s32 func_80055E24(u8 *cmd, u8 *obj) {
    s32 r;
    scripted = 1;
    r = func_80055E24_orig(cmd, obj);
    scripted = 0;
    return r;
}

/* TWINE_ENDLESS_DUMP: the section's doors, kinds of things and people as the room starts, and the loads asked for */
static void dump(void) {
    static s32 last_load = -1, last = -1, dumped;
    s32 i, j, counts[256];
    if (D_8009CDA8 < last)
        last_load = -1, dumped = 0;
    last = D_8009CDA8;
    if (!dumped++) {
        for (i = 0; i < 256; i++)
            counts[i] = 0;
        for (i = 0; i < D_8009CF68; i++)
            for (j = 0; j < D_8009CF6C[i]; j++) {
                s32 *p = D_8009CF70[i] + j * 6;
                u32 t = p[3] & 0xFF;
                counts[t]++;
                if (TYPE_DEF(t)[2] == 0x22)
                    ELOG("dump: door type %d at %d %d %d to %d cb %08X", t, p[0] + D_800D2FB0[0],
                         p[1] + D_800D2FB0[1], p[2] + D_800D2FB0[2], (u32)p[3] >> 27, *(u32 *)(TYPE_DEF(t) + 0x38));
            }
        for (i = 0; i < 256; i++)
            if (counts[i]) {
                char hex[0x70 * 2 + 1];
                for (j = 0; j < 0x70; j++)
                    snprintf(hex + j * 2, 3, "%02X", TYPE_DEF(i)[j]);
                ELOG("dump: type %d x%d class %d/%d def8 %d: %s", i, counts[i], TYPE_DEF(i)[1], TYPE_DEF(i)[2], TYPE_DEF(i)[8], hex);
            }
        for (i = 0; i < D_8009CF68; i++)
            for (j = 0; j < D_8009CF6C[i]; j++) {
                s32 *p = D_8009CF70[i] + j * 6;
                if (TYPE_DEF(p[3] & 0xFF)[1] == 6)
                    ELOG("dump: placed person type %d at %d %d %d flags %08X w4 %08X w5 %08X", p[3] & 0xFF,
                         p[0] + D_800D2FB0[0], p[1] + D_800D2FB0[1], p[2] + D_800D2FB0[2], p[3], p[4], p[5]);
            }
    }
    if (D_8009CDA8 % 100 == 0)
        for (i = D_800B0DA0; i <= D_800B07B0; i++)
            if (OBJ(i)[4])
                ELOG("dump: person %d type %d health %d ai %d at %d %d %d", i, OBJ(i)[6], HEALTH(OBJ(i)), OBJ(i)[0xA7],
                     POS(OBJ(i))[0], POS(OBJ(i))[1], POS(OBJ(i))[2]);
    if (D_800D2F90 != last_load) {
        s16 *to = *(s16 **)(D_8009E9C4 + 8);
        ELOG("dump: load state %d -> %d (to %d)", last_load, D_800D2F90, to ? to[5] : -1);
        last_load = D_800D2F90;
    }
}
