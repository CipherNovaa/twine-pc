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
 * MOD_REPLACES_NATIVE: func_8002D4E4 func_8002E1B0 RestartPrompt_Open func_8004CA5C func_8004B270 func_80054414 func_8002E318 func_80048B4C func_80055E24 func_8005E020 func_8003C1BC func_80041DBC
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
unsigned strlen(const char *s);
char *strchr(const char *s, int c);
char *strstr(const char *s, const char *t);
void port_endless_logf(s32 frame, const char *fmt, ...);
extern int port_subframe; /* (interp.c: an extra drawing between ticks runs, whose memory is put back after) */
#define ELOG(...) port_endless_logf(D_8009CDA8, __VA_ARGS__)

extern u8 D_800D90C0[];   /* the cutscenes' state (+0x124: bits 4-19 those played) */
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
u8 *func_8003C1BC_orig(s32 *rec, s32 x, s32 y, s32 z);
u8 *func_80039A9C(s32 arg0, u8 type, s32 kind);  /* makes an object of a kind (4: a pickup) at MAKE_AT */
void func_800395E8(u8 *o);                       /* takes an object out of the game */
void func_8005E020(u8 *o);                       /* a person hit: hurt, or falls */
void func_8005E020_orig(u8 *o);
void func_80030FA8(s16 *at);                     /* puts Bond at a point (section, yaw, x, y, z) */
s32 func_800705A8(s32 floor, s16 *pos, s32 arg2, s32 arg3); /* the floor's height under pos, -0x7FFF none */
#define MAKE_AT ((s32 *)0x1F8003D4u)             /* (the scratchpad's x, y, z) */
s32 func_8002D4E4_orig(s32 text);
void func_80041DBC(void);
void func_80041DBC_orig(void);
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
void port_endless_restart(void);
void quick_grenade_give(void);
u32 port_ext_free_bytes(void); /* (malloc3.c) */
void func_80053DE4(s16 type, s16 sub, s32 x, s32 y, s32 z); /* an event on the AI's board (0: a gunshot) */

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
    s32 lx, lz, moved; /* (where he was when he last moved on, and when) */
} hunters[HUNTERS];
static s32 stuck_at[8][2]; /* places guards were sent from that left them stuck (behind glass, at a wall) */
static int stuck_count, seated_said;

/* the room's own fixed guards (snipers, grenade throwers, posts: those of the game's standing AIs, 2-5) that fell,
 * to come back to their places half a minute later */
#define RETURNS 8
static struct {
    s32 rec[6], at[3], due;
} returns[RETURNS];

/* how each people's slot's person was made (the log's: placed, sent, a fallen one's return, remade after a slot freed) */
static const char *made_how[64];
static int making;               /* (what spawn()/the returns are making now: 0 the section) */
static s32 *freed_recs[16];      /* (records (their flags words) whose person free_slot took out) */
static int freed_next;
static s32 floor_at(s32 x, s32 y, s32 z);
static void people_log(void) {
    int i;
    for (i = D_800B0DA0; i <= D_800B07B0; i++) {
        u8 *o = OBJ(i), *st = *(u8 **)(o + 0x94);
        s32 *src = *(s32 **)(o + 0x60);
        if (o == g_Player || !o[4])
            continue;
        ELOG("people: %d type %d %s, health %d, ai %d at %d %d %d (floor %d, yaw %d); record %08X flags %08X w4 %08X; "
             "state +24 %08X; obj +8 %d +A4 %02X +A6 %02X +48 %08X +4C %08X +E %04X", i, o[6], made_how[i & 63] ? made_how[i & 63] : "?",
             HEALTH(o), o[0xA7], POS(o)[0], POS(o)[1], POS(o)[2], floor_at(POS(o)[0], POS(o)[1] - 500, POS(o)[2]),
             *(s16 *)(*(u8 **)(o + 0x90) + 0x1A), (u32)src, src ? src[0] : 0, src ? src[1] : 0,
             st ? *(s32 *)(st + 0x24) : -1, o[8], o[0xA4], o[0xA6], *(s32 *)(o + 0x48), *(s32 *)(o + 0x4C), *(u16 *)(o + 0xE));
    }
}

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

/* Bond can see him (or as good as): near (6000), or ahead within some 60 degrees, out to 40000 (walls not
 * counted: when in doubt, seen). Nobody seen is taken out, recalled or freed. */
static int seen(u8 *o) {
    s32 dx = POS(o)[0] - g_Args->playerX, dz = POS(o)[2] - g_Args->playerZ, d = dist2(0, 0, dx, dz);
    s32 fx = isin(gPsi), fz = isin(gPsi + 0x400);
    if (d < 6000)
        return 1;
    return d < 40000 && (dx / 16 * fx + dz / 16 * fz) / (d / 16 + 1) > 4096 / 2;
}

/* someone taken out of the room (why: for the log, with whether Bond could see him) */
static void take_out(u8 *o, const char *why) {
    ELOG("removed: type %d (%s), health %d, %d off, %s", o[6], why, HEALTH(o),
         dist2(g_Args->playerX, g_Args->playerZ, POS(o)[0], POS(o)[2]), seen(o) ? "Bond could see him" : "out of Bond's sight");
    func_800395E8(o);
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
/* What Bond carries in a run, and what may drop: what fights (guns, launchers, grenades, satchels and their
 * detonator, the stunner, his fists; not the pen bomb, the tranquilizer or the flare gun). The missions' tools and story
 * things (keys, cards, the palmtop, the scanner, the briefcase, the wrench, the grapple, the bug, the camera, the
 * VLF disruptor, the radiation badge and detector) are taken away, and Flashpoint's gas grenade too (its slot can't be
 * thrown: picked up, it did nothing). */
static const char *const combat_items[] = {
    "UNARMED", "WOLFRAM P2K", "KOFFLER KS5", "FRINESI SHOTGUN", "FRINESI AUTO", "INGALLS TYPE 20", "AR36 SNIPER",
    "BELGIQUE PS100", "MEYER TMP", "IAC DEFENDER", "KAZAKOVICH KA-57", "ROCKET LAUNCHER", "GRENADE LAUNCHER",
    "FRAG GRENADE", "FLASH BANG", "SATCHEL CHARGE", "SATCHEL DETONATO", "CELL PHONE STUNN", 0};

static int combat_item(const char *name) {
    int k;
    for (k = 0; combat_items[k]; k++)
        if (!strncmp(name, combat_items[k], strlen(combat_items[k])))
            return 1;
    return 0;
}

/* the mission's tools and story things out of Bond's hands (whatever it gives him at its start too) */
static void strip_tools(void) {
    int i;
    for (i = 0; i < SLOTS; i++) {
        Weapon *w = &g_Weapons[i];
        if (w->pad0[0] >= 'A' && w->pad0[0] <= 'Z' && (w->flags & 0x1800) && !combat_item((char *)w->pad0)) {
            ELOG("inventory: %s taken away (no use in a fight)", (char *)w->pad0);
            w->flags &= ~0x1800;
        }
    }
}

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

static int draw_slot = -1; /* (a slot to change to, as soon as the game lets him) */
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
    }
    {   /* the weapon in his hand now (or the one he is changing to: ours, or the game's) */
        int h = draw_slot >= 0 ? draw_slot : D_800B0DDC >= 0 && D_800B0DDC < SLOTS ? D_800B0DDC : D_800B0DD4;
        if (h < 0 || h >= SLOTS || g_Weapons[h].pad0[0] < 'A' || g_Weapons[h].pad0[0] > 'Z')
            h = D_800B0DD4;
        if (h >= 0 && h < SLOTS && g_Weapons[h].pad0[0] >= 'A' && g_Weapons[h].pad0[0] <= 'Z') {
            strncpy(c->in_hand, (char *)g_Weapons[h].pad0, sizeof(c->in_hand) - 1);
            c->in_hand[sizeof(c->in_hand) - 1] = 0;
        }
        ELOG("carry: Bond leaves with %s in hand (slot %d; in hand %d, changing to %d)", c->in_hand, h, D_800B0DD4,
             D_800B0DDC);
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
int weapon_change_to(int slot);
static void draw_weapon(void) {
    EndlessCarry *c = &port_endless_carry;
    const char *want = c->valid ? c->in_hand : "WOLFRAM P2K";
    int i;
    static const char *const guns[] = {"WOLFRAM P2K", 0}; /* (never a launcher or an explosive unasked) */
    int g;
    for (i = 0; i < SLOTS; i++) {
        Weapon *w = &g_Weapons[i];
        if (!strncmp((char *)w->pad0, want, 23)) {
            if (!c->valid) /* (the run's first room: the pistol his) */
                w->flags |= 0x1800;
            if ((w->flags & 0x1800) == 0x1800) {
                if (i != D_800B0DD4)
                    draw_slot = i;
                ELOG("weapon: %s drawn", want);
                return;
            }
        }
    }
    /* the mission hasn't his (or he hasn't it here): his pistol (stage 2's borrowing will keep his own everywhere) */
    for (g = 0; guns[g]; g++)
        for (i = 0; i < SLOTS; i++) {
            Weapon *w = &g_Weapons[i];
            if (!strncmp((char *)w->pad0, guns[g], 23) && (w->flags & 0x1800) == 0x1800 && w->ammo > 0) {
                if (i != D_800B0DD4)
                    draw_slot = i;
                ELOG("weapon: %s drawn (no %s here)", guns[g], want);
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

/* The fighters among the mission's people (workbench/research/STAGE2_REPORT.md, Q5), by what they can do: armed
 * (the type's weapon, +0x2C: the civilians have none), a guard's health (+0x20: the story's people have 7000-50000)
 * and hittable (hit mask +0x28 in the type, +0xE in a person, not 0x7FFF nor 0x800; not passed by explosions,
 * +0x40 / +0x4C & 0x400). Guards that may not be killed (+0x4C & 0x40000000: Night Watch's, Courier's bank guards)
 * fight in a run. */
static int fighter_type(u32 t, int script) {
    u8 *def = TYPE_DEF(t);
    (void)script;
    return t && def[1] == 6 && t != ((u8 *)g_Player)[6] && def[0x2C] != 0 && *(s32 *)(def + 0x20) <= 5000 &&
           *(u16 *)(def + 0x28) != 0x7FFF && !(*(u16 *)(def + 0x28) & 0x800) && !(*(s32 *)(def + 0x40) & 0x400);
}

/* the people who break a run, taken out: unkillable (hit mask 0x7FFF or 0x800, passed by explosions) or of a story
 * person's health (the assassin's 50000, Renard's, Electra's): civilians and scientists stay, and may be killed */
static int unkillable_type(u32 t) {
    u8 *def = TYPE_DEF(t);
    return t && def[1] == 6 && t != ((u8 *)g_Player)[6] &&
           (*(s32 *)(def + 0x20) > 5000 || *(u16 *)(def + 0x28) == 0x7FFF || (*(u16 *)(def + 0x28) & 0x800) ||
            (*(s32 *)(def + 0x40) & 0x400));
}

static int unkillable(u8 *o) { /* (a person now: scripts change these as they go, Electra's hit mask) */
    return unkillable_type(o[6]) || *(u16 *)(o + 0xE) == 0x7FFF || (*(u16 *)(o + 0xE) & 0x800) ||
           (*(s32 *)(o + 0x4C) & 0x400);
}

static int story_person(u32 t, s32 *rec) { /* (a placed person who would break the run: an unkillable one who
                                            * fights; unkillable civilians, the bank's receptionist, stay) */
    (void)rec;
    return unkillable_type(t) && TYPE_DEF(t)[0x2C] != 0;
}

/* The mission's story people, out: those of a kind it places only once (counts: how many of each kind it places),
 * their records too, so that its spawner doesn't make them again */
static void no_story_people(s32 *counts) {
    int i, j;
    for (i = 0; i < D_8009CF68; i++)
        for (j = 0; j < D_8009CF6C[i]; j++) {
            s32 *p = D_8009CF70[i] + j * 6;
            u32 t = p[3] & 0xFF;
            if (story_person(t, p)) {
                ELOG("room: story person (type %d) left out of the section", t);
                p[3] = (p[3] & ~0xFF) | 0x100; /* (no kind, and made) */
            }
        }
    for (i = D_800B0DA0; i <= D_800B07B0; i++) {
        u8 *o = OBJ(i);
        if (o != g_Player && o[4] && o[5] == 6 && unkillable(o) && TYPE_DEF(o[6])[0x2C] && !is_copy(o)) {
            take_out(o, "a story person, as the room starts");
        }
    }
}

/* Nobody where Bond arrives (a section entered from its far side may have a guard's post on its load point, a seated
 * one at a desk say): Bond arrives instead at the nearest clear floor a little way off, facing the same way */
static int clear(int f);
static void clear_arrival(void) {
    s32 who[8][3], best = 0x7FFFFFFF, mid_x = 0, mid_y = 0, mid_z = 0;
    int n = 0, i, j, k, b = -1;
    for (i = D_800B0DA0; i <= D_800B07B0 && n < 8; i++) { /* (the people there, and the section's placed ones) */
        u8 *o = OBJ(i);
        if (o != g_Player && o[4] && dist2(POS(o)[0], POS(o)[2], entry[2], entry[4]) < 900)
            who[n][0] = POS(o)[0], who[n][2] = POS(o)[2], n++;
    }
    for (i = 0; i < D_8009CF68; i++)
        for (j = 0; j < D_8009CF6C[i] && n < 8; j++) {
            s32 *p = D_8009CF70[i] + j * 6, x = p[0] + D_800D2FB0[0], z = p[2] + D_800D2FB0[2];
            if ((p[3] & 0xFF) && !(p[3] & 0x400) && TYPE_DEF(p[3] & 0xFF)[1] == 6 && dist2(x, z, entry[2], entry[4]) < 900)
                who[n][0] = x, who[n][2] = z, n++;
        }
    if (!n)
        return;
    {   /* (places round the arrival, nearest first: floor at its height, away from them all) */
        s32 fy = floor_at(entry[2], entry[3] - 500, entry[4]), r, a;
        fy = fy == -0x7FFF ? entry[3] : fy;
        for (r = 1000; r <= 3000 && b < 0; r += 250)
            for (a = 0; a < 4096 && b < 0; a += 256) {
                s32 x = entry[2] + (isin(a) * r >> 12), z = entry[4] + (isin(a + 0x400) * r >> 12);
                s32 h = floor_at(x, fy - 500, z);
                if (h == -0x7FFF || h - fy > 300 || fy - h > 300)
                    continue;
                for (k = 0; k < n && dist2(x, z, who[k][0], who[k][2]) >= 1300; k++)
                    ;
                if (k == n)
                    b = 0, best = r, mid_x = x, mid_y = h, mid_z = z;
            }
    }
    if (b < 0) {
        ELOG("room: someone at Bond's arrival, and no clear place near it");
        return;
    }
    {
        s16 at[5];
        at[0] = D_800D2F8C, at[1] = entry[1], at[2] = mid_x, at[3] = mid_y, at[4] = mid_z;
        func_80030FA8(at);
        ELOG("room: someone at Bond's arrival: he arrives at %d %d %d instead (%d off)", mid_x, mid_y, mid_z, best);
        entry[2] = mid_x, entry[3] = mid_y, entry[4] = mid_z;
    }
}

/* Each level's AI scripts by index (w4 bits 20-22, obj+0xA7; workbench research, the AIBnn tables): which are a
 * fighting guard's that moves (GOON*, HEADGOON, KYLE, VGOON, BGOON*, GUARD, GOONCH, COREGOON). A copy takes a kind
 * whose record has one: others are civilians', story people's, a sniper's (fixed) or a friend's (Turncoat's GABOR). */
static const char guard_scripts[12][9] = {
    {0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0},  /* L01 GOON PILOT SNIPER ELECTRA BONDNIS DIRECTOR */
    {1, 1, 0, 0, 0, 0, 1, 0, 0},  /* L02 HEADGOON GOON1 LACHASE CIGAR RECEPT SCIENC1 KYLE BONDNIS */
    {1, 0, 0, 0, 0, 0, 0, 0, 0},  /* L03 GOON CIVI BULL1 BULL2 SNIPER BUM XMAS GABOR */
    {1, 0, 0, 0, 0, 0, 0, 0, 0},  /* L04 VGOON SERVANT ELECTRA DAVIDOV PHOLDER DAVIDOVNIS BONDNIS */
    {1, 0, 0, 0, 0, 0, 0, 0, 0},  /* L05 GOON05 XMAS ZUKOVSKY DIRECTOR */
    {1, 0, 0, 0, 0, 0, 0, 0, 0},  /* L06 GOON XMAS TECH M BONDNIS DIRECTOR */
    {1, 0, 0, 0, 0, 1, 1, 1, 0},  /* L07 GOON1B XMAS RENARD SCIENC1 BONDNIS BGOON BGOON2 GUARD */
    {0, 1, 1, 0, 0, 0, 0, 0, 0},  /* L08 CIGARG GOONCH BGOON2 SNIPER PUB SCIENC1 RENARD BONDNIS */
    {1, 0, 0, 0, 0, 0, 0, 0, 0},  /* L09 GOON GABOR ELECTRA BULLION M BONDNIS DIRECTOR */
    {1, 0, 0, 0, 0, 1, 0, 0, 0},  /* L10 GOON RENARD XMAS RENARD1 BONDNIS COREGOON DIRECTOR */
    {1, 0, 0, 0, 0, 0, 0, 0, 0},  /* L11 GOON ZUKOVSKY ELECTRA HOTGIRL DEALER BULLION DIRECTOR */
};
#define FIXED_SCRIPT(w4) (g_Args->level >= 1 && g_Args->level <= 11 && !guard_scripts[g_Args->level][(w4) >> 20 & 7])
/* the room as it starts: the kinds of guards, its doorways and floor */
static void room_start(void) {
    s32 i, j, counts[256];
    room_frames = carried = budget = open = kills_scored = said = burst = at_entry = 0;
    draw_slot = -1;
    kind_count = copy_next = door_count = stuck_count = seated_said = 0;
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
            if (!fighter_type(t, 0) || kind_count == KINDS) /* (copies only of armed guards) */
                continue;
            if (p[3] & 0x400) /* (a guard placed in a pose, seated at a desk: his copies would sit on air) */
                continue;
            if (getenv("TWINE_ENDLESS_TEST_KIND") && t != (u32)atoi(getenv("TWINE_ENDLESS_TEST_KIND")))
                continue;
            for (k = 0; k < kind_count && (kinds[k][3] & 0xFF) != t; k++)
                ;
            if (k == kind_count)
                memcpy(kinds[kind_count++], p, sizeof(kinds[0]));
            else if (FIXED_SCRIPT(kinds[k][4]) && !FIXED_SCRIPT(p[4])) /* (one of the kind who moves, rather) */
                memcpy(kinds[k], p, sizeof(kinds[0]));
        }
    {   /* kinds whose record holds no moving guard's script (a sniper's, a civilian's, a friend's: some the game makes
         * unhittable, hit mask 7FFF) are copied only when the room has no other */
        int mobile = 0, n = 0;
        for (i = 0; i < kind_count; i++)
            mobile += !FIXED_SCRIPT(kinds[i][4]);
        for (i = 0; i < kind_count; i++)
            if (!mobile || !FIXED_SCRIPT(kinds[i][4]))
                memcpy(kinds[n++], kinds[i], sizeof(kinds[0]));
            else
                ELOG("room: kind type %d (script %d: not a moving guard's) not copied", kinds[i][3] & 0xFF, kinds[i][4] >> 20 & 7);
        kind_count = n;
    }
    no_story_people(counts);
    {   /* (the log: the mission's placed people kept and left out, its armed guards among them) */
        int kept = 0, out = 0, armed = 0;
        for (i = 0; i < D_8009CF68; i++)
            for (j = 0; j < D_8009CF6C[i]; j++) {
                s32 *p = D_8009CF70[i] + j * 6;
                u32 t = p[3] & 0xFF;
                if (!t || TYPE_DEF(t)[1] != 6) {
                    if ((p[3] & 0x1FF) == 0x100) /* (one left out: no kind now) */
                        out++;
                    continue;
                }
                kept++;
                armed += fighter_type(t, 0);
            }
        ELOG("room: the mission's people: %d kept (%d armed guards), %d left out", kept, armed, out);
    }
    read_floor();
    clear_arrival();
    if (!segment)
        carry_in(); /* (Bond as he left the last room, before the room's first frame) */
    if (port_endless_room_started() > 1) { /* RESTART MISSION: the whole run again, fresh */
        ELOG("restart: the run begins again");
        port_endless_restart();
        g_Args->flags |= 0x40;
    }
    if (port_ext_free_bytes())
        ELOG("memory: the ext heap (8 MB of RAM) has %u bytes free", port_ext_free_bytes());
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

static int stuck_near(s32 x, s32 z) { /* (by where one sent got stuck) */
    int k;
    for (k = 0; k < stuck_count; k++)
        if (dist2(stuck_at[k][0], stuck_at[k][1], x, z) < 2500)
            return 1;
    return 0;
}

static int lately(s32 x, s32 z) { /* (by where one of the last few came, or by where Bond came in) */
    int k;
    if (stuck_near(x, z))
        return 1;
    if (dist2(x, z, entry[2], entry[4]) < 3500)
        return 1;
    for (k = 0; k < 4; k++)
        if ((recent[k][0] | recent[k][2]) && dist2(recent[k][0], recent[k][2], x, z) < 2500)
            return 1;
    return 0;
}

#define LEVEL_DY 1000 /* (a place on Bond's own floor: another floor's guards would stay up there and shoot down) */

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
                                              : far < (seen ? 8000 : 4000) || far > (seen ? 11000 : 10000) || on < at + 1000)
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
            if (!reach[i] || shut[i] || d <= best || d > 11000 || mid[i][1] - by > LEVEL_DY || by - mid[i][1] > LEVEL_DY ||
                (dx / 16 * fx + dz / 16 * fz) / (d / 16 + 1) > 4096 / 4 || lately(mid[i][0], mid[i][2]) || !clear(i))
                continue;
            best = d, b = i;
        }
        if (b < 0) { /* the last: any place he can walk to, not right by him (a tiny room) */
            for (i = 0; i < faces; i++) {
                s32 d = dist2(bx, bz, mid[i][0], mid[i][2]);
                if (reach[i] && !shut[i] && d > 2000 && d < 11000 && d > best - 2500 && !stuck_near(mid[i][0], mid[i][2]) &&
                    mid[i][1] - by <= LEVEL_DY &&
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
    s32 best = -1;
    for (i = D_800B0DA0; i <= D_800B07B0; i++) {
        u8 *o = OBJ(i);
        s32 d;
        if (o == g_Player || !o[4])
            continue;
        /* only a body or a guard the director sent, out of Bond's sight: the room's own (its snipers, its posts)
         * keep their slots while they live, and if none can go the director waits */
        if ((HEALTH(o) > 0 && !is_copy(o)) || seen(o))
            continue;
        d = dist2(g_Args->playerX, g_Args->playerZ, POS(o)[0], POS(o)[2]);
        if (o[6] == type) /* (each kind has its own slots: the level's guy slots, KGnnn.INI) */
            d += 40000;
        if (HEALTH(o) <= 0) /* (a body: it goes first) */
            d += 80000;
        ELOG("slots: %d: type %d, health %d, %d off", i, o[6], HEALTH(o), d);
        if (d > best)
            best = d, b = i;
    }
    if (b < 0)
        return 0;
    ELOG("slots: %d freed (type %d, %d off)", b, OBJ(b)[6], best);
    take_out(OBJ(b), HEALTH(OBJ(b)) > 0 ? "a sent guard, his slot freed" : "a body, its slot freed");
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
    {   /* the place itself: floor under it (a doorway's or a polygon's middle may be in a wall, a door's frame or
         * a solid volume, where a guard can't be shot nor move), and nobody within 500 */
        s32 fh = floor_at(mid[i][0], mid[i][1] - 500, mid[i][2]);
        int k;
        for (k = D_800B0DA0; k <= D_800B07B0; k++)
            if (OBJ(k) != g_Player && OBJ(k)[4] && HEALTH(OBJ(k)) > 0 &&
                dist2(POS(OBJ(k))[0], POS(OBJ(k))[2], mid[i][0], mid[i][2]) < 500)
                break;
        if (fh == -0x7FFF || k <= D_800B07B0) {
            ELOG("spawn: the place at %d %d %d won't do (%s)%s", mid[i][0], mid[i][1], mid[i][2],
                 k <= D_800B07B0 ? "someone there" : "no floor under it", k <= D_800B07B0 ? "" : ": not used again");
            if (k > D_800B07B0 && stuck_count < 8)
                stuck_at[stuck_count][0] = mid[i][0], stuck_at[stuck_count][1] = mid[i][2], stuck_count++;
            cooldown = SEC;
            return;
        }
    }
    rec = copies[copy_next];
    copy_next = (copy_next + 1) % COPIES;
    memcpy(rec, kinds[rnd() % kind_count], sizeof(copies[0]));
    rec[0] = mid[i][0] - D_800D2FB0[0];
    rec[1] = mid[i][1] - D_800D2FB0[1];
    rec[2] = mid[i][2] - D_800D2FB0[2];
    rec[3] &= ~0x300;           /* not made yet; not asleep */
    rec[4] &= ~(0x1F << 7);     /* no path to walk (the kind's was in its own section) */
    rec[4] &= ~(0xFF << 12);    /* nor the pose it was placed in (a seated guard's, at a desk: */
    rec[4] &= ~(7 << 24);       /* its start pose and state, w4's bits 12-19 and 24-26, and the flags' 14-26) */
    rec[3] &= ~(0x1FFF << 14);
    rec[3] &= ~0x400;           /* (and the pose's flag) */
    making = 1;
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
    making = 0;
    if (!o) {
        ELOG("spawn: the game made no guard (type %d: the people's six slots are full)", rec[3] & 0xFF);
        cooldown = SEC * 5;
        return;
    }
    for (h = 0; h < HUNTERS && hunters[h].o && hunters[h].o[4] && HEALTH(hunters[h].o) > 0; h++)
        ;
    if (h < HUNTERS)
        hunters[h].o = o, hunters[h].born = hunters[h].moved = room_frames, hunters[h].x = hunters[h].lx = mid[i][0],
        hunters[h].z = hunters[h].lz = mid[i][2];
    recent[recent_next][0] = mid[i][0], recent[recent_next][2] = mid[i][2];
    recent_next = (recent_next + 1) % 4;
    /* a gunshot heard where Bond is, on the AI's board: a guard sent comes alert (the game's own guards stand and wait
     * for a noise or the sight of him; Masquerade's take him for one of theirs till then) */
    func_80053DE4(0, 0, g_Args->playerX, g_Args->playerY, g_Args->playerZ);
    ELOG("spawn (%s): guard type %d %s at %d %d %d, %d from Bond, budget %d; ai %d, state %08X", why, rec[3] & 0xFF,
         door ? "in at a doorway" : "on the floor", mid[i][0], mid[i][1], mid[i][2],
         dist2(g_Args->playerX, g_Args->playerZ, mid[i][0], mid[i][2]), budget, o[0xA7],
         *(u8 **)(o + 0x94) ? *(s32 *)(*(u8 **)(o + 0x94) + 0x24) : -1);
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
        if (dist2(POS(o)[0], POS(o)[2], hunters[h].lx, hunters[h].lz) > 400)
            hunters[h].lx = POS(o)[0], hunters[h].lz = POS(o)[2], hunters[h].moved = room_frames;
        if (getenv("TWINE_ENDLESS_TEST_MEET") && room_frames - hunters[h].born == SEC * 3) {
            s16 at[5]; /* (headless tests: Bond put 1500 from the guard sent, facing him: does he fire?) */
            s32 a, ba = 0, best = -0x7FFFFFFF, toward = *(u16 *)(*(u8 **)(o + 0x90) + 0x62) & 0xFFF; /* (in front of him) */
            for (a = 0; a < 4096; a += 64) { /* (nearest that way with floor at his height) */
                s32 x = POS(o)[0] + (isin(a) * 1500 >> 12), z = POS(o)[2] + (isin(a + 0x400) * 1500 >> 12);
                s32 fh = floor_at(x, POS(o)[1] - 500, z), off = (a - toward) & 0xFFF;
                off = off > 0x800 ? 0x1000 - off : off;
                if (fh != -0x7FFF && fh - POS(o)[1] < 200 && POS(o)[1] - fh < 200 && -off > best)
                    best = -off, ba = a;
            }
            if (best > -0x7FFFFFFF) {
                at[0] = D_800D2F8C, at[1] = (s16)((ba + 0x800) & 0xFFF), at[2] = POS(o)[0] + (isin(ba) * 1500 >> 12);
                at[3] = POS(o)[1], at[4] = POS(o)[2] + (isin(ba + 0x400) * 1500 >> 12);
                func_80030FA8(at);
                ELOG("test: Bond put by the sent guard type %d", o[6]);
                if (getenv("TWINE_ENDLESS_TEST_NOISE"))
                    func_80053DE4(0, 0, at[2], at[3], at[4]);
            }
        }
        if (room_frames - hunters[h].born == SEC * 5)
            ELOG("sent guard type %d five seconds on: ai %d, state %08X, health %d, at %d %d %d", o[6], o[0xA7],
                 *(u8 **)(o + 0x94) ? *(s32 *)(*(u8 **)(o + 0x94) + 0x24) : -1, HEALTH(o), POS(o)[0], POS(o)[1], POS(o)[2]);
        if (room_frames - hunters[h].born == SEC && floor_at(POS(o)[0], POS(o)[1] - 500, POS(o)[2]) == -0x7FFF) {
            /* a second on, no floor under him (in a wall or a solid volume: he can't be shot): taken out at once,
             * seen or not (he has only just come), and none sent there again */
            if (stuck_count < 8)
                stuck_at[stuck_count][0] = hunters[h].x, stuck_at[stuck_count][1] = hunters[h].z, stuck_count++;
            take_out(o, "a sent guard with no floor under him");
            hunters[h].o = 0;
            budget += SEC * 5;
            continue;
        }
        if (seen(o)) /* (never before his eyes) */
            continue;
        if (room_frames - hunters[h].moved > SEC * 10 && d > 2500 && d < 8000) {
            /* one that came no nearer and stood still a while (in a place he can't leave: behind the bank's glass, at
             * a wall; more would stand there with him): sent back, and none sent from where he came again */
            ELOG("recall: guard type %d stuck at %d %d (%d off, sent from %d %d %d s ago): sent back, that place not "
                 "used again", o[6], POS(o)[0], POS(o)[2], d, hunters[h].x, hunters[h].z, (room_frames - hunters[h].born) / SEC);
            if (stuck_count < 8)
                stuck_at[stuck_count][0] = hunters[h].x, stuck_at[stuck_count][1] = hunters[h].z, stuck_count++;
            take_out(o, "a sent guard stuck");
            hunters[h].o = 0;
            budget += SEC * 10;
            continue;
        }
        if (room_frames - hunters[h].born > SEC * 15 && d > 9000) {
            ELOG("recall: guard type %d left %d off for %d s: sent back", o[6], d, (room_frames - hunters[h].born) / SEC);
            take_out(o, "a sent guard far off");
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
        int fights = D_8009D130[i * 4] >= 0 && D_8009D130[i * 4] < SLOTS &&
                     combat_item((char *)g_Weapons[D_8009D130[i * 4]].pad0) &&
                     strncmp((char *)g_Weapons[D_8009D130[i * 4]].pad0, "UNARMED", 7);
        if (fights && D_8009D130[i * 4 + 1] > 0)
            weapons[n++] = i; /* (a weapon lying about: its own pickup) */
        if (fights)
            ammo[na++] = i;   /* (its ammo, in a box) */
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
    if (loading)
        loading++;
    if (loading && D_800D2F90 == 0 && (D_800D2F8C != last_section || loading > SEC * 3)) {
        /* (or the same section again, a door back into it: three seconds on, the next room all the same) */
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
        strip_tools();
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
        flood();
    if (room_frames % (SEC * 4) == 0)
        people_log();
    if (getenv("TWINE_ENDLESS_TEST_LOOK") && room_frames > SEC * 2) { /* (headless tests: Bond faces person N) */
        u8 *o = OBJ(atoi(getenv("TWINE_ENDLESS_TEST_LOOK")));
        if (o[4]) {
            s32 dx = POS(o)[0] - g_Args->playerX, dz = POS(o)[2] - g_Args->playerZ, a, best = -0x7FFFFFFF, ba = 0;
            for (a = 0; a < 4096; a += 8) {
                s32 d = (isin(a) >> 4) * (dx >> 4) + (isin(a + 0x400) >> 4) * (dz >> 4);
                if (d > best)
                    best = d, ba = a;
            }
            gPsi = ba;
        }
    } /* (where Bond can walk to now) */
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
        s32 h;
        if (o != g_Player && o[4] && o[5] == 6) {
            if (unkillable(o) && TYPE_DEF(o[6])[0x2C] && !seen(o)) { /* (made unkillable by its script since: Electra's hit mask, say) */
                ELOG("room: person type %d (script %d, hit mask %04X) unkillable", o[6], o[0xA7], *(u16 *)(o + 0xE));
                take_out(o, "unkillable");
                continue;
            }
            *(s32 *)(o + 0x4C) &= ~0x40000000; /* (a guard who may not be killed: he may in a run) */
        }
        h = o[4] ? HEALTH(o) : 0;
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
            making = 2;
            o = func_8003C1BC(rec, returns[i].at[0], returns[i].at[1], returns[i].at[2]);
            if (!o && free_slot(rec[3] & 0xFF)) {
                rec[3] &= ~0x300;
                o = func_8003C1BC(rec, returns[i].at[0], returns[i].at[1], returns[i].at[2]);
            }
            making = 0;
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
    if (g_Args->level == 2) /* the bank's receptionist (type 60, placed seated at her desk): in the seated animation
                             * the opening cutscene leaves her in (0x3C), which her script, without it, never takes */
        for (i = D_800B0DA0; i <= D_800B07B0; i++) {
            u8 *o = OBJ(i), *an = *(u8 **)(o + 0x94);
            s32 *src = *(s32 **)(o + 0x60);
            if (o != g_Player && o[4] && o[6] == 60 && HEALTH(o) > 0 && an && src && (src[0] & 0x400) &&
                *(s16 *)(an + 0x10) != 0x3C) {
                if (!seated_said++)
                    ELOG("room: the receptionist kept seated (her script's animation %d)", *(s16 *)(an + 0x10));
                *(s16 *)(an + 0x10) = *(s16 *)(an + 0x12) = 0x3C;
                an[0x14] = 0, an[0x16] = 1, *(s16 *)(an + 0x18) = 0;
            }
        }
    if (getenv("TWINE_ENDLESS_TEST_SHOOT") && room_frames >= atoi(getenv("TWINE_ENDLESS_TEST_SHOOT")) * SEC &&
        room_frames % (SEC * 2) == 0) /* (headless tests: everyone shot, past the game's own hit tests: who is immune?) */
        for (i = D_800B0DA0; i <= D_800B07B0; i++) {
            u8 *o = OBJ(i);
            u16 mask = *(u16 *)(o + 0xE);
            s32 was = HEALTH(o);
            if (o == g_Player || !o[4] || o[5] != 6 || was <= 0)
                continue;
            if (mask == 0x7FFF || (mask & 0x800) || (*(s32 *)(o + 0x4C) & 0x02000400) || (o[0xA4] & 2)) {
                ELOG("test shot: person %d type %d immune (hit mask %04X, +4C %08X, +A4 %02X)", i, o[6], mask,
                     *(s32 *)(o + 0x4C), o[0xA4]);
                continue;
            }
            HEALTH(o) -= 700;
            func_8005E020(o);
            ELOG("test shot: person %d type %d health %d -> %d", i, o[6], was, HEALTH(o));
        }
    if (getenv("TWINE_ENDLESS_TEST_GOD") && HEALTH(g_Player) > 0 && HEALTH(g_Player) < 0x10000)
        HEALTH(g_Player) = 0x20000; /* (headless tests: Bond doesn't fall) */
    if (getenv("TWINE_ENDLESS_TEST_DRAW") && room_frames == 60) /* (headless tests: Bond changes to the weapon named) */
        for (i = 0; i < 10; i++)
            if (!strncmp((char *)g_Weapons[i].pad0, getenv("TWINE_ENDLESS_TEST_DRAW"), strlen(getenv("TWINE_ENDLESS_TEST_DRAW"))))
                draw_slot = i;
    if (draw_slot >= 0 && D_800B0DD4 != draw_slot) {
#ifdef MOD_WEAPONKEYS
        if (weapon_change_to(draw_slot))
#endif
            draw_slot = -1;
    }
    if (room_frames % SEC == 0)
        strip_tools(); /* (what the mission gives him later too: its scripts, its pickups) */
    if (room_frames == SEC * 5)
        log_weapons("five seconds in");
    if (getenv("TWINE_ENDLESS_TEST_RESTART") && room_frames == atoi(getenv("TWINE_ENDLESS_TEST_RESTART")))
        func_80041DBC(); /* (headless tests: the watch's RESTART MISSION) */
    if (getenv("TWINE_ENDLESS_TEST_OPEN") && room_frames == atoi(getenv("TWINE_ENDLESS_TEST_OPEN")))
        open = 1; /* (headless tests: the exit open) */
    if (getenv("TWINE_ENDLESS_TEST_LOCKED") && room_frames >= 100 && room_frames < 140)
        D_800D2F90 = 2; /* (headless tests: a shut door, walked into again and again) */
    if (getenv("TWINE_ENDLESS_TEST_DOOR") && room_frames == 100 && !segment) { /* (headless tests: a door to load point N) */
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
                log_weapons("as Bond leaves for the next section");
                carry_out(); /* (the weapon in hand kept across: the same mission's) */
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
    /* the watch's RESTART MISSION asks the same, Bond alive: its YES reloads (func_80041DBC: the run again) */
    if (g_Player && HEALTH(g_Player) > 0) {
        ELOG("restart: the watch's RESTART MISSION asked");
        RestartPrompt_Open_orig();
        return;
    }
    ELOG("run end: Bond is dead");
    D_8009CAA4 = 0;
    g_Args->flags |= 0x80;
}

/* RESTART MISSION (the watch's "RELOADING MISSION...": the mission's main returns with g_Args' flags 0x40 and the
 * boot stub starts it again): in a run, the whole run again, fresh */
void func_80041DBC(void) {
    s16 e[5];
    if (port_endless_room(e)) {
        ELOG("restart: RESTART MISSION: the run begins again");
        port_endless_restart();
    }
    func_80041DBC_orig();
}

/* A placed thing made as Bond nears it (the section's spawner): in a run, never a person who isn't a fighter (the
 * story's people: not even for the fade-in's frames) */
u8 *func_8003C1BC(s32 *rec, s32 x, s32 y, s32 z) {
    s16 e[5];
    u32 t = rec[3] & 0xFF;
    if (port_endless_room(e) && g_Args && t && story_person(t, rec)) {
        if (!port_subframe)
            ELOG("room: story person (type %d, script %d) not made", t, (rec[4] >> 20) & 7);
        rec[3] |= 0x100; /* (made, as far as the spawner knows) */
        return 0;
    }
    if (port_endless_room(e) && g_Args && t && TYPE_DEF(t)[1] != 6) { /* the mission's pickups of tools and story things */
        int k;
        for (k = 0; k < 19; k++)
            if (D_8009D130[k * 4 + 1] == (s32)t && D_8009D130[k * 4] >= 0 && D_8009D130[k * 4] < SLOTS &&
                !combat_item((char *)g_Weapons[D_8009D130[k * 4]].pad0)) {
                if (!port_subframe)
                    ELOG("room: the mission's pickup of %s (type %d) not made", (char *)g_Weapons[D_8009D130[k * 4]].pad0, t);
                rec[3] |= 0x100;
                return 0;
            }
    }
    if (port_endless_room(e) && g_Args && t && TYPE_DEF(t)[1] == 6 && (rec[3] & 0x400) && making != 1) {
        /* a person placed in a pose: Courier's guards seated at desks (the pose is his record's whole start, not its flag
         * alone, and sits on air without the mission's chair, frozen there) is not made, the director's guards
         * stand; elsewhere (King's Ransom's bar) he stands up, the room kept its people */
        if (g_Args->level == 2 && TYPE_DEF(t)[0x2C] != 0) { /* (an armed guard: the desks' civilians stay) */
            if (!port_subframe)
                ELOG("room: posed person (type %d) at %d %d %d not made", t, x, y, z);
            rec[3] |= 0x100;
            return 0;
        }
        if (g_Args->level != 2) {
            if (!port_subframe)
                ELOG("room: posed person (type %d) made standing", t);
            rec[3] &= ~0x400;
        } /* (Courier's civilians at the counter and behind the glass keep their place, as the mission has them) */
    }
    {
        s32 w3 = rec[3], w4 = rec[4];
        u8 *o = func_8003C1BC_orig(rec, x, y, z);
        if (o && port_endless_room(e) && TYPE_DEF(t)[1] == 6 && (u8 *)o >= (u8 *)D_800AFFC8 &&
            (u8 *)o < (u8 *)D_800AFFC8 + 64 * 0x124) {
            int i = ((u8 *)o - (u8 *)D_800AFFC8) / 0x124, k;
            const char *how = making == 1 ? "sent" : making == 2 ? "back (a fallen fixed guard)" : "placed";
            for (k = 0; k < 16; k++)
                if (freed_recs[k] == &rec[3])
                    how = "placed again (its slot freed before)";
            made_how[i & 63] = how;
            if (*(s32 *)(o + 0x4C) & 0x02000000) { /* (made inactive: left out of the game's collisions and updates,
                                                    * so no shot touches him; the mission's script would wake him) */
                if (!port_subframe)
                    ELOG("room: person type %d at %d %d %d made inactive (+4C %08X): left out", t, x, y, z,
                         *(s32 *)(o + 0x4C));
                func_800395E8(o);
                rec[3] |= 0x100;
                return 0;
            }
            if (!port_subframe)
                ELOG("made: person %d type %d %s at %d %d %d, record %08X flags %08X w4 %08X; ai %d +48 %08X +4C %08X", i, t,
                     how, x, y, z, (u32)&rec[3], w3, w4, o[0xA7], *(s32 *)(o + 0x48), *(s32 *)(o + 0x4C));
        }
        return o;
    }
}

/* A person hit: one that is hit again and again without being hurt (a story person the game won't let die,
 * under a guard's look) is taken out of the room */
void func_8005E020(u8 *o) {
    func_8005E020_orig(o); /* (its old backstop, people hit unhurt taken out, took ordinary guards: gone) */
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
        /* (marked seen, as playing it marks it: the mission's scripts look at that, the bank's receptionist taking
         * her seat at her desk once the opening has played) */
        if (nis >= 1 && nis <= 16)
            *(u32 *)(D_800D90C0 + 0x124) |= 1u << (4 + nis - 1);
        ELOG("story: cutscene %d skipped (marked seen)", nis);
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
                if (dist2(p[0] + D_800D2FB0[0], p[2] + D_800D2FB0[2], g_Args->playerX, g_Args->playerZ) < 4000)
                    ELOG("dump: by the entry: type %d class %d/%d at %d %d %d flags %08X cb %08X", p[3] & 0xFF,
                         TYPE_DEF(p[3] & 0xFF)[1], TYPE_DEF(p[3] & 0xFF)[2], p[0] + D_800D2FB0[0], p[1] + D_800D2FB0[1],
                         p[2] + D_800D2FB0[2], p[3], *(u32 *)(TYPE_DEF(p[3] & 0xFF) + 0x38));
                if (TYPE_DEF(p[3] & 0xFF)[1] == 6)
                    ELOG("dump: placed person type %d at %d %d %d flags %08X w4 %08X w5 %08X", p[3] & 0xFF,
                         p[0] + D_800D2FB0[0], p[1] + D_800D2FB0[1], p[2] + D_800D2FB0[2], p[3], p[4], p[5]);
            }
    }
    if (D_8009CDA8 % 100 == 0)
        for (i = D_800B0DA0; i <= D_800B07B0; i++)
            if (OBJ(i)[4])
            {
                u8 *o = OBJ(i);
                s32 *src = *(s32 **)(o + 0x60);
                ELOG("dump: person %d type %d health %d ai %d at %d %d %d floor %d; record %08X flags %08X w4 %08X; "
                     "obj +8 %d +A4 %02X +48 %08X%s", i, o[6], HEALTH(o), o[0xA7], POS(o)[0], POS(o)[1], POS(o)[2],
                     floor_at(POS(o)[0], POS(o)[1] - 500, POS(o)[2]), (u32)src, src ? src[0] : 0, src ? src[1] : 0,
                     o[8], o[0xA4], *(s32 *)(o + 0x48), is_copy(o) ? " (a copy)" : "");
                if (getenv("TWINE_ENDLESS_DUMP_TYPE") && o[6] == atoi(getenv("TWINE_ENDLESS_DUMP_TYPE"))) {
                    char hex[0x124 * 2 + 1], hex2[0x80 * 2 + 1];
                    u8 *m = *(u8 **)(o + 0x90), *st = *(u8 **)(o + 0x94);
                    for (j = 0; j < 0x124; j++)
                        snprintf(hex + j * 2, 3, "%02X", o[j]);
                    ELOG("dump: object %d: %s", i, hex);
                    for (j = 0; j < 0x80 && m; j++)
                        snprintf(hex2 + j * 2, 3, "%02X", m[j]);
                    ELOG("dump: its +90: %s", m ? hex2 : "-");
                    for (j = 0; j < 0x80 && st; j++)
                        snprintf(hex2 + j * 2, 3, "%02X", st[j]);
                    ELOG("dump: its +94: %s", st ? hex2 : "-");
                }
            }
    if (D_800D2F90 != last_load) {
        s16 *to = *(s16 **)(D_8009E9C4 + 8);
        ELOG("dump: load state %d -> %d (to %d)", last_load, D_800D2F90, to ? to[5] : -1);
        last_load = D_800D2F90;
    }
}
