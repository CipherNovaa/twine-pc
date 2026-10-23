/* The casino mission (LEVEL11.EXE, mission 3) on the PC: what the native build changes in its code (port/mods.py
 * builds this into the level's module). Names are the level's own.
 *
 * MOD_REPLACES_NATIVE: func_80065CA4 func_800665AC
 */
#include "common.h"

void func_80065CA4_orig(void);
void func_800665AC_orig(u8 **zone);
void func_80032494(s32 point, s32 arg1);
void port_log(const char *fmt, ...);
extern int port_blackjack;

#define FRAME (*(s32 *)0x8009C44Cu)        /* the mission's frame counter */
#define SECTION (*(s32 *)0x800E3618u)      /* the map section loaded */
#define PLAYER (*(u8 **)0x800B3F70u)       /* Bond's object */
#define ARGS (*(u8 **)0x800B355Cu)         /* g_Args */
#define USE_PRESSED (*(u32 *)0x800B42BCu)  /* the buttons pressed this frame */
#define SEATED (*(s32 *)0x8009DBB8u)       /* at the table */
#define SITTING (*(s32 *)0x8009DB68u)      /* the sit-down (0) or stand-up (1) under way, -1 none */
#define CLOSED (*(s32 *)0x8009DBCCu)       /* the table closed: Bond lost it all */
#define PURSE (*(s32 *)0x8009DB6Cu)        /* the high table's purse */
#define LOAD_POINT ((s16 *)0x8009DB20u)    /* load point 2 (section 3): section, yaw, x, y, z, ... */

/* The front end's BLACKJACK (port_blackjack, fixes/shell.c): the mission goes straight to the high table's room
 * (section 3, the level's own warp to load point 2, func_80032494, with that point moved to the table's seat)
 * and Bond sits down at the table as soon as he is by it: the dealer's use (func_800665AC, called while Bond is
 * near) gets the use button pressed for him, and its sit-down runs as the game's own. Going broke ("YOU LOST IT
 * ALL!") gives the purse back once Bond is up, and he sits down again; a retry after the mission fails starts
 * over the same way. EXIT goes back to the front end. */
static int phase, last_frame;

void func_80065CA4(void) {
    func_80065CA4_orig();
    if (!port_blackjack)
        return;
    if (FRAME < last_frame)
        phase = 0; /* the mission started again */
    last_frame = FRAME;
    switch (phase) {
    case 0:
        LOAD_POINT[2] = -0x514; /* the high table's seat (the dealer's use puts Bond there) */
        LOAD_POINT[4] = -0xC8;
        func_80032494(2, 0);
        phase = 1;
        port_log("blackjack: to the high table\n");
        break;
    case 1:
        if (SECTION == 3)
            phase = 2;
        break;
    default:
        /* EXIT (Triangle) stands Bond up: back to the front end, as the pause menu's ABORT does it (g_Args +0x115
         * and its flags' 0x80: the frame loop returns and the mission ends as its own) */
        if (SITTING == 1 && !CLOSED && ARGS) {
            port_blackjack = 0;
            ARGS[0x115] = 1;
            ARGS[0] |= 0x80;
            port_log("blackjack: left the table\n");
        }
        if (CLOSED && SITTING == -1 && !SEATED) {
            PURSE = 25000;
            CLOSED = 0;
            port_log("blackjack: broke; the purse again\n");
        }
        break;
    }
}

void func_800665AC(u8 **zone) {
    u32 pressed = USE_PRESSED;
    if (port_blackjack && phase == 2 && zone[0] == PLAYER && !SEATED && SITTING == -1 && !CLOSED && ARGS)
        USE_PRESSED |= *(u16 *)(ARGS + 0x14E);
    func_800665AC_orig(zone);
    USE_PRESSED = pressed;
}
