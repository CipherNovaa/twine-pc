/* Quick grenade (native PC build only): G, or L3 on a gamepad, throws a frag grenade while Bond keeps the weapon he
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
 * health bar.
 * The ammo counter (func_80045A28, every frame the HUD shows) draws its digits as sprites; the count is one more,
 * copied from its first, and the grenade flat rectangles. The GPU reads them from PS1 memory: the ammo counter's
 * own MIPS code, which only its 5-byte jump to the native function is left of (port_ps1_addr).
 *
 * quick_grenade is called by the quick punch's func_800788E0 (port/mods/quickpunch.c) every tick, after the
 * weapon's state machine.
 *
 * MOD_REPLACES_NATIVE: func_80076A8C func_80045A28
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
void func_80045A28_orig(s32 ot);
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
#define NOTICE 8000     /* how far off guards notice a live grenade (5000) */

static s32 wait;
static s32 frame = 0x7FFFFFFF; /* the frame counter when last seen: a smaller one is a new mission */

/* the frag grenades' slot, or -1 */
static s32 grenades(void) {
    s32 i;
    for (i = 0; i < 6; i++)
        if (!strcmp((char *)&g_Weapons[i], "FRAG GRENADE"))
            return i;
    return -1;
}

void quick_grenade(void) {
    Weapon *w;
    s16 held, range;
    u8 type;
    s32 i, pressed = port_quick_grenade(), fresh = D_8009CDA8 < frame;
    frame = D_8009CDA8;
    if (wait > 0)
        wait--;
    if (W(D_800D31C0, 2, s16) == 2500)
        W(D_800D31C0, 2, s16) = BLAST;
    if (g_Args->level != 8 || (i = grenades()) < 0)
        return;
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
    /* the next one to hand, as the throw's reload does, or none left */
    if (w->clip == 0 && w->reserve > 0) {
        w->clip = 1;
        w->reserve--;
    }
    if (w->ammo <= 0)
        w->flags &= ~0x1800;
    wait = GAP;
}

/* as the game's, with guards noticing it further off */
s32 func_80076A8C(u8 *grenade) {
    s32 i = D_800B0DA0;
    u8 *p = D_800AFFC8 + i * 0x124;
    s32 pos = W(grenade, 0x90, s32) + 0x28;
    for (; i <= D_800B07B0; i++, p += 0x124)
        if (p[4] != 0 && func_8005397C(pos, W(p, 0x90, s32) + 0x28) < (u32)(W(p, 0x2A, u16) + NOTICE) &&
            W(p, 0x118, s32) != 0 && (func_80051728((s32)p, W(p, 0x5C, s32)) & 0x40))
            **(u32 **)(p + 0x118) |= 0x04000000;
    return 0;
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

void func_80045A28(s32 ot) {
    static const u8 shadow[3] = {SHADOW, SHADOW, SHADOW};
    u8 *mem;
    Tile *t;
    s32 i, x, y, row, col, end, n;
    Weapon *w;
    func_80045A28_orig(ot);
    if (g_Args->level != 8 || (i = grenades()) < 0)
        return;
    w = &g_Weapons[i];
    if ((w->flags & 0x1800) != 0x1800 || w->ammo <= 0 || D_800B0DDC == i)
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
    n = w->ammo > 9 ? 9 : w->ammo;
    /* gold, as the 007 is */
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
    t = (Tile *)count((u32 *)ot, (u8 *)t, x + ICON_W + 3, y + 1, n, SHADOW, SHADOW, SHADOW);
    for (row = 0; row < ICON_H; row++) {
        for (col = 0; icon[row][col] == '.'; col++)
            ;
        for (end = ICON_W; icon[row][end - 1] == '.'; end--)
            ;
        t = tile((u32 *)ot, t, x + col + 1, y + row + 1, end - col, shadow);
    }
}
