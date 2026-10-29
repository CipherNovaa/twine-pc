/* Weapon keys and the weapon list (native PC build only).
 *
 * The number keys pick a weapon by its group, the same in every mission (GROUPS below): 1 pistols, 2 submachine guns
 * and assault rifles, 3 sniper rifles, 4 shotguns, 5 explosives and launchers, 6 gadgets, 7 fists. A key takes the
 * group's weapon held last (else its first); pressed again (its group's weapon in hand) the next of the group, round:
 * the weapons checked in the inventory only, when any of the group is. A group he has nothing of does nothing. Q (and
 * L1 in the modern pad layout, port/src/video.c) takes the weapon held before the one in hand, whatever they are:
 * pressed again, back. A weapon only passed through on the way (held under half a second) isn't one held.
 *
 * The weapon buttons (next, previous: the pad's, the mouse wheel, Z and X) are the game's (func_800798D4, every frame
 * Bond has the controls: it steps the slot to change to, D_800B0DDC, through func_80079C54 past the slots that aren't
 * his or are out of their round, then sets the weapon's state machine putting the one in hand away, func_80079B94;
 * its state 5 then loads and draws the new one). In an endless run they step through the weapons Bond carries
 * instead (port/mods/endless.c: his own and the lent ones, each an entry of its own with its own checkmark, the
 * inventory's toggle), in the groups' order, the ones checked only. A key or Q goes through the same change: the one in
 * hand put away, the chosen one drawn, the game's own animations and sounds. When the game wouldn't change weapons
 * (mid-change, fire held) nothing happens.
 *
 * Each change shows a strip on the HUD (TWINE_WEAPON_STRIP, default B): the seven groups' numbers, the chosen
 * weapon's lit (the ones Bond has nothing of dim), a dot under each for each weapon of the group (the chosen one's
 * gold), and its name under them, in a box made of the ammo counter's own pieces, in the place of the game's own name
 * band (the band that names the weapon as a change goes on: TWINE_WEAPON_STRIP_AT=centre; or band, box, msg), shown
 * as long as that band is, else
 * a second and a half, fading over its last quarter second. The letters are the game's own (func_800291B0: the
 * menus' font and their two text colours), in the menus' spare letter sprites; the tiles, modes and box quads in the
 * MIPS code of the two functions this mod replaces (port_ps1_addr).
 *
 * TWINE_TRACE=select logs each choice (the list as stepped, what was in hand, what is drawn next and why) and the
 * strip's showings. TWINE_TEST_SELECT_OLD=1: the old way (the game's own stepping over slots, the pool stepped as a
 * lent slot is left; number keys by place, Q between the last two picked by number), to show what changed.
 *
 * MOD_REPLACES_NATIVE: func_800798D4 func_80079C54
 *
 * Names are LEVEL01's; port/mods.py maps them onto each level. */
#include "twine.h"

int snprintf(char *s, unsigned n, const char *fmt, ...);
char *getenv(const char *name);
int atoi(const char *s);
int strncmp(const char *a, const char *b, unsigned n);
int strcmp(const char *a, const char *b);
void *memcpy(void *d, const void *s, unsigned n);
unsigned strlen(const char *s);
void port_log(const char *fmt, ...);
int port_trace(const char *what);

extern s16 D_800B0DD4;      /* the weapon in hand (its slot in g_Weapons) */
extern s16 D_800B0DD8;      /* the one before a change */
extern s16 D_800B0DDC;      /* the one to change to */
extern u32 D_800F11F0;      /* the weapon's state */
extern s32 D_800DDEEC;      /* fire held */
extern u8 D_800AC38C;
extern s32 D_8009CDA8;      /* the mission's frame counter (its ticks) */
extern s8 D_8009CABF;       /* the HUD hidden (the opening iris) */
extern s16 D_8009CAA4;      /* a menu open */
extern s32 D_8009CD00;
extern u32 D_800B8EFC[2][5]; /* the HUD's ordering tables (GsOT: +4 its table) */
extern s32 D_800B8F24;      /* the frame being made (0 or 1) */
extern int D_800AC54C;      /* the menus' texts */
extern u8 D_800AB744[];     /* their table: 0x60 bytes each, +0 the first letter's sprite, +4 the last's */
extern int D_800AEDA8;      /* the menus' letters in use (of 230: "inc MAX_TEXT_SPRITES!") */
extern u8 D_800AC550[];     /* their sprites, two (one for each frame) of 0x14 bytes each */
extern u8 D_800AE940[];     /* the font: 8 bytes a character (u, v, width, height, .., .., y offset) */
extern s16 D_800AEDD4;      /* the gap between letters */
extern s16 D_800AC4E0, D_800AC4E4; /* the font's texture page */
extern u16 D_800AC4D8, D_800AC4DC; /* the menus' text colour (its palette) */
extern s16 D_800AEDA0, D_800AEDA4; /* the chosen item's */
extern int port_subframe;   /* an extra drawing between ticks (smooth motion) */
extern int port_smooth_t;   /* ... and how far into the tick (0-4096) */
int port_weapon_key(void);
void trace_change_by(const char *who);
void func_800798D4_orig(void);
s32 func_80079C54_orig(s32 dir);
s32 func_800794A0(void);
s32 func_80079C04(void);
void func_8007A17C(s32 state);
void func_800291B0(char *s, s16 x, s16 y, s32 *n, s32 arg4, s32 pal);

extern s32 D_8009EEC8;
extern s32 D_8009EED0;

/* endless.c's (weak: none in a build without it): the weapons Bond carries in a run */
int endless_carried(void) __attribute__((weak)); /* how many (-1: not in a run) */
const char *endless_carried_name(int e) __attribute__((weak));
int endless_carried_ticked(int e) __attribute__((weak));
int endless_carried_current(void) __attribute__((weak));
int endless_carried_take(int e, const char *why) __attribute__((weak));
const char *endless_carried_before(void) __attribute__((weak));
void endless_carried_set_before(const char *name) __attribute__((weak));
int endless_room_drawing(void) __attribute__((weak));

/* each mission's weapon slots (g_Weapons' count: the one its own weapon buttons wrap at, endless.c's weapon_slots) */
static const u8 slot_count[12] = {6, 6, 12, 9, 9, 11, 12, 13, 11, 12, 10, 4};
#define SLOTS (g_Args->level < 12 ? slot_count[g_Args->level] : 6)

/* THE GROUPS: every weapon and thing Bond can hold in the eleven missions (their slots' names, as the weapons
 * catalogue has them: port_weapon_find), by the start of its name. Their order here is the order the weapon buttons
 * step through them in a run (and the inventory lists them), the same in every mission. Anything not named: 6. */
static const struct {
    const char *name;
    u8 group;
} groups[] = {
    {"WOLFRAM P2K", 1}, {"IAC DEFENDER", 1}, {"TRANQUILIZER", 1},
    {"KOFFLER KS5", 2}, {"MEYER TMP", 2}, {"INGALLS TYPE 20", 2}, {"BELGIQUE PS100", 2}, {"KAZAKOVICH KA-57", 2},
    {"AR36 SNIPER", 3},
    {"FRINESI SHOTGUN", 4}, {"FRINESI AUTO", 4},
    {"SATCHEL CHARGE", 5}, {"SATCHEL DET", 5}, {"ROCKET LAUNCHER", 5}, {"GRENADE LAUNCHER", 5}, {"FRAG GRENADE", 5},
    {"GAS GRENADE", 5}, {"PIPE BOMB", 5}, {"PEN EXPLOSIVE", 5},
    {"FLASH BANG", 6}, {"CELL PHONE STUN", 6}, {"FLARE GUN", 6}, {"NIGHT VISION", 6}, {"GRAPPLING HOOK", 6},
    {"MICRO CAMERA", 6}, {"TELEPHONE BUG", 6}, {"CREDIT CARD", 6}, {"FINGERPRINT", 6}, {"VLF DISRUPTOR", 6},
    {"RADIATION", 6}, {"WRENCH", 6}, {"BRIEFCASE", 6}, {"ARKOV'S ID CARD", 6}, {"SECURITY KEYCARD", 6},
    {"BULLION'S KEY", 6}, {"M'S KEYCARD", 6}, {"GABOR'S KEYCARD", 6}, {"CELL DOOR KEY", 6}, {"CONTROL ROOM KEY", 6},
    {"ID CARD", 6}, {"PALMTOP", 6},
    {"UNARMED", 7},
};
#define NGROUPS (int)(sizeof(groups) / sizeof(groups[0]))

static int prefix(const char *name, const char *p) {
    while (*p)
        if (*name++ != *p++)
            return 0;
    return 1;
}

/* its place in the order (anything not named after them, in the gadgets) */
int weaponkeys_rank(const char *name) {
    int k;
    for (k = 0; k < NGROUPS; k++)
        if (prefix(name, groups[k].name))
            return k;
    return NGROUPS;
}

static int group_of(const char *name) {
    int k = weaponkeys_rank(name);
    return k < NGROUPS ? groups[k].group : 6;
}

/* THE LIST: in a run, endless.c's (each weapon he carries); else the slots he has (owned: flags bit 11), the mission's
 * own list. In the groups' order. */
typedef struct {
    int id;           /* endless's entry, or the slot */
    const char *name;
    int ticked;       /* in the weapon buttons' round */
    int rank;
} Ent;

static int in_run(void) {
    return endless_carried && endless_carried() >= 0;
}

static int valid_slot(int i) {
    const char *n = (const char *)g_Weapons[i].pad0;
    return n[0] >= 'A' && n[0] <= 'Z';
}

static int list(Ent *e, int *run) {
    int n = 0, i, j, cnt = endless_carried ? endless_carried() : -1;
    *run = cnt >= 0;
    if (cnt >= 0) {
        for (i = 0; i < cnt && n < 64; i++, n++) {
            e[n].id = i, e[n].name = endless_carried_name(i), e[n].ticked = endless_carried_ticked(i);
            e[n].rank = weaponkeys_rank(e[n].name) * 64 + i;
        }
    } else
        for (i = 0; i < SLOTS && n < 64; i++)
            if (valid_slot(i) && (g_Weapons[i].flags & 0x800)) {
                e[n].id = i, e[n].name = (const char *)g_Weapons[i].pad0;
                e[n].ticked = g_Weapons[i].flags >> 12 & 1;
                e[n].rank = weaponkeys_rank(e[n].name) * 64 + i;
                n++;
            }
    for (i = 1; i < n; i++) /* (by rank) */
        for (j = i; j > 0 && e[j - 1].rank > e[j].rank; j--) {
            Ent t = e[j];
            e[j] = e[j - 1], e[j - 1] = t;
        }
    return n;
}

/* the entry in hand (or being changed to), -1 none */
static int current(const Ent *e, int n, int run) {
    int i, c = -1;
    if (run)
        c = endless_carried_current();
    else
        c = D_800B0DDC >= 0 && D_800B0DDC < SLOTS ? D_800B0DDC : D_800B0DD4;
    for (i = 0; i < n; i++)
        if (e[i].id == c)
            return i;
    return -1;
}

/* entry t to be changed to: 1 a change to begin */
static int take(const Ent *t, int run, const char *why) {
    if (run)
        return endless_carried_take(t->id, why);
    D_800B0DDC = (s16)t->id;
    return t->id != D_800B0DD4;
}

static void trace_list(const char *how, const Ent *e, int n, int c, int t) {
    char b[400];
    int i, o = 0;
    if (!port_trace("select"))
        return;
    for (i = 0; i < n && o < 360; i++)
        o += snprintf(b + o, sizeof(b) - o, "%s%s%.16s%s(%d)", i ? ", " : "", i == c ? "*" : "", e[i].name,
                      e[i].ticked ? "" : "[-]", group_of(e[i].name));
    port_log("trace: L%d f%d select: %s: %s -> %s; list %s\n", g_Args->level, D_8009CDA8, how,
             c >= 0 ? e[c].name : "(none)", t >= 0 ? e[t].name : "(nothing)", b);
}

/* ---- the HUD strip ---- */
#define STRIP_TICKS 30 /* a second and a half (20 ticks a second) */
#define STRIP_FADE 5   /* the last quarter second fading */
#define STRIP_MAX 24
static s32 strip_at = -1000000;
static char strip_name[28];
static u8 strip_have; /* (the groups he has something of: bit g) */
static s8 strip_group;
static struct { char name[24]; u8 group, ticked, chosen; } strip_list[STRIP_MAX]; /* (what he carries, in order) */
static int strip_n;

static void strip_show(const char *name, const Ent *e, int n) {
    int i;
    snprintf(strip_name, sizeof(strip_name), "%s", name);
    strip_group = (s8)group_of(name);
    strip_have = 0;
    strip_n = 0;
    for (i = 0; i < n; i++) {
        strip_have |= 1 << group_of(e[i].name);
        if (strip_n < STRIP_MAX) {
            snprintf(strip_list[strip_n].name, sizeof(strip_list[0].name), "%s", e[i].name);
            strip_list[strip_n].group = (u8)group_of(e[i].name), strip_list[strip_n].ticked = (u8)e[i].ticked;
            strip_list[strip_n++].chosen = !strncmp(e[i].name, name, 23);
        }
    }
    strip_at = D_8009CDA8;
    if (port_trace("select"))
        port_log("trace: L%d f%d select: strip shown: group %d lit, \"%s\" (groups %02X)\n", g_Args->level, D_8009CDA8,
                 strip_group, strip_name, strip_have);
}

/* the weapon buttons' choice (func_80079B94: 1 next, 3 previous; 2 and 5 the game's own picks): in a run, the next
 * checked weapon he carries that way, in the groups' order */
s32 func_80079C54(s32 dir) {
    Ent e[64];
    int n, c, k, t = -1, run;
    s32 r;
    if ((dir != 1 && dir != 3) || !in_run()) {
        r = func_80079C54_orig(dir);
        if (r && (dir == 1 || dir == 3) && D_800B0DDC >= 0 && D_800B0DDC < SLOTS) {
            n = list(e, &run);
            strip_show((const char *)g_Weapons[D_800B0DDC].pad0, e, n);
        }
        return r;
    }
    D_800B0DD8 = D_800B0DD4;
    n = list(e, &run);
    c = current(e, n, run);
    for (k = 1; k <= n && t < 0; k++) {
        int i = c >= 0 ? c + (dir == 1 ? k : -k) : dir == 1 ? k - 1 : n - k;
        i = ((i % n) + n) % n;
        if (i != c && e[i].ticked)
            t = i;
    }
    trace_list(dir == 1 ? "the next-weapon button" : "the previous-weapon button", e, n, c, t);
    if (t < 0) {
        D_800B0DDC = D_800B0DD8;
        return 0;
    }
#ifdef MOD_TRACE
    trace_change_by(dir == 1 ? "the next-weapon button (the weapons he carries)" : "the previous-weapon button (the weapons he carries)");
#endif
    r = take(&e[t], 1, dir == 1 ? "the next-weapon button" : "the previous-weapon button");
    strip_show(e[t].name, e, n);
    return r;
}

/* ---- Q: the weapon held before ---- */
/* A weapon counts as held once it has been in hand SETTLE ticks (half a second), or at once when Q chose it: the ones
 * a number key or the weapon buttons only pass through on the way (key 1 twice: the P2K, then the IAC Defender) aren't
 * the weapon before. Each group keeps the last weapon held of it, for its number key. */
#define SETTLE 10
static char held[28], before[28]; /* (the weapon in hand, settled; the one before it) */
static char cand[28];             /* (the one in hand, settling) */
static s32 cand_at;
static int cand_by_q;
static char group_last[8][28];    /* (each group's last held) */
static int level = -1;
static s32 last_frame;

static void memset_groups(void) {
    int g;
    for (g = 0; g < 8; g++)
        group_last[g][0] = 0;
}

static void history(int run) {
    const char *now;
    int s = D_800B0DD4;
    if (level != g_Args->level || D_8009CDA8 < last_frame) { /* (a mission begun: in a run, Q's weapon carried in) */
        level = g_Args->level;
        held[0] = before[0] = cand[0] = 0;
        memset_groups();
        if (run && endless_carried_before)
            snprintf(before, sizeof(before), "%s", endless_carried_before());
    }
    last_frame = D_8009CDA8;
    if (s < 0 || s >= SLOTS || !valid_slot(s) || !(g_Weapons[s].flags & 0x800) || D_800B0DDC != s || D_8009CABF ||
        D_800F11F0 == 3 || D_800F11F0 == 5 || D_800F11F0 == 6 || (run && endless_room_drawing && endless_room_drawing()))
        return; /* (not settled: changing, the mission's opening (its own weapon), or the room's carried one to come) */
    now = (const char *)g_Weapons[s].pad0;
    if (!strncmp(now, held, 23))
        return;
    if (strncmp(now, cand, 23)) { /* (just in hand: it settles from now) */
        snprintf(cand, sizeof(cand), "%s", now);
        cand_at = D_8009CDA8;
    }
    if (!cand_by_q && held[0] && D_8009CDA8 - cand_at < SETTLE)
        return;
    cand_by_q = 0;
    if (held[0]) {
        snprintf(before, sizeof(before), "%s", held);
        if (run && endless_carried_set_before)
            endless_carried_set_before(before);
    }
    snprintf(held, sizeof(held), "%s", now);
    if (group_of(held) >= 1 && group_of(held) <= 7)
        snprintf(group_last[group_of(held)], sizeof(group_last[0]), "%s", held);
    if (port_trace("select"))
        port_log("trace: L%d f%d select: in hand %s (before it %s)\n", g_Args->level, D_8009CDA8, held,
                 before[0] ? before : "(none)");
}

/* ---- the old way (TWINE_TEST_SELECT_OLD=1) ---- */
static int select_old(void) {
    static int v = -1;
    if (v < 0)
        v = getenv("TWINE_TEST_SELECT_OLD") && atoi(getenv("TWINE_TEST_SELECT_OLD"));
    return v;
}
static int picked = -1, other = -1, old_held = -1, old_before = -1, old_level;
static int old_key(int key) {
    int slot;
    if (g_Args->level != old_level)
        old_level = g_Args->level, picked = other = old_held = old_before = -1;
    if (D_800B0DD4 != old_held)
        old_before = old_held, old_held = D_800B0DD4;
    if (!key)
        return -1;
    if (key < 0) {
        slot = picked < 0 ? old_before : D_800B0DD4 == picked ? other : picked;
        D_800B0DDC = slot;
        if (slot < 0 || slot >= SLOTS || func_80079C04())
            slot = D_800B0DD4;
    } else {
        for (slot = 0; slot < SLOTS; slot++) {
            D_800B0DDC = slot;
            if (!func_80079C04() && --key == 0)
                break;
        }
        if (slot == SLOTS)
            slot = D_800B0DD4;
        else if (slot != D_800B0DD4)
            other = D_800B0DD4, picked = slot;
    }
    return slot;
}

static void begin_change(void) { /* as func_80079B94 does it for a weapon button */
    D_800AC38C = 0;
    if (D_800F11F0 - 3 < 2 || D_800F11F0 == 6) {
        D_8009EEC8 = 15;
        D_8009EED0 = 100;
        func_8007A17C(6);
    } else
        func_8007A17C(3);
}

void func_800798D4(void) {
    int key = port_weapon_key(), n, c, t = -1, k, run;
    Ent e[64];
    func_800798D4_orig();
    if (select_old()) {
        int slot = old_key(key);
        if (slot < 0 || D_800DDEEC || !func_800794A0())
            return;
        D_800B0DD8 = D_800B0DD4;
        if (slot == D_800B0DD4) {
            D_800B0DDC = D_800B0DD8;
            return;
        }
        D_800B0DDC = slot;
#ifdef MOD_TRACE
        trace_change_by("the number keys");
#endif
        begin_change();
        return;
    }
    n = list(e, &run);
    history(run);
    if (!key || D_800DDEEC || !func_800794A0())
        return;
    c = current(e, n, run);
    if (key < 0) { /* Q: the weapon held before (the one held, if the one in hand hasn't settled yet) */
        const char *want = c >= 0 && held[0] && strncmp(e[c].name, held, 23) ? held : before;
        for (k = 0; k < n && t < 0; k++)
            if (want[0] && !strncmp(e[k].name, want, 23))
                t = k;
    } else if (key <= 7) { /* a group: its last held (else its first), or (its weapon in hand) the next of it; the
                            * weapons checked in the inventory only, if any of the group is */
        int from = c >= 0 && group_of(e[c].name) == key ? c : -1, checked = 0;
        for (k = 0; k < n; k++)
            checked |= group_of(e[k].name) == key && e[k].ticked;
        if (from < 0 && group_last[key][0])
            for (k = 0; k < n && t < 0; k++)
                if (!strncmp(e[k].name, group_last[key], 23) && (e[k].ticked || !checked))
                    t = k;
        for (k = 1; k <= n && t < 0; k++) {
            int i = from >= 0 ? (from + k) % n : k - 1;
            if (group_of(e[i].name) == key && (e[i].ticked || !checked))
                t = i;
        }
    }
    trace_list(key < 0 ? "Q (the weapon held before)" : key == 1 ? "key 1" : key == 2 ? "key 2" : key == 3 ? "key 3" :
               key == 4 ? "key 4" : key == 5 ? "key 5" : key == 6 ? "key 6" : key == 7 ? "key 7" : "a key with no group",
               e, n, c, t);
    if (t < 0)
        return;
    strip_show(e[t].name, e, n);
    if (t == c)
        return;
    D_800B0DD8 = D_800B0DD4;
    if (!take(&e[t], run, key < 0 ? "Q, the weapon held before" : "a number key")) {
        D_800B0DDC = D_800B0DD8;
        return;
    }
    cand_by_q = key < 0; /* (Q's choice counts as held at once) */
#ifdef MOD_TRACE
    trace_change_by(key < 0 ? "Q (the weapon held before)" : "the number keys (the groups)");
#endif
    begin_change();
}

/* A change to a slot, as the weapon buttons make one (endless mode: the weapon Bond held in the last room drawn in
 * this one): 1 begun, 0 if the game won't change now (mid-change, fire held) */
int weapon_change_to(int slot) {
    if (slot == D_800B0DD4)
        return 1;
    if (D_800DDEEC || !func_800794A0())
        return 0;
    D_800B0DD8 = D_800B0DD4;
    D_800B0DDC = slot;
    begin_change();
    return 1;
}

/* ---- the strip, drawn (quickgrenade.c's func_800463C8, with the rest of the HUD) ----
 * TWINE_WEAPON_STRIP picks its look (variants for choosing one): A the groups' numbers and the name under them,
 * bottom centre on a dark band (the default); A2 the same in a box like the ammo counter's, just above it; B as A2
 * with a dot under each number for each weapon of the group (the chosen one's lit); C a list of what he carries by
 * group, as the pause menu's INVENTORY lists it (its row spacing, the chosen one in its highlight, the checkmark
 * column), in such a box above the ammo counter; off none. */
extern u8 D_800D7838[] __attribute__((weak)), D_800D4688[] __attribute__((weak)), D_800D77E8[] __attribute__((weak)); /* the ammo counter's box: its left end, middle, right end
                                                     * (textured quads, two for each frame) */
static void link0(u32 *ot, void *prim, u32 words) { /* (at the head of the table's first: drawn before what's there) */
    *(u32 *)prim = words << 24 | (ot[0] & 0xFFFFFF);
    ot[0] = (ot[0] & 0xFF000000) | ((u32)(long)prim & 0xFFFFFF);
}

static s32 text_width(const char *s) {
    s32 w = 0;
    for (; *s; s++)
        w += D_800AE940[((u8)*s - 0x20) * 8 + 2] + D_800AEDD4;
    return w - D_800AEDD4;
}

static u32 *hud_ot;
static s32 hud_n, hud_lvl;
/* The other primitives (tiles, modes, quads) in PS1 memory of their own: the MIPS code of the two functions this mod
 * replaces, which only their 5-byte jumps to the native ones are left of (port_ps1_addr; not the letters' spare
 * sprites: the menus' colour setting, func_80028B58, writes a text's sprites' +0xE, a tile's height) */
u32 port_ps1_addr(void *fn);
static u8 *prim_at, *prim_end;
static int prim_area;
static void prims_begin(void) {
    static const struct { void *fn; u32 size; } area[2] = {{(void *)func_800798D4, 0x2C0}, {(void *)func_80079C54, 0x118}};
    u32 a = port_ps1_addr(area[0].fn);
    prim_area = 0;
    prim_at = a ? (u8 *)(long)(a + 8 + D_800B8F24 * ((area[0].size - 8) / 2 & ~3u)) : 0;
    prim_end = a ? prim_at + ((area[0].size - 8) / 2 & ~3u) : 0;
}
static u8 *prim(u32 size) {
    if (!prim_at || prim_at + size > prim_end) {
        static const u32 sz = 0x118; /* (the smallest of the levels': 0x11C in Masquerade's) */
        u32 a = prim_area++ ? 0 : port_ps1_addr((void *)func_80079C54);
        if (!a)
            return 0;
        prim_at = (u8 *)(long)(a + 8 + D_800B8F24 * ((sz - 8) / 2 & ~3u));
        prim_end = prim_at + ((sz - 8) / 2 & ~3u);
    }
    prim_at += size;
    return prim_at - size;
}
#define CLUT_TEXT ((u16)(D_800AC4DC << 6 | (D_800AC4D8 >> 4 & 0x3F)))
#define CLUT_LIT ((u16)(D_800AEDA4 << 6 | (D_800AEDA0 >> 4 & 0x3F)))
/* the letters' sprites taken: their colour and code put back before the HUD is made again (the menus' texts set
 * neither, so a text of the game's in such a sprite would show in the strip's colour: weaponkeys_hud_pre) */
static u32 kept_word[230][2];
static u8 kept[230];
/* THE GAME'S NAME BAND, REPLACED (TWINE_WEAPON_STRIP_AT=centre): the box with the name stands in its place for as
 * long as it would show (D_8009EED0's frames: the band itself is not drawn, the count kept here), started by any
 * change (the game's own too: a weapon picked up, the room's carried weapon) */
static int strip_variant(void);
static int strip_at_where(void);
static s32 band_left, band_now; /* (the band's frames left; this drawing's) */
static int replaces_band(void) {
    int v = strip_variant();
    return (v == 'B' || v == '2') && strip_at_where() == 'c';
}
void weaponkeys_hud_pre(void) {
    int i;
    band_now = band_left;
    if (replaces_band() && D_8009EED0 > 0) { /* (a band asked: ours instead) */
        if (!port_subframe) {
            if (band_left <= 0 && (D_8009CDA8 < strip_at || D_8009CDA8 - strip_at > 20) && D_800B0DDC >= 0 &&
                D_800B0DDC < SLOTS) { /* (not one of ours: the game's own change) */
                Ent e[64];
                int run, n = list(e, &run), c = run ? current(e, n, run) : -1;
                /* (in a run, the weapon he carries that is in hand or coming: at a room's start the game's slot is
                 * still the mission's own, not shown, while the one carried in waits to be taken out) */
                strip_show(c >= 0 ? e[c].name : (const char *)g_Weapons[D_800B0DDC].pad0, e, n);
            }
            band_left = D_8009EED0;
        }
        band_now = D_8009EED0;
        D_8009EED0 = 0;
    }
    if (port_subframe) /* (an extra drawing: its changes go with it) */
        return;
    if (band_left > 0)
        band_left--;
    for (i = 0; i < 230; i++)
        if (kept[i]) {
            *(u32 *)(D_800AC550 + i * 0x28 + 4) = kept_word[i][0];
            *(u32 *)(D_800AC550 + i * 0x28 + 0x18) = kept_word[i][1];
            kept[i] = 0;
        }
}
static void text(const char *t, s32 x, s32 y, u16 clut, s32 v) { /* (the game's letters, linked) */
    s32 first = hud_n, i;
    func_800291B0((char *)t, (s16)x, (s16)y, &hud_n, 0, -1);
    for (i = first; i < hud_n && i < 230; i++) {
        u8 *p = D_800AC550 + i * 0x28 + D_800B8F24 * 0x14;
        if (!kept[i] && !port_subframe) { /* (an extra drawing's changes are put back by the runtime) */
            kept_word[i][0] = *(u32 *)(D_800AC550 + i * 0x28 + 4), kept_word[i][1] = *(u32 *)(D_800AC550 + i * 0x28 + 0x18);
            kept[i] = 1;
        }
        p[4] = p[5] = p[6] = (u8)(v * hud_lvl / 128), p[7] = 0x64; /* (a textured sprite, its colour modulated) */
        *(u16 *)(p + 0xE) = clut;
        link0(hud_ot, p, 4);
    }
}
static void tile(s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b, int code) {
    u8 *p = prim(16);
    if (!p)
        return;
    p[4] = (u8)(r * hud_lvl / 128), p[5] = (u8)(g * hud_lvl / 128), p[6] = (u8)(b * hud_lvl / 128), p[7] = (u8)code;
    *(s16 *)(p + 8) = (s16)x, *(s16 *)(p + 0xA) = (s16)y, *(s16 *)(p + 0xC) = (s16)w, *(s16 *)(p + 0xE) = (s16)h;
    link0(hud_ot, p, 3);
}
static void mode(int abr) { /* (the letters' texture page, as the menus' texts have it: func_80028C08) */
    s32 tp = (D_800AC4E4 & 0x100) >> 4 | (D_800AC4E0 & 0x3FF) >> 6 | (D_800AC4E4 & 0x200) << 2;
    u8 *p = prim(12);
    if (!p)
        return;
    *(u32 *)(p + 4) = 0xE1000000 | (tp & 0x9FF) | abr << 5, *(u32 *)(p + 8) = 0;
    link0(hud_ot, p, 2);
}
static void quad(const u8 *from, s32 x0, s32 x1, s32 y0, s32 y1) { /* (a piece of the ammo counter's box) */
    u8 *p = prim(0x28);
    if (!p)
        return;
    memcpy(p, from + D_800B8F24 * 0x28, 0x28);
    *(s16 *)(p + 8) = *(s16 *)(p + 0x18) = (s16)x0, *(s16 *)(p + 0x10) = *(s16 *)(p + 0x20) = (s16)x1;
    *(s16 *)(p + 0xA) = *(s16 *)(p + 0x12) = (s16)y0, *(s16 *)(p + 0x1A) = *(s16 *)(p + 0x22) = (s16)y1;
    p[4] = p[5] = p[6] = (u8)(0x80 * hud_lvl / 128);
    link0(hud_ot, p, 9);
}
static void box(s32 x, s32 y, s32 w, s32 h) { /* (drawn first: linked last) */
    quad(D_800D7838, x, x + 24, y, y + h);
    quad(D_800D4688, x + 24, x + w - 24, y, y + h);
    quad(D_800D77E8, x + w - 24, x + w, y, y + h);
}

/* where the box of A2/B goes (TWINE_WEAPON_STRIP_AT): centre (the default) in the game's own name band (the band
 * that names the weapon at the bottom centre as a change goes on), one box with the name over it, as long as it
 * shows; band on top of that band (the name left to it); box above the ammo counter's box; msg centred where the
 * HUD's messages show */
static int strip_at_where(void) {
    static int v = -1;
    if (v < 0) {
        const char *e = getenv("TWINE_WEAPON_STRIP_AT");
        v = !e || !*e || !strcmp(e, "centre") ? 'c' : !strcmp(e, "band") ? 'b' : !strcmp(e, "box") ? 'r' : 'm';
    }
    return v;
}
extern s32 D_8009EED0; /* the game's name band: the frames it has left (func_800463C8) */

static int strip_variant(void) { /* 'A' (and '2': A2), 'B', 'C', 0 off */
    static int v = -1;
    if (v < 0) {
        const char *e = getenv("TWINE_WEAPON_STRIP");
        v = !e || !*e || !strcmp(e, "B") ? 'B' : !strcmp(e, "A") ? 'A' : !strcmp(e, "A2") ? '2' : !strcmp(e, "C") ? 'C' : 0;
    }
    return v;
}

void weaponkeys_hud(void) {
    s32 age, x, y, w, h = 0, k, i, gap, bl = 0, bt = 0, var = strip_variant(), bx = 401, bw = 82, by = 199; /* (the ammo counter's box) */
    static int told;
    char digit[2] = {0, 0};
    int at = strip_at_where(), band = (var == 'B' || var == '2') && at == 'b',
        with_band = (var == 'B' || var == '2') && (at == 'b' || at == 'c'); /* (as long as the game's band shows) */
    if (!var || strip_at < 0 || D_8009CABF || D_8009CAA4 || g_CutScene || D_8009CD00)
        return;
    age = (D_8009CDA8 - strip_at - (port_subframe ? 1 : 0)) * 4096 + (port_subframe ? port_smooth_t : 0);
    if (with_band && (at == 'c' ? band_now : D_8009EED0) > 0 && D_8009CDA8 >= strip_at && D_8009CDA8 - strip_at < 200)
        age = 0; /* (with the game's name band, as long as it shows) */
    if (D_8009CDA8 < strip_at || age < 0 || age >= STRIP_TICKS * 4096)
        return;
    hud_lvl = age < (STRIP_TICKS - STRIP_FADE) * 4096 ? 128 : (STRIP_TICKS * 4096 - age) * 128 / (STRIP_FADE * 4096);
    if (!port_subframe && port_trace("strip")) { /* (each tick it's drawn: its name, and the gun on screen) */
        extern u8 *D_8009CD98;
        int s = D_800B0DD4;
        int boxed = D_800D7838 && D_800D4688 && D_800D77E8;
        port_log("trace: L%d f%d strip: \"%s\" drawn (level %d) as %s; in hand %d (%s), to %d, state %d, gun %s\n",
                 g_Args->level, D_8009CDA8, strip_name, hud_lvl,
                 var == 'C' ? (boxed ? "C, the list in the ammo box's pieces" : "A, the fallback (no ammo box pieces)")
                 : var == 'B' || var == '2' ? (boxed ? (var == 'B' ? "B, the ammo box's pieces with dots" : "A2, the ammo box's pieces")
                                                     : "A, the fallback (no ammo box pieces)")
                 : "A, the band", s,
                 s >= 0 && s < SLOTS ? (const char *)g_Weapons[s].pad0 : "-", D_800B0DDC, D_800F11F0,
                 !D_8009CD98 ? "none" : *(u32 *)(D_8009CD98 + 0x7C) & 0x80000000 ? "hidden" : "shown");
    }
    hud_ot = (u32 *)D_800B8EFC[D_800B8F24][1];
    prims_begin();
    hud_n = D_800AEDA8; /* (the letters past those the menus use: the count, or past the last text's (the game's name
                         * band registers its text without counting its letters)) */
    if (D_800AC54C > 0 && D_800AC54C <= 40 && *(u16 *)(D_800AB744 + (D_800AC54C - 1) * 0x60 + 4) + 1 > hud_n)
        hud_n = *(u16 *)(D_800AB744 + (D_800AC54C - 1) * 0x60 + 4) + 1;
    if (hud_n < 0 || hud_n + (var == 'C' ? 40 : 7 + 26 + 24) > 230) {
        if (told++ < 3)
            port_log("weaponkeys: no letters left for the strip (%d in use)\n", hud_n);
        return;
    }
    if (g_Args->displayW > 0)
        bx = g_Args->displayW - 0x6F; /* (as func_800456D0 places it on the 512 wide screen: 401) */
    if (var == 'C' && D_800D7838 && D_800D4688 && D_800D77E8) { /* the list, as the INVENTORY's rows (12 apart, the mark's column, the chosen one lit) */
        s32 n = 0, wmax = 0, need = hud_n + 8;
        while (n < strip_n && (need += (s32)strlen(strip_list[n].name) + 2) < 230) /* (as many as there are letters for) */
            n++;
        for (i = 0; i < n; i++)
            if (text_width(strip_list[i].name) > wmax)
                wmax = text_width(strip_list[i].name);
        w = wmax + 40 > bw ? wmax + 40 : bw;
        h = n * 12 + 8;
        x = bx + bw - w, y = by - 2 - h;
        for (i = 0; i < n; i++) {
            digit[0] = (char)('0' + strip_list[i].group);
            text(digit, x + 10 - text_width(digit) / 2, y + 5 + i * 12, strip_list[i].chosen ? CLUT_LIT : CLUT_TEXT, strip_list[i].chosen ? 128 : 72);
            text(strip_list[i].ticked ? "*" : "=", x + 16, y + 5 + i * 12, CLUT_TEXT, 128);
            text(strip_list[i].name, x + 32, y + 5 + i * 12, strip_list[i].chosen ? CLUT_LIT : CLUT_TEXT, 128);
        }
        mode(0);
        box(x, y, w, h);
        return;
    }
    if (!D_800D7838 || !D_800D4688 || !D_800D77E8) /* (a mission whose names for its box aren't known: the band) */
        var = var == 'C' ? 'A' : 'A';
    gap = var == 'A' ? 18 : 14;
    w = text_width(strip_name);
    if (var == 'A') {
        x = (g_Args->displayW ? g_Args->displayW / 2 : 256) - 3 * gap, y = 190;
        w = w > 7 * gap ? w : 7 * gap;
    } else {
        s32 cx = g_Args->displayW ? g_Args->displayW / 2 : 256;
        w = band ? 7 * gap + 16 : w + 18 > 7 * gap + 16 ? w + 18 : 7 * gap + 16; /* (the game's band: the name's + 17) */
        h = band ? (var == 'B' ? 18 : 15) : var == 'B' ? 32 : 28;
        if (at == 'r')
            w = w > bw ? w : bw, bl = bx + bw - w, bt = by - 2 - h; /* (its right end the ammo counter's) */
        else if (at == 'b')
            bl = cx - w / 2, bt = 0xC4 - 1 - h; /* (on the game's name band: func_800463C8's, y 0xC4-0xD4) */
        else if (at == 'c')
            bl = cx - w / 2, bt = 0xC4 + 0x10 - h; /* (its bottom the band's) */
        else
            bl = cx - w / 2, bt = 0xAC - h / 2; /* (the HUD messages' row: func_80048E60's y 0xAC) */
        x = bl + w / 2 - 3 * gap;
        y = bt + 5;
    }
    /* the groups' numbers; the lit one in the highlight's colour, those he has nothing of dim */
    for (k = 1; k <= 7; k++) {
        int lit = k == strip_group, have = strip_have >> k & 1;
        digit[0] = (char)('0' + k);
        text(digit, x + (k - 1) * gap - text_width(digit) / 2, y, lit ? CLUT_LIT : CLUT_TEXT, lit || have ? 128 : 56);
    }
    if (var == 'B') { /* a dot for each weapon of the group, the chosen one's lit */
        for (k = 1; k <= 7; k++) {
            s32 c = 0, j = 0, dx;
            for (i = 0; i < strip_n; i++)
                c += strip_list[i].group == k;
            dx = x + (k - 1) * gap - (c * 4 - 1) / 2 + 1;
            for (i = 0; i < strip_n; i++)
                if (strip_list[i].group == k) {
                    if (strip_list[i].chosen)
                        tile(dx + j * 4, y + 10, 3, 2, 216, 176, 80, 0x60);
                    else
                        tile(dx + j * 4, y + 10, 3, 2, 150, 150, 150, 0x60);
                    j++;
                }
        }
    } else /* a bar under the lit number */
        tile(x + (strip_group - 1) * gap - 4, y + 9, 9, 1, 216, 176, 80, 0x60);
    /* the weapon's name under them, centred */
    k = text_width(strip_name);
    if (var == 'A')
        text(strip_name, x + 3 * gap - k / 2, y + 12, CLUT_TEXT, 128);
    else if (!band)
        text(strip_name, bl + w / 2 - k / 2, y + (var == 'B' ? 15 : 12), CLUT_TEXT, 128);
    mode(0);
    if (var == 'A') { /* the dark band behind (subtracted from the picture), and its mode */
        tile(x + 3 * gap - w / 2 - 8, y - 4, w + 16, 28, 72, 72, 72, 0x62);
        mode(2);
    } else
        box(bl, bt, w, h);
}
