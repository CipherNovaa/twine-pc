/* Native port entry: lays out PS1 memory at its own addresses, loads the level executable's image (its
 * initialised data is used as is), points every game function's PS1 address at the native function, does
 * what the boot stub (SLUS_012.72) does before it starts a level, and calls the game's main. */
#define _GNU_SOURCE
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <ucontext.h>
#include <unistd.h>
#include "port.h"
#include "spu.h"

/* Generated per level (port/gen_native.py): PS1 address -> native function, for code that calls through a
 * function pointer read from PS1 data (callback tables, object update functions). */
typedef struct {
    u32 addr;
    void *fn;
} PortFunc;
extern const PortFunc port_funcs[];
extern const int port_func_count;
extern const int port_level;                  /* which level this build is */
extern s32 (*const port_game_main)(s32, void *); /* the level's main (native or recompiled) */
extern const u32 port_bss_start, port_bss_end;


static FILE *log_file;

void port_log(const char *fmt, ...) {
    va_list ap;
    RT_ENTER();
    va_start(ap, fmt);
    vfprintf(log_file ? log_file : stderr, fmt, ap);
    va_end(ap);
    if (log_file)
        fflush(log_file);
    RT_LEAVE();
}

void port_fatal(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fputs("fatal: ", stderr);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(1);
}

static void map_fixed(int fd, u32 offset, u32 addr, u32 size, int prot) {
    void *p = mmap((void *)addr, size, prot, (fd < 0 ? MAP_PRIVATE | MAP_ANONYMOUS : MAP_SHARED) | MAP_FIXED_NOREPLACE,
                   fd, offset);
    if (p != (void *)addr)
        port_fatal("cannot map PS1 memory at %08X (%s); this build must be 32-bit and non-PIE\n", addr,
                   p == MAP_FAILED ? "mmap failed" : "address taken");
}

/* Map PS1 memory before anything else allocates. Main RAM is executable: its code area holds the jumps
 * that turn PS1 function addresses into native calls. Like the PS1, the same RAM also appears at
 * 0xA0000000 (KSEG1) and from 0x00010000 (KUSEG; the first 64 KB can't be mapped on Linux), for code that
 * masks or rebases pointers. */
__attribute__((constructor(101))) static void map_ps1_memory(void) {
    int fd = memfd_create("ps1-ram", 0);
    if (fd < 0 || ftruncate(fd, PS1_RAM_SIZE))
        port_fatal("cannot create PS1 RAM\n");
    map_fixed(fd, 0, PS1_RAM_BASE, PS1_RAM_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
    map_fixed(fd, 0, 0xA0000000u, PS1_RAM_SIZE, PROT_READ | PROT_WRITE);
    map_fixed(fd, 0x10000, 0x00010000u, PS1_RAM_SIZE - 0x10000, PROT_READ | PROT_WRITE);
    map_fixed(-1, 0, PS1_SCRATCH_BASE, PS1_SCRATCH_SIZE, PROT_READ | PROT_WRITE);
}

/* The PS1 maps RAM at 0x00000000 too (KUSEG); its first 64 KB is kernel workspace, and the game does
 * touch it through NULL-based pointers (e.g. `*p->e &= ~0x100` with e still NULL at level load). Linux
 * can't map that page range, so an access there faults: decode the instruction's address registers and
 * move the small one onto the KSEG0 mirror at 0x80000000 (the same RAM on the PS1), then retry. */
static int x86reg_to_greg(int r) {
    static const int map[8] = {REG_EAX, REG_ECX, REG_EDX, REG_EBX, REG_ESP, REG_EBP, REG_ESI, REG_EDI};
    return map[r & 7];
}

static void crash_report(u32 fault, u32 eip) {
    char cmd[256];
    fprintf(stderr, "crash: access to %08X from %08X\n", fault, eip);
    port_log("crash: access to %08X from %08X\n", fault, eip);
    snprintf(cmd, sizeof(cmd), "addr2line -f -e /proc/%d/exe %08X >&2", (int)getpid(), eip);
    if (system(cmd)) {
    }
}

static void on_segv(int sig, siginfo_t *si, void *ctx) {
    ucontext_t *uc = ctx;
    greg_t *g = uc->uc_mcontext.gregs;
    u8 *ip = (u8 *)g[REG_EIP];
    u32 fault = (u32)si->si_addr;
    static u32 logged[64];
    int n, base = -1, index = -1, scale = 0;
    u8 op;
    if (fault >= 0x10000) {
        crash_report(fault, (u32)g[REG_EIP]);
        signal(SIGSEGV, SIG_DFL);
        return; /* a real crash: let it happen again with the default action */
    }
    while (*ip == 0x66 || *ip == 0x67 || *ip == 0xF2 || *ip == 0xF3 || *ip == 0x2E || *ip == 0x3E || *ip == 0x26 ||
           *ip == 0x64 || *ip == 0x65 || *ip == 0x36)
        ip++;
    op = *ip++;
    if (op == 0xA4 || op == 0xA5 || op == 0xA6 || op == 0xA7 || op == 0xAA || op == 0xAB || op == 0xAC ||
        op == 0xAD) {
        /* string instructions: ESI source, EDI destination */
        if ((u32)g[REG_ESI] < 0x10000)
            base = 6;
        else
            base = 7;
    } else {
        u8 modrm, mod, rm;
        if (op == 0x0F)
            ip++;
        modrm = *ip++;
        mod = modrm >> 6;
        rm = modrm & 7;
        if (mod != 3) {
            if (rm == 4) {
                u8 sib = *ip++;
                scale = sib >> 6;
                index = ((sib >> 3) & 7) == 4 ? -1 : (sib >> 3) & 7;
                base = ((sib & 7) == 5 && mod == 0) ? -1 : sib & 7;
            } else if (!(rm == 5 && mod == 0))
                base = rm;
        }
    }
    if (base >= 0 && (u32)g[x86reg_to_greg(base)] < 0x10000)
        g[x86reg_to_greg(base)] += 0x80000000;
    else if (index >= 0 && ((u32)g[x86reg_to_greg(index)] << scale) < 0x10000)
        g[x86reg_to_greg(index)] += 0x80000000u >> scale;
    else {
        crash_report(fault, (u32)g[REG_EIP]);
        signal(SIGSEGV, SIG_DFL);
        return;
    }
    for (n = 0; n < 64 && logged[n] && logged[n] != (u32)g[REG_EIP]; n++)
        ;
    if (n < 64 && !logged[n]) {
        logged[n] = g[REG_EIP];
        port_log("low-memory access %08X from %08X (redirected to the KSEG0 mirror)\n", fault, (u32)g[REG_EIP]);
    }
}

static int unimpl_seen[4096];

/* Called by the generated stubs of library functions nothing native implements yet: log each once. */
void port_unimplemented(const char *name, u32 addr) {
    u32 h = (addr >> 2) & 4095;
    if (unimpl_seen[h] != (s32)addr) {
        unimpl_seen[h] = addr;
        port_log("unimplemented: %s (%08X)\n", name, addr);
    }
}

/* Copy a PS-X EXE's text+data image to its load address. */
static void load_exe(const char *path) {
    FILE *f = fopen(path, "rb");
    u8 head[0x800];
    u32 addr, size;
    if (!f)
        port_fatal("cannot open %s\n", path);
    if (fread(head, 1, sizeof(head), f) != sizeof(head) || memcmp(head, "PS-X EXE", 8))
        port_fatal("%s is not a PS-X EXE\n", path);
    addr = *(u32 *)(head + 0x18);
    size = *(u32 *)(head + 0x1C);
    if (addr < PS1_RAM_BASE || addr + size > PS1_RAM_BASE + PS1_RAM_SIZE)
        port_fatal("%s loads outside RAM (%08X+%X)\n", path, addr, size);
    if (fread((void *)addr, 1, size, f) != size)
        port_fatal("%s is truncated\n", path);
    fclose(f);
    port_log("loaded %s at %08X (%X bytes)\n", path, addr, size);
}

/* A 5-byte `jmp native` at each game function's PS1 address. */
static void install_trampolines(void) {
    int i;
    for (i = 0; i < port_func_count; i++) {
        u8 *p = (u8 *)port_funcs[i].addr;
        p[0] = 0xE9;
        *(s32 *)(p + 1) = (s32)((u8 *)port_funcs[i].fn - (p + 5));
    }
}

#define ARGS_ADDR 0x8001F164u

/* The boot stub's g_Args setup (SLUS_012.72 func_80010000 and func_80010544), then what it does before it
 * runs a level (func_80010240): the level number and the executable's path. */
static void boot_init_args(s32 level) {
    u8 *a = (u8 *)ARGS_ADDR;
    s32 i;
    *(u32 *)0x8001F158 = ARGS_ADDR;
    memset(a, 0, 0xDB * 4);
    *(s16 *)(a + 0x140) = 0x50;
    *(s16 *)(a + 0x13E) = 0x50;
    a[0x138] = 0x50;
    *(s16 *)(a + 0x13A) = 0x65;
    *(s16 *)(a + 0x144) = 0x65;
    /* button map {use weapon, prev, next, reload, crouch, sniper, strafe L, strafe R} */
    *(s16 *)(a + 0x148) = 0x40;
    *(s16 *)(a + 0x14A) = 0x10;
    *(s16 *)(a + 0x14C) = 0x80;
    *(s16 *)(a + 0x14E) = 0x20;
    *(s16 *)(a + 0x150) = 4;
    *(s16 *)(a + 0x152) = 8;
    *(s16 *)(a + 0x154) = 1;
    *(s16 *)(a + 0x156) = 2;
    a[0x1BC] = 1; /* CD archive mode */
    a[0x1BD] = 1;
    a[0x1BE] = 1;
    a[0x12A] = 1;
    a[0x117] = 1; /* reverse sniper */
    *(s16 *)(a + 0x142) = 0x7F;
    a[0x136] = 0x7F;
    a[0x137] = 0x7F;
    for (i = 0; i < 4; i++) {
        a[0x164 + i] = 0xFF;
        a[0x168 + i] = 0x7F;
    }
    *(u32 *)(a + 4) |= 0xB7;
    a[0x146] = 1;
    a[0x147] = 1;
    for (i = 0; i < 11; i++) {
        a[0x17C + i] = 0;
        a[0x170 + i] = 0;
    }
    *(u8 *)0x8001F2D4 = 1;
    *(s16 *)(a + 0x13C) = -1;
    for (i = 0; i < 11; i++) {
        *(s32 *)(a + 0x2E4 + i * 4) = -1;
        *(s32 *)(a + 0x314 + i * 4) = -1;
        *(s32 *)(a + 0x340 + i * 4) = -1;
    }
    *(s32 *)0x8001F474 = -1;

    *(u32 *)a |= 0x40; /* skip the front end (the boot-to-level patch) */
    *(s16 *)(a + 0x188) = level;
    *(u32 *)(a + 0x204) = 0x801FFF00; /* the stub's stack pointer when it starts a child */
    sprintf((char *)(a + 0x234), "\\L%02d\\LEVEL%02d.EXE;1", level, level);
}

int main(int argc, char **argv) {
    char exe_default[64];
    const char *exe, *boot = argc > 2 ? argv[2] : "workbench/extracted/SLUS_012.72";
    s32 level = port_level, r;
    snprintf(exe_default, sizeof(exe_default), "build/level%02d/level%02d.exe", level, level);
    exe = argc > 1 ? argv[1] : exe_default;
    log_file = fopen("port.log", "w");
    load_exe(boot); /* resident stub data (g_Args lives in it) */
    load_exe(exe);
    install_trampolines();
    boot_init_args(level);
    /* start(): clear the level's BSS */
    memset((void *)port_bss_start, 0, port_bss_end - port_bss_start);
    {
        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_sigaction = on_segv;
        sa.sa_flags = SA_SIGINFO | SA_NODEFER;
        sigaction(SIGSEGV, &sa, NULL);
    }
    port_video_init();
    port_audio_init();
    port_irq_start();
    r = port_game_main(1, (void *)ARGS_ADDR);
    port_log("game main returned %d\n", r);
    return 0;
}

/* func_80026800 (asm in the PS1 build): returns the stack pointer. */
u32 func_80026800(void) {
    return (u32)__builtin_frame_address(0);
}

/* PsyQ libc rand/srand (the C standard's sample generator; game randomness depends on its sequence). */
static u32 rand_next = 1;

int rand(void) {
    rand_next = rand_next * 1103515245 + 12345;
    return (rand_next >> 16) & 0x7FFF;
}

void srand(unsigned seed) {
    rand_next = seed;
}
