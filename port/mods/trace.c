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

static int on_weapon = -1, on_change;
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
    }
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
