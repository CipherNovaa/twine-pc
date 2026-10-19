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

/* Matching-only hints for the PS1 build, empty in the native port (-DNATIVE_PORT):
 * `register s32 x REG("a2");` pins a MIPS register; KEEP(x) hides x's value from the optimizer. */
#ifndef NATIVE_PORT
#define REG(r) __asm__(r)
#define KEEP(x) __asm__("" : "+r"(x))
#else
#define REG(r)
#define KEEP(x) ((void)0)
#endif

#endif
