/* Native port entry: lays out PS1 memory at its own addresses, loads the level executable's image (its
 * initialised data is used as is), points every game function's PS1 address at the native function, does
 * what the boot stub (SLUS_012.72) does before it starts a level, and calls the game's main. */
#define _GNU_SOURCE
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "port.h"

/* Generated per level (port/gen_native.py): PS1 address -> native function, for code that calls through a
 * function pointer read from PS1 data (callback tables, object update functions). */
typedef struct {
    u32 addr;
    void *fn;
} PortFunc;
extern const PortFunc port_funcs[];
extern const int port_func_count;

s32 game_main(s32 argc, void *args);

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
    const char *exe = argc > 1 ? argv[1] : "build/level01/level01.exe";
    const char *boot = argc > 2 ? argv[2] : "workbench/extracted/SLUS_012.72";
    s32 level = 1, r;
    log_file = fopen("port.log", "w");
    load_exe(boot); /* resident stub data (g_Args lives in it) */
    load_exe(exe);
    install_trampolines();
    boot_init_args(level);
    /* start(): clear the level's BSS */
    memset((void *)0x800AB540, 0, 0x800FDB2C - 0x800AB540);
    port_video_init();
    port_irq_start();
    r = game_main(1, (void *)ARGS_ADDR);
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
