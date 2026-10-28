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
 * MOD_REPLACES_NATIVE: func_8004900C func_80057750 func_80076014 func_80076104 func_8002D4E4 func_8002E1B0 RestartPrompt_Open func_8004CA5C func_8004B270 func_80054414 func_8002E318 func_80048B4C func_80055E24 func_8005E020 func_8003C1BC func_80041DBC func_80053ED8 func_80053114 func_800572F4 func_80026F30 func_80027248 func_80081458 func_8004DD9C func_8007A17C func_800753F0 func_800449BC func_80044A60 func_80042E6C func_80043078 func_800291B0 func_80033F38 func_800350F0 SsUtAllKeyOff func_80026C90 func_80053804 func_8005519C func_80034A48 func_80026D08 func_800272A4 func_80036938
 *
 * Names are LEVEL01's; port/mods.py maps them onto each level. */
#include "twine.h"
#include "../src/endless.h"

char *getenv(const char *name);
int snprintf(char *s, unsigned n, const char *fmt, ...);
int atoi(const char *s);
void *memcpy(void *d, const void *s, unsigned n);
void *memset(void *d, int c, unsigned n);
void *memmove(void *d, const void *s, unsigned n);
char *strncpy(char *d, const char *s, unsigned n);
int strncmp(const char *a, const char *b, unsigned n);
int strcmp(const char *a, const char *b);
unsigned strlen(const char *s);
char *strchr(const char *s, int c);
char *strstr(const char *s, const char *t);
void port_endless_logf(s32 frame, const char *fmt, ...);
int port_trace(const char *what);
void endless_muted_point(u32 id);
int port_sound_bank(int level, const u8 **vh, const u8 **vb);
typedef struct { u8 prog, tone, note, voice, vol, rnd, pad[2]; } SfxDefE; /* (the mission's sounds' table) */
int port_tone_play(int level, int program, int tone, int note, s32 l, s32 r, int loop);
s32 func_80026B44(s32 rnd); /* (the game's sounds' random note step) */
s32 func_80026ED0(void);   /* (its voices in use) */
int port_tone_play_pcm(int level, int program, int tone, int note, s32 vol, int loop);
s32 func_80026F30_orig(u32 id, s32 flags);
extern u32 port_rc_ra, port_rc_psp; /* (recomp_rt.c: the PS1 return address, stack pointer of a call from
                                       * recompiled code) */
void port_log(const char *fmt, ...);
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
extern u32 g_PadHeld;
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
s32 func_8002E1B0(void); /* (the mission completed: the run's say) */
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
void port_endless_story_off(int level);
void port_endless_restart(void);
void quick_grenade_give(void);
typedef struct {
    char name[28];
    u8 level, gun, animlen, pad;
    u8 def[0xB8], type[0x70], anim[16];
    char path[24];
    u8 sfx[8]; /* (its fire sound's entry in its mission's sounds' table: program, tone, note, voice, volume, randomness) */
    u8 kind, pad2[3]; /* (its entry in the pickups' table, D_8009D130: every mission's numbering; 0xFF none) */
    char got[28], ammo_got[32]; /* (that entry's messages: the weapon's, its ammo's) */
} WeaponRec; /* (port/src/endless.c's catalogue of every mission's weapons) */
void port_weapon_put(int level, const u8 *def, const u8 *type, const u8 *anim, int animlen, const char *path, int gun,
                     const u8 *sfx, int kind, const char *got, const char *ammo_got);
void port_weapons_saved(void);
const WeaponRec *port_weapon_find(const char *name, int level);
int port_endless_surveying(void);
void port_endless_survey_next(void);
extern char *D_8009DB50[];  /* the first-person models' files, by model type (a weapon's +0x5B) */
extern u8 *D_8009D0FC;      /* the model types' entries (0x70 bytes: +0x68 animation numbers, +0x6C their count) */
extern u8 D_800E2F58;       /* the model type in the load buffer (D_800E2F5C, its file, 0xE000 bytes) */
extern s32 D_800F0F5C;      /* a model is loading */
void func_80076014_orig(s32 idx);
void func_80076104_orig(s32 idx);
u32 port_disc_file(const char *name, u8 **data);
u32 port_disc_part(const char *name, u32 offset, u32 len, u8 **data);
void free(void *p);
void *malloc(unsigned n);
int quick_grenade_enemy(const s32 *from, const s32 *to, u8 *owner); /* (quickgrenade.c) */
void quick_grenade_test_blast(s32 *pos, u8 *owner); /* (quickgrenade.c, tests) */
int port_live_grenades(s32 (*at)[3], int max);
s32 func_80027248(s32 sound, s32 at, s32 arg2); /* a sound of the level's bank at a place */
s32 func_80051728(u8 *obj, u8 *target) __attribute__((weak)); /* what a person makes of another: 0x40 he sees him */
s32 func_800746D0(s32 mesh, s32 *from, s32 *to, s16 *normal, s16 *hit, s32 *flags); /* the walls between: nonzero */

/* PS1 code called from the mod's own with pointers (the walls between two points, the floor's height, Bond put at a
 * point): the game's code keeps its pointer arguments in the scratchpad (func_800746D0 at +0x40..), where the game's
 * own code later reads bytes of them it never sets itself (an unaligned store over one, rc_8006FB5C). Pointers to the
 * mod's locals were the host's stack addresses, which the host's address randomization moves run to run: two runs of
 * the same build and input parted (a Fallen Angel blast's debris in k_gl_lent at f1088 in one of three places). Now
 * the arguments are copied to the mod's own (fixed) memory and, for the queries, which change nothing of the game's,
 * the scratchpad is put back as it was after (TWINE_TEST_RAMHASH shows it the same run to run). */
static u8 scratch_kept[0x400];
static s32 walls_between(s32 mesh, const s32 *from, const s32 *to, s16 *normal, s16 *hit, s32 *flags) {
    static s32 f[4], t[4], fl;
    static s16 nn[4], hh[4];
    s32 r;
    memcpy(f, from, 12), memcpy(t, to, 12), f[3] = t[3] = 0, fl = flags ? *flags : 0;
    memcpy(scratch_kept, (void *)0x1F800000u, 0x400);
    r = func_800746D0(mesh, f, t, nn, hh, flags ? &fl : 0);
    memcpy((void *)0x1F800000u, scratch_kept, 0x400);
    if (normal)
        memcpy(normal, nn, 8);
    if (hit)
        memcpy(hit, hh, 8);
    if (flags)
        *flags = fl;
    return r;
}
static s32 floor_query(s32 floor, const s16 *pos, s32 a2, s32 a3) {
    static s16 p[4];
    s32 r;
    memcpy(p, pos, 8);
    memcpy(scratch_kept, (void *)0x1F800000u, 0x400);
    r = func_800705A8(floor, p, a2, a3);
    memcpy((void *)0x1F800000u, scratch_kept, 0x400);
    return r;
}
static void bond_put(const s16 *at) {
    static s16 a[6];
    memcpy(a, at, 10);
    func_80030FA8(a);
}
extern s32 D_800E2F2C;  /* (the section's walls, for it) */
u32 port_ext_free_bytes(void); /* (malloc3.c) */
void func_80053DE4(s16 type, s16 sub, s32 x, s32 y, s32 z); /* an event on the AI's board (0: a gunshot) */

extern u8 *D_8009CD98;     /* the weapon in Bond's hands (an object) */
extern s32 D_8009CD4C;
#define OBJ(i) ((u8 *)D_800AFFC8 + (i) * 0x124)
static s32 hp_seen[64]; /* (headless tests: each person's health as the frame began, for the damage a hit did) */
#define TYPE_DEF(t) ((u8 *)g_Args->unk228 + (t) * 0x70) /* the object types: +1 their class (6 people), +2 (0x22 a door's zone) */
#define HEALTH(o) (*(s32 *)((u8 *)(o) + 0x34))
#define POS(o) ((s32 *)(*(u8 **)((u8 *)(o) + 0x90) + 0x28))
#define SEC 20                                   /* frames a second (the game's 20 Hz) */
/* each mission's sounds' table: how many (func_80026F30 plays those below; a lent weapon's own sound is given an id
 * past them, below 0x80, which the guns' code takes as no sound) */
static const u8 sound_count[12] = {0x69, 0x69, 0x7C, 0x77, 0x69, 0x62, 0x76, 0x79, 0x73, 0x71, 0x7D, 0x69};
#define LENT_SOUND (sound_count[g_Args->level < 12 ? g_Args->level : 0]) /* (the first lent weapon's sound id) */

static int weapon_slots(void) { /* g_Weapons' slots, the mission's own set: the count its weapon buttons wrap at (each
                                 * level's psyq_libs.s, the stepping loop's `slti v0,v1,N`: Night Watch's 9, its
                                 * tools in 6-8 included; it was 6 here, so a run never saw them: no lent weapon got
                                 * a slot there, the credit card lock and the camera stayed his) */
    static const s8 n[12] = {0, 6, 12, 9, 9, 11, 12, 13, 11, 12, 10, 4};
    return g_Args && g_Args->level >= 1 && g_Args->level <= 11 ? n[g_Args->level] : 6;
}
#define SLOTS weapon_slots()

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
static u8 door_pt[DOORS], door_type[DOORS]; /* (each zone's load point, its type) */
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
typedef struct {
    s16 type, pad;
    s32 x, y, z, w;
    u32 time;
} AiEvent; /* (the AI board's events: D_800DA270's 27, 0x18 bytes each) */
static AiEvent hunt_ev[64];
static s32 last_seen_at = -1, bond_still_since, nav_goal[8][3], drive_rest[64], spread_until[64], shots_held, shots_fired;
static u8 driving[64], seen_for[64], react[64];
static u8 hunt_on[64]; /* (THE RAMP: allowed to hunt now) */
static s32 shot_last[64], shot_burst[64], shot_rest[64]; /* (THE RAMP: his last shot, his burst's start, resting till) */
static int ramp_off = -1;
static u8 *cover_state_ptr(void);
static void nav_reset(void);
static void grenade_reset(void);
static void callouts_reset(void);
static void callout_hit(void);
static void blast_frame(void);
static int blast_active(void);

/* the room's own fixed guards (snipers, grenade throwers, posts: those of the game's standing AIs, 2-5) that fell,
 * to come back to their places half a minute later */
#define RETURNS 8
static struct {
    s32 rec[6], at[3], due;
} returns[RETURNS];

/* how each people's slot's person was made (the log's: placed, sent, a fallen one's return, remade after a slot freed) */
static const char *made_how[64];
static s32 made_at[64]; /* (when: the AI lets his script hear and wake a few seconds first) */
static int making;               /* (what spawn()/the returns are making now: 0 the section) */
static s32 *freed_recs[16];      /* (records (their flags words) whose person free_slot took out) */
static int freed_next;
static s32 floor_at(s32 x, s32 y, s32 z);
static u16 fired_by[64]; /* (each person's shots, for the people lines: people_log) */
static void people_log(void) {
    int i;
    for (i = D_800B0DA0; i <= D_800B07B0; i++) {
        u8 *o = OBJ(i), *st = *(u8 **)(o + 0x94);
        s32 *src = *(s32 **)(o + 0x60);
        if (o == g_Player || !o[4])
            continue;
        ELOG("people: %d type %d %s, health %d, ai %d at %d %d %d (floor %d, yaw %d); record %08X flags %08X w4 %08X; "
             "state +24 %08X anim %d; obj +8 %d +A4 %02X +A6 %02X +48 %08X +4C %08X +E %04X ai +4 %08X +54 %d +A4 %08X shots %d", i, o[6], made_how[i & 63] ? made_how[i & 63] : "?",
             HEALTH(o), o[0xA7], POS(o)[0], POS(o)[1], POS(o)[2], floor_at(POS(o)[0], POS(o)[1] - 500, POS(o)[2]),
             *(s16 *)(*(u8 **)(o + 0x90) + 0x1A), (u32)src, src ? src[0] : 0, src ? src[1] : 0,
             st ? *(s32 *)(st + 0x24) : -1, st ? *(s16 *)(st + 0x10) : -1, o[8], o[0xA4], o[0xA6], *(s32 *)(o + 0x48), *(s32 *)(o + 0x4C), *(u16 *)(o + 0xE), *(u8 **)(o + 0x118) ? *(u32 *)(*(u8 **)(o + 0x118) + 4) : 0,
             *(u8 **)(o + 0x118) ? *(s32 *)(*(u8 **)(o + 0x118) + 0x54) : -1,
             *(u8 **)(o + 0x118) ? *(u32 *)(*(u8 **)(o + 0x118) + 0xA4) : 0, fired_by[i & 63]);
    }
}

static s16 entry[5];
static s32 was_alive[64]; /* each person's health last frame (a guard falling leaves pickups) */
static s32 was_at[3], said; /* where Bond was as the last frame began; when a message was last given */
/* LOCKED DOORS: a door the mission keeps shut (its own message: "DOOR IS LOCKED", "ACCESS DENIED", a key or card
 * wanted, ...) is a way on like any other once the room's exit is open: Bond using it leaves (no automatic exits:
 * only by his use); before that, the mod's "EXIT LOCKED" in place of the mission's message */
static s32 door_msg_at = -1, locked_exit_at = -1; /* (a door's refusal heard; Bond asking to leave by one) */
static int far_exit(void);
static s32 fanfare_at = -1, fanfare_clip; /* (City of Walkways' helicopter room's end: func_8002E1B0) */
static int fanfare_done(void);
static char door_msg_text[32];
extern u16 port_pad_buttons[2];
extern u8 port_pad_sticks[2][4]; /* (RX RY LX LY) */
static int dt_on = -1, dt_n, dt_list[DOORS], dt_floor[DOORS], dt_done; /* (TWINE_ENDLESS_TEST_DOORS) */
static const char *dt_result;
extern s8 D_8009CABF; /* (the mission's loop leaves the controls unread: its opening iris) */
static int wave_due; /* (the room's "WAVE N" for when the opening iris is over) */
static s32 last_sent; /* (when the director last got a guard in: THE POSTED GUARDS) */
static s32 last_frame = -1, room_frames, carried, kills_at_start, kills_scored, budget, cooldown, open;
static s32 burst, segment, last_section = -1, loading, at_entry, survey_left, seen_gen = -1;
int port_endless_gen(void);
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

/* the people's slots (port/mods/people.c: 12 with 8 MB of RAM, else the game's 6): the director's cap grows with them */
int people_slots(void) __attribute__((weak));
void people_guard_types(const u8 *types, int n, const u8 *models, int m) __attribute__((weak));
int people_game_slot_as(int s, int src, int kind) __attribute__((weak));
u8 *people_record(int i) __attribute__((weak));
u8 *people_busy(int i) __attribute__((weak));
static int people_cap(void) {
    return people_slots ? people_slots() : 6;
}
static int fill_test(void) { /* TWINE_ENDLESS_TEST_FILL=1 (tests): the director sends guards until every slot is taken */
    static int on = -1;
    if (on < 0)
        on = getenv("TWINE_ENDLESS_TEST_FILL") != NULL;
    return on;
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
static int draw_hidden;     /* (the gun hidden while the carried one loads) */
static int draw_direct;     /* (this frame: a room's start in a mission loaded afresh, draw_slot taken out at once) */
static const char *detonator_charges(int slot);
static void carry_ticks(void);
static u32 user_out; /* (the slots out of the weapon buttons' round by Bond's checkmark: the inventory's, carried in) */
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
        c->weapon[j].owned = (w->flags & 0x800) != 0 /* (owned: bit 12 only says in the weapon buttons' round) */;
        if (c->weapon[j].owned && strncmp(c->weapon[j].name, "SATCHEL DET", 11)) /* (its checkmark, for the run) */
            c->weapon[j].unticked = !(w->flags >> 12 & 1);
        c->weapon[j].ammo = w->ammo;
        c->weapon[j].clip = w->clip;
        c->weapon[j].reserve = w->reserve;
    }
    carry_ticks(); /* (the lent ones waiting out of a slot: their checkmarks) */
    {   /* the weapon in his hand now (or the one he is changing to: ours, or the game's) */
        int h = draw_slot >= 0 ? draw_slot : D_800B0DDC >= 0 && D_800B0DDC < SLOTS ? D_800B0DDC : D_800B0DD4;
        if (h < 0 || h >= SLOTS || g_Weapons[h].pad0[0] < 'A' || g_Weapons[h].pad0[0] > 'Z')
            h = D_800B0DD4;
        if (h >= 0 && h < SLOTS && detonator_charges(h)) {
            strncpy(c->in_hand, detonator_charges(h), sizeof(c->in_hand) - 1); /* (the lent detonator: its charges) */
            c->in_hand[sizeof(c->in_hand) - 1] = 0;
        } else if (h >= 0 && h < SLOTS && g_Weapons[h].pad0[0] >= 'A' && g_Weapons[h].pad0[0] <= 'Z') {
            strncpy(c->in_hand, (char *)g_Weapons[h].pad0, sizeof(c->in_hand) - 1);
            c->in_hand[sizeof(c->in_hand) - 1] = 0;
        }
        ELOG("carry: Bond leaves with %s in hand (slot %d; in hand %d, changing to %d)", c->in_hand, h, D_800B0DD4,
             D_800B0DDC);
    }
}

/* WEAPONS IN EVERY MISSION. Each mission's weapons go into the catalogue as it starts (port/src/endless.c): each
 * slot's definition (0xB8 bytes), its first-person model type's entry and animation numbers, and its model's file. A
 * weapon Bond carries that this mission lacks is then lent to it in a slot of one of its tools (the tools are no use
 * in a fight and taken away): the donor's definition, fired by this mission's pistol's fire handler (the guns share
 * one per mission: the donor's own is in the donor's code), its pistol's sound and bullets, and the donor's model,
 * whose file (another mission's: Lnn/NAME.HMD in that mission's archive) the model loader reads from the disc
 * (func_80076014/func_80076104 below; the model carries its own textures, which the game uploads as it makes it).
 * Guns only for now: a weapon fired otherwise (a launcher, a grenade) needs its handler's state in this mission. */
static int pistol_slot(void) {
    int i;
    for (i = 0; i < SLOTS; i++)
        if (!strncmp((char *)g_Weapons[i].pad0, "WOLFRAM P2K", 23))
            return i;
    return -1;
}

static u32 lent_mask; /* (the slots lent to this mission: theirs are another's) */
int endless_slot_lent(int slot) { /* (trace.c's aim trace) */
    return slot >= 0 && slot < 32 && (lent_mask >> slot & 1);
}
static u32 lent_work[0xE000 / 4]; /* (a lent weapon's model: func_800753F0) */
static u8 lent_work_type; /* (the model type whose file is in it) */
static char lent_work_path[24]; /* (and that file) */

/* The menu texts' letters (func_800291B0 makes a sprite of each: the game stops at 230, "inc MAX_TEXT_SPRITES!"):
 * logged when many are in use, with the texts */
extern int D_800AEDA8, D_800AC54C; /* (the letters in use, the texts) */
extern u8 D_800AB744[];            /* (the texts: 0x60 bytes each, the text at +0x10) */
static void text_watch(void) {
    static int worst;
    int i;
    if (D_800AEDA8 > worst && D_800AEDA8 >= 120 && D_800AEDA8 / 20 != worst / 20) {
        char b[400];
        int o = 0;
        for (i = 0; i < D_800AC54C && i < 40 && o < 360; i++)
            o += snprintf(b + o, sizeof(b) - o, "%s\"%.20s\"", i ? ", " : "", (char *)D_800AB744 + i * 0x60 + 0x10);
        ELOG("text: %d letters in %d texts: %s", D_800AEDA8, D_800AC54C, b);
    }
    if (D_800AEDA8 > worst)
        worst = D_800AEDA8;
}

/* ... and never past them: a text that would take more is cut to what's left (and logged with the texts in use),
 * where the game would stop */
void func_800291B0_orig(char *s, s16 x, s16 y, s32 *n, s32 arg4, s32 pal);
static struct { char s[40]; s32 n, after, x, y, arg4; u32 ra, sp; s32 frame, sub; } text_last[8];
static int text_at;
extern void (*port_fatal_hook)(void);
static void text_fatal(void) { /* (the game ends: the last texts drawn, the menu's texts) */
    int i, o = 0;
    char b[400];
    for (i = 0; i < 8; i++) {
        int k = (text_at + i) % 8;
        if (text_last[k].s[0] || text_last[k].n)
            ELOG("text (last %d): \"%s\" at %d,%d from %d letters to %d (arg4 %d, ra %08X sp %08X, frame %d%s)", 8 - i,
                 text_last[k].s, text_last[k].x, text_last[k].y, text_last[k].n, text_last[k].after, text_last[k].arg4,
                 text_last[k].ra, text_last[k].sp, text_last[k].frame, text_last[k].sub ? ", an extra drawing" : "");
    }
    for (i = 0; i < D_800AC54C && i < 40 && o < 360; i++)
        o += snprintf(b + o, sizeof(b) - o, "%s\"%.20s\"", i ? ", " : "", (char *)D_800AB744 + i * 0x60 + 0x10);
    ELOG("text: the game ends with %d letters in %d texts: %s", D_800AEDA8, D_800AC54C, b);
}
void func_800291B0(char *s, s16 x, s16 y, s32 *n, s32 arg4, s32 pal) {
    char cut[96];
    unsigned len = strlen(s), room = *n < 230 ? 230 - *n : 0;
    int me = text_at;
    port_fatal_hook = text_fatal;
    text_at = (text_at + 1) % 8;
    snprintf(text_last[me].s, sizeof(text_last[me].s), "%s", s);
    text_last[me].n = *n, text_last[me].after = -1, text_last[me].x = x, text_last[me].y = y, text_last[me].arg4 = arg4;
    text_last[me].ra = port_rc_ra, text_last[me].sp = port_rc_psp, text_last[me].frame = D_8009CDA8;
    text_last[me].sub = port_subframe;
    if (len > room) {
        static int told;
        if (told++ < 5) {
            char b[400];
            int i, o = 0;
            for (i = 0; i < D_800AC54C && i < 40 && o < 360; i++)
                o += snprintf(b + o, sizeof(b) - o, "%s\"%.20s\"", i ? ", " : "", (char *)D_800AB744 + i * 0x60 + 0x10);
            ELOG("text: \"%.40s\" cut to %u letters (%d in use, the game's most 230): %s", s, room, *n, b);
        }
        if (len >= sizeof(cut))
            len = sizeof(cut) - 1;
        memcpy(cut, s, len);
        cut[room < len ? room : len] = 0;
        s = cut;
    }
    func_800291B0_orig(s, x, y, n, arg4, pal);
    text_last[me].after = *n;
    if (getenv("TWINE_TEXT_TRACE")) /* (each text drawn: its letters' count before and after, where it's kept) */
        ELOG("text: \"%s\" at %d,%d: %d -> %d letters (count at %08X; ra %08X sp %08X%s)", s, x, y, text_last[me].n, *n,
             (u32)n, text_last[me].ra, text_last[me].sp, port_subframe ? ", an extra drawing" : "");
}
static void lent_ots(void);

static void weapons_capture(void) {
    int i, p = pistol_slot(), n = 0;
    u32 gun_fire = p >= 0 ? *(u32 *)((u8 *)&g_Weapons[p] + 0x3C) : 0;
    for (i = 0; i < SLOTS; i++) {
        u8 *w = (u8 *)&g_Weapons[i], t = w[0x5B], *te = D_8009D0FC ? D_8009D0FC + t * 0x70 : 0;
        int k;
        if (w[0] < 'A' || w[0] > 'Z' || (lent_mask & 1 << i)) /* (and not a slot lent to this mission) */
            continue;
        for (k = 0; k < 19 && D_8009D130[k * 4] != i; k++)
            ;
        port_weapon_put(g_Args->level, w, te, te ? *(u8 **)(te + 0x68) : 0, te ? *(u16 *)(te + 0x6C) : 0,
                        t && D_8009DB50[t] ? D_8009DB50[t] : "", gun_fire && *(u32 *)(w + 0x3C) == gun_fire,
                        w[0x5A] < LENT_SOUND && *(u8 **)((u8 *)g_Args + 0x130) ? *(u8 **)((u8 *)g_Args + 0x130) + w[0x5A] * 8 : 0,
                        k < 19 ? k : -1, k < 19 ? (char *)D_8009D130[k * 4 + 2] : 0, k < 19 ? (char *)D_8009D130[k * 4 + 3] : 0);
        n++;
    }
    port_weapons_saved();
    ELOG("weapons: the mission's %d weapons in the catalogue", n);
}

/* The tool slots a lent weapon works from as at home, per mission: the missions' own code treats some slots by
 * their number as their tools (Courier's 1 and 5, King's Ransom's grapple at 7, Meltdown's 4-6 ...: a gun there
 * fires once, or never, or as a grapple), found by firing an automatic lent into each, then a single-shot gun (the
 * IAC Defender): City of Walkways' 7, its flare gun's, plays the flare's reload sounds 7 and 10 frames into every
 * shot (playtest 8: "the IAC plays its reload sound after every shot"), so not 7 there */
/* Night Watch (4): its tools' slots 4-8 tried the same way (a Frinesi auto and the IAC Defender lent into each,
 * 2026-10-28): 4 (the tranquilizer's) and 8 (the palmtop's, its watch slot) fire as at home, sounds and all; 5, 6 and
 * 7 (the bug's, the card lock's, the camera's) never fire at all */
static const u16 clean_slots[12] = {0, 0, 1 << 6 | 1 << 11, 1 << 8, 1 << 4 | 1 << 8, 1 << 9, 1 << 6 | 1 << 10 | 1 << 11,
                                    1 << 11 | 1 << 12, 1 << 10, 1 << 10 | 1 << 11, 1 << 9, 0};
static struct {
    u8 level, loop;
    u8 sfx[8];
} lent_sound[8]; /* (a lent weapon's fire sound, its own: func_80026F30 below) */
static char lent_path[8][24];
static char lent_got[8][28], lent_ammo_got[8][32];
static u8 lent_anim[8][16];
s32 func_80026F30(u32 id, s32 flags);
void func_8007A17C(s32 state); /* (the weapon's state set: below) */

/* A PS1 function by its address (recompiled or native), from the mod's own code */
typedef struct { u32 r[32]; u32 hi, lo; u32 cop0[16]; } RcCtx;
void port_rc_call(RcCtx *c, u32 addr);
u32 port_rc_enter(void);
void port_rc_leave(u32 sp);
static void call_ps1(u32 addr) {
    RcCtx c;
    u32 sp;
    memset(&c, 0, sizeof(c));
    sp = port_rc_enter();
    c.r[29] = sp;
    port_rc_call(&c, addr);
    port_rc_leave(sp);
}

/* The Frinesi shotgun's and auto's fire (their missions' handler, func_800393A8 in King's Ransom: the guns' fire
 * five times over, one shell, one sound) in a mission without it: this mission's guns' fire five times, the shell
 * and the sound and the shot counted once */
static u32 lent_gun_fire; /* (this mission's guns' fire handler) */
static void lent_shotgun(void) {
    Weapon *w = &g_Weapons[D_800B0DD4];
    u8 *wb = (u8 *)w, snd = wb[0x5A];
    s32 shots = *(s32 *)((u8 *)g_Args + 0x2C8), k;
    s16 ammo, clip, reserve;
    /* (Courier's double-barrelled Frinesi shotgun fires both barrels a press, as its own handler does
     * (func_800391D4): six pellets, two shells, two shots counted; the auto, one shell, five pellets) */
    int both = !strncmp((char *)w->pad0, "FRINESI SHOTGUN", 15);
    if (!lent_gun_fire)
        return;
    call_ps1(lent_gun_fire);
    ammo = w->ammo, clip = w->clip, reserve = w->reserve;
    wb[0x5A] = 0xFF;
    for (k = 0; k < (both ? 5 : 4); k++) /* (the other pellets) */
        call_ps1(lent_gun_fire);
    wb[0x5A] = snd;
    if (both && (w->flags >> 13 & 1) && !(*(u32 *)((u8 *)g_Args + 4) >> 14 & 1) && clip > 0) /* (the second shell,
                                                                                          * but with the cheat) */
        clip--, ammo = clip + reserve;
    w->ammo = ammo, w->clip = clip, w->reserve = reserve;
    *(s32 *)((u8 *)g_Args + 0x2C8) = shots + (both ? 2 : 1);
}

static int shotgun_rec(const WeaponRec *r) {
    return !strncmp(r->name, "FRINESI", 7);
}

/* Thrown, launched and set weapons (frag grenade, flash bang, grenade launcher, rocket launcher, satchel charge and
 * its detonator) in a mission without them: their home code makes their mission's own objects, which this one
 * hasn't, so the quick grenade's own object flies instead (port/mods/quickgrenade.c quick_projectile), thrown or
 * fired from in front of Bond's view, the weapon's own fire animation (state 8) and sound */
int quick_projectile(int kind) __attribute__((weak));
int quick_detonate(void) __attribute__((weak));
s32 *quick_grenade_count(void) __attribute__((weak));
static int throw_kind(const char *nm) { /* (quickgrenade.c's QP_: -2 the detonator, -1 none) */
    return !strncmp(nm, "FRAG GRENADE", 12)     ? 0
           : !strncmp(nm, "FLASH BANG", 10)       ? 1
           : !strncmp(nm, "GRENADE LAUNCHER", 16) ? 2
           : !strncmp(nm, "ROCKET LAUNCHER", 15)  ? 3
           : !strncmp(nm, "SATCHEL CHARGE", 14)   ? 4
           : !strncmp(nm, "SATCHEL DETONATOR", 17) ? -2
                                                   : -1;
}

static s8 slot_pool[16];
static u8 pool_round[16];
static int sat_pi, det_pi, charges_out, sat_switch; /* (below: SATCHELS AND THEIR DETONATOR) */
static s32 sat_switch_at;
static void lent_throw(void) {
    static s32 last_tick = -100;
    Weapon *w = &g_Weapons[D_800B0DD4];
    u8 *wb = (u8 *)w;
    int kind = throw_kind((char *)w->pad0);
    if (D_8009CDA8 - last_tick >= 0 && D_8009CDA8 - last_tick < 15) /* (one a press: the throw's own pace) */
        return;
    last_tick = D_8009CDA8;
    if (kind == -2) { /* the detonator: the satchels out go off */
        int n = quick_detonate ? quick_detonate() : 0;
        ELOG("weapons: the detonator set off %d satchel charges", n);
        func_8007A17C(8);
        charges_out = 0;
        if (det_pi >= 0)
            pool_round[det_pi] = 0;
        if (sat_pi >= 0 && slot_pool[D_800B0DD4] == det_pi) /* (back to the charges, as at home) */
            sat_switch = D_800B0DD4, sat_switch_at = D_8009CDA8 + 7;
        return;
    }
    if (kind < 0 || !quick_projectile || w->ammo <= 0 || !quick_projectile(kind))
        return;
    if (w->flags & 0x2000) { /* (a clip: from it, as the guns' fire takes it) */
        w->clip--;
        if (w->clip <= 0 && w->reserve > 0 && wb[0x50] <= 1) /* (one at a time: the next to hand, as the throw's) */
            w->clip = 1, w->reserve--;
        w->ammo = w->clip + w->reserve;
    } else
        w->ammo--;
    (*(s32 *)((u8 *)g_Args + 0x2C8))++;
    if (wb[0x5A] < 0x80)
        func_80026F30(wb[0x5A], 0);
    func_8007A17C(8);
    if (kind == 4) { /* a charge out: the detonator Bond's, taken out as the throw ends (as at home) */
        charges_out++;
        if (det_pi >= 0) {
            pool_round[det_pi] = 1;
            if (slot_pool[D_800B0DD4] == sat_pi)
                sat_switch = D_800B0DD4, sat_switch_at = 0;
        }
    }
    ELOG("weapons: %s fired (the mod's own %s), %d left", (char *)w->pad0,
         kind == 0 ? "grenade" : kind == 1 ? "flash bang" : kind == 2 ? "round" : kind == 3 ? "rocket" : "charge", w->ammo);
}

/* THE LAUNCHERS, LENT, AS AT HOME. Their home missions fire them through weapon states of their own, which other
 * missions' state machines haven't (there they hit the default: state 1 at once, an error printed):
 *  - the rocket launcher (City of Walkways, L05): its fire handler (func_80039148) sets state 0x16, which plays the
 *    weapon's fire animation (its +0x7C) and, 5 frames into it, launches the rocket (func_80038C08: the mission's gun
 *    fire with the rocket's own touches) and goes on in state 8 with the animation running on (func_8007DCD8); the
 *    animation done, state 1. The rocket is one of the mission's bullets (kind 1, the model type's +0x2C), made from
 *    the weapon's numbers as every gun's are (func_8003872C: speed +0xB2, damage +0xB4, and +0x3B bit 0 -> the
 *    bullet's +0x64 bit 23: a blast where it hits, func_80033F38's tail: sound 0x40, func_80065614), with its hit
 *    mask +0x3A 4 (it hurts what bullets don't), +0x0E bit 8 (no bullet hole, no reaction, no gunshot heard where
 *    it lands) and +0x68's low bits cleared. (City of Walkways' state 0x17 is the wrench's, not the launcher's.)
 *    Lent, the rocket is this mission's bullet with City of Walkways' bullets' type put in for it (its entry: box
 *    and size, which the blast's reach grows from; its model): see Donor below.
 *  - the grenade launcher (Fallen Angel, L09): see lent_gl below.
 * Here each lent launcher's fire handler sets the home state; the state machine (quickpunch.c's func_800788E0) asks
 * endless_weapon_pre before it runs and endless_weapon_post after: meanwhile it runs state 8, whose code is every
 * mission's and does what the home state does but the launch (the same animation, def +0x74 + 8 = +0x7C, started
 * the same way; done -> state 1), and the launch is done here at the home state's moment. Outside the state machine
 * the state is the home one (the trace, the HUD, the other code see 0x16 as at home). TWINE_TRACE=weapon logs each
 * step ("launch:"). */
extern s16 D_800B0DA8, D_800B07B8; /* the bullets' objects (kind 1): first, last */
extern s32 D_800F11F0;            /* the weapon's state */
void func_80076184(s32 unit, s32 on); /* the muzzle flash shown */
extern s16 D_8009CF48;            /* frames since a shot */
void func_8007980C(void);           /* a thrower's next to hand (reload, state 0xB), as its throw ends */
static int launch_trace(void) {
    static int on = -1;
    if (on < 0)
        on = port_trace("weapon");
    return on;
}
static int lent_launcher(void) { /* (the weapon in hand: a lent launcher? 3 the rocket launcher, 2 the grenade one) */
    int k;
    if (D_800B0DD4 < 0 || D_800B0DD4 >= 16 || !(lent_mask >> D_800B0DD4 & 1))
        return -1;
    k = throw_kind((char *)g_Weapons[D_800B0DD4].pad0);
    return k == 2 || k == 3 ? k : -1;
}
static void lent_rocket_fire(void) { /* (the weapon's fire handler: func_80039148) */
    func_8007A17C(0x16);
    if (launch_trace())
        port_log("trace: L%d f%d launch: rocket launcher fired: state 0x16\n", g_Args->level, D_8009CDA8);
}
/* a member of a mission's archive (L<nn>/LVL<nn>CD.BIN: entries of 0x1C bytes, a name and its first sector), whole,
 * malloc'd; its size in *size */
u32 port_disc_part(const char *name, u32 offset, u32 len, u8 **data);
static u8 *disc_member(int lv, const char *name, u32 *size) {
    char arcname[32];
    u8 *arc, *e, *data = 0;
    u32 n;
    *size = 0;
    snprintf(arcname, sizeof(arcname), "L%02d/LVL%02dCD.BIN", lv, lv);
    if (!(n = port_disc_part(arcname, 0, 0x4000, &arc)))
        return 0;
    for (e = arc; e + 0x38 <= arc + n && *e; e += 0x1C)
        if (!strncmp((char *)e, name, 24)) {
            u32 at = *(u16 *)(e + 26) * 0x800u, end = *(u16 *)(e + 26 + 0x1C) * 0x800u;
            if (end > at && !(*size = port_disc_part(arcname, at, end - at, &data)))
                data = 0;
            break;
        }
    free(arc);
    return data;
}

/* (research, TWINE_ENDLESS_TEST_FINDTMD=<type>: that object type's model in the mission's DATA<nn>.BIN, its textured
 * primitives' texture pages and colours, its entry; and TWINE_ENDLESS_TEST_SFX=<id,...>: those sounds' table entries) */
extern s32 D_800D2D30[];
static void find_tmd(void) {
    const char *t = getenv("TWINE_ENDLESS_TEST_FINDTMD"), *sx = getenv("TWINE_ENDLESS_TEST_SFX");
    char name[24];
    u32 size, k, *ob;
    u8 *data;
    if (sx) {
        const u8 *d = *(u8 **)((u8 *)g_Args + 0x130);
        while (d && *sx) {
            int id = atoi(sx);
            ELOG("research: sound %d: %02X %02X %02X %02X %02X %02X %02X %02X", id, d[id * 8], d[id * 8 + 1],
                 d[id * 8 + 2], d[id * 8 + 3], d[id * 8 + 4], d[id * 8 + 5], d[id * 8 + 6], d[id * 8 + 7]);
            while (*sx && *sx != ',')
                sx++;
            if (*sx)
                sx++;
        }
    }
    if (getenv("TWINE_ENDLESS_TEST_ANIMS")) { /* (the people's animations: each one's frames and a hash of its data) */
        extern u8 **D_800DDEE4;
        extern u8 *D_800DDEE0;
        s32 n = (s32)((u8 **)(void *)D_800DDEE0 - D_800DDEE4), i;
        ELOG("research: animations: table %08X, next %08X: %d", (u32)(long)D_800DDEE4, (u32)(long)D_800DDEE0, n);
        for (i = 0; i < n && i < 400; i++) {
            u8 *e = D_800DDEE4[i];
            u32 h = 2166136261u, j, fr;
            if ((u32)(long)e < 0x80000000u || (u32)(long)e >= 0x80800000u) {
                ELOG("research: animation %d: at %08X", i, (u32)(long)e);
                continue;
            }
            fr = *(u16 *)e;
            for (j = 2; j < fr * 0x4C && j < 0x20000; j++)
                h = (h ^ e[j]) * 16777619u;
            ELOG("research: animation %d: %u frames, hash %08X", i, fr, h);
        }
        if (getenv("TWINE_ENDLESS_TEST_ANIMFIND")) { /* (=id,...: where those animations' frames and root motion
                                                       * are in the mission's archive's files) */
            const char *s = getenv("TWINE_ENDLESS_TEST_ANIMFIND");
            static const char *const files[] = {"DATA%02d.BIN", "GUY%02d1.BIN", 0};
            while (*s) {
                int id = atoi(s), f;
                u8 *e = D_800DDEE4[id], *m = ((u8 **)(void *)D_800DDEE0)[id];
                for (f = 0; files[f]; f++) {
                    char nm[32], fn[24];
                    u32 size, k;
                    u8 *data;
                    snprintf(fn, sizeof(fn), files[f], g_Args->level);
                    snprintf(nm, sizeof(nm), "L%02d/%s", g_Args->level, fn);
                    if (!(data = disc_member(g_Args->level, nm, &size)))
                        continue;
                    for (k = 0; k + 64 <= size; k += 2) {
                        if (!memcmp(data + k, e, 64))
                            ELOG("research: animation %d (%08X) frames at %s+0x%X", id, (u32)(long)e, nm, k);
                        if (!memcmp(data + k, m, 24))
                            ELOG("research: animation %d (%08X) root motion at %s+0x%X", id, (u32)(long)m, nm, k);
                    }
                    free(data);
                }
                while (*s && *s != ',')
                    s++;
                if (*s)
                    s++;
            }
        }
    }
    if (!t || !(ob = (u32 *)(long)D_800D2D30[atoi(t)]))
        return;
    ELOG("research: type %s model %08X: verts %08X x%u normals %08X x%u prims %08X x%u scale %d", t, (u32)(long)ob,
         ob[0], ob[1], ob[2], ob[3], ob[4], ob[5], ob[6]);
    { /* (its entry's bytes that aren't 0: {at, value}) */
        u8 *te = TYPE_DEF(atoi(t));
        char line[600];
        int m = 0;
        for (k = 0; k < 0x70 && m < 560; k++)
            if (te[k])
                m += snprintf(line + m, sizeof(line) - m, "{0x%X, 0x%X}, ", k, te[k]);
        line[m] = 0;
        ELOG("research: its entry: %s", line);
    }
    snprintf(name, sizeof(name), "L%02d/DATA%02d.BIN", g_Args->level, g_Args->level);
    if (!(data = disc_member(g_Args->level, name, &size)))
        return;
    for (k = 0; k + 32 <= size; k += 4)
        if (!memcmp(data + k, (void *)(long)ob[4], 32)) {
            u32 objtab = k - (ob[4] - (u32)(long)ob);
            ELOG("research: its primitives at %s+0x%X: the model (its header) at +0x%X: %08X %08X %08X", name, k,
                 objtab - 12, *(u32 *)(data + objtab - 12), *(u32 *)(data + objtab - 8), *(u32 *)(data + objtab - 4));
            { /* (its vertices: in memory and in the file) */
                s16 *mv = (s16 *)(long)ob[0], *fv = (s16 *)(data + objtab + *(u32 *)(data + objtab));
                u32 v;
                for (v = 0; v < ob[1] && v < 12; v++)
                    ELOG("research: vertex %u: memory %d %d %d, file %d %d %d", v, mv[v * 4], mv[v * 4 + 1], mv[v * 4 + 2],
                         fv[v * 4], fv[v * 4 + 1], fv[v * 4 + 2]);
            }
        }
    free(data);
    if (getenv("TWINE_ENDLESS_TEST_FINDTIM")) { /* (=x,y: the TIMs of LVL<nn>TIM.BIN whose picture or colours hold it) */
        int fx = atoi(getenv("TWINE_ENDLESS_TEST_FINDTIM")), fy = atoi(strchr(getenv("TWINE_ENDLESS_TEST_FINDTIM"), ',') + 1);
        snprintf(name, sizeof(name), "L%02d/LVL%02dTIM.BIN", g_Args->level, g_Args->level);
        if ((data = disc_member(g_Args->level, name, &size))) {
            for (k = 0; k + 20 <= size; k += 4)
                if (*(u32 *)(data + k) == 0x10 && (*(u32 *)(data + k + 4) & ~0xBu) == 0) {
                    u8 *q = data + k + 8;
                    u32 fl = *(u32 *)(data + k + 4);
                    s16 cx = 0, cy = 0, cw = 0, ch = 0;
                    if (fl & 8) {
                        cx = *(s16 *)(q + 4), cy = *(s16 *)(q + 6), cw = *(s16 *)(q + 8), ch = *(s16 *)(q + 10);
                        if (*(u32 *)q > 0x10000)
                            continue;
                        q += *(u32 *)q;
                    }
                    if (q + 12 > data + size)
                        continue;
                    {
                        s16 x = *(s16 *)(q + 4), y = *(s16 *)(q + 6), w = *(s16 *)(q + 8), h = *(s16 *)(q + 10);
                        if ((fx >= x && fx < x + w && fy >= y && fy < y + h) ||
                            (fx >= cx && fx < cx + cw && fy >= cy && fy < cy + ch))
                            ELOG("research: %s+0x%X: TIM flags %X, colours at %d,%d %dx%d, picture at %d,%d %dx%d", name, k,
                                 fl, cx, cy, cw, ch, x, y, w, h);
                    }
                }
            free(data);
        }
    }
    {
        u8 *q = (u8 *)(long)ob[4];
        for (k = 0; k < ob[5] && k < 400; k++, q += 4 + q[1] * 4)
            if (q[3] & 4)
                ELOG("research: prim %u mode %02X flag %02X olen %d ilen %d: clut %04X tpage %04X uv %d,%d %d,%d", k, q[3],
                     q[2], q[0], q[1], *(u16 *)(q + 6), *(u16 *)(q + 10), q[4], q[5], q[8], q[9]);
            else if (k < 4)
                ELOG("research: prim %u mode %02X flag %02X olen %d ilen %d (untextured)", k, q[3], q[2], q[0], q[1]);
    }
}

/* THE GRENADE LAUNCHER, LENT, AS AT HOME (Fallen Angel, L09). Its fire handler (func_800398A4) sets state 0x26 (the
 * muzzle flash shown, the fire timer reset); state 0x26 plays the fire animation (+0x7C) with the sound 0x2B as it
 * starts; 1 frame into it the round is launched from the weapon's model (func_80076CDC: its matrix and the weapon
 * object's +0x3C offset, between PushMatrix and PopMatrix: func_80076BF8 makes a projectile (kind 1) of the type at
 * the weapon object's +0x3A (Fallen Angel's 32), subtype +7 0x2C, +0xA5 0x18, +0x6A 0x818, damage 20000, +0x28
 * 2000, one off the clip; it flies at 500 pitched 0x5B up when aiming, else as Bond looks); the animation done, the
 * next round to hand (func_8007980C: state 0xB, the reload). In flight every mission's projectile code drops it
 * (subtype 0x2C: +0x9A += 0x10 a frame) and tells the guards near of it (func_80076B9C); where it hits, the hit loop
 * (func_80033F38) sets it off by its subtype (Fallen Angel's func_80079BB4: func_80065614's blast, sound 0x4B, the
 * object out), which only Fallen Angel's (and King's Ransom's, as another thing) have for 0x2C: other missions print
 * an error and leave it be. Here: the state as the rocket's (endless_weapon_pre/post); the round made by the
 * mission's own func_80039A9C with Fallen Angel's type entry and model put in its bullets' type for the call (the
 * object's +0 then the entry, kept here); its blast done here as Fallen Angel's, before the mission's hit loop
 * (func_80033F38 below) sees it. The model (DATA09.BIN, a TMD of 74 textured triangles) and its texture (a TIM of
 * LVL09TIM.BIN) are read from the disc, the texture put where no mission of a run has anything (944,320; colours
 * 16,508: every room's VRAM checked, TWINE_ENDLESS_TEST_VRAM) and the model's texture places moved there. Its two
 * sounds play from Fallen Angel's bank (their entries in its sounds' table below). TWINE_TRACE=weapon logs it. */
/* (the projectiles' types lent: a mission's object type's entry and model, put in the mission's bullets' type while
 * its code makes the object (then the object's +0 the entry, kept here). The model, a TMD, is read from the donor's
 * DATA<nn>.BIN; a textured one's texture from its LVL<nn>TIM.BIN, put where no mission of a run has anything and the
 * model's texture places moved there. The entry: its bytes that aren't 0, as the donor's table has them (that
 * table is made as the mission starts, not read from a file: TWINE_ENDLESS_TEST_FINDTMD dumps one)) */
typedef struct {
    const char *what;
    int level;
    const char *file;
    u32 at, prims;         /* (the model in the file, its primitives) */
    const char *tim_file;  /* (its texture: 0 none) */
    u32 tim_at;
    s16 pic_x, pic_y, tex_x, tex_y, clut_x, clut_y; /* (where the donor has its picture, 16x64 at 4 bits, and where
                                                     * it goes: picture and colours) */
    const u8 (*set)[2];
    int nset;
    u32 model[0xC00 / 4];
    u16 clut[16], image[16 * 64];
    int assets; /* (1 read, -1 not to be had) */
    u8 entry[0x70], save[0x70];
    s32 save_model;
    u8 tt;
} Donor;
/* Fallen Angel's object type 32 (class 4, subtype 4, its box, size, +0x64) and City of Walkways' bullets' (9) */
static const u8 gl_set[][2] = {{0, 32}, {1, 4}, {2, 4}, {0x10, 0xDD}, {0x11, 0xFF}, {0x12, 0x23}, {0x14, 0xA4},
                               {0x15, 0xFF}, {0x16, 0x48}, {0x18, 0xDD}, {0x19, 0xFF}, {0x1A, 0x23}, {0x1C, 0xC0},
                               {0x1E, 0x32}, {0x64, 0x4A}};
static const u8 rocket_set[][2] = {{0, 9}, {1, 1}, {0x10, 0xE5}, {0x11, 0xFF}, {0x12, 0x1B}, {0x14, 0xE5}, {0x15, 0xFF},
                                   {0x16, 0x1B}, {0x18, 0x77}, {0x19, 0xFD}, {0x1A, 0x03}, {0x1C, 0x90}, {0x1D, 0x02},
                                   {0x1E, 0x8A}, {0x1F, 0x02}, {0x20, 0xEE}, {0x21, 0x02}, {0x24, 0xC8}, {0x2C, 0x01},
                                   {0x5C, 0x29}, {0x64, 0x14}};
static Donor gl_type = {"the grenade launcher's round (Fallen Angel's type 32)", 9, "L09/DATA09.BIN", 0x9884, 74,
                        "L09/LVL09TIM.BIN", 0x11E24, 784, 384, 944, 320, 16, 508, gl_set,
                        sizeof(gl_set) / sizeof(gl_set[0])};
static Donor rocket_type = {"the rocket (City of Walkways' bullets, type 9)", 5, "L05/DATA05.BIN", 0x34A4, 20, 0, 0,
                            0, 0, 0, 0, 0, 0, rocket_set, sizeof(rocket_set) / sizeof(rocket_set[0])};
static const u8 gl_fire_sfx[8] = {4, 3, 0x28, 0, 0x7F, 0, 0, 0};  /* (Fallen Angel's sound 0x2B: program, tone, note,
                                                                   * voice, volume, randomness) */
static const u8 gl_blast_sfx[8] = {6, 7, 0x3C, 1, 0x7F, 0, 0, 0}; /* (its 0x4B) */
#define GL_LIVE 4
static u8 *gl_round[GL_LIVE];
static s32 gl_round_born[GL_LIVE];
void GsMapModelingData(u32 *p);
void gpu_load_image(s32 x, s32 y, s32 w, s32 h, const u16 *src);
void PushMatrix(void);
void PopMatrix(void);
void func_8003B53C(u8 *obj, s32 yaw, s32 pitch, s32 speed); /* a projectile sent off that way at that speed */
void func_8003B808(s32 pos);       /* a place's pans (g_Args+0x136, +0x137) for a sound there */
s32 func_80065614(u8 *obj, s32 a, s32 b); /* an explosion where it is (and a blast, func_800378FC): 0 none made */
extern u8 D_800B0018[];            /* the view's rotation */
extern u16 D_800B0C5C;             /* aimed: the yaw */
extern s32 D_800B0C64;             /*        the pitch */
extern s8 D_800D920C;              /* (the rumble's) */
extern s16 D_800D9208;
static s32 tone_as_game(int level, const u8 *d, s32 flags, int loop); /* (below: func_80026F30's way) */

static void donor_load(Donor *d) {
    u32 n = 0, tn = 0, size, i, k, *ob, ta = d->tim_at;
    u8 *data = disc_member(d->level, d->file, &n), *tim = d->tim_file ? disc_member(d->level, d->tim_file, &tn) : 0, *q;
    s32 du = ((d->tex_x & 63) - (d->pic_x & 63)) * 4, dv = (d->tex_y & 255) - (d->pic_y & 255);
    d->assets = -1;
    if (data && n > d->at + 40 && *(u32 *)(data + d->at) == 0x41 && *(u32 *)(data + d->at + 8) == 1 &&
        (!d->tim_file || (tim && tn > ta + 0x40 + 0x800 && *(u32 *)(tim + ta) == 0x10 && *(u32 *)(tim + ta + 4) == 8 &&
                          *(u32 *)(tim + ta + 8) == 0x2C && *(s16 *)(tim + ta + 0x10) == 16 &&
                          *(u32 *)(tim + ta + 0x34) == 12 + 0x800 && *(s16 *)(tim + ta + 0x38) == d->pic_x &&
                          *(s16 *)(tim + ta + 0x3A) == d->pic_y && *(s16 *)(tim + ta + 0x3C) == 16 &&
                          *(s16 *)(tim + ta + 0x3E) == 64))) {
        ob = (u32 *)(data + d->at + 12); /* (verts, their count, normals, theirs, primitives, theirs: from the table) */
        q = (u8 *)ob + ob[4];
        for (i = 0; i < ob[5] && q + 4 <= data + n; i++)
            q += 4 + q[1] * 4;
        size = q - (u8 *)ob;
        if (ob[0] + ob[1] * 8 > size)
            size = ob[0] + ob[1] * 8;
        if (ob[2] + ob[3] * 8 > size)
            size = ob[2] + ob[3] * 8;
        if (ob[5] == d->prims && size + 12 <= sizeof(d->model) && d->at + 12 + size <= n) {
            memcpy(d->model, data + d->at, size + 12);
            if (d->tim_file) {
                memcpy(d->clut, tim + ta + 0x14, sizeof(d->clut));
                memcpy(d->image, tim + ta + 0x40, sizeof(d->image));
                /* (each textured primitive's colours, texture page and places in it moved) */
                q = (u8 *)(d->model + 3) + d->model[3 + 4];
                for (i = 0; i < d->model[3 + 5]; i++, q += 4 + q[1] * 4)
                    if (q[3] & 4) {
                        *(u16 *)(q + 6) = d->clut_y << 6 | d->clut_x >> 4;
                        *(u16 *)(q + 10) = (*(u16 *)(q + 10) & ~0x1F) | (d->tex_x / 64) | (d->tex_y / 256) << 4;
                        for (k = 0; k < (q[3] & 8 ? 4u : 3u); k++)
                            q[4 + k * 4] += du, q[5 + k * 4] += dv;
                    }
            }
            GsMapModelingData(d->model + 1);
            memset(d->entry, 0, sizeof(d->entry));
            for (i = 0; i < (u32)d->nset; i++)
                d->entry[d->set[i][0]] = d->set[i][1];
            d->assets = 1;
        }
    }
    free(data);
    free(tim);
    ELOG("weapons: %s %s", d->what, d->assets > 0 ? "read from its mission's files" : "NOT found on the disc");
}

/* the donor's type into this mission's type tt (its entry, its model) while the mission's code makes one: 0 if it
 * can't be had */
static int donor_in(Donor *d, u8 tt) {
    if (!d->assets)
        donor_load(d);
    if (d->assets < 0)
        return 0;
    d->tt = tt;
    memcpy(d->save, TYPE_DEF(tt), 0x70);
    d->entry[0] = tt;
    memcpy(TYPE_DEF(tt), d->entry, 0x70);
    d->save_model = D_800D2D30[tt];
    D_800D2D30[tt] = (s32)(long)(d->model + 3);
    if (d->tim_file) { /* (again each time: what the mission loads may cover it) */
        gpu_load_image(d->tex_x, d->tex_y, 16, 64, d->image);
        gpu_load_image(d->clut_x, d->clut_y, 16, 1, d->clut);
    }
    return 1;
}

/* ... and back: the object made then given the donor's entry for good */
static void donor_out(Donor *d, u8 *made) {
    memcpy(TYPE_DEF(d->tt), d->save, 0x70);
    D_800D2D30[d->tt] = d->save_model;
    if (made)
        *(u8 **)made = d->entry;
}

/* The lent rocket's smoke trail: the rocket's step (every mission's) leaves a puff every other frame from its sprite
 * table's entry for it (0x14 bytes: size, mode, colour, texture place, page, colours), which City of Walkways fills
 * (and Turncoat, Flashpoint, Masquerade, Fallen Angel, Meltdown, the same); in Courier and King's Ransom the entry is
 * its size and mode with nothing else (colour 0, page and colours 0), so each puff was drawn from VRAM's top left
 * corner, half transparent: a big dark-grey "house" of puffs by Bond (playtest 8: "the rocket looks wrong in
 * Courier, a large grey box"). Such an entry is given the trail's grey and the texture of the entry after it, the
 * mission's own smoke puff (C8 C8, mode 35: every mission's, for its blasts); City of Walkways' own numbers point
 * elsewhere in King's Ransom's VRAM (yellow and black stripes there: TWINE_ENDLESS_TEST_TRAIL_HOME=1 shows them).
 * TWINE_ENDLESS_TEST_NO_TRAIL_FIX=1: as it was. TWINE_TRACE=proj with TWINE_TRACE_PROJ_ALL=1 logs each puff's entry. */
extern u8 D_800DB884[]; /* the sprites (30 of 0xA0 bytes: +0x9B in use, +0x50 its entry) */
static void trail_entry(u8 *d) { /* (an empty entry: the next one's texture, the trail's grey) */
    int k;
    u8 *n = d + 0x14;
    for (k = 5; k < 0x14 && !d[k]; k++)
        ;
    if (k < 0x14 || !(n[12] | n[13]) || !(n[14] | n[15]))
        return; /* (an entry with its texture, or none after it to take one from) */
    static const u8 home[12] = {0x80, 0x80, 0x3F, 0x3F, 0x60, 0x02, 0xC0, 0x01, 0x60, 0x02, 0x80, 0x01};
    d[5] = d[6] = d[7] = 0x60;
    if (getenv("TWINE_ENDLESS_TEST_TRAIL_HOME")) /* (research: City of Walkways' numbers: in King's Ransom's VRAM that
                                                  * place holds yellow and black stripes) */
        memcpy(d + 8, home, 12);
    else /* (the mission's own smoke puff, the entry after: its blasts' smoke) */
        memcpy(d + 8, n + 8, 12);
    ELOG("weapons: the lent rocket's trail (sprite entry %08X) had no texture here: the mission's smoke puff given it",
         (u32)(long)d);
}
static void rocket_trail(void) {
    int i, k;
    static int gen = -1;
    if (getenv("TWINE_ENDLESS_TEST_NO_TRAIL_FIX"))
        return;
    if (gen != port_endless_gen()) { /* (each mission, before its first puff: its sprite table's trail entry, found
                                      * by its shape -- 3C 3C, mode 27, then every mission's smoke entry, C8 C8, mode
                                      * 35, white, its texture at 0,80 3F 3F; the table is made as the mission starts) */
        u8 *a;
        gen = port_endless_gen();
        for (a = (u8 *)0x800C0000u; a < (u8 *)0x80100000u - 0x28; a += 4)
            if (a[0] == 0x3C && a[2] == 0x3C && a[4] == 0x27 && a[0x14] == 0xC8 && a[0x16] == 0xC8 && a[0x18] == 0x35 &&
                a[0x19] == 0x80 && a[0x1C] == 0 && a[0x1D] == 0x80 && a[0x1E] == 0x3F && a[0x1F] == 0x3F && !a[1] &&
                !a[3] && !a[0x15] && !a[0x17])
                trail_entry(a);
    }
    for (i = 0; i < 30; i++) { /* (and any puff's entry: in case the table wasn't found) */
        u8 *sp = D_800DB884 + i * 0xA0, *d = *(u8 **)(sp + 0x50);
        if (!sp[0x9B] || !d || (u32)(long)d < 0x80000000u)
            continue;
        trail_entry(d);
    }
}

static void lent_rocket_launch(void) { /* (func_80038C08: the gun fire, the rocket's touches, state 8 on) */
    u8 was[256], *def = (u8 *)&g_Weapons[D_800B0DD4];
    int i, n = 0;
    for (i = D_800B0DA8; i <= D_800B07B8 && i < 256; i++)
        was[i] = OBJ(i)[4];
    if (!lent_gun_fire || !donor_in(&rocket_type, D_8009CD98[0x3A])) /* (the gun fire makes the bullet of the type
                                                                          * at the weapon object's +0x3A: City of
                                                                          * Walkways' bullets' entry and model) */
        return;
    call_ps1(lent_gun_fire);
    donor_out(&rocket_type, 0);
    rocket_trail();
    for (i = D_800B0DA8; i <= D_800B07B8 && i < 256; i++) {
        u8 *o = OBJ(i);
        if (!was[i] && o[4] && o[5] == 1 && *(void **)(o + 0x88) == g_Player) {
            *(u8 **)o = rocket_type.entry;
            o[0x3A] = 4;
            *(u16 *)(o + 0xE) |= 0x100;
            *(u32 *)(o + 0x68) &= ~0xFu;
            n++;
            if (launch_trace())
                port_log("trace: L%d f%d launch: rocket #%d (type %d, damage %d, blast %d)\n", g_Args->level,
                         D_8009CDA8, i, o[6], *(s32 *)(o + 0x34), *(u32 *)(o + 0x64) >> 23 & 1);
        }
    }
    if ((s8)def[0x7C] != -1) { /* (func_8007DCD8: state 8, the animation asked again (+0x78 -1)) */
        u8 *inst = *(u8 **)(D_8009CD98 + 0x114);
        func_8007A17C(8);
        /* (at home this is inside the state machine, before its last step (func_800755BC) takes the animation asked,
         * +0x88, as the one running on; here that step has run already this frame: the same done) */
        if (inst && *(s32 *)(inst + 0x88) >= 0)
            *(s32 *)(inst + 0x78) = *(s32 *)(inst + 0x88), *(s32 *)(inst + 0x7C) = *(s32 *)(inst + 0x8C);
    }
    if (!n)
        ELOG("weapons: the lent rocket launcher made no rocket (no bullet free)");
}

static void lent_gl_fire(void) { /* (func_800398A4) */
    s32 k = ((u32)g_Weapons[D_800B0DD4].flags >> 15) & 0xF;
    func_8007A17C(0x26);
    if (k > 0)
        func_80076184(k, 0);
    D_8009CF48 = 0;
    if (launch_trace())
        port_log("trace: L%d f%d launch: grenade launcher fired: state 0x26\n", g_Args->level, D_8009CDA8);
}

/* the round launched from the weapon's model (func_80076CDC with func_80076BF8) */
static void gl_launch(void) {
    u8 *obj = D_8009CD98, *player = (u8 *)g_Player, *proj, *pm, tt;
    Weapon *w = &g_Weapons[D_800B0DD4];
    s32 k, m[3];
    const s16 *r = (const s16 *)D_800B0018;
    s16 *off;
    if (!obj || !*(u8 **)(obj + 0x114))
        return;
    { /* (where: the model's matrix, its translation, and the weapon's offset turned as the view is) */
        s32 *t = (s32 *)(*(u8 **)(obj + 0x114) + 0x38 + 0x14);
        off = (s16 *)(obj + 0x3C);
        for (k = 0; k < 3; k++)
            m[k] = (r[k * 3] * off[0] + r[k * 3 + 1] * off[1] + r[k * 3 + 2] * off[2]) >> 12;
        MAKE_AT[0] = t[0] + m[0], MAKE_AT[1] = t[1] + m[1], MAKE_AT[2] = t[2] + m[2];
        ((s16 *)0x1F8003E0u)[0] = off[0], ((s16 *)0x1F8003E0u)[1] = off[1], ((s16 *)0x1F8003E0u)[2] = off[2];
        ((s32 *)0x1F8003E8u)[0] = m[0], ((s32 *)0x1F8003E8u)[1] = m[1], ((s32 *)0x1F8003E8u)[2] = m[2];
    }
    if (w->ammo <= 0)
        return;
    tt = obj[0x3A]; /* (the mission's bullets' type: its entry and model Fallen Angel's round's for the call) */
    if (!donor_in(&gl_type, tt))
        return;
    proj = func_80039A9C((s32)(long)player, tt, 1);
    donor_out(&gl_type, proj);
    if (!proj) {
        ELOG("weapons: the lent grenade launcher made no round (no object free)");
        return;
    }
    proj[7] = 0x2C;
    proj[0xA5] = 0x18;
    *(s16 *)(proj + 0x6A) = 0x818;
    *(s32 *)(proj + 0x34) = 0x4E20;
    *(s16 *)(proj + 0x28) = 0x7D0;
    D_800D920C = 2;
    w->clip--;
    *(s32 *)(proj + 0x2C) = D_8009CDA8;
    *(u16 *)(proj + 0xAC) = (u16)MAKE_AT[1];
    D_800D9208 = (s16)(*(u32 *)((u8 *)w + 0x5C) >> 26);
    *(s16 *)(obj + 0x38) = 500;
    pm = *(u8 **)(player + 0x90);
    if (D_8009CD4C)
        func_8003B53C(proj, *(s32 *)&D_800B0C5C, D_800B0C64 + 0x5B, 500);
    else
        func_8003B53C(proj, *(s16 *)(pm + 0x62), *(s16 *)(pm + 0x60), 500);
    *(s16 *)(proj + 0x56) = -1;
    *(s16 *)(*(u8 **)(proj + 0x90) + 0x60) = 0x400;
    *(s16 *)(*(u8 **)(proj + 0x90) + 0x62) = D_8009CD4C ? (s16)D_800B0C5C : *(s16 *)(pm + 0x62);
    *(s32 *)(*(u8 **)(proj + 0x90) + 0x10) = 2;
    D_8009CF48 = 0;
    k = ((u32)w->flags >> 15) & 0xF;
    if (k > 0)
        func_80076184(k, 0);
    for (k = 0; k < GL_LIVE && gl_round[k] && gl_round[k][4] && *(u8 **)gl_round[k] == gl_type.entry &&
                *(s32 *)(gl_round[k] + 0x2C) == gl_round_born[k];
         k++)
        ;
    if (k < GL_LIVE)
        gl_round[k] = proj, gl_round_born[k] = D_8009CDA8;
    if (launch_trace())
        port_log("trace: L%d f%d launch: grenade round #%d (the mission's type %d as Fallen Angel's 32) at %d %d %d, clip %d\n",
                 g_Args->level, D_8009CDA8, (int)((proj - (u8 *)(long)D_800AFFC8) / 0x124), tt, MAKE_AT[0], MAKE_AT[1],
                 MAKE_AT[2], w->clip);
}

/* the mission's hit loop about to run: a lent launcher's round that has hit something goes off as Fallen Angel's
 * does (func_80079BB4) */
static int gl_live(int g) { /* (round g still that round) */
    u8 *o = gl_round[g];
    return o && o[4] && *(u8 **)o == gl_type.entry && *(s32 *)(o + 0x2C) == gl_round_born[g];
}

/* THE HIT LOOP ONLY IN THE TICK'S OWN DRAWING (campaign and runs alike): the game's hit loop (each projectile's hits:
 * the person or thing hit hurt, the impact's sound, spark and blast) runs in its render step (the people's drawing,
 * func_8005AEF8), so smooth motion's extra drawings (port/src/interp.c, mods/smooth.c), which run the render step
 * again before the tick's own, ran it too, each time on the tick's hits: its memory was put back after, but what it
 * did outside PS1 RAM stayed (the sound trace saw an impact's sound called 10+ times a tick; host-played sounds,
 * the lent weapons', would play again), and it cost each drawing. Game logic, not drawing: now skipped there.
 * TWINE_TEST_SMOOTH_HITS=1 runs it in every drawing again (to show the bug). TWINE_TRACE=hitloop logs each tick's
 * run: projectiles with hits, sounds asked for meanwhile, extra drawings that skipped it. */
static int hitloop_sounds; /* (sounds asked for while the hit loop runs: func_80026F30) */
static int hitloop_in;
static int hitloop_trace(void) {
    static int on = -1;
    if (on < 0)
        on = port_trace("hitloop");
    return on;
}
void func_80033F38_orig(void);
static void hit_loop(void);
static void hits_frame(void);
void func_80033F38(void) {
    static int test = -1;
    static s32 skipped, last_tick = -1;
    if (test < 0)
        test = getenv("TWINE_TEST_SMOOTH_HITS") && atoi(getenv("TWINE_TEST_SMOOTH_HITS"));
    if (port_subframe && !test) {
        skipped++;
        return;
    }
    if (getenv("TWINE_ENDLESS_AMMO_LOG") && !port_subframe) { /* (headless tests: each of Bond's bullets that hits a person,
                                                               * its own damage (+0x34, the weapon's +0xAC): the hit's
                                                               * health drop is that times where it hit) */
        int i, k;
        for (i = D_800B0DA8; i <= D_800B07B8 && i < 256; i++) {
            u8 *o = OBJ(i), *b = *(u8 **)(o + 0x10C);
            if (!o[4] || !b || *(void **)(o + 0x88) != (void *)g_Player)
                continue;
            for (k = 0; k < b[0x64] && k < 8; k++) {
                u8 *t = *(u8 **)(b + k * 0x14 + 0xC);
                if (t && t[4] && t[5] == 6 && t != (u8 *)g_Player) {
                    ELOG("bullet: %s's hit person %d: its damage %d", D_800B0DD4 >= 0 ? (char *)g_Weapons[D_800B0DD4].pad0 : "-",
                         (int)((t - OBJ(0)) / 0x124), *(s32 *)(o + 0x34));
                    break;
                }
            }
        }
    }
    if (hitloop_trace()) {
        int i, n = 0;
        for (i = D_800B0DA8; i <= D_800B07B8 && i < 256; i++) { /* (projectiles with hits: +0x108's +0x64) */
            u8 *o = OBJ(i), *b = *(u8 **)(o + 0x10C);
            if (o[4] && b && b[0x64])
                n++;
        }
        hitloop_sounds = 0, hitloop_in = 1;
        hit_loop();
        hitloop_in = 0;
        if (n || hitloop_sounds || port_subframe)
            port_log("trace: L%d f%d hitloop: %s, %d with hits, %d sounds%s (extra drawings skipping it since: %d)\n",
                     g_Args->level, D_8009CDA8, port_subframe ? "an extra drawing's (TWINE_TEST_SMOOTH_HITS)" : "the tick's",
                     n, hitloop_sounds, D_8009CDA8 == last_tick && !port_subframe ? ", AGAIN this tick" : "", skipped);
        if (!port_subframe)
            last_tick = D_8009CDA8;
        skipped = 0;
        return;
    }
    hit_loop();
}

/* (the lent grenade round's hits, then the game's loop) */
static void hit_loop(void) {
    int g, k;
    for (g = 0; g < GL_LIVE; g++) {
        u8 *o = gl_round[g], *base;
        if (!o)
            continue;
        if (!gl_live(g)) {
            if (!port_subframe)
                gl_round[g] = 0;
            continue;
        }
        base = *(u8 **)(o + 0x10C);
        if (!base || !base[0x64])
            continue;
        if (port_subframe) {
            base[0x64] = 0;
            continue;
        }
        base[0x64] = 0;
        *(s16 *)(o + 0x98) = *(s16 *)(o + 0x9A) = *(s16 *)(o + 0x9C) = 0;
        *(s16 *)(o + 0x9E) = *(s16 *)(o + 0xA0) = *(s16 *)(o + 0xA2) = 0;
        k = func_80065614(o, 1, 5);
        func_8003B808((s32)(long)(*(u8 **)(o + 0x90) + 0x14));
        tone_as_game(9, gl_blast_sfx, 8, 0);
        if (launch_trace())
            port_log("trace: L%d f%d launch: grenade round #%d went off at %d %d %d (%s)\n", g_Args->level, D_8009CDA8,
                     (int)((o - (u8 *)(long)D_800AFFC8) / 0x124), POS(o)[0], POS(o)[1], POS(o)[2],
                     k ? "a blast" : "no blast: no room for its explosion");
        func_800395E8(o);
        if (launch_trace())
            port_log("trace: L%d f%d launch: (the round's object after: active %d, hits %d)\n", g_Args->level, D_8009CDA8,
                     o[4], base[0x64]);
        gl_round[g] = 0;
    }
    func_80033F38_orig();
}

/* a projectile's own step each tick (func_800352B8 calls it before moving it): a lent launcher's round as Fallen
 * Angel's (its subtype 0x2C: dropping 0x10 a tick if anyone's, the guards near told of it, func_80076A8C), where the
 * mission's own does something else or nothing for 0x2C (King's Ransom's nothing): the mission's run with the
 * subtype 0 meanwhile (a bullet's: nothing of its own in any mission; 4, say, is a grenade in Christmas's) */
void func_80076A8C(u8 *o);
void func_800350F0_orig(u8 *o);
void func_800350F0(u8 *o) {
    int g;
    for (g = 0; g < GL_LIVE && !(gl_round[g] == o && gl_live(g)); g++)
        ;
    if (g == GL_LIVE) {
        func_800350F0_orig(o);
        if (*(u8 **)o == rocket_type.entry) /* (the lent rocket: its trail's puffs) */
            rocket_trail();
        return;
    }
    o[7] &= 0xC0;
    func_800350F0_orig(o);
    o[7] = (o[7] & 0xC0) | 0x2C;
    if (*(s32 *)(o + 0x88))
        *(s16 *)(o + 0x9A) += 0x10;
    func_80076A8C(o);
}

/* THE FLASH BANG, LENT, AS AT HOME (Courier, L02). Its fire handler (func_800397C4) sets weapon state 15 and takes one
 * from the ammo. State 15 (Courier's jump table, jtgt_8007CAD4) plays the weapon's fire animation (+0x7C) and, as it
 * starts, sound 0x34 with a counter (D_800A1198) at 0; each frame of the state: the screen's fade colour for a frame
 * (func_8004B4F8 = LEVEL01's func_8004A8A8: counter 0, 1, 3 white FF FF FF, 2 E1 E1 FF, 4 C3 C3 FF) and its two
 * prims (LEVEL01's D_800D77B8) drawn solid for counters 0-3, see-through again at 4 (func_80048B0C: SetSemiTrans);
 * the counter up one; the animation done: sound 0xD, state 1, the counter -1, and no ammo left: the weapon out of the
 * round (func_8007DEF4: flags bit 12 off). A guard seeing Bond (the AI's look, func_80053DF0 = LEVEL01's
 * func_80051AE0, as it finds him seen: func_8005584C = func_80053804) while the counter is 1 gets AI flag 0x40000, and
 * his script (Courier's GOON1.AIB, 0x41D: an interrupt on that flag) does the rest: within 5000 he is blinded (his
 * "my eyes!" (Courier's sound 0x6A) at his place, the blinded animation at rate 2 or 6 (half and half: rand), into his
 * standing one, then his recovering one; his combat state +0x3C 0 meanwhile, so he can't shoot); between 5000 and
 * 12000 nothing (his main routine again); further, he stands a while (60 frames). Only Courier and Flashpoint (whose
 * state 15 and scripts are the same: counter D_8009EF40, its own "my eyes!", 0x60) have any of this: elsewhere
 * state 15 is the default's (state 1 at once, an error printed) and no script has the flag's interrupt.
 * Lent, elsewhere: the fire handler as at home; state 15 run as state 8 (every mission's: the same animation, started
 * the same way; done -> 1, see endless_weapon_pre) with the rest done here at its moments (the sounds from Courier's
 * bank, the fade with the mission's own function and prims); the guard's flag set as at home (func_80053804 below)
 * and his blinding run by the game's own script interpreter from a copy of Courier's routine (THE FLASH BANG'S GUARDS,
 * below). In Courier and Flashpoint the game does it all. TWINE_TRACE=flash logs each step (and mods/trace.c the
 * state, fade and the guards, at home and lent alike). */
static const u8 cour_flash_sfx[8] = {4, 0x0C, 0x7E, 0, 0x7F, 0, 0, 0};    /* (Courier's sound 0x34: program, tone, note,
                                                                           * voice, volume, randomness) */
static const u8 cour_flash_end_sfx[8] = {1, 0x0C, 0x7E, 0, 0x7F, 0, 0, 0}; /* (its 0x0D) */
static s32 flash_count = -1; /* (Courier's D_800A1198, for a lent flash bang) */
static int flash_starting, flash_done;
void func_8004A8A8(s8 r, s8 g, s8 b, s16 frames); /* the screen's fade colour for a while */
extern u8 D_800D77B8[];                         /* its two prims (0x18 bytes each, one a buffer) */
static int flash_trace(void) {
    static int on = -1;
    if (on < 0)
        on = port_trace("flash");
    return on;
}
/* In a run the flash is made to last twice as long (the playtest's ask): the white over the screen 8 frames solid and
 * 2 see-through (Courier's 4 and 1: each colour of its sequence twice), at home too; a blinded guard twice as long
 * (THE FLASH BANG'S GUARDS), at home too (Courier's and Flashpoint's guards then run the same commands). Courier's
 * own: TWINE_ENDLESS_TEST_FLASH_VANILLA=1 (for comparing home and lent, tools/flash_trace.py). */
static int flash_vanilla(void) {
    static int v = -1;
    if (v < 0)
        v = getenv("TWINE_ENDLESS_TEST_FLASH_VANILLA") && atoi(getenv("TWINE_ENDLESS_TEST_FLASH_VANILLA"));
    return v;
}
/* the fade for state 15's counter c (before it goes up): Courier's, or twice as long */
static void flash_fade(s32 c) {
    static const u8 fade[5][3] = {{0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xE1, 0xE1, 0xFF}, {0xFF, 0xFF, 0xFF},
                                  {0xC3, 0xC3, 0xFF}};
    int b, k = flash_vanilla() ? c : c / 2, semi = flash_vanilla() ? c == 4 : c >= 8;
    if (c < 0 || k > 4)
        return;
    for (b = 0; b < 2; b++) /* (func_80048B0C: SetSemiTrans) */
        D_800D77B8[b * 0x18 + 7] = semi ? D_800D77B8[b * 0x18 + 7] | 2 : D_800D77B8[b * 0x18 + 7] & ~2;
    func_8004A8A8(fade[k][0], fade[k][1], fade[k][2], 1);
}
static int flash_native(void) { /* (the mission's own state 15 and scripts) */
    return g_Args->level == 2 || g_Args->level == 6;
}
static int lent_flash(void) { /* (a lent flash bang in hand) */
    return D_800B0DD4 >= 0 && D_800B0DD4 < 16 && (lent_mask >> D_800B0DD4 & 1) &&
           throw_kind((char *)g_Weapons[D_800B0DD4].pad0) == 1;
}
static s32 native_flash_count(void) { /* (Courier's D_800A1198, Flashpoint's D_8009EF40) */
    return g_Args->level == 2 ? *(s16 *)0x800A1198u : g_Args->level == 6 ? *(s16 *)0x8009EF40u : -1;
}
/* (mods/trace.c) the lent flash bang's counter, as Courier's D_800A1198; -99 none lent here */
s32 endless_flash_count(void) {
    return flash_native() ? -99 : flash_count;
}
static void lent_flash_fire(void) { /* (func_800397C4) */
    func_8007A17C(0xF);
    g_Weapons[D_800B0DD4].ammo--;
    ELOG("weapons: %s fired (state 15, as at home), %d left", (char *)g_Weapons[D_800B0DD4].pad0, g_Weapons[D_800B0DD4].ammo);
    if (flash_trace())
        port_log("trace: L%d f%d flash: fired: state 15 (%s)\n", g_Args->level, D_8009CDA8,
                 flash_native() ? "the mission's own" : "run as state 8, the rest as Courier's");
}
/* state 15's own, each frame of it (after the state machine ran it as state 8) */
static void flash_state_post(void) {
    s32 c;
    if (flash_starting) {
        tone_as_game(2, cour_flash_sfx, 0, 0);
        flash_count = 0;
    }
    c = flash_count;
    flash_fade(c);
    flash_count++;
    if (flash_trace())
        port_log("trace: L%d f%d flash: state 15 counter %d -> %d%s%s\n", g_Args->level, D_8009CDA8, c, flash_count,
                 flash_starting ? ", its animation started, sound 0x34 (Courier's)" : "", flash_done ? ", done" : "");
    if (flash_done) {
        Weapon *w = &g_Weapons[D_800B0DD4];
        tone_as_game(2, cour_flash_end_sfx, 0, 0);
        D_800F11F0 = 1;
        flash_count = -1;
        if (w->ammo < 1)
            w->flags &= ~0x1000;
        if (flash_trace())
            port_log("trace: L%d f%d flash: state 15 ends: sound 0xD (Courier's), state 1, ammo %d\n", g_Args->level,
                     D_8009CDA8, w->ammo);
    }
}

/* the state machine about to run (quickpunch.c): a lent launcher's home state run as state 8 meanwhile; what the
 * home state does besides, after it (endless_weapon_post) */
static s32 launch_state;   /* (the home state put back after) */
static int launch_action;  /* (1 the rocket launched) */
/* THE WEAPON'S NAME ON A CHANGE: as a put-away ends the state machine waits for the next one (state 6) and has the
 * HUD show its name in a band at the bottom (D_8009EED0, 100 frames: func_800463C8), but not for the mission's watch
 * slot (its constant there: straight to state 5, the menu's watch shows no name). A lent weapon in that slot (King's
 * Ransom's only shared one) went unnamed: its name shown as any other weapon's (TWINE_TEST_NO_WATCH_NAME=1: not). */
extern s32 D_8009EED0;
extern s16 D_8009EEC4, D_800B0DD8;
static const u8 watch_slots[12];
static int watch_on;
static s32 band_from;
static void band_name(void) {
    int w = g_Args->level < 12 ? watch_slots[g_Args->level] : 0;
    if (band_from == 3 && D_800F11F0 == 5 && w && (D_800B0DDC == w || D_800B0DD8 == w) && (lent_mask >> w & 1) &&
        !D_8009CAA4 && !watch_on && D_8009EEC4 < 0 && D_8009EED0 <= 0 && !getenv("TWINE_TEST_NO_WATCH_NAME")) {
        D_8009EED0 = 100;
        if (port_trace("select"))
            port_log("trace: L%d f%d select: the name band for the lent weapon in the watch slot %d\n", g_Args->level,
                     D_8009CDA8, w);
    }
}

int endless_weapon_pre(void) {
    int k;
    u8 *inst, *def;
    s32 a, running, done, elapsed;
    band_from = D_800F11F0;
    launch_state = 0, launch_action = 0;
    if (D_800F11F0 == 0xF && !flash_native() && D_8009CD98 && lent_flash() &&
        (inst = *(u8 **)(D_8009CD98 + 0x114))) { /* (the lent flash bang's state 15) */
        def = (u8 *)&g_Weapons[D_800B0DD4];
        a = (s8)def[0x7C];
        running = *(s32 *)(inst + 0x78) == a;
        flash_starting = !running;
        flash_done = running && *(s32 *)(inst + 0x80) < 2;
        launch_state = 0xF;
        D_800F11F0 = 8;
        return 1;
    }
    if ((D_800F11F0 != 0x16 && D_800F11F0 != 0x26) || !D_8009CD98 || (k = lent_launcher()) < 0)
        return 0;
    if ((k == 3) != (D_800F11F0 == 0x16))
        return 0;
    inst = *(u8 **)(D_8009CD98 + 0x114);
    def = (u8 *)&g_Weapons[D_800B0DD4];
    if (!inst)
        return 0;
    a = (s8)def[0x7C];
    running = *(s32 *)(inst + 0x78) == a;
    done = *(s32 *)(inst + 0x80) < 2;
    elapsed = running ? (*(u8 **)(inst + 0x74))[a] - *(s32 *)(inst + 0x80) : -1;
    if (k == 3 && running && !done && elapsed == 5)
        launch_action = 1;
    if (k == 2) {
        if (!running)
            launch_action = 2; /* (its sound as the animation starts) */
        else if (done)
            launch_action = 3; /* (the next round to hand) */
        else if (elapsed == 1) { /* (the round launched: here, before the state machine, as at home inside it before
                                  * its last step moves the model on) */
            PushMatrix();
            gl_launch();
            PopMatrix();
        }
    }
    launch_state = D_800F11F0;
    D_800F11F0 = 8;
    return 1;
}

static void rocket_trail(void);
void endless_weapon_post(int token) {
    s32 home = launch_state;
    if (!token && flash_native() && !flash_vanilla() && D_800F11F0 == 0xF) { /* (at home, in a run: twice as long) */
        s16 e[5];
        if (port_endless_room(e))
            flash_fade(native_flash_count() - 1);
    }
    if (token && home == 0x16) /* (the rocket's first puff, made as it's launched: given its texture before it's drawn) */
        rocket_trail();
    band_name();
    if (!token || !home)
        return;
    launch_state = 0;
    if (home == 0xF) {
        if (D_800F11F0 == 8 || flash_done) {
            D_800F11F0 = 0xF;
            flash_state_post();
        } else if (flash_trace()) /* (the state machine moved on: a change, the menu) */
            port_log("trace: L%d f%d flash: state 15 left: %d\n", g_Args->level, D_8009CDA8, D_800F11F0);
        return;
    }
    if (launch_action == 3 && (D_800F11F0 == 1 || D_800F11F0 == 8)) { /* (func_8007980C, as at home inside the state
                                                                       * machine: its last step has run already) */
        u8 *inst = *(u8 **)(D_8009CD98 + 0x114);
        D_800F11F0 = home;
        func_8007980C();
        if (inst && *(s32 *)(inst + 0x88) >= 0)
            *(s32 *)(inst + 0x78) = *(s32 *)(inst + 0x88), *(s32 *)(inst + 0x7C) = *(s32 *)(inst + 0x8C);
        if (launch_trace())
            port_log("trace: L%d f%d launch: state 0x%X ends: %d (the next to hand)\n", g_Args->level, D_8009CDA8, home,
                     D_800F11F0);
        return;
    }
    if (D_800F11F0 != 8) { /* (the state machine moved on: the animation done (state 1), a change, the menu) */
        if (launch_trace())
            port_log("trace: L%d f%d launch: state 0x%X ends: %d\n", g_Args->level, D_8009CDA8, home, D_800F11F0);
        return;
    }
    D_800F11F0 = home;
    if (launch_action == 1)
        lent_rocket_launch();
    if (launch_action == 2)
        tone_as_game(9, gl_fire_sfx, 0, 0);
}

/* the lent frag grenade and the quick throw's count kept as one */
static void frag_sync(void) {
    static s32 last = -1;
    s32 *q = quick_grenade_count ? quick_grenade_count() : 0;
    int i;
    for (i = 0; i < SLOTS && i < 16; i++)
        if ((lent_mask >> i & 1) && throw_kind((char *)g_Weapons[i].pad0) == 0 && q) {
            Weapon *w = &g_Weapons[i];
            if (last >= 0 && w->ammo != last)
                *q += w->ammo - last; /* (a box's, or the lent one thrown) */
            if (*q < 0)
                *q = 0;
            w->ammo = (s16)*q, w->clip = *q > 0, w->reserve = *q > 0 ? *q - 1 : 0;
            last = w->ammo;
            return;
        }
    last = -1;
}

/* More weapons than the mission has slots for them (King's Ransom, Turncoat and Meltdown have one): the lent ones
 * share the slots, a pool. The weapon buttons step through the pool's weapons in a shared slot before they step on
 * (pool_step); each keeps its own ammo (Bond's carry holds it while it isn't in a slot), and drops feed it there
 * too (func_8004CA5C). */
static struct {
    char name[24];
    const WeaponRec *r;
} pool[16];
static int pool_n;
static s8 slot_pool[16]; /* (a slot's pool weapon now, -1 none) */
/* SATCHELS AND THEIR DETONATOR, LENT, AS AT HOME. At home (King's Ransom, its slots 5 and 6) the detonator is Bond's
 * only while charges are out: a charge thrown, the game changes to the detonator; the detonator fired, back to the
 * charges (a slot with none left: the game changes on as from any empty weapon). Lent, the charges and the detonator
 * share the charges' slot: the detonator (pool entry det_pi, from the catalogue, never from the carry) goes into it
 * as a charge has been thrown, the charges back as it has fired (sat_switch: as the throw's or the detonation's
 * animation ends, the other loaded and drawn at once, state 5, as at home); while no charge is out the detonator is in no slot, out of the weapon buttons' round and the
 * inventory. */
static int sat_pi = -1, det_pi = -1, charges_out, sat_switch = -1;
static s32 sat_switch_at; /* (the detonator's: back to the charges 7 frames into its fire, as at home) */
static const char *detonator_charges(int slot) { /* (the lent detonator in that slot: its charges' name) */
    return slot >= 0 && slot < 16 && (lent_mask >> slot & 1) && det_pi >= 0 && slot_pool[slot] == det_pi && sat_pi >= 0
               ? pool[sat_pi].name : 0;
}
static s8 slot_idx[16];  /* (a lent slot's place among them: its sound, model file, animation numbers, messages) */
static u8 pool_round[16]; /* (a pool weapon in the weapon buttons' round: the inventory's toggle, flags bit 12) */

static int carry_entry(const char *name) {
    EndlessCarry *c = &port_endless_carry;
    int j;
    for (j = 0; j < c->weapons; j++)
        if (!strncmp(c->weapon[j].name, name, 23))
            return j;
    return -1;
}

/* THE PAUSE MENU'S WATCH. Opening the pause menu, the game changes to a slot of each mission's (a constant in its
 * weapon state machine's state 0x1D: levels 1-11 slots 5 11 8 8 9 11 12 10 11 9 3) whose model is Bond's watch (the
 * Motorola, MOTO.HMD: the menu is drawn on its face), and changes back as it closes. That slot is one of the tools'
 * slots a weapon is lent into (clean_slots: every mission's watch slot is one), so the menu showed the lent weapon
 * raised and no watch. While the menu has it, the slot is the watch again: its own definition (bar the name, owned
 * bits and ammo, which the inventory shows), model type entry (animations) and model file, put back from what the
 * first lend into it kept; the lent weapon's come back as the menu closes, once the watch's put-away animation is
 * asked (state 3, its weapon to change to after the menu taken: D_8009EEC4 -1). The one loaded is marked stale both
 * ways (the slot's model type number is the same), so the state machine loads the other as the put-away ends. */
static const u8 watch_slots[12] = {0, 5, 11, 8, 8, 9, 11, 12, 10, 11, 9, 3};
static u8 watch_own_def[0xB8], watch_own_te[0x70], watch_lent_def[0xB8], watch_lent_te[0x70];
static char *watch_own_path, *watch_lent_path;
static int watch_on;
#define WATCH_SLOT (g_Args->level < 12 ? watch_slots[g_Args->level] : 0)
extern s16 D_8009EEC4; /* the weapon to change to as the menu closes (-1 none) */
extern s32 D_800F11F0; /* the weapon's state */

static void watch_swap(int in) {
    int i = WATCH_SLOT;
    u8 *w = (u8 *)&g_Weapons[i], t = w[0x5B], *te = D_8009D0FC + t * 0x70;
    if (in) {
        memcpy(watch_lent_def, w, 0xB8), memcpy(watch_lent_te, te, 0x70), watch_lent_path = D_8009DB50[t];
        memcpy(w + 0x50, watch_own_def + 0x50, 0xB8 - 0x50);
        *(s32 *)(w + 0x38) = (*(s32 *)(watch_own_def + 0x38) & ~0x1800) | (*(s32 *)(watch_lent_def + 0x38) & 0x1800);
        memcpy(te, watch_own_te, 0x70), D_8009DB50[t] = watch_own_path;
    } else {
        s32 owned = *(s32 *)(w + 0x38) & 0x1800; /* (the inventory's toggle, meanwhile) */
        memcpy(w + 0x50, watch_lent_def + 0x50, 0xB8 - 0x50);
        *(s32 *)(w + 0x38) = (*(s32 *)(watch_lent_def + 0x38) & ~0x1800) | owned;
        memcpy(te, watch_lent_te, 0x70), D_8009DB50[t] = watch_lent_path;
    }
    w[0x5B] = t;
    if (D_800E2F58 == t)
        D_800E2F58 = 0;
    watch_on = in;
    if (port_trace("weapon"))
        port_log("trace: L%d f%d weapon: the watch slot %d %s (state %d, in hand %d, to %d, after the menu %d)\n",
                 g_Args->level, D_8009CDA8, i, in ? "is the watch (the menu's)" : "is the lent weapon's again",
                 D_800F11F0, D_800B0DD4, D_800B0DDC, D_8009EEC4);
}

/* each time the controller is read (menus too, whose frames the mission's frame counter doesn't count) */
static void watch_frame(void) {
    int i = WATCH_SLOT;
    if (!i || !(lent_mask >> i & 1))
        return;
    if (!watch_on && D_8009CAA4 && D_800B0DDC == i && D_8009EEC4 >= 0 && (D_800B0DD4 != i || D_800F11F0 == 5))
        watch_swap(1); /* (the menu raising the watch: once the lent weapon in that slot is put away) */
    else if (watch_on && D_8009EEC4 < 0 && (D_800F11F0 == 3 || D_800F11F0 == 5 || D_800B0DD4 != i))
        watch_swap(0); /* (the menu closing: the watch's put-away begun, its animation asked; the weapon after it
                        * loads as that ends, the one loaded being stale) */
}

/* pool weapon pi into lent slot i (its place k): the donor's definition, this mission's guns' fire, its own sound,
 * model and animations, its pickups' entry */
static void lend_into(int i, int pi, int k) {
    const WeaponRec *r = pool[pi].r;
    int p = pistol_slot();
    u8 *w = (u8 *)&g_Weapons[i], *pw = (u8 *)&g_Weapons[p], t = w[0x5B], *te;
    u32 pickup = *(u32 *)(w + 0x40);
    if (i == WATCH_SLOT && watch_on) /* (the menu's watch: the lent weapon's things back first) */
        watch_swap(0);
    if (i == WATCH_SLOT && !(lent_mask >> i & 1)) { /* (the watch's own, for the menu) */
        memcpy(watch_own_def, w, 0xB8), memcpy(watch_own_te, D_8009D0FC + t * 0x70, 0x70);
        watch_own_path = D_8009DB50[t];
    }
    if (slot_pool[i] >= 0) { /* (the one there before: its pickups' entry back to none) */
        const WeaponRec *o = pool[(int)slot_pool[i]].r;
        if (o->kind < 19 && D_8009D130[o->kind * 4] == i)
            D_8009D130[o->kind * 4] = -1;
    }
    memcpy(w, r->def, 0xB8);
    lent_gun_fire = *(u32 *)(pw + 0x3C);
    *(u32 *)(w + 0x3C) = shotgun_rec(r)               ? (u32)(long)lent_shotgun
                         : throw_kind(r->name) == 3  ? (u32)(long)lent_rocket_fire
                         : throw_kind(r->name) == 2  ? (u32)(long)lent_gl_fire
                         : throw_kind(r->name) == 1  ? (u32)(long)lent_flash_fire
                         : throw_kind(r->name) != -1 ? (u32)(long)lent_throw
                                                     : lent_gun_fire; /* (this mission's guns' fire) */
    *(u32 *)(w + 0x40) = pickup;
    memset(&lent_sound[k], 0, sizeof(lent_sound[k]));
    if (w[0x5A] != 0xFF && r->sfx[4] && LENT_SOUND + k < 0x80) { /* (its own sound: func_80026F30) */
        lent_sound[k].level = r->level;
        lent_sound[k].loop = r->def[0x38] >> 1 & 1; /* (an automatic's: a loop while it fires) */
        memcpy(lent_sound[k].sfx, r->sfx, 8);
        w[0x5A] = LENT_SOUND + k;
    }
    w[0x5B] = t;
    lent_mask |= 1 << i;
    *(s32 *)(w + 0x38) &= ~0x1800; /* (owned as Bond's carry says) */
    te = D_8009D0FC + t * 0x70;
    memcpy(te, r->type, 0x70);
    te[0] = t;
    te[0x2C] = (D_8009D0FC + pw[0x5B] * 0x70)[0x2C]; /* (this mission's bullets) */
    memcpy(lent_anim[k], r->anim, 16);
    *(u8 **)(te + 0x68) = lent_anim[k];
    *(u16 *)(te + 0x6C) = r->animlen;
    snprintf(lent_path[k], sizeof(lent_path[k]), "%s", r->path);
    D_8009DB50[t] = lent_path[k];
    if (D_800E2F58 == t) /* (the model loaded is of this slot's type number but another weapon's (the pool's weapon
                          * before it, or the mission's own): loaded afresh when the slot is drawn, func_80075FE4) */
        D_800E2F58 = 0;
    if (r->kind < 19 && D_8009D130[r->kind * 4] < 0) { /* (its ammo in boxes, its pickups' messages its own) */
        s32 *e = &D_8009D130[r->kind * 4];
        snprintf(lent_got[k], sizeof(lent_got[k]), "%s", r->got);
        snprintf(lent_ammo_got[k], sizeof(lent_ammo_got[k]), "%s", r->ammo_got);
        e[0] = i;
        e[1] = -1; /* (no gun of it lies about here: its model is its mission's) */
        e[2] = (s32)(long)lent_got[k];
        e[3] = (s32)(long)lent_ammo_got[k];
    }
    *(s32 *)(w + 0x38) = (*(s32 *)(w + 0x38) & ~0x1000) | pool_round[pi] << 12;
    if (pi == det_pi) { /* (Bond's while charges are out: owned, in the round, its one "round") */
        *(s32 *)(w + 0x38) |= 0x1800;
        ((Weapon *)w)->ammo = ((Weapon *)w)->clip = 1, ((Weapon *)w)->reserve = 0;
    }
    slot_pool[i] = (s8)pi, slot_idx[i] = (s8)k;
    if (port_trace("weapon")) /* (its own model: tools/endless_check.py's x checks each one drawn shows it) */
        port_log("trace: L%d f%d weapon: %s into lent slot %d, its model %s\n", g_Args->level, D_8009CDA8,
                 pool[pi].name, i, r->path);
}

/* a lent slot's weapon as Bond's carry has it (owned, its ammo) */
static void lent_ammo_in(int i) {
    EndlessCarry *c = &port_endless_carry;
    int j = slot_pool[i] >= 0 ? carry_entry(pool[(int)slot_pool[i]].name) : -1;
    Weapon *w = &g_Weapons[i];
    if (j < 0 || slot_pool[i] == det_pi) /* (the detonator's "round": always one, while it's his) */
        return;
    if (c->weapon[j].owned) /* (owned, and in the weapon buttons' round unless the inventory took it out) */
        w->flags = (w->flags & ~0x1000) | 0x800 | pool_round[(int)slot_pool[i]] << 12;
    w->ammo = c->weapon[j].ammo, w->clip = c->weapon[j].clip, w->reserve = c->weapon[j].reserve;
}

/* ... and back into the carry as it leaves the slot */
static void lent_ammo_out(int i) {
    EndlessCarry *c = &port_endless_carry;
    int j = slot_pool[i] >= 0 ? carry_entry(pool[(int)slot_pool[i]].name) : -1;
    Weapon *w = &g_Weapons[i];
    if (j < 0 || slot_pool[i] == det_pi) /* (the detonator's "round": always one, while it's his) */
        return;
    c->weapon[j].owned = (w->flags & 0x800) != 0;
    c->weapon[j].ammo = w->ammo, c->weapon[j].clip = w->clip, c->weapon[j].reserve = w->reserve;
    pool_round[(int)slot_pool[i]] = w->flags >> 12 & 1;
}

static int pool_shown(int pi) {
    int i;
    for (i = 0; i < 16; i++)
        if (slot_pool[i] == pi && (lent_mask >> i & 1))
            return 1;
    return 0;
}

static int pool_slot_for(int pi);
static void lend_weapons(void) {
    EndlessCarry *c = &port_endless_carry;
    int j, i, k = 0;
    memset(lent_sound, 0, sizeof(lent_sound));
    memset(slot_pool, -1, sizeof(slot_pool));
    pool_n = 0;
    user_out = 0;
    if (!c->valid || pistol_slot() < 0 || !D_8009D0FC)
        return;
    for (j = 0; j < c->weapons && pool_n < 16; j++) {
        const WeaponRec *r;
        int have = 0;
        if (!c->weapon[j].owned || !strncmp(c->weapon[j].name, "SATCHEL DET", 11)) /* (the detonator: with the charges) */
            continue;
        for (i = 0; i < SLOTS; i++)
            have |= !strncmp((char *)g_Weapons[i].pad0, c->weapon[j].name, 23);
        if (getenv("TWINE_ENDLESS_TEST_SELFLEND") && !strncmp(c->weapon[j].name, getenv("TWINE_ENDLESS_TEST_SELFLEND"), 23))
            r = port_weapon_find(c->weapon[j].name, -1);
        else if (have || !(r = port_weapon_find(c->weapon[j].name, g_Args->level)))
            continue;
        if (r && !strncmp(r->name, "SATCHEL CHARGE", 14) && !port_weapon_find("SATCHEL DETONATOR", -1))
            r = 0; /* (satchels only with their detonator) */
        if (!r || !(r->gun || shotgun_rec(r) || throw_kind(r->name) != -1) || !r->path[0]) {
            if (r)
                ELOG("weapons: %s can't be lent here yet (not a gun)", c->weapon[j].name);
            continue;
        }
        { /* the frag grenade, the rocket and grenade launchers: their home missions throw and fire them through states
           * of their own (King's Ransom's 0x18, City of Walkways' 0x16-0x17, Fallen Angel's 0x26-0x27: animation,
           * the moment the projectile leaves, the next one drawn), which other missions' weapon state machines
           * haven't: not lent until they are (tools/lent_matrix.py: home and lent differ) */
            int tk = throw_kind(r->name);
            if (tk == 0 && !getenv("TWINE_ENDLESS_TEST_LEND_ALL")) {
                ELOG("weapons: %s not lent (its home mission's own throw/fire states)", c->weapon[j].name);
                continue;
            }
        }
        if (!strncmp(c->weapon[j].name, c->in_hand, 23) && pool_n) { /* (the one in his hand first: a slot for it) */
            memmove(&pool[1], &pool[0], sizeof(pool[0]) * pool_n);
            i = 0;
        } else
            i = pool_n;
        snprintf(pool[i].name, sizeof(pool[i].name), "%s", c->weapon[j].name);
        pool[i].r = r;
        pool_n++;
    }
    sat_pi = det_pi = -1, charges_out = 0, sat_switch = -1;
    for (j = 0; j < pool_n; j++) /* (lent charges: their detonator, the catalogue's, beside them) */
        if (!strncmp(pool[j].name, "SATCHEL CHARGE", 14) && pool_n < 16) {
            const WeaponRec *d = port_weapon_find("SATCHEL DETONATOR", -1);
            if (d) {
                snprintf(pool[pool_n].name, sizeof(pool[pool_n].name), "%s", d->name);
                pool[pool_n++].r = d;
            }
            break;
        }
    for (j = 0; j + 1 < pool_n; j++) /* (the detonator next to the satchel charges: one press between them) */
        if (!strncmp(pool[j].name, "SATCHEL CHARGE", 14)) {
            for (i = 0; i < pool_n && strncmp(pool[i].name, "SATCHEL DET", 11); i++)
                ;
            if (i < pool_n && i != j + 1) {
                typeof(pool[0]) det = pool[i];
                if (i > j) {
                    memmove(&pool[j + 2], &pool[j + 1], sizeof(pool[0]) * (i - j - 1));
                    pool[j + 1] = det;
                } else {
                    memmove(&pool[i], &pool[i + 1], sizeof(pool[0]) * (j - i));
                    pool[j] = det; /* (the charges moved down one: the detonator after them) */
                }
            }
            break;
        }
    for (j = 0; j < pool_n; j++) { /* (in the weapon buttons' round as he left the last room: its checkmark; a weapon
                                    * new to him is in it, as a weapon picked up is) */
        int ce = carry_entry(pool[j].name);
        pool_round[j] = !(ce >= 0 && c->weapon[ce].unticked);
        if (!strncmp(pool[j].name, "SATCHEL CHARGE", 14))
            sat_pi = j;
        if (!strncmp(pool[j].name, "SATCHEL DET", 11))
            det_pi = j, pool_round[j] = 0; /* (no charge out yet) */
    }
    for (j = 0; j < pool_n; j++) {
        if (j == det_pi) /* (no slot of its own: the charges' once one is out) */
            continue;
        for (i = 0; i < SLOTS && i < 16; i++) /* (a tool's slot, one the mission's code leaves alone) */
            if ((getenv("TWINE_ENDLESS_TEST_LENDAT") ? i == atoi(getenv("TWINE_ENDLESS_TEST_LENDAT"))
                                                    : (clean_slots[g_Args->level] >> i & 1)) &&
                g_Weapons[i].pad0[0] >= 'A' && g_Weapons[i].pad0[0] <= 'Z' && !combat_item((char *)g_Weapons[i].pad0) &&
                !(lent_mask & 1 << i) && ((u8 *)&g_Weapons[i])[0x5B]) /* (and a model type of its own) */
                break;
        if (i == SLOTS || i == 16) {
            if (pool_slot_for(j) < 0) /* (no shared slot at all: nothing could ever bring it out; not offered) */
                ELOG("weapons: %s can't be lent here (no slot of this mission's works for a lent weapon)", pool[j].name);
            else
                ELOG("weapons: %s waits in the pool (no slot left: the weapon buttons step to it)", pool[j].name);
            continue;
        }
        lend_into(i, j, k++);
        ELOG("weapons: %s lent from level %d into slot %d (model %s, type %d)", pool[j].name, pool[j].r->level, i,
             pool[j].r->path, ((u8 *)&g_Weapons[i])[0x5B]);
    }
}

/* The weapon buttons (D_800DDE08 previous, D_800DDDA4 next, pressed this frame) stepping away from a lent slot while
 * the pool has another weapon that way not in a slot: that weapon into this slot instead, and the change to it */
extern s32 D_800DDE08, D_800DDDA4;
extern s16 D_800B0DD8; /* the weapon before a change */
extern s32 D_800DDEEC; /* fire held */
extern s32 D_800F11F0; /* the weapon's state */
/* A pool weapon going into the slot in hand: it goes in once the weapon there is put away (the state machine's load,
 * state 5): its put-away animation, sound and ammo are its own, then the new one's model is loaded (the slot's model
 * type number stays, so the one loaded is marked stale: func_80075FE4 says no, and the state machine loads) */
static int pool_pending_slot = -1, pool_pending_pi, pool_pending_put;
static int select_old(void) {
    static int v = -1;
    if (v < 0)
        v = getenv("TWINE_TEST_SELECT_OLD") && atoi(getenv("TWINE_TEST_SELECT_OLD"));
    return v;
}
static void pool_later(int c, int pi) {
    pool_pending_slot = c, pool_pending_pi = pi, pool_pending_put = 0;
    if (D_800E2F58 == ((u8 *)&g_Weapons[c])[0x5B])
        D_800E2F58 = 0;
}
static void pool_pending(void) {
    int c = pool_pending_slot;
    if (c < 0 || D_800F11F0 == 3 || D_800F11F0 == 6) /* (still putting it away) */
        return;
    if (c == D_800B0DD4 && D_800F11F0 != 5 && !pool_pending_put && D_8009CAA4 == 0) {
        /* (chosen in the inventory for the slot in hand: no change began as the menu closed, so it's put away now) */
        pool_pending_put = 1;
        D_800B0DD8 = D_800B0DD4, D_800B0DDC = (s16)c;
        func_8007A17C(3);
        return;
    }
    pool_pending_slot = -1;
    lent_ammo_out(c);
    lend_into(c, pool_pending_pi, slot_idx[c]);
    lent_ammo_in(c);
    if (getenv("TWINE_ENDLESS_AMMO_LOG"))
        ELOG("pool: %s in slot %d now (state %d)", pool[pool_pending_pi].name, c, D_800F11F0);
}

static void pool_step(void) {
    static s32 last_state;
    int c = D_800B0DD8, dir, pi;
    s32 st = D_800F11F0;
    pool_pending();
    if (sat_switch >= 0 && pool_pending_slot < 0 && D_8009CAA4 == 0) {
        if (D_800B0DD4 != sat_switch) /* (he changed weapons meanwhile) */
            sat_switch = -1;
        else if (sat_switch_at ? D_8009CDA8 >= sat_switch_at : st == 1) { /* (the throw's animation over; the
                                                                            * detonator's 7 frames in) */
            int to = slot_pool[sat_switch] == det_pi ? sat_pi : det_pi;
            /* (as the game does at home: no put-away, the other loaded and drawn at once, state 5) */
            lent_ammo_out(sat_switch);
            lend_into(sat_switch, to, slot_idx[sat_switch]);
            lent_ammo_in(sat_switch);
            D_800B0DD8 = D_800B0DDC = (s16)sat_switch;
            func_8007A17C(5);
#ifdef MOD_TRACE
            trace_change_by(to == det_pi ? "endless: a charge out, the detonator taken out (as at home)"
                                         : "endless: the charges set off, the charges taken out again (as at home)");
#endif
            ELOG("weapons: %s into slot %d (%s)", pool[to].name, sat_switch,
                 to == det_pi ? "a charge out, as at home" : "the charges set off, as at home");
            sat_switch = -1;
        }
    }
    if ((st == 3 || st == 6) && last_state != 3 && last_state != 6 && getenv("TWINE_ENDLESS_AMMO_LOG"))
        ELOG("pool: change from %d to %d (in hand %d), lent %d, pool %d, buttons %d %d, draw_slot %d", c, D_800B0DDC,
             D_800B0DD4, c >= 0 && c < 16 ? (int)(lent_mask >> c & 1) : -1, c >= 0 && c < 16 ? slot_pool[c] : -9,
             D_800DDE08, D_800DDDA4, draw_slot);
    /* (the old way, before the weapons Bond carries were one list: TWINE_TEST_SELECT_OLD=1, with weaponkeys.c's) */
    if ((st == 3 || st == 6) && last_state != 3 && last_state != 6 && c >= 0 && c < 16 && (lent_mask >> c & 1) &&
        slot_pool[c] >= 0 && D_800B0DDC != c && (D_800DDE08 || D_800DDDA4) && draw_slot < 0 && select_old()) {
        dir = D_800DDDA4 ? 1 : -1;
        pi = slot_pool[c] + dir; /* (the next one in a slot already: the game steps on to it) */
        while (pi >= 0 && pi < pool_n && !pool_round[pi] && !pool_shown(pi)) /* (one left out of the round) */
            pi += dir;
        if (pi >= 0 && pi < pool_n && !pool_shown(pi)) {
            pool_later(c, pi);
            D_800B0DDC = (s16)c;
#ifdef MOD_TRACE
            trace_change_by("endless: the weapon buttons stepping through the pool");
#endif
            ELOG("weapons: the pool's %s into slot %d (the weapon buttons step through the pool)", pool[pi].name, c);
        }
    }
    last_state = st;
}

/* THE WEAPONS BOND CARRIES, ONE LIST (port/mods/weaponkeys.c steps through it: the weapon buttons, the number keys, Q;
 * the inventory shows it). Each weapon he owns is an entry of its own, whether it is in a slot (the mission's own, or a
 * lent one in a shared slot) or waits in the pool out of one, and has a checkmark of its own (the inventory's toggle:
 * in the weapon buttons' round or not; kept for the run in Bond's carry). Taking a pool weapon puts it into a shared
 * slot first, one not in hand if there is one (the change then draws it, as the game draws any slot); into the slot in
 * hand it goes once the weapon there is put away (pool_later). Either way the game's own put-away and draw: the same
 * animations and sounds as any change. The lent detonator is his only while charges are out (as at home). */
static struct { s8 slot, pi; } car[64];
static int car_n;
static int car_owned_pool(int pi) { /* (a pool weapon of his, waiting out of a slot) */
    EndlessCarry *c = &port_endless_carry;
    int j;
    if (pool_shown(pi) || pool_slot_for(pi) < 0) /* (and only while a shared slot can take it: an entry the weapon
                                                   * buttons stop on and can't bring out would leave him stuck) */
        return 0;
    if (pi == det_pi)
        return charges_out > 0;
    return (j = carry_entry(pool[pi].name)) >= 0 && c->weapon[j].owned;
}
static int car_build(void) {
    s16 e5[5];
    int i;
    if (!port_endless_room(e5) || !g_Player || select_old())
        return -1;
    car_n = 0;
    for (i = 0; i < SLOTS && car_n < 64; i++)
        if (g_Weapons[i].pad0[0] >= 'A' && g_Weapons[i].pad0[0] <= 'Z' && (g_Weapons[i].flags & 0x800))
            car[car_n].slot = (s8)i, car[car_n++].pi = -1;
    for (i = 0; i < pool_n && car_n < 64; i++)
        if (car_owned_pool(i))
            car[car_n].slot = -1, car[car_n++].pi = (s8)i;
    return car_n;
}
/* how many (-1: not in a run, the game's own list of slots) */
int endless_carried(void) {
    return car_build();
}
const char *endless_carried_name(int e) {
    if (e < 0 || e >= car_n)
        return "";
    return car[e].slot >= 0 ? (const char *)g_Weapons[(int)car[e].slot].pad0 : pool[(int)car[e].pi].name;
}
int endless_carried_ticked(int e) { /* (its checkmark: in the weapon buttons' round) */
    if (e < 0 || e >= car_n)
        return 0;
    if (car[e].slot >= 0)
        return g_Weapons[(int)car[e].slot].flags >> 12 & 1;
    return car[e].pi == det_pi ? 1 : pool_round[(int)car[e].pi];
}
/* the weapon in hand, or the one a change is drawing next (a pool weapon going into the slot in hand included; the
 * room's carried weapon still to come out: that one, not the mission's own in the game's slot meanwhile, which isn't
 * shown) */
int endless_carried_current(void) {
    int i, s = draw_slot >= 0 && draw_slot < SLOTS ? draw_slot
             : D_800B0DDC >= 0 && D_800B0DDC < SLOTS ? D_800B0DDC : D_800B0DD4;
    for (i = 0; i < car_n; i++)
        if (pool_pending_slot >= 0 ? car[i].pi == pool_pending_pi && car[i].slot < 0 : car[i].slot == s)
            return i;
    return -1;
}
/* the shared slot a pool weapon goes into: a lent one, not the one in hand if another is, nor the lent detonator's
 * while charges are out (-1: none) */
static int pool_slot_for(int pi) {
    int i, best = -1, score = -1;
    (void)pi;
    for (i = 0; i < SLOTS && i < 16; i++)
        if ((lent_mask >> i & 1) && slot_pool[i] >= 0) {
            int sc = (i != D_800B0DD4) * 4 + (slot_pool[i] != det_pi || !charges_out) * 2 + (i != D_800B0DDC);
            if (sc > score)
                score = sc, best = i;
        }
    return best;
}
static void trace_select(const char *fmt, const char *a, int n) {
    if (port_trace("select"))
        port_log("trace: L%d f%d select: %s%s %d\n", g_Args->level, D_8009CDA8, fmt, a, n);
}
/* entry e to be the weapon changed to (the change itself begun by the caller, as the game's buttons begin one):
 * 1 a change to begin, 0 none (it is the one in hand) */
int endless_carried_take(int e, const char *why) {
    int s, pi;
    if (e < 0 || e >= car_n)
        return 0;
    if (car[e].slot >= 0) {
        s = car[e].slot;
        if (pool_pending_slot >= 0) { /* (a pool weapon was to go into the slot in hand: not now) */
            trace_select("a pool weapon's going in called off: ", pool[pool_pending_pi].name, pool_pending_slot);
            pool_pending_slot = -1;
        }
        D_800B0DDC = (s16)s;
        trace_select("to slot: ", (const char *)g_Weapons[s].pad0, s);
        return s != D_800B0DD4;
    }
    pi = car[e].pi;
    if ((s = pool_slot_for(pi)) < 0)
        return 0;
    if (s == D_800B0DD4) { /* (the slot in hand: in as the weapon there is put away) */
        pool_later(s, pi);
        D_800B0DDC = (s16)s;
        ELOG("weapons: the pool's %s into slot %d (%s; as the one in hand is put away)", pool[pi].name, s, why);
    } else {
        if (pool_pending_slot >= 0)
            pool_pending_slot = -1;
        lent_ammo_out(s);
        lend_into(s, pi, slot_idx[s]);
        lent_ammo_in(s);
        D_800B0DDC = (s16)s;
        ELOG("weapons: the pool's %s into slot %d (%s)", pool[pi].name, s, why);
    }
    trace_select("a pool weapon into a shared slot: ", pool[pi].name, s);
    return 1;
}
/* Q's weapon (the one held before), kept in the carry for the run */
const char *endless_carried_before(void) {
    return port_endless_carry.before;
}
void endless_carried_set_before(const char *name) {
    snprintf(port_endless_carry.before, sizeof(port_endless_carry.before), "%s", name);
}
/* the room's carried weapon still to be drawn (Bond's hand not his own yet) */
int endless_room_drawing(void) {
    return draw_slot >= 0 || draw_direct || draw_hidden;
}
static void test_ticks(const char *t) {
    int i, n = car_build();
    if (atoi(t) != g_Args->level || !(t = strchr(t, ':')))
        return;
    for (i = 0; i < n; i++) {
        const char *nm = endless_carried_name(i), *p = t + 1;
        int want = 0;
        while (*p) {
            int k = 0;
            while (p[k] && p[k] != '|' && nm[k] == p[k])
                k++;
            if (!p[k] || p[k] == '|')
                want = 1;
            while (*p && *p != '|')
                p++;
            if (*p)
                p++;
        }
        if (car[i].slot >= 0) {
            g_Weapons[(int)car[i].slot].flags = (g_Weapons[(int)car[i].slot].flags & ~0x1000) | want << 12;
            user_out = (user_out & ~(1u << car[i].slot)) | (u32)!want << car[i].slot;
        }
        else if (car[i].pi != det_pi)
            pool_round[(int)car[i].pi] = (u8)want;
        ELOG("weapons: %s %s the weapon buttons' round (test)", nm, want ? "in" : "out of");
    }
}
/* (Bond leaving: the lent weapons waiting out of a slot keep their checkmarks) */
static void carry_ticks(void) {
    EndlessCarry *c = &port_endless_carry;
    int pi, j;
    for (pi = 0; pi < pool_n; pi++)
        if (pi != det_pi && !pool_shown(pi) && (j = carry_entry(pool[pi].name)) >= 0 && c->weapon[j].owned)
            c->weapon[j].unticked = !pool_round[pi];
}

/* The pause menu's inventory (func_80042E6C draws it, func_80043078 runs it) in a run: the pool's weapons waiting
 * out of a slot are listed too, after the slots', and the list scrolls when it's longer than the page (rows from
 * y 58, 12 apart, above the hint at 195). Choosing a pool weapon lends it into a shared slot (one not in hand if there
 * is one) and changes to it, as choosing a slot's does; its toggle (left out of the weapon buttons' round) is kept for
 * it while it waits. */
M2C_UNK func_8002872C(M2C_UNK *text, M2C_UNK x, M2C_UNK y); /* a menu item: its text at x, y */
void func_80028B58(s32 item, s32 a, s32 b);                 /* an item's colours */
M2C_UNK Menu_ResetState();
M2C_UNK func_8007F0D8(M2C_UNK);
s16 func_80043B14(s16 first, s16 count);                    /* the menu's buttons: 1 back, 2 moved, 3 chosen */
void PauseMenu_Open(void);
void func_800414AC(void);                                   /* the menu closed, the game on */
void func_80042E6C_orig(void);
void func_80043078_orig(void);
extern int D_800AC54C;               /* the next item */
extern u16 D_800AC4D8, D_800AC4DC;   /* an item's colours */
extern s16 D_800AEDA0, D_800AEDA4;   /* the chosen one's */
extern s16 D_800B0794, D_800B0798;   /* the cursor's item, before it moved */
extern s16 D_8009CFBE;               /* the items to move through */
extern s16 D_8009EEC4;               /* the weapon to change to as the menu closes (-1 none) */
extern char D_800A83BC[];            /* the title */
static char inv_in[] = "*", inv_out[] = "=", inv_hint[] = "\\ TOGGLE"; /* (every mission's: the toggle's marks, its hint) */
extern M2C_UNK D_800A8410, D_800A8418; /* the footer's two */
#define INV_ROWS 11
static s16 inv_entry[48]; /* (a row: a slot, or -1 - a pool weapon) */
static int inv_n, inv_top;

static int menu_sound(int n) { /* (the menu's sounds: levels 4 and 10 number them one on) */
    return n + (g_Args->level == 4 || g_Args->level == 10);
}

static int inv_round(int e) { /* (in the weapon buttons' round: flags bit 12, shown with the first mark) */
    return e >= 0 ? (int)(g_Weapons[e].flags >> 12 & 1) : pool_round[-1 - e];
}

int weaponkeys_rank(const char *name) __attribute__((weak)); /* (the weapons' order: weaponkeys.c's groups) */
static void inv_draw(void) {
    int i, j, v;
    s32 rank[48];
    inv_n = 0;
    car_build(); /* (every weapon he carries, in a slot or not, in the order the weapon buttons step through them) */
    for (i = 0; i < car_n && inv_n < 48; i++) {
        s16 en = car[i].slot >= 0 ? car[i].slot : (s16)(-1 - car[i].pi);
        s32 r = (weaponkeys_rank ? weaponkeys_rank(endless_carried_name(i)) : 0) * 64 + i;
        for (j = inv_n; j > 0 && rank[j - 1] > r; j--)
            inv_entry[j] = inv_entry[j - 1], rank[j] = rank[j - 1];
        inv_entry[j] = en, rank[j] = r;
        inv_n++;
    }
    v = inv_n < INV_ROWS ? inv_n : INV_ROWS;
    if (inv_top > inv_n - v)
        inv_top = inv_n - v;
    if (inv_top < 0)
        inv_top = 0;
    for (i = 0; i < v; i++) {
        int e = inv_entry[inv_top + i];
        func_8002872C(e >= 0 ? (M2C_UNK *)&g_Weapons[e] : (M2C_UNK *)pool[-1 - e].name, 0x64, 0x3A + 0xC * i);
        func_80028B58(D_800AC54C - 1, (s16)D_800AC4D8, (s16)D_800AC4DC);
    }
    for (i = 0; i < v; i++) {
        func_8002872C((M2C_UNK *)(inv_round(inv_entry[inv_top + i]) ? inv_in : inv_out), 0x50, 0x3A + 0xC * i);
        func_80028B58(D_800AC54C - 1, (s16)D_800AC4D8, (s16)D_800AC4DC);
    }
    D_8009CFBE = (s16)v;
    if (D_800B0794 >= v)
        D_800B0794 = (s16)(v ? v - 1 : 0);
    func_80028B58(D_800B0794, D_800AEDA0, D_800AEDA4);
    func_80028B58(D_800B0794 + v, D_800AEDA0, D_800AEDA4);
    func_8002872C((M2C_UNK *)D_800A83BC, 0x32, 0x2E);
    func_8002872C(&D_800A8410, 0x19, 0xD7);
    func_8002872C(&D_800A8418, 0x19A, 0xD7);
    func_8002872C((M2C_UNK *)inv_hint, 0xC8, 0xC3);
    if (getenv("TWINE_ENDLESS_AMMO_LOG")) {
        char b[160];
        int o = 0;
        for (i = 0; i < v && o < 140; i++)
            o += snprintf(b + o, sizeof(b) - o, "%s%s", i ? ", " : "",
                          inv_entry[inv_top + i] >= 0 ? (char *)g_Weapons[inv_entry[inv_top + i]].pad0 : pool[-1 - inv_entry[inv_top + i]].name);
        ELOG("inventory: rows %d-%d of %d, cursor %d: %s", inv_top, inv_top + v - 1, inv_n, inv_top + D_800B0794, b);
    }
}

void func_80042E6C(void) {
    s16 e[5];
    if (!port_endless_room(e) || !g_Player || (select_old() && !pool_n)) {
        func_80042E6C_orig();
        return;
    }
    if (D_800B0794 == 0)
        inv_top = 0; /* (the menu just opened: func_80042E34 put the cursor on the first) */
    inv_draw();
}

void func_80043078(void) {
    s16 e[5];
    s32 sel;
    int v, at;
    if (!port_endless_room(e) || !g_Player || (select_old() && !pool_n)) {
        func_80043078_orig();
        return;
    }
    v = D_8009CFBE;
    sel = func_80043B14(0, (s16)v);
    at = inv_top + D_800B0794;
    if (sel == 1) {
        func_80026F30(menu_sound(1), 0);
        PauseMenu_Open();
    } else if (sel == 2) {
        if (inv_n > v && D_800B0798 == v - 1 && D_800B0794 == 0) { /* (down past the page's last: scroll) */
            if (inv_top + v < inv_n)
                inv_top++, D_800B0794 = (s16)(v - 1);
            else
                inv_top = 0;
            Menu_ResetState();
            inv_draw();
        } else if (inv_n > v && D_800B0798 == 0 && D_800B0794 == v - 1) { /* (up past its first) */
            if (inv_top > 0)
                inv_top--, D_800B0794 = 0;
            else
                inv_top = inv_n - v;
            Menu_ResetState();
            inv_draw();
        } else {
            func_80028B58(D_800B0798 + v, (s16)D_800AC4D8, (s16)D_800AC4DC);
            func_80028B58(D_800B0794 + v, D_800AEDA0, D_800AEDA4);
        }
    } else if (sel == 3 && at < inv_n) {
        int en = inv_entry[at], slot = en;
        func_80026F30(menu_sound(1), 0);
        if (en < 0) { /* a pool weapon: into a shared slot, not the one in hand if another is lent */
            int pi = -1 - en;
            if ((slot = pool_slot_for(pi)) < 0)
                return;
            if (slot == D_800B0DD4) /* (the one in hand: in as it's put away) */
                pool_later(slot, pi);
            else {
                lent_ammo_out(slot);
                lend_into(slot, pi, slot_idx[slot]);
                lent_ammo_in(slot);
            }
            ELOG("weapons: the pool's %s into slot %d (chosen in the inventory)", pool[pi].name, slot);
        }
        D_8009EEC4 = (s16)slot;
        func_800414AC();
    } else if (g_PadPressed & 0x80 && at < inv_n) { /* (Triangle: in or out of the weapon buttons' round) */
        int en = inv_entry[at];
        func_80026F30(menu_sound(2), 0);
        func_8007F0D8(0);
        if (en >= 0) {
            g_Weapons[en].flags ^= 0x1000;
            user_out = (user_out & ~(1u << en)) | (u32)!(g_Weapons[en].flags & 0x1000) << en;
        } else if (-1 - en != det_pi) /* (the lent detonator: his, in the round, while charges are out) */
            pool_round[-1 - en] ^= 1;
        if (port_trace("select"))
            port_log("trace: L%d select: inventory: %s %s the weapon buttons' round\n", g_Args->level,
                     en >= 0 ? (char *)g_Weapons[en].pad0 : pool[-1 - en].name, inv_round(en) ? "into" : "out of");
        Menu_ResetState();
        inv_draw();
    }
}

/* a box of a pool weapon's ammo while it isn't in a slot: the ammo to Bond's carry, as the game gives a box's
 * (func_8004CB2C's count, the pickup's message and sound) */
extern s16 D_8009D124; /* the pickup's sound */
static int pool_box(u8 *o) {
    EndlessCarry *c = &port_endless_carry;
    int pi, j, n, clip, cap;
    s32 kind;
    static char msg[48];
    if (o[6] != (u8)D_8009D108 || !pool_n)
        return 0;
    kind = *(s32 *)(o + 0x34);
    for (pi = 0; pi < pool_n && pool[pi].r->kind != kind; pi++)
        ;
    if (pi == pool_n || pool_shown(pi) || (j = carry_entry(pool[pi].name)) < 0)
        return 0;
    clip = pool[pi].r->def[0x50], cap = *(s16 *)(pool[pi].r->def + 0x5C);
    if (cap > 0 && c->weapon[j].reserve >= cap)
        return 0;
    n = clip < 2 ? 1 : *(s32 *)(o + 0x60) ? clip : (s32)(rnd() % (clip >> 1)) + 1 + (clip >> 2);
    if (clip >= 2 && !(clip & 1) && (n & 1))
        n++;
    c->weapon[j].reserve += n, c->weapon[j].ammo += n;
    if (cap > 0 && c->weapon[j].reserve > cap)
        c->weapon[j].ammo -= c->weapon[j].reserve - cap, c->weapon[j].reserve = cap;
    snprintf(msg, sizeof(msg), pool[pi].r->ammo_got[0] ? pool[pi].r->ammo_got : "GOT %d ROUNDS", n);
    func_80048B4C(msg, 0x80, 0);
    func_80026F30(D_8009D124, 0);
    if (*(s32 **)(o + 0x60)) {
        **(s32 **)(o + 0x60) = 0;
        *(s32 **)(o + 0x60) = 0;
    }
    func_800395E8(o);
    ELOG("pickup: %s's ammo +%d (a lent weapon waiting in the pool)", pool[pi].name, n);
    return 1;
}

/* AMMO TO THE GUN THAT NEEDS IT. In a run an ammo pickup (a box, whatever kind it holds, or a gun of his own lying
 * about, a guard's drop: the game takes that as its ammo) isn't wasted on a full weapon: the ammo goes to the first of
 *   1. the weapon in his hand,
 *   2. the weapons checked in the weapon list (in the weapon buttons' round), the emptiest first (reserve / its most),
 *   3. any other weapon he carries (his own or lent, in a slot or waiting in the pool), the emptiest first,
 * that isn't full and fires bullets or shells; "AMMO FULL" (the pickup left lying) only when none can take it. How
 * much: the game's count for the weapon that takes it (func_8004CB2C: a clip's worth from a gun linked to a guard,
 * otherwise a quarter clip plus 1 to half a clip, made even for an even clip; 1 for a clip of one), capped at its
 * most, into its reserve (func_8004C50C, as a box gives); the message its own pickups' ("GOT %d ... ROUNDS").
 * Explosives' pickups (grenades, satchels, rockets, launcher rounds, flash bangs), the stunner's and armour: the
 * game's own, as before. */
static int explosive(const char *nm);
static int bullet_gun(const char *nm) {
    return combat_item(nm) && !explosive(nm) && strncmp(nm, "UNARMED", 7) && strncmp(nm, "CELL PHONE", 10) &&
           strncmp(nm, "SATCHEL", 7);
}
typedef struct {
    int slot, pi, j, ticked, hand;
    s32 have, cap, clip;
    const char *name;
} AmmoTaker;
static int ammo_takers(AmmoTaker *t) {
    EndlessCarry *c = &port_endless_carry;
    int i, n = 0;
    for (i = 0; i < SLOTS && n < 48; i++) {
        Weapon *w = &g_Weapons[i];
        const char *nm = (const char *)w->pad0;
        if (nm[0] < 'A' || nm[0] > 'Z' || !(w->flags & 0x800) || !bullet_gun(nm) || *(s16 *)&w->maxAmmo <= 0 ||
            (i == WATCH_SLOT && watch_on) || ((lent_mask >> i & 1) && slot_pool[i] >= 0 && slot_pool[i] == det_pi))
            continue;
        t[n].slot = i, t[n].pi = -1, t[n].j = -1, t[n].name = nm;
        t[n].ticked = w->flags >> 12 & 1, t[n].hand = i == D_800B0DD4;
        t[n].have = w->reserve, t[n].cap = *(s16 *)&w->maxAmmo, t[n].clip = w->clipSize;
        n++;
    }
    for (i = 0; i < pool_n && n < 48; i++) {
        const WeaponRec *r = pool[i].r;
        int j;
        if (pool_shown(i) || i == det_pi || (j = carry_entry(pool[i].name)) < 0 || !c->weapon[j].owned ||
            !bullet_gun(pool[i].name) || *(s16 *)(r->def + 0x5C) <= 0)
            continue;
        t[n].slot = -1, t[n].pi = i, t[n].j = j, t[n].name = pool[i].name;
        t[n].ticked = pool_round[i], t[n].hand = 0;
        t[n].have = c->weapon[j].reserve, t[n].cap = *(s16 *)(r->def + 0x5C), t[n].clip = r->def[0x50];
        n++;
    }
    return n;
}
/* (its rank: the hand first, then the checked, then the rest; within each the emptiest) */
static int ammo_before(const AmmoTaker *a, const AmmoTaker *b) {
    int ga = a->hand ? 0 : a->ticked ? 1 : 2, gb = b->hand ? 0 : b->ticked ? 1 : 2;
    if (ga != gb)
        return ga < gb;
    return (long long)a->have * b->cap < (long long)b->have * a->cap;
}
/* the pickups' message for a weapon's ammo ("GOT %d KS5 ROUNDS"): its entry's in D_8009D130, or the pool's record's */
static const char *ammo_msg(const AmmoTaker *t) {
    int i;
    if (t->slot >= 0) {
        for (i = 0; i < 19; i++)
            if (D_8009D130[i * 4] == t->slot && D_8009D130[i * 4 + 3])
                return (const char *)(long)D_8009D130[i * 4 + 3];
        if ((lent_mask >> t->slot & 1) && slot_pool[t->slot] >= 0 && pool[(int)slot_pool[t->slot]].r->ammo_got[0])
            return pool[(int)slot_pool[t->slot]].r->ammo_got;
    } else if (pool[t->pi].r->ammo_got[0])
        return pool[t->pi].r->ammo_got;
    return "GOT %d ROUNDS";
}
/* 1: taken (the ammo given, or it isn't an ammo pickup the rule has: 0, the game's own) */
static int ammo_pickup(u8 *o) {
    s16 *types = &D_8009D108; /* (the ammo box, a second box, the first entry's ammo: D_8009D108, 10A, 10C) */
    AmmoTaker t[48];
    const char *src = 0, *what;
    int i, n, best = -1, kind = -1, gun = 0;
    s32 amount, give, before;
    char line[400];
    int len = 0;
    static char msg[48], full[] = "AMMO FULL";
    static u8 *full_o;
    static s32 full_at;
    if ((types[0] > 0 && o[6] == (u8)types[0]) || (types[1] > 0 && o[6] == (u8)types[1]))
        kind = *(s32 *)(o + 0x34);
    else if (types[2] > 0 && o[6] == (u8)types[2])
        kind = 0;
    else
        for (i = 0; i < 19; i++) /* (a gun lying about: one he owns is its ammo, as the game takes it) */
            if (D_8009D130[i * 4 + 1] == o[6] && D_8009D130[i * 4] >= 0 && D_8009D130[i * 4] < SLOTS) {
                if (!(g_Weapons[D_8009D130[i * 4]].flags & 0x800))
                    return 0; /* (a gun new to him: the game gives it) */
                kind = i, gun = 1;
                break;
            }
    if (kind < 0 || kind >= 19)
        return 0;
    if (D_8009D130[kind * 4] >= 0 && D_8009D130[kind * 4] < SLOTS)
        src = (const char *)g_Weapons[D_8009D130[kind * 4]].pad0;
    else
        for (i = 0; i < pool_n && !src; i++)
            if (pool[i].r->kind == kind)
                src = pool[i].name;
    if (!src || !bullet_gun(src)) { /* (explosives', the stunner's: the game's own, or the pool's) */
        if (src && (full_o != o || D_8009CDA8 - full_at > SEC * 2))
            ELOG("ammo pickup: %s (%s's ammo): not bullets or shells, the game's own", gun ? "a gun of his lying about"
                                                                                        : "an ammo box", src);
        full_o = o, full_at = D_8009CDA8;
        return 0;
    }
    what = gun ? "a gun of his lying about" : "an ammo box";
    n = ammo_takers(t);
    for (i = 0; i < n; i++)
        if (t[i].have < t[i].cap && (best < 0 || ammo_before(&t[i], &t[best])))
            best = i;
    for (i = 0; i < n && len < (int)sizeof(line) - 60; i++)
        len += snprintf(line + len, sizeof(line) - len, "%s%s%s %d/%d%s", i ? ", " : "", t[i].name,
                        t[i].hand ? " (in hand)" : t[i].ticked ? " (checked)" : "", t[i].have, t[i].cap,
                        t[i].have >= t[i].cap ? " full" : "");
    if (best < 0) {
        func_80048B4C(full, 0x80, 0);
        if (full_o != o || D_8009CDA8 - full_at > SEC * 2) /* (once, not every frame he stands on it) */
            ELOG("ammo pickup: %s (%s's ammo): every weapon full (%s): AMMO FULL", what, src, n ? line : "none");
        full_o = o, full_at = D_8009CDA8;
        return 2;
    }
    {
        s32 clip = t[best].clip;
        amount = clip < 2 ? 1 : *(s32 *)(o + 0x60) ? clip : (s32)(rnd() % (clip >> 1)) + 1 + (clip >> 2);
        if (clip >= 2 && !(clip & 1) && (amount & 1))
            amount++;
    }
    give = amount < t[best].cap - t[best].have ? amount : t[best].cap - t[best].have;
    if (t[best].slot >= 0) {
        Weapon *w = &g_Weapons[t[best].slot];
        before = w->ammo;
        w->reserve += give;
        w->ammo = w->clip + w->reserve;
        ELOG("pickup: %s's ammo %d -> %d%s", t[best].name, before, w->ammo,
             lent_mask >> t[best].slot & 1 ? " (a lent weapon)" : "");
    } else {
        EndlessCarry *c = &port_endless_carry;
        before = c->weapon[t[best].j].ammo;
        c->weapon[t[best].j].reserve += give, c->weapon[t[best].j].ammo += give;
        ELOG("pickup: %s's ammo %d -> %d (a lent weapon waiting in the pool)", t[best].name, before,
             c->weapon[t[best].j].ammo);
    }
    ELOG("ammo pickup: %s (%s's ammo) -> %s (%s): +%d (its count %d, clip %d; reserve %d -> %d of %d); weapons: %s",
         what, src, t[best].name, t[best].hand ? "in hand" : t[best].ticked ? "checked, the emptiest" : "carried, the emptiest",
         give, amount, t[best].clip, t[best].have, t[best].have + give, t[best].cap, line);
    snprintf(msg, sizeof(msg), ammo_msg(&t[best]), give);
    func_80048B4C(msg, 0x80, 0);
    func_80026F30(D_8009D124, 0);
    if (*(s32 **)(o + 0x60)) {
        **(s32 **)(o + 0x60) = 0;
        *(s32 **)(o + 0x60) = 0;
    }
    func_800395E8(o);
    return 1;
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
        if (c->weapon[j].owned && strncmp((char *)w->pad0, "SATCHEL DET", 11)) /* (a mission's own detonator: its
                                                                                * code makes it his as a charge is set) */
            w->flags = (w->flags & ~0x1000) | 0x800 | (!c->weapon[j].unticked) << 12; /* (owned, and in the weapon
                                                                       * buttons' round as he left it: the checkmark) */
        if (c->weapon[j].owned && c->weapon[j].unticked && i < 32)
            user_out |= 1u << i;
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
void trace_change_by(const char *who);
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
            if (w->flags & 0x800) { /* (owned: in the round or not) */
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
    return floor_query(D_800E2F54, at, 0, 0);
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

/* The AI scripts whose people a run leaves out (bits by the script's index: the AIB tables, guard_scripts below):
 * M (Flashpoint's control room, Fallen Angel's: the story's, and her harm fails the mission) and BONDNIS, Bond's
 * stand-in for the missions' cutscenes (unhittable, hit mask 7FFF, health 1: Fallen Angel's 9:1 has one by its
 * entry; none of the cutscenes play in a run) */
static const u8 out_scripts[12] = {0,
                                   1 << 4,            /* L01 BONDNIS */
                                   1 << 7,            /* L02 BONDNIS */
                                   0,
                                   1 << 6,            /* L04 BONDNIS */
                                   0,
                                   1 << 3 | 1 << 4,   /* L06 M, BONDNIS */
                                   1 << 4,            /* L07 BONDNIS */
                                   1 << 7,            /* L08 BONDNIS */
                                   1 << 4 | 1 << 5,   /* L09 M, BONDNIS */
                                   1 << 4,            /* L10 BONDNIS */
                                   0};
#define OUT_SCRIPT(s) (g_Args->level >= 1 && g_Args->level <= 11 && (out_scripts[g_Args->level] >> ((s) & 7) & 1))
#define M_SCRIPT(w4) OUT_SCRIPT((w4) >> 20)

static int story_person(u32 t, s32 *rec) { /* (a placed person who would break the run: an unkillable one who
                                            * fights, M, Bond's stand-in; unkillable civilians, the bank's
                                            * receptionist, stay) */
    return (unkillable_type(t) && TYPE_DEF(t)[0x2C] != 0) || (rec && t && TYPE_DEF(t)[1] == 6 && M_SCRIPT((u32)rec[4]));
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
        } else if (o != g_Player && o[4] && o[5] == 6 && OUT_SCRIPT(o[0xA7]) && !is_copy(o)) {
            take_out(o, "M or Bond's cutscene stand-in, as the room starts");
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
        if (o != g_Player && o[4] && !(*(s32 *)(o + 0x4C) & 0x02000000) && /* (one left out, inactive: no one) */
            dist2(POS(o)[0], POS(o)[2], entry[2], entry[4]) < 900)
            who[n][0] = POS(o)[0], who[n][2] = POS(o)[2], n++;
    }
    for (i = 0; i < D_8009CF68; i++)
        for (j = 0; j < D_8009CF6C[i] && n < 8; j++) {
            s32 *p = D_8009CF70[i] + j * 6, x = p[0] + D_800D2FB0[0], z = p[2] + D_800D2FB0[2];
            if ((p[3] & 0xFF) && !(p[3] & 0x500) && /* (not posed, nor made already or left out: those are above) */
                TYPE_DEF(p[3] & 0xFF)[1] == 6 && dist2(x, z, entry[2], entry[4]) < 900)
                who[n][0] = x, who[n][2] = z, n++;
        }
    if (!n)
        return;
    {   /* (places round the arrival, nearest first: the middle of a polygon of the floor Bond can walk to from it, at
         * its height, with floor all round it (no wall, ledge or furniture's edge by it), away from them all. A place
         * merely over some floor (the 4:-1 playtest's) can be behind a wall or in a corner he can't leave) */
        s32 fy = floor_at(entry[2], entry[3] - 500, entry[4]), d;
        int f;
        fy = fy == -0x7FFF ? entry[3] : fy;
        flood();
        for (f = 0; f < faces; f++) {
            if (!reach[f] || shut[f] || mid[f][1] - fy > 150 || fy - mid[f][1] > 150)
                continue;
            d = dist2(mid[f][0], mid[f][2], entry[2], entry[4]);
            if (d < 1000 || d > 3000 || d >= best)
                continue;
            for (k = 0; k < n && dist2(mid[f][0], mid[f][2], who[k][0], who[k][2]) >= 1300; k++)
                ;
            if (k < n || !clear(f))
                continue;
            b = f, best = d, mid_x = mid[f][0], mid_y = floor_at(mid[f][0], mid[f][1] - 500, mid[f][2]), mid_z = mid[f][2];
            if (mid_y == -0x7FFF)
                mid_y = mid[f][1];
        }
    }
    if (b < 0) {
        ELOG("room: someone at Bond's arrival, and no clear place near it");
        return;
    }
    {
        s16 at[5];
        at[0] = D_800D2F8C, at[1] = entry[1], at[2] = mid_x, at[3] = mid_y, at[4] = mid_z;
        bond_put(at);
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
/* A room whose kinds of guards have no slot of their own in its section (King's Ransom's bar: its slots bound to its
 * three posted guards' type, the bar's two civilians' and the story person's left out of the room; its kinds, 35
 * and 37, none: nobody ever came): a slot of the section's that nobody of the room can need any more (free, its
 * type no kind's, nobody of it about and no record of it still to be made) is made one of a kind's, wearing a
 * guard's model (people.c). Each kind with none gets one, while such slots last. */
static int slot_unneeded(int s) { /* (free, its type no kind's nor a fighter's, nobody of it about, none to be made) */
    u32 t = people_record(s)[0x1C];
    int i, j;
    if (*people_busy(s) != 0 || fighter_type(t, 0))
        return 0;
    for (i = 0; i < kind_count; i++)
        if ((kinds[i][3] & 0xFF) == t)
            return 0;
    for (i = D_800B0DA0; i <= D_800B07B0; i++)
        if (OBJ(i) != g_Player && OBJ(i)[4] && OBJ(i)[6] == t)
            return 0;
    for (i = 0; i < D_8009CF68; i++)
        for (j = 0; j < D_8009CF6C[i]; j++) {
            s32 *p = D_8009CF70[i] + j * 6;
            if ((u32)(p[3] & 0xFF) == t && !(p[3] & 0x100))
                return 0;
        }
    return 1;
}
static void kind_slots(void) {
    int k, s, src = -1, next = 0;
    if (!people_game_slot_as || !people_record || !people_busy || !kind_count)
        return;
    for (s = 0; s < 6 && src < 0; s++)
        if (fighter_type(people_record(s)[0x1C], 0))
            src = s;
    if (src < 0)
        return;
    for (k = 0; k < kind_count; k++) { /* (first the kinds with no slot of their own) */
        u32 kt = kinds[k][3] & 0xFF;
        int have = 0;
        for (s = 0; s < 6; s++)
            have |= people_record(s)[0x1C] == kt;
        for (s = 0; !have && s < 6; s++)
            if (slot_unneeded(s) && people_game_slot_as(s, src, kt)) {
                ELOG("room: slot %d (nobody's now) made a type %d guard's (a kind with none), in slot %d's model", s, kt, src);
                break;
            }
    }
    for (s = 0; s < 6; s++) /* (then any other slot nobody can need: one more guard at a time, the kinds in turn) */
        if (slot_unneeded(s)) {
            u32 kt = kinds[next++ % kind_count][3] & 0xFF;
            if (people_game_slot_as(s, src, kt))
                ELOG("room: slot %d (nobody's now) made a type %d guard's, in slot %d's model", s, kt, src);
        }
}

static int kind_has_slot(u32 t) { /* (a slot of the section's bound to type t, the people's extra ones too) */
    int s, n = people_slots ? people_slots() : 6;
    if (!people_record)
        return 1;
    for (s = 0; s < n; s++)
        if (people_record(s)[0x1C] == t)
            return 1;
    return 0;
}

/* the room as it starts: the kinds of guards, its doorways and floor */
static int trace_sound(void);
static void flash_reset(void);
static void room_start(void) {
    find_tmd();
    if (trace_sound()) { /* (the mission's sounds of program 9, its voice lines: their numbers, for muting) */
        const u8 *d = *(u8 **)((u8 *)g_Args + 0x130);
        int k;
        for (k = 0; d && k < 0x80; k++)
            if (d[k * 8] == 9)
                port_log("trace: L%d sound: table %u (0x%X) is program 9 tone %d note %d volume %d\n", g_Args->level, k, k,
                         d[k * 8 + 1], d[k * 8 + 2], d[k * 8 + 4]);
    }
    s32 i, j, counts[256];
    room_frames = carried = budget = open = kills_scored = said = burst = at_entry = 0;
    fanfare_at = -1;
    last_sent = 0;
    flash_reset();
    draw_slot = -1;
    kind_count = copy_next = door_count = stuck_count = seated_said = 0;
    for (i = 0; i < 64; i++)
        hunt_ev[i].type = 0x1C, driving[i] = 0, seen_for[i] = 0, drive_rest[i] = spread_until[i] = 0; /* (none yet) */
    memset(cover_state_ptr(), 0, 64);
    nav_reset();
    memset(nav_goal, 0, sizeof(nav_goal));
    last_seen_at = -1, bond_still_since = 0;
    memset(hunters, 0, sizeof(hunters));
    memset(returns, 0, sizeof(returns));
    memset(hunt_on, 1, sizeof(hunt_on)); /* (THE RAMP: decided in its first frame) */
    memset(shot_burst, 0, sizeof(shot_burst)), memset(shot_rest, 0, sizeof(shot_rest));
    cooldown = SEC * 5;
    kills_at_start = *(u16 *)((u8 *)g_Args + 0x2C4);
    rng ^= (D_8009CABF ? 61 : D_8009CDA8) * 2654435761u ^ entry[2] ^ D_800D2F8C; /* (begun in the opening iris: drawn
                                                                                 * as when rooms began with the controls,
                                                                                 * at frame 61) */
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
            if (t && getenv("TWINE_ENDLESS_TEST_FINDCB") && /* (research: =hex: the placed things whose type's
                                                             * handler, def+0x38, is that function) */
                *(u32 *)(TYPE_DEF(t) + 0x38) == (u32)strtoul(getenv("TWINE_ENDLESS_TEST_FINDCB"), 0, 16))
                ELOG("research: type %d (handler %08X) at %d %d %d", t, *(u32 *)(TYPE_DEF(t) + 0x38),
                     p[0] + D_800D2FB0[0], p[1] + D_800D2FB0[1], p[2] + D_800D2FB0[2]);
            if (t && TYPE_DEF(t)[2] == 0x22 && door_count < DOORS) { /* a door's zone */
                doors[door_count][0] = p[0] + D_800D2FB0[0];
                doors[door_count][1] = p[1] + D_800D2FB0[1];
                doors[door_count][2] = p[2] + D_800D2FB0[2];
                door_pt[door_count] = (u32)p[3] >> 27, door_type[door_count] = (u8)t;
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
        if (people_guard_types) { /* (the people's extra slots, past the game's six: copies of these kinds' slots, or
                                   * of a guard's slot for a kind the section has none of) */
            u8 t[16], m[6];
            int nm = 0;
            for (i = 0; i < kind_count && i < 16; i++)
                t[i] = kinds[i][3] & 0xFF;
            for (j = 0; people_record && j < 6; j++)
                if (fighter_type(people_record(j)[0x1C], 0))
                    m[nm++] = people_record(j)[0x1C];
            people_guard_types(t, i, m, nm);
        }
    }
    no_story_people(counts);
    kind_slots();
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
        lent_mask = 0, lent_work_type = 0, watch_on = 0; /* (a mission loaded afresh: its own slots again) */
    pool_pending_slot = -1;
    weapons_capture();
    if (port_endless_surveying()) { /* (a survey's room: its weapons taken, left at once) */
        ELOG("weapons: level %d surveyed", g_Args->level);
        port_endless_survey_next();
        g_Args->flags |= 0x40;
        survey_left = 1; /* (the next mission's first frame starts a room, whatever its frame counter: endless_frame) */
        return;
    }
    if (!segment) {
        lend_weapons(); /* (his weapons this mission lacks, lent before he comes in with them) */
        carry_in(); /* (Bond as he left the last room, before the room's first frame) */
    }
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

/* Rooms whose guards come in by every doorway, Bond's own among them (the playtest: King's Ransom's bar, two doors,
 * one Bond comes in by; Night Watch's small section 6, 4:8, one way in): elsewhere only by those further from where
 * he came in than he is. Level, section. */
static const s8 every_door[][2] = {{8, 5}, {4, 6}};
static int both_doors(void) {
    int i;
    for (i = 0; i < (int)(sizeof(every_door) / sizeof(every_door[0])); i++)
        if (g_Args->level == every_door[i][0] && D_800D2F8C == every_door[i][1])
            return 1;
    return 0;
}

static int lately(s32 x, s32 z) { /* (by where one of the last few came, or by where Bond came in; or someone there) */
    int k;
    if (stuck_near(x, z))
        return 1;
    for (k = D_800B0DA0; people_cap() > 6 && k <= D_800B07B0; k++) /* (more than six slots: spawn() would refuse
                                                                        * it; more people, more places taken) */
        if (OBJ(k) != g_Player && OBJ(k)[4] && HEALTH(OBJ(k)) > 0 && dist2(POS(OBJ(k))[0], POS(OBJ(k))[2], x, z) < 500)
            return 1;
    if (dist2(x, z, entry[2], entry[4]) < 3500 && !both_doors())
        return 1;
    for (k = 0; k < 4; k++)
        if ((recent[k][0] | recent[k][2]) && dist2(recent[k][0], recent[k][2], x, z) < 2500)
            return 1;
    return 0;
}

/* (a place on Bond's own floor: another floor's guards would stay up there and shoot down. Turncoat's train cars
 * (sections 1-4): the car's own floor only, not the strip of platform outside its middle door, 470 up, where the
 * game's own AI (-o) walked a guard sent there off the floor mesh, along the platform) */
static int level_dy(void) {
    return g_Args->level == 3 && D_800D2F8C >= 1 && D_800D2F8C <= 4 ? 300 : 1000;
}
#define LEVEL_DY 1000
#define PLACE_DY level_dy() /* (where a guard is sent; his ways about, the hunters' paths, keep LEVEL_DY) */

static int place(int *door) {
    s32 bx = g_Args->playerX, by = g_Args->playerY, bz = g_Args->playerZ, at = dist2(bx, bz, entry[2], entry[4]);
    s32 fx = isin(gPsi), fz = isin(gPsi + 0x400); /* the way he looks */
    static u16 pick[FACES];
    int i, k, n = 0;
    *door = 1;
    for (k = 0; k < door_count; k++) {
        s32 best = 0x7FFFFFFF, b = -1, d = dist2(bx, bz, doors[k][0], doors[k][2]);
        if (d < 3500 || d > 20000 || (dist2(entry[2], entry[4], doors[k][0], doors[k][2]) < at + 500 && !both_doors()))
            continue;
        for (i = 0; i < faces; i++) {
            s32 e;
            if (!reach[i] || shut[i] || (e = dist2(doors[k][0], doors[k][2], mid[i][0], mid[i][2])) > 2200 ||
                e < 800 || mid[i][1] - doors[k][1] > 1200 || doors[k][1] - mid[i][1] > 1200 ||
                mid[i][1] - by > PLACE_DY || by - mid[i][1] > PLACE_DY)
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
        if (!reach[i] || shut[i] || mid[i][1] - by > PLACE_DY || by - mid[i][1] > PLACE_DY)
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
                dist2(doors[k][0], doors[k][2], mid[i][0], mid[i][2]) > 800 && mid[i][1] - by < PLACE_DY &&
                by - mid[i][1] < PLACE_DY && !lately(mid[i][0], mid[i][2]) && clear(i)) {
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
            if (!reach[i] || shut[i] || d <= best || d > 11000 || mid[i][1] - by > PLACE_DY || by - mid[i][1] > PLACE_DY ||
                (dx / 16 * fx + dz / 16 * fz) / (d / 16 + 1) > 4096 / 4 || lately(mid[i][0], mid[i][2]) || !clear(i))
                continue;
            best = d, b = i;
        }
        if (b < 0) { /* the last: any place he can walk to, not right by him (a tiny room) */
            for (i = 0; i < faces; i++) {
                s32 d = dist2(bx, bz, mid[i][0], mid[i][2]);
                if (reach[i] && !shut[i] && d > 2000 && d < 11000 && d > best - 2500 && !stuck_near(mid[i][0], mid[i][2]) &&
                    mid[i][1] - by <= PLACE_DY &&
                    by - mid[i][1] <= PLACE_DY)
                    best = d + 2500, b = i;
            }
        }
        if (b < 0) { /* (why: for the log) */
            int r = 0, lv = 0, cl = 0, la = 0;
            for (i = 0; i < faces; i++)
                if (reach[i] && !shut[i]) {
                    r++;
                    if (mid[i][1] - by <= PLACE_DY && by - mid[i][1] <= PLACE_DY) {
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
    {   /* (a kind with no slot of its own in the section can never be made: its record (its script) with the type of
         * the next kind that has one, as the kinds-in-turn below would come to) */
        int k0 = rnd() % kind_count, k;
        memcpy(rec, kinds[k0], sizeof(copies[0]));
        for (k = 0; k < kind_count && !kind_has_slot(kinds[(k0 + k) % kind_count][3] & 0xFF); k++)
            ;
        if (k > 0 && k < kind_count)
            rec[3] = (rec[3] & ~0xFF) | (kinds[(k0 + k) % kind_count][3] & 0xFF);
    }
    rec[0] = mid[i][0] - D_800D2FB0[0];
    rec[1] = mid[i][1] - D_800D2FB0[1];
    rec[2] = mid[i][2] - D_800D2FB0[2];
    rec[3] &= ~0x300;           /* not made yet; not asleep */
    rec[4] &= ~(0x1F << 7);     /* no path to walk (the kind's was in its own section) */
    rec[4] &= ~(0xFF << 12);    /* nor the pose it was placed in (a seated guard's, at a desk: */
    rec[4] &= ~(7 << 24);       /* its start pose and state, w4's bits 12-19 and 24-26, and the flags' 14-26) */
    rec[4] &= ~(3u << 27);      /* (and bits 27-28: Meltdown's kinds walk a set line with them, every copy to one spot) */
    rec[3] &= ~(0x1FFF << 14);
    rec[3] &= ~0x400;           /* (and the pose's flag) */
    making = 1;
    o = func_8003C1BC(rec, mid[i][0], mid[i][1], mid[i][2]);
    for (h = 0; !o && people_cap() > 6 && h < kind_count; h++) { /* (more than six slots: the kinds in turn with a
                                                                * slot free, none freed while one is) */
        rec[3] = (rec[3] & ~0x3FF) | (kinds[h][3] & 0xFF);
        o = func_8003C1BC(rec, mid[i][0], mid[i][1], mid[i][2]);
        rec[3] &= o ? ~0 : ~0x300;
    }
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
        ELOG("spawn: the game made no guard (type %d: the people's slots are full)", rec[3] & 0xFF);
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
    last_sent = room_frames;
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
            s32 was[3];
            was[0] = g_Args->playerX, was[1] = g_Args->playerY, was[2] = g_Args->playerZ;
            g_Args->playerX = POS(o)[0], g_Args->playerY = POS(o)[1], g_Args->playerZ = POS(o)[2];
            flood(); /* (the floor the guard can walk to: reach[], from him for this; from Bond again below) */
            g_Args->playerX = was[0], g_Args->playerY = was[1], g_Args->playerZ = was[2];
            for (a = 0; a < 4096; a += 64) { /* (nearest that way with floor at his height) */
                s32 x = POS(o)[0] + (isin(a) * 1500 >> 12), z = POS(o)[2] + (isin(a + 0x400) * 1500 >> 12);
                s32 fh = floor_at(x, POS(o)[1] - 500, z), off = (a - toward) & 0xFFF;
                off = off > 0x800 ? 0x1000 - off : off;
                int k, shut_between = 1, f = -1;
                s32 fd = 600;
                for (k = 0; k < faces; k++) /* (the floor there: one he can walk to without a doorway, not across a
                                             * door that may be shut between them: the train cars' middle doors) */
                    if (dist2(x, z, mid[k][0], mid[k][2]) < fd && mid[k][1] - POS(o)[1] < 300 && POS(o)[1] - mid[k][1] < 300)
                        fd = dist2(x, z, mid[k][0], mid[k][2]), f = k;
                /* (floor beyond a doorway from him; in Turncoat's train cars no door zone near the way between them at
                 * all: their middle doors shut between Bond and the guard (-o 3:5); a guard by one: no meeting) */
                shut_between = f >= 0 && !reach[f] && !shut[f];
                for (k = 0; level_dy() < 1000 && k < door_count && !shut_between; k++) { /* (the cars: no door near
                                                                                          * the way between them) */
                    s32 dx = x - POS(o)[0], dz = z - POS(o)[2], px = doors[k][0] - POS(o)[0], pz = doors[k][2] - POS(o)[2];
                    s32 t = (px * dx + pz * dz) / 1500; /* (along the way, 0..1500) */
                    t = t < 0 ? 0 : t > 1500 ? 1500 : t;
                    shut_between = dist2(px, pz, dx * t / 1500, dz * t / 1500) < 1600;
                }
                if (fh != -0x7FFF && fh - POS(o)[1] < 200 && POS(o)[1] - fh < 200 && -off > best && !shut_between)
                    best = -off, ba = a;
            }
            if (best > -0x7FFFFFFF) {
                at[0] = D_800D2F8C, at[1] = (s16)((ba + 0x800) & 0xFFF), at[2] = POS(o)[0] + (isin(ba) * 1500 >> 12);
                at[3] = POS(o)[1], at[4] = POS(o)[2] + (isin(ba + 0x400) * 1500 >> 12);
                bond_put(at);
                g_Args->playerX = at[2], g_Args->playerY = at[3], g_Args->playerZ = at[4];
                ELOG("test: Bond put by the sent guard type %d, at %d %d %d (he at %d %d %d)", o[6], at[2], at[3], at[4],
                     POS(o)[0], POS(o)[1], POS(o)[2]);
                if (getenv("TWINE_ENDLESS_TEST_NOISE"))
                    func_80053DE4(0, 0, at[2], at[3], at[4]);
                if (getenv("TWINE_ENDLESS_TEST_MEET_GRENADE")) { /* (and a grenade of Bond's lobbed at him) */
                    s32 from[3], to[3];
                    from[0] = at[2], from[1] = at[3] - 1500, from[2] = at[4];
                    to[0] = POS(o)[0], to[1] = POS(o)[1], to[2] = POS(o)[2];
                    quick_grenade_enemy(from, to, 0);
                    ELOG("test: a grenade of Bond's lobbed at the guard");
                }
            }
            flood(); /* (Bond's floor again) */
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
        if ((room_frames - hunters[h].moved > SEC * 10 && d > 2500 && d < 8000) || room_frames - hunters[h].moved > SEC * 15) {
            /* (or wedged anywhere a long while: a doorway's corner) */
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

/* explosives (thrown, launched, set): their ammo scarcer in drops */
static int explosive(const char *nm) {
    return !strncmp(nm, "FRAG", 4) || !strncmp(nm, "SATCHEL", 7) || !strncmp(nm, "FLASH", 5) ||
           !strncmp(nm, "GRENADE LAUNCHER", 16) || !strncmp(nm, "ROCKET", 6) || !strncmp(nm, "GAS GRENADE", 11) ||
           !strncmp(nm, "PIPE BOMB", 9);
}

/* What a guard leaves as he falls: armour, ammo for one of the mission's weapons, now and then a weapon */
static s32 last_drop[3], last_box[3];
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
    /* and ammo, mostly: for one of the weapons Bond carries (one lent to the mission too), the emptier the likelier,
     * none for one full; explosives' scarcer than bullets. (So a favourite gun is kept fed, whatever else he has.) */
    if (rnd() % 100 < 70 && na) {
        s32 weight[19], total = 0, pick = -1;
        for (i = 0; i < 19; i++) {
            s32 sl = D_8009D130[i * 4], cap, have;
            Weapon *w;
            const char *nm;
            weight[i] = 0;
            if (sl < 0 || sl >= SLOTS)
                continue;
            w = &g_Weapons[sl];
            nm = (char *)w->pad0;
            if (!(w->flags & 0x800) || /* (his: in the weapon buttons' round, or out of it by his checkmark only (one the
                                        * mission keeps out itself, King's Ransom's frag grenade, as before)) */
                (!(w->flags & 0x1000) && !(user_out >> sl & 1) && !(lent_mask >> sl & 1)) ||
                !combat_item(nm) || !strncmp(nm, "UNARMED", 7) || !strncmp(nm, "SATCHEL DET", 11))
                continue;
            cap = *(s16 *)&w->maxAmmo, have = w->reserve;
            if (cap > 0 && have >= cap)
                continue;
            weight[i] = cap > 0 ? 10 + 90 * (cap - have) / cap : 50;
            if (explosive(nm))
                weight[i] = (weight[i] + 3) / 4;
            total += weight[i];
        }
        {   /* (and the pool's weapons waiting for a slot: Bond's carry holds their ammo) */
            EndlessCarry *c = &port_endless_carry;
            int pi, j;
            for (pi = 0; pi < pool_n; pi++) {
                const WeaponRec *r = pool[pi].r;
                s32 cap = *(s16 *)(r->def + 0x5C), have;
                if (pool_shown(pi) || r->kind >= 19 || weight[r->kind] || (j = carry_entry(pool[pi].name)) < 0 ||
                    !c->weapon[j].owned)
                    continue;
                have = c->weapon[j].reserve;
                if (cap > 0 && have >= cap)
                    continue;
                weight[r->kind] = cap > 0 ? 10 + 90 * (cap - have) / cap : 50;
                total += weight[r->kind];
            }
        }
        if (total > 0) {
            s32 r2 = rnd() % total;
            for (i = 0; i < 19 && pick < 0; i++)
                if (weight[i] && (r2 -= weight[i]) < 0)
                    pick = i;
        } else
            pick = ammo[rnd() % na];
        if (pick >= 0 && (box = pickup(D_8009D108, x + 150, y, z + 150, "ammo box"))) {
            *(s32 *)(box + 0x34) = pick;
            last_box[0] = x + 150, last_box[1] = y, last_box[2] = z + 150;
            if (D_8009D130[pick * 4] >= 0) {
                ELOG("pickup: the box holds %s's ammo%s", (char *)g_Weapons[D_8009D130[pick * 4]].pad0,
                     lent_mask >> D_8009D130[pick * 4] & 1 ? " (a lent weapon)" : "");
            } else
                ELOG("pickup: the box holds kind %d's ammo (a lent weapon waiting in the pool)", pick);
        }
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

/* ---- Enemy AI, IMPROVED (the enemy AI setting: TWINE_ENEMY_AI "original" keeps the game's own; endless only) ----
 *
 * Built on the game's own AI: each guard's script reacts to the nearest event it perceives on the AI's board
 * (func_80053ED8, every frame: gunshots, alarms, ...), going to see. A hunter is given an event of his own instead,
 * a gunshot at a place picked for him, so that the scripts' own walking, searching and fighting do the rest:
 *  - HUNT: the place is Bond's last known position (where a guard last saw him, or heard him fire; refreshed now
 *    and then as if by radio), on the far side of which...
 *  - NO BUNCHING / FLANKING: ...each hunter takes his own point of a ring round it (angles spread by his number),
 *    on floor Bond can reach: a camping Bond is closed on from every side, not through one doorway;
 *  - STAGGERED ADVANCE: hunters advance in two halves by turns (3 s), the others holding where they are;
 *  - SEARCH: when Bond is lost (no sighting for 8 s) and a hunter has got to his point, he searches nearby
 *    places a while.
 * A guard who sees Bond fights as the game has him (his script); fixed guards (snipers, posts) are left alone. */
static s32 hunt_at[64][3], hunt_since[64];
static s32 last_seen[3], bond_was[3], bond_prev[3], bond_speed;
static int ai_on = -1;

static int ai_improved(void) {
    if (ai_on < 0) {
        const char *v = getenv("TWINE_ENEMY_AI");
        ai_on = !(v && (v[0] == 'o' || v[0] == 'O' || v[0] == '0'));
    }
    return ai_on;
}

static u8 blind_now[64]; /* (blinded by a flash bang: the port's AI keeps off him, THE FLASH BANG'S GUARDS) */
/* THE POSTED GUARDS: a room's own guards at their posts (a fixed script: King's Ransom's bar's three, its "PUB")
 * keep to them, as at home; but when the director has got nobody in for 20 s (the room's slots all held: the bar's
 * by those three), they hunt Bond like any sent guard. Never a sniper (each level's sniper script, -1: none). */
static const s8 sniper_script[12] = {-1, 2, -1, 4, -1, -1, -1, -1, 3, -1, -1, -1};
static int posted_hunt(void) {
    return room_frames - last_sent > SEC * 20;
}
static int any_hunter(u8 *o) { /* (one who would hunt Bond: a copy, or one of the room's own moving guards) */
    s32 *src = *(s32 **)(o + 0x60);
    if (o == g_Player || !o[4] || o[5] != 6 || HEALTH(o) <= 0 || !*(u8 **)(o + 0x118) || blind_now[(o - OBJ(0)) / 0x124 & 63])
        return 0;
    if (is_copy(o))
        return 1;
    if (src && FIXED_SCRIPT(src[1]) && posted_hunt() && fighter_type(o[6], 0) &&
        (src[1] >> 20 & 7) != sniper_script[g_Args->level < 12 ? g_Args->level : 0])
        return 1; /* (THE POSTED GUARDS: a room nobody comes to has its own posted guards come for Bond) */
    return src && !FIXED_SCRIPT(src[1]) && fighter_type(o[6], 0);
}

/* THE RAMP: the early waves gentler. As many hunt Bond at once as the wave allows (the room's own guards counted
 * with the director's: 2 in the first wave, one more a wave, all from the fifth), the nearest first; the others keep
 * to their places (they hear no distant gunshots: func_80053ED8) and fight only when they see him. And as many
 * shoot at once as the wave allows (1 in the first two waves, 2 in the next two, 3, then all from the seventh), each
 * in bursts (half a second at first, a second from the fifth wave) with a rest between (two seconds at first, down to
 * half a second): func_80053114. TWINE_ENDLESS_TEST_NO_RAMP=1: as before (tests). */
static int ramp(void) {
    if (ramp_off < 0)
        ramp_off = getenv("TWINE_ENDLESS_TEST_NO_RAMP") != 0;
    return !ramp_off;
}
static int hunt_cap(void) {
    int w = port_endless_wave();
    return !ramp() || w >= 5 ? 64 : 1 + w;
}
static int shoot_cap(void) {
    int w = port_endless_wave();
    return !ramp() || w >= 7 ? 64 : 1 + (w - 1) / 2;
}
static int hunter(u8 *o) {
    if (!any_hunter(o))
        return 0;
    return hunt_on[((o - OBJ(0)) / 0x124) & 63];
}
static int no_way(int i) { /* (resting from a way to Bond that wasn't there: his part of the floor doesn't reach) */
    return drive_rest[i] > room_frames;
}
static void ramp_hunters(void) { /* (who may hunt: the nearest as many as the wave allows; every half second) */
    int i, j, n = 0, cap = hunt_cap(), order[64];
    s32 d[64];
    for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) {
        u8 *o = OBJ(i);
        hunt_on[i] = 0;
        if (!any_hunter(o))
            continue;
        d[i] = dist2(POS(o)[0], POS(o)[2], g_Args->playerX, g_Args->playerZ) - (react[i] ? 100000 : 0) +
               (no_way(i) ? 1000000 : 0);
        for (j = n; j > 0 && d[order[j - 1]] > d[i]; j--) /* (those who see him first, then the nearest; last those
                                                          * with no way to him) */
            order[j] = order[j - 1];
        order[j] = i, n++;
    }
    for (j = 0; j < n && j < cap; j++)
        hunt_on[order[j]] = 1;
}

/* the reachable floor polygon nearest (x, z) on Bond's floor (-1 none) */
static int floor_near(s32 x, s32 z, s32 y) {
    int i, b = -1;
    s32 best = 0x7FFFFFFF;
    for (i = 0; i < faces; i++) {
        s32 d;
        if (!reach[i] || shut[i] || mid[i][1] - y > LEVEL_DY || y - mid[i][1] > LEVEL_DY)
            continue;
        d = dist2(x, z, mid[i][0], mid[i][2]);
        if (d < best)
            best = d, b = i;
    }
    return b;
}

/* LOCOMOTION: a hunter who doesn't see Bond is walked there by the port, along the floor (a path over the section's
 * floor polygons, breadth first from his goal), his script held meanwhile (its wait, ctx+0x1C4) and his model
 * turned and given his kind's running animation (the game moves him by the animation's own steps, its walls and
 * all); seeing Bond, or there, he is his script's again. The running animation's number differs by level and kind:
 * the ones the port saw the game's own guards run with, and any it sees them run with from then on. */
static u16 navd[8][FACES];
static s32 way[64][3], drive_moved[64][3], drive_check[64], on_floor[64][3], on_floor_at[64];
static u8 walked_ever[64];
static u8 drive_unstick[64], idle[64], seen_sight[64], spread_tries[64];
static s32 ring_off[64];
static s32 nav_blocked[FACES]; /* (until when a polygon is left out of paths: guards wedged on it) */
static s16 drive_anim[64];
static s32 hold_ground[64];
static void nav_reset(void) {
    memset(nav_blocked, 0, sizeof(nav_blocked));
    memset(drive_anim, 0xFF, sizeof(drive_anim));
    memset(hold_ground, 0, sizeof(hold_ground));
    memset(walked_ever, 0, sizeof(walked_ever));
    memset(on_floor_at, 0, sizeof(on_floor_at));
    grenade_reset();
    callouts_reset();
}
/* COVER: a guard in a fight who is hit, or finds Bond's gun on him, runs to a place close by out of Bond's sight,
 * holds there a moment, steps back out to where he could see him and shoots a while, and so on: never more than two
 * at once in cover (the others keep him pinned) */
enum { COVER_NONE, COVER_GO, COVER_HOLD, COVER_PEEK_GO, COVER_PEEK };
static u8 cover_state[64];
static s32 cover_at[64][3], peek_at[64][3], cover_until[64];
static u8 *cover_state_ptr(void) {
    return cover_state;
}
static s32 idle_at[64][3], idle_check[64];
static struct { u8 type; s16 anim; s16 count; } run_seen[16];
static s32 run_was[64][4];

static s16 run_anim(u8 type) {
    static const s16 seed[12] = {0, 0, 32, 17, 0, 16, 33, 33, 79, 16, 17, 0};
    int k;
    for (k = 0; k < 16; k++)
        if (run_seen[k].type == type && run_seen[k].count >= 3)
            return run_seen[k].anim;
    if (g_Args->level == 5 && type == 30)
        return 29;
    if (g_Args->level == 10 && type == 47)
        return 103;
    if (g_Args->level == 10 && type == 40)
        return 19;
    return g_Args->level >= 1 && g_Args->level <= 11 ? seed[g_Args->level] : 0;
}

static void learn_run(int i, u8 *o) { /* (a guard of the game's moving fast: the animation he does it with) */
    u8 *an = *(u8 **)(o + 0x94);
    s32 d;
    int k;
    if (!an || driving[i])
        return;
    d = dist2(POS(o)[0], POS(o)[2], run_was[i][0], run_was[i][2]);
    if (run_was[i][3] == *(s16 *)(an + 0x10) && d > 60 * 10 && d < 4000) {
        for (k = 0; k < 16 && run_seen[k].count && (run_seen[k].type != o[6] || run_seen[k].anim != *(s16 *)(an + 0x10)); k++)
            ;
        if (k < 16) {
            run_seen[k].type = o[6], run_seen[k].anim = *(s16 *)(an + 0x10);
            if (run_seen[k].count++ == 3)
                ELOG("ai: type %d runs with animation %d", o[6], run_seen[k].anim);
        }
    }
    run_was[i][0] = POS(o)[0], run_was[i][2] = POS(o)[2], run_was[i][3] = *(s16 *)(an + 0x10);
}

static int face_at(s32 x, s32 y, s32 z) { /* (the floor polygon under a place: nearest middle at about its height) */
    int i, b = -1;
    s32 best = 0x7FFFFFFF;
    for (i = 0; i < faces; i++) {
        s32 d, dy = mid[i][1] - y;
        if (dy > 800 || dy < -800 || shut[i])
            continue;
        d = dist2(x, z, mid[i][0], mid[i][2]);
        if (d < best)
            best = d, b = i;
    }
    return b;
}

static void nav_build(int h, s32 x, s32 y, s32 z) { /* (distances in polygons to the goal, for hunter h) */
    u8 *mesh = (u8 *)D_800E2F54;
    int g = face_at(x, y, z), head = 0, tail = 0, k, j;
    memset(navd[h], 0xFF, sizeof(navd[h]));
    nav_goal[h][0] = x, nav_goal[h][1] = y, nav_goal[h][2] = z;
    if (g < 0 || !mesh)
        return;
    navd[h][g] = 0;
    queue[tail++] = g;
    while (head < tail) {
        int c = queue[head++];
        u16 *f = (u16 *)(*(u8 **)(mesh + 0x24) + c * 8);
        for (k = 0; k < 3; k++) {
            if (f[k] >= VERTS)
                continue;
            for (j = vfirst[f[k]]; j < vfirst[f[k] + 1]; j++) {
                int n = vfaces[j];
                if (navd[h][n] == 0xFFFF && !shut[n] && mid[n][1] - mid[c][1] < 700 && mid[c][1] - mid[n][1] < 700 &&
                    nav_blocked[n] <= room_frames) {
                    navd[h][n] = navd[h][c] + 1;
                    queue[tail++] = n;
                }
            }
        }
    }
}

/* the next place on his way: two polygons on down the distances from where he is (none: -1) */
static int nav_next(int h, u8 *o, s32 *to) {
    u8 *mesh = (u8 *)D_800E2F54;
    int c = face_at(POS(o)[0], POS(o)[1], POS(o)[2]), step, k, j;
    if (c < 0 || navd[h][c] == 0xFFFF)
        return -1;
    for (step = 0; step < 2 && navd[h][c]; step++) {
        u16 *f = (u16 *)(*(u8 **)(mesh + 0x24) + c * 8);
        int b = c;
        for (k = 0; k < 3; k++) {
            if (f[k] >= VERTS)
                continue;
            for (j = vfirst[f[k]]; j < vfirst[f[k] + 1]; j++)
                if (navd[h][vfaces[j]] < navd[h][b])
                    b = vfaces[j];
        }
        if (b == c)
            break;
        c = b;
    }
    to[0] = mid[c][0], to[1] = mid[c][1], to[2] = mid[c][2];
    return c;
}

static int sight(u8 *o);
static int sight_from(s32 x, s32 y, s32 z);
static int steer_to(u8 *o, s32 x, s32 z, int run_it) { /* (a guard turned toward a place and running at it, his
                                                           * script held: one frame of it) */
    u8 *an = *(u8 **)(o + 0x94), *ctx = *(u8 **)(o + 0x118);
    s32 dx = x - POS(o)[0], dz = z - POS(o)[2], a, best = -0x7FFFFFFF, ba = 0, d;
    s16 *yaw, run = run_anim(o[6]);
    if (!an || !ctx || !run)
        return 0;
    for (a = 0; a < 4096; a += 16) {
        d = (isin(a) >> 4) * (dx >> 4) + (isin(a + 0x400) >> 4) * (dz >> 4);
        if (d > best)
            best = d, ba = a;
    }
    *(s32 *)(ctx + 0x1C4) = 2;
    yaw = (s16 *)(*(u8 **)(o + 0x90) + 0x62);
    d = (ba - *yaw) & 0xFFF;
    if (d > 0x800)
        d -= 0x1000;
    d = d > 0x60 ? 0x60 : d < -0x60 ? -0x60 : d;
    *yaw = (s16)((*yaw + d) & 0xFFF);
    if (run_it && *(s16 *)(an + 0x10) != run) {
        *(s16 *)(an + 0x10) = *(s16 *)(an + 0x12) = run;
        an[0x14] = 0, an[0x16] = 1;
    }
    *(s32 *)(an + 0x24) &= ~0x40000; /* (standing to shoot: the animation's steps not taken; he runs now) */
    if (run_it == 2) { /* (in a fight the game doesn't take his steps: moved by hand, along the floor, not through
                        * a wall) */
        s32 step = 70, len = dist2(0, 0, dx, dz), nx, nz, fy, from[4], to[4];
        s16 n[4], h[4];
        if (len < step)
            step = len;
        if (len > 0) {
            nx = POS(o)[0] + dx * step / len, nz = POS(o)[2] + dz * step / len;
            fy = floor_at(nx, POS(o)[1] - 500, nz);
            from[0] = POS(o)[0], from[1] = POS(o)[1] - 600, from[2] = POS(o)[2], from[3] = 0;
            to[0] = nx + dx * 100 / len, to[1] = POS(o)[1] - 600, to[2] = nz + dz * 100 / len, to[3] = 0;
            if (fy != -0x7FFF && fy - POS(o)[1] < 200 && POS(o)[1] - fy < 200 && !walls_between(D_800E2F2C, from, to, n, h, 0))
                POS(o)[0] = nx, POS(o)[2] = nz, POS(o)[1] = fy;
        }
    }
    return 1;
}
static int steer_to(u8 *o, s32 x, s32 z, int run_it); /* (below) */

static void cover_frame(void) {
    int i, in_cover = 0;
    for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++)
        in_cover += cover_state[i] != COVER_NONE;
    for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) {
        u8 *o = OBJ(i), *ctx;
        if (!hunter(o) || !(ctx = *(u8 **)(o + 0x118))) {
            cover_state[i] = COVER_NONE;
            continue;
        }
        switch (cover_state[i]) {
        case COVER_NONE:
            if (in_cover < 2 && react[i] > SEC && !(room_frames % 4) &&
                (((*(s32 *)ctx & 0x08000001) && (rnd() & 1)) || (react[i] > SEC * 4 && !(rnd() % 40)))) {
                /* (or long in the open: now and then) */
                /* (hit, or Bond aiming at him: a place out of sight nearby, the nearest of some tried) */
                int k, b = -1;
                s32 best = 0x7FFFFFFF;
                for (k = 0; k < 14; k++) {
                    s32 a = (s32)(rnd() & 0xFFF), r = 500 + (s32)(rnd() % 1300);
                    int f = face_at(POS(o)[0] + (isin(a) * r >> 12), POS(o)[1], POS(o)[2] + (isin(a + 0x400) * r >> 12));
                    s32 d;
                    if (f < 0 || sight_from(mid[f][0], mid[f][1], mid[f][2]))
                        continue;
                    d = dist2(POS(o)[0], POS(o)[2], mid[f][0], mid[f][2]);
                    if (d < best && d > 300 && d < 2500) /* (close by: not a polygon across the room) */
                        best = d, b = f;
                }
                if (b >= 0) {
                    cover_state[i] = COVER_GO, in_cover++;
                    cover_at[i][0] = mid[b][0], cover_at[i][1] = mid[b][1], cover_at[i][2] = mid[b][2];
                    peek_at[i][0] = POS(o)[0], peek_at[i][1] = POS(o)[1], peek_at[i][2] = POS(o)[2];
                    cover_until[i] = room_frames + SEC * 3;
                    ELOG("ai: type %d takes cover at %d %d (%d off)", o[6], cover_at[i][0], cover_at[i][2], best);
                }
            }
            break;
        case COVER_GO:
        case COVER_PEEK_GO: {
            s32 *t = cover_state[i] == COVER_GO ? cover_at[i] : peek_at[i];
            if (dist2(POS(o)[0], POS(o)[2], t[0], t[2]) < 300 || room_frames > cover_until[i]) {
                cover_state[i] = cover_state[i] == COVER_GO ? COVER_HOLD : COVER_PEEK;
                cover_until[i] = room_frames + SEC * (cover_state[i] == COVER_HOLD ? 2 : 3);
            } else
                steer_to(o, t[0], t[2], 2);
            break;
        }
        case COVER_HOLD: /* (out of sight, waiting: his script's own standing, kept to the place) */
            if (dist2(POS(o)[0], POS(o)[2], cover_at[i][0], cover_at[i][2]) > 400)
                steer_to(o, cover_at[i][0], cover_at[i][2], 2);
            if (room_frames > cover_until[i])
                cover_state[i] = COVER_PEEK_GO, cover_until[i] = room_frames + SEC * 3;
            break;
        case COVER_PEEK: /* (out where he sees him: his script shoots) */
            if (room_frames > cover_until[i]) {
                cover_state[i] = (rnd() % 3) ? COVER_GO : COVER_NONE; /* (again, or done with it) */
                cover_until[i] = room_frames + SEC * 3;
            }
            break;
        }
    }
}

/* GRENADES (IMPROVED): a guard near a live grenade (Bond's, or another's) shouts and scatters from it, run by
 * hand away from it a second and a half; and when Bond camps, or keeps out of their sight, a hunter some way off
 * lobs one at him (the mod's own grenade: quickgrenade.c), shouting as he throws. The shout is each level's
 * guards' "grenade!" (King's Ransom's, as the user picked it out: none of the levels' scripts shouts at a grenade,
 * their handler of the game's "grenade!" flag only plays a dive), and logged. */
void port_voice_play(int level, int program, int tone, s32 vol); /* (quickgrenade.c) */
static s32 scatter_until[64], scatter_to[64][3], enemy_throw_at = -SEC * 30, grenade_shouts, grenade_throws, last_shout[64];
static void grenade_reset(void) {
    memset(scatter_until, 0, sizeof(scatter_until));
    memset(last_shout, 0x80, sizeof(last_shout)); /* (long ago) */
    enemy_throw_at = -SEC * 30;
}

static void shout(u8 *o, const char *why) {
    /* (King's Ransom's guard's "grenade!": its voice program 9, tone 2, lent to every mission; louder near Bond) */
    s32 d = dist2(POS(o)[0], POS(o)[2], g_Args->playerX, g_Args->playerZ);
    port_voice_play(8, 9, 2, d > 12000 ? 0x1800 : 0x6000 - d * 0x4800 / 12000);
    grenade_shouts++;
    ELOG("ai: type %d shouts \"grenade!\" (%s)", o[6], why);
}

static void grenade_frame(void) {
    s32 g[8][3];
    int n = port_live_grenades(g, 8), i, k;
    for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) {
        u8 *o = OBJ(i);
        if (o == g_Player || !o[4] || o[5] != 6 || HEALTH(o) <= 0 || !*(u8 **)(o + 0x118) || blind_now[i])
            continue;
        if (room_frames < scatter_until[i]) { /* (running from it) */
            steer_to(o, scatter_to[i][0], scatter_to[i][2], 2);
            continue;
        }
        for (k = 0; k < n; k++) {
            s32 d = dist2(POS(o)[0], POS(o)[2], g[k][0], g[k][2]);
            if (d < 3600 && g[k][1] - POS(o)[1] < 2000 && POS(o)[1] - g[k][1] < 2000) { /* (the blast reaches 3250) */
                s32 dx = POS(o)[0] - g[k][0], dz = POS(o)[2] - g[k][2], f;
                if (!d)
                    dx = 1;
                d = d ? d : 1;
                f = floor_near(POS(o)[0] + dx * 4000 / d, POS(o)[2] + dz * 4000 / d, POS(o)[1]);
                scatter_to[i][0] = f >= 0 ? mid[f][0] : POS(o)[0] + dx * 4000 / d;
                scatter_to[i][2] = f >= 0 ? mid[f][2] : POS(o)[2] + dz * 4000 / d;
                scatter_until[i] = room_frames + SEC * 5 / 2;
                cover_state[i] = COVER_NONE, driving[i] = 0;
                if (room_frames - last_shout[i] > SEC * 4) /* (once a grenade, not again as he runs on) */
                    shout(o, "and runs from it"), last_shout[i] = room_frames;
                break;
            }
        }
    }
    if (room_frames - enemy_throw_at > SEC * 20 && !(room_frames % SEC) && last_seen_at >= 0 &&
        (room_frames - bond_still_since > SEC * 6 || (room_frames - last_seen_at > SEC * 3 && room_frames - last_seen_at < SEC * 8)))
        for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) { /* (Bond camping, or keeping out of sight: one lobbed) */
            u8 *o = OBJ(i);
            s32 d, from[3], to[3];
            if (!hunter(o) || room_frames < scatter_until[i])
                continue;
            d = dist2(POS(o)[0], POS(o)[2], g_Args->playerX, g_Args->playerZ);
            if (d < 2500 || d > 8000)
                continue;
            from[0] = POS(o)[0], from[1] = POS(o)[1] - 1500, from[2] = POS(o)[2];
            to[0] = g_Args->playerX, to[1] = g_Args->playerY, to[2] = g_Args->playerZ;
            {   /* (not with a friend near where it would land) */
                int j;
                for (j = D_800B0DA0; j <= D_800B07B0; j++)
                    if (j != i && OBJ(j)[4] && OBJ(j)[5] == 6 && HEALTH(OBJ(j)) > 0 &&
                        dist2(POS(OBJ(j))[0], POS(OBJ(j))[2], to[0], to[2]) < 3000)
                        break;
                if (j <= D_800B07B0)
                    continue;
            }
            if (quick_grenade_enemy(from, to, o)) {
                enemy_throw_at = room_frames;
                grenade_throws++;
                ELOG("ai: type %d throws a grenade at Bond (%d off)", o[6], d);
                shout(o, "as he throws");
            }
            break;
        }
}

static void drive_frame(void) {
    int i;
    for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) {
        u8 *o = OBJ(i), *an, *ctx;
        int h = i & 7, f;
        s32 dx, dz, a, best, ba = 0, goal[3];
        s16 run;
        if (!o[4] || o[5] != 6 || blind_now[i])
            continue;
        learn_run(i, o);
        if (room_frames < hold_ground[i] && *(u8 **)(o + 0x118)) { /* (wedged: standing, not walked back into it by his
                                                                    * script; he fights when he sees Bond) */
            if (!react[i]) {
                u8 *an3 = *(u8 **)(o + 0x94);
                *(s32 *)(*(u8 **)(o + 0x118) + 0x1C4) = 2;
                if (an3 && drive_anim[i] >= 0 && *(s16 *)(an3 + 0x10) != drive_anim[i])
                    *(s16 *)(an3 + 0x10) = *(s16 *)(an3 + 0x12) = drive_anim[i], an3[0x14] = 0, an3[0x16] = 1;
            }
            continue;
        }
        if (!(room_frames & 3) || driving[i] || cover_state[i] != COVER_NONE || room_frames < scatter_until[i]) {
            /* (walked by the port: kept on
                                                                                  * the floor) */
            if (floor_at(POS(o)[0], POS(o)[1] - 500, POS(o)[2]) == -0x7FFF) {
                if (driving[i] || cover_state[i] != COVER_NONE || room_frames < scatter_until[i]) {
                    if (on_floor_at[i]) /* (back where he last stood on it) */
                        POS(o)[0] = on_floor[i][0], POS(o)[1] = on_floor[i][1], POS(o)[2] = on_floor[i][2];
                    driving[i] = 0, cover_state[i] = COVER_NONE, drive_rest[i] = room_frames + SEC * 4;
                    scatter_until[i] = 0;
                    ELOG("ai: type %d off the floor: left to his script", o[6]);
                    continue;
                }
                else if (on_floor_at[i] && room_frames - on_floor_at[i] > SEC * 2 && walked_ever[i]) {
                    /* (one the port walked, left off the floor since: put back too) */
                    POS(o)[0] = on_floor[i][0], POS(o)[1] = on_floor[i][1], POS(o)[2] = on_floor[i][2];
                    ELOG("ai: type %d found off the floor: put back", o[6]);
                }
            } else
                on_floor[i][0] = POS(o)[0], on_floor[i][1] = POS(o)[1], on_floor[i][2] = POS(o)[2], on_floor_at[i] = room_frames;
            if (driving[i] || cover_state[i] != COVER_NONE || room_frames < scatter_until[i])
                walked_ever[i] = 1;
        }
        if (cover_state[i] != COVER_NONE || room_frames < scatter_until[i]) /* (cover's, or scattering) */
            continue;
        if (!hunter(o) || !(an = *(u8 **)(o + 0x94)) || !(ctx = *(u8 **)(o + 0x118)) || hunt_ev[i].type != 0) {
            driving[i] = 0;
            continue;
        }
        if (!driving[i]) { /* (his own going: where he was 3 s ago) */
            if (room_frames - idle_check[i] >= SEC * 3) {
                idle[i] = dist2(POS(o)[0], POS(o)[2], idle_at[i][0], idle_at[i][2]) < 250;
                idle_at[i][0] = POS(o)[0], idle_at[i][2] = POS(o)[2], idle_check[i] = room_frames;
            }
        }
        if (!driving[i] && !idle[i]) { /* (his script has him going somewhere: his own way) */
            continue;
        }
        if (room_frames < drive_rest[i] || room_frames - made_at[i] < SEC * 4) { /* (resting from a way that wasn't
                                                     * there, or a wall; or just made, his script waking: his own) */
            driving[i] = 0;
            continue;
        }
        if (room_frames >= spread_until[i] &&
            ((*(s32 *)ctx & 0x80) || (!(room_frames % 4) ? (seen_sight[i] = (u8)sight(o)) : seen_sight[i]))) {
            /* (he sees Bond: his script's, after a moment's reaction) */
            if ((seen_for[i] & 0x7F) < 0x7F)
                seen_for[i]++;
            if ((seen_for[i] & 0x7F) > SEC / 3) {
                driving[i] = 0;
                continue;
            }
        } else
            seen_for[i] &= 0x80;
        goal[0] = hunt_ev[i].x, goal[1] = hunt_ev[i].y, goal[2] = hunt_ev[i].z;
        if (dist2(POS(o)[0], POS(o)[2], goal[0], goal[2]) < 900 || !(run = run_anim(o[6]))) {
            driving[i] = 0;
            continue;
        }
        if (dist2(goal[0], goal[2], nav_goal[h][0], nav_goal[h][2]) > 400 || !(room_frames % SEC))
            nav_build(h, goal[0], goal[1], goal[2]);
        if (!driving[i] || dist2(POS(o)[0], POS(o)[2], way[i][0], way[i][2]) < 500 || !(room_frames % (SEC / 2))) {
            if ((f = nav_next(h, o, way[i])) < 0) {
                if (!(seen_for[i] & 0x80)) /* (once: his part of the floor doesn't reach Bond's) */
                    ELOG("ai: no way for type %d at %d %d to %d %d", o[6], POS(o)[0], POS(o)[2], goal[0], goal[2]);
                seen_for[i] |= 0x80;
                drive_rest[i] = room_frames + SEC * 6;
                driving[i] = 0;
                continue;
            }
        }
        if (!driving[i]) {
            if (*(s16 *)(an + 0x10) != run)
                drive_anim[i] = *(s16 *)(an + 0x10);
            ELOG("ai: type %d walked from %d %d toward %d %d (animation %d)", o[6], POS(o)[0], POS(o)[2], goal[0], goal[2], run);
            driving[i] = 1, drive_check[i] = room_frames;
            drive_moved[i][0] = POS(o)[0], drive_moved[i][2] = POS(o)[2];
        }
        if (room_frames - drive_check[i] > SEC * 2) { /* (stuck: left to his script a while) */
            if (dist2(POS(o)[0], POS(o)[2], drive_moved[i][0], drive_moved[i][2]) < 300) {
                /* (running in place: the polygons he is wedged on, and the one he was making for, blocked for paths a
                 * while, and a way round them looked for; wedged again, he is his script's, standing) */
                int c = face_at(POS(o)[0], POS(o)[1], POS(o)[2]), w = face_at(way[i][0], way[i][1], way[i][2]);
                if (c >= 0)
                    nav_blocked[c] = room_frames + SEC * 120;
                if (w >= 0 && w != c)
                    nav_blocked[w] = room_frames + SEC * 120;
                memset(nav_goal, 0, sizeof(nav_goal)); /* (every hunter's way looked for again) */
                if (drive_unstick[i]++ < 1) {
                    ELOG("ai: type %d wedged at %d %d: polygons %d, %d blocked, another way", o[6], POS(o)[0], POS(o)[2], c, w);
                    drive_check[i] = room_frames;
                    drive_moved[i][0] = POS(o)[0], drive_moved[i][2] = POS(o)[2];
                    driving[i] = 0; /* (a new way next frame) */
                    continue;
                }
                ELOG("ai: type %d wedged again at %d %d: standing his ground (his script held till he sees Bond)", o[6],
                     POS(o)[0], POS(o)[2]);
                drive_unstick[i] = 0;
                drive_rest[i] = room_frames + SEC * 15;
                hold_ground[i] = room_frames + SEC * 60;
                {   /* (sent from a doorway whose way in leads here: none sent from it again) */
                    int hh;
                    for (hh = 0; hh < HUNTERS; hh++)
                        if (hunters[hh].o == o && stuck_count < 8)
                            stuck_at[stuck_count][0] = hunters[hh].x, stuck_at[stuck_count][1] = hunters[hh].z, stuck_count++;
                }
                ring_off[i] += 1024 + (s32)(rnd() & 511); /* (his point of the ring moved on: not that way again) */
                if (drive_anim[i] >= 0 && an) /* (not running in place: the animation he had before) */
                    *(s16 *)(an + 0x10) = *(s16 *)(an + 0x12) = drive_anim[i], an[0x14] = 0, an[0x16] = 1;
                *(s32 *)(ctx + 0x1C4) = 0;
                driving[i] = 0;
                continue;
            }
            drive_check[i] = room_frames, drive_moved[i][0] = POS(o)[0], drive_moved[i][2] = POS(o)[2];
            drive_unstick[i] = 0;
        }
    steer:
        dx = way[i][0] - POS(o)[0], dz = way[i][2] - POS(o)[2];
        for (a = 0, best = -0x7FFFFFFF; a < 4096; a += 16) {
            s32 d = (isin(a) >> 4) * (dx >> 4) + (isin(a + 0x400) >> 4) * (dz >> 4);
            if (d > best)
                best = d, ba = a;
        }
        *(s32 *)(ctx + 0x1C4) = 2; /* (his script waits) */
        {   /* (turned toward it at a man's pace, not at once: 0x60 a frame, some 8 degrees) */
            s16 *yaw = (s16 *)(*(u8 **)(o + 0x90) + 0x62);
            s32 d = ((ba - *yaw) & 0xFFF);
            if (d > 0x800)
                d -= 0x1000;
            if (d > 0x60)
                d = 0x60;
            else if (d < -0x60)
                d = -0x60;
            *yaw = (s16)((*yaw + d) & 0xFFF);
        }
        if (*(s16 *)(an + 0x10) != run) {
            *(s16 *)(an + 0x10) = *(s16 *)(an + 0x12) = run;
            an[0x14] = 0, an[0x16] = 1;
        }
        *(s32 *)(an + 0x24) &= ~0x40000; /* (standing to shoot: no steps; he runs now) */
    }
}

/* (the reachable floor polygon nearest (x, z) not within 900 of another hunter's goal: each his own place) */
/* whether he can see (or shoot) Bond: nothing of the section's walls between their heads, and not too far */
static int sight_from(s32 x, s32 y, s32 z) {
    s32 from[4], to[4], d = dist2(x, z, g_Args->playerX, g_Args->playerZ);
    s16 n[4], h[4];
    if (d > 14000 || y - g_Args->playerY > 6000 || g_Args->playerY - y > 6000)
        return 0;
    from[0] = x, from[1] = y - 1200, from[2] = z, from[3] = 0;
    to[0] = g_Args->playerX, to[1] = g_Args->playerY - 1200, to[2] = g_Args->playerZ, to[3] = 0;
    return !walls_between(D_800E2F2C, from, to, n, h, 0);
}

static int sight(u8 *o) {
    return sight_from(POS(o)[0], POS(o)[1], POS(o)[2]);
}

static int floor_near_free(s32 x, s32 z, s32 y, int me) {
    int i, j, b = -1;
    s32 best = 0x7FFFFFFF;
    for (i = 0; i < faces; i++) {
        s32 d;
        if (!reach[i] || shut[i] || mid[i][1] - y > LEVEL_DY || y - mid[i][1] > LEVEL_DY)
            continue;
        d = dist2(x, z, mid[i][0], mid[i][2]);
        if (d >= best)
            continue;
        for (j = D_800B0DA0; j <= D_800B07B0 && j < 64; j++)
            if (j != me && hunt_ev[j].type == 0 && dist2(hunt_ev[j].x, hunt_ev[j].z, mid[i][0], mid[i][2]) < 900)
                break;
        if (j <= D_800B07B0 && j < 64)
            continue;
        best = d, b = i;
    }
    return b >= 0 ? b : floor_near(x, z, y);
}

/* CALLOUTS (IMPROVED): the guards call out as they fight, in lines their own missions' guards have (each mission's voice
 * program 9; any mission's line can be lent to another, as the "grenade!" above): on catching sight of Bond after a
 * while, "I see him!", "intruder!", "over there!" ... ("he's up there!" when he is well above); as they go after him
 * unseen, "get him!", "surrender!" ...; when Bond is hit (now and then), "I've got him!" (a flash bang's "my eyes!" is the
 * script's own: THE FLASH BANG'S GUARDS). Each guard
 * keeps one mission's voice where it has a line for the call; a call at most every 2 s, a guard's every 8 s. (Which
 * guards are men isn't known yet: step 10.) */
typedef struct { u8 level, tone; const char *text; } Line;
enum { CALL_SPOT, CALL_ABOVE, CALL_CHASE, CALL_HIT, CALLS };
static const Line call_lines[CALLS][12] = {
    {{2, 0, "what the?"}, {2, 5, "intruder!"}, {6, 0, "what the!"}, {6, 3, "I see him!"}, {9, 0, "I see him!"},
     {7, 5, "halt!"}, {9, 1, "stop right there!"}, {8, 0, "he's over there!"}, {3, 5, "he's over there!"},
     {10, 0, "over there!"}, {7, 3, "he's over here!"}},
    {{10, 3, "he's up there!"}},
    {{9, 2, "get him!"}, {7, 2, "kill him!"}, {8, 1, "let's get him"}, {2, 4, "surrender!"}, {6, 2, "surrender!"},
     {10, 2, "surrender!"}, {3, 0, "don't let him escape!"}, {2, 1, "raise the alarm!"}},
    {{8, 3, "I've got him!"}, {10, 6, "I think I got him!"}},
};
static const u8 call_voices[] = {2, 3, 6, 7, 8, 9, 10};
static s32 last_call = -SEC * 10, call_at[64], unseen_since[64];
static int calls_made;
static u32 call_seed = 12345;
static u32 call_rnd(void) { /* (the calls' own: the director's and the AI's draws stay as they were) */
    call_seed = call_seed * 1103515245u + 12345u;
    return call_seed >> 16;
}

static void callouts_reset(void) {
    int i;
    last_call = -SEC * 10;
    call_seed = 12345;
    for (i = 0; i < 64; i++)
        call_at[i] = -SEC * 60, unseen_since[i] = -SEC * 60;
}

static int call_out(int i, int call, const char *why) {
    u8 *o = OBJ(i);
    const Line *l = call_lines[call], *pick = 0;
    int n = 0, k, voice = call_voices[(i * 7 + o[6]) % sizeof(call_voices)];
    s32 d;
    if (room_frames - last_call < SEC * 2 || room_frames - call_at[i] < SEC * 8)
        return 0;
    while (n < 12 && l[n].text)
        n++;
    for (k = 0; k < n && !pick; k++)
        if (l[k].level == voice)
            pick = &l[k];
    if (!pick)
        pick = &l[call_rnd() % n];
    d = dist2(POS(o)[0], POS(o)[2], g_Args->playerX, g_Args->playerZ);
    port_voice_play(pick->level, 9, pick->tone, d > 12000 ? 0x1800 : 0x6000 - d * 0x4800 / 12000);
    last_call = call_at[i] = room_frames;
    calls_made++;
    ELOG("ai: type %d calls out \"%s\" (level %d's, %s)", o[6], pick->text, pick->level, why);
    return 1;
}

static void callout_frame(void) {
    int i;
    for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) {
        u8 *o = OBJ(i);
        if (!hunter(o) || HEALTH(o) <= 0 || room_frames < scatter_until[i])
            continue;
        if (react[i] == 1 && room_frames - unseen_since[i] > SEC * 10) /* (he's seen Bond, the first time a while) */
            call_out(i, g_Args->playerY < POS(o)[1] - 1500 ? CALL_ABOVE : CALL_SPOT, "seeing Bond");
        if (react[i])
            unseen_since[i] = room_frames;
        else if (driving[i] && room_frames - unseen_since[i] > SEC * 3 && !(call_rnd() % (SEC * 6)))
            call_out(i, CALL_CHASE, "going after him");
    }
}

/* Bond hit: a guard who sees him, the nearest, calls it now and then */
static void callout_hit(void) {
    static s32 last_hit_call = -SEC * 60;
    int i, best = -1;
    s32 bd = 9000;
    if (!ai_improved() || room_frames - last_hit_call < SEC * 10 || call_rnd() % 3)
        return; /* (not each time: one in three, ten seconds apart at least) */
    for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) {
        u8 *o = OBJ(i);
        s32 d;
        if (!hunter(o) || HEALTH(o) <= 0 || !react[i])
            continue;
        d = dist2(POS(o)[0], POS(o)[2], g_Args->playerX, g_Args->playerZ);
        if (d < bd)
            bd = d, best = i;
    }
    if (best >= 0 && call_out(best, CALL_HIT, "Bond hit"))
        last_hit_call = room_frames;
}

/* THE FLASH BANG'S GUARDS, LENT (see THE FLASH BANG above). Courier's GOON1.AIB from its interrupt on 0x40000 (script
 * offset 0x41D words): distance (+0x110: his script's vars[4]) over 5000 -> over 12000 stand 60 frames (anim 25 looped),
 * else back to the main routine; within: his interrupts on 0x40000 and 0x80 (seeing Bond) off, +0x10 bit 4 set,
 * anim+3 1 (no root motion), "my eyes!" (0x6A at his place), vars[5] = rand() % 0x1000, rate 2 if under 0x800 else 6,
 * vars[7] (+0x3C, his combat state) 0, the blinded animation (78) whole, a blend from its last frame into his standing
 * one (25) and that whole; then a jump (op 0, to 0x461: rate 2, root motion on, every interrupt on again: a flash now
 * blinds him again), vars[7] 3, his recovering one (15) frames 0-1, vars[7] 2, vars[8] up one, the rest of it, rate
 * 2, then his attack routine (0x3B5): 65 frames of animation at his rate, 130 or 334 frames in all. (Its HEADGOON's
 * guards: rate 2 always, the blinded animation into 49, his interrupt on again, his main routine.) Lent, the same
 * commands (in the mod's memory, run by the game's own
 * interpreter: the same handlers, the game's rand) with the mission's numbers for those animations, Courier's own
 * frames (from its DATA02.BIN) where the mission has none the same (only Flashpoint has the blinded one; Christmas's
 * and Meltdown's recovering one differ): put after the mission's table (D_800DDEE4, root motion D_800DDEE0) in
 * copies of it; the voice from Courier's bank (or Flashpoint's "my eyes!", for a guard with its voice: CALLOUTS).
 * Not the same: his own script resumes where it was (not the attack routine: no other mission's script has one at a
 * known place), with his interrupts back on and +0x10 bit 4 as before; meanwhile only interrupts of the kinds Courier's
 * guards have stay on (the jump to 0x461 leaves the rest off: none of Courier's guards has them); the jump's other
 * resets (script flags +0x18 off +0x10, vars by mask +0x1C, the call stack) aren't done; the 5000-12000 case leaves
 * him be; a flash as he recovers blinds him again only within 5000. */
typedef struct { u32 at, mat; u16 frames; } FlashAnim;
static const FlashAnim cour_anims[3] = {{0x5CF7C, 0x5D81C, 29}, {0x49760, 0x49DA0, 21}, {0x473B0, 0x477E0, 14}};
static const char *const cour_anim_names[3] = {"blinded", "standing", "recovering"};
static u8 *cour_anim[3], *cour_mot[3];
static int cour_anims_read; /* (1 read, -1 not to be had) */
static u8 **anim_orig, *mot_orig; /* (the mission's tables, before the copies) */
static u8 *anim_ext[512], *mot_ext[512];
static int anim_ids[3] = {-1, -1, -1}, anim_extended, anim_n;
/* (mods/trace.c) the animations' table's count while it is the copy here; -1 not */
int endless_anim_count(void) {
    extern u8 **D_800DDEE4;
    return anim_extended && D_800DDEE4 == anim_ext ? anim_n : -1;
}
static u32 blind_code[48], far_code[2];
static u32 blind_recover, blind_end; /* (the recovering part's start, the end, in blind_code) */
static const u8 eyes_sfx[2][8] = {{9, 2, 0x1E, 0, 0x7F, 0, 0, 0},  /* (Courier's 0x6A) */
                                  {9, 1, 0x14, 0, 0x7F, 0, 0, 0}}; /* (Flashpoint's 0x60) */
static struct {
    u8 on, far;
    s32 since;
    u32 pc, items, flags10;
} blind[64];

static int flash_anims(void) {
    extern u8 **D_800DDEE4;
    extern u8 *D_800DDEE0;
    int k, j, n, extra = 0;
    if (D_800DDEE4 == anim_ext && anim_extended)
        return 1;
    if (D_800DDEE4 == anim_orig && !anim_extended && anim_ids[0] >= 0)
        return 1;
    if (!cour_anims_read) { /* (Courier's frames and root motion, from its DATA02.BIN) */
        u32 size;
        u8 *data = disc_member(2, "L02/DATA02.BIN", &size);
        cour_anims_read = -1;
        if (data) {
            for (k = 0; k < 3; k++) {
                u32 fl = cour_anims[k].frames * 0x4Cu, ml = cour_anims[k].frames * 6u;
                if (cour_anims[k].mat + ml > size || *(u16 *)(data + cour_anims[k].at) != cour_anims[k].frames)
                    break;
                cour_anim[k] = malloc(fl), cour_mot[k] = malloc(ml);
                memcpy(cour_anim[k], data + cour_anims[k].at, fl);
                memcpy(cour_mot[k], data + cour_anims[k].mat, ml);
            }
            if (k == 3)
                cour_anims_read = 1;
            free(data);
        }
        ELOG("ai: Courier's flash-bang animations %s", cour_anims_read > 0 ? "read from its DATA02.BIN" : "NOT found");
    }
    if (cour_anims_read < 0)
        return 0;
    if (D_800DDEE4 == anim_ext) /* (the copies, but the mission's were put back since: as they are) */
        return 1;
    anim_orig = D_800DDEE4, mot_orig = D_800DDEE0;
    n = (int)((u8 **)(void *)D_800DDEE0 - D_800DDEE4); /* (the root motion's table follows the frames') */
    if (n <= 0 || n + 3 > 512)
        return 0;
    for (k = 0; k < 3; k++) { /* (the mission's own where it has the same, frame for frame) */
        u32 fl = cour_anims[k].frames * 0x4Cu, ml = cour_anims[k].frames * 6u;
        anim_ids[k] = -1;
        for (j = 0; j < n && anim_ids[k] < 0; j++)
            if (*(u16 *)D_800DDEE4[j] == cour_anims[k].frames && !memcmp(D_800DDEE4[j], cour_anim[k], fl) &&
                !memcmp(((u8 **)(void *)D_800DDEE0)[j], cour_mot[k], ml))
                anim_ids[k] = j;
        if (anim_ids[k] < 0)
            anim_ids[k] = n + extra++;
    }
    anim_extended = extra > 0;
    anim_n = n + extra;
    if (anim_extended) {
        memcpy(anim_ext, D_800DDEE4, n * sizeof(u8 *));
        memcpy(mot_ext, D_800DDEE0, n * sizeof(u8 *));
        for (k = 0; k < 3; k++)
            if (anim_ids[k] >= n)
                anim_ext[anim_ids[k]] = cour_anim[k], mot_ext[anim_ids[k]] = cour_mot[k];
        D_800DDEE4 = anim_ext;
        D_800DDEE0 = (u8 *)mot_ext;
    }
    { /* Courier's GOON1.AIB 0x41D (as above), the mission's numbers for the animations; in a run (not
         * TWINE_ENDLESS_TEST_FLASH_VANILLA) the blinded pose held on its last frame as long again as the whole routine
         * (rate 2: 130 frames more, rate 6: 334), the playtest's ask: twice as long blinded */
        u32 B = anim_ids[0], I = anim_ids[1], R = anim_ids[2], *c = blind_code, n2 = 0, jb, je;
        int hold = !flash_vanilla();
#define EMIT(w) (c[n2++] = (w))
        EMIT(0x0000000C), EMIT(4);                   /* +0x10 |= 4 */
        EMIT(0x00000131);                            /* anim+3 = 1 */
        EMIT(0x00290508), EMIT(0), EMIT(0x1000);     /* vars[5] = rand() % 0x1000 */
        EMIT(0x09000406), EMIT(5), EMIT(0x800);      /* vars[5] < 0x800: the next skipped */
        jb = n2, EMIT(0x00000001);                   /* -> rate 6 */
        EMIT(0x0000FF19);                            /* rate 2 */
        EMIT(0x00090709), EMIT(0);                   /* vars[7] = 0 */
        EMIT(0xFF00001B | B << 8);                   /* B 0..last */
        if (hold)
            EMIT(0xFFFF001B | B << 8), EMIT(128 << 16 | 0x1C); /* (held: 2 + 128) */
        EMIT(0x0000001A), EMIT(B | 0xFF00 | I << 16); /* B last -> I 0 */
        EMIT(0xFF00001B | I << 8);                   /* I 0..last */
        je = n2, EMIT(0x00000001);                   /* -> the end */
        c[jb] |= (n2 - jb) << 16;
        EMIT(0x00000619);                            /* rate 6 */
        EMIT(0x00090709), EMIT(0);                   /* vars[7] = 0 */
        EMIT(0xFF00001B | B << 8);                   /* B */
        if (hold)
            EMIT(0xFFFF001B | B << 8), EMIT(328 << 16 | 0x1C); /* (6 + 328) */
        EMIT(0x0000001A), EMIT(B | 0xFF00 | I << 16); /* B -> I */
        EMIT(0xFF00001B | I << 8);                   /* I */
        c[je] |= (n2 - je) << 16;
        blind_recover = n2;
        EMIT(0x0000FF19);                            /* rate 2  } (Courier's: a jump, op 0, to 0x461, which */
        EMIT(0x00000031);                            /* anim+3 0 }  resets these on the way) */
        EMIT(0x00090709), EMIT(3);                   /* vars[7] = 3 */
        EMIT(0x0100001B | R << 8);                   /* R 0..1 */
        EMIT(0x00090709), EMIT(2);                   /* vars[7] = 2 */
        EMIT(0x0009080A), EMIT(1);                   /* vars[8] += 1 */
        EMIT(0xFF02001B | R << 8);                   /* R 2..last */
        EMIT(0x0000FF19);                            /* rate 2 */
        blind_end = n2;
        EMIT(0x00000007);                            /* the end (here: his own script again, func_8005519C) */
#undef EMIT
        far_code[0] = 0xFF00001B | I << 8; /* (I looped) */
        far_code[1] = 0xFFFF0001;
    }
    ELOG("ai: flash-bang animations: blinded %d, standing %d, recovering %d (the mission's %d; %d of Courier's put after)",
         anim_ids[0], anim_ids[1], anim_ids[2], n, extra);
    return 1;
}

/* his "my eyes!" at his place, as the script's sound command plays it (func_80027248): the mission's own at home
 * (Courier's 0x6A, Flashpoint's 0x60), else from Courier's bank, or Flashpoint's for a guard with its voice; 1 if
 * Flashpoint's */
static int flash_cry(int i, u8 *obj) {
    u8 *pos = *(u8 **)(obj + 0x90) + 0x14;
    int voice = flash_native() ? g_Args->level == 6 : call_voices[(i * 7 + obj[6]) % sizeof(call_voices)] == 6;
    if (flash_native()) {
        func_80027248(g_Args->level == 6 ? 0x60 : 0x6A, (s32)(long)pos, 0);
        return voice;
    }
    func_8003B808((s32)(long)pos);
    tone_as_game(voice ? 6 : 2, eyes_sfx[voice], 8, 0);
    return voice;
}

/* the guard seen to see Bond (the AI's look, func_80051AE0): with the lent flash bang's counter at 1, AI flag 0x40000
 * as at home, and his script's interrupt for it (Courier's) */
void func_80053804_orig(u8 *obj);
void func_80053804(u8 *obj) {
    u8 *ctx = *(u8 **)(obj + 0x118);
    int i, k, voice = 0;
    s32 d;
    func_80053804_orig(obj);
    if ((flash_native() ? (flash_vanilla() ? -1 : native_flash_count()) : flash_count) != 1 || !ctx ||
        !port_endless_room(entry))
        return; /* (at home, in a run, the same commands as lent: the mission's own interrupt for the flag is put off
                 * with the others not of Courier's kinds, its routine never runs; TWINE_ENDLESS_TEST_FLASH_VANILLA
                 * leaves the mission's own) */
    *(u32 *)ctx |= 0x40000;
    i = (int)((obj - OBJ(0)) / 0x124);
    d = *(s32 *)(obj + 0x110);
    if (flash_trace())
        port_log("trace: L%d f%d flash: #%d type %d flagged 0x40000 (distance %d, script %d)\n", g_Args->level, D_8009CDA8,
                 i, obj[6], d, obj[0xA7]);
    if (i < 0 || i >= 64)
        return;
    if (blind[i].on) { /* (his interrupt for it off meanwhile, as at home; on again from his recovering (Courier's jump
                        * to it, op 0, puts every interrupt on): the routine again) */
        u32 pc = *(u32 *)(ctx + 0xA4);
        if (blind[i].far || pc <= (u32)(long)&blind_code[blind_recover + 1] || pc > (u32)(long)&blind_code[blind_end] || d > 5000)
            return;
        *(u32 *)(ctx + 0xA4) = (u32)(long)blind_code;
        *(s32 *)(ctx + 0x1C4) = 0;
        flash_cry(i, obj);
        ELOG("ai: type %d flashed at %d again as he recovers: blinded again", obj[6], d);
        if (flash_trace())
            port_log("trace: L%d f%d flash: #%d blinded again (recovering)\n", g_Args->level, D_8009CDA8, i);
        return;
    }
    if (g_Args->level < 1 || g_Args->level > 11 || !guard_scripts[g_Args->level][obj[0xA7] & 7]) {
        ELOG("ai: type %d flashed: not a guard's script (%d), as Courier's civilians", obj[6], obj[0xA7]);
        return;
    }
    if (d > 5000 && d <= 12000) {
        ELOG("ai: type %d flashed at %d: too far to be blinded (as at home)", obj[6], d);
        return;
    }
    if (!flash_anims()) {
        ELOG("ai: type %d flashed: no animations for it", obj[6]);
        return;
    }
    blind[i].on = 1, blind[i].far = d > 12000, blind[i].since = room_frames;
    blind_now[i] = 1;
    blind[i].pc = *(u32 *)(ctx + 0xA4), blind[i].flags10 = *(u32 *)(ctx + 0x10) & 4, blind[i].items = 0;
    for (k = 0; k < 32 && (u32)k < *(u32 *)(ctx + 0xAC); k++) { /* (his interrupts: only those of the kinds Courier's
                                                                   * guards have on meanwhile, its trace says: on
                                                                   * flags 1, 4, 0x4000 (his target down), 0x10000,
                                                                   * 0x08000000; none on seeing Bond or not, a timer) */
        u32 *item = (u32 *)(ctx + 0xB0 + k * 8), *cmd = (u32 *)(long)item[1];
        if (!item[0] && cmd && (((u16 *)cmd)[1] != 1 || (cmd[1] & ~0x08014005u)))
            item[0] = 1, blind[i].items |= 1u << k;
    }
    if (flash_trace()) /* (his interrupts: on or off, their conditions and what they do) */
        for (k = 0; k < 32 && (u32)k < *(u32 *)(ctx + 0xAC); k++) {
            u32 *item = (u32 *)(ctx + 0xB0 + k * 8), *cmd = (u32 *)(long)item[1];
            if (cmd)
                port_log("trace: L%d f%d flash: #%d interrupt %d %s: %08X %08X %08X %08X%s\n", g_Args->level, D_8009CDA8,
                         i, k, item[0] ? "off" : "on", cmd[0], cmd[1], cmd[2], cmd[3],
                         blind[i].items >> k & 1 ? " (put off)" : "");
        }
    *(u32 *)(ctx + 0xA4) = (u32)(long)(blind[i].far ? far_code : blind_code);
    *(s32 *)(ctx + 0x1C4) = 0;
    if (!blind[i].far)
        voice = flash_cry(i, obj);
    ELOG("ai: type %d flashed at %d: %s%s", obj[6], d, blind[i].far ? "stands a while (as at home: far)" : "blinded, crying out",
         blind[i].far ? "" : voice ? " (Flashpoint's voice)" : " (Courier's voice)");
    if (flash_trace())
        port_log("trace: L%d f%d flash: #%d %s (pc %08X kept; interrupts off %08X; voice level %d)\n", g_Args->level,
                 D_8009CDA8, i, blind[i].far ? "far: stands 60 frames" : "blinded: Courier's routine",
                 blind[i].pc, blind[i].items, blind[i].far ? 0 : voice ? 6 : 2);
}

/* a guard back to his script where it was: his interrupts on again, +0x10 bit 4 as before */
static void blind_back(int i, u8 *ctx, const char *how) {
    int k;
    *(u32 *)(ctx + 0xA4) = blind[i].pc;
    *(s32 *)(ctx + 0x1C4) = 0;
    for (k = 0; k < 32; k++)
        if (blind[i].items >> k & 1)
            *(u32 *)(ctx + 0xB0 + k * 8) = 0;
    *(u32 *)(ctx + 0x10) = (*(u32 *)(ctx + 0x10) & ~4u) | blind[i].flags10;
    blind[i].on = blind_now[i] = 0;
    ELOG("ai: type %d back to his script after %d frames %s", OBJ(i)[6], room_frames - blind[i].since, how);
    if (flash_trace())
        port_log("trace: L%d f%d flash: #%d back to his script after %d frames (%s)\n", g_Args->level, D_8009CDA8, i,
                 room_frames - blind[i].since, how);
}

/* the script command "the end" (op 7): at the blinded routine's end, his own script again, at once (the interpreter
 * runs on while his wait is 0: as Courier's routine's last command, a jump, has him at his attack routine the same
 * frame) */
s32 func_8005519C_orig(u32 *cmd, u8 *obj, u8 *chr);
s32 func_8005519C(u32 *cmd, u8 *obj, u8 *chr) {
    int i;
    if (cmd == &blind_code[blind_end] && obj && chr && (i = (int)((chr - OBJ(0)) / 0x124)) >= 0 && i < 64 && blind[i].on &&
        *(u8 **)(chr + 0x118) == obj) {
        blind_back(i, obj, "blinded");
        return 1;
    }
    return func_8005519C_orig(cmd, obj, chr);
}

/* each frame: a far guard's 60 frames over, back to his script; one a handler of his took, let go */
static void blind_frame(void) {
    extern u8 **D_800DDEE4;
    int i;
    if (anim_extended && D_800DDEE4 != anim_ext && D_800DDEE4 == anim_orig) { /* (the mission put its own back) */
        extern u8 *D_800DDEE0;
        D_800DDEE4 = anim_ext;
        D_800DDEE0 = (u8 *)mot_ext;
    }
    if (flash_native()) /* (at home the game's own routine: the port's AI keeps off him as lent, while his animation is
                         * one of it (Courier's 78 25 49 15, Flashpoint's 85 30 63 17), from the flag on) */
        for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) {
            static const s16 own[2][4] = {{78, 25, 49, 15}, {85, 30, 63, 17}};
            u8 *o = OBJ(i), *ctx = *(u8 **)(o + 0x118), *an = *(u8 **)(o + 0x94);
            const s16 *a = own[g_Args->level == 6];
            s16 asked;
            if (!o[4] || o[5] != 6 || !ctx || !an || HEALTH(o) <= 0) {
                blind_now[i] = 0;
                continue;
            }
            if (!blind_now[i] && ((*(u32 *)ctx | *(u32 *)(ctx + 4)) & 0x40000) && *(s32 *)(o + 0x110) <= 5000 &&
                guard_scripts[g_Args->level][o[0xA7] & 7])
                blind_now[i] = 1, blind[i].since = room_frames;
            asked = *(s16 *)(an + 0x12);
            if (blind_now[i] && room_frames - blind[i].since > 2 && asked != a[0] && asked != a[1] && asked != a[2] &&
                asked != a[3])
                blind_now[i] = 0;
        }
    for (i = 0; i < 64; i++) {
        u8 *o, *ctx;
        u32 pc;
        int end;
        if (!blind[i].on)
            continue;
        o = OBJ(i);
        ctx = *(u8 **)(o + 0x118);
        if (!o[4] || HEALTH(o) <= 0 || !ctx) {
            blind[i].on = blind_now[i] = 0;
            continue;
        }
        pc = *(u32 *)(ctx + 0xA4);
        if (blind[i].far ? pc - (u32)(long)far_code >= sizeof(far_code)
                         : pc - (u32)(long)blind_code >= sizeof(blind_code)) { /* (a handler of his took him) */
            blind[i].on = blind_now[i] = 0;
            if (flash_trace())
                port_log("trace: L%d f%d flash: #%d left the routine (a handler of his script's) after %d frames: script "
                         "+%X, his flags %08X %08X %08X\n", g_Args->level, D_8009CDA8, i, room_frames - blind[i].since,
                         (pc - *(u32 *)(ctx + 0xA0)) / 4, *(u32 *)ctx, *(u32 *)(ctx + 4), *(u32 *)(ctx + 0x10));
            continue;
        }
        end = blind[i].far ? room_frames - blind[i].since >= 60 : pc == (u32)(long)&blind_code[blind_end];
        if (end) /* (the far one's time up; or the routine's end not run by the interpreter yet: as it would) */
            blind_back(i, ctx, blind[i].far ? "standing" : "blinded");
    }
}

/* THE DOUBLE-BARRELLED SHOTGUN, STRONGER IN A RUN (the playtest: undertuned for its slow reload, even in the game):
 * Courier's Frinesi shotgun (both barrels a press, 6 pellets) does twice its damage a pellet (the weapon's +0xAC, which
 * the guns' fire, func_8003872C, gives each bullet as its +0x34), at home and lent. TWINE_ENDLESS_TEST_SHOTGUN_VANILLA=1
 * leaves it as it is. */
static void shotgun_frame(void) {
    static s16 boosted[16];
    int i;
    if (getenv("TWINE_ENDLESS_TEST_SHOTGUN_VANILLA") && atoi(getenv("TWINE_ENDLESS_TEST_SHOTGUN_VANILLA")))
        return;
    for (i = 0; i < 16; i++) {
        s16 *dmg = (s16 *)((u8 *)&g_Weapons[i] + 0xAC);
        if (strncmp((char *)g_Weapons[i].pad0, "FRINESI SHOTGUN", 15) || *dmg == boosted[i] || *dmg <= 0)
            continue;
        ELOG("weapons: FRINESI SHOTGUN (slot %d): %d damage a pellet -> %d (a run's)", i, *dmg, *dmg * 2);
        *dmg = boosted[i] = *dmg * 2;
    }
}

static void flash_reset(void) {
    memset(blind, 0, sizeof(blind));
    memset(blind_now, 0, sizeof(blind_now));
    flash_count = -1;
}

static void ai_frame(void) {
    int i, n = 0, k = 0;
    s32 bx = g_Args->playerX, by = g_Args->playerY, bz = g_Args->playerZ;
    if (!ai_improved())
        return;
    if (!(room_frames % (SEC / 2)) || room_frames < 3)
        ramp_hunters();
    bond_speed = (bond_speed * 3 + dist2(bx, bz, bond_prev[0], bond_prev[2])) / 4; /* (per frame, smoothed) */
    bond_prev[0] = bx, bond_prev[2] = bz;
    if (dist2(bx, bz, bond_was[0], bond_was[2]) > 800)
        bond_was[0] = bx, bond_was[1] = by, bond_was[2] = bz, bond_still_since = room_frames;
    for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) { /* (everyone who might shoot: his reaction kept count of) */
        u8 *o = OBJ(i);
        if (o == g_Player || !o[4] || o[5] != 6 || HEALTH(o) <= 0 || !*(u8 **)(o + 0x118)) {
            react[i] = 0;
            continue;
        }
        n += hunter(o);
        if ((**(s32 **)(o + 0x118) & 0x80) || (func_80051728 && (func_80051728(o, g_Player) & 0x40)) || sight(o)) {
            /* (he sees Bond: the game's flag, its own test, or the walls say so) */
            last_seen[0] = bx, last_seen[1] = by, last_seen[2] = bz, last_seen_at = room_frames;
            if (react[i] < 255)
                react[i]++;
        } else
            react[i] = 0;
    }
    if (last_seen_at < 0 || room_frames - last_seen_at > SEC * 12 || (room_frames % (SEC * 6)) == 0) {
        /* (none has seen him yet, or long ago: they learn where he is, as by radio, now and then) */
        if (last_seen_at < 0 || room_frames % (SEC * 6) == 0)
            last_seen[0] = bx, last_seen[1] = by, last_seen[2] = bz, last_seen_at = room_frames - SEC * 9;
    }
    cover_frame();
    grenade_frame();
    drive_frame();
    callout_frame();
    if (!(room_frames % (SEC / 2))) /* SPREAD: two hunters standing in each other: the later one steps aside */
        for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) {
            u8 *o = OBJ(i);
            int j;
            if (!hunter(o) || room_frames < spread_until[i])
                continue; /* (stepping already) */
            for (j = D_800B0DA0; j <= D_800B07B0 && j < 64; j++) /* (in a hunter, or in one of the room's own) */
                if (j != i && OBJ(j)[4] && OBJ(j)[5] == 6 && HEALTH(OBJ(j)) > 0 && (!hunter(OBJ(j)) || j < i) &&
                    room_frames >= spread_until[j] && dist2(POS(o)[0], POS(o)[2], POS(OBJ(j))[0], POS(OBJ(j))[2]) < 400) {
                    s32 a = (s32)(rnd() & 0xFFF), x = POS(o)[0] + (isin(a) * 1200 >> 12), z = POS(o)[2] + (isin(a + 0x400) * 1200 >> 12);
                    int f = floor_near(x, z, POS(o)[1]), t;
                    if (dist2(POS(o)[0], POS(o)[2], POS(OBJ(j))[0], POS(OBJ(j))[2]) < 300 && spread_tries[i]++ > 0)
                        for (t = 0; t < 12; t++) { /* (still in each other after stepping: eased apart by hand, away from
                                                     * the other first, then any way) */
                            s32 ax = POS(o)[0] - POS(OBJ(j))[0], az = POS(o)[2] - POS(OBJ(j))[2], b, best2 = -0x7FFFFFFF, c2;
                            s32 nx, nz;
                            for (c2 = 0, b = 0; c2 < 4096; c2 += 64) {
                                s32 dd = (isin(c2) >> 4) * (ax >> 4) + (isin(c2 + 0x400) >> 4) * (az >> 4);
                                if (dd > best2)
                                    best2 = dd, b = c2;
                            }
                            b = (b + (t ? (s32)(rnd() % 1024) - 512 : 0)) & 0xFFF;
                            nx = POS(o)[0] + (isin(b) * 600 >> 12), nz = POS(o)[2] + (isin(b + 0x400) * 600 >> 12);
                            s32 fy = floor_at(nx, POS(o)[1] - 500, nz), from[4], to[4];
                            s16 nn[4], hh[4];
                            from[0] = POS(o)[0], from[1] = POS(o)[1] - 600, from[2] = POS(o)[2], from[3] = 0;
                            to[0] = nx, to[1] = POS(o)[1] - 600, to[2] = nz, to[3] = 0;
                            if (fy != -0x7FFF && fy - POS(o)[1] < 200 && POS(o)[1] - fy < 200 &&
                                !walls_between(D_800E2F2C, from, to, nn, hh, 0)) {
                                POS(o)[0] = nx, POS(o)[1] = fy, POS(o)[2] = nz;
                                spread_tries[i] = 0;
                                hold_ground[i] = room_frames + SEC * 20; /* (standing there a while: not walked back) */
                                if (drive_anim[i] < 0 && *(u8 **)(o + 0x94))
                                    drive_anim[i] = *(s16 *)(*(u8 **)(o + 0x94) + 0x10);
                                if (drive_anim[i] >= 0 && *(u8 **)(o + 0x94)) {
                                    u8 *an2 = *(u8 **)(o + 0x94);
                                    *(s16 *)(an2 + 0x10) = *(s16 *)(an2 + 0x12) = drive_anim[i], an2[0x14] = 0, an2[0x16] = 1;
                                }
                                ring_off[i] += 1365;
                                ELOG("ai: type %d eased apart from another", o[6]);
                                break;
                            }
                        }
                    if (f >= 0 && nav_blocked[f] <= room_frames && room_frames >= drive_rest[i]) { /* (not resting
                                                                              * from a wedge: stepped, not eased) */
                        hunt_ev[i].type = 0, hunt_ev[i].x = mid[f][0], hunt_ev[i].y = mid[f][1], hunt_ev[i].z = mid[f][2];
                        hunt_ev[i].time = D_8009CDA8 + SEC;
                        spread_until[i] = room_frames + SEC * 2;
                        drive_rest[i] = 0, made_at[i] = -SEC * 10, idle[i] = 1;
                        ELOG("ai: type %d steps aside from another", o[6]);
                    }
                    break;
                }
        }
    if (room_frames % (SEC * 5) == 0)
        ELOG("ai: %d hunters, Bond last seen %d s ago at %d %d; guards' shots %d, held %d (no sight, or reacting)", n,
             (room_frames - last_seen_at) / SEC, last_seen[0], last_seen[2], shots_fired, shots_held);
    if (room_frames % (SEC / 2))
        return;
    for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) {
        u8 *o = OBJ(i);
        s32 a, r, x, z, f, lost;
        if (!hunter(o)) {
            hunt_ev[i].type = 0x1C; /* (none) */
            continue;
        }
        if (room_frames < spread_until[i]) { /* (stepping aside: his own place a moment) */
            k++;
            continue;
        }
        lost = room_frames - last_seen_at > SEC * 8;
        a = (k * 4096 / (n ? n : 1) + (room_frames / (SEC * 20)) * 512 + ring_off[i]) & 0xFFF;
        r = room_frames - bond_still_since > SEC * 8 ? 1400 : 2200; /* (a camping Bond: closer, from all sides) */
        x = last_seen[0] + (isin(a) * r >> 12), z = last_seen[2] + (isin(a + 0x400) * r >> 12);
        if (lost && dist2(POS(o)[0], POS(o)[2], hunt_at[i][0], hunt_at[i][2]) < 900 &&
            room_frames - hunt_since[i] > SEC * 4) { /* (got there, nobody: searching about) */
            x = last_seen[0] + (s32)(rnd() % 6000) - 3000, z = last_seen[2] + (s32)(rnd() % 6000) - 3000;
            hunt_since[i] = room_frames;
        }
        f = floor_near_free(x, z, last_seen[1], i);
        if (f >= 0)
            x = mid[f][0], z = mid[f][2];
        if (((room_frames / (SEC * 3)) & 1) != (k & 1) && !lost) { /* (his half holds this turn) */
            hunt_ev[i].type = 0x1C;
            k++;
            continue;
        }
        if (dist2(x, z, hunt_at[i][0], hunt_at[i][2]) > 300)
            hunt_since[i] = room_frames;
        hunt_at[i][0] = x, hunt_at[i][1] = f >= 0 ? mid[f][1] : last_seen[1], hunt_at[i][2] = z;
        hunt_ev[i].type = 0;
        hunt_ev[i].x = x, hunt_ev[i].y = hunt_at[i][1], hunt_ev[i].z = z;
        hunt_ev[i].time = D_8009CDA8 + SEC;
        k++;
    }
}

AiEvent *func_80053ED8_orig(u8 *obj);
AiEvent *func_80053ED8(u8 *obj) { /* the event a person reacts to: a hunter's own, in IMPROVED */
    s16 e[5];
    int i;
    if (ai_on > 0 && port_endless_room(e) && obj >= OBJ(0) && obj < OBJ(64)) {
        i = (obj - OBJ(0)) / 0x124;
        if (i >= D_800B0DA0 && i <= D_800B07B0 && any_hunter(obj) && !hunter(obj) && !react[i]) {
            AiEvent *g = func_80053ED8_orig(obj); /* (one waiting his turn: at his place till he sees Bond, deaf to
                                                   * what goes on further off) */
            return g && dist2(g->x, g->z, POS(obj)[0], POS(obj)[2]) > 3000 ? 0 : g;
        }
        if (i >= D_800B0DA0 && i <= D_800B07B0 && hunt_ev[i].type == 0 && driving[i] && hunter(obj)) {
            AiEvent *g = func_80053ED8_orig(obj);
            static int told;
            if (!told++)
                ELOG("ai: hunter type %d sent to %d %d %d", obj[6], hunt_ev[i].x, hunt_ev[i].y, hunt_ev[i].z);
            return g && g->type == 0 ? g : &hunt_ev[i]; /* (a real gunshot he hears comes first) */
        }
    }
    return func_80053ED8_orig(obj);
}

/* TWINE_ENDLESS_TEST_DOORS=1 (headless tests): every door of the room, as Bond would try it once the exit is open:
 * each door zone by the floor he can reach, in turn (5 s each from frame 80): Bond put a little way before it, facing
 * it, walks into it (the pad's up) pressing use now and then; logged "doortest: door K ...: opens, to load point N" (the
 * load undone), "leaves (a locked door's ...)", "refused: \"MESSAGE\"" or "nothing" */
void func_80030FA8(s16 *at);
static s32 face(s32 fx, s32 fz, s32 x, s32 z) { /* (the yaw from (fx, fz) to (x, z)) */
    s32 dx = x - fx, dz = z - fz, a, best = -0x7FFFFFFF, ba = 0;
    for (a = 0; a < 4096; a += 8) {
        s32 d = (isin(a) >> 4) * (dx >> 4) + (isin(a + 0x400) >> 4) * (dz >> 4);
        if (d > best)
            best = d, ba = a;
    }
    return ba;
}
extern s32 D_800DF0F8;
extern u16 D_800B0C5C, D_800B0AEC;
static void steer(s32 yaw) { /* (Bond turned to yaw, as func_80030FA8 turns him) */
    *(s16 *)(*(u8 **)((u8 *)g_Player + 0x90) + 0x62) = (s16)yaw;
    gPsi = yaw, D_800DF0F8 = yaw << 6, *(s32 *)&D_800B0C5C = yaw, D_800B0AEC = (u16)yaw, g_Camera.rotY = yaw;
}
static void door_test(void) {
    s32 t = room_frames - 80, k, ph;
    int i;
    if (room_frames == 60) {
        open = 1;
        dt_n = dt_done = 0;
        flood();
        for (k = 0; k < door_count; k++) { /* (by the floor he can reach: a polygon of it near, about level) */
            s32 best = 0x7FFFFFFF, b = -1, bn = 0x7FFFFFFF, near = -1;
            for (i = 0; i < faces; i++) {
                s32 e = dist2(doors[k][0], doors[k][2], mid[i][0], mid[i][2]);
                if (!reach[i] || shut[i] || mid[i][1] - doors[k][1] > 1500 || doors[k][1] - mid[i][1] > 1500)
                    continue;
                if (e < bn)
                    bn = e, near = i;
                if (e >= 500 && e <= 1400 && e < best)
                    best = e, b = i;
            }
            if (near >= 0 && bn < 1800)
                dt_list[dt_n] = k, dt_floor[dt_n++] = b >= 0 ? b : near;
        }
        dt_list[dt_n] = -1;
        ELOG("doortest: %d doors by the floor Bond can reach (of %d door zones), use button %04X", dt_n, door_count,
             *(u16 *)((u8 *)g_Args + 0x14E));
        if (!dt_n)
            dt_list[0] = -1;
        return;
    }
    if (t >= 0 && (dt_list[0] < 0 || t / 100 >= dt_n)) { /* (the doors done: then the way out by the entry) */
        s32 e = t - dt_n * 100;
        u16 use = *(u16 *)((u8 *)g_Args + 0x14E);
        if (!dt_done++) {
            ELOG("doortest: done; to the entry");
            bond_put(entry);
        }
        port_pad_buttons[0] = (u16) ~(e % 20 == 10 ? (u16)(use << 8 | use >> 8) : 0);
        port_pad_sticks[0][3] = 0x80;
        return;
    }
    if (t < 0)
        return;
    i = t / 100, ph = t % 100, k = dt_list[i];
    if (ph == 0) {
        s16 at[5];
        s32 f = dt_floor[i];
        at[0] = D_800D2F8C, at[1] = face(mid[f][0], mid[f][2], doors[k][0], doors[k][2]);
        at[2] = mid[f][0], at[3] = mid[f][1], at[4] = mid[f][2];
        bond_put(at);
        dt_result = 0, door_msg_at = -1;
        return;
    }
    if (ph < 99) {
        if (dt_result) { /* (decided: Bond stands still the rest of his turn) */
            port_pad_buttons[0] = 0xFFFF, port_pad_sticks[0][3] = 0x80;
            return;
        }
        u16 use = *(u16 *)((u8 *)g_Args + 0x14E); /* (the game's order: the pad's two bytes swapped) */
        s32 f = dt_floor[i], ax = doors[k][0] - mid[f][0], az = doors[k][2] - mid[f][2], al = dist2(0, 0, ax, az), tx, tz, d;
        if (al < 1)
            al = 1;
        tx = doors[k][0] + ax * 800 / al, tz = doors[k][2] + az * 800 / al; /* (on through the zone, 800 past it) */
        d = dist2(g_Args->playerX, g_Args->playerZ, tx, tz);
        if (d > 150) /* (walking; there, still) */
            steer(face(g_Args->playerX, g_Args->playerZ, tx, tz));
        port_pad_buttons[0] = (u16) ~(ph % 20 == 10 ? (u16)(use << 8 | use >> 8) : 0);
        port_pad_sticks[0][3] = d > 150 ? 0x40 : 0x80; /* (the left stick forward) */
        if (getenv("TWINE_ENDLESS_TEST_DOORS_LOG") && ph % 10 == 5)
            ELOG("doortest: door %d, Bond at %d %d %d yaw %d, pad held %04X", k, g_Args->playerX, g_Args->playerY,
                 g_Args->playerZ, gPsi, g_PadHeld);
        return;
    }
    port_pad_buttons[0] = 0xFFFF, port_pad_sticks[0][3] = 0x80;
    ELOG("doortest: door %d (type %d cb %08X) at %d %d %d (load point %d), Bond from %d %d now at %d %d (%d off): %s%s%s", k,
         door_type[k], *(u32 *)(TYPE_DEF(door_type[k]) + 0x38), doors[k][0], doors[k][1], doors[k][2], door_pt[k], mid[dt_floor[i]][0], mid[dt_floor[i]][2],
         g_Args->playerX, g_Args->playerZ, dist2(g_Args->playerX, g_Args->playerZ, doors[k][0], doors[k][2]),
         dt_result ? dt_result : door_msg_at >= 0 ? "refused: \"" : "nothing",
         !dt_result && door_msg_at >= 0 ? door_msg_text : "", !dt_result && door_msg_at >= 0 ? "\"" : "");
}

static int hunters_now(void) { /* (those who would hunt, about: alive, near enough to make the fight) */
    int i, n = 0;
    for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++)
        n += any_hunter(OBJ(i)) && dist2(g_Args->playerX, g_Args->playerZ, POS(OBJ(i))[0], POS(OBJ(i))[2]) <= 12000 &&
             !no_way(i); /* (one with no way to him makes no fight) */
    return n;
}

static void draw_frame(void) { /* (the weapon he came in with taken out: each frame, from the mission's first) */
    if (draw_slot >= 0 && D_800B0DD4 == draw_slot && D_800F11F0 != 5 && D_800B0DDC == draw_slot)
        draw_slot = -1; /* (in hand: his to change from as he likes) */
    if (draw_slot >= 0 && D_800B0DD4 != draw_slot && draw_direct && D_8009CD98 &&
        !(*(u32 *)(D_8009CD98 + 0x7C) & 0x80000000)) {
        *(u32 *)(D_8009CD98 + 0x7C) |= 0x80000000; /* (the mission's own not shown while his comes: from the first
                                                     * frame, the opening iris's) */
        draw_hidden = 1;
    }
    if (draw_slot >= 0 && D_800B0DD4 != draw_slot && draw_direct && (D_800F11F0 == 1 || D_800F11F0 == 7) &&
        !D_800DDEEC) {
        /* (a room's start, a mission loaded afresh: the weapon he came in with is the one taken out, its model loaded
         * and drawn (state 5), without the mission's own first put away before it) */
#ifdef MOD_TRACE
        trace_change_by("endless: the weapon carried in, taken out at the start");
#endif
        D_800B0DD8 = D_800B0DD4;
        D_800B0DDC = (s16)draw_slot;
        if (D_8009CD98 && !(*(u32 *)(D_8009CD98 + 0x7C) & 0x80000000)) {
            *(u32 *)(D_8009CD98 + 0x7C) |= 0x80000000; /* (the mission's own not shown meanwhile) */
            draw_hidden = 1;
        }
        func_8007A17C(5);
        draw_slot = -1;
    }
    if (!D_8009CABF) /* (kept through the opening iris, till the weapon's state machine lets it be taken out) */
        draw_direct = 0;
    if (draw_hidden && draw_slot < 0 && D_800F11F0 != 5) { /* (loaded: shown, drawn) */
        draw_hidden = 0;
        if (D_8009CD98)
            *(u32 *)(D_8009CD98 + 0x7C) &= 0x7FFFFFFF;
    }
    if (draw_slot >= 0 && D_800B0DD4 != draw_slot && !draw_direct) {
#ifdef MOD_TRACE
        trace_change_by("endless: the weapon carried in, drawn");
#endif
#ifdef MOD_WEAPONKEYS
        if (weapon_change_to(draw_slot))
#endif
            draw_slot = -1;
    }
}

int port_spu_sounding(char *buf, int size);
static int trace_voices(void) {
    static int on = -1;
    if (on < 0)
        on = port_trace("voices");
    return on;
}

static void test_story_flag(void);
/* TWINE_TEST_CHEAT=action@frame,... (headless tests): in the run's first room, the game's cheat action (Cheat_DoAction:
 * what a developers' code does; 3 invincibility, 7 ammo, 8 all weapons, 0 the debug menu) at that room frame */
extern int port_endless_invincible;
void Cheat_DoAction(s32 action);
static void test_cheats(void) {
    const char *s = getenv("TWINE_TEST_CHEAT");
    if (!s || (port_endless_wave() != 1 && !getenv("TWINE_TEST_CHEAT_EVERY"))) /* (_EVERY: every room, recordings) */
        return;
    while (*s) {
        int a = atoi(s), f = strchr(s, '@') ? atoi(strchr(s, '@') + 1) : -1;
        if (f == room_frames) {
            ELOG("test: cheat action %d (as a developers' code)", a);
            Cheat_DoAction(a);
        }
        if (!strchr(s, ','))
            break;
        s = strchr(s, ',') + 1;
    }
}

/* (headless tests: TWINE_ENDLESS_TEST_PICKUPS="<level>:<room frame> <what>;...", in that mission's rooms:
 *   ammo <name>=<reserve|full>[/<clip>]  a weapon he carries (slot or pool; name a prefix, * every bullet gun) so
 *   box <name>                            an ammo box holding that weapon's kind, at Bond's feet
 *   gun <name>                            that weapon's own pickup there (a guard's drop of it)
 *   armour | frag                         heavy armour, the mission's frag grenade pickup there
 *   ticks <name>|<name>...                only these checked (as TWINE_ENDLESS_TEST_TICKS)
 *   draw <name>                           Bond changes to it) */
static int test_name(const char *nm, const char *want, int k) {
    return (k == 1 && want[0] == '*') || !strncmp(nm, want, k);
}
static void test_pickups(const char *s) {
    char cmd[16], arg[96];
    while (*s) {
        arg[0] = 0;
        int lv = atoi(s), f = strchr(s, ':') ? atoi(strchr(s, ':') + 1) : -1;
        const char *sp = strchr(s, ' ');
        if (lv == g_Args->level && f == room_frames && sp && sscanf(sp + 1, "%15[^ ;] %95[^;]", cmd, arg) >= 1) {
            int i, k = (int)strcspn(arg, "="), hit = 0;
            u8 *o = 0;
            if (!strcmp(cmd, "ammo")) {
                const char *v = arg[k] ? arg + k + 1 : "0";
                AmmoTaker t[48];
                int n = ammo_takers(t);
                for (i = 0; i < n; i++)
                    if (test_name(t[i].name, arg, k)) {
                        s32 r = !strncmp(v, "full", 4) ? t[i].cap : atoi(v), cl = strchr(v, '/') ? atoi(strchr(v, '/') + 1) : -1;
                        if (t[i].slot >= 0) {
                            Weapon *w = &g_Weapons[t[i].slot];
                            w->reserve = r;
                            if (cl >= 0)
                                w->clip = cl;
                            w->ammo = w->clip + w->reserve;
                        } else {
                            EndlessCarry *c = &port_endless_carry;
                            c->weapon[t[i].j].reserve = r;
                            if (cl >= 0)
                                c->weapon[t[i].j].clip = cl;
                            c->weapon[t[i].j].ammo = c->weapon[t[i].j].clip + r;
                        }
                        ELOG("test: %s's reserve %d%s", t[i].name, r, cl >= 0 ? ", clip set" : "");
                        hit = 1;
                    }
                for (i = 0; !hit && k > 1 && i < SLOTS; i++) /* (another weapon of his in a slot: a grenade's, say) */
                    if (!strncmp((char *)g_Weapons[i].pad0, arg, k) && (g_Weapons[i].flags & 0x800)) {
                        Weapon *w = &g_Weapons[i];
                        w->reserve = !strncmp(v, "full", 4) ? *(s16 *)&w->maxAmmo : atoi(v);
                        if (strchr(v, '/'))
                            w->clip = atoi(strchr(v, '/') + 1);
                        w->ammo = w->clip + w->reserve;
                        ELOG("test: %s's reserve %d, clip %d", (char *)w->pad0, w->reserve, w->clip);
                    }
            } else if (!strcmp(cmd, "box") || !strcmp(cmd, "gun")) {
                int kind = -1;
                for (i = 0; i < 19 && kind < 0; i++)
                    if (D_8009D130[i * 4] >= 0 && D_8009D130[i * 4] < SLOTS &&
                        !strncmp((char *)g_Weapons[D_8009D130[i * 4]].pad0, arg, strlen(arg)))
                        kind = i;
                for (i = 0; i < pool_n && kind < 0; i++)
                    if (!strncmp(pool[i].name, arg, strlen(arg)))
                        kind = pool[i].r->kind;
                if (kind < 0 || kind >= 19)
                    ELOG("test: no pickup of %s's here", arg);
                else if (cmd[0] == 'b' && (o = pickup(D_8009D108, POS(g_Player)[0], POS(g_Player)[1], POS(g_Player)[2],
                                                      "ammo box (test)")))
                    *(s32 *)(o + 0x34) = kind, ELOG("test: the box holds %s's ammo (kind %d)", arg, kind);
                else if (cmd[0] == 'g')
                    o = pickup(D_8009D130[kind * 4 + 1], POS(g_Player)[0], POS(g_Player)[1], POS(g_Player)[2], "gun (test)");
            } else if (!strcmp(cmd, "armour"))
                o = pickup(D_8009D106, POS(g_Player)[0], POS(g_Player)[1], POS(g_Player)[2], "heavy armour (test)");
            else if (!strcmp(cmd, "frag")) {
                for (i = 0; i < 19; i++)
                    if (D_8009D130[i * 4] >= 0 && D_8009D130[i * 4 + 1] > 0 &&
                        !strncmp((char *)&g_Weapons[D_8009D130[i * 4]], "FRAG GRENADE", 12))
                        break;
                if (i < 19)
                    o = pickup(D_8009D130[i * 4 + 1], POS(g_Player)[0], POS(g_Player)[1], POS(g_Player)[2], "frag grenade (test)");
                else
                    ELOG("test: no frag grenade pickup in this mission");
            } else if (!strcmp(cmd, "ticks")) {
                char t[120];
                snprintf(t, sizeof(t), "%d:%s", g_Args->level, arg);
                test_ticks(t);
            } else if (!strcmp(cmd, "draw")) {
                for (i = 0; i < SLOTS; i++)
                    if (!strncmp((char *)g_Weapons[i].pad0, arg, strlen(arg)))
                        draw_slot = i;
            }
            (void)o;
        }
        if (!strchr(s, ';'))
            break;
        s = strchr(s, ';') + 1;
    }
}

void endless_frame(void) {
    int own, n, wave, i;
    s32 kills, back;
    if (port_subframe) /* (an extra drawing's: not the mission's frame) */
        return;
    test_story_flag();
    if (port_endless_room(entry) && g_Player)
        watch_frame();
    if (trace_voices() && D_8009CAA4 && port_endless_room(entry)) { /* (the watch's menu up: what sounds meanwhile) */
        static int menu_calls;
        char what[640];
        if (++menu_calls % 25 == 1) {
            port_spu_sounding(what, sizeof(what));
            port_log("trace: L%d f%d voices: the menu up (call %d): %s\n", g_Args->level, D_8009CDA8, menu_calls, what);
        }
    }
    if (getenv("TWINE_ENDLESS_DUMP"))
        dump();
    if (!port_endless_room(entry) || !g_Player)
        return;
    if (D_8009CDA8 == last_frame && port_endless_gen() == seen_gen) /* (once a frame: the controller is read again
                                                                     * in some dialogs) */
        return;
    if (D_8009CDA8 < last_frame || last_frame < 0 || port_endless_gen() != seen_gen) { /* (a mission loaded since:
                                                                                         * its frame counter may be
                                                                                         * where the last's left off) */
        seen_gen = port_endless_gen();
        room_start();
    }
    last_frame = D_8009CDA8;
    port_endless_ungate(g_Args->level);
    port_endless_story_off(g_Args->level);
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
        if (D_8009CABF) /* (the opening iris: shown as it opens) */
            wave_due = 1;
        else
            func_80048B4C(msg_wave, 0x80, 0);
        ELOG("room ready: health %d armour %d, weapon %s, floor Bond can reach: %d polygons", bond_health(),
             D_8009CFF0, weapon_name(), flood());
        strip_tools();
        if (port_endless_invincible && !g_Invincible) { /* (invincibility turned on in a room before: still on, as
                                                         * through a mission's own checkpoints; Cheat_DoAction's 3) */
            g_Invincible = g_GodMode = 1;
            *(s16 *)((u8 *)g_Player + 0xE) = 0x7FFF;
            ELOG("cheats: invincibility kept on from the room before");
        }
        draw_weapon();
        draw_direct = !segment;
        log_weapons("as the room starts");
        /* the first room: a heavy armour ahead of Bond */
        if (port_endless_wave() == 1 && !segment && !getenv("TWINE_ENDLESS_TEST_NO_ARMOUR")) { /* (media captures) */
            s32 x = g_Args->playerX + (isin(gPsi) * 1300 >> 12), z = g_Args->playerZ + (isin(gPsi + 0x400) * 1300 >> 12);
            s32 y = floor_at(x, g_Args->playerY - 500, z);
            pickup(D_8009D106, x, y == -0x7FFF ? g_Args->playerY : y, z, "heavy armour (the first room's)");
        }
    }
    if (wave_due && !D_8009CABF) {
        wave_due = 0;
        func_80048B4C(msg_wave, 0x80, 0);
    }
    room_frames++;
    port_endless_invincible = g_Invincible != 0; /* (the debug menu's, a code's or the bonus cheat's: for the next room) */
    test_cheats();
    blind_frame(); /* (a lent flash bang's blinded guards) */
    shotgun_frame();
    hits_frame(); /* (TWINE_TRACE=hits, path: health put back, guards that hardly move) */
    if (trace_voices() && (room_frames == 1 || room_frames == 20 || room_frames == 40 || room_frames % 100 == 0)) {
        char what[640]; /* (TWINE_TRACE=voices: what sounds; a sound left on from the last mission, the playtest's
                         * endless SMG fire, shows as keyed on before this executable) */
        port_spu_sounding(what, sizeof(what));
        port_log("trace: L%d f%d voices: room frame %d: %s\n", g_Args->level, D_8009CDA8, room_frames, what);
    }
    blast_frame(); /* (a guard's grenade sparing his friends: over) */
    if (room_frames % (SEC * 10) == 0)
        flood();
    if (room_frames % (getenv("TWINE_ENDLESS_PEOPLE_FAST") ? SEC / 2 : SEC * 4) == 0)
        people_log();
    if (getenv("TWINE_ENDLESS_TEST_LOOK") && room_frames > SEC * 2) { /* (headless tests: Bond faces person N; "extra":
                                                                        * each alive one drawn from a people's slot past
                                                                        * the game's six in turn, 4 s each) */
        static int look = -1;
        u8 *o;
        if (!strcmp(getenv("TWINE_ENDLESS_TEST_LOOK"), "extra")) {
            int k, n;
            u8 *sub = look > 0 ? *(u8 **)(OBJ(look) + 0x94) : NULL;
            if (look < 0 || room_frames % (SEC * 4) == 0 || !OBJ(look)[4] || HEALTH(OBJ(look)) <= 0 || !sub || sub[0xA] < 6)
                for (k = 1, look = look < 0 ? 0 : look, n = D_800B07B0 - D_800B0DA0 + 1; k <= n; k++) {
                    int c = D_800B0DA0 + (look - D_800B0DA0 + k + n) % n;
                    u8 *q = OBJ(c), *qs = *(u8 **)(q + 0x94);
                    if (q != g_Player && q[4] && q[5] == 6 && HEALTH(q) > 0 && qs && qs[0xA] >= 6) {
                        if (c != look)
                            ELOG("test look: person %d (slot %d)", c, qs[0xA]);
                        look = c;
                        break;
                    }
                }
            o = OBJ(look > 0 ? look : 0);
        } else if (!strcmp(getenv("TWINE_ENDLESS_TEST_LOOK"), "near")) { /* ("near": the nearest person up) */
            int k;
            s32 bd = 0x7FFFFFFF;
            o = g_Player;
            for (k = D_800B0DA0; k <= D_800B07B0; k++) {
                u8 *q = OBJ(k);
                s32 d;
                if (q == g_Player || !q[4] || q[5] != 6 || HEALTH(q) <= 0)
                    continue;
                d = dist2(g_Args->playerX, g_Args->playerZ, POS(q)[0], POS(q)[2]);
                if (d < bd)
                    bd = d, o = q;
            }
        } else
            o = OBJ(atoi(getenv("TWINE_ENDLESS_TEST_LOOK")));
        if (o[4] && o != g_Player) {
            s32 dx = POS(o)[0] - g_Args->playerX, dz = POS(o)[2] - g_Args->playerZ, a, best = -0x7FFFFFFF, ba = 0;
            for (a = 0; a < 4096; a += 8) {
                s32 d = (isin(a) >> 4) * (dx >> 4) + (isin(a + 0x400) >> 4) * (dz >> 4);
                if (d > best)
                    best = d, ba = a;
            }
            if (!strcmp(getenv("TWINE_ENDLESS_TEST_LOOK"), "near")) /* (turned as the game turns him; LOOK_OFF: that far
                                                                     * to his right of the man) */
                steer((ba + (getenv("TWINE_ENDLESS_TEST_LOOK_OFF") ? atoi(getenv("TWINE_ENDLESS_TEST_LOOK_OFF")) : 0)) & 0xFFF);
            else
                gPsi = ba;
        }
    } /* (where Bond can walk to now) */
    if (getenv("TWINE_TEST_AUTOAIM")) /* (headless tests: the pause menu's auto-aim option, 1 on, 0 off) */
        g_Args->flags2 = atoi(getenv("TWINE_TEST_AUTOAIM")) ? g_Args->flags2 | 0x10 : g_Args->flags2 & ~0x10;
    /* the debug menu, the game's cheats (used, or on: g_Args +4 bits 3, 9-16) or the port's take the run off the
     * high scores */
    if (g_DevMenu[0])
        port_endless_unrank("debug menu");
    if (*(u32 *)((u8 *)g_Args + 4) & 0x1FE08) { /* (named as the CHEATS screen names them, for the stats screen) */
        static const struct { u32 bit; const char *name; } cheat[] = {
            {1 << 14, "infinite ammo"}, {1 << 15, "all weapons"}, {1 << 16, "invulnerable"}, {1 << 13, "power guns"},
            {1 << 12, "power goons"}, {1 << 10, "stealthy bond"}, {1 << 3, "a cheat code"}, {0x1FE08, "cheats"}};
        u32 f = *(u32 *)((u8 *)g_Args + 4);
        int k = 0;
        while (!(f & cheat[k].bit))
            k++;
        port_endless_unrank(cheat[k].name);
    }
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
            if (unkillable(o) && TYPE_DEF(o[6])[0x2C] && !seen(o) && !blast_active()) { /* (not while a guard's
                                                                                       * grenade spares them) */ /* (made unkillable by its script since: Electra's hit mask, say) */
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
    else if (fill_test() && !cooldown && room_frames > SEC * 2) { /* (TWINE_ENDLESS_TEST_FILL: every slot filled) */
        spawn("test fill");
        cooldown = SEC / 2;
    } else if (ramp() && hunters_now() >= hunt_cap()) /* (THE RAMP: as many hunting as the wave allows) */
        ;
    else if (!cooldown && room_frames > SEC * 2 && burst < 3 && own + burst < 3) {
        burst++;
        spawn("few guards here");
        if (cooldown < SEC * 3 / 2)
            cooldown = SEC * 3 / 2;
    } else if (!cooldown && budget >= SEC * 20 && n < 2 + wave * people_cap() / 6) {
        spawn("director");
        budget = 0;
        cooldown = SEC * 4 + rnd() % (SEC * 4);
    }
    recall();
    ai_frame();

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
    if (getenv("TWINE_ENDLESS_TEST_GRAB_BOX") && room_frames % atoi(getenv("TWINE_ENDLESS_TEST_GRAB_BOX")) == 0 && last_box[0]) {
        s16 at[5]; /* (headless tests: Bond onto the last ammo box) */
        at[0] = D_800D2F8C, at[1] = gPsi, at[2] = last_box[0], at[3] = last_box[1], at[4] = last_box[2];
        bond_put(at);
        last_box[0] = 0;
    }
    if (getenv("TWINE_ENDLESS_TEST_GRAB") && room_frames == atoi(getenv("TWINE_ENDLESS_TEST_GRAB")) && last_drop[0]) {
        s16 at[5]; /* (headless tests: Bond onto the last drop) */
        at[0] = D_800D2F8C, at[1] = gPsi, at[2] = last_drop[0], at[3] = last_drop[1], at[4] = last_drop[2];
        bond_put(at);
    }
    if (getenv("TWINE_ENDLESS_TEST_VRAM") && room_frames == 100) { /* (research: VRAM places no mission uses) */
        extern u16 gpu_vram[512 * 1024];
        static const s16 r[][4] = {{928, 320, 16, 64}, {944, 320, 16, 64}, {928, 384, 16, 64}, {944, 384, 16, 64},
                                   {960, 320, 16, 64}, {0, 508, 16, 1}, {16, 508, 16, 1}, {0, 509, 16, 1}, {32, 508, 16, 1}};
        unsigned q;
        for (q = 0; q < sizeof(r) / sizeof(r[0]); q++) {
            int x, y, used = 0;
            for (y = r[q][1]; y < r[q][1] + r[q][3]; y++)
                for (x = r[q][0]; x < r[q][0] + r[q][2]; x++)
                    used += gpu_vram[y * 1024 + x] != 0;
            ELOG("research: VRAM %d,%d %dx%d: %d used", r[q][0], r[q][1], r[q][2], r[q][3], used);
        }
    }
    if (getenv("TWINE_ENDLESS_TEST_ARM") && room_frames == 40 && /* (headless tests: every weapon, ammo; "first": in the
                                                                   * run's first room only) */
        (strcmp(getenv("TWINE_ENDLESS_TEST_ARM"), "first") || port_endless_wave() == 1)) {
        for (i = 0; i < 10; i++)
            if (g_Weapons[i].pad0[0] >= 'A' && g_Weapons[i].pad0[0] <= 'Z')
                g_Weapons[i].flags |= 0x1800, g_Weapons[i].ammo += 20, g_Weapons[i].reserve += 20;
        log_weapons("armed (test)");
    }
    if (getenv("TWINE_ENDLESS_TEST_TICKS") && room_frames == 41) /* (headless tests: <level>:<name>|<name>...: in that
                                                                   * mission's rooms, only these checked in the inventory) */
        test_ticks(getenv("TWINE_ENDLESS_TEST_TICKS"));
    if (getenv("TWINE_ENDLESS_TEST_PICKUPS"))
        test_pickups(getenv("TWINE_ENDLESS_TEST_PICKUPS"));
    if (getenv("TWINE_ENDLESS_AI_DUMP") && room_frames == SEC * 3) /* (a guard's script: its handlers and code) */
        for (i = D_800B0DA0; i <= D_800B07B0; i++) {
            u8 *o = OBJ(i), *ctx = *(u8 **)(o + 0x118);
            u32 n, k, w;
            if (!o[4] || o[5] != 6 || !ctx)
                continue;
            n = *(u32 *)(ctx + 0xAC);
            ELOG("aidump: type %d ai %d: %u handlers, code at %08X pc %08X", o[6], o[0xA7], n, *(u32 *)(ctx + 0xA0), *(u32 *)(ctx + 0xA4));
            for (k = 0; k < n && k < 24; k++) {
                u32 *h = *(u32 **)(ctx + 0xB0 + k * 8 + 4), f;
                char hex[200];
                int m = 0;
                if (!h)
                    continue;
                f = ((u16 *)h)[1];
                for (w = 0; w < 10; w++)
                    m += snprintf(hex + m, sizeof(hex) - m, "%08X ", h[w]);
                ELOG("aidump:  handler %u (on %u, cond %04X): %s", k, *(u32 *)(ctx + 0xB0 + k * 8), f, hex);
            }
            {
                u32 *c = *(u32 **)(ctx + 0xA0);
                char hex[9 * 8 + 1];
                for (k = 0; c && k < 512; k += 8) {
                    int m = 0;
                    for (w = 0; w < 8; w++)
                        m += snprintf(hex + m, sizeof(hex) - m, "%08X ", c[k + w]);
                    ELOG("aidump:  code +%03X: %s", k * 4, hex);
                }
            }
            break;
        }
    if (getenv("TWINE_ENDLESS_SOUND_LOG")) /* (who has been told of a grenade: the game's flag) */
        for (i = D_800B0DA0; i <= D_800B07B0; i++) {
            u8 *o = OBJ(i);
            if (o[4] && o[5] == 6 && *(u8 **)(o + 0x118) && (**(s32 **)(o + 0x118) & 0x04000000))
                ELOG("sound: type %d told of a grenade (flags %08X)", o[6], **(s32 **)(o + 0x118));
        }
    if (getenv("TWINE_ENDLESS_TEST_SHOOT") && room_frames >= atoi(getenv("TWINE_ENDLESS_TEST_SHOOT")) * SEC &&
        room_frames % (SEC * 2) == 0 && !blast_active()) /* (not while a guard's grenade spares them) */ /* (headless tests: everyone shot, past the game's own hit tests: who is immune?) */
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
    if (getenv("TWINE_ENDLESS_TEST_GUARD_BLAST")) { /* (headless tests: N[,bond]: at room
        * frame N a grenade's blast at the guard farthest from Bond, a guard's (another guard the thrower), or Bond's) */
        static int target = -1;
        const char *t = getenv("TWINE_ENDLESS_TEST_GUARD_BLAST");
        int at = atoi(t), bond = strstr(t, "bond") != 0;
        if (room_frames == at) {
            int owner = -1;
            s32 far = -1;
            target = -1;
            for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) {
                u8 *o = OBJ(i);
                s32 d;
                if (o == g_Player || !o[4] || o[5] != 6 || HEALTH(o) <= 0 || !TYPE_DEF(o[6])[0x2C])
                    continue;
                d = dist2(g_Args->playerX, g_Args->playerZ, POS(o)[0], POS(o)[2]);
                if (d > far)
                    far = d, target = i;
            }
            for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++)
                if (i != target && OBJ(i) != g_Player && OBJ(i)[4] && OBJ(i)[5] == 6 && HEALTH(OBJ(i)) > 0)
                    owner = i;
            if (target >= 0 && (bond || owner >= 0)) {
                s32 pos[3];
                pos[0] = POS(OBJ(target))[0], pos[1] = POS(OBJ(target))[1] - 100, pos[2] = POS(OBJ(target))[2];
                ELOG("test: a grenade's blast, %s, at person %d type %d (health %d, %d from Bond)",
                     bond ? "Bond's" : "a guard's", target, OBJ(target)[6], HEALTH(OBJ(target)), far);
                quick_grenade_test_blast(pos, bond ? 0 : OBJ(owner));
            } else
                target = -1;
        }
        if (target >= 0 && room_frames == at + SEC) {
            ELOG("test: the blast's person %d a second on: %s, health %d", target,
                 OBJ(target)[4] && HEALTH(OBJ(target)) > 0 ? "up" : "down", HEALTH(OBJ(target)));
            target = -1;
        }
    }
    if (getenv("TWINE_ENDLESS_TEST_GOD") && HEALTH(g_Player) > 0 && HEALTH(g_Player) < 0x10000)
        HEALTH(g_Player) = 0x20000; /* (headless tests: Bond doesn't fall) */
    if (getenv("TWINE_ENDLESS_TEST_DRAW") && room_frames == 60) /* (headless tests: Bond changes to the weapon named) */
        for (i = 0; i < SLOTS; i++)
            if (!strncmp((char *)g_Weapons[i].pad0, getenv("TWINE_ENDLESS_TEST_DRAW"), strlen(getenv("TWINE_ENDLESS_TEST_DRAW"))))
                draw_slot = i;
    draw_frame();
    if (getenv("TWINE_ENDLESS_AMMO_LOG") && room_frames == 41) /* (headless tests: which of his weapons drops can feed) */
        for (i = 0; i < SLOTS; i++)
            if ((g_Weapons[i].flags & 0x1800) == 0x1800 && *(s16 *)&g_Weapons[i].maxAmmo > 0)
                ELOG("ammo: %s reserve %d of %d%s", (char *)g_Weapons[i].pad0, g_Weapons[i].reserve,
                     *(s16 *)&g_Weapons[i].maxAmmo, g_Weapons[i].reserve >= *(s16 *)&g_Weapons[i].maxAmmo ? " (full)" : "");
    if (getenv("TWINE_ENDLESS_AMMO_LOG"))
        for (i = 0; i < 64; i++)
            hp_seen[i] = HEALTH(OBJ(i));
    if (getenv("TWINE_ENDLESS_AMMO_LOG") && D_800B0DD4 >= 0 && D_800B0DD4 < SLOTS) { /* (headless tests: shots fired) */
        static s32 last_clip = -1;
        if (g_Weapons[D_800B0DD4].clip != last_clip)
            ELOG("ammo: %s clip %d at frame %d", (char *)g_Weapons[D_800B0DD4].pad0, g_Weapons[D_800B0DD4].clip, room_frames);
        last_clip = g_Weapons[D_800B0DD4].clip;
        {
            extern s32 D_800F11F0;
            static s32 last_state = -99;
            extern s16 D_8009CF48;
            extern s32 D_800DDEE8;
            if (D_800DDEE8 && room_frames % 2 == 0)
                ELOG("ammo: held, timer %d, +62 %d, state %d", D_8009CF48, *(s16 *)((u8 *)&g_Weapons[D_800B0DD4] + 0x62), D_800F11F0);
            if (D_800F11F0 != last_state || (D_800F11F0 >= 0x21 && D_800F11F0 <= 0x24 && room_frames % 4 == 0))
                ELOG("ammo: weapon state %d at frame %d (view +7C %08X, aiming %d)", D_800F11F0, room_frames,
                     D_8009CD98 ? *(u32 *)((u8 *)D_8009CD98 + 0x7C) : 0, D_8009CD4C);
            last_state = D_800F11F0;
        }
    }
    text_watch();
    pool_step();
    frag_sync();
    if (lent_work_type)
        lent_ots();
    if (room_frames % SEC == 0)
        strip_tools(); /* (what the mission gives him later too: its scripts, its pickups) */
    if (room_frames == SEC * 5)
        log_weapons("five seconds in");
    if (getenv("TWINE_ENDLESS_TEST_TEXTS") && room_frames == SEC) { /* (headless tests: menu texts past the letters) */
        static char t[] = "ENDLESS TEXT TEST LINE OF THIRTY";
        int k;
        for (k = 0; k < atoi(getenv("TWINE_ENDLESS_TEST_TEXTS")); k++)
            func_8002872C((M2C_UNK *)t, 0x20, 0x20 + 8 * (k % 20));
    }
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
    if (getenv("TWINE_ENDLESS_TEST_EXIT")) { /* (N, or N,M,...: the first room at N, the second at M, ..., the
                                              * rooms after the list's end at its last) */
        const char *t = getenv("TWINE_ENDLESS_TEST_EXIT");
        int w = port_endless_wave();
        while (--w > 0 && strchr(t, ','))
            t = strchr(t, ',') + 1;
        if (room_frames == atoi(t))
            open = 1, D_800D2F90 = 2, room_frames = SEC * 60; /* (headless tests: as if Bond went through a door) */
    }
    if (getenv("TWINE_ENDLESS_TEST_SLOTS") && (room_frames == SEC * 2 || room_frames % (SEC * 10) == 0)) {
        /* (research: the people's six slots: each one's INI record (its texture chunk, its object type), taken or
         * not, its owner) */
        extern u8 D_800D2E84[], D_800DE0C8[];
        extern u8 *D_800DDEF0[];
        for (i = 0; i < 6; i++) {
            u8 *ow = D_800DDEF0[i];
            int k = ow ? (int)((ow - OBJ(0)) / 0x124) : -1;
            ELOG("slots: game slot %d: type %d (chunk %d), taken %d, owner %d%s type %d health %d at %d %d %d", i,
                 D_800D2E84[i * 0x20 + 0x1C], D_800D2E84[i * 0x20 + 1], D_800DE0C8[i], k, ow && !ow[4] ? " (gone)" : "",
                 ow ? ow[6] : -1, ow ? HEALTH(ow) : 0, ow ? POS(ow)[0] : 0, ow ? POS(ow)[1] : 0, ow ? POS(ow)[2] : 0);
        }
    }
    if (getenv("TWINE_ENDLESS_TEST_PUT") && room_frames == SEC * 2) { /* (headless tests: x,y,z,yaw: Bond put there,
                                                                     * facing that way; x,y,z,yaw;x,y,z,yaw...: the
                                                                     * first room's, the second's, ...) */
        const char *pt = getenv("TWINE_ENDLESS_TEST_PUT");
        static int put_room;
        s16 at[5];
        int n;
        for (n = put_room++; n > 0 && pt; n--)
            pt = strchr(pt, ';') ? strchr(pt, ';') + 1 : 0;
        at[0] = D_800D2F8C;
        for (n = 0; n < 4 && pt; n++, pt = strchr(pt, ',') ? strchr(pt, ',') + 1 : 0)
            at[n == 3 ? 1 : n + 2] = (s16)atoi(pt);
        bond_put(at);
        ELOG("test: Bond put at %d %d %d, yaw %d", at[2], at[3], at[4], at[1]);
    }
    if (getenv("TWINE_ENDLESS_TEST_WALK") && room_frames >= SEC && room_frames <= SEC * 5) {
        /* (headless tests: Bond walked by the left stick, a second each way: forward, back, left, right; how far he
         * got from where he stood logged: one put where he can't move shows nothing) */
        static s32 from[2], most;
        int ph = (room_frames - SEC) / SEC;
        s32 d;
        if (room_frames == SEC)
            from[0] = g_Args->playerX, from[1] = g_Args->playerZ, most = 0;
        d = dist2(g_Args->playerX, g_Args->playerZ, from[0], from[1]);
        most = d > most ? d : most;
        port_pad_sticks[0][2] = room_frames == SEC * 5 ? 0x80 : ph == 2 ? 0x40 : ph == 3 ? 0xC0 : 0x80;
        port_pad_sticks[0][3] = room_frames == SEC * 5 ? 0x80 : ph == 0 ? 0x40 : ph == 1 ? 0xC0 : 0x80;
        if (room_frames == SEC * 5)
            ELOG("test: walk: Bond moved %d at most from %d %d (now %d %d)", most, from[0], from[1], g_Args->playerX,
                 g_Args->playerZ);
    }
    if (getenv("TWINE_ENDLESS_TEST_NEAR") && room_frames == SEC * 2) { /* (headless tests: Bond put before the first
                                                                    * person of that type, facing her) */
        for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) {
            u8 *o = OBJ(i);
            s32 best = 0x7FFFFFFF, b = -1, f;
            if (o == g_Player || !o[4] || o[6] != atoi(getenv("TWINE_ENDLESS_TEST_NEAR")))
                continue;
            for (f = 0; f < faces; f++) {
                s32 d = dist2(POS(o)[0], POS(o)[2], mid[f][0], mid[f][2]) - 1300;
                if (d < 0)
                    d = -d;
                if (mid[f][1] - POS(o)[1] < 600 && POS(o)[1] - mid[f][1] < 600 && d < best)
                    best = d, b = f;
            }
            if (b >= 0) {
                s16 at[5];
                at[0] = D_800D2F8C, at[1] = face(mid[b][0], mid[b][2], POS(o)[0], POS(o)[2]);
                at[2] = mid[b][0], at[3] = mid[b][1], at[4] = mid[b][2];
                bond_put(at);
                ELOG("test: Bond put %d from person %d (type %d), facing her", dist2(mid[b][0], mid[b][2], POS(o)[0], POS(o)[2]), i, o[6]);
            }
            break;
        }
    }
    if (dt_on < 0)
        dt_on = getenv("TWINE_ENDLESS_TEST_DOORS") != 0;
    if (dt_on && !segment)
        door_test();
    if (getenv("TWINE_ENDLESS_TEST_COMPLETE") && room_frames == atoi(getenv("TWINE_ENDLESS_TEST_COMPLETE")))
        func_8002E1B0(); /* (headless tests: the mission completed then, as the last helicopter's fall does) */
    if (fanfare_done() && !(g_Args->flags & 0x40) && !loading) {
        D_8009CFFC = 1; /* (the fade out) */
        segment = 0;
        leave(wave, "the helicopter down, its fanfare played");
        return;
    }
    if (locked_exit_at >= 0 && open && !(g_Args->flags & 0x40) && !loading) {
        locked_exit_at = -1;
        if (dt_on && dt_list[0] >= 0 && !dt_done && !getenv("TWINE_ENDLESS_TEST_DOORS_LEAVE")) /* (the test: noted,
                                                                                                  * not left) */
            dt_result = "leaves (a locked door's, the exit open)";
        else {
            D_8009CFFC = 1; /* (the fade out) */
            segment = 0;
            leave(wave, dist2(g_Args->playerX, g_Args->playerZ, entry[2], entry[4]) < 1500
                            ? "the use button at the entry" /* (the door he came in by, locked behind him) */
                            : "a door the mission keeps locked, the exit open");
            return;
        }
    }
    locked_exit_at = -1;
    /* the way out by the entry (for a room whose doors lead nowhere: a story's, a lift's, or only for show): once the
     * exit is open, standing where he came in, Bond is told he may leave, and leaves with the use button */
    if (open && !(g_Args->flags & 0x40) && !loading && (!dt_on || dt_done) &&
        (dist2(g_Args->playerX, g_Args->playerZ, entry[2], entry[4]) < 1500 || far_exit())) {
        int far = dist2(g_Args->playerX, g_Args->playerZ, entry[2], entry[4]) >= 1500;
        if (!at_entry++) {
            func_80048B4C(msg_leave, 0x80, 0);
            ELOG("%s: Bond told he may leave here", far ? "far end" : "entry");
        }
        if (g_PadPressed & *(u16 *)((u8 *)g_Args + 0x14E)) {
            D_8009CFFC = 1; /* (the fade out) */
            leave(wave, far ? "the use button at the far end" : "the use button at the entry");
            return;
        }
    } else
        at_entry = 0;
    if (D_800D2F90 == 2 && !(g_Args->flags & 0x40) && !loading) {
        s16 *to = *(s16 **)(D_8009E9C4 + 8);
        int point = to ? (to - (D_8009E9C4 + 0xC)) / 8 : -1;
        back = dist2(g_Args->playerX, g_Args->playerZ, entry[2], entry[4]) < 3000 && room_frames < SEC * 60;
        if (open && !back && dt_on && dt_list[0] >= 0) { /* (the door test: noted, the load undone) */
            static char r[48];
            snprintf(r, sizeof(r), "opens, to load point %d (section %d)", point, to ? to[0] : -1);
            dt_result = r;
            D_800D2F90 = 0;
            D_8009CFFC = -1;
            *(s32 *)(D_8009E9C4 + 8) = 0;
        } else if (open && !back) { /* the room cleared */
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

/* EXTRA EXITS: a section's end the mission comes in by one way only (no door zone to leave by), a way out as the
 * entry is: once the exit is open, standing there, the use button leaves. Turncoat's first subway car (section 1):
 * its far end from the train (the mission's way down from the street), the end of the subway stretch. */
static const s32 far_exits[][4] = { /* level, section, x, z */
    {3, 1, -700, 13050},
};
static int far_exit(void) {
    int i;
    for (i = 0; i < (int)(sizeof(far_exits) / sizeof(far_exits[0])); i++)
        if (g_Args->level == far_exits[i][0] && D_800D2F8C == far_exits[i][1] &&
            dist2(g_Args->playerX, g_Args->playerZ, far_exits[i][2], far_exits[i][3]) < 1500)
            return 1;
    return 0;
}

/* THE CARRIED WEAPON FROM THE MISSION'S FIRST FRAME: endless_frame runs at the top of each frame, from Pad_Update,
 * which the mission's loop doesn't call while its opening iris plays (D_8009CABF): the room started at frame 61, the
 * mission's own weapon shown till then. func_8004900C (each frame of the mission's, before its weapon's state machine
 * runs) starts the room in the mission's first frame instead: its weapons lent, Bond's carried in, the one in his hand
 * taken out (endless_frame once; then from frame 61 on as ever) */
void func_8004900C_orig(void);
void func_8004900C(void) {
    s16 e[5];
    if (getenv("TWINE_ENDLESS_TEST_IRIS_LOG") && port_endless_room(e) && D_8009CDA8 < 70)
        ELOG("iris: frame %d, pad skipped %d, cut scene %d, weapon slot %d state %d, to %d", D_8009CDA8, D_8009CABF,
             g_CutScene, D_800B0DD4, D_800F11F0, D_800B0DDC);
    if (!port_subframe && port_endless_room(e) && g_Player && !getenv("TWINE_ENDLESS_TEST_LATE_DRAW") &&
        (port_endless_gen() != seen_gen || D_8009CDA8 < last_frame || last_frame < 0))
        endless_frame(); /* (a mission's first frame: the room started now) */
    else if (!port_subframe && port_endless_room(e) && g_Player && D_8009CABF && carried && !getenv("TWINE_ENDLESS_TEST_LATE_DRAW"))
        draw_frame(); /* (its opening iris: endless_frame waits for the controls, the carried weapon is taken out) */
    func_8004900C_orig();
}

/* THE BANK'S RECEPTIONIST (Courier, type 60, placed seated at her desk): seated, still, as in the campaign. Her
 * script (RECEPT) takes its seated loop (its animation 0x3C) once the opening cutscene is marked seen (func_80054414
 * below), and leaves it, cowering (animation 0x28, up and down: the flicker the playtest saw against a seated pose
 * forced on her each frame, now gone) when its handlers (func_80057750: run each frame against what she perceives,
 * the script's flags) see Bond with a gun in his hand (flags 0x480: 0x80 she sees him, 0x400 armed), something heard
 * (0x80000000), a fright (0x40000) or an alarm (third word 0x800). In a run Bond is always armed and the room always
 * fighting: those are not passed to her script, which keeps her seated (her being hit still is: flag 1). */
#define RECEPT_FRIGHT0 (0x80000000u | 0x40000 | 0x400 | 0x200 | 0x80)
#define RECEPT_FRIGHT4 0x800u
void func_80057750_orig(u32 *sub, u8 *obj);
void func_80057750(u32 *sub, u8 *obj) {
    s16 e[5];
    if (obj && obj[6] == 60 && g_Args->level == 2 && port_endless_room(e)) {
        s32 *src = *(s32 **)(obj + 0x60);
        u32 pc = sub[0xA4 / 4] - sub[0xA0 / 4];
        if (getenv("TWINE_ENDLESS_TEST_RECEPT_LOG")) {
            static u32 l0, l4, lpc;
            if (sub[0] != l0 || sub[4] != l4 || pc != lpc)
                ELOG("recept: f%d pc %04X flags %08X %08X %08X", D_8009CDA8, pc, sub[0], sub[2], sub[4]);
            l0 = sub[0], l4 = sub[4], lpc = pc;
        }
        if (src && (src[0] & 0x400) && !getenv("TWINE_ENDLESS_TEST_RECEPT_FREE") &&
            ((sub[0] & RECEPT_FRIGHT0) || (sub[4] & RECEPT_FRIGHT4))) {
            if (!seated_said++)
                ELOG("room: the receptionist kept seated (what would frighten her not passed to her script: flags %08X %08X, "
                     "her script at %04X)", sub[0], sub[4], pc);
            sub[0] &= ~RECEPT_FRIGHT0;
            sub[4] &= ~RECEPT_FRIGHT4;
        }
    }
    func_80057750_orig(sub, obj);
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

/* The mission completed: never in a run. But City of Walkways' helicopter room (section 3, 5:1): its last helicopter
 * down is the room's end: the mission's fanfare (its end-of-mission XA clip, func_8002B064(D_800E23E8), as the
 * end-of-mission handler plays it, without the rest of that), then the next room once it has played
 * (fanfare_done: the clip playing no longer it; or 12 s) */
void func_8002B064(s32 clip);
#define L05_END_CLIP (*(s32 *)0x800F070C) /* (LEVEL05's D_800E23E8: the mission's end-of-mission clip, 0x83) */
#define L05_CLIP_NOW (*(s32 *)0x800B4FCC) /* (LEVEL05's D_800AFEBC: the clip playing, -1 none) */
s32 func_8002E1B0(void) {
    s16 e[5];
    if (port_endless_room(e)) {
        if (g_Args->level == 5 && D_800D2F8C == 3 && fanfare_at < 0) {
            fanfare_clip = L05_END_CLIP;
            fanfare_at = room_frames;
            func_8002B064(fanfare_clip);
            ELOG("story: the helicopter down: the mission's fanfare (clip %d, CD volume %d), then the next room",
                 fanfare_clip, *(s16 *)((u8 *)g_Args + 0x142));
            return 0;
        }
        ELOG("story: the mission's completion skipped");
        return 0;
    }
    return func_8002E1B0_orig();
}
static int fanfare_done(void) { /* (the fanfare over: the room left) */
    if (fanfare_at < 0 || room_frames < fanfare_at)
        return 0;
    if (room_frames - fanfare_at < SEC / 2 || (L05_CLIP_NOW == fanfare_clip && room_frames - fanfare_at < SEC * 12))
        return 0;
    ELOG("story: the fanfare over after %d s (clip playing now %d)", (room_frames - fanfare_at) / SEC, L05_CLIP_NOW);
    fanfare_at = -1;
    return 1;
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
            made_at[i & 63] = room_frames;
            fired_by[i & 63] = 0;
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
                ELOG("made: person %d type %d %s at %d %d %d, record %08X flags %08X w4 %08X; ai %d +48 %08X +4C %08X; slot %d, "
                     "ai context %08X", i, t, how, x, y, z, (u32)&rec[3], w3, w4, o[0xA7], *(s32 *)(o + 0x48),
                     *(s32 *)(o + 0x4C), *(u8 **)(o + 0x94) ? (*(u8 **)(o + 0x94))[0xA] : -1, *(u32 *)(o + 0x118));
        }
        return o;
    }
}

/* BOND'S HITS ON PEOPLE, LOGGED (TWINE_TRACE=hits): each bullet that reaches a person (func_80034A48: the hit loop's,
 * for every projectile's hit on a person; arg2 the model's part it hit, which picks the damage: part 14 the type's
 * whole health (+0's +0x20; the head), parts 2, 5, 9, 10, 12, 13 half the bullet's damage, 3 and 6 a quarter (7, 8,
 * 11 an eighth or a quarter on an armoured one, +0x48 & 0x02000000), the rest all of it), its health before and
 * after, and whether he falls; a hit that takes nothing, with why (the game's own reasons, read before the hit as
 * func_80034A48 has them); health put back after a hit (within 10 s); one down to no health that isn't falling a
 * second on; and after every 5th hit a person survives, what he has taken. The lines:
 *   trace: L<l> f<frame> hits: person <slot> type <t> (<sent|room's>) hit by <weapon> in part <p> (<share>): damage
 *          <d> (the bullet's <b>), health <h0> -> <h1>[, dies]
 *   trace: ... hits: person ... hit by ... in part ...: NO DAMAGE, health <h0> -> <h1>: <reason>; +0E .. +48 .. +4C ..
 *          (no-kill flag on|off) +A4 .. ai .. state ..
 *   trace: ... hits: person ... health put back <h0> -> <h1>, <n> frames after Bond's hit; ...
 *   trace: ... hits: person ... down to health <h> a second ago but not falling; ...
 *   trace: ... hits: person ... <n> hits and still up: <k> hurt him (<total> damage), <z> did nothing; health <h> of
 *          the type's <full>; ...
 * And sent guards that hardly move (TWINE_TRACE=hits or path): each 3 s a sent guard hunting Bond (THE RAMP lets him)
 * moved under 300, unless he stands seeing Bond and not running:
 *   trace: ... path: sent guard <slot> type <t> moved <d> in 3 s, at x y z (made <n> s ago): the port walking him
 *          <0|1>, sees Bond <0|1>, animation asked <a> running <r> (run <ra>), ai <script> state <+24> combat <vars[7]>
 * (Names are the port's own guesses where the game's are unknown; read with the code.) */
void func_80034A48_orig(u8 *o, u8 *proj, s16 part, s16 arg3);
static int hits_on = -1, path_on;
static int hits_trace(void) {
    if (hits_on < 0)
        hits_on = port_trace("hits"), path_on = hits_on || port_trace("path");
    return hits_on;
}
static s32 hit_key[64], hit_n[64], hit_hurt[64], hit_dmg[64], hit_after[64], hit_at[64], hit_down[64];
static s32 path_at[64][3], path_check[64];
static s32 hit_person_key(int i, u8 *o) { /* (a new person in the slot: his counts begin again) */
    return (made_at[i] + 1) * 0x101 ^ o[6] << 24 ^ port_endless_wave() << 16 ^ (s32)(long)made_how[i];
}
static const char *part_share(int part, u8 *o) {
    if (part == 14)
        return "the head: his whole health";
    if ((part == 7 && (*(s32 *)(o + 0x48) & 0x02000000)))
        return "an eighth (armoured)";
    if (((part == 8 || part == 11) && (*(s32 *)(o + 0x48) & 0x02000000)) || part == 3 || part == 6)
        return "a quarter";
    if (part == 2 || part == 5 || part == 9 || part == 12 || part == 10 || part == 13)
        return "half";
    return "all";
}
static void person_flags(char *b, int n, u8 *o) {
    u8 *ctx = *(u8 **)(o + 0x118), *st = *(u8 **)(o + 0x94);
    snprintf(b, n, "+0E %04X +48 %08X +4C %08X (no-kill flag %s) +A4 %02X, ai %d state %08X combat %d", *(u16 *)(o + 0xE),
             *(s32 *)(o + 0x48), *(s32 *)(o + 0x4C), *(s32 *)(o + 0x4C) & 0x40000000 ? "ON" : "off", o[0xA4], o[0xA7],
             st ? *(s32 *)(st + 0x24) : -1, ctx ? *(s32 *)(ctx + 0x3C) : -1);
}
void func_80034A48(u8 *o, u8 *proj, s16 part, s16 arg3) {
    s32 before, after, bullet;
    int i, bond;
    u8 *by, *ctx;
    char why[96], fl[160], weap[40];
    s16 e5[5];
    if (!hits_trace() || port_subframe || !o || !proj || (u8 *)o < OBJ(0) || (u8 *)o >= OBJ(64) || o == (u8 *)g_Player ||
        o[5] != 6) {
        func_80034A48_orig(o, proj, part, arg3);
        return;
    }
    i = (int)(((u8 *)o - OBJ(0)) / 0x124);
    before = HEALTH(o), bullet = *(s32 *)(proj + 0x34), by = *(u8 **)(proj + 0x88), bond = by == (u8 *)g_Player;
    ctx = *(u8 **)(o + 0x118);
    why[0] = 0;
    if (*(u16 *)(o + 0xE) == 0x7FFF)
        snprintf(why, sizeof(why), "immune (his hit mask 7FFF)%s", blast_active() ? ", a guard's grenade's second" : "");
    else if (before <= 0)
        snprintf(why, sizeof(why), "already down (no health)");
    else if (!bond && !(ctx && (*(s32 *)(ctx + 0x10) & 0x200)))
        snprintf(why, sizeof(why), "not Bond's bullet (his AI takes no damage from others': its +0x10 & 0x200 clear)");
    person_flags(fl, sizeof(fl), o);
    func_80034A48_orig(o, proj, part, arg3);
    after = HEALTH(o);
    if (bond)
        snprintf(weap, sizeof(weap), "%s", D_800B0DD4 >= 0 && D_800B0DD4 < 16 ? (char *)g_Weapons[D_800B0DD4].pad0 : "?");
    else if (by >= OBJ(0) && by < OBJ(256))
        snprintf(weap, sizeof(weap), "object %d (type %d)'s shot", (int)((by - OBJ(0)) / 0x124), by[6]);
    else
        snprintf(weap, sizeof(weap), "a shot of nobody's");
    if (hit_key[i] != hit_person_key(i, o))
        hit_key[i] = hit_person_key(i, o), hit_n[i] = hit_hurt[i] = hit_dmg[i] = 0, hit_down[i] = 0;
    if (!bond && after >= before && why[0]) /* (a guard's stray shot on a friend: the game's way, not logged) */
        return;
    hit_n[i]++;
    hit_at[i] = room_frames, hit_after[i] = after;
    if (after < before) {
        hit_hurt[i]++, hit_dmg[i] += before - after;
        port_log("trace: L%d f%d hits: person %d type %d (%s) hit by %s in part %d (%s): damage %d (the bullet's %d), "
                 "health %d -> %d%s\n", g_Args->level, D_8009CDA8, i, o[6], is_copy(o) ? "sent" : "room's", weap, part,
                 part_share(part, o), before - after, bullet, before, after, after <= 0 ? ", dies" : "");
    } else {
        if (!why[0])
            snprintf(why, sizeof(why), after > before ? "health went up" : "the damage worked out as nothing (part %d's share of %d)",
                     part, bullet);
        port_log("trace: L%d f%d hits: person %d type %d (%s) hit by %s in part %d (%s): NO DAMAGE, health %d -> %d: %s; %s%s%s\n",
                 g_Args->level, D_8009CDA8, i, o[6], is_copy(o) ? "sent" : "room's", weap, part, part_share(part, o), before,
                 after, why, fl, g_Invincible ? "; g_Invincible on" : "", (g_Args->flags2 >> 13 & 1) ? "; 4x damage cheat on" : "");
    }
    if (after > 0 && hit_n[i] % 5 == 0) {
        s32 full = *(u8 **)o ? *(s32 *)(*(u8 **)o + 0x20) : 0;
        port_log("trace: L%d f%d hits: person %d type %d (%s, %s %d s ago) %d hits and still up: %d hurt him (%d damage), "
                 "%d did nothing; health %d of the type's %d; %s\n", g_Args->level, D_8009CDA8, i, o[6],
                 is_copy(o) ? "sent" : "room's", made_how[i] ? made_how[i] : "made", port_endless_room(e5) ? (room_frames - made_at[i]) / SEC : -1,
                 hit_n[i], hit_hurt[i], hit_dmg[i], hit_n[i] - hit_hurt[i], after, full, fl);
    }
}
/* each frame of a room: health put back after a hit, one down not falling; sent guards that hardly move */
static void hits_frame(void) {
    int i;
    char fl[160];
    hits_trace();
    if ((!hits_on && !path_on) || port_subframe)
        return;
    for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) {
        u8 *o = OBJ(i), *an, *ctx;
        if (o == (u8 *)g_Player || !o[4] || o[5] != 6)
            continue;
        if (hits_on && hit_key[i] == hit_person_key(i, o) && hit_n[i] && room_frames - hit_at[i] <= SEC * 10) {
            if (HEALTH(o) > hit_after[i]) {
                person_flags(fl, sizeof(fl), o);
                port_log("trace: L%d f%d hits: person %d type %d (%s) health put back %d -> %d, %d frames after Bond's hit; %s\n",
                         g_Args->level, D_8009CDA8, i, o[6], is_copy(o) ? "sent" : "room's", hit_after[i], HEALTH(o),
                         room_frames - hit_at[i], fl);
                hit_after[i] = HEALTH(o);
            } else
                hit_after[i] = HEALTH(o);
            if (HEALTH(o) <= 0 && !(o[0xA4] & 2) && room_frames - hit_at[i] == SEC && !hit_down[i]) {
                person_flags(fl, sizeof(fl), o);
                hit_down[i] = 1;
                port_log("trace: L%d f%d hits: person %d type %d (%s) down to health %d a second ago but not falling; %s\n",
                         g_Args->level, D_8009CDA8, i, o[6], is_copy(o) ? "sent" : "room's", HEALTH(o), fl);
            }
        }
        if (!path_on)
            continue;
        if (!is_copy(o) || HEALTH(o) <= 0 || !hunter(o) || room_frames - made_at[i] < SEC) {
            path_check[i] = 0;
            continue;
        }
        if (!path_check[i] || path_check[i] > room_frames) {
            path_check[i] = room_frames, path_at[i][0] = POS(o)[0], path_at[i][1] = POS(o)[1], path_at[i][2] = POS(o)[2];
            continue;
        }
        if (room_frames - path_check[i] < SEC * 3)
            continue;
        {
            s32 moved = dist2(POS(o)[0], POS(o)[2], path_at[i][0], path_at[i][2]), run = run_anim(o[6]);
            int sees;
            an = *(u8 **)(o + 0x94), ctx = *(u8 **)(o + 0x118);
            sees = (ctx && (*(s32 *)ctx & 0x80)) || seen_sight[i];
            if (moved < 300 && an && (!sees || *(s16 *)(an + 0x12) == run || driving[i]))
                port_log("trace: L%d f%d path: sent guard %d type %d moved %d in 3 s, at %d %d %d (made %d s ago): the "
                         "port walking him %d, sees Bond %d, animation asked %d running %d (run %d), ai %d state %08X "
                         "combat %d\n", g_Args->level, D_8009CDA8, i, o[6], moved, POS(o)[0], POS(o)[1], POS(o)[2],
                         (room_frames - made_at[i]) / SEC, driving[i], sees, *(s16 *)(an + 0x10), *(s16 *)(an + 0x12), run,
                         o[0xA7], *(s32 *)(an + 0x24), ctx ? *(s32 *)(ctx + 0x3C) : -1);
            path_check[i] = room_frames, path_at[i][0] = POS(o)[0], path_at[i][1] = POS(o)[1], path_at[i][2] = POS(o)[2];
        }
    }
}

/* A person hit: one that is hit again and again without being hurt (a story person the game won't let die,
 * under a guard's look) is taken out of the room */
void func_8005E020(u8 *o) {
    s16 e[5];
    if (getenv("TWINE_ENDLESS_AMMO_LOG") && port_endless_room(e) && (u8 *)o >= OBJ(0) && (u8 *)o < OBJ(64) && o != g_Player &&
        D_800B0DD4 >= 0) {
        int i = (int)(((u8 *)o - OBJ(0)) / 0x124);
        if (hp_seen[i] > HEALTH(o))
            ELOG("hit: person %d by %s: %d damage", i, (char *)g_Weapons[D_800B0DD4].pad0, hp_seen[i] - HEALTH(o));
        hp_seen[i] = HEALTH(o);
    }
    func_8005E020_orig(o); /* (its old backstop, people hit unhurt taken out, took ordinary guards: gone) */
}

/* A pickup touched by Bond: logged when taken */
s32 func_8004CA5C(u8 *o, u8 *who) {
    s32 type = o[6], health = HEALTH(g_Player), armour = D_8009CFF0, ammo = D_800B0DD4 >= 0 ? g_Weapons[D_800B0DD4].ammo : 0;
    s32 before[16], i, n = SLOTS < 16 ? SLOTS : 16, r;
    s16 e[5];
    for (i = 0; i < n; i++)
        before[i] = g_Weapons[i].ammo;
    if (port_endless_room(e) && *(s32 *)(who + 0x34) > 0 && !getenv("TWINE_ENDLESS_TEST_AMMO_OLD") &&
        (r = ammo_pickup(o)) != 0) /* (TWINE_ENDLESS_TEST_AMMO_OLD=1: the old way, the box's kind only) */
        return r == 1;
    if (port_endless_room(e) && *(s32 *)(who + 0x34) > 0 && pool_box(o))
        return 1;
    r = func_8004CA5C_orig(o, who);
    if (r && !port_subframe && port_endless_room(e)) {
        ELOG("pickup taken: type %d: health %d -> %d, armour %d -> %d, ammo in hand %d -> %d", type, health,
             HEALTH(g_Player), armour, D_8009CFF0, ammo, D_800B0DD4 >= 0 ? g_Weapons[D_800B0DD4].ammo : 0);
        for (i = 0; i < n; i++)
            if (g_Weapons[i].ammo != before[i])
                ELOG("pickup: %s's ammo %d -> %d%s", (char *)g_Weapons[i].pad0, before[i], g_Weapons[i].ammo,
                     lent_mask >> i & 1 ? " (a lent weapon)" : "");
    }
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
    if (g_Player && damage > 0 && !port_subframe && port_endless_room(e) && HEALTH(g_Player) < health && HEALTH(g_Player) > 0)
        callout_hit();
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
static int door_refusal(const char *t) { /* (the missions' messages for a door that won't open) */
    if (strstr(t, "UNLOCKED")) /* ("DOOR UNLOCKED", "ALREADY UNLOCKED": a door opening) */
        return 0;
    return strstr(t, "LOCKED") || strstr(t, "DENIED") || strstr(t, "CANNOT REACH LOCK") || strstr(t, "UNLOCK DOOR") ||
           strstr(t, "LOCATOR CARD") || strstr(t, "MUST BE OFFLINE") || /* (Flashpoint 6:13's channel doors) */
           (!strncmp(t, "NEED ", 5) && (strstr(t, "CARD") || strstr(t, "KEY") ||
                                                                     strstr(t, "FINGERPRINT")));
}
/* City of Walkways' gas valve (its zone's handler, LEVEL05's func_8006DF58: a use button there is told "GAS VALVE
 * JAMMED!" / "USE THE WRENCH"; only the wrench's blow works it): in a run the use button works it, as the blow does
 * but for Bond's turn to it: the gas on (valve state D_800A3320 0 or 3 -> 2, the jet's ticks D_800A3498 200, the
 * times it was turned D_800A3334 + 1; its note at the second turn left out); already on, nothing */
static s32 valve_at = -1;
static int valve_use(const char *text) {
    s32 *state = (s32 *)0x800A3320;
    if (g_Args->level != 5)
        return 0;
    if (!strcmp(text, "USE THE WRENCH"))
        return valve_at == room_frames;
    if (strcmp(text, "GAS VALVE JAMMED!"))
        return 0;
    if (valve_at != room_frames && (*state == 0 || *state == 3)) {
        *state = 2, *(s32 *)0x800A3498 = 200, (*(s32 *)0x800A3334)++;
        ELOG("story: the gas valve turned on by the use button (no wrench)");
    }
    valve_at = room_frames;
    return 1;
}

s32 func_80048B4C(char *text, u8 prio, u8 flag) {
    s16 e[5];
    s32 r;
    if (text && port_endless_room(e) && !port_subframe && valve_use(text))
        return 0;
    if (text && port_endless_room(e) && text != msg_locked && text != msg_open && text != msg_back &&
        text != msg_leave && door_refusal(text) && !port_subframe) {
        /* a door the mission keeps shut: once the exit is open, a way on (endless_frame leaves); before, the room's
         * own "EXIT LOCKED" */
        int keep = getenv("TWINE_ENDLESS_TEST_NO_LOCKED_EXIT") != 0; /* (tests: the mission's way, as before) */
        if (door_msg_at != room_frames)
            ELOG("door: the mission's \"%s\" (%s)", text, keep ? "kept (test)" : open ? "the exit open: Bond leaves by it" : "exit locked");
        door_msg_at = room_frames;
        snprintf(door_msg_text, sizeof(door_msg_text), "%s", text);
        if (!keep && open) {
            locked_exit_at = room_frames;
            return 0;
        }
        if (!keep)
            text = msg_locked;
    }
    if (text && port_endless_room(e)) {
        int story = prio != 0x80 || scripted || strstr(text, "MISSION") || strstr(text, "FAILED") || strstr(text, "ESCAPED") ||
                    strstr(text, "OBJECTIVE") || strstr(text, "ALARM") ||
                    strstr(text, "FOR MESSAGE"); /* (Courier's "PRESS SELECT FOR MESSAGE": MI6's message, its alert off) */
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

/* FAIRER SHOOTING (IMPROVED): a guard's shot (func_80053114: his gun fires at his target) only with Bond in his
 * sight through the section's walls, not in the first half second he has seen him (a reaction), and less sure the
 * further off Bond is and the faster he moves: the game spreads a guard's shot by (10 - his aim), his aim the high
 * half of his animation record's first byte, which is lowered for the shot (1 past 8000, 2 past 12000, 1 more for a
 * running Bond). */
void func_80053114_orig(u8 *obj);
static int ramp_shot(int i) { /* (THE RAMP's shooting: may guard i fire now?) */
    int w = port_endless_wave(), j, n = 0;
    s32 len = w >= 5 ? SEC : SEC / 2, rest = w >= 6 ? SEC / 2 : SEC * 2 - (w - 1) * SEC * 3 / 10;
    if (!ramp() || shoot_cap() >= 64)
        return 1;
    if (room_frames < shot_rest[i])
        return 0;
    if (!shot_burst[i] || room_frames - shot_last[i] > SEC / 2) { /* (a new burst: if few others are in one) */
        for (j = D_800B0DA0; j <= D_800B07B0 && j < 64; j++)
            n += j != i && OBJ(j)[4] && HEALTH(OBJ(j)) > 0 && shot_burst[j] && room_frames - shot_last[j] <= SEC / 2;
        if (n >= shoot_cap())
            return 0;
        shot_burst[i] = room_frames + 1;
    }
    if (room_frames + 1 - shot_burst[i] > len) { /* (his burst over: a rest) */
        shot_rest[i] = room_frames + rest;
        shot_burst[i] = 0;
        return 0;
    }
    shot_last[i] = room_frames;
    return 1;
}
void func_80053114(u8 *obj) {
    s16 e[5];
    u8 *an, keep;
    int i, pen = 0;
    s32 d;
    if (obj != g_Player && obj >= OBJ(0) && obj < OBJ(64) && obj[5] == 6 && port_endless_room(e) &&
        (!*(u8 **)(obj + 0x5C) || *(u8 **)(obj + 0x5C) == g_Player) && !ramp_shot((obj - OBJ(0)) / 0x124 & 63)) {
        shots_held++; /* (THE RAMP: his turn to shoot not come, or resting) */
        return;
    }
    if (ai_on <= 0 || obj == g_Player || obj[5] != 6 || !port_endless_room(e) ||
        (*(u8 **)(obj + 0x5C) && *(u8 **)(obj + 0x5C) != g_Player) ||
        !(an = *(u8 **)(obj + 0x94))) {
        if (obj != g_Player && (u8 *)obj >= OBJ(0) && (u8 *)obj < OBJ(64))
            fired_by[(obj - OBJ(0)) / 0x124]++;
        func_80053114_orig(obj);
        return;
    }
    i = (obj - OBJ(0)) / 0x124 & 63;
    if (!((func_80051728 && (func_80051728(obj, g_Player) & 0x40)) || sight(obj))) {
        /* (the game's own test of his seeing Bond, walls and all; Meltdown has none: the section's walls) */
        shots_held++;
        return;
    }
    if (react[i] < SEC * 2 / 5) { /* (only just seen him: some 0.4 s to react) */
        shots_held++;
        return;
    }
    d = dist2(POS(obj)[0], POS(obj)[2], g_Args->playerX, g_Args->playerZ);
    pen = (d > 12000 ? 2 : d > 8000 ? 1 : 0) + (bond_speed > 60 ? 1 : 0);
    keep = an[0];
    if ((keep >> 4) < 10) /* (a sure shot, 10, the game's own, is left so) */
        an[0] = (u8)(((keep >> 4) > pen ? (keep >> 4) - pen : 0) << 4 | (keep & 0xF));
    func_80053114_orig(obj);
    an[0] = keep;
    shots_fired++;
    fired_by[i]++;
}

/* A script's sound at its person (op 0x1D: func_800572F4, a sound of the level's bank at him; the guards' calls and
 * shouts among them): logged (the AI's shouts are found by it) */
static int muted_line(u32 id);
s32 func_800572F4_orig(u32 *cmd, u8 *obj, u8 *chr);
s32 func_800572F4(u32 *cmd, u8 *obj, u8 *chr) {
    s16 e[5];
    if (obj && chr && port_endless_room(e) && getenv("TWINE_ENDLESS_SOUND_LOG"))
        ELOG("sound: script of type %d (ai %d) plays sound %d (flags %08X)", chr[6], chr[0xA7],
             (*cmd >> 16 & 3) == 1 ? -1 : (int)(*cmd >> 8 & 0xFF), *(u8 **)(chr + 0x118) ? **(s32 **)(chr + 0x118) : 0);
    if (obj && chr && port_endless_room(e)) {
        u32 id = (*cmd >> 16 & 3) == 1 ? ((u8 *)obj + 0x20)[(*cmd >> 8) & 0xFF] : (*cmd >> 8 & 0xFF);
        if (muted_line(id)) {
            SfxDefE *d = *(SfxDefE **)((u8 *)g_Args + 0x130);
            u8 vol = d[id].vol;
            s32 r;
            d[id].vol = 0; /* (silent: the game plays nothing at no volume) */
            r = func_800572F4_orig(cmd, obj, chr);
            d[id].vol = vol;
            ELOG("sound: story line %u muted", id);
            return r;
        }
    }
    return func_800572F4_orig(cmd, obj, chr);
}

/* A guard's grenade going off (quickgrenade.c): the people (Bond aside) not hurt by it (a guard's grenade is for Bond,
 * not his friends). The game's own way: hit mask bit 0x400, which keeps off a blast or a projectile that isn't Bond's
 * (func_80037B6C's blasts check the blast's maker, +0x88 of the object it was made for: this one's the guard; and
 * func_80037768's shots), set on everyone for the blast's second and taken off after (a mask that had it keeps it).
 * Bond's bullets (func_80034A48: no mask bit but 0x7FFF) and his own blasts (0x800 only) still hurt them: before, the
 * masks were made 0x7FFF for that second, and nothing Bond did could hurt anyone meanwhile. The people near it are
 * counted, and how many are still up as the second ends. TWINE_TEST_OLD_BLAST=1: the old 0x7FFF (tests). */
static u16 blast_masks[64];
static u8 blast_near[64];
static s32 blast_until = -1, blast_at[3];
void endless_enemy_blast(s32 *pos) {
    int i, n = 0, old = getenv("TWINE_TEST_OLD_BLAST") != NULL;
    if (blast_until >= 0)
        return;
    for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) {
        u8 *o = OBJ(i);
        blast_near[i] = 0;
        if (o == g_Player)
            continue;
        blast_masks[i] = *(u16 *)(o + 0xE);
        *(u16 *)(o + 0xE) = old ? 0x7FFF : blast_masks[i] | 0x400;
        if (pos && o[4] && o[5] == 6 && HEALTH(o) > 0 && dist2(pos[0], pos[2], POS(o)[0], POS(o)[2]) < 4000 &&
            POS(o)[1] - pos[1] < 1500 && pos[1] - POS(o)[1] < 1500)
            blast_near[i] = 1, n++;
    }
    if (pos)
        blast_at[0] = pos[0], blast_at[1] = pos[1], blast_at[2] = pos[2];
    blast_until = D_8009CDA8 + SEC;
    ELOG("ai: a guard's grenade goes off at %d %d %d: his friends spared from it till frame %d (%s), %d people within "
         "4000 of it", blast_at[0], blast_at[1], blast_at[2], blast_until,
         old ? "hit masks 7FFF: the old way, nothing hurts them" : "hit masks +0x400: not hurt by others' blasts and shots, Bond's still hurt", n);
}

static int blast_active(void) {
    return blast_until >= 0;
}

static void blast_frame(void) {
    int i, near = 0, up = 0, n = 0;
    char who[160];
    who[0] = 0;
    if (blast_until < 0 || (D_8009CDA8 < blast_until && D_8009CDA8 >= blast_until - SEC * 2))
        return; /* (the frame counter going back: a new mission, put back at once) */
    for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++)
        if (OBJ(i) != g_Player) {
            u16 *m = (u16 *)(OBJ(i) + 0xE);
            if (getenv("TWINE_TEST_OLD_BLAST"))
                *m = blast_masks[i];
            else if (*m != 0x7FFF) /* (only the bit, the script's changes meanwhile kept) */
                *m = (*m & ~0x400) | (blast_masks[i] & 0x400);
            if (blast_near[i]) {
                near++;
                up += OBJ(i)[4] && HEALTH(OBJ(i)) > 0;
                if (n < 140)
                    n += snprintf(who + n, sizeof(who) - n, " %d%s", i, OBJ(i)[4] && HEALTH(OBJ(i)) > 0 ? "" : "(down)");
            }
        }
    blast_until = -1;
    ELOG("ai: the people's hit masks put back: of the %d near the blast (people%s), %d still up", near, who, up);
}

/* The first-person model loader (a weapon change: func_80076014, func_80076104 without the disc archive): a model
 * of another mission's (a lent weapon's, "Lnn/NAME.HMD" naming that mission) read from that mission's archive on
 * the disc (Lnn/LVLnnCD.BIN: entries of 0x1C bytes, name and first sector), kept after the first read */
static struct {
    char path[24];
    u8 *data;
    u32 size;
} donor_hmd[8];

void port_smooth_keep(void *at, u32 size);
void port_lent_images(const void *at, u32 size);
void port_lent_ot(int k, const void *org, u32 tags);
extern u8 D_800D2AFC[][0x14], D_800B8F88[][0x14]; /* (the weapon in hand's ordering tables, the frame's) */

/* the lent model's textures go into VRAM only while it's drawn: where its tags and the frame's are */
static void lent_ots(void) {
    int k;
    for (k = 0; k < 2; k++) {
        port_lent_ot(k, *(void **)(D_800D2AFC[k] + 4), 1u << *(u32 *)D_800D2AFC[k]);
        port_lent_ot(2 + k, *(void **)(D_800B8F88[k] + 4), 1u << *(u32 *)D_800B8F88[k]);
    }
}

static int lent_model(s32 idx) {
    u8 t = ((u8 *)&g_Weapons[idx])[0x5B];
    const char *p = t ? D_8009DB50[t] : 0;
    int k, lv;
    if (!p || p[0] != 'L' || p[3] != '/' || !(lent_mask & 1 << idx) || (watch_on && idx == WATCH_SLOT))
        return 0;
    lv = (p[1] - '0') * 10 + (p[2] - '0');
    if (lv == g_Args->level && !getenv("TWINE_ENDLESS_TEST_SELFLEND"))
        return 0;
    for (k = 0; k < 8 && donor_hmd[k].data && strncmp(donor_hmd[k].path, p, 23); k++)
        ;
    if (k == 8)
        k = 0, free(donor_hmd[0].data), donor_hmd[0].data = 0;
    if (!donor_hmd[k].data) { /* (the archive's directory, then the file alone: reading the whole archive, megabytes,
                                 * stalled the weapon change) */
        char arcname[32];
        u8 *arc, *e, *data;
        u32 n;
        snprintf(arcname, sizeof(arcname), "L%02d/LVL%02dCD.BIN", lv, lv);
        if (!(n = port_disc_part(arcname, 0, 0x4000, &arc)))
            return 0;
        for (e = arc; e + 0x38 <= arc + n && *e; e += 0x1C)
            if (!strncmp((char *)e, p, 24)) {
                u32 at = *(u16 *)(e + 26) * 0x800u, end = *(u16 *)(e + 26 + 0x1C) * 0x800u;
                u32 size = end > at ? end - at : 0;
                if (size > sizeof(lent_work))
                    size = sizeof(lent_work);
                if (size && (size = port_disc_part(arcname, at, size, &data))) {
                    donor_hmd[k].data = data;
                    donor_hmd[k].size = size;
                    snprintf(donor_hmd[k].path, sizeof(donor_hmd[k].path), "%s", p);
                }
                break;
            }
        free(arc);
        if (!donor_hmd[k].data) {
            ELOG("weapons: %s not found on the disc", p);
            return 0;
        }
    }
    /* (into a buffer of the mod's, as big as the biggest: some missions' own is smaller than another's models,
     * Courier's 0xD800 bytes to the AR36's 0xD860; the model is made from it: func_800753F0) */
    memcpy(lent_work, donor_hmd[k].data, donor_hmd[k].size);
    port_smooth_keep(lent_work, sizeof(lent_work));
    port_lent_images(lent_work, sizeof(lent_work)); /* (its textures kept by the runtime: libgpu.c) */
    lent_ots();
    lent_work_type = t;
    snprintf(lent_work_path, sizeof(lent_work_path), "%s", donor_hmd[k].path);
    D_800F0F5C = 0;
    D_800E2F58 = t;
    return 1;
}

/* (mods/trace.c) the first-person model's file loaded: a lent one's, or the mission's own for the type loaded */
const char *endless_model_file(void) {
    if (lent_work_type && D_800E2F58 == lent_work_type)
        return lent_work_path;
    return D_800E2F58 ? D_8009DB50[D_800E2F58] : "-";
}

void *func_800753F0_orig(s32 type);
void *func_800753F0(s32 type) {
    if (lent_work_type && (type & 0xFF) == lent_work_type && D_800E2F58 == lent_work_type)
        return lent_work;
    return func_800753F0_orig(type);
}

/* A lent weapon's fire sound, its own mission's (a sound id of its own, past the mission's table: LENT_SOUND):
 * its tone decoded from that mission's bank and played by the runtime (port/src/tones.c), as the game plays its
 * sounds (func_80026F30: the volume the table and the settings give, its note); an automatic weapon's sound loops
 * until the game stops it (func_80081458, the handle func_80026F30 gave) */
void port_sfx_stop(int handle);

s32 func_80026F30_orig(u32 id, s32 flags);
int port_trace(const char *what);
void endless_muted_point(u32 id);
static int trace_sound(void);
/* a sound of another mission's (its table's entry d: program, tone, note, voice, volume, randomness) played from that
 * mission's bank as the game plays its own (func_80026F30: the volume, the place's pans (flags 8, set by
 * func_8003B808 for a sound at a place), the note moved by the caller's (flags 4) or at random by the table's (its own
 * function, so the same draws)): the handle func_80026F30 gives (0x4000 marks it the runtime's), -1 none */
static s32 tone_as_game(int level, const u8 *d, s32 flags, int loop) {
    s32 vol = (flags & 2) ? *((u8 *)g_Args + 0x138) * *(s16 *)((u8 *)g_Args + 0x13A) / 127
                          : *(s16 *)((u8 *)g_Args + 0x13A), l, r, note = d[2];
    int h;
    if (func_80026ED0() >= 0x14 || ((g_CutScene == 2 || g_CutScene == 3))) {
        if (trace_sound()) /* (not played, as the game's own aren't then: logged all the same, as theirs are) */
            port_log("trace: L%d f%d sound: lent (level %d program %d tone %d note %d) not played (%s): handle -1\n",
                     g_Args->level, D_8009CDA8, level, d[0], d[1], d[2], g_CutScene ? "a cut scene" : "no voice free");
        return -1;
    }
    vol = vol * d[4] / 127;
    if (flags & 8)
        r = vol * ((u8 *)g_Args)[0x136] / 127, l = vol * ((u8 *)g_Args)[0x137] / 127;
    else
        l = r = vol;
    l = l < 0 ? 0 : l > 127 ? 127 : l;
    r = r < 0 ? 0 : r > 127 ? 127 : r;
    if (flags & 4)
        note += *(u16 *)((u8 *)g_Args + 0x134);
    else if (d[5])
        note += func_80026B44(d[5]);
    note = note < 0 ? 0 : note > 127 ? 127 : note;
    if (!l && !r)
        return -1;
    vol = l > r ? l : r;
    h = port_tone_play(level, d[0], d[1], note, l, r, loop);
    if (getenv("TWINE_ENDLESS_AMMO_LOG"))
        ELOG("sound: a lent weapon's own (level %d program %d tone %d note %d), volume %d%s: handle %d", level, d[0], d[1],
             note, vol, loop ? ", looping" : "", h);
    if (trace_sound())
        port_log("trace: L%d f%d sound: lent (level %d program %d tone %d note %d) flags %X volume %d%s: handle %d\n",
                 g_Args->level, D_8009CDA8, level, d[0], d[1], note, flags, vol, loop ? " looping" : "", h);
    return h ? 0x4000 | (h & 0x3FFF) : -1;
}
void port_log(const char *fmt, ...);
static int trace_sound(void);
s32 func_80026F30(u32 id, s32 flags) {
    hitloop_sounds += hitloop_in; /* (every sound comes here, a sound at a place too) */
    if (id >= LENT_SOUND && id < 0x80 && id - LENT_SOUND < 8 && lent_sound[id - LENT_SOUND].level) {
        int n = id - LENT_SOUND;
        s32 h = tone_as_game(lent_sound[n].level, lent_sound[n].sfx, flags, lent_sound[n].loop);
        if (trace_sound())
            port_log("trace: L%d f%d sound: (that was lent %u)\n", g_Args->level, D_8009CDA8, id);
        return h;
    }
    {   /* the mission's story lines muted in a run, whoever plays them: King's Ransom's "hurry James" (0x65) comes
         * straight here from its mission logic (func_8006AEC0, each frame: as its stage counter D_800E3168 moves on),
         * not by a script's sound op (func_800572F4) nor as a sound at a place (func_80027248) */
        s16 e[5];
        if (id < 0x80 && port_endless_room(e) && muted_line(id)) {
            static u32 last_id = ~0u;
            static s32 last_at;
            if (id != last_id || room_frames < last_at || room_frames - last_at > SEC * 10) /* (a loop asked for again
                                                                                     * each frame: Night Watch's siren) */
                ELOG("sound: story line %u muted (played directly, from %08X)", id, port_rc_ra), last_id = id,
                    last_at = room_frames;
            if (trace_sound()) { /* (the PS1 stack's code addresses: its callers, roughly; the recompiled code's calls
                                  * keep no return address, so port_rc_ra is often 0) */
                int k;
                for (k = 0; k < 64 && port_rc_psp >= 0x80000000u && port_rc_psp < 0x801FFF00u; k++) {
                    u32 v = *(u32 *)(long)(port_rc_psp + k * 4);
                    if (v >= 0x80010000u && v < 0x800A0000u && !(v & 3))
                        port_log("trace: sound stack +%X %08X\n", k * 4, v);
                }
            }
            endless_muted_point(id);
            return -1;
        }
    }
    if (trace_sound()) { /* (the mission's own: its table's entry, g_Args+0x130 + id * 8: program, tone, note, voice) */
        static const u8 none[8] = {0xFF, 0xFF, 0xFF, 0xFF};
        const u8 *e = *(u8 **)((u8 *)g_Args + 0x130) && id < 0x80 ? *(u8 **)((u8 *)g_Args + 0x130) + id * 8 : none;
        s32 h = func_80026F30_orig(id, flags);
        port_log("trace: L%d f%d sound: %u flags %X (program %d tone %d note %d voice %d): handle %d (called from %08X)\n",
                 g_Args->level, D_8009CDA8, id, flags, e[0], e[1], e[2], e[3], h, port_rc_ra);
        { /* (the bank's own numbers for it, from the disc: the program's volume and pan, the tone's volume, pan,
           * centre, ADSR; and the game's volume setting, the table's volume, the place's pans: for the lent tones) */
            const u8 *vh, *vb;
            if (e[0] != 0xFF && port_sound_bank(g_Args->level, &vh, &vb)) {
                const u8 *pa = vh + 0x20 + e[0] * 16, *ta = vh + 0x20 + 0x800 + (e[0] * 16 + e[1]) * 32;
                extern u16 spu_regs[0x200];
                if (h >= 0 && h < 24)
                    port_log("trace: sound: its voice %d: vol %04X %04X pitch %04X adsr %04X %04X%s\n", h,
                             spu_regs[h * 8], spu_regs[h * 8 + 1], spu_regs[h * 8 + 2], spu_regs[h * 8 + 4],
                             spu_regs[h * 8 + 5], ((spu_regs[0xCC] | (u32)spu_regs[0xCD] << 16) >> h & 1) ? ", reverb" : "");
                port_log("trace: sound: bank level %d: master %d pan %d; program %d volume %d pan %d; tone %d volume %d "
                         "pan %d centre %d.%d adsr %04X %04X mode %d; setting %d, table %d, place %d %d, flags %X\n",
                         g_Args->level, vh[0x18], vh[0x19], e[0], pa[1], pa[4], e[1], ta[2], ta[3], ta[4], ta[5],
                         ta[16] | ta[17] << 8, ta[18] | ta[19] << 8, ta[1], *(s16 *)((u8 *)g_Args + 0x13A), e[4],
                         ((u8 *)g_Args)[0x136], ((u8 *)g_Args)[0x137], flags);
            }
        }
        return h;
    }
    return func_80026F30_orig(id, flags);
}

/* (a debugger's breakpoint: a muted line's caller, bt) */
void __attribute__((noinline)) endless_muted_point(u32 id) {
    __asm__ volatile("" ::"r"(id));
}

/* A sound at a place (the game's 3D one: a person's cry, an object's): the mission's story lines muted in a run as
 * the scripts' are (King's Ransom's "hurry James" came this way, not by a script) */
s32 func_80027248_orig(s32 sound, s32 pos, s32 flags);
s32 func_80027248(s32 sound, s32 pos, s32 flags) {
    s16 e[5];
    u32 id = sound & 0xFF;
    if (trace_sound()) {
        const u8 *d = *(u8 **)((u8 *)g_Args + 0x130);
        port_log("trace: L%d f%d sound: at a place %u flags %X (program %d tone %d) from %08X\n", g_Args->level,
                 D_8009CDA8, id, flags, d ? d[id * 8] : -1, d ? d[id * 8 + 1] : -1, port_rc_ra);
    }
    if (port_endless_room(e) && muted_line(id)) {
        ELOG("sound: story line %u muted (a sound at a place, from %08X)", id, port_rc_ra);
        return 0;
    }
    return func_80027248_orig(sound, pos, flags);
}

static int trace_sound(void) {
    static int on = -1;
    if (on < 0)
        on = port_trace("sound");
    return on;
}

/* The mission's music. It isn't the CD's (the XA streams hold the speech) nor a libsnd sequence: it is one long
 * looping tone of the level's sound bank, an entry of the same table as the sounds (g_Args+0x130), started by
 * func_80026D08 at the music volume (g_Args+0x144) and kept at it each frame by func_800272A4. Each mission's music
 * function (its own, each frame: e.g. Night Watch's func_8006A388) asks for entry 0, and two missions have a second
 * song, entry 1, which their logic switches to (stopping the first, func_80026C14, then starting the other):
 *   Night Watch (level 4): entry 1 while Bond's story flag 2 is up (his +0x124 bit 5, set by the mission's scripts),
 *     no cutscene runs and the mission's D_8009FB30 is set (an object of the kind D_8009DD54 seen to:
 *     func_80069F58); entry 0 otherwise;
 *   Meltdown (level 10): entry 1 from the time its stage D_800EA8DC reaches 4 (and on: it is never set back).
 * The other missions only ever play entry 0.
 * In a run, a room plays the mission's action song from the start: entry 0 asked for is entry 1 in those two missions
 * (the volume kept by entry 1's, as it plays), so whatever the mission's logic asks for, the action song plays.
 * TWINE_TRACE=music logs each song started (entry, the table's program/tone/note/volume, the bank tone's sample
 * size and loop, the voice, the caller) and each stop of it: "trace: L<level> f<frame> music: ...". */
static int trace_music(void) {
    static int on = -1;
    if (on < 0)
        on = port_trace("music");
    return on;
}
static s32 music_voice = -1, music_entry = -1; /* the song playing (the voice its key on took), its entry */

/* the entry a run plays for the one asked for */
/* the caller, for the trace: port_rc_ra, or (the recompiled code's calls keep no return address) the first code
 * address on the PS1 stack, roughly the caller's caller */
static u32 music_caller(void) {
    int k;
    if (port_rc_ra)
        return port_rc_ra;
    for (k = 0; k < 32 && port_rc_psp >= 0x80000000u && port_rc_psp < 0x801FFF00u; k++) {
        u32 v = *(u32 *)(long)(port_rc_psp + k * 4);
        if (v >= 0x80026800u && v < 0x800A0000u && !(v & 3))
            return v;
    }
    return 0;
}

static u32 music_for(u32 id) {
    s16 e[5];
    if (id == 0 && (g_Args->level == 4 || g_Args->level == 10) && port_endless_room(e))
        return 1;
    return id;
}

s32 func_80026D08_orig(u32 id, s32 flags);
s32 func_80026D08(u32 id, s32 flags) {
    u32 to = music_for(id);
    s32 h = func_80026D08_orig(to, flags);
    if (h >= 0)
        music_voice = h, music_entry = (s32)to;
    if (trace_music()) {
        const u8 *t = *(u8 **)((u8 *)g_Args + 0x130), *vh, *vb;
        const u8 *d = t && to < 0x69 ? t + to * 8 : NULL;
        char bank[96] = "";
        s16 e[5];
        if (d && port_sound_bank(g_Args->level, &vh, &vb)) { /* (the tone's sample: its size in the bank, in bytes) */
            const u8 *ta = vh + 0x20 + 0x800 + (d[0] * 16 + d[1]) * 32;
            u32 vag = ta[22] | ta[23] << 8, ps = vh[0x12] | vh[0x13] << 8;
            const u8 *sz = vh + 0x20 + 0x800 + ps * 16 * 32;
            if (vag < 256) { /* (its length played at the entry's note: 28 samples a 16-byte block, at 44100 Hz
                              * moved by the note's distance from the tone's centre, a semitone each) */
                u32 bytes = (sz[vag * 2] | sz[vag * 2 + 1] << 8) * 8u;
                int st = d[2] - ta[4];
                double rate = 44100.0;
                for (; st < 0; st++)
                    rate /= 1.0594630943592953;
                for (; st > 0; st--)
                    rate *= 1.0594630943592953;
                snprintf(bank, sizeof(bank), ", bank sample %u (%u bytes: a %.1f s loop)", vag, bytes,
                         bytes / 16.0 * 28 / rate);
            }
        }
        port_log("trace: L%d f%d music: start entry %u%s (program %d tone %d note %d volume %d)%s: voice %d, from %08X%s\n",
                 g_Args->level, D_8009CDA8, to, to != id ? " (the action song: entry 0 asked for, in a run)" : "",
                 d ? d[0] : -1, d ? d[1] : -1, d ? d[2] : -1, d ? d[4] : -1, bank, h, music_caller(),
                 port_endless_room(e) ? ", in a run" : "");
    }
    return h;
}

void func_800272A4_orig(s16 voice, s32 id, s32 flags);
void func_800272A4(s16 voice, s32 id, s32 flags) {
    func_800272A4_orig(voice, voice == music_voice && id >= 0 ? (s32)music_for((u32)id) : id, flags);
}

/* TWINE_TEST_STORY_FLAG=frame:n (tests, campaign or a run): Bond's story flag n (1-16: his +0x124 bit n + 3, the
 * flags the mission's scripts set and its logic reads, func_80053B40) raised from that mission frame on, as the
 * mission's scripts would: Night Watch's 2 is its action song's (tools/endless_check.py's t) */
extern u32 D_800D91E4; /* (Bond's object, D_800D90C0, +0x124) */
static void test_story_flag(void) {
    static int frame = -2, n;
    const char *v;
    if (frame == -2) {
        frame = -1;
        if ((v = getenv("TWINE_TEST_STORY_FLAG")) && strchr(v, ':'))
            frame = atoi(v), n = atoi(strchr(v, ':') + 1);
    }
    if (frame >= 0 && n >= 1 && n <= 16 && D_8009CDA8 >= frame && !(D_800D91E4 >> (n + 3) & 1)) {
        D_800D91E4 |= 1u << (n + 3);
        port_log("test: f%d Bond's story flag %d raised (TWINE_TEST_STORY_FLAG)\n", D_8009CDA8, n);
    }
}

static void music_stopped(const char *how) {
    if (music_voice < 0)
        return;
    if (trace_music())
        port_log("trace: L%d f%d music: stop entry %d (voice %d): %s, from %08X\n", g_Args->level, D_8009CDA8,
                 music_entry, music_voice, how, music_caller());
    music_voice = music_entry = -1;
}

void func_80081458_orig(s32 voice);
void func_80081458(s32 voice) {
    if (voice == music_voice)
        music_stopped("its voice keyed off");
    if (voice & 0x4000) {
        port_sfx_stop(voice & 0x3FFF); /* (the handle's low bits: spu.c's handles are counted from 1) */
        if (getenv("TWINE_ENDLESS_AMMO_LOG"))
            ELOG("sound: a lent weapon's stopped (handle %d)", voice & 0x3FFF);
    }
    else
        func_80081458_orig(voice);
}

/* The game keys all its voices off (SsUtAllKeyOff: the mission's end; func_80026C90(-1): every voice but one stopped,
 * as the menu over the mission opens): the runtime's own (a lent weapon's sound, a callout) with them, as more of the
 * SPU's voices. A lent automatic's looping fire played on through the menu (its stop comes from the weapon's state
 * machine, which the menu holds still) and, held through a door, on through the next missions (playtest 8; spu.c
 * also stops them as an executable starts). TWINE_TEST_SOUND_KEEP=1: not (the bug, for tests). */
int port_spu_runtime_off(int keep);
static void runtime_off(const char *why, int keep) {
    int n;
    if (getenv("TWINE_TEST_SOUND_KEEP"))
        return;
    n = port_spu_runtime_off(keep);
    if (n && trace_sound())
        port_log("trace: L%d f%d sound: %s: %d of the runtime's own with them\n", g_Args->level, D_8009CDA8, why, n);
}

void SsUtAllKeyOff_orig(s32 mode);
void SsUtAllKeyOff(s32 mode) {
    runtime_off("all keyed off", 0);
    music_stopped("all voices keyed off");
    SsUtAllKeyOff_orig(mode);
}

void func_80026C90_orig(s32 keep);
void func_80026C90(s32 keep) {
    runtime_off("all voices stopped", keep >= 0 && (keep & 0x4000) ? keep & 0x3FFF : 0);
    if (keep != music_voice)
        music_stopped("all voices stopped (the menu over the mission)");
    func_80026C90_orig(keep);
}

/* A lent sniper rifle's scope: its home mission's code raises the scope (the weapon's state 0x21, then 0x22) where
 * other weapons aim (func_8004DD9C), and fires from it (0x23), by the rifle's slot there; a mission without one has
 * no such code, so the lent rifle is given it here (the states' own code is every mission's) */
extern s32 D_800F11F0; /* the weapon's state */
void func_8007A17C(s32 state);
extern s32 D_8009CD4C; /* aiming */
static int lent_sniper(void) {
    return D_800B0DD4 >= 0 && (lent_mask >> D_800B0DD4 & 1) && !strncmp((char *)g_Weapons[D_800B0DD4].pad0, "AR36", 4);
}

void func_8004DD9C_orig(void);
void func_8004DD9C(void) {
    if (lent_sniper() && D_800F11F0 != 0x21 && D_800F11F0 != 0x22) {
        func_8007A17C(0x21);
        return;
    }
    func_8004DD9C_orig();
}

/* ... and its gun isn't drawn in closer while aimed, as other guns are (the aim's per-frame pull in, and its
 * hold): the missions with a sniper rifle leave its slot out there (levels 1 and 8 slot 3, levels 3 and 5 slot 2),
 * and the scope's picture, part of the rifle's model, would come too near the eye to be drawn */
void func_800449BC_orig(void);
void func_800449BC(void) {
    if (!lent_sniper())
        func_800449BC_orig();
}

void func_80044A60_orig(void);
void func_80044A60(void) {
    if (!lent_sniper())
        func_80044A60_orig();
}

/* AUTO-AIM WITH A LENT WEAPON, AS AT HOME. The pause menu's auto-aim (g_Args->flags2 bit 4) is func_80036938's, every
 * frame: the nearest person on screen inside a cone in front of Bond, nothing of the game's in the way (objects,
 * walls), locked (g_Player+0x5C: the guns' fire aims at him, func_8003872C; the crosshair red on him, func_80049A88).
 * It asks nothing of the weapon but its slot: each mission leaves out the slots of its own that never auto-aim, by
 * number (levels 1: 2 and 3, 3: 2, 5: 2, 8: 3, 9: 5; the others none): the AR36 sniper rifle's and the grenade
 * launcher's in every mission that has them. A lent weapon is in one of the tools' slots (clean_slots), none of them
 * left out anywhere, so it locks on as any gun of the mission does (seen in TWINE_TRACE=aim); but the rifle and the
 * launcher lent locked on where they never do at home. Now they don't. */
static const u8 aim_left_out[12] = {0, 1 << 2 | 1 << 3, 0, 1 << 2, 0, 1 << 2, 0, 0, 1 << 3, 1 << 5, 0, 0};
static int no_autoaim_name(const char *nm) {
    return !strncmp(nm, "AR36", 4) || !strncmp(nm, "GRENADE LAUNCHER", 16);
}
void func_80036938_orig(void);
void func_80036938(void) {
    u32 had = g_Args->flags2 & 0x10;
    int off = had && D_800B0DD4 >= 0 && D_800B0DD4 < 16 && (lent_mask >> D_800B0DD4 & 1) &&
              no_autoaim_name((char *)g_Weapons[D_800B0DD4].pad0);
    static s16 said = -1;
    if (g_Args->level > 0 && g_Args->level < 12 && D_800B0DD4 >= 0 && D_800B0DD4 < 16 && said != D_800B0DD4 &&
        !(lent_mask >> D_800B0DD4 & 1) && g_Weapons[D_800B0DD4].pad0[0] >= 'A' &&
        ((aim_left_out[g_Args->level] >> D_800B0DD4 & 1) != 0) != (no_autoaim_name((char *)g_Weapons[D_800B0DD4].pad0) != 0)) {
        said = D_800B0DD4; /* (the table and the names disagree: a mission's own weapon this note doesn't know) */
        ELOG("aim: %s in slot %d: auto-aim %s at home, the lent rule says otherwise", (char *)g_Weapons[D_800B0DD4].pad0,
             D_800B0DD4, aim_left_out[g_Args->level] >> D_800B0DD4 & 1 ? "left out" : "on");
    }
    if (off && getenv("TWINE_TEST_AIM_OLD")) /* (tests: the old way, the lent rifle or launcher locking on) */
        off = 0;
    if (off) /* (as its home mission's code: no target for it) */
        g_Args->flags2 &= ~0x10u;
    func_80036938_orig();
    g_Args->flags2 |= had;
}

void func_8007A17C_orig(s32 state);
void func_8007A17C(s32 state) {
    if ((state == 8 || state == 9) && D_8009CD4C && lent_sniper())
        state = 0x23;
    func_8007A17C_orig(state);
}

static void trace_model(const char *how, s32 idx) {
    static int on = -1;
    if (on < 0)
        on = port_trace("weapon");
    if (on)
        port_log("trace: L%d f%d weapon: model asked (%s) for slot %d (%.16s, type %d), in hand %d\n", g_Args->level,
                 D_8009CDA8, how, idx, idx >= 0 && idx < 16 ? (char *)g_Weapons[idx].pad0 : "-",
                 idx >= 0 && idx < 16 ? ((u8 *)&g_Weapons[idx])[0x5B] : -1, D_800B0DD4);
}

void func_80076014(s32 idx) {
    trace_model("80076014", idx);
    if (!lent_model(idx)) {
        port_lent_images(0, 0);
        lent_work_type = 0;
        func_80076014_orig(idx);
    }
}

void func_80076104(s32 idx) {
    trace_model("80076104", idx);
    if (!lent_model(idx)) {
        port_lent_images(0, 0);
        lent_work_type = 0;
        func_80076104_orig(idx);
    }
}

/* (the missions' story and public-address voice lines, muted in a run: they'd tell Bond of a story he isn't in;
 * played by a script's sound op, by their number in the mission's table, g_Args+0x130, 8 bytes each: program,
 * tone, note, voice, volume, randomness) */
static int muted_line(u32 id) {
    static const u8 mute[][3] = { /* (level, program, tone) */
        {8, 9, 7},                       /* King's Ransom's "hurry James" */
        {2, 9, 3}, {2, 9, 6},            /* Courier's ending: "open the door!", "open up" */
        {6, 9, 4}, {7, 9, 4},            /* Christmas's "I got it" */
        {7, 9, 0}, {7, 9, 1}, {10, 9, 7}, {10, 9, 8}, /* the public address: "warning", "blast doors open", ... */
        {7, 9, 7}, {7, 9, 8},            /* the friendly guard's "hello", "good day sir" (Masquerade's disguise) */
        {4, 7, 9}};                      /* Night Watch's alarm siren (0x4B, its loop while D_8009FAA0: func_8006A388) */
    SfxDefE *d = *(SfxDefE **)((u8 *)g_Args + 0x130);
    unsigned k;
    if (!d || id > 0xFF)
        return 0;
    for (k = 0; k < sizeof(mute) / sizeof(mute[0]); k++)
        if (mute[k][0] == g_Args->level && mute[k][1] == d[id].prog && mute[k][2] == d[id].tone)
            return 1;
    return 0;
}
