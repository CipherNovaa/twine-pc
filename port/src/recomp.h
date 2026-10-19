#ifndef RECOMP_H
#define RECOMP_H

/* Statically recompiled MIPS (port/recomp.py): each PS1 function still in assembly becomes a C function
 * over an R3000 register context, reading and writing PS1 memory at its own addresses. */
#include "port.h"
#include "gte_native.h"

typedef struct {
    u32 r[32];
    u32 hi, lo;
    u32 cop0[16];
} R3000;

typedef void (*RcFunc)(R3000 *c);

#define M8(a) (*(s8 *)(u32)(a))
#define MU8(a) (*(u8 *)(u32)(a))
#define M16(a) (*(s16 *)(u32)(a))
#define MU16(a) (*(u16 *)(u32)(a))
#define M32(a) (*(u32 *)(u32)(a))

/* Loads and stores of recompiled code: hardware registers (0x1F801000-0x1F802FFF, any segment) go to
 * the I/O model (io.c), everything else is memory. The first 64 KB of KUSEG (kernel RAM, which BIOS patches
 * and NULL-based pointers reach) can't be mapped on Linux: those addresses use the KSEG0 mirror. */
u32 port_io_read(u32 addr, int size);
void port_io_write(u32 addr, u32 value, int size);
#define IS_IO(a) ((((a) & 0x1FFFFFFFu) - 0x1F801000u) < 0x2000u)
static inline u32 RA(u32 a) { return a < 0x10000u ? a | 0x80000000u : a; }
static inline u32 LD32(u32 a) { return IS_IO(a) ? port_io_read(a, 4) : M32(RA(a)); }
static inline u32 LD16(u32 a) { return IS_IO(a) ? port_io_read(a, 2) : MU16(RA(a)); }
static inline u32 LD8(u32 a) { return IS_IO(a) ? port_io_read(a, 1) : MU8(RA(a)); }
static inline void ST32(u32 a, u32 v) { if (IS_IO(a)) port_io_write(a, v, 4); else M32(RA(a)) = v; }
static inline void ST16(u32 a, u32 v) { if (IS_IO(a)) port_io_write(a, v, 2); else MU16(RA(a)) = (u16)v; }
static inline void ST8(u32 a, u32 v) { if (IS_IO(a)) port_io_write(a, v, 1); else MU8(RA(a)) = (u8)v; }

/* call a PS1 address (recompiled or native code) from recompiled code */
void port_rc_call(R3000 *c, u32 addr);
u32 port_call16(void *fn, const u32 *args);
void port_rc_bad(u32 addr, u32 word);
void port_rc_syscall(R3000 *c, u32 pc);
u32 port_rc_enter(void);
void port_rc_leave(u32 sp);

static inline u32 rc_lwl(u32 rt, u32 addr) {
    u32 sh = addr & 3, w = M32(RA(addr & ~3u));
    return (rt & (0x00FFFFFFu >> (sh * 8))) | (w << (24 - sh * 8));
}

static inline u32 rc_lwr(u32 rt, u32 addr) {
    u32 sh = addr & 3, w = M32(RA(addr & ~3u));
    return (rt & ~(0xFFFFFFFFu >> (sh * 8))) | (w >> (sh * 8));
}

static inline void rc_swl(u32 rt, u32 addr) {
    u32 sh = addr & 3, *p = (u32 *)RA(addr & ~3u);
    *p = (*p & ~(0xFFFFFFFFu >> (24 - sh * 8))) | (rt >> (24 - sh * 8));
}

static inline void rc_swr(u32 rt, u32 addr) {
    u32 sh = addr & 3, *p = (u32 *)RA(addr & ~3u);
    *p = (*p & ~(0xFFFFFFFFu << (sh * 8))) | (rt << (sh * 8));
}

static inline void rc_div(R3000 *c, s32 n, s32 d) {
    if (d == 0) {
        c->lo = n >= 0 ? 0xFFFFFFFFu : 1;
        c->hi = (u32)n;
    } else if ((u32)n == 0x80000000u && d == -1) {
        c->lo = 0x80000000u;
        c->hi = 0;
    } else {
        c->lo = (u32)(n / d);
        c->hi = (u32)(n % d);
    }
}

static inline void rc_divu(R3000 *c, u32 n, u32 d) {
    if (d == 0) {
        c->lo = 0xFFFFFFFFu;
        c->hi = n;
    } else {
        c->lo = n / d;
        c->hi = n % d;
    }
}

#endif
