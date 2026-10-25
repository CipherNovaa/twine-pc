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
    } weapon[ENDLESS_WEAPONS];
    char in_hand[24]; /* the weapon he held */
} EndlessCarry;

typedef struct {
    int rooms;               /* rooms cleared */
    int kills, headshots, shots, hits;
    int time;                /* in the missions' time units (their counter of vblanks) */
    int score;
    int unranked;            /* a save state was loaded, or a cheat or the debug menu used */
} EndlessRun;

#endif
