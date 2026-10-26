/* Runtime for recompiled MIPS (recomp.h): calls by PS1 address, the PS1 stack recompiled code runs on,
 * and the few instructions that need the host (syscall, invalid words). */
#define _GNU_SOURCE
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "recomp.h"

#include "module.h"

s32 EnterCriticalSection(void);
void ExitCriticalSection(void);

/* Recompiled code keeps its stack in memory its loads and stores reach directly (not PS1 RAM, which its
 * mirrors fill up to 8 MB: main.c); each entry from native code takes a fresh 16 KB slice (calls nest
 * strictly, interrupts included). */
#define RC_STACK_TOP 0x1E100000u
#define RC_STACK_SIZE 0x00100000u
#define RC_SLICE 0x4000u
static u32 rc_sp = RC_STACK_TOP;

__attribute__((constructor(102))) static void map_rc_stack(void) {
    void *p = mmap((void *)(RC_STACK_TOP - RC_STACK_SIZE), RC_STACK_SIZE, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
    if (p != (void *)(RC_STACK_TOP - RC_STACK_SIZE))
        port_fatal("cannot map the recompiled code's stack\n");
}

u32 port_rc_enter(void) {
    u32 sp = rc_sp - 0x40;
    rc_sp -= RC_SLICE;
    if (rc_sp <= RC_STACK_TOP - RC_STACK_SIZE)
        port_fatal("recompiled code nested too deep\n");
    return sp;
}

void port_rc_leave(u32 sp) {
    (void)sp;
    rc_sp += RC_SLICE;
}

/* where the stack is, and back to it (main.c: a save state's load leaves the calls under way) */
u32 port_rc_sp(void) {
    return rc_sp;
}

void port_rc_sp_set(u32 sp) {
    rc_sp = sp;
}

/* Code by PS1 address: the running executable's, else the resident stub's (callbacks it left installed).
 * KSEG1 addresses (0xA0...) are the same code uncached. */
static RcFunc rc_lookup_in(const PortModule *m, u32 addr) {
    int lo = 0, hi = *m->rc_func_count - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (m->rc_funcs[mid].addr == addr)
            return (RcFunc)m->rc_funcs[mid].fn;
        if (m->rc_funcs[mid].addr < addr)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return NULL;
}

static void *native_lookup_in(const PortModule *m, u32 addr) {
    int lo = 0, hi = m->func_count - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (m->funcs[mid].addr == addr)
            return m->funcs[mid].fn;
        if (m->funcs[mid].addr < addr)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return NULL;
}

static RcFunc rc_lookup(u32 addr) {
    RcFunc f;
    if ((addr & 0xE0000000) == 0xA0000000)
        addr ^= 0x20000000;
    f = rc_lookup_in(port_module, addr);
    return f || port_module == port_boot ? f : rc_lookup_in(port_boot, addr);
}

static void *native_lookup(u32 addr) {
    void *f;
    if ((addr & 0xE0000000) == 0xA0000000)
        addr ^= 0x20000000;
    f = native_lookup_in(port_module, addr);
    return f || port_module == port_boot ? f : native_lookup_in(port_boot, addr);
}

typedef u32 (*NativeFn)(u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);

/* Call a native function with 16 stack arguments and restore the stack pointer afterwards ourselves: a
 * function returning a struct pops its hidden result pointer (`ret $4`), which a plain call through a
 * generic function pointer wouldn't expect. */
u32 port_call16(void *fn, const u32 *a) {
    u32 r;
    __asm__ volatile("mov %%esp, %%edi\n\t"
                     "sub $64, %%esp\n\t"
                     "and $-16, %%esp\n\t"
                     "mov $0, %%ecx\n\t"
                     "1:\n\t"
                     "mov (%%esi,%%ecx,4), %%eax\n\t"
                     "mov %%eax, (%%esp,%%ecx,4)\n\t"
                     "inc %%ecx\n\t"
                     "cmp $16, %%ecx\n\t"
                     "jne 1b\n\t"
                     "call *%%ebx\n\t"
                     "mov %%edi, %%esp\n\t"
                     : "=a"(r)
                     : "b"(fn), "S"(a)
                     : "ecx", "edx", "edi", "memory", "cc");
    return r;
}

s32 OpenEvent(u32 cls, s32 spec, s32 mode, void *fn);
s32 CloseEvent(u32 h);
s32 EnableEvent(u32 h);
s32 DisableEvent(u32 h);
s32 TestEvent(u32 h);
void gpu_gp0(const u32 *words, int n);
u32 port_exec(u32 header, s32 argc, u32 argv);
void bios_init_heap(u32 addr, u32 size);
u32 bios_malloc(u32 size);
void bios_free(u32 p);
u32 bios_calloc(u32 n, u32 size);
u32 bios_realloc(u32 p, u32 size);
void port_deliver_event(u32 cls, u32 spec);
s32 card_open(const char *name, u32 mode);
s32 card_lseek(s32 fd, s32 off, s32 whence);
s32 card_rw(s32 fd, u8 *buf, s32 len, int write);
s32 card_close(s32 fd);
u32 card_firstfile(const char *pattern, u32 dir);
u32 card_nextfile(u32 dir);
s32 card_erase(const char *name);
s32 card_info(u32 port);
s32 card_sector(u32 port, u32 sector, u8 *buf, int write);

#define A(n) c->r[4 + (n)]
#define STR(n) ((char *)A(n))

/* PS1 BIOS functions (jumps to 0xA0/0xB0/0xC0 with the function number in $t1). TWINE_BIOS_LOG=1 logs the calls
 * (but TestEvent and the string functions) with their first arguments. */
static void bios_call(R3000 *c, u32 table) {
    static int log = -1;
    u32 fn = c->r[9] & 0xFF;
    u32 *v0 = &c->r[2];
    if (log < 0)
        log = getenv("TWINE_BIOS_LOG") != NULL;
    if (log && !(table == 0xB0 && fn == 0x0B) && !(table == 0xA0 && fn < 0x30))
        port_log("BIOS %02X:%02X (%08X %08X %08X)\n", table, fn, A(0), A(1), A(2));
    switch ((table << 8) | fn) {
    case 0xA013: /* setjmp */
        *v0 = 0;
        return;
    case 0xA015:
        *v0 = (u32)strcat(STR(0), STR(1));
        return;
    case 0xA017:
        *v0 = strcmp(STR(0), STR(1));
        return;
    case 0xA018:
        *v0 = strncmp(STR(0), STR(1), A(2));
        return;
    case 0xA019:
        *v0 = (u32)strcpy(STR(0), STR(1));
        return;
    case 0xA01A:
        *v0 = (u32)strncpy(STR(0), STR(1), A(2));
        return;
    case 0xA01B:
        *v0 = strlen(STR(0));
        return;
    case 0xA024:
        *v0 = (u32)strstr(STR(0), STR(1));
        return;
    case 0xA025:
        *v0 = toupper(A(0));
        return;
    case 0xA028: /* bzero */
        memset((void *)A(0), 0, A(1));
        return;
    case 0xA02A:
        *v0 = (u32)memcpy((void *)A(0), (void *)A(1), A(2));
        return;
    case 0xA02B:
        *v0 = (u32)memset((void *)A(0), A(1), A(2));
        return;
    case 0xA02E:
        *v0 = (u32)memchr((void *)A(0), A(1), A(2));
        return;
    case 0xA02F:
        *v0 = rand();
        return;
    case 0xA00D:
        *v0 = strtol(STR(0), (char **)A(1), A(2));
        return;
    case 0xA033:
        *v0 = bios_malloc(A(0));
        return;
    case 0xA034:
        bios_free(A(0));
        return;
    case 0xA037:
        *v0 = bios_calloc(A(0), A(1));
        return;
    case 0xA038:
        *v0 = bios_realloc(A(0), A(1));
        return;
    case 0xA039: /* InitHeap */
        bios_init_heap(A(0), A(1));
        return;
    case 0xA03F: /* printf */
    case 0xB03F: /* puts */
        port_log(STR(0), A(1), A(2), A(3), M32(c->r[29] + 16), M32(c->r[29] + 20), M32(c->r[29] + 24));
        if (fn == 0x3F && table == 0xB0)
            port_log("\n");
        return;
    case 0xA043: /* Exec: start the executable the stub has loaded (main.c) */
        *v0 = port_exec(A(0), A(1), A(2));
        return;
    case 0xA044: /* FlushCache */
    case 0xA070: /* _bu_init: the BIOS memory card driver (memcard.c needs no setup) */
    case 0xA072: /* CdRemove: the BIOS CD driver (libcd here is native) */
    case 0xA09F: /* SetMem: RAM size (mirrors are mapped as for 2 MB, main.c) */
        return;
    case 0xA049: /* GPU_cw */
        gpu_gp0(&A(0), 1);
        return;
    case 0xB007: /* DeliverEvent */
        port_deliver_event(A(0), A(1));
        return;
    case 0xB008:
        *v0 = OpenEvent(A(0), A(1), A(2), (void *)A(3));
        return;
    case 0xB009:
        *v0 = CloseEvent(A(0));
        return;
    case 0xB00A: /* WaitEvent */
        *v0 = 1;
        return;
    case 0xB00B:
        *v0 = TestEvent(A(0));
        return;
    case 0xB00C:
        *v0 = EnableEvent(A(0));
        return;
    case 0xB00D:
        *v0 = DisableEvent(A(0));
        return;
    case 0xB017: /* ReturnFromException */
    case 0xB018: /* ResetEntryInt */
    case 0xB019: /* HookEntryInt */
    case 0xB05B: /* ChangeClearPAD */
    case 0xC002: /* SysEnqIntRP: interrupt handlers are native (irq.c) */
    case 0xC003: /* SysDeqIntRP */
    case 0xC00A: /* ChangeClearRCnt */
        *v0 = 0;
        return;
    case 0xB038:
        port_fatal("PS1 code called exit(%d)\n", A(0));
    case 0xB056: /* GetC0Table, B0:57 GetB0Table: the kernel's tables (main.c kernel_tables), which PsyQ's GTE
                  * and interrupt setup patch into the kernel's exception handler */
        *v0 = 0x80000674u;
        return;
    case 0xB057:
        *v0 = 0x80000874u;
        return;
    /* memory cards (memcard.c) */
    case 0xB032: /* open */
        *v0 = card_open(STR(0), A(1));
        return;
    case 0xB033: /* lseek */
        *v0 = card_lseek(A(0), A(1), A(2));
        return;
    case 0xB034: /* read */
    case 0xB035: /* write */
        *v0 = card_rw(A(0), (u8 *)RA(A(1)), A(2), fn == 0x35);
        return;
    case 0xB036: /* close */
        *v0 = card_close(A(0));
        return;
    case 0xB042: /* firstfile */
        *v0 = card_firstfile(STR(0), A(1));
        return;
    case 0xB043: /* nextfile */
        *v0 = card_nextfile(A(0));
        return;
    case 0xB045: /* erase */
        *v0 = card_erase(STR(0));
        return;
    case 0xA0AB: /* _card_info */
    case 0xA0AC: /* _card_load */
        *v0 = card_info(A(0));
        return;
    case 0xB04E: /* _card_write */
    case 0xB04F: /* _card_read */
        *v0 = card_sector(A(0), A(1), (u8 *)RA(A(2)), fn == 0x4E);
        return;
    case 0xB04A: /* InitCard, StartCard, StopCard, _new_card, _card_status: nothing to set up, always ready */
    case 0xB04B:
    case 0xB04C:
    case 0xB050:
    case 0xB05C:
        *v0 = 1;
        return;
    }
    port_log("BIOS %02X:%02X not implemented (a0=%08X)\n", table, fn, A(0));
    *v0 = 0;
}

u32 port_rc_psp; /* (and its stack pointer) */
u32 port_rc_ra; /* (the PS1 return address of the recompiled code's call into native code now: for traces) */
void port_rc_call(R3000 *c, u32 addr) {
    RcFunc f = rc_lookup(addr);
    NativeFn n;
    if (f) {
        f(c);
        return;
    }
    if ((addr & 0x1FFFFFFF) == 0xA0 || (addr & 0x1FFFFFFF) == 0xB0 || (addr & 0x1FFFFFFF) == 0xC0) {
        bios_call(c, addr & 0xFF);
        return;
    }
    /* a native function: by its PS1 address, or a host code pointer the game stored */
    n = (NativeFn)native_lookup(addr);
    if (!n && (addr < PS1_RAM_BASE || addr >= PS1_RAM_BASE + PS1_RAM_SIZE) && addr >= 0x08048000u &&
        addr < 0x10000000u)
        n = (NativeFn)addr;
    if (!n)
        port_fatal("recompiled code called %08X: no function there\n", addr);
    {
        u32 sp = c->r[29], a[16], k, was = port_rc_ra;
        for (k = 0; k < 4; k++)
            a[k] = c->r[4 + k];
        for (k = 4; k < 16; k++)
            a[k] = M32(sp + 16 + (k - 4) * 4);
        port_rc_ra = c->r[31], port_rc_psp = c->r[29];
        c->r[2] = port_call16((void *)n, a);
        port_rc_ra = was;
    }
}

/* The boot stub's instruction cache flush (native_names.txt): nothing to do on a PC. */
void port_flush_cache(void) {
}

void port_rc_bad(u32 addr, u32 word) {
    port_fatal("recompiled code reached an invalid instruction %08X at %08X\n", word, addr);
}

/* BIOS syscalls: a0 = 1 EnterCriticalSection, 2 ExitCriticalSection */
void port_rc_syscall(R3000 *c, u32 pc) {
    switch (c->r[4]) {
    case 1:
        c->r[2] = EnterCriticalSection();
        break;
    case 2:
        ExitCriticalSection();
        break;
    default:
        port_log("syscall %u at %08X\n", c->r[4], pc);
        break;
    }
}
