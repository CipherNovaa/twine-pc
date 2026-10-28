/* Quick grenade (native PC build only): G, or R1 on a gamepad, throws a frag grenade while Bond keeps the weapon he
 * holds: the same grenade, sound and ammo as throwing it from the hand, without the grenade drawn.
 *
 * Only King's Ransom has frag grenades (a slot named FRAG GRENADE, owned when its flags have 0x1800). Firing it
 * (its fire function, the slot's +0x3C, sets the weapon's state 0x18) plays the throw: sound 0x52 ten frames in,
 * and at the frame that lets go, between PushMatrix and PopMatrix, the grenade made from the object in Bond's
 * hands (0x80077E98, the level's func_80076CDC: from its model's position and the weapon's +0x3C offset, an object
 * of the type at its +0x3A, for the slot in hand, which takes it off the clip of one). That type is the hand's
 * model's (func_8004C368: +0x2C of the model's entry in D_8009D0FC, the model the slot's +0x5B), 31 for the
 * grenade. LEVEL01's func_80076CDC differs, so the level's is called at its address (where the port puts a jump to
 * each function), with the grenade's slot and type lent to the weapon in hand for the call. (The satchel charges'
 * throw, 0x800395EC, is another.) The next is taken from the reserve into the clip, and a throw that leaves
 * none takes the grenade out of Bond's weapons, as the hand's does. With the grenade in hand, it is the ordinary throw.
 *
 * Few grenades lie about in the mission, so Bond starts it with four (topped up to four if he has fewer).
 *
 * Grenades also reach further, thrown either way: a blast's size grows to the end radius of the level's blast table
 * (D_800D31C0, +2: 2500, set when the level starts; func_800378FC makes a blast from it, func_80037B6C hurts what is
 * within it, axis by axis, plus the target's size), which is made bigger here (the level's other blasts too); and
 * guards notice a live grenade (func_80076A8C, every tick of one: a guard within 5000 of it, plus his size, who
 * can see it, is told to shout and run) further off, a copy of it with a bigger distance.
 *
 * The HUD shows the grenades Bond has, while he has some and the grenade isn't in hand (when it is, the ammo
 * counter shows them): a small grenade drawn in pixels, and the count in the ammo counter's digits, under the
 * health bar. The ammo counter (func_80045A28) draws its digits as sprites; the count is one more, copied from its
 * first, and the grenade flat rectangles, added to the HUD's (func_800463C8). The GPU reads them from PS1 memory:
 * the ammo counter's own MIPS code, which only its 5-byte jump to the native function is left of (port_ps1_addr).
 *
 * quick_grenade is called by the quick punch's func_800788E0 (port/mods/quickpunch.c) every tick, after the
 * weapon's state machine.
 *
 * MOD_REPLACES_NATIVE: func_80076A8C func_800463C8
 *
 * Names are LEVEL01's; port/mods.py maps them onto each level. */
#include "twine.h"
#include <string.h>

#define W(w, off, type) (*(type *)((u8 *)(w) + (off)))

extern u8 *D_8009CD98;      /* the weapon in Bond's hands (an object) */
extern s16 D_800B0DD4;      /* the weapon in hand (its slot in g_Weapons) */
extern u32 D_800F11F0;      /* the weapon's state */
extern s32 D_8009CDA8;      /* the frame counter (the mission's) */
extern u8 *D_8009D0FC;      /* the first-person models' tables (0x70 bytes each) */
extern u8 D_800D31C0[];     /* the level's blast table: +2 the end radius */
extern s32 D_800B0DA0;      /* the objects' array (0x124 bytes each), and the first and last of the people in it */
extern u8 *D_800AFFC8;
extern s32 D_800B07B0;
extern s16 D_800B0DDC;      /* the weapon to change to (the one in hand when none is changing) */
extern u8 D_800D7588[];     /* the ammo counter's sprites (7 for each of two frames) */
extern s32 D_800B8F24;      /* the frame being made (0 or 1) */
extern u8 D_800AE9C0[];     /* the ammo counter's digits' places in the texture: u, v, width (8 bytes each) */
extern s16 D_800AEDD4;      /* the gap between them */
u32 port_ps1_addr(void *fn);
u32 func_8005397C(s32 a, s32 b);
/* weak: Meltdown has no such function, nor the one using it (port/mod_names.txt) */
s32 func_80051728(s32 obj, s32 arg1) __attribute__((weak));
void func_800463C8_orig(void);
void func_80045A28(s32 ot);
extern u32 D_800B8EFC[2][5]; /* the HUD's ordering tables (GsOT: +4 its table) */
extern s8 D_8009CABF;       /* (the HUD hidden, for these) */
extern s16 D_8009CAA4;
extern s32 D_8009CD00;
/* the health bar's sprites (weak: Meltdown's HUD has none like them, and its count doesn't slide) */
extern u8 D_800D7AB8[] __attribute__((weak));
static void count_shown(s32 ot);
u32 port_disc_file(const char *name, u8 **data);
void gpu_load_image(s32 x, s32 y, s32 w, s32 h, const u16 *src);
u32 GTE_cfc2(int r);
void GTE_ctc2(int r, u32 v);
void GsMapModelingData(u32 *p);
void GsLinkObject4(u32 tmd, void *obj, s32 n);
void GsGetLs(void *coord, void *m);
void GsSortObject4(void *obj, void *ot, s32 shift, void *scratch);
s32 func_80065614(void *obj, u8 life, u8 kind);
s32 func_80027248(s32 sound, s32 at, s32 arg2);
s32 func_800746D0(s32 mesh, s32 *from, s32 *to, s16 *normal, s16 *hit, s32 *flags);
/* (the walls test with the mod's own memory for its arguments and the scratchpad put back after: the game's code keeps
 * its pointer arguments there, and host stack addresses, moved by the host's address randomization, showed through in
 * later game code: see endless.c's walls_between) */
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
void free(void *p);
void *malloc(unsigned n);
void port_log(const char *fmt, ...);
int snprintf(char *s, unsigned n, const char *fmt, ...);
char *strstr(const char *s, const char *t);
extern s32 D_800E2F2C;      /* the level's shot mesh */
extern s32 D_800A0370[];    /* sines and cosines (cos << 16 | sin), 4096 a turn */
extern u8 D_800B8F88[];     /* the frames' ordering tables (0x14 bytes each) */
extern u32 D_800B087C[];    /* what the objects are sorted with */
extern s32 g_CamPosX[3];
extern s32 D_8009CD4C;      /* aiming */
void port_sfx(const s16 *pcm, u32 n, u32 rate, s32 vol);
extern const u8 swing_sound[11]; /* the punch's swing (port/mods/quickpunch.c) */
int port_quick_grenade(void);
void func_80026F30(s32 sound, s32 arg1);
void func_8007A17C(s32 state);
void PushMatrix(void);
void PopMatrix(void);

#define THROW 0x18      /* the weapon's state throwing a grenade */
#define THROW_SOUND 0x52
#define SPAWN ((void (*)(u8 *))0x80077E98)
#define GAP 10          /* ticks between quick throws (half a second) */
#define START 4         /* the grenades Bond starts the mission with */
#define BLAST 3250      /* a blast's end radius (2500) */
#define BLAST_SOUND 0x40 /* the blast's sound (func_800697C8's) */
#define NOTICE 8000     /* how far off guards notice a live grenade (5000) */

static s32 wait;
static void own_grenades(s32 pressed, s32 fresh);
static void own_fly(s32 fresh);
void endless_enemy_blast(s32 *pos) __attribute__((weak)); /* (where it goes off) */
#define LIVE_REAL 4
static int dist_ok(const s32 *a, const s32 *b) { /* (the same grenade a tick on: near where it was) */
    s32 dx = a[0] - b[0], dz = a[2] - b[2];
    return dx > -1500 && dx < 1500 && dz > -1500 && dz < 1500;
}
static s32 frame = 0x7FFFFFFF; /* the frame counter when last seen: a smaller one is a new mission */

/* the frag grenades' slot, or -1 */
static s32 grenades(void) {
    s32 i;
    for (i = 0; i < 6; i++)
        if (!strcmp((char *)&g_Weapons[i], "FRAG GRENADE"))
            return i;
    return -1;
}

/* the cheat (infinite_grenades = 1, the front end's PORT SETTINGS): a throw leaves as many as there were */
char *getenv(const char *name);
static int infinite(void) {
    const char *s = getenv("TWINE_INFINITE_GRENADES");
    return s && *s == '1';
}

void quick_grenade(void) {
    s16 ammo, clip, reserve;
    Weapon *w;
    s16 held, range;
    u8 type;
    s32 i, pressed = port_quick_grenade(), fresh = D_8009CDA8 < frame;
    frame = D_8009CDA8;
    if (g_Args->level == 11)
        return; /* none in the casino mission (Russian Roulette: its blackjack table) */
    if (wait > 0)
        wait--;
    if (W(D_800D31C0, 2, s16) == 2500)
        W(D_800D31C0, 2, s16) = BLAST;
    if (g_Args->level != 8 || (i = grenades()) < 0) {
        own_grenades(pressed, fresh);
        return;
    }
    own_fly(fresh); /* (King's Ransom: the mod's grenades only the enemies' here, flown all the same) */
    w = &g_Weapons[i];
    if (fresh && ((w->flags & 0x1800) != 0x1800 || w->ammo < START)) {
        w->flags |= 0x1800;
        w->clip = 1;
        w->reserve = START - 1;
        w->ammo = START;
    }
    if (!pressed)
        return;
    if ((w->flags & 0x1800) != 0x1800 || w->ammo <= 0)
        return;
    if (i == D_800B0DD4) {
        /* in hand: as fire does, where it can */
        if (D_800F11F0 == 1 || D_800F11F0 == 2)
            func_8007A17C(THROW);
        return;
    }
    /* not while the weapons change (put away, loaded, drawn) */
    if (wait > 0 || (D_800F11F0 >= 3 && D_800F11F0 <= 6))
        return;
    held = D_800B0DD4;
    ammo = w->ammo, clip = w->clip, reserve = w->reserve;
    type = D_8009CD98[0x3A];
    range = W(D_8009CD98, 0x38, s16);
    D_800B0DD4 = i;
    D_8009CD98[0x3A] = D_8009D0FC[W(w, 0x5B, u8) * 0x70 + 0x2C];
    func_80026F30(THROW_SOUND, 0);
    PushMatrix();
    SPAWN(D_8009CD98);
    PopMatrix();
    D_8009CD98[0x3A] = type;
    W(D_8009CD98, 0x38, s16) = range;
    D_800B0DD4 = held;
    if (infinite()) { /* the cheat: as many as before */
        w->ammo = ammo, w->clip = clip, w->reserve = reserve;
        wait = GAP;
        return;
    }
    /* the next one to hand, as the throw's reload does, or none left */
    if (w->clip == 0 && w->reserve > 0) {
        w->clip = 1;
        w->reserve--;
    }
    if (w->ammo <= 0)
        w->flags &= ~0x1800;
    wait = GAP;
}

/* the guards who can see a live grenade at pos (x, y, z) told to shout and run */
static void notice(s32 *pos) {
    s32 i = D_800B0DA0;
    u8 *p = D_800AFFC8 + i * 0x124;
    if (!func_80051728)
        return;
    for (; i <= D_800B07B0; i++, p += 0x124)
        if (p[4] != 0 && func_8005397C((s32)pos, W(p, 0x90, s32) + 0x28) < (u32)(W(p, 0x2A, u16) + NOTICE) &&
            W(p, 0x118, s32) != 0 && (func_80051728((s32)p, W(p, 0x5C, s32)) & 0x40))
            **(u32 **)(p + 0x118) |= 0x04000000;
}

/* as the game's, with guards noticing it further off */
static s32 real_at[LIVE_REAL][3], real_tick[LIVE_REAL]; /* (King's Ransom's own frags, as each tick of one sees them) */
s32 func_80076A8C(u8 *grenade) {
    s32 *pos = (s32 *)(W(grenade, 0x90, u8 *) + 0x28), k, b = 0;
    notice(pos);
    for (k = 0; k < LIVE_REAL; k++) { /* (kept a moment, for the AI's grenade watch: port_live_grenades) */
        if (real_tick[k] < real_tick[b])
            b = k;
        if (dist_ok(real_at[k], pos) && D_8009CDA8 - real_tick[k] < 3) {
            b = k;
            break;
        }
    }
    memcpy(real_at[b], pos, 12);
    real_tick[b] = D_8009CDA8;
    return 0;
}

/* Grenades of the mod's own, for the missions without frag grenades: King's Ransom's model (L08/DATA08.BIN in
 * L08/LVL08CD.BIN, the type its frag throws, a TMD of 74 textured triangles) and texture (TIM 32 of
 * L08/LVL08TIM.BIN), read from the disc (port_disc_file) once, the texture put where no mission has anything (x
 * 928, y 320; its colours at 0, 508) and the model's texture page, colours and places in it moved there. They fly
 * and bounce, losing most of their speed, here (through the level's shot mesh, func_800746D0, as projectiles are moved), guards who see one
 * shout and run, and three seconds after the throw it goes off as the frag does (func_80065614 and the blast
 * sound, func_80027248, from an object of this mod's own, the blast Bond's). The model is drawn with the
 * objects (func_8007545C), lit from above. The throw sounds as King's Ransom's does: its sample is only in that
 * level's sound bank (and the gas grenade's level's), so it is decoded from there (SOUNDFX/L8FX.VH and .VB) and
 * played by the runtime (port_sfx, spu.c), after the punch's swing. */
#define FUSE 60         /* ticks from the throw to the blast */
#define SPEED 500       /* the throw's speed, units a tick (King's Ransom's) */
#define LOFT 0x100      /* thrown this much above where Bond looks (4096 a turn) */
#define GRAVITY 30      /* units a tick, a tick */
#define LIVE 4          /* grenades in the air at once */
#define MODEL 0x810C     /* the model in DATA08.BIN */
#define TIM 0xEEC4       /* the texture in LVL08TIM.BIN */
#define TOSS 3          /* ticks from the swing's sound to the toss's */
#define THROW_PROGRAM 7
#define THROW_TONE 0
#define THROW_RATE 15198 /* its note (10) 18 below the tone's centre (28, less 57/128): 44100 Hz * 2^(-18.45/12) */
#define TEX_X 928
#define TEX_Y 320
#define CLUT_X 0
#define CLUT_Y 508

typedef struct { s16 m[3][3]; s32 t[3]; } Mat;
typedef struct Coord { u32 flg; Mat coord, workm; void *param; struct Coord *super, *sub; } Coord;
typedef struct { u32 attribute; Coord *coord2; u32 *tmd; u32 id; } DObj;

static u32 model[0xAD0 / 4];
static u16 clut[16], image[16 * 64];
static int assets;      /* 1 loaded, -1 not to be had */
static DObj dobj;
static Coord coord;
static struct {
    s32 on, ticks, pos[3], vel[3];
    u8 *owner; /* (the guard who threw it: endless's enemy AI; none: Bond's) */
    s32 aim[3]; /* (where a guard's is meant to land) */
    s32 kind;   /* (endless's lent weapons: QP_FRAG, a flash bang, a launcher's round, a rocket, a satchel charge) */
} live[LIVE];
enum { QP_FRAG, QP_FLASH, QP_ROUND, QP_ROCKET, QP_SATCHEL };
void endless_flash(const s32 *pos) __attribute__((weak)); /* (a flash bang gone off: endless.c dazzles who sees it) */
static s32 frags;       /* Bond's, in a mission without frag grenades */
static s16 throw_pcm[4096]; /* the throw's sound */
static u32 throw_len;
static s32 toss;        /* ticks to the toss's sound */
static s32 tick;        /* the frame counter the grenades were last moved at */

/* the throw's sound, King's Ransom's (its sound 0x52: program 7, tone 0 of its bank), decoded from the bank's
 * ADPCM: blocks of 16 bytes, a shift and filter, flags, and 28 four-bit samples */
static u32 decode_tone(const u8 *vh, const u8 *vb, u32 program, u32 tone, s16 *out, u32 max) {
    static const s32 pos[5] = {0, 60, 115, 98, 122}, neg[5] = {0, 0, -52, -55, -60};
    u32 programs = W(vh, 0x12, u16), tones = 0x20 + 0x800, vag, i, off = 0, size, b, k, n = 0;
    const u16 *sizes = (const u16 *)(vh + tones + programs * 16 * 32);
    s32 s1 = 0, s2 = 0, sample, filter, shift;
    if (W(vh, 0, u32) != 0x56414270 || program >= programs) /* "pBAV" */
        return 0;
    vag = W(vh, tones + (program * 16 + tone) * 32 + 0x16, u16);
    if (!vag)
        return 0;
    for (i = 1; i < vag; i++)
        off += sizes[i] * 8;
    size = sizes[vag] * 8;
    for (b = 0; b < size / 16 && n + 28 <= max; b++) {
        const u8 *blk = vb + off + b * 16;
        shift = blk[0] & 15;
        filter = blk[0] >> 4 > 4 ? 0 : blk[0] >> 4;
        for (k = 0; k < 28; k++) {
            sample = (s16)((blk[2 + k / 2] >> (k & 1) * 4 & 15) << 12) >> shift;
            sample += (s1 * pos[filter] + s2 * neg[filter] + 32) >> 6;
            sample = sample > 32767 ? 32767 : sample < -32768 ? -32768 : sample;
            s2 = s1;
            s1 = sample;
            out[n++] = sample;
        }
        if (blk[1] & 1)
            break;
    }
    return n;
}

static void load_sound(const u8 *vh, const u8 *vb) {
    throw_len = decode_tone(vh, vb, THROW_PROGRAM, THROW_TONE, throw_pcm, sizeof(throw_pcm) / 2);
}

/* (endless's enemy AI) a level's voice, a tone of its sound bank's (SOUNDFX/LnFX.VH, .VB in Lnn/LVLnnCD.BIN),
 * decoded once and played by the runtime (port_sfx) at the rate the voices were made at (11025 Hz: half what a
 * bank's samples are taken as, as heard): a guard's "grenade!" lent to every mission */
#define VOICES 64
static struct {
    s8 level, program, tone;
    s16 *pcm;
    u32 len;
} voices[VOICES];

int port_sound_bank(int level, const u8 **vh, const u8 **vb);
int port_trace(const char *what);
int port_tone_play(int level, int program, int tone, int note, s32 l, s32 r, int loop);
void port_voice_play(int level, int program, int tone, s32 vol) {
    int k;
    if (!getenv("TWINE_TEST_TONES_PCM")) { /* (a voice as the SPU's own, as the game keys its voices on: tones.c; the
                                            * old raw PCM with TWINE_TEST_TONES_PCM) */
        s32 v = vol * 127 / 0x7FFF;
        int h = port_tone_play(level, program, tone, -1, v, v, 0);
        static int on = -1;
        if (on < 0)
            on = port_trace("sound");
        if (on)
            port_log("trace: sound: voice level %d program %d tone %d, volume %d: handle %d\n", level, program, tone, v,
                     h);
        return;
    }
    for (k = 0; k < VOICES && voices[k].pcm && !(voices[k].level == level && voices[k].program == program &&
                                              voices[k].tone == tone); k++)
        ;
    if (k == VOICES)
        return;
    if (!voices[k].pcm) {
        const u8 *vh, *vb;
        s16 *buf;
        if (!port_sound_bank(level, &vh, &vb)) /* (the bank read once: port/src/tones.c) */
            return;
        if ((buf = malloc(2 * 32768))) {
            voices[k].len = decode_tone((u8 *)vh, (u8 *)vb, (u32)program, (u32)tone, buf, 32768);
            voices[k].pcm = buf;
            voices[k].level = (s8)level, voices[k].program = (s8)program, voices[k].tone = (s8)tone;
        }
        if (!voices[k].pcm)
            return;
        port_log("voice: level %d program %d tone %d: %u samples\n", level, program, tone, voices[k].len);
    }
    if (voices[k].len) {
        static int on = -1;
        if (on < 0)
            on = port_trace("sound");
        if (on)
            port_log("trace: sound: voice level %d program %d tone %d, volume %d\n", level, program, tone, vol);
        port_sfx(voices[k].pcm, voices[k].len, 11025, vol);
    }
}

/* the frag's model and texture, from King's Ransom's archive (entries of 0x1C bytes: name, first sector) */
static void load(void) {
    u8 *arc, *data = NULL, *tim = NULL, *vh = NULL, *vb = NULL, *e, *q, *at;
    u32 n = port_disc_file("L08/LVL08CD.BIN", &arc), i, k;
    assets = -1;
    if (!n)
        return;
    for (e = arc; e + 0x1C <= arc + n && *e; e += 0x1C) {
        at = arc + W(e, 26, u16) * 0x800;
        if (!strcmp((char *)e, "L08/DATA08.BIN"))
            data = at;
        else if (!strcmp((char *)e, "L08/LVL08TIM.BIN"))
            tim = at;
        else if (!strcmp((char *)e, "SOUNDFX/L8FX.VH"))
            vh = at;
        else if (!strcmp((char *)e, "SOUNDFX/L8FX.VB"))
            vb = at;
    }
    if (vh && vb)
        load_sound(vh, vb);
    if (data && tim && W(data + MODEL, 0, u32) == 0x41 && W(tim + TIM, 0, u32) == 0x10 && W(tim + TIM, 4, u32) == 8 &&
        W(tim + TIM, 0x10, u32) == 0x10010) {
        memcpy(model, data + MODEL, sizeof(model));
        q = tim + TIM + 8;
        memcpy(clut, q + 12, sizeof(clut));
        q += W(q, 0, u32);
        memcpy(image, q + 12, sizeof(image));
        /* each textured primitive's colours, texture page and places in it moved (u by 64, v by -64) */
        q = (u8 *)model + 12 + model[3 + 4];
        for (i = 0; i < model[3 + 5]; i++, q += 4 + q[1] * 4)
            if (q[3] & 4) {
                W(q, 6, u16) = CLUT_Y << 6 | CLUT_X >> 4;
                W(q, 10, u16) = (W(q, 10, u16) & ~0x1F) | (TEX_X / 64) | (TEX_Y / 256) << 4;
                for (k = 0; k < (q[3] & 8 ? 4u : 3u); k++) {
                    at = q + 4 + (k < 2 ? k * 4 : k == 2 ? 8 : 12);
                    at[0] += 64;
                    at[1] -= 64;
                }
            }
        GsMapModelingData(model + 1);
        GsLinkObject4((u32)(model + 3), &dobj, 0);
        dobj.coord2 = &coord;
        assets = 1;
    }
    free(arc);
}

static s32 rsin(s32 a) {
    return (s16)D_800A0370[a & 0xFFF];
}

static s32 rcos(s32 a) {
    return D_800A0370[a & 0xFFF] >> 16;
}

/* one goes off: the blast and its sound where it is */
static void blast_or_flash(s32 *pos, u8 *owner, int harm);
static void blast(s32 *pos, u8 *owner) {
    blast_or_flash(pos, owner, 1);
}

static void blast_or_flash(s32 *pos, u8 *owner, int harm) {
    static u8 obj[0x124], sub[0x80];
    memset(obj, 0, sizeof(obj));
    memset(sub, 0, sizeof(sub));
    W(obj, 0x88, void *) = owner ? owner : g_Player; /* (a guard's: not Bond's kill) */
    if (owner && endless_enemy_blast)
        endless_enemy_blast(pos); /* (a guard's spares his friends: endless.c) */
    W(obj, 0x90, u8 *) = sub;
    W(sub, 0x14, s16) = W(sub, 0x1C, s16) = W(sub, 0x24, s16) = 4096;
    memcpy(sub + 0x28, pos, 12);
    if (harm)
        func_80065614(obj, 1, 5);
    func_80027248(BLAST_SOUND, (s32)(sub + 0x14), 0);
}

/* (headless tests, endless.c's TWINE_ENDLESS_TEST_GUARD_BLAST: a grenade's blast at pos, owner's (0: Bond's)) */
void quick_grenade_test_blast(s32 *pos, u8 *owner) {
    blast(pos, owner);
}

/* the grenades in the air moved a tick: falling, bouncing off what they hit (at half the speed) */
static void fly(void) {
    s32 g, i, from[4], to[4];
    s16 normal[4], hit[4];
    s32 flags;
    if (toss > 0 && --toss == 0 && throw_len)
        port_sfx(throw_pcm, throw_len, THROW_RATE, 0x6000);
    for (g = 0; g < LIVE; g++) {
        if (!live[g].on)
            continue;
        if (++live[g].ticks >= FUSE && live[g].kind != QP_SATCHEL && live[g].kind != QP_ROUND &&
            live[g].kind != QP_ROCKET) {
            live[g].on = 0;
            if (live[g].kind == QP_FLASH) {
                if (endless_flash)
                    endless_flash(live[g].pos);
                blast_or_flash(live[g].pos, live[g].owner, 0);
            } else
                blast(live[g].pos, live[g].owner);
            continue;
        }
        if (live[g].ticks >= 150 && (live[g].kind == QP_ROUND || live[g].kind == QP_ROCKET)) { /* (nothing hit) */
            live[g].on = 0;
            blast(live[g].pos, live[g].owner);
            continue;
        }
        if (live[g].kind != QP_ROCKET)
            live[g].vel[1] += GRAVITY;
        for (i = 0; i < 3; i++) {
            from[i] = live[g].pos[i];
            to[i] = from[i] + live[g].vel[i];
        }
        if (walls_between(D_800E2F2C, from, to, normal, hit, &flags) != 0 &&
            (live[g].kind == QP_ROUND || live[g].kind == QP_ROCKET)) { /* (a launcher's round, a rocket: off as it hits) */
            for (i = 0; i < 3; i++) /* (hit: the s16 point where it hit; 12 bytes of it read past its end before) */
                live[g].pos[i] = hit[i];
            live[g].on = 0;
            blast(live[g].pos, live[g].owner);
            continue;
        } else if (live[g].kind == QP_SATCHEL && !live[g].vel[0] && !live[g].vel[2] && live[g].ticks > 1) {
            continue; /* (a satchel charge come to rest: there until detonated) */
        } else if (walls_between(D_800E2F2C, from, to, normal, hit, &flags) != 0) {
            /* off the surface: the speed across it turned back at a quarter, along it a third kept (it lands
             * near where it comes down, not skidding on) */
            long long dot = 0, len = 0;
            s32 n;
            for (i = 0; i < 3; i++) {
                dot += (long long)live[g].vel[i] * normal[i];
                len += (long long)normal[i] * normal[i];
            }
            for (n = 1; (long long)n * n < len; n++)
                ;
            for (i = 0; i < 3; i++) {
                live[g].pos[i] = hit[i] + normal[i] * 16 / n;
                s32 across = len ? (s32)(dot * normal[i] / len) : 0;
                live[g].vel[i] = (live[g].vel[i] - across) / 3 - across / 4;
            }
            if (live[g].kind == QP_SATCHEL) /* (a satchel charge sticks where it lands) */
                live[g].vel[0] = live[g].vel[1] = live[g].vel[2] = 0;
        } else
            for (i = 0; i < 3; i++)
                live[g].pos[i] = to[i];
        notice(live[g].pos);
    }
}

/* one thrown from in front of the camera, where Bond looks and a little above */
static void throw_own(void) {
    s32 g, yaw = W(&g_Camera, 0x82, s16), pitch = W(&g_Camera, 0x80, s16) + LOFT, i;
    s32 dir[3];
    for (g = 0; g < LIVE && live[g].on; g++)
        ;
    if (g == LIVE)
        return;
    dir[0] = rsin(yaw) * rcos(pitch) >> 12;
    dir[1] = -rsin(pitch);
    dir[2] = rcos(yaw) * rcos(pitch) >> 12;
    for (i = 0; i < 3; i++) {
        live[g].pos[i] = g_CamPosX[i] + (dir[i] * 300 >> 12);
        live[g].vel[i] = dir[i] * SPEED >> 12;
    }
    live[g].ticks = 0;
    live[g].on = 1;
    live[g].owner = 0;
    live[g].kind = QP_FRAG;
    if (!infinite())
        frags--;
    /* the punch's swing, then the frag's toss */
    if ((u32)(g_Args->level - 1) < 11)
        func_80026F30(swing_sound[g_Args->level - 1], 0);
    toss = TOSS;
    /* (again: what the level loads may cover it) */
    gpu_load_image(TEX_X, TEX_Y, 16, 64, image);
    gpu_load_image(CLUT_X, CLUT_Y, 16, 1, clut);
}

/* (endless's lent weapons) what one of another mission's throws or launches, from in front of Bond's view: a frag
 * grenade, a flash bang (no blast: a flash), a grenade launcher's round or a rocket (off as they hit something,
 * the rocket flying straight), a satchel charge (lies where it lands until quick_detonate). The mod's own object,
 * flown and drawn as its grenades are. 0 if none can be (no model, or four out) */
int quick_projectile(int kind) {
    s32 g, yaw = W(&g_Camera, 0x82, s16), pitch = W(&g_Camera, 0x80, s16), i, speed, loft;
    s32 dir[3];
    if (!assets)
        load();
    if (assets < 0)
        return 0;
    for (g = 0; g < LIVE && live[g].on; g++)
        ;
    if (g == LIVE)
        return 0;
    speed = kind == QP_ROCKET ? 900 : kind == QP_ROUND ? 700 : kind == QP_SATCHEL ? 300 : SPEED;
    loft = kind == QP_ROCKET ? 0 : kind == QP_ROUND ? 0x60 : LOFT;
    pitch += loft;
    dir[0] = rsin(yaw) * rcos(pitch) >> 12;
    dir[1] = -rsin(pitch);
    dir[2] = rcos(yaw) * rcos(pitch) >> 12;
    for (i = 0; i < 3; i++) {
        live[g].pos[i] = g_CamPosX[i] + (dir[i] * 300 >> 12);
        live[g].vel[i] = dir[i] * speed >> 12;
    }
    live[g].ticks = 0;
    live[g].on = 1;
    live[g].owner = 0;
    live[g].kind = kind;
    if (kind <= QP_FLASH || kind == QP_SATCHEL)
        toss = TOSS;
    gpu_load_image(TEX_X, TEX_Y, 16, 64, image);
    gpu_load_image(CLUT_X, CLUT_Y, 16, 1, clut);
    return 1;
}

/* (endless's lent weapons) the detonator: every satchel charge out goes off; how many did */
int quick_detonate(void) {
    s32 g, n = 0;
    for (g = 0; g < LIVE; g++)
        if (live[g].on && live[g].kind == QP_SATCHEL) {
            live[g].on = 0;
            blast(live[g].pos, live[g].owner);
            n++;
        }
    return n;
}

/* (endless's lent weapons) the frag grenades the quick throw has: lent ones count with them */
s32 *quick_grenade_count(void) {
    return &frags;
}

/* one more grenade for Bond (endless mode: one a guard dropped, in a mission without them) */
void quick_grenade_give(void) {
    frags++;
}

/* (endless's enemy AI) a grenade a guard throws, from `from` to land about `to`: the mod's own grenade, lobbed (it
 * flies, bounces and goes off as Bond's); 0 if none can be (no model, or four in the air) */
int quick_grenade_enemy(const s32 *from, const s32 *to, u8 *owner) {
    s32 g, t, i, d;
    if (!assets)
        load();
    if (assets < 0)
        return 0;
    for (g = 0; g < LIVE && live[g].on; g++)
        ;
    if (g == LIVE)
        return 0;
    d = 0;
    {
        s32 dx = to[0] - from[0], dz = to[2] - from[2];
        long long l = (long long)dx * dx + (long long)dz * dz;
        while ((long long)d * d < l)
            d += 16;
    }
    t = d / 260; /* (ticks in the air: about 260 a tick across) */
    t = t < 14 ? 14 : t > 40 ? 40 : t;
    for (i = 0; i < 3; i += 2) {
        live[g].pos[i] = from[i];
        live[g].vel[i] = (to[i] - from[i]) / t;
    }
    live[g].pos[1] = from[1];
    live[g].vel[1] = (to[1] - from[1] - GRAVITY * t * t / 2) / t; /* (y down: up first, gravity bringing it down) */
    live[g].ticks = 0;
    live[g].on = 1;
    live[g].owner = owner;
    live[g].kind = QP_FRAG;
    memcpy(live[g].aim, to, 12);
    toss = TOSS;
    gpu_load_image(TEX_X, TEX_Y, 16, 64, image);
    gpu_load_image(CLUT_X, CLUT_Y, 16, 1, clut);
    return 1;
}

/* (endless's enemy AI) the grenades live now, the mod's own and King's Ransom's: their places into at[][3] */
int port_live_grenades(s32 (*at)[3], int max) {
    int g, n = 0;
    for (g = 0; g < LIVE && n < max; g++)
        if (live[g].on && live[g].owner) { /* (a guard's: where it will land, from the first: his friends keep off;
                                            * and where it is, once it is down near them) */
            memcpy(at[n++], live[g].aim, 12);
            if (live[g].ticks > 10 && n < max)
                memcpy(at[n++], live[g].pos, 12);
        } else if (live[g].on && live[g].ticks > 14) /* (Bond's: landed, about, not while still in the air) */
            memcpy(at[n++], live[g].pos, 12);
    for (g = 0; g < LIVE_REAL && n < max; g++)
        if (real_tick[g] && D_8009CDA8 - real_tick[g] < 3 && D_8009CDA8 >= real_tick[g])
            memcpy(at[n++], real_at[g], 12);
    return n;
}

static void own_fly(s32 fresh) {
    s32 g;
    if (fresh) {
        for (g = 0; g < LIVE; g++)
            live[g].on = 0;
        tick = D_8009CDA8;
    }
    for (; tick < D_8009CDA8; tick++)
        fly();
}

/* the mod's grenades: moved each tick, one thrown for the key */
static void own_grenades(s32 pressed, s32 fresh) {
    s32 g;
    if (fresh) {
        frags = START;
        for (g = 0; g < LIVE; g++)
            live[g].on = 0;
        tick = D_8009CDA8;
        if (!assets)
            load();
    }
    for (; tick < D_8009CDA8; tick++)
        fly();
    if (pressed && assets > 0 && frags > 0 && wait == 0 && D_8009CD4C == 0) {
        throw_own();
        wait = GAP;
    }
}

/* drawn with the objects, lit from above */
void grenades_draw(void) {
    static const s16 light[3] = {0, -4096, 0};
    Mat ls;
    u32 keep[13];
    s32 g, i, j, spin;
    if (assets <= 0)
        return;
    for (g = 0; g < LIVE; g++) {
        if (!live[g].on)
            continue;
        spin = live[g].ticks * 300;
        memset(&coord, 0, sizeof(coord));
        coord.coord.m[0][0] = 4096;
        coord.coord.m[1][1] = coord.coord.m[2][2] = rcos(spin);
        coord.coord.m[1][2] = -rsin(spin);
        coord.coord.m[2][1] = rsin(spin);
        for (i = 0; i < 3; i++)
            coord.coord.t[i] = live[g].pos[i];
        GsGetLs(&coord, &ls);
        for (i = 0; i < 13; i++)
            keep[i] = GTE_cfc2(i);
        gte_SetRotMatrix(&ls);
        gte_SetTransMatrix(&ls);
        /* the light's direction in the model's own axes: its rotation's columns */
        for (j = 0; j < 3; j++) {
            s32 l = 0;
            for (i = 0; i < 3; i++)
                l += light[i] * coord.coord.m[i][j];
            W(&ls, j * 2, s16) = l >> 12;
        }
        GTE_ctc2(8, (u16)W(&ls, 0, s16) | (u32)W(&ls, 2, s16) << 16);
        GTE_ctc2(9, (u16)W(&ls, 4, s16));
        for (i = 10; i < 13; i++)
            GTE_ctc2(i, 0);
        GsSortObject4(&dobj, &D_800B8F88[D_800B8F24 * 0x14], 2, D_800B087C);
        for (i = 0; i < 13; i++)
            GTE_ctc2(i, keep[i]);
    }
}

/* the grenade, a row of pixels at a time, in these colours: Spoon (the lever), Keyline, Green, Light, White */
static const char keys[] = "SKGLW";
static const u8 colour[5][3] = {{150, 160, 150}, {20, 32, 20}, {56, 140, 64}, {120, 200, 112}, {224, 232, 216}};
static const char *const icon[] = {
    "...KSSK.",
    "..KGKKS.",
    ".KGGGGKS",
    "KGLWWGGK",
    "KGGGGGGK",
    "KGLWWGGK",
    ".KGGGGK.",
    "..KKKK..",
};
#define ICON_W 8
#define ICON_H 8
#define HALF 0x400 /* the bytes the primitives take for each of the two frames (the function has 0x844) */
#define SHADOW 16 /* the drop shadow's grey, as the 007's */

/* a TILE (flat rectangle) or POLY_FT4 (textured quad), as the GPU reads them: the tag links them (the last linked
 * is drawn first) */
typedef struct {
    u32 tag;
    u8 r, g, b, code;
    s16 x, y, w, h;
} Tile;

static void link(u32 *ot, void *prim, u32 words) {
    *(u32 *)prim = words << 24 | (ot[1] & 0xFFFFFF);
    ot[1] = (ot[1] & 0xFF000000) | ((u32)prim & 0xFFFFFF);
}

static Tile *tile(u32 *ot, Tile *t, s32 x, s32 y, s32 w, const u8 *rgb) {
    t->r = rgb[0];
    t->g = rgb[1];
    t->b = rgb[2];
    t->code = 0x60;
    t->x = x;
    t->y = y;
    t->w = w;
    t->h = 1;
    link(ot, t, 3);
    return t + 1;
}

/* with the cheat, an infinity sign instead of the count: two rings side by side, 9 x 5, of rectangles */
static Tile *infinity(u32 *ot, Tile *t, s32 x, s32 y, const u8 *rgb) {
    static const s8 r[7][4] = {{1, 0, 3, 1}, {1, 4, 3, 1}, {0, 1, 1, 3}, {4, 1, 1, 3},
                               {5, 0, 3, 1}, {5, 4, 3, 1}, {8, 1, 1, 3}};
    s32 i;
    for (i = 0; i < 7; i++) {
        t = tile(ot, t, x + r[i][0], y + r[i][1], r[i][2], rgb);
        t[-1].h = r[i][3];
    }
    return t;
}

/* the count, one of the ammo counter's digits (its first sprite has the texture and so on), in a colour */
static u8 *count(u32 *ot, u8 *sprite, s32 x, s32 y, s32 n, u8 r, u8 g, u8 b) {
    u8 *digit = D_800AE9C0 + n * 8;
    memcpy(sprite, D_800D7588 + D_800B8F24 * 0x118, 0x28);
    W(sprite, 8, s16) = W(sprite, 0x18, s16) = x;
    W(sprite, 0x10, s16) = W(sprite, 0x20, s16) = x + digit[2];
    W(sprite, 0xA, s16) = W(sprite, 0x12, s16) = y;
    W(sprite, 0x1A, s16) = W(sprite, 0x22, s16) = y + 8;
    sprite[0xC] = sprite[0x1C] = digit[0];
    sprite[0x14] = sprite[0x24] = digit[0] + digit[2];
    sprite[0xD] = sprite[0x15] = digit[1];
    sprite[0x1D] = sprite[0x25] = digit[1] + 8;
    /* (the texture's colour is scaled by these, 128 its own) */
    sprite[4] = r;
    sprite[5] = g;
    sprite[6] = b;
    link(ot, sprite, 9);
    return sprite + 0x28;
}

/* with the rest of the HUD (its ordering table D_800B8EFC[]'s, func_800463C8), when the ammo counter could show */
void weaponkeys_hud(void) __attribute__((weak)); /* (the weapon list's strip: port/mods/weaponkeys.c) */
void weaponkeys_hud_pre(void) __attribute__((weak));
void func_800463C8(void) {
    if (weaponkeys_hud_pre)
        weaponkeys_hud_pre();
    func_800463C8_orig();
    if (D_8009CABF == 0 && D_8009CAA4 == 0 && g_CutScene == 0 && D_8009CD00 == 0) {
        count_shown(D_800B8EFC[D_800B8F24][1]);
        if (weaponkeys_hud)
            weaponkeys_hud();
    }
}

static void count_shown(s32 ot) {
    static const u8 shadow[3] = {SHADOW, SHADOW, SHADOW};
    u8 *mem;
    Tile *t;
    if (g_Args->level == 11)
        return;
    s32 i, x, y, row, col, end, n;
    Weapon *w;
    if (g_Args->level == 8 && (i = grenades()) >= 0) {
        w = &g_Weapons[i];
        if ((w->flags & 0x1800) != 0x1800 || w->ammo <= 0 || D_800B0DDC == i)
            return;
        n = w->ammo;
    } else if (assets > 0 && frags > 0)
        n = frags;
    else
        return;
    mem = (u8 *)port_ps1_addr((void *)func_80045A28);
    if (!mem)
        return;
    /* past the jump, the frame's half */
    t = (Tile *)(mem + 0x10 + D_800B8F24 * HALF);
    /* under the gold line's left end, before the 007 and as tall as its zeros (on the 512x240 screen the line is at
     * y 41, the first zero x 60-68, y 42-51) */
    x = 30;
    y = 44;
    /* and as the health bar is, sliding in when the mission starts (its sprite's top at y 25 when in place) */
    if (D_800D7AB8)
        y += W(D_800D7AB8 + D_800B8F24 * 0x28, 0xA, s16) - 25;
    n = n > 9 ? 9 : n;
    /* gold, as the 007 is */
    if (infinite()) {
        static const u8 gold[3] = {216, 176, 80};
        t = infinity((u32 *)ot, t, x + ICON_W + 2, y + 2, gold);
    } else
        t = (Tile *)count((u32 *)ot, (u8 *)t, x + ICON_W + 2, y, n, 150, 124, 64);
    /* the grenade, a rectangle for each run of one colour in a row */
    for (row = 0; row < ICON_H; row++)
        for (col = 0; col < ICON_W; col = end) {
            for (end = col + 1; end < ICON_W && icon[row][end] == icon[row][col]; end++)
                ;
            if (icon[row][col] != '.')
                t = tile((u32 *)ot, t, x + col, y + row, end - col, colour[strchr(keys, icon[row][col]) - keys]);
        }
    /* under them, a drop shadow a pixel down and right, as the 007 has */
    if (infinite())
        t = infinity((u32 *)ot, t, x + ICON_W + 3, y + 3, shadow);
    else
        t = (Tile *)count((u32 *)ot, (u8 *)t, x + ICON_W + 3, y + 1, n, SHADOW, SHADOW, SHADOW);
    for (row = 0; row < ICON_H; row++) {
        for (col = 0; icon[row][col] == '.'; col++)
            ;
        for (end = ICON_W; icon[row][end - 1] == '.'; end--)
            ;
        t = tile((u32 *)ot, t, x + col + 1, y + row + 1, end - col, shadow);
    }
}
