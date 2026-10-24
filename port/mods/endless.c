/* Endless mode (native PC build only): the level's part of the run (port/src/endless.c keeps the run and starts
 * each room's mission at the room's point).
 *
 * A room is a section of the mission. It keeps its own guards, and more come on top of them: a director spends a
 * budget that grows with the time spent in the room and with the wave (the rooms cleared, plus one), sending a copy
 * of one of the section's own guards to one of their posts (their places in the section's lists) that is far from
 * Bond and further on than he is from where he came in, so that none appear behind him. A copy is made the way the
 * section makes its own: from its record (func_8003C1BC; the record is the copy's, kept here).
 *
 * The room's doors stay shut until the fight is over: its own guards down, or enough kills, or after a while. Then
 * any door to another section (one the mission asks the map to load: D_800D2F90 2, through the door's load point)
 * leaves the room, but for the one Bond came in by. The mission's main then returns (g_Args' flags 0x40, which its
 * frame loop's top checks) and the next room's mission starts in its place (main.c, or the boot stub), with Bond's
 * weapons, ammo, health and armour carried across: each weapon by its slot's name, which is the same in every
 * mission that has it.
 *
 * The mission's own ends are left out: its objectives neither complete it (func_8002E1B0) nor fail it
 * (func_8002D4E4, but for Bond's death, GAME OVER). When Bond dies the mission ends as when RESTART? is answered
 * NO, without asking, and the front end's mission stats show the run (port/src/endless.c, fixes/shell.c).
 *
 * endless_frame is called at the top of every frame of the mission (mods/savestate.c's Pad_Update).
 * TWINE_ENDLESS_DUMP=1 logs the section's doors, its kinds of placed things and the loads asked for.
 *
 * MOD_REPLACES_NATIVE: func_8002D4E4 func_8002E1B0 RestartPrompt_Open
 *
 * Names are LEVEL01's; port/mods.py maps them onto each level. */
#include "twine.h"
#include "../src/endless.h"

char *getenv(const char *name);
int snprintf(char *s, unsigned n, const char *fmt, ...);
int atoi(const char *s);
void *memcpy(void *d, const void *s, unsigned n);
char *strncpy(char *d, const char *s, unsigned n);
int strncmp(const char *a, const char *b, unsigned n);
char *strchr(const char *s, int c);
void port_log(const char *fmt, ...);


extern s32 D_800D2F90;   /* the map's loading: 0 none, 2 a load point's section asked for, ... */
extern s32 D_800D2F8C;   /* the section loaded */
extern s16 D_8009E9C4[]; /* the start point (section, yaw, x, y, z); +0x10 the load point to go to */
extern s32 D_8009CDA8;   /* the mission's frame counter */
extern s16 D_8009CF68;   /* the section's lists of placed things: how many lists */
extern s32 **D_8009CF70; /* each list (6 words a thing: x, y, z from the section's origin, flags, w4, w5) */
extern s32 *D_8009CF6C;  /* each list's count */
extern s32 D_800D2FB0[]; /* the section's origin */
extern s32 D_8009CFF0;   /* Bond's armour (0x20000 full) */
extern s32 D_8009CFFC;   /* the fade: 1 out, -1 in */
extern s32 D_800AFFC8;   /* the objects (0x124 bytes each) */
extern s16 D_800B0DA0, D_800B07B0; /* the first and last of the people among them */
extern s16 D_8009D104, D_8009D106; /* the object types of the light and the heavy armour */
extern s16 D_8009D108, D_8009D10A, D_8009D10C; /* ... of the ammo boxes (their +0x34: whose ammo) */
extern s32 D_8009D130[];  /* the weapons' pickups, 19 of 4 words: weapon slot, object type, messages */
u8 *func_8003C1BC(s32 *rec, s32 x, s32 y, s32 z); /* makes a placed thing from its record */
u8 *func_80039A9C(s32 arg0, u8 type, s32 kind); /* makes an object (kind 2: a pickup) at the place below */
void func_8005E020(u8 *o); /* a person hit: hurt, or falls */
void func_8004B270(s32 damage, s32 through_armour, s32 always); /* Bond hurt */
#define MAKE_AT ((s32 *)0x1F8003D4u) /* (the scratchpad's x, y, z) */
void func_80048B4C(char *text, s32 arg1, s32 arg2); /* a message on the HUD */
void func_80030FA8(s16 *at);
extern s32 D_800E2F54;   /* the section's floor (its contour polygons) */
s32 func_800705A8(s32 floor, s16 *pos, s32 arg2, s32 arg3); /* the floor's height under pos, -0x7FFF none */ /* puts Bond at a point (section, yaw, x, y, z) */

extern EndlessCarry port_endless_carry;
int port_endless_room(s16 *entry);
int port_endless_wave(void);
void port_endless_next(void);
void port_endless_unrank(const char *why);
int port_endless_room_started(void);
void port_endless_score(int points);
extern s32 g_DevMenu[];

#define OBJ(i) ((u8 *)D_800AFFC8 + (i) * 0x124)
#define TYPE_DEF(t) ((u8 *)g_Args->unk228 + (t) * 0x70) /* the object types: +1 their class (6 people) */

static void dump(void);
/* the HUD's messages (in writable memory: the game's own are in its data) */
static char msg_open[] = "EXIT OPEN", msg_locked[] = "EXIT LOCKED", msg_back[] = "NOT THAT WAY", msg_wave[16];
s32 func_8002D4E4_orig(s32 text);
s32 func_8002E1B0_orig(void);
void RestartPrompt_Open_orig(void);
extern s16 D_8009CAA4; /* a menu over the mission (its frame loop runs the menu alone) */

/* The guards to copy: one record of each kind of the mission's guards (people of whom it places more than one: not
 * Bond, nor the one-offs who are his allies or his targets). The lists hold the whole mission's things, each
 * section's in its own part of the world. */
#define KINDS 8
static s32 kinds[KINDS][6];
static int kind_count;
/* where they may come: places on the section's floor (its contour polygons' middles) */
#define SPOTS 256
static s32 spots[SPOTS][3];
static int spot_count;
/* the copies' records (the objects made from them keep pointing at them: +0x60) */
#define COPIES 64
static s32 copies[COPIES][6];
static int copy_next;

static s16 entry[5];
static s32 was_alive[64]; /* each person's health last frame (a guard falling leaves pickups) */
static s32 was_at[3], said; /* where Bond was as the last frame began; when a message was last given */
static s32 last_frame = -1, room_frames, carried, kills_at_start, kills_scored, budget, cooldown, open;
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

/* the people alive (but Bond), and how many of those are the section's own */
static int alive(int *own) {
    int i, n = 0;
    *own = 0;
    for (i = D_800B0DA0; i <= D_800B07B0; i++) {
        u8 *o = OBJ(i);
        s32 *src = *(s32 **)(o + 0x60);
        if (o == g_Player || !o[4] || *(s32 *)(o + 0x34) <= 0)
            continue;
        n++;
        if (!(src >= copies[0] && src < copies[COPIES]))
            (*own)++;
    }
    return n;
}

/* Bond as he leaves the room, for the next (the weapons this mission doesn't have kept as they were) */
static void carry_out(void) {
    EndlessCarry *c = &port_endless_carry;
    int i, j;
    c->valid = 1;
    c->health = *(s32 *)((u8 *)g_Player + 0x34);
    c->armour = D_8009CFF0;
    for (i = 0; i < 6; i++) {
        Weapon *w = &g_Weapons[i];
        if (!w->pad0[0])
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
}

/* ... and as he comes into this one: what he had of this mission's weapons, and more of an ammo than it gives */
static void carry_in(void) {
    EndlessCarry *c = &port_endless_carry;
    int i, j;
    if (!c->valid)
        return;
    if (c->health > 0)
        *(s32 *)((u8 *)g_Player + 0x34) = c->health;
    D_8009CFF0 = c->armour;
    for (i = 0; i < 6; i++) {
        Weapon *w = &g_Weapons[i];
        for (j = 0; j < c->weapons; j++)
            if (w->pad0[0] && !strncmp(c->weapon[j].name, (char *)w->pad0, 23))
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

/* the section's floor's height under (x, z) (-0x7FFF: none of it there) */
static s32 floor_at(s32 x, s32 y, s32 z) {
    s16 at[4];
    if (x < -0x7FFF || x > 0x7FFF || z < -0x7FFF || z > 0x7FFF)
        return -0x7FFF;
    at[0] = x, at[1] = y, at[2] = z, at[3] = 0;
    return func_800705A8(D_800E2F54, at, 0, 0);
}

/* the room as the mission starts: the kinds of guards and the places they may come to */
static void room_start(void) {
    s32 i, j, counts[256];
    u8 *mesh = (u8 *)D_800E2F54;
    room_frames = carried = budget = open = kills_scored = said = 0;
    kind_count = spot_count = copy_next = 0;
    cooldown = 20 * 8;
    kills_at_start = *(u16 *)((u8 *)g_Args + 0x2C4);
    rng ^= D_8009CDA8 * 2654435761u ^ entry[2];
    for (i = 0; i < 256; i++)
        counts[i] = 0;
    for (i = 0; i < D_8009CF68; i++)
        for (j = 0; j < D_8009CF6C[i]; j++)
            counts[D_8009CF70[i][j * 6 + 3] & 0xFF]++;
    for (i = 0; i < D_8009CF68; i++)
        for (j = 0; j < D_8009CF6C[i]; j++) {
            s32 *p = D_8009CF70[i] + j * 6, k;
            u32 t = p[3] & 0xFF;
            if (!t || TYPE_DEF(t)[1] != 6 || counts[t] < 2 || t == ((u8 *)g_Player)[6] || kind_count == KINDS)
                continue;
            for (k = 0; k < kind_count && (kinds[k][3] & 0xFF) != t; k++)
                ;
            if (k == kind_count)
                memcpy(kinds[kind_count++], p, sizeof(kinds[0]));
        }
    /* the floor: its polygons (+0x24, 8 bytes each: three corners and the normal) over its corners (+0x1C) */
    if (mesh) {
        s32 faces = *(s16 *)(mesh + 0x10), step = faces / SPOTS + 1;
        for (i = rnd() % step; i < faces && spot_count < SPOTS; i += step) {
            u16 *f = (u16 *)(*(u8 **)(mesh + 0x24) + i * 8);
            s16 *v0 = (s16 *)(*(u8 **)(mesh + 0x1C) + f[0] * 8), *v1 = (s16 *)(*(u8 **)(mesh + 0x1C) + f[1] * 8),
                *v2 = (s16 *)(*(u8 **)(mesh + 0x1C) + f[2] * 8);
            s32 x = (v0[0] + v1[0] + v2[0]) / 3, y = (v0[1] + v1[1] + v2[1]) / 3, z = (v0[2] + v1[2] + v2[2]) / 3, h;
            if ((h = floor_at(x, y - 200, z)) == -0x7FFF || h < y - 300) /* (a floor above it: under the stairs) */
                continue;
            spots[spot_count][0] = x;
            spots[spot_count][1] = h;
            spots[spot_count][2] = z;
            spot_count++;
        }
    }
    if (port_endless_room_started() > 1)
        port_endless_unrank("the room started over");
    port_log("endless: room (section %d, wave %d): %d kinds of guards, %d places; people %d-%d\n", D_800D2F8C,
             port_endless_wave(), kind_count, spot_count, D_800B0DA0, D_800B07B0);
}

/* A copy of one of the kinds of guards at one of the places: far from Bond, and further from where he came in than
 * he is. */
static void spawn(void) {
    s32 bx = g_Args->playerX, bz = g_Args->playerZ, at = dist2(bx, bz, entry[2], entry[4]);
    int i, n = 0, pick[SPOTS];
    s32 *rec;
    if (!kind_count)
        return;
    s32 fx = isin(gPsi), fz = isin(gPsi + 0x400); /* the way he looks */
    for (i = 0; i < spot_count; i++) {
        s32 dx = spots[i][0] - bx, dz = spots[i][2] - bz, far = dist2(bx, bz, spots[i][0], spots[i][2]);
        s32 on = dist2(entry[2], entry[4], spots[i][0], spots[i][2]);
        /* in front of him (within some 50 degrees) a place must be further off, past what he can make out */
        s32 seen = far > 0 && (dx / 16 * fx + dz / 16 * fz) / (far / 16 + 1) > 4096 * 2 / 3;
        if (getenv("TWINE_ENDLESS_TEST_NEAR") ? far > 1500 && far < 4000
                                              : far > (seen ? 12000 : 6000) && far < 22000 && on > at + 1500)
            pick[n++] = i; /* (headless tests: near him) */
    }
    if (!n)
        return;
    i = pick[rnd() % n];
    rec = copies[copy_next];
    copy_next = (copy_next + 1) % COPIES;
    memcpy(rec, kinds[rnd() % kind_count], sizeof(copies[0]));
    rec[0] = spots[i][0] - D_800D2FB0[0];
    rec[1] = spots[i][1] - D_800D2FB0[1];
    rec[2] = spots[i][2] - D_800D2FB0[2];
    rec[3] &= ~0x300;           /* not made yet; not asleep */
    rec[4] &= ~(0x1F << 7);     /* no path to walk (the kind's was in its own section) */
    if (func_8003C1BC(rec, spots[i][0], spots[i][1], spots[i][2]))
        port_log("endless: frame %d: a guard (type %d) at %d %d %d\n", D_8009CDA8, rec[3] & 0xFF, spots[i][0],
                 spots[i][1], spots[i][2]);
}

/* A pickup of this mission's at (x, y, z) (none if it has none of that type) */
static u8 *pickup(s32 type, s32 x, s32 y, s32 z) {
    if (type <= 0 || type > 0xFF)
        return 0;
    MAKE_AT[0] = x, MAKE_AT[1] = y, MAKE_AT[2] = z;
    return func_80039A9C(0, type, 2);
}

/* What a guard leaves as he falls: armour, ammo for one of the mission's weapons, now and then a weapon */
static void drop(u8 *o) {
    s32 *pos = (s32 *)(*(u8 **)(o + 0x90) + 0x28), x = pos[0], y = pos[1] - 100, z = pos[2], r = rnd() % 100, i, n;
    s32 weapons[19];
    u8 *box;
    for (i = n = 0; i < 19; i++)
        if (D_8009D130[i * 4] >= 0 && D_8009D130[i * 4 + 1] > 0)
            weapons[n++] = i;
    if (r < 8)
        pickup(D_8009D106, x, y, z);
    else if (r < 35)
        pickup(D_8009D104 > 0 ? D_8009D104 : D_8009D106, x, y, z);
    else if (r < 50 && n)
        pickup(D_8009D130[weapons[rnd() % n] * 4 + 1], x, y, z);
    /* and ammo, mostly */
    if (rnd() % 100 < 70 && n && (box = pickup(D_8009D108, x + 150, y, z + 150)))
        *(s32 *)(box + 0x34) = weapons[rnd() % n];
    port_log("endless: frame %d: a guard down at %d %d %d (%d)\n", D_8009CDA8, x, y, z, r);
}

void endless_frame(void) {
    int own, n, wave;
    s32 kills, back;
    if (getenv("TWINE_ENDLESS_DUMP"))
        dump();
    if (!port_endless_room(entry) || !g_Player)
        return;
    if (D_8009CDA8 < last_frame || last_frame < 0)
        room_start();
    last_frame = D_8009CDA8;
    if (g_CutScene)
        return;
    if (!carried) {
        int i;
        carried = 1;
        carry_in();
        for (i = 0; i < 64; i++)
            was_alive[i] = 0;
        snprintf(msg_wave, sizeof(msg_wave), "WAVE %d", port_endless_wave());
        func_80048B4C(msg_wave, 0x80, 0);
        /* the first room: a heavy armour by Bond */
        if (port_endless_wave() == 1) {
            s32 x = g_Args->playerX + (isin(gPsi) * 700 >> 12), z = g_Args->playerZ + (isin(gPsi + 0x400) * 700 >> 12);
            s32 y = floor_at(x, g_Args->playerY - 500, z);
            pickup(D_8009D106, x, y == -0x7FFF ? g_Args->playerY : y, z);
        }
    }
    room_frames++;
    /* the debug menu, the game's cheats (used, or on: g_Args +4 bits 3, 9-16) or the port's take the run off the
     * high scores */
    if (g_DevMenu[0])
        port_endless_unrank("the debug menu");
    if (*(u32 *)((u8 *)g_Args + 4) & 0x1FE08)
        port_endless_unrank("cheats");
    if (getenv("TWINE_INFINITE_GRENADES") && atoi(getenv("TWINE_INFINITE_GRENADES")))
        port_endless_unrank("infinite grenades");
    if (getenv("TWINE_ENDLESS_DUMP") && room_frames % 100 == 0)
        port_log("endless: room frame %d health %d armour %d\n", room_frames, *(s32 *)((u8 *)g_Player + 0x34),
                 D_8009CFF0);
    wave = port_endless_wave();
    kills = *(u16 *)((u8 *)g_Args + 0x2C4) - kills_at_start;
    /* the score: a kill is worth more the sooner it comes after Bond came in (twice as much at once, down to its
     * worth after a minute): moving on pays, camping doesn't */
    if (kills > kills_scored) {
        s32 bonus = room_frames < 20 * 60 ? 100 - room_frames * 100 / (20 * 60) : 0;
        port_endless_score((kills - kills_scored) * (100 + bonus) * (10 + wave) / 10);
        kills_scored = kills;
    }
    if (getenv("TWINE_ENDLESS_TEST_KILL") && room_frames % atoi(getenv("TWINE_ENDLESS_TEST_KILL")) == 0) {
        int i; /* (headless tests: the guards fall) */
        for (i = D_800B0DA0; i <= D_800B07B0; i++)
            if (OBJ(i) != g_Player && OBJ(i)[4] && *(s32 *)(OBJ(i) + 0x34) > 0) {
                *(s32 *)(OBJ(i) + 0x34) = 0;
                func_8005E020(OBJ(i));
            }
    }
    n = alive(&own);
    {
        int i;
        for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) {
            u8 *o = OBJ(i);
            s32 h = o[4] ? *(s32 *)(o + 0x34) : 0;
            if (o != g_Player && was_alive[i] > 0 && h <= 0 && o[4])
                drop(o);
            was_alive[i] = h;
        }
    }

    /* the director: more the longer Bond stays and the further he has come, as many at once as the wave allows */
    budget += 2 + wave;
    if (cooldown > 0)
        cooldown--;
    if (!cooldown && budget >= 20 * 20 && n < 2 + wave && n < 12) {
        spawn();
        budget = 0;
        cooldown = 20 * 3 + rnd() % (20 * 4);
    }

    /* the doors: open once the room's own guards about are down (and one at least), or after kills enough, or a
     * while */
    if (!open && ((!own && kills && room_frames > 20 * 20) || kills >= 3 + wave || room_frames > 20 * 90)) {
        open = 1;
        func_80048B4C(msg_open, 0x80, 0);
    }
    if (getenv("TWINE_ENDLESS_TEST_DIE") && room_frames == atoi(getenv("TWINE_ENDLESS_TEST_DIE")) &&
        (!strchr(getenv("TWINE_ENDLESS_TEST_DIE"), '@') || wave == atoi(strchr(getenv("TWINE_ENDLESS_TEST_DIE"), '@') + 1)))
        func_8004B270(0x40000, 1, 1); /* (headless tests: Bond hit hard, at frame N or N@wave) */
    if (getenv("TWINE_ENDLESS_TEST_LOCKED") && room_frames >= 100 && room_frames < 140)
        D_800D2F90 = 2; /* (headless tests: a shut door, walked into again and again) */
    if (getenv("TWINE_ENDLESS_TEST_EXIT") && room_frames == atoi(getenv("TWINE_ENDLESS_TEST_EXIT")))
        open = 1, D_800D2F90 = 2, room_frames = 20 * 60; /* (headless tests: as if Bond went through a door) */
    if (D_800D2F90 == 2 && !(g_Args->flags & 0x40)) {
        back = dist2(g_Args->playerX, g_Args->playerZ, entry[2], entry[4]) < 3000 && room_frames < 20 * 60;
        if (open && !back) { /* the room cleared: its bonus, more for a quick one */
            port_endless_score(500 + 250 * wave + (room_frames < 20 * 90 ? (20 * 90 - room_frames) / 2 : 0));
            carry_out();
            port_endless_next();
            g_Args->flags |= 0x40;
        } else { /* shut: the load the door asked for undone, and Bond back out of the doorway */
            s32 *pos = (s32 *)(*(u8 **)((u8 *)g_Player + 0x90) + 0x28);
            D_800D2F90 = 0;
            D_8009CFFC = -1;
            *(s32 *)(D_8009E9C4 + 8) = 0;
            pos[0] = was_at[0], pos[1] = was_at[1], pos[2] = was_at[2];
            if (room_frames - said > 20 * 3 || room_frames < said) {
                func_80048B4C(back ? msg_back : msg_locked, 0x80, 0);
                said = room_frames;
            }
        }
    }
    {
        s32 *pos = (s32 *)(*(u8 **)((u8 *)g_Player + 0x90) + 0x28);
        was_at[0] = pos[0], was_at[1] = pos[1], was_at[2] = pos[2];
    }
}

/* The mission failed (text: why): in a run, only by Bond's death */
s32 func_8002D4E4(s32 text) {
    s16 e[5];
    if (port_endless_room(e) && g_Player && *(s32 *)((u8 *)g_Player + 0x34) > 0)
        return 0;
    return func_8002D4E4_orig(text);
}

/* The mission completed: never in a run */
s32 func_8002E1B0(void) {
    s16 e[5];
    if (port_endless_room(e))
        return 0;
    return func_8002E1B0_orig();
}

/* RESTART? after Bond's death: in a run, NO at once (the prompt's own NO: func_80042020) */
void RestartPrompt_Open(void) {
    s16 e[5];
    if (!port_endless_room(e)) {
        RestartPrompt_Open_orig();
        return;
    }
    D_8009CAA4 = 0;
    g_Args->flags |= 0x80;
}

/* TWINE_ENDLESS_DUMP: the section's doors and kinds of things as the mission starts, Bond's place now and then,
 * and the loads asked for */
static void dump(void) {
    static s32 last_load = -1, last = -1, dumped;
    s32 i, j, counts[256];
    if (D_8009CDA8 < last)
        last_load = -1, dumped = 0;
    last = D_8009CDA8;
    if (!dumped || (D_8009CDA8 % 100) == 0) {
        s16 at[4];
        at[0] = g_Args->playerX, at[1] = g_Args->playerY - 1000, at[2] = g_Args->playerZ, at[3] = 0;
        port_log("endless: frame %d section %d cut %d pos %d %d %d yaw %d floor %d\n", D_8009CDA8, D_800D2F8C, g_CutScene,
                 g_Args->playerX, g_Args->playerY, g_Args->playerZ, gPsi, func_800705A8(D_800E2F54, at, 0, 0));
    }
    if (!dumped++) {
        for (i = 0; i < 256; i++)
            counts[i] = 0;
        for (i = 0; i < D_8009CF68; i++)
            for (j = 0; j < D_8009CF6C[i]; j++) {
                s32 *p = D_8009CF70[i] + j * 6;
                u32 t = p[3] & 0xFF;
                counts[t]++;
                if (TYPE_DEF(t)[2] == 0x22)
                    port_log("endless: door type %d at %d %d %d to %d heading %d cb %08X\n", t, p[0] + D_800D2FB0[0],
                             p[1] + D_800D2FB0[1], p[2] + D_800D2FB0[2], (u32)p[3] >> 27, (p[3] >> 10) & 0xF00,
                             *(u32 *)(TYPE_DEF(t) + 0x38));
            }
        port_log("endless: pickups: armour %d %d, ammo boxes %d %d %d\n", D_8009D104, D_8009D106, D_8009D108,
                 D_8009D10A, D_8009D10C);
        for (i = 0; i < 19; i++)
            port_log("endless: pickup %d: slot %d type %d\n", i, D_8009D130[i * 4], D_8009D130[i * 4 + 1]);
        for (i = 0; i < 256; i++)
            if (counts[i])
                port_log("endless: type %d x%d class %d/%d cb %08X fl %08X\n", i, counts[i], TYPE_DEF(i)[1],
                         TYPE_DEF(i)[2], *(u32 *)(TYPE_DEF(i) + 0x38), *(u32 *)(TYPE_DEF(i) + 0x3C));
    }
    if (D_800D2F90 != last_load) {
        s16 *to = *(s16 **)(D_8009E9C4 + 8);
        port_log("endless: frame %d load state %d -> %d (to %d)\n", D_8009CDA8, last_load, D_800D2F90,
                 to ? to[5] : -1);
        last_load = D_800D2F90;
    }
}
