#ifndef ENDLESS_H
#define ENDLESS_H

/* Endless mode: what the runtime (port/src/endless.c) and the level's part (port/mods/endless.c) share. */

#define ENDLESS_WEAPONS 32

/* Bond as he left the last room, and the run so far */
typedef struct {
    int valid;          /* (none before the first room is left) */
    int health, armour; /* the player's +0x34; the armour (LEVEL01's D_8009CFF0) */
    int weapons;
    struct {
        char name[24]; /* the weapon slot's name: the same weapon in another mission has the same name */
        int owned;
        short ammo, clip, reserve;
        short unticked; /* left out of the weapon buttons' round (the inventory's checkmark off): kept for the run */
    } weapon[ENDLESS_WEAPONS];
    char in_hand[24]; /* the weapon he held */
    char before[24];  /* the one he held before it (Q, the last weapon: port/mods/weaponkeys.c) */
} EndlessCarry;

typedef struct {
    int rooms;               /* rooms cleared */
    int kills, headshots, shots, hits;
    int time;                /* in the missions' time units (their counter of vblanks) */
    int score;
    int unranked;            /* a save state was loaded, or a cheat or the debug menu used */
} EndlessRun;

#endif
