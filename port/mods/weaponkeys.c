/* Weapon keys (native PC build only): the number keys pick a weapon by its place among those Bond has (1 the
 * first, in the order the weapon buttons step through them), as the weapon buttons would step to it: the one in hand is
 * put away and the chosen one drawn, the same animations and sounds.
 *
 * The weapon buttons (func_800798D4, every frame Bond has the controls) step the slot to change to (D_800B0DDC)
 * past the slots Bond doesn't have (func_80079C04 says so of D_800B0DDC), keep the one in hand in D_800B0DD8 and
 * set the weapon's state machine putting it away (func_80079B94); its state 5 then loads and draws the new one.
 * A key goes through the same, with the slot it names; when the game wouldn't change weapons (mid-change, fire
 * held) or Bond has fewer weapons, nothing happens. Q goes back and forth between the last two picked by number
 * (the gun and the stunner, say), or before any is, back to the weapon held before.
 *
 * MOD_REPLACES_NATIVE: func_800798D4
 *
 * Names are LEVEL01's; port/mods.py maps them onto each level. */
#include "twine.h"

extern s16 D_800B0DD4;      /* the weapon in hand (its slot in g_Weapons) */
extern s16 D_800B0DD8;      /* the one before a change */
extern s16 D_800B0DDC;      /* the one to change to */
extern u32 D_800F11F0;      /* the weapon's state */
extern s32 D_800DDEEC;      /* fire held */
extern u8 D_800AC38C;
int port_weapon_key(void);
void trace_change_by(const char *who);
void func_800798D4_orig(void);
s32 func_800794A0(void);
s32 func_80079C04(void);
void func_8007A17C(s32 state);

extern s32 D_8009EEC8;
extern s32 D_8009EED0;

/* each mission's weapon slots (g_Weapons' count: its reset function's) */
static const u8 slot_count[12] = {6, 6, 12, 9, 6, 11, 12, 13, 11, 12, 10, 6};
#define SLOTS (g_Args->level < 12 ? slot_count[g_Args->level] : 6)

static int picked = -1;  /* the slot last picked by number */
static int other = -1;   /* the one in hand then */
static int held = -1;    /* the one in hand last frame, and before it */
static int held_before = -1;
static int level;

void func_800798D4(void) {
    int key = port_weapon_key(), slot;
    func_800798D4_orig();
    if (g_Args->level != level) {
        level = g_Args->level;
        picked = other = held = held_before = -1;
    }
    if (D_800B0DD4 != held) {
        held_before = held;
        held = D_800B0DD4;
    }
    if (!key || D_800DDEEC || !func_800794A0())
        return;
    D_800B0DD8 = D_800B0DD4;
    if (key < 0) {
        /* Q: the other of the last two picked by number (none picked yet: the one held before) */
        slot = picked < 0 ? held_before : D_800B0DD4 == picked ? other : picked;
        D_800B0DDC = slot;
        if (slot < 0 || slot >= SLOTS || func_80079C04())
            slot = D_800B0DD4;
    } else {
        /* the key's weapon: the slots Bond has, counted in the order the weapon buttons step through them */
        for (slot = 0; slot < SLOTS; slot++) {
            D_800B0DDC = slot;
            if (!func_80079C04() && --key == 0)
                break;
        }
        if (slot == SLOTS)
            slot = D_800B0DD4;
        else if (slot != D_800B0DD4) {
            other = D_800B0DD4;
            picked = slot;
        }
    }
    if (slot == D_800B0DD4) {
        D_800B0DDC = D_800B0DD8;
        return;
    }
    D_800B0DDC = slot;
#ifdef MOD_TRACE
    trace_change_by("the number keys");
#endif
    /* as func_80079B94 does it for a weapon button */
    D_800AC38C = 0;
    if (D_800F11F0 - 3 < 2 || D_800F11F0 == 6) {
        D_8009EEC8 = 15;
        D_8009EED0 = 100;
        func_8007A17C(6);
    } else
        func_8007A17C(3);
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
    D_800AC38C = 0;
    if (D_800F11F0 - 3 < 2 || D_800F11F0 == 6) {
        D_8009EEC8 = 15;
        D_8009EED0 = 100;
        func_8007A17C(6);
    } else
        func_8007A17C(3);
    return 1;
}
