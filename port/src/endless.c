/* Endless mode: one run of rooms until Bond dies. A room is a section of one of the missions, entered where the
 * mission itself enters it (its start point or one of its load points) and left through any of the section's
 * doors to another section (port/mods/endless.c, in the level). Then the next room's mission is started in the
 * place of the one that ran, Bond's state carried across.
 *
 * Each level keeps where it starts in its data: its start point (section, yaw, x, y, z: the record the level's
 * loader reads when it starts, func_80032074 in LEVEL01) and after it its load points, 0x10 bytes each (the same
 * five, then the point's id, which the doors name, and a sound the mission plays as Bond arrives there through a door:
 * an id of its sound table, func_80026F30; not the music, which is the mission's own logic: port/mods/endless.c,
 * func_80026D08). A room's mission starts at the room's point
 * when that point is copied over the start point before the mission's main runs.
 *
 * TWINE_ENDLESS=1 starts a run (with TWINE_LEVEL unset or naming the first room's level, for tests);
 * TWINE_ENDLESS_ROOM=level:point starts it in that room (point -1: the start point); TWINE_ENDLESS_NEXT=l:p,l:p...
 * (rooms of the table) chooses the rooms after it, in order. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <sys/time.h>
#include "port.h"
#include "module.h"
#include "endless.h"

/* each level's start point (its load points follow it, from +0x18) */
static const u32 start_point[12] = {0,          0x8009E9C4, 0x800A3E74, 0x800A0ED4, 0x8009FF14, 0x800A382C,
                                    0x800A1584, 0x800A4C3C, 0x800A198C, 0x800A4004, 0x800A369C, 0x8009DAE8};

typedef struct {
    s8 level, point; /* point: the load point (-1 the start point) */
    const char *need; /* a weapon Bond must carry (owned, with ammo) for the room to be chosen; 0 none */
} Room;

/* Switches for rooms that depend on other work (tools/endless_check.py's pool follows them): Night Watch's rooms,
 * kept only with its action music in a run (the playtest's ruling; wt/music); City of Walkways' helicopter (5:1),
 * whose end is the mission's fanfare then the next room (port/mods/endless.c's func_8002E1B0) */
#ifndef NIGHT_WATCH_ROOMS
#define NIGHT_WATCH_ROOMS 1
#endif
#ifndef HELICOPTER_ROOM
#define HELICOPTER_ROOM 1
#endif

/* The rooms: sections of the missions that make a fight with a way out (a door to another section that opens
 * without the story: no keycard, objective, lift or wait), picked by playing them. A section may be entered from
 * either end (King's Ransom's 4, played backwards, is one of the best). Left out: Russian Roulette (the casino),
 * Cold Reception (the ski slope and the rest, played: none kept), the sections that start in a cutscene or where
 * Bond dies of the place alone, rooms whose doors the story keeps shut, the lifts. The rest of each mission's points
 * were each played (2026-10-27's tour) and kept or left out by the player's verdict.
 * King's Ransom flows: from one of its rooms the next of its sections follows, for a stretch (port_endless_segment);
 * so does Turncoat's subway: the station (5) and the train's four cars (4 to 1, played back from the station), car
 * to car by their end doors, the last car left by its far end (port/mods/endless.c's extra exits). */
static Room rooms[] = {
    /* Courier: the bank's floor and the later part (its lift arrivals left out) */
    {2, -1}, {2, 4}, {2, 5}, {2, 6},
    /* and section 1 by its far door (7) */
    {2, 7},
    /* King's Ransom: all of it but its end (section 3: the balloon, scripted, no way on), the bar's two doors (5)
     * and the section played backwards (4) among the best */
    {8, -1}, {8, 0}, {8, 2}, {8, 3}, {8, 4}, {8, 5}, {8, 6},
    /* Night Watch (its guards may not be killed in the story; in a run they may, and its failures are skipped):
     * sections 2 (0) and 4 (4), and the small section 6 (8: its guards come in by Bond's own door too, the level
     * mod's every_door). The rest played and left out (1, 2, 3, 5, 6) */
#if NIGHT_WATCH_ROOMS
    {4, 0}, {4, 4}, {4, 8},
#endif
    /* Masquerade: the nuclear facility (6 was empty and 7's doors shut before the slots and the story gates were
     * mended: back on trial) */
    {7, 2}, {7, 6}, {7, 7},
    /* and its start (-1, section 1: entered from its far end (1) it has no door near) and section 2 from its middle (3).
     * (0, 4 and 5 left out: section 2's far end spawned a guard on no floor; 4 is small and Renard's; 5 sent nobody) */
    {7, -1}, {7, 3},
    /* Flashpoint: the night-vision pipeline (13); section 1, its start, entered from each of its doors (-1, 1, 3, 5,
     * 6: M left out, MI6's alert off), section 3 from either end and its side (2, 8, 11), section 4 (10), section 5
     * (7), section 6 (12). (4, section 4's other end, had no fight reach Bond headless: not played) */
    {6, 0}, {6, 9}, {6, 13},
    {6, -1}, {6, 1}, {6, 3}, {6, 5}, {6, 6}, {6, 2}, {6, 8}, {6, 11}, {6, 10}, {6, 7}, {6, 12},
    /* City of Walkways: the warehouse (4), section 2 (0) */
    {5, -1}, {5, 4}, {5, 0},
#if HELICOPTER_ROOM
    /* the helicopter (section 3): only with a rocket launcher; its gas valve worked by the use button, no wrench
     * (port/mods/endless.c) */
    {5, 1, "ROCKET LAUNCHER"},
#endif
    /* Turncoat: the streets, and the subway. The campaign's way: down from the street into the first car (12), car
     * to car (0, 2, 4), out by the last car's middle door (7's) to the station (6), the bomb. Played back: the
     * station's far end (9: the bomb's timer and MI6's messages off, port_endless_story_off; its grenade door on to
     * the train), into the last car and on to the first (5, 3, 1). (The station's near end (6) is passed through
     * from 9; 8 is the story's, 10 and 14 empty.) */
    {3, -1}, {3, 11}, {3, 13},
    {3, 12}, {3, 0}, {3, 2}, {3, 4}, {3, 7}, {3, 9}, {3, 5}, {3, 3}, {3, 1},
    /* Fallen Angel: the MS keycard area and the castle's halls; section 1 by its other door (1) */
    {9, 0}, {9, 2}, {9, 4}, {9, 1},
    /* Meltdown; sections 2 and 1 by their other doors (0, 3) */
    {10, -1}, {10, 1}, {10, 2}, {10, 4}, {10, 6}, {10, 0}, {10, 3},
    {0, 0}, /* (TWINE_ENDLESS_ROOM's, when not one of these) */
};
#define ROOMS ((int)(sizeof(rooms) / sizeof(rooms[0])) - 1)

/* the mission each level folder holds (main.c's) */
static const s16 level_mission[12] = {-1, 2, 0, 8, 4, 7, 6, 5, 1, 9, 10, 3};

int port_endless; /* a run is on */
EndlessCarry port_endless_carry;
EndlessRun port_endless_run;
int port_endless_unranked PORT_NOSAVE; /* (a state loaded must not take it back) */
char port_endless_unranked_why[32] PORT_NOSAVE; /* (why: for the stats screen) */
int port_endless_invincible; /* (the debug menu's or the developers' code's invincibility, on as Bond left the last
                              * room: a mission's own flag is new in each, port/mods/endless.c puts it back on) */
int port_endless_stats; /* the front end shows the run's stats next */
static int room = -1, next_room = -1, room_starts, segments;
static s16 cur_entry[5]; /* where the room was entered: its section, yaw, x, y, z */
static void run_over(u8 *args);
static u32 seed;

static u32 next_rand(void) {
    seed = seed * 1103515245 + 12345;
    return seed >> 8;
}

void port_endless_logf(s32 frame, const char *fmt, ...);

/* a room Bond may be sent to now: what it needs, he carries (the carry, as he leaves the room before) */
static int room_ok(int i) {
    int k;
    const EndlessCarry *c = &port_endless_carry;
    if (!rooms[i].need)
        return 1;
    for (k = 0; c->valid && k < c->weapons && k < ENDLESS_WEAPONS; k++)
        if (c->weapon[k].owned && !strncmp(c->weapon[k].name, rooms[i].need, sizeof(c->weapon[k].name)) &&
            (c->weapon[k].ammo > 0 || c->weapon[k].clip > 0 || c->weapon[k].reserve > 0))
            return 1;
    return 0;
}

/* a room of another level than the one before: the level first (each as likely), then one of its rooms (of those
 * whose needs Bond meets) */
static int pick_room(int after) {
    int levels[12], n = 0, i, l, k = 0, r;
    static int forced; /* (TWINE_ENDLESS_NEXT=level:point,...: headless tests, the rooms after the first, in order) */
    const char *f = getenv("TWINE_ENDLESS_NEXT");
    if (f && after >= 0) {
        for (i = 0; i < forced && f; i++)
            f = strchr(f, ',') ? strchr(f, ',') + 1 : 0;
        if (f && *f) {
            forced++;
            l = atoi(f), r = strchr(f, ':') ? atoi(strchr(f, ':') + 1) : -1;
            for (i = 0; i < ROOMS; i++)
                if (rooms[i].level == l && rooms[i].point == r)
                    return i;
        }
    }
    for (l = 1; l <= 11; l++)
        for (i = 0; i < ROOMS; i++)
            if (rooms[i].level == l && room_ok(i)) {
                if (after < 0 || rooms[after].level != l)
                    levels[n++] = l;
                break;
            }
    l = levels[next_rand() % n];
    for (i = 0; i < ROOMS; i++)
        k += rooms[i].level == l && room_ok(i);
    r = next_rand() % k;
    for (i = 0; i < ROOMS; i++)
        if (rooms[i].level == l && room_ok(i) && !r--)
            return i;
    return 0;
}

/* a run begun: nothing carried, nothing done, the first room chosen */
static void begin(void) {
    seed = getenv("TWINE_ENDLESS_SEED") ? (u32)atoi(getenv("TWINE_ENDLESS_SEED")) : (u32)time(NULL);
    port_endless = 1;
    port_endless_stats = 0;
    memset(&port_endless_carry, 0, sizeof(port_endless_carry));
    memset(&port_endless_run, 0, sizeof(port_endless_run));
    port_endless_unranked = 0;
    port_endless_unranked_why[0] = 0;
    port_endless_invincible = 0;
    next_room = -1;
    room = pick_room(-1);
}

/* WEAPONS IN EVERY MISSION: a catalogue of every mission's weapons, as each mission defines them when it starts
 * (the level's mod hands each slot over: port_weapon_put), kept in weapons.bin in the data folder (what the game
 * itself makes of its own code: the user's, never the repo's). A run's first start surveys the missions not in it
 * yet (each started a moment, its weapons taken, and left): a weapon Bond carries can then be lent to any mission. */
typedef struct {
    char name[28];
    u8 level, gun, animlen, pad;
    u8 def[0xB8], type[0x70], anim[16];
    char path[24];
    u8 sfx[8]; /* (its fire sound's entry in its mission's sounds' table: program, tone, note, voice, volume, randomness) */
    u8 kind, pad2[3]; /* (its entry in the pickups' table, D_8009D130: every mission's numbering; 0xFF none) */
    char got[28], ammo_got[32]; /* (that entry's messages: the weapon's, its ammo's) */
} WeaponRec;
#define WEAPON_RECS 128
static WeaponRec weapon_recs[WEAPON_RECS];
static int weapon_rec_count = -1;
static u16 surveyed; /* (levels whose weapons are in: bits) */
static int survey_queue[12], survey_n, survey_at, surveying, room_after_survey = -1;
static Room room_saved;

static void weapons_path(char *out, size_t len) {
    const char *xdg = getenv("XDG_DATA_HOME"), *home = getenv("HOME");
    if (xdg && *xdg)
        snprintf(out, len, "%s/twine/weapons.bin", xdg);
    else
        snprintf(out, len, "%s/.local/share/twine/weapons.bin", home ? home : ".");
}

static void weapons_load(void) {
    char path[512];
    FILE *f;
    int i;
    if (weapon_rec_count >= 0)
        return;
    weapon_rec_count = 0;
    surveyed = 0;
    weapons_path(path, sizeof(path));
    if ((f = fopen(path, "rb"))) {
        char magic[8];
        if (fread(magic, 1, 8, f) == 8 && !memcmp(magic, "TWWEAP03", 8))
            weapon_rec_count = (int)fread(weapon_recs, sizeof(WeaponRec), WEAPON_RECS, f);
        fclose(f);
    }
    for (i = 0; i < weapon_rec_count; i++)
        surveyed |= 1 << weapon_recs[i].level;
}

static void weapons_save(void) {
    char path[512], *c;
    FILE *f;
    weapons_path(path, sizeof(path));
    for (c = path + 1; *c; c++)
        if (*c == '/') {
            *c = 0;
            mkdir(path, 0755);
            *c = '/';
        }
    if ((f = fopen(path, "wb"))) {
        fwrite("TWWEAP03", 1, 8, f);
        fwrite(weapon_recs, sizeof(WeaponRec), weapon_rec_count, f);
        fclose(f);
    }
}

/* the level's mod: a mission's weapon, as it starts (gun: its fire handler the mission's pistol's) */
void port_weapon_put(int level, const u8 *def, const u8 *type, const u8 *anim, int animlen, const char *path, int gun,
                     const u8 *sfx, int kind, const char *got, const char *ammo_got) {
    int i;
    weapons_load();
    for (i = 0; i < weapon_rec_count; i++)
        if (weapon_recs[i].level == level && !strncmp(weapon_recs[i].name, (const char *)def, 27))
            break;
    if (i == weapon_rec_count) {
        if (i == WEAPON_RECS)
            return;
        weapon_rec_count++;
    }
    memset(&weapon_recs[i], 0, sizeof(WeaponRec));
    memcpy(weapon_recs[i].name, def, 27);
    weapon_recs[i].level = (u8)level, weapon_recs[i].gun = (u8)gun;
    memcpy(weapon_recs[i].def, def, 0xB8);
    if (type)
        memcpy(weapon_recs[i].type, type, 0x70);
    weapon_recs[i].animlen = (u8)(animlen > 16 ? 16 : animlen < 0 ? 0 : animlen);
    if (anim)
        memcpy(weapon_recs[i].anim, anim, weapon_recs[i].animlen);
    snprintf(weapon_recs[i].path, sizeof(weapon_recs[i].path), "%s", path ? path : "");
    if (sfx)
        memcpy(weapon_recs[i].sfx, sfx, 8);
    weapon_recs[i].kind = (u8)(kind < 0 ? 0xFF : kind);
    snprintf(weapon_recs[i].got, sizeof(weapon_recs[i].got), "%s", got ? got : "");
    snprintf(weapon_recs[i].ammo_got, sizeof(weapon_recs[i].ammo_got), "%s", ammo_got ? ammo_got : "");
    surveyed |= 1 << level;
}

/* the level's mod, when it has handed them all over */
void port_weapons_saved(void) {
    weapons_save();
}

/* the record of a weapon of that name, defined by another mission than `level` (the first such: a gun's first) */
const WeaponRec *port_weapon_find(const char *name, int level) {
    int i, b = -1;
    weapons_load();
    for (i = 0; i < weapon_rec_count; i++)
        if (weapon_recs[i].level != level && !strncmp(weapon_recs[i].name, name, 27) &&
            (b < 0 || (weapon_recs[i].gun && !weapon_recs[b].gun)))
            b = i;
    return b >= 0 ? &weapon_recs[b] : NULL;
}

/* the survey neither seen nor heard: a black screen with a line (video.c), silence (spu.c), from the first surveyed
 * mission's start (not before: the shell's briefing still waits for its button) until the run's first room's mission
 * starts. A survey that makes no progress for 10 s (600 frames shown) is given up: the run starts where it is (the
 * missions not surveyed are catalogued when they first start) */
static int covering;
static int survey_room; /* (the mission started is a survey's) */
static u32 survey_mark; /* (the frame of its last progress) */
u32 port_video_frames(void);
static void survey_abandon(void);
int port_video_covered(void) {
    if (covering && surveying && port_video_frames() - survey_mark > 600)
        survey_abandon();
    return covering;
}

int port_audio_muted(void) {
    return covering;
}

/* the level's mod: a survey's room (its weapons taken, then left at once) */
int port_endless_surveying(void) {
    return surveying;
}

/* the survey: before the run's first room, the missions of the room table whose weapons aren't known yet */
static int survey_begin(void) {
    int i;
    weapons_load();
    survey_n = survey_at = 0;
    if (getenv("TWINE_ENDLESS_NO_SURVEY") || (getenv("TWINE_ENDLESS_ROOM") && !getenv("TWINE_ENDLESS_SURVEY")))
        return 0;
    for (i = 0; i < ROOMS; i++)
        if (!(surveyed & (1 << rooms[i].level))) {
            int k;
            for (k = 0; k < survey_n && survey_queue[k] != rooms[i].level; k++)
                ;
            if (k == survey_n && survey_n < 12)
                survey_queue[survey_n++] = rooms[i].level;
        }
    if (!survey_n)
        return 0;
    room_after_survey = room;
    room_saved = rooms[ROOMS];
    rooms[ROOMS].level = survey_queue[0], rooms[ROOMS].point = -1;
    room = ROOMS;
    surveying = 1;
    survey_mark = port_video_frames();
    if (getenv("TWINE_ENDLESS_TEST_SURVEY_STALL")) /* (test: covered at once, as before a fix: the shell's briefing then
                                                    * waits unseen, the survey makes no progress: given up) */
        covering = 1;
    port_endless_logf(-1, "weapons: a survey of %d missions first (their weapons not known yet)", survey_n);
    return 1;
}

/* the level's mod: the survey's room done (its weapons taken): the next mission to survey, or the run's room */
void port_endless_survey_next(void) {
    survey_mark = port_video_frames();
    if (++survey_at < survey_n) {
        rooms[ROOMS].level = survey_queue[survey_at], rooms[ROOMS].point = -1;
        next_room = ROOMS;
    } else {
        rooms[ROOMS] = room_saved;
        next_room = room_after_survey;
        surveying = 0;
        port_endless_logf(-1, "weapons: survey done (%d weapons known)", weapon_rec_count);
    }
}

/* the survey given up (or the run before it, at its briefing): the run's own room next */
static void survey_abandon(void) {
    if (!surveying)
        return;
    port_endless_logf(-1, "weapons: survey given up at mission %d of %d (no progress for 10 s): the run starts", survey_at + 1,
                      survey_n);
    rooms[ROOMS] = room_saved;
    if (room == ROOMS)
        room = room_after_survey;
    if (next_room == ROOMS)
        next_room = room_after_survey;
    surveying = 0;
    covering = 0;
}

static void room_from_env(const char *at);

/* fixes/shell.c: ENDLESS chosen on mission select: a run begun, its first room's mission named in g_Args */
void port_endless_begin(u8 *args) {
    begin();
    room_from_env(getenv("TWINE_ENDLESS_ROOM"));
    survey_begin();
    *(s16 *)(args + 0x188) = rooms[room].level;
    *(s16 *)(args + 0x18A) = level_mission[rooms[room].level];
    port_endless_logf(-1, "the run starts in level %d point %d", rooms[room].level, rooms[room].point);
}

/* the level's mod: RESTART MISSION in a run: the whole run again, fresh (a new first room, the starting kit, its
 * heavy armour); the restarted mission's main returns at once for it */
void port_endless_restart(void) {
    int was = rooms[room].level;
    begin();
    next_room = room;
    room = -1;
    port_endless_logf(-1, "restart: a fresh run from level %d point %d (from level %d)", rooms[next_room].level,
                      rooms[next_room].point, was);
}

/* fixes/shell.c: the run given up at its briefing */
void port_endless_cancel(void) {
    if (surveying) {
        rooms[ROOMS] = room_saved;
        surveying = covering = 0;
    }
    port_endless = 0;
    room = -1;
}

/* main.c: the level a run starts in (0: no run) */
int port_endless_start(void) {
    const char *s = getenv("TWINE_ENDLESS"), *at = getenv("TWINE_ENDLESS_ROOM");
    if (!s || !atoi(s))
        return 0;
    begin();
    room_from_env(at);
    port_endless_logf(-1, "the run starts in level %d point %d", rooms[room].level, rooms[room].point);
    survey_begin();
    return rooms[room].level;
}

/* TWINE_ENDLESS_ROOM=level:point: the first room (headless tests; for a run begun on mission select too) */
static void room_from_env(const char *at) {
    int i;
    if (at) {
        int l = atoi(at), p = strchr(at, ':') ? atoi(strchr(at, ':') + 1) : -1;
        rooms[ROOMS].level = l;
        rooms[ROOMS].point = p;
        room = ROOMS;
        for (i = 0; i < ROOMS; i++)
            if (rooms[i].level == l && rooms[i].point == p)
                room = i;
    }
}

/* main.c, before a level's main runs: in a run, the room's point copied over the level's start point */
static int level_gen; /* (each mission started in a run: the level's mod knows a new one by it) */
int port_endless_gen(void) {
    return level_gen;
}

void port_endless_level(const PortModule *m) {
    s16 *start;
    level_gen++;
    covering = surveying && port_endless && m->level;
    survey_mark = port_video_frames();
    survey_room = surveying;
    if (m->level && getenv("TWINE_TEST_FLAGS2")) /* (headless tests: g_Args->flags2 bits on as each mission starts,
                                                   * as the front end's CHEATS screen leaves them: 0x8000 all weapons,
                                                   * 0x400 stealthy bond, 0x10000 invulnerable, 0x1000 power goons,
                                                   * 0x2000 power guns, 0x4000 infinite ammo) */
        *(u32 *)0x8001F168u |= (u32)strtoul(getenv("TWINE_TEST_FLAGS2"), NULL, 0);
    if (!port_endless && m->level && m->level < 12 && getenv("TWINE_TEST_START_POINT")) { /* (headless tests: the
                                                       * campaign's mission started at that load point, as a room's) */
        start = (s16 *)start_point[m->level];
        memcpy(start, start + 0xC + atoi(getenv("TWINE_TEST_START_POINT")) * 8, 10);
        port_log("test: level %d started at its load point %s\n", m->level, getenv("TWINE_TEST_START_POINT"));
    }
    if (!port_endless || !m->level || (room < 0 && next_room < 0))
        return;
    if (next_room >= 0) {
        room = next_room;
        next_room = -1;
    }
    room_starts = 0;
    if (rooms[room].level != m->level) { /* (a survey given up as the shell started its mission: a room of this one) */
        port_endless_logf(-1, "level %d started in a run, the room's is %d: a room of level %d instead", m->level,
                          rooms[room].level, m->level);
        rooms[ROOMS].level = m->level, rooms[ROOMS].point = -1;
        room = ROOMS;
    }
    start = (s16 *)start_point[m->level];
    if (rooms[room].point >= 0)
        memcpy(start, start + 0xC + rooms[room].point * 8, 10);
    memcpy(cur_entry, start, 10);
    segments = 0;
    port_endless_logf(-1, "room in level %d at point %d (section %d at %d %d %d)", m->level, rooms[room].point,
             start[0], start[2], start[3], start[4]);
}

/* An endless mode log line, for matching a session's events to what the player saw: "endless:", the wall clock
 * (to the millisecond) and the mission's frame, then the event */
u32 port_video_frames(void);
void port_endless_logf(s32 frame, const char *fmt, ...) {
    char text[512];
    struct timeval tv;
    struct tm tm;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(text, sizeof(text), fmt, ap);
    va_end(ap);
    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &tm);
    port_log("endless: %02d:%02d:%02d.%03d f%d v%u %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, (int)(tv.tv_usec / 1000),
             frame, port_video_frames(), text);
}

/* the level's mod: the run made unranked (why: for the log) */
void port_endless_unrank(const char *why) {
    if (port_endless && !port_endless_unranked) {
        port_endless_logf(-1, "the run is unranked from here (%s)", why);
        snprintf(port_endless_unranked_why, sizeof(port_endless_unranked_why), "%s", why);
    }
    port_endless_unranked |= port_endless;
}

/* the level's mod, as a room's mission starts: 1 the first time, more when it started over (RESTART MISSION) */
int port_endless_room_started(void) {
    return ++room_starts;
}

/* Each level's gate on its doors to other sections (LEVEL01's D_8009CE40: the story's say, a function of the
 * section asked for that may refuse it: "ACCESS DENIED", "the channels must be offline"...) */
static const u32 section_gate[12] = {0,          0x8009CE40, 0x800A1078, 0x8009EA18, 0x8009D8A4, 0x800A1470,
                                     0x8009EE20, 0x800A1DDC, 0x8009F294, 0x800A1ADC, 0x800A0FF0, 0x8009C4E4};

/* the level's mod, every frame of a run: the story's gate off, so that a room's doors open by the run's rules */
void port_endless_ungate(int level) {
    if (port_endless && level > 0 && level < 12 && *(u32 *)section_gate[level]) {
        port_endless_logf(-1, "story: the doors' gate (%08X) taken off", *(u32 *)section_gate[level]);
        *(u32 *)section_gate[level] = 0;
    }
}

/* the level's mod, every frame of a run: a mission's story clocks and messages that have no place in a fight off.
 * MI6's messages' alert, every mission's (the "MI6" sign: a new message waiting, set by the mission's message post,
 * L2_func_800426E8, LEVEL03's func_8004205C; the messages themselves stay in the watch's list): its flag cleared.
 * Turncoat's station (section 5): the bomb's two-minute timer (D_800D8AC4: > 0 running; the mission's own stop,
 * func_80045ADC, sets its top bit: no clock drawn, its end (func_8006AEE4: the blast that kills Bond) never called). */
static const u32 mi6_alert[12] = {0,          0,          0x800A1204, 0x8009EB98, 0x8009DA24, 0x800A15F0,
                                  0x8009EFA4, 0x800A1F5C, 0x8009F414, 0x800A1C64, 0x800A1170, 0x8009C664};
void port_endless_story_off(int level) {
    static int said, said_gen = -1;
    if (!port_endless || level < 1 || level > 11)
        return;
    if (said_gen != level_gen) /* (logged once a mission) */
        said = 0, said_gen = level_gen;
    if (level == 3 && *(s32 *)0x800D8AC4 > 0) {
        *(u32 *)0x800D8AC4 |= 0x80000000u;
        if (!(said & 1))
            port_endless_logf(-1, "story: the mission's timer stopped");
        said |= 1;
    }
    if (mi6_alert[level] && *(s32 *)mi6_alert[level] && !getenv("TWINE_ENDLESS_TEST_MI6_KEEP")) { /* (tests: kept) */
        *(s32 *)mi6_alert[level] = 0;
        if (!(said & 2))
            port_endless_logf(-1, "story: MI6's message alert off");
        said |= 2;
    }
}

/* the level's mod: points scored (kills, rooms cleared) */
void port_endless_score(int points) {
    port_endless_run.score += points;
}

/* the level's mod: the run's wave (1 in the first room) */
int port_endless_wave(void) {
    if (getenv("TWINE_ENDLESS_TEST_WAVE")) /* (tests: the run as if that many rooms were behind it) */
        return port_endless_run.rooms + atoi(getenv("TWINE_ENDLESS_TEST_WAVE"));
    return port_endless_run.rooms + 1;
}

/* the level's mod: the room's entry (section, yaw, x, y, z), or 0 outside a run */
int port_endless_room(s16 *entry) {
    if (!port_endless || room < 0)
        return 0;
    memcpy(entry, cur_entry, 10);
    return 1;
}

/* The level's mod, as Bond goes through a door to another of the mission's sections (its load point): 1 if that
 * section is the next room (a mission that flows, for a stretch of a few rooms: the mission loads it its own way),
 * else 0 (the next room is another mission's) */
int port_endless_segment(int point) {
    int l = rooms[room].level;
    s16 *lp;
    if (point < 0 || (l != 8 && l != 3) || segments >= (l == 3 ? 4 : 2))
        return 0;
    lp = (s16 *)start_point[l] + 0xC + point * 8;
    if (l == 8 && lp[0] == 3) /* (King's Ransom's end: the balloon) */
        return 0;
    if (l == 3 && (lp[0] < 1 || lp[0] > 5)) /* (Turncoat: only its subway, the cars (1-4) and the station (5)) */
        return 0;
    memcpy(cur_entry, lp, 10);
    segments++;
    room_starts = 0; /* (a new room, not the old one started over) */
    port_endless_run.rooms++;
    port_endless_logf(-1, "next room: the mission's own section %d (load point %d), stretch %d", lp[0], point, segments);
    return 1;
}

/* The level's mod, as Bond leaves the room: the next room chosen. (The mission's main then returns, and shows its
 * loading picture for the level g_Args names, from its own files: its own picture.) */
void port_endless_next(void) {
    if (next_room >= 0) /* (chosen already) */
        return;
    next_room = pick_room(room);
    port_endless_logf(-1, "next room: level %d point %d", rooms[next_room].level, rooms[next_room].point);
}

/* The high scores: endless.txt in the port's data folder, a line a run, the best first: score, name (the initials
 * the front end's hi-score table takes), rooms cleared, kills, accuracy in per cent, time in seconds, the date.
 * A run is left out when unranked. */
#define SCORES 10
typedef struct {
    int score;
    char name[8];
    char rest[96];
} Score;

static void scores_path(char *out, size_t len) {
    const char *xdg = getenv("XDG_DATA_HOME"), *home = getenv("HOME");
    if (xdg && *xdg)
        snprintf(out, len, "%s/twine/endless.txt", xdg);
    else
        snprintf(out, len, "%s/.local/share/twine/endless.txt", home ? home : ".");
}

static int scores_read(Score *s) {
    char path[512], line[160];
    int n = 0;
    FILE *f;
    scores_path(path, sizeof(path));
    if (!(f = fopen(path, "r")))
        return 0;
    while (n < SCORES && fgets(line, sizeof(line), f)) {
        int at = 0;
        if (line[0] == '#' || sscanf(line, "%d %7s %n", &s[n].score, s[n].name, &at) < 2)
            continue;
        snprintf(s[n].rest, sizeof(s[n].rest), "%s", line + at);
        s[n].rest[strcspn(s[n].rest, "\n")] = 0;
        n++;
    }
    fclose(f);
    return n;
}

/* The table on the memory card (fixes/shell.c reads and writes it as its own save beside the campaign's): once a
 * card's is read it is the table shown and kept to; endless.txt stays as the backup of every ranked run (and, on
 * a card without the save yet, its table is the card's first) */
static Score card_tab[SCORES];
static int card_n = -1; /* (-1: no card's table read) */

static int table_read(Score *s) {
    if (card_n < 0)
        return scores_read(s);
    memcpy(s, card_tab, sizeof(card_tab));
    return card_n;
}

/* The card's form: "TWEN", version 1, the count, then SCORES entries of 24 bytes (score, initials, rooms, kills,
 * accuracy, seconds, date as yyyymmdd), and a sum of all the bytes before it */
#define CARD_TABLE (16 + SCORES * 24 + 4)
static void put32(u8 *p, u32 v) {
    p[0] = (u8)v, p[1] = (u8)(v >> 8), p[2] = (u8)(v >> 16), p[3] = (u8)(v >> 24);
}
static u32 get32(const u8 *p) {
    return p[0] | p[1] << 8 | p[2] << 16 | (u32)p[3] << 24;
}

int port_endless_table_pack(u8 *out, int len) {
    Score s[SCORES];
    int n = table_read(s), i;
    u32 sum = 0;
    if (len < CARD_TABLE)
        return 0;
    memset(out, 0, CARD_TABLE);
    memcpy(out, "TWEN", 4);
    put32(out + 4, 1);
    put32(out + 8, (u32)n);
    for (i = 0; i < n; i++) {
        u8 *e = out + 16 + i * 24;
        int rooms = 0, kills = 0, acc = 0, secs = 0, y = 0, m = 0, d = 0;
        sscanf(s[i].rest, "%d %d %d %d %d-%d-%d", &rooms, &kills, &acc, &secs, &y, &m, &d);
        put32(e, (u32)s[i].score);
        if (strcmp(s[i].name, "-"))
            memcpy(e + 4, s[i].name, strlen(s[i].name) < 4 ? strlen(s[i].name) : 4);
        put32(e + 8, (u32)rooms);
        put32(e + 12, (u32)kills);
        put32(e + 16, (u32)(acc << 24 | (secs & 0xFFFFFF)));
        put32(e + 20, (u32)(y * 10000 + m * 100 + d));
    }
    for (i = 0; i < CARD_TABLE - 4; i++)
        sum += out[i];
    put32(out + CARD_TABLE - 4, sum);
    return CARD_TABLE;
}

/* a card's table (1), or 0 if it isn't one (corrupt: the table stays as it was) */
int port_endless_table_unpack(const u8 *in, int len) {
    u32 sum = 0, n;
    int i;
    if (len < CARD_TABLE || memcmp(in, "TWEN", 4) || get32(in + 4) != 1 || (n = get32(in + 8)) > SCORES)
        return 0;
    for (i = 0; i < CARD_TABLE - 4; i++)
        sum += in[i];
    if (sum != get32(in + CARD_TABLE - 4))
        return 0;
    for (i = 0; i < (int)n; i++) {
        const u8 *e = in + 16 + i * 24;
        u32 a = get32(e + 16), date = get32(e + 20);
        card_tab[i].score = (int)get32(e);
        memset(card_tab[i].name, 0, sizeof(card_tab[i].name));
        memcpy(card_tab[i].name, e + 4, 4);
        if (!card_tab[i].name[0])
            strcpy(card_tab[i].name, "-");
        snprintf(card_tab[i].rest, sizeof(card_tab[i].rest), "%u %u %u %u %04u-%02u-%02u", get32(e + 8),
                 get32(e + 12), a >> 24, a & 0xFFFFFF, date / 10000, date / 100 % 100, date % 100);
    }
    card_n = (int)n;
    port_endless_logf(-1, "scores: the card's table read (%d entries)", card_n);
    return 1;
}

/* a card without the table: endless.txt's becomes its first */
void port_endless_table_import(void) {
    card_n = scores_read(card_tab);
    port_endless_logf(-1, "scores: no table on the card: endless.txt's (%d entries) taken for it", card_n);
}

/* fixes/shell.c: the best three, for the front end's hi-score table (scores 0 past the last) */
void port_endless_best(char names[3][4], u32 scores[3]) {
    Score s[SCORES];
    int n = table_read(s), i;
    const char *fake = getenv("TWINE_TEST_BEST"); /* (media: NAME:score,NAME:score,NAME:score) */
    if (fake) {
        for (i = 0; i < 3; i++) {
            memset(names[i], 0, 4);
            scores[i] = 0;
            if (fake && sscanf(fake, "%3[^:]:%u", names[i], &scores[i]) == 2)
                fake = strchr(fake, ',') ? strchr(fake, ',') + 1 : NULL;
        }
        return;
    }
    for (i = 0; i < 3; i++) {
        memset(names[i], 0, 4);
        if (i < n && strcmp(s[i].name, "-"))
            memcpy(names[i], s[i].name, strlen(s[i].name) < 3 ? strlen(s[i].name) : 3);
        scores[i] = i < n ? (u32)s[i].score : 0;
    }
}

/* fixes/shell.c, after the run's stats: the run kept (name: the initials it was given, "" none), unless unranked */
void port_endless_keep(const char *name) {
    EndlessRun *r = &port_endless_run;
    Score s[SCORES + 1], now;
    char path[512];
    int n, i;
    FILE *f;
    time_t t = time(NULL);
    struct tm tm;
    if (port_endless_unranked)
        return;
    n = scores_read(s);
    localtime_r(&t, &tm);
    now.score = r->score;
    snprintf(now.name, sizeof(now.name), "%s", *name ? name : "-");
    snprintf(now.rest, sizeof(now.rest), "%d %d %d %d %04d-%02d-%02d", r->rooms, r->kills,
             r->shots ? r->hits * 100 / r->shots : 0, r->time / 60, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    for (i = n; i > 0 && s[i - 1].score < now.score; i--)
        s[i] = s[i - 1];
    s[i] = now;
    if (++n > SCORES)
        n = SCORES;
    if (card_n < 0) /* (no card's table yet: endless.txt's, the run with it, is the first) */
        memcpy(card_tab, s, sizeof(card_tab)), card_n = n;
    else {
        Score c[SCORES + 1];
        memcpy(c, card_tab, sizeof(card_tab));
        for (i = card_n; i > 0 && c[i - 1].score < now.score; i--)
            c[i] = c[i - 1];
        c[i] = now;
        if (++card_n > SCORES)
            card_n = SCORES;
        memcpy(card_tab, c, sizeof(card_tab));
    }
    scores_path(path, sizeof(path));
    {   /* the folder, as savestate.c makes its own */
        char *c;
        for (c = path + 1; *c; c++)
            if (*c == '/') {
                *c = 0;
                mkdir(path, 0755);
                *c = '/';
            }
    }
    if (!(f = fopen(path, "w")))
        return;
    fprintf(f, "# TWINE endless mode: score name rooms kills accuracy%% seconds date\n");
    for (i = 0; i < n; i++)
        fprintf(f, "%d %s %s\n", s[i].score, s[i].name, s[i].rest);
    fclose(f);
}

/* The run is over: its totals in g_Args where the mission's own go, for the front end's mission stats, which
 * fixes/shell.c shows as the run's (as a mission completed, the campaign's progress left as it was) */

static void run_over(u8 *args) {
    EndlessRun *r = &port_endless_run;
    /* the points the rooms gave, and for the run as a whole: head shots and accuracy */
    r->score += r->headshots * 250 + (r->shots ? r->hits * 5000 / r->shots : 0);
    if (getenv("TWINE_TEST_RUN")) /* (media: rooms,kills,headshots,shots,hits,seconds,score of a run, as given) */
        sscanf(getenv("TWINE_TEST_RUN"), "%d,%d,%d,%d,%d,%d,%d", &r->rooms, &r->kills, &r->headshots, &r->shots,
               &r->hits, &r->time, &r->score);
    *(u16 *)(args + 0x2C4) = r->kills > 0x7FFF ? 0x7FFF : r->kills;
    *(u16 *)(args + 0x2C6) = r->headshots > 0x7FFF ? 0x7FFF : r->headshots;
    *(s32 *)(args + 0x2C8) = r->shots;
    *(s32 *)(args + 0x2CC) = r->hits;
    *(s32 *)(args + 0x2D8) = r->time;
    *(u32 *)args &= ~0x40u; /* on to the front end */
    args[0x114] = 0;        /* (no mission completed: the boot stub plays no film) */
    port_endless_logf(-1, "the run is over: %d rooms, %d kills, %d of %d shots hit, score %d%s", r->rooms, r->kills,
             r->hits, r->shots, r->score, port_endless_unranked ? " (unranked)" : "");
    port_endless = 0;
    port_endless_stats = 1;
    room = -1;
}

/* main.c, as a level's main returns: in a run, the next room's mission named in g_Args (level, mission, its
 * executable) and the front end skipped (flags 0x40), for the boot stub or main.c to start */
void port_endless_returned(const PortModule *m, u8 *args) {
    int l;
    if (!port_endless || !m->level)
        return;
    if (room < 0 && next_room >= 0) { /* (RESTART MISSION: a fresh run, the first room next; nothing counted) */
        l = rooms[next_room].level;
        *(u32 *)args |= 0x40;
        *(s16 *)(args + 0x188) = l;
        *(s16 *)(args + 0x18A) = level_mission[l];
        sprintf((char *)(args + 0x234), "\\L%02d\\LEVEL%02d.EXE;1", l, l);
        return;
    }
    if (survey_room) { /* (a survey's mission: nothing of it counted) */
        l = rooms[next_room].level;
        *(u32 *)args |= 0x40;
        *(s16 *)(args + 0x188) = l;
        *(s16 *)(args + 0x18A) = level_mission[l];
        sprintf((char *)(args + 0x234), "\\L%02d\\LEVEL%02d.EXE;1", l, l);
        return;
    }
    /* the mission's counts (cleared as each starts: kills, head shots, shots, hits; its time) */
    port_endless_run.kills += *(u16 *)(args + 0x2C4);
    port_endless_run.headshots += *(u16 *)(args + 0x2C6);
    port_endless_run.shots += *(s32 *)(args + 0x2C8);
    port_endless_run.hits += *(s32 *)(args + 0x2CC);
    port_endless_run.time += *(s32 *)(args + 0x2D8);
    if (next_room < 0) { /* Bond died: the run is over */
        run_over(args);
        return;
    }
    port_endless_run.rooms++;
    l = rooms[next_room].level;
    *(u32 *)args |= 0x40;
    *(s16 *)(args + 0x188) = l;
    *(s16 *)(args + 0x18A) = level_mission[l];
    sprintf((char *)(args + 0x234), "\\L%02d\\LEVEL%02d.EXE;1", l, l);
}

/* main.c, after a level's main returned with no boot stub to start the next: the next room's level (0: none) */
int port_endless_after(void) {
    return port_endless && next_room >= 0 ? rooms[next_room].level : 0;
}
