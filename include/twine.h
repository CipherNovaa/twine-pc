#ifndef TWINE_H
#define TWINE_H

#include "common.h"
#include "psyq.h"
#include "gte.h"

/* Game state shared by the boot stub, front-end and levels (lives in SLUS_012.72 at 0x8001F164). */
typedef struct Args {
    /* 0x000 */ u32 flags; /* bit6: boot stub skips the front-end; bit7: death prompt says ABORT? instead of RESTART? */
    /* 0x004 */ u32 flags2; /* bit8: analog controller option (front-end); bit15: all-weapons bonus; bit16: invincibility bonus */
    /* 0x008 */ u8 pad008[0x10C];
    /* 0x114 */ u8 missionComplete; /* boot stub plays the next movie when set */
    /* 0x115 */ u8 unk115;
    /* 0x116 */ u8 pad116[0x16];
    /* 0x12C */ s16 vabId; /* sound bank, closed with SsVabClose */
    /* 0x12E */ u8 pad12E[0x14];
    /* 0x142 */ u8 unk142;
    /* 0x143 */ u8 pad143[0x5];
    /* 0x148 */ u16 buttonMap[8]; /* use weapon, prev weapon, next weapon, reload, crouch, sniper, strafe L, strafe R */
    /* 0x158 */ u8 pad158[0x30];
    /* 0x188 */ s16 level; /* 1-11; boot stub uses 99 = front-end, 98 = movie player */
    /* 0x18A */ s16 unk18A;
    /* 0x18C */ u16 displayW;
    /* 0x18E */ u16 displayH;
    /* 0x190 */ u16 screenW; /* 512 */
    /* 0x192 */ u16 screenH; /* 240 */
    /* 0x194 */ u8 pad194[0x28];
    /* 0x1BC */ s8 cdArchive; /* 1 = load from LVLxxCD.BIN archives (retail); developer cheat codes need 0 */
    /* 0x1BD */ u8 unk1BD;
    /* 0x1BE */ u8 unk1BE;
    /* 0x1BF */ u8 pad1BF[0x49];
    /* 0x208 */ s32 playerX; /* debug screen GWX */
    /* 0x20C */ s32 playerY; /* GWY */
    /* 0x210 */ s32 playerZ; /* GWZ */
    /* 0x214 */ u8 pad214[0x8];
    /* 0x21C */ s32 unk21C;
    /* 0x220 */ s32 unk220;
    /* 0x224 */ u8 pad224[0x4];
    /* 0x228 */ s32 unk228;
    /* 0x22C */ u8 pad22C[0x4];
    /* 0x230 */ u8 unk230;
    /* 0x231 */ u8 pad231[0x3];
    /* 0x234 */ char exePath[0xA8]; /* boot stub sprintf()s the next executable's path here */
    /* 0x2DC */ s32 unk2DC;
    /* 0x2E0 */ u8 pad2E0[0x8C];
} Args; /* size 0x36C */

extern Args *g_Args;

/* Weapon inventory slot. flags |= 0x1800 marks a weapon as owned/usable. */
typedef struct Weapon {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s32 flags;
    /* 0x3C */ u8 pad3C[0x8];
    /* 0x44 */ s16 ammo;
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s16 clip;
    /* 0x4A */ s16 reserve;
    /* 0x4C */ u8 pad4C[0x4];
    /* 0x50 */ u8 clipSize;
    /* 0x51 */ u8 pad51[0xB];
    /* 0x5C */ u16 maxAmmo;
    /* 0x5E */ u8 pad5E[0x5A];
} Weapon; /* size 0xB8 */

extern Weapon g_Weapons[6];

/* Active camera; rotations are 4096 = 360 degrees. */
typedef struct Camera {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 targetX;
    /* 0x08 */ s32 targetY;
    /* 0x0C */ s32 targetZ;
    /* 0x10 */ s16 rotX;
    /* 0x12 */ s16 rotY;
    /* 0x14 */ s16 rotZ;
    /* 0x16 */ u8 pad16[0x32];
    /* 0x48 */ s32 posX;
    /* 0x4C */ s32 posY;
    /* 0x50 */ s32 posZ;
} Camera;

extern Camera g_Camera;

/* Purpose unknown yet: a set of word lists with a shared index (read by func_8004D72C). */
typedef struct List {
    /* 0x0 */ u32 count;
    /* 0x4 */ s32 *items;
} List;
typedef struct ListSet {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ List *lists;
    /* 0x08 */ u8 pad8[0xC];
    /* 0x14 */ u32 index;
} ListSet;

/* Menu state at 0x800D441C (symbol still named g_MenuUpdateFunc): the active menu's per-frame handler,
 * then the pause menu's cursor and row numbers (-1 when a row is hidden). */
typedef struct Menu {
    /* 0x00 */ void (*update)(void);
    /* 0x04 */ s32 cursor;
    /* 0x08 */ s32 abortItem;
    /* 0x0C */ s32 optionsItem;
    /* 0x10 */ s32 restartItem;
    /* 0x14 */ s32 inventoryItem;
    /* 0x18 */ s32 soundItem;
    /* 0x1C */ s32 objectivesItem;
    /* 0x20 */ s32 messagesItem;
    /* 0x24 */ u8 pad24[0x14];
    /* 0x38 */ s32 unk38;
} Menu;
extern Menu g_MenuUpdateFunc;

/* Object running per-object command scripts; opcode handlers are dispatched through the table at D_8009D7A8. */
typedef struct ScriptObj {
    /* 0x000 */ u8 pad0[0x20];
    /* 0x020 */ s32 vars[0x21];
    /* 0x0A4 */ u32 *pc;
    /* 0x0A8 */ u8 padA8[0x11C];
    /* 0x1C4 */ s32 wait;
} ScriptObj;
extern u16 g_Projection; /* GTE projection distance (H); lower = wider field of view */

extern s16 gPsi;          /* player heading (developers' name); 4096 = 360 degrees */
extern void *g_Player;    /* player object; health at +0xE (0x7FFF = full) */
extern u8 g_Invincible;
extern u8 g_GodMode;      /* keeps health topped up; set with invincibility */
extern u8 g_CutScene;
extern s16 g_DebugTextX;
extern s16 g_DebugTextY;
extern s32 g_FrameRate;
extern s32 g_FrameCap;

#endif
