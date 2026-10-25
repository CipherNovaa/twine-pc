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
} Room;

/* The rooms: sections of the missions that make a fight with a way out (a door to another section that opens
 * without the story: no keycard, objective, lift or wait), picked by playing them. A section may be entered from
 * either end (King's Ransom's 4, played backwards, is one of the best). Left out: Russian Roulette (the casino) and
 * Night Watch (its guards may not be killed: back with the borrowed guards of stage 2), the sections that start in a
 * cutscene or where Bond dies of the place alone, the ski slope, rooms whose doors the story keeps shut, the lifts.
 * King's Ransom flows: from one of its rooms the next of its sections follows, for a stretch (port_endless_segment). */
static Room rooms[] = {
    /* Courier: the bank's floor and the later part (its lift arrivals left out) */
    {2, -1}, {2, 4}, {2, 5}, {2, 6},
    /* King's Ransom: all of it but its end (section 3: the balloon, scripted, no way on), the bar's two doors (5)
     * and the section played backwards (4) among the best */
    {8, -1}, {8, 0}, {8, 2}, {8, 3}, {8, 4}, {8, 5}, {8, 6},
    /* Masquerade */
    {7, 2},
    /* Flashpoint: the night-vision pipeline (13) */
    {6, 0}, {6, 9}, {6, 13},
    /* City of Walkways: the warehouse (4) */
    {5, -1}, {5, 4},
    /* Turncoat: the streets */
    {3, -1}, {3, 11}, {3, 13},
    /* Fallen Angel: the MS keycard area and the castle's halls */
    {9, 0}, {9, 2}, {9, 4},
    /* Meltdown */
    {10, -1}, {10, 1}, {10, 2}, {10, 4}, {10, 6},
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
    port_endless_unranked_why[0] = 0;
    next_room = -1;
    room = pick_room(-1);
}

/* fixes/shell.c: ENDLESS chosen on mission select: a run begun, its first room's mission named in g_Args */
void port_endless_begin(u8 *args) {
    begin();
    *(s16 *)(args + 0x188) = rooms[room].level;
    *(s16 *)(args + 0x18A) = level_mission[rooms[room].level];
    port_endless_logf(-1, "the run starts in level %d point %d", rooms[room].level, rooms[room].point);
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
    port_endless_logf(-1, "the run starts in level %d point %d", rooms[room].level, rooms[room].point);
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
        port_endless_logf(-1, "level %d started in a run, the room's is %d", m->level, rooms[room].level);
        return;
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
    port_log("endless: %02d:%02d:%02d.%03d f%d %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, (int)(tv.tv_usec / 1000),
             frame, text);
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
    memcpy(entry, cur_entry, 10);
    return 1;
}

/* The level's mod, as Bond goes through a door to another of the mission's sections (its load point): 1 if that
 * section is the next room (a mission that flows, for a stretch of a few rooms: the mission loads it its own way),
 * else 0 (the next room is another mission's) */
int port_endless_segment(int point) {
    int l = rooms[room].level;
    s16 *lp;
    if (point < 0 || l != 8 || segments >= 2)
        return 0;
    lp = (s16 *)start_point[l] + 0xC + point * 8;
    if (lp[0] == 3) /* (King's Ransom's end: the balloon) */
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
