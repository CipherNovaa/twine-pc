#ifndef COMMON_H
#define COMMON_H

#include "include_asm.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;

#include "m2c_macros.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

#ifndef NULL
#define NULL ((void *)0)
#endif

/* Near misses are banked under #ifdef NON_MATCHING (C for the native port; the matching build keeps the
 * asm). `make cmod CMODS=NONMATCHING LEVELS=level01` builds a disc that runs them. */
#ifdef MOD_NONMATCHING
#define NON_MATCHING
#endif

/* Matching-only hints for the PS1 build, empty in the native port (-DNATIVE_PORT):
 * `register s32 x REG("a2");` pins a MIPS register; KEEP(x) hides x's value from the optimizer. */
#ifndef NATIVE_PORT
#define REG(r) __asm__(r)
#define KEEP(x) __asm__("" : "+r"(x))
#else
#define REG(r)
#define KEEP(x) ((void)0)
#endif

/* MIPS `break` (a debugger trap in dev builds), and the symbol of the game's main(): the native port has
 * its own main and calls this one as game_main. */
#ifndef NATIVE_PORT
#define BREAK(code) __asm__ volatile("break " #code)
#define GAME_MAIN "main"
#else
#define BREAK(code) ((void)0)
#define GAME_MAIN "game_main"
#endif

#endif
