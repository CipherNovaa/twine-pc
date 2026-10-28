/* Tracing (native PC build only): what the game does, frame by frame, as "trace:" lines in port.log, for proving
 * behaviour (the suite's checks, tools/weapon_trace.py) and for bug reports. Off unless TWINE_TRACE names it (a list:
 * TWINE_TRACE=weapon,change or all); off, each frame costs a test of one flag.
 *
 * weapon  Bond's weapon and its first-person model each frame anything of it changes: the slot in hand (D_800B0DD4),
 *         the one before a change (D_800B0DD8) and the one to change to (D_800B0DDC), the weapon's state (D_800F11F0),
 *         the model type its slot names (+0x5B) and the one loaded (D_800E2F58; loading D_800F0F5C), the model's
 *         animation (its instance, the gun object's +0x114: +0x88 the one asked for, +0x78 the one running, +0x80
 *         frames left of it; its first track's frame +0x10, length +0xC, speed +0xD, start +0x16), the muzzle flash
 *         (the model's unit (flags >> 15) & 15: shown when its flag is 0), clip and reserve, fire held (D_800DDEE8),
 *         the fire timer (D_8009CF48), a menu over the mission (D_8009CAA4).
 * change  each change of the weapon to change to, and who asked: a mod (trace_change_by: endless's carry, pool,
 *         inventory; the number keys) or the weapon buttons (pressed that frame), else the game.
 * person  each person of type TWINE_TRACE_TYPE (default: all) whenever their script's place (its pc, from the
 *         script's start), wait, animation (asked +0x10, running +0x12, frame +0x14) or the object's flags (+0xA4)
 *         change: "trace: L<level> f<frame> person <slot> type <t>: pc ... wait ... anim ...".
 * proj    the objects that aren't people nor pickups (projectiles, blasts, smoke, flashes: kinds 1-3, 5, 7, 8): each one as
 *         it appears (its index, kind +5, type +7 and that type's entry in the mission's table, g_Args+0x228, 0x70
 *         bytes: +0xB the object it brings along, +0x2D, +0x5C its sound; its model D_800D2D30[type]; +0x3A, +0x0E,
 *         +0x68; who made it +0x88; where), a projectile (kind 1) each frame as it flies, and each as it goes (how
 *         long it lived, where). For proving a lent launcher's rocket or round as at home (tools/lent_matrix.py).
 * flash   the flash bang (Courier's, lent or at home; Flashpoint's): each frame of weapon state 15, its counter (Courier's
 *         D_800A1198, Flashpoint's D_8009EF40, a lent one's endless_flash_count()), the screen fade (D_800D8150 on,
 *         its colour D_8009D008, frames D_8009D00C) and its two prims' see-through bit (D_800D77B8 +7 bit 1); each
 *         person flagged 0x40000 by the AI (seen at the frame's top: the AI's flags of the frame before, +0 or +4)
 *         and, for 30 s after, each change of his animation (the one asked, +0x12, and the one it blends from, +0x10:
 *         their numbers, frames and a hash of their data, the same in every mission for the same animation), its rate
 *         (+0x20), root motion off (+3), his combat state (the script's vars[7], +0x3C), his script's place (+0xA4
 *         from its start +0xA0, or "mod" in the mod's memory).
 *         For proving the lent flash bang as at home (tools/flash_trace.py).
 * strip   the HUD's weapon strip (port/mods/weaponkeys.c) each tick it is drawn: the name it shows, its fade, the
 *         weapon in hand and to change to, the weapon's state, the gun model shown or hidden; and a menu over the
 *         mission opened or closed (here): "trace: L<level> f<frame> strip: ...".
 * hits    each bullet that reaches a person, its damage, health before and after, a hit that does nothing and why,
 *         health put back, people hit 5, 10, ... times still up; path: sent guards hunting Bond that hardly move in
 *         3 s (port/mods/endless.c's func_80034A48, hits_frame).
 * (sound: port/mods/endless.c's func_80026F30 and the runtime's port_tone_play / port_voice_play log under "sound".)
 *
 * The overlay (debug menu, F9: its WEAPON TRACE page is the mod debugmenu.c's) shows the weapon line live.
 *
 * Names are LEVEL01's; port/mods.py maps them onto each level. */
#include "twine.h"

int snprintf(char *s, unsigned n, const char *fmt, ...);
int port_trace(const char *what);
void port_log(const char *fmt, ...);
int strcmp(const char *a, const char *b);
extern int port_subframe;
const char *endless_model_file(void) __attribute__((weak));

extern s16 D_800B0DD4, D_800B0DD8, D_800B0DDC; /* the weapon in hand, the one before a change, the one to change to */
extern s32 D_800F11F0;  /* the weapon's state */
extern u8 *D_8009CD98;  /* the first-person gun object (+0x114 its model instance) */
extern u8 D_800E2F58;   /* the model type loaded */
extern s32 D_800F0F5C;  /* a model loading */
extern s32 D_800DDEE8;  /* fire held */
extern s16 D_8009CF48;  /* the fire timer */
extern s32 D_800DDE08, D_800DDDA4; /* the weapon buttons, previous and next, pressed this frame */
extern s32 D_8009CDA8;  /* the mission's frame */
extern s16 D_8009CAA4;  /* a menu over the mission */

static int on_weapon = -1, on_change, on_person, on_proj, on_flash;
extern u8 *D_800AFFC8;  /* the objects (0x124 bytes each) */
extern s16 D_800B0AA8;  /* how many */
extern s32 D_800D2D30[]; /* the object types' models */
extern void *g_Player;
extern u8 D_800D30D8[]; /* the blasts (6, 0x24 bytes each) */
static const char *change_by; /* (a mod's, this frame) */
static s16 last_to = -1;
static char last_line[256];
char trace_weapon_line[256]; /* (the debug menu's page) */

/* a mod about to set the weapon to change to (D_800B0DDC) */
void trace_change_by(const char *who) {
    change_by = who;
}

/* the weapon line (in trace_weapon_line) */
void trace_weapon_state(void) {
    u8 *m = D_8009CD98 ? *(u8 **)(D_8009CD98 + 0x114) : NULL, *tr = NULL;
    Weapon *w = D_800B0DD4 >= 0 && D_800B0DD4 < 16 ? &g_Weapons[D_800B0DD4] : NULL;
    int k = w ? (w->flags >> 15) & 15 : 0, flash = -1;
    if (m && *(s32 *)(m + 0x6C) > 0)
        tr = (*(u8 ***)(m + 0x70))[0];
    if (m && k > 0 && (u32)k < *(u32 *)(m + 0xC) && (*(s32 **)(m + 4))[k * 2])
        flash = !(*(u8 **)(m + 8))[k];
    snprintf(trace_weapon_line, sizeof(trace_weapon_line),
             "slot %d (before %d, to %d) %s, state %d, model %d loaded %d%s file %s, anim %d asked %d left %d, track frame %d "
             "len %d speed %d start %d, flash %s, clip %d reserve %d, fire %d timer %d, rate %d/%d%s",
             D_800B0DD4, D_800B0DD8, D_800B0DDC, w ? (char *)w->pad0 : "-", D_800F11F0, w ? ((u8 *)w)[0x5B] : -1,
             D_800E2F58, D_800F0F5C ? " (loading)" : "", endless_model_file ? endless_model_file() : "?", m ? *(s32 *)(m + 0x78) : -9, m ? *(s32 *)(m + 0x88) : -9,
             m ? *(s32 *)(m + 0x80) : -9, tr ? *(s16 *)(tr + 0x10) : -9, tr ? tr[0xC] : -9, tr ? (s8)tr[0xD] : -9,
             tr ? *(u16 *)(tr + 0x16) : -9, flash < 0 ? "none" : flash ? "SHOWN" : "hidden", w ? w->clip : 0,
             w ? w->reserve : 0, D_800DDEE8 != 0, D_8009CF48, g_FrameRate, g_FrameCap, D_8009CAA4 ? ", menu" : "");
}


/* TWINE_TEST_TONE_AB=id (tests, in any mission): that sound of the mission's, three ways 3 s apart from frame 100: the
 * game's own, as a lent weapon's is played (a voice as the SPU's, tones.c), and the old raw PCM: for comparing
 * recordings (TWINE_WAV, with TWINE_TEST_NO_CD) */
typedef struct { u8 prog, tone, note, voice, vol, rnd, pad[2]; } SfxDefT;
s32 func_80026F30(u32 id, s32 flags);
int port_tone_play(int level, int program, int tone, int note, s32 l, s32 r, int loop);
int port_tone_play_pcm(int level, int program, int tone, int note, s32 vol, int loop);
char *getenv(const char *name);
extern u8 *D_800B0AB4;   /* the model effects (muzzle flashes: 0x34 bytes each) */
extern s16 D_800B0C84;   /* how many */
int atoi(const char *s);
static void tone_ab(void) {
    static int on = -1;
    u32 id;
    SfxDefT *d = *(SfxDefT **)((u8 *)g_Args + 0x130);
    s32 vol, f = D_8009CDA8;
    if (on < 0)
        on = getenv("TWINE_TEST_TONE_AB") != 0;
    if (!on || !d || (f != 100 && f != 160 && f != 220))
        return;
    id = (u32)atoi(getenv("TWINE_TEST_TONE_AB"));
    if (id >= 0x80)
        return;
    vol = *(s16 *)((u8 *)g_Args + 0x13A) * d[id].vol / 127;
    port_log("trace: L%d f%d sound test: %u (program %d tone %d note %d) %s\n", g_Args->level, f, id, d[id].prog,
             d[id].tone, d[id].note, f == 100 ? "the game's own" : f == 160 ? "as lent" : "as raw PCM");
    if (f == 100)
        func_80026F30(id, 0);
    else if (f == 160)
        port_tone_play(g_Args->level, d[id].prog, d[id].tone, d[id].note, vol, vol, 0);
    else
        port_tone_play_pcm(g_Args->level, d[id].prog, d[id].tone, d[id].note, vol * 0x7F, 0);
}

/* people's scripts and animations (TWINE_TRACE=person) */
extern s16 D_800B0DA0, D_800B07B0;
static void trace_people(void) {
    static int type = -2;
    static u32 last[64][7];
    int i;
    if (type == -2)
        type = getenv("TWINE_TRACE_TYPE") ? atoi(getenv("TWINE_TRACE_TYPE")) : -1;
    for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) {
        u8 *o = (u8 *)D_800AFFC8 + i * 0x124, *an = *(u8 **)(o + 0x94), *ctx = *(u8 **)(o + 0x118);
        u32 now[7];
        if (!o[4] || o[5] != 6 || (type >= 0 && o[6] != type))
            continue;
        now[0] = ctx ? *(u32 *)(ctx + 0xA4) - *(u32 *)(ctx + 0xA0) : 0xFFFFFFFF;
        now[1] = ctx ? *(u32 *)(ctx + 0x1C4) : 0;
        now[2] = an ? (u32)(u16)*(s16 *)(an + 0x10) << 16 | (u16)*(s16 *)(an + 0x12) : 0;
        now[3] = an ? *(u16 *)(an + 0x14) : 0;
        now[4] = *(u32 *)(o + 0xA4);
        now[5] = *(u32 *)(o + 0x34);
        now[6] = ctx ? *(u32 *)ctx : 0;
        if (now[0] == last[i][0] && now[2] == last[i][2] && now[4] == last[i][4] &&
            now[5] == last[i][5] && now[6] == last[i][6])
            continue;
        port_log("trace: L%d f%d person %d type %d: pc %04X wait %d anim %d/%d frame %d flags %08X health %d script flags %08X\n",
                 g_Args->level, D_8009CDA8, i, o[6], now[0], (s32)now[1], (s16)(now[2] >> 16), (s16)now[2], now[3],
                 now[4], (s32)now[5], now[6]);
        { int k; for (k = 0; k < 7; k++) last[i][k] = now[k]; }
    }
}

static int proj_all(void) { /* (TWINE_TRACE_PROJ_ALL=1: every kind of object, people and effects too: research) */
    static int on = -1;
    if (on < 0)
        on = getenv("TWINE_TRACE_PROJ_ALL") != 0;
    return on;
}

/* TWINE_TRACE=proj: the objects that aren't people nor pickups, as they come, fly and go */
static void trace_proj(void) {
    static s32 seen[256], fresh = 0x7FFFFFFF;
    static s32 at[256][3];
    int i, n = D_800B0AA8 < 256 ? D_800B0AA8 : 256;
    if (!D_800AFFC8)
        return;
    if (D_8009CDA8 < fresh) { /* (a new mission) */
        int k;
        for (k = 0; k < 256; k++)
            seen[k] = -1;
    }
    fresh = D_8009CDA8;
    for (i = 1; i < n; i++) {
        u8 *o = D_800AFFC8 + i * 0x124, *m = *(u8 **)(o + 0x90);
        int kind = o[5];
        s32 born = *(s32 *)(o + 0x2C);
        if (!o[4] || ((kind == 6 || kind == 4 || kind == 9 || kind == 0 || kind == 10) && !proj_all()) || !m) {
            if (seen[i] >= 0)
                port_log("trace: L%d f%d proj: #%d gone, lived %d frames, last at %d %d %d\n", g_Args->level, D_8009CDA8, i,
                         D_8009CDA8 - seen[i], at[i][0], at[i][1], at[i][2]);
            seen[i] = -1;
            continue;
        }
        if (seen[i] != born) {
            u8 *te = (u8 *)(long)g_Args->unk228 + o[7] * 0x70;
            if (seen[i] >= 0)
                port_log("trace: L%d f%d proj: #%d gone, lived %d frames, last at %d %d %d\n", g_Args->level, D_8009CDA8, i,
                         D_8009CDA8 - seen[i], at[i][0], at[i][1], at[i][2]);
            u8 *ent = *(u8 **)o; /* (its type's entry, 0x70 bytes) */
            port_log("trace: L%d f%d proj: #%d new kind %d type %d/%d (entry %02X%02X%02X%02X %02X%02X%02X%02X %02X%02X%02X%02X "
                     "+5C %d +5D %d, model %08X) 3A %d 3B %d 0E %04X 34 %d 64 %08X 68 %08X by %s at %d %d %d\n",
                     g_Args->level, D_8009CDA8, i, kind, o[6], o[7], ent ? ent[0] : 0, ent ? ent[1] : 0,
                     ent ? ent[2] : 0, ent ? ent[3] : 0, ent ? ent[4] : 0, ent ? ent[5] : 0, ent ? ent[6] : 0,
                     ent ? ent[7] : 0, ent ? ent[8] : 0, ent ? ent[9] : 0, ent ? ent[10] : 0, ent ? ent[11] : 0,
                     ent ? ent[0x5C] : 0, ent ? ent[0x5D] : 0, (u32)D_800D2D30[o[6]], o[0x3A], o[0x3B],
                     *(u16 *)(o + 0xE), *(s32 *)(o + 0x34), *(u32 *)(o + 0x64), *(u32 *)(o + 0x68),
                     *(void **)(o + 0x88) == g_Player ? "Bond" : *(void **)(o + 0x88) ? "another" : "none",
                     *(s32 *)(m + 0x28), *(s32 *)(m + 0x2C), *(s32 *)(m + 0x30));
            if (*(void **)(o + 0x88) == g_Player && kind == 1) { /* (Bond's: the object, its type's entry, its model's
                                                                  * start, in hex: for comparing home and lent) */
                char hex[0x124 * 2 + 1];
                int k;
                for (k = 0; k < 0x124; k++)
                    snprintf(hex + k * 2, 3, "%02X", o[k]);
                port_log("trace: L%d f%d proj: #%d object %s\n", g_Args->level, D_8009CDA8, i, hex);
                for (k = 0; k < 0x70 && ent; k++)
                    snprintf(hex + k * 2, 3, "%02X", ent[k]);
                port_log("trace: L%d f%d proj: #%d entry %s\n", g_Args->level, D_8009CDA8, i, ent ? hex : "-");
                for (k = 0; k < 0x8C; k++)
                    snprintf(hex + k * 2, 3, "%02X", m[k]);
                port_log("trace: L%d f%d proj: #%d part %s\n", g_Args->level, D_8009CDA8, i, hex);
            }
            seen[i] = born;
        } else if (kind == 1)
            port_log("trace: L%d f%d proj: #%d at %d %d %d (0E %04X 64 %08X 68 %08X model %08X)\n", g_Args->level,
                     D_8009CDA8, i, *(s32 *)(m + 0x28), *(s32 *)(m + 0x2C), *(s32 *)(m + 0x30), *(u16 *)(o + 0xE),
                     *(u32 *)(o + 0x64), *(u32 *)(o + 0x68), *(u32 *)(m + 8));
        at[i][0] = *(s32 *)(m + 0x28), at[i][1] = *(s32 *)(m + 0x2C), at[i][2] = *(s32 *)(m + 0x30);
    }
    if (proj_all() && D_800B0AB4) { /* (research: the model effects, muzzle flashes: each one as it's made) */
        static s32 eff_made[64];
        for (i = 0; i < D_800B0C84 && i < 64; i++) {
            u8 *e = D_800B0AB4 + i * 0x34, *pt = *(u8 **)(e + 0x1C);
            if (e[1] && pt && *(s32 *)(e + 0x18) != eff_made[i]) {
                eff_made[i] = *(s32 *)(e + 0x18);
                port_log("trace: L%d f%d proj: effect %d made (tick %d) model %08X at %d %d %d, +0 %02X%02X%02X%02X\n",
                         g_Args->level, D_8009CDA8, i, eff_made[i], *(u32 *)(pt + 8), *(s32 *)(pt + 0x28),
                         *(s32 *)(pt + 0x2C), *(s32 *)(pt + 0x30), e[0], e[1], e[2], e[3]);
            }
        }
    }
    if (proj_all()) { /* (research: the sprites (smoke, fire, flashes: 30 of 0xA0 bytes), each one as it's made: its
                       * definition (+0x50), place, size, and the record's first bytes) */
        extern u8 D_800DB884[];
        static s32 spr_made[30];
        for (i = 0; i < 30; i++) {
            u8 *e = D_800DB884 + i * 0xA0;
            if (e[0x9B] && *(s32 *)(e + 0x94) != spr_made[i]) {
                u8 *d = *(u8 **)(e + 0x50);
                char hex[0x40 * 2 + 1];
                int k;
                spr_made[i] = *(s32 *)(e + 0x94);
                for (k = 0; k < 0x40; k++)
                    snprintf(hex + k * 2, 3, "%02X", d && (u32)(long)d >= 0x80000000u ? d[k] : 0);
                port_log("trace: L%d f%d proj: sprite %d made (tick %d) def %08X at %d %d %d size %d %d; def %s\n",
                         g_Args->level, D_8009CDA8, i, spr_made[i], (u32)(long)d, ((s16 *)(e + 0x54))[0],
                         ((s16 *)(e + 0x54))[1], ((s16 *)(e + 0x54))[2], *(s16 *)(e + 0x78), *(s16 *)(e + 0x7A), hex);
            } else if (!e[0x9B])
                spr_made[i] = 0;
        }
    }
    { /* the blasts (func_800378FC's six: a blast's reach grows over its frames, func_80037B6C hurts within it) */
        static s32 blast_time[6];
        for (i = 0; i < 6; i++) {
            u8 *b = D_800D30D8 + i * 0x24;
            if ((*(u32 *)(b + 0x10) & 1) && *(s32 *)(b + 0x20) != blast_time[i])
                port_log("trace: L%d f%d proj: blast %d at %d %d %d by %s, reach %d to %d, %d a frame\n", g_Args->level,
                         D_8009CDA8, i, *(s32 *)(b + 4), *(s32 *)(b + 8), *(s32 *)(b + 0xC),
                         *(void **)b == g_Player ? "Bond" : *(void **)b ? "another" : "none", *(u16 *)(b + 0x14),
                         *(u16 *)(b + 0x16), *(s16 *)(b + 0x18));
            blast_time[i] = (*(u32 *)(b + 0x10) & 1) ? *(s32 *)(b + 0x20) : 0;
        }
    }
}

/* TWINE_TRACE=flash: state 15, the fade, the people flagged by it and their animations after */
s32 endless_flash_count(void) __attribute__((weak));
extern s8 D_800D8150;   /* the fade on */
extern u8 D_8009D008[]; /* its colour */
extern s16 D_8009D00C;  /* its frames */
extern u8 D_800D77B8[]; /* its two prims */
extern u8 **D_800DDEE4; /* the people's animations (frames of 0x4C bytes, the first u16 their count) */
extern s16 D_800B0DA0, D_800B07B0; /* the first and last person */
extern u8 *D_800DDEE0;  /* (their root motion's table, after it) */
int endless_anim_count(void) __attribute__((weak)); /* (the table's count while endless.c's copy is in use) */
static u32 anim_hash(s32 id, u32 *frames) {
    s32 n = endless_anim_count ? endless_anim_count() : -1;
    u8 *e;
    u32 h = 2166136261u, j, fr;
    if (n < 0)
        n = (u32)(long)D_800DDEE4 - 0x80000000u < 0x800000u ? (s32)((u8 **)(void *)D_800DDEE0 - D_800DDEE4) : 0;
    e = id >= 0 && id < n && n < 600 ? D_800DDEE4[id] : 0;
    *frames = 0;
    if (!e)
        return 0;
    fr = *(u16 *)e;
    *frames = fr;
    for (j = 2; j < fr * 0x4C && j < 0x20000; j++)
        h = (h ^ e[j]) * 16777619u;
    return h;
}
static void trace_flash(void) {
    static s32 follow[64], last[64][5], flagged[64], fresh = 0x7FFFFFFF, after = -1;
    s32 c = -99;
    int i;
    if (D_8009CDA8 < fresh) {
        for (i = 0; i < 64; i++)
            follow[i] = -1, flagged[i] = -9;
        after = -1;
    }
    fresh = D_8009CDA8;
    if (endless_flash_count && endless_flash_count() != -99)
        c = endless_flash_count();
    else if (g_Args->level == 2)
        c = *(s16 *)0x800A1198u;
    else if (g_Args->level == 6)
        c = *(s16 *)0x8009EF40u;
    if (D_800F11F0 == 15)
        after = D_8009CDA8 + 4;
    if (D_800F11F0 == 15 || D_8009CDA8 <= after)
        port_log("trace: L%d f%d flash: weapon state %d counter %d, fade %s %02X %02X %02X for %d, prims %s %s\n",
                 g_Args->level, D_8009CDA8, D_800F11F0, c, D_800D8150 ? "on" : "off", D_8009D008[0], D_8009D008[1],
                 D_8009D008[2], D_8009D00C, D_800D77B8[7] & 2 ? "see-through" : "solid",
                 D_800D77B8[0x18 + 7] & 2 ? "see-through" : "solid");
    if (!D_800AFFC8)
        return;
    for (i = D_800B0DA0; i <= D_800B07B0 && i < 64; i++) {
        u8 *o = D_800AFFC8 + i * 0x124, *ctx = *(u8 **)(o + 0x118), *an = *(u8 **)(o + 0x94);
        s32 k[5];
        if (!o[4] || o[5] != 6 || !ctx || !an || o == (u8 *)g_Player || *(s32 *)(o + 0x34) <= 0) {
            if (follow[i] >= D_8009CDA8)
                port_log("trace: L%d f%d flash: #%d gone (out or down)\n", g_Args->level, D_8009CDA8, i);
            follow[i] = -1;
            continue;
        }
        if (((*(u32 *)ctx | *(u32 *)(ctx + 4)) & 0x40000) && D_8009CDA8 - flagged[i] > 1) { /* (+4: the flags of the
                                                                              * frame before, func_8005772C) */
            flagged[i] = D_8009CDA8;
            port_log("trace: L%d f%d flash: #%d type %d flagged 0x40000 (distance %d, combat state %d, health %d)\n",
                     g_Args->level, D_8009CDA8, i, o[6], *(s32 *)(o + 0x110), *(s32 *)(ctx + 0x3C), *(s32 *)(o + 0x34));
            if (follow[i] < D_8009CDA8)
                last[i][1] = -1;
            for (k[0] = 0; k[0] < 32 && (u32)k[0] < *(u32 *)(ctx + 0xAC); k[0]++) { /* (his script's interrupts) */
                u32 *item = (u32 *)(ctx + 0xB0 + k[0] * 8), *cmd = (u32 *)(long)item[1];
                if (cmd)
                    port_log("trace: L%d f%d flash: #%d interrupt %d %s: %08X %08X %08X %08X\n", g_Args->level,
                             D_8009CDA8, i, k[0], item[0] ? "off" : "on", cmd[0], cmd[1], cmd[2], cmd[3]);
            }
            follow[i] = D_8009CDA8 + 600;
        }
        if (follow[i] < D_8009CDA8)
            continue;
        k[0] = *(s16 *)(an + 0x10), k[1] = *(s16 *)(an + 0x12), k[2] = *(s32 *)(an + 0x20), k[3] = an[3];
        k[4] = *(s32 *)(ctx + 0x3C);
        if (k[1] != last[i][1] || k[2] != last[i][2] || k[3] != last[i][3] || k[4] != last[i][4]) { /* (not the blend's
                                                                                * from: it follows playback) */
            u32 f0, f1, h0 = anim_hash(k[0], &f0), h1 = anim_hash(k[1], &f1), pc = *(u32 *)(ctx + 0xA4),
                base = *(u32 *)(ctx + 0xA0);
            char at[16];
            if (pc - base < 0x10000)
                snprintf(at, sizeof(at), "+%X", (pc - base) / 4);
            else
                snprintf(at, sizeof(at), "mod");
            port_log("trace: L%d f%d flash: #%d anim %d (%u frames %08X) -> %d (%u frames %08X) rate %d keep %d, combat "
                     "state %d, script %s\n", g_Args->level, D_8009CDA8, i, k[0], f0, h0, k[1], f1, h1, k[2], k[3], k[4], at);
            last[i][0] = *(s16 *)(an + 0x10), last[i][1] = k[1], last[i][2] = k[2], last[i][3] = k[3], last[i][4] = k[4];
        }
    }
}

/* each frame, at the top of the mission's frame loop (mods/savestate.c) */
void trace_frame(void) {
    static s32 last_frame = -99999;
    if (port_subframe || (D_8009CDA8 == last_frame && !D_8009CAA4)) /* (once a frame: not in smooth motion's extra
                                                                         * drawings; in a menu, whose frames don't count,
                                                                         * each time) */
        return;
    last_frame = D_8009CDA8;
    tone_ab();
    if (on_weapon < 0) {
        on_weapon = port_trace("weapon");
        on_change = port_trace("change");
        on_person = port_trace("person");
        on_proj = port_trace("proj");
        on_flash = port_trace("flash");
    }
    {   /* (TWINE_TRACE=strip, weaponkeys.c's HUD strip: a menu over the mission opened and closed, for the strip after) */
        static int menu_was, on_strip = -1;
        if (on_strip < 0)
            on_strip = port_trace("strip");
        if (on_strip && (D_8009CAA4 != 0) != menu_was)
            port_log("trace: L%d f%d strip: a menu %s\n", g_Args->level, D_8009CDA8, (menu_was = D_8009CAA4 != 0) ? "open" : "closed");
    }
    if (on_flash)
        trace_flash();
    if (on_person)
        trace_people();
    if (on_proj)
        trace_proj();
    if (on_change && D_800B0DDC != last_to) {
        port_log("trace: L%d f%d change: to %d (%s) from %d, in hand %d, by %s\n", g_Args->level, D_8009CDA8, D_800B0DDC,
                 D_800B0DDC >= 0 && D_800B0DDC < 16 ? (char *)g_Weapons[D_800B0DDC].pad0 : "-", last_to, D_800B0DD4,
                 change_by ? change_by : D_800DDE08 ? "the previous-weapon button" : D_800DDDA4 ? "the next-weapon button"
                                                                                       : "the game");
    }
    last_to = D_800B0DDC;
    change_by = NULL;
    if (on_weapon) {
        trace_weapon_state();
        if (strcmp(trace_weapon_line, last_line)) {
            port_log("trace: L%d f%d weapon: %s\n", g_Args->level, D_8009CDA8, trace_weapon_line);
            snprintf(last_line, sizeof(last_line), "%s", trace_weapon_line);
        }
    }
}
