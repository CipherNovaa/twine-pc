/* Endless mode: one run of rooms until Bond dies. A room is a section of one of the missions, entered where the
 * mission itself enters it (its start point or one of its load points) and left through any of the section's
 * doors to another section (port/mods/endless.c, in the level). Then the next room's mission is started in the
 * place of the one that ran, Bond's state carried across.
 *
 * Each level keeps where it starts in its data: its start point (section, yaw, x, y, z: the record the level's
 * loader reads when it starts, func_80032074 in LEVEL01) and after it its load points, 0x10 bytes each (the same
 * five, then the point's id, which the doors name, and its music). A room's mission starts at the room's point
 * when that point is copied over the start point before the mission's main runs.
 *
 * TWINE_ENDLESS=1 starts a run (with TWINE_LEVEL unset or naming the first room's level, for tests);
 * TWINE_ENDLESS_ROOM=level:point starts it in that room (point -1: the start point). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "port.h"
#include "module.h"
#include "endless.h"

/* each level's start point (its load points follow it, from +0x18) */
static const u32 start_point[12] = {0,          0x8009E9C4, 0x800A3E74, 0x800A0ED4, 0x8009FF14, 0x800A382C,
                                    0x800A1584, 0x800A4C3C, 0x800A198C, 0x800A4004, 0x800A369C, 0x8009DAE8};

typedef struct {
    s8 level, point; /* point: the load point (-1 the start point) */
} Room;

/* The rooms: each mission's sections, entered at its start point and at each of its load points, but those that
 * start in a cutscene or a loading the run would wait through, those where Bond dies of the place alone (on the
 * train's roof, by City of Walkways' water), Cold Reception's ski slope and a room with nearly no floor to send
 * guards to. Russian Roulette (the casino) has none. (Surveyed headless, 2026-10-24.) */
static Room rooms[] = {
    /* Courier */
    {2, -1}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7},
    /* King's Ransom */
    {8, -1}, {8, 0}, {8, 2}, {8, 3}, {8, 4}, {8, 5}, {8, 6}, {8, 7},
    /* Cold Reception */
    {1, -1}, {1, 0}, {1, 2},
    /* Night Watch */
    {4, -1}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, {4, 8}, {4, 10}, {4, 11}, {4, 12}, {4, 13}, {4, 14}, {4, 15},
    /* Masquerade */
    {7, -1}, {7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 6}, {7, 7},
    /* Flashpoint */
    {6, -1}, {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6}, {6, 8}, {6, 9}, {6, 10}, {6, 11}, {6, 13},
    /* City of Walkways */
    {5, -1}, {5, 0}, {5, 2}, {5, 3}, {5, 4},
    /* Turncoat */
    {3, -1}, {3, 0}, {3, 4}, {3, 6}, {3, 8}, {3, 10}, {3, 11}, {3, 12}, {3, 13}, {3, 14},
    /* Fallen Angel */
    {9, -1}, {9, 0}, {9, 2}, {9, 3}, {9, 4}, {9, 5},
    /* Meltdown */
    {10, -1}, {10, 0}, {10, 1}, {10, 2}, {10, 4}, {10, 5}, {10, 6}, {10, 7},
    {0, 0}, /* (TWINE_ENDLESS_ROOM's, when not one of these) */
};
#define ROOMS ((int)(sizeof(rooms) / sizeof(rooms[0])) - 1)

/* the mission each level folder holds (main.c's) */
static const s16 level_mission[12] = {-1, 2, 0, 8, 4, 7, 6, 5, 1, 9, 10, 3};

int port_endless; /* a run is on */
EndlessCarry port_endless_carry;
EndlessRun port_endless_run;
int port_endless_unranked PORT_NOSAVE; /* (a state loaded must not take it back) */
int port_endless_stats; /* the front end shows the run's stats next */
static int room = -1, next_room = -1, room_starts;
static void run_over(u8 *args);
static u32 seed;

static u32 next_rand(void) {
    seed = seed * 1103515245 + 12345;
    return seed >> 8;
}

/* a room of another level than the one before: the level first (each as likely), then one of its rooms */
static int pick_room(int after) {
    int levels[12], n = 0, i, l, k = 0, r;
    for (l = 1; l <= 11; l++)
        for (i = 0; i < ROOMS; i++)
            if (rooms[i].level == l) {
                if (after < 0 || rooms[after].level != l)
                    levels[n++] = l;
                break;
            }
    l = levels[next_rand() % n];
    for (i = 0; i < ROOMS; i++)
        k += rooms[i].level == l;
    r = next_rand() % k;
    for (i = 0; i < ROOMS; i++)
        if (rooms[i].level == l && !r--)
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
    next_room = -1;
    room = pick_room(-1);
}

/* fixes/shell.c: ENDLESS chosen on mission select: a run begun, its first room's mission named in g_Args */
void port_endless_begin(u8 *args) {
    begin();
    *(s16 *)(args + 0x188) = rooms[room].level;
    *(s16 *)(args + 0x18A) = level_mission[rooms[room].level];
    port_log("endless: the run starts in level %d point %d\n", rooms[room].level, rooms[room].point);
}

/* fixes/shell.c: the run given up at its briefing */
void port_endless_cancel(void) {
    port_endless = 0;
    room = -1;
}

/* main.c: the level a run starts in (0: no run) */
int port_endless_start(void) {
    const char *s = getenv("TWINE_ENDLESS"), *at = getenv("TWINE_ENDLESS_ROOM");
    int i;
    if (!s || !atoi(s))
        return 0;
    begin();
    if (at) {
        int l = atoi(at), p = strchr(at, ':') ? atoi(strchr(at, ':') + 1) : -1;
        rooms[ROOMS].level = l;
        rooms[ROOMS].point = p;
        room = ROOMS;
        for (i = 0; i < ROOMS; i++)
            if (rooms[i].level == l && rooms[i].point == p)
                room = i;
    }
    port_log("endless: the run starts in level %d point %d\n", rooms[room].level, rooms[room].point);
    return rooms[room].level;
}

/* main.c, before a level's main runs: in a run, the room's point copied over the level's start point */
void port_endless_level(const PortModule *m) {
    s16 *start;
    if (!port_endless || !m->level || room < 0)
        return;
    if (next_room >= 0) {
        room = next_room;
        next_room = -1;
    }
    room_starts = 0;
    if (rooms[room].level != m->level) {
        port_log("endless: level %d started in a run, the room's is %d\n", m->level, rooms[room].level);
        return;
    }
    start = (s16 *)start_point[m->level];
    if (rooms[room].point >= 0)
        memcpy(start, start + 0xC + rooms[room].point * 8, 10);
    port_log("endless: room in level %d at point %d (section %d at %d %d %d)\n", m->level, rooms[room].point,
             start[0], start[2], start[3], start[4]);
}

/* the level's mod: the run made unranked (why: for the log) */
void port_endless_unrank(const char *why) {
    if (port_endless && !port_endless_unranked)
        port_log("endless: the run is unranked from here (%s)\n", why);
    port_endless_unranked |= port_endless;
}

/* the level's mod, as a room's mission starts: 1 the first time, more when it started over (RESTART MISSION) */
int port_endless_room_started(void) {
    return ++room_starts;
}

/* the level's mod: points scored (kills, rooms cleared) */
void port_endless_score(int points) {
    port_endless_run.score += points;
}

/* the level's mod: the run's wave (1 in the first room) */
int port_endless_wave(void) {
    return port_endless_run.rooms + 1;
}

/* the level's mod: the room's entry (section, yaw, x, y, z), or 0 outside a run */
int port_endless_room(s16 *entry) {
    if (!port_endless || room < 0)
        return 0;
    memcpy(entry, (s16 *)start_point[rooms[room].level], 10);
    return 1;
}

/* The level's mod, as Bond leaves the room: the next room chosen. (The mission's main then returns, and shows its
 * loading picture for the level g_Args names, from its own files: its own picture.) */
void port_endless_next(void) {
    next_room = pick_room(room);
    port_log("endless: next room: level %d point %d\n", rooms[next_room].level, rooms[next_room].point);
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

/* fixes/shell.c: the best three, for the front end's hi-score table (scores 0 past the last) */
void port_endless_best(char names[3][4], u32 scores[3]) {
    Score s[SCORES];
    int n = scores_read(s), i;
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
    *(u16 *)(args + 0x2C4) = r->kills > 0x7FFF ? 0x7FFF : r->kills;
    *(u16 *)(args + 0x2C6) = r->headshots > 0x7FFF ? 0x7FFF : r->headshots;
    *(s32 *)(args + 0x2C8) = r->shots;
    *(s32 *)(args + 0x2CC) = r->hits;
    *(s32 *)(args + 0x2D8) = r->time;
    *(u32 *)args &= ~0x40u; /* on to the front end */
    args[0x114] = 0;        /* (no mission completed: the boot stub plays no film) */
    port_log("endless: the run is over: %d rooms, %d kills, %d of %d shots hit, score %d%s\n", r->rooms, r->kills,
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
