/* Native port entry: lays out PS1 memory at its own addresses and runs the game's executables the way the
 * console does. The boot stub (SLUS_012.72) is loaded from the disc and its main loop runs; it loads the
 * movie player, the front end and the levels itself and starts each with BIOS Exec, which switches to that
 * executable's native code (port_exec). With TWINE_LEVEL=n the runtime does what the stub does before it
 * starts level n instead, and starts it directly. Executable images (initialised data included) are used as
 * loaded; every function's PS1 address jumps to its native code. */
#define _GNU_SOURCE
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <ucontext.h>
#include <unistd.h>
#include "port.h"
#include "module.h"
#include "spu.h"
#include "intro.h"

const PortModule *port_module, *port_boot;

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
 * that turn PS1 function addresses into native calls. Like on the console, the 2 MB repeat through the first
 * 8 MB of each region: KSEG0 (0x80000000), KSEG1 (0xA0000000) and KUSEG (from 0x00010000; the first 64 KB
 * can't be mapped on Linux), for code that rebases pointers or sizes memory for 8 MB (the boot stub's heap
 * and stack defaults). */
__attribute__((constructor(101))) static void map_ps1_memory(void) {
    int fd = memfd_create("ps1-ram", 0), i;
    if (fd < 0 || ftruncate(fd, PS1_RAM_SIZE))
        port_fatal("cannot create PS1 RAM\n");
    for (i = 0; i < 4; i++) {
        u32 at = i * PS1_RAM_SIZE;
        map_fixed(fd, 0, PS1_RAM_BASE + at, PS1_RAM_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
        map_fixed(fd, 0, 0xA0000000u + at, PS1_RAM_SIZE, PROT_READ | PROT_WRITE);
        if (i == 0)
            map_fixed(fd, 0x10000, 0x00010000u, PS1_RAM_SIZE - 0x10000, PROT_READ | PROT_WRITE);
        else
            map_fixed(fd, 0, at, PS1_RAM_SIZE, PROT_READ | PROT_WRITE);
    }
    map_fixed(-1, 0, PS1_SCRATCH_BASE, PS1_SCRATCH_SIZE, PROT_READ | PROT_WRITE);
}

/* The kernel's function tables, where GetC0Table/GetB0Table (B0:56/57) point: what PsyQ's set-up code reads
 * from them is the exception handler's address (C0 table entry 6), whose code it then patches. Nothing runs
 * that code here; the patches land in otherwise unused kernel RAM. */
static void kernel_tables(void) {
    *(u32 *)(0x80000674u + 6 * 4) = 0x80000C80u;
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

/* Read a file from the disc (libcd, cdrom.c). Returns its size, or 0 if it isn't there. */
typedef struct {
    u8 minute, second, sector, track;
    u32 size;
    char name[16];
} DiscFile;
void *CdSearchFile(DiscFile *fp, char *name);
s32 CdControl(u8 com, u8 *param, u8 *result);
s32 CdRead(s32 sectors, u32 *buf, s32 mode);

static u32 read_disc_file(const char *name, u8 **data) {
    DiscFile f;
    char path[64];
    snprintf(path, sizeof(path), "\\%s;1", name);
    for (char *c = path; *c; c++)
        if (*c == '/')
            *c = '\\';
    if (!CdSearchFile(&f, path))
        return 0;
    *data = malloc((f.size + 2047) & ~2047u);
    CdControl(2, &f.minute, NULL);
    CdRead((f.size + 2047) / 2048, (u32 *)*data, 0x80);
    return f.size;
}

/* Load an executable's text+data image from the disc to its address. */
static void load_exe(const char *name) {
    u8 *f;
    u32 n = read_disc_file(name, &f), addr, size;
    if (!n || n < 0x800 || memcmp(f, "PS-X EXE", 8))
        port_fatal("%s is not on the disc, or not an executable\n", name);
    addr = *(u32 *)(f + 0x18);
    size = *(u32 *)(f + 0x1C);
    if (addr < PS1_RAM_BASE || addr + size > PS1_RAM_BASE + PS1_RAM_SIZE || size > n - 0x800)
        port_fatal("%s loads outside RAM (%08X+%X)\n", name, addr, size);
    memcpy((void *)addr, f + 0x800, size);
    free(f);
    port_log("loaded %s at %08X (%X bytes)\n", name, addr, size);
}

/* A 5-byte `jmp native` at each of the executable's function addresses. */
static void install_trampolines(const PortModule *m) {
    int i;
    for (i = 0; i < m->func_count; i++) {
        u8 *p = (u8 *)m->funcs[i].addr;
        p[0] = 0xE9;
        *(s32 *)(p + 1) = (s32)((u8 *)m->funcs[i].fn - (p + 5));
    }
}

static const PortModule *find_module(const char *name) {
    int i;
    for (i = 0; i < port_module_count; i++)
        if (!strcmp(port_modules[i]->name, name))
            return port_modules[i];
    port_fatal("this build has no %s\n", name);
}

/* BIOS Exec(header, argc, argv) (A0:43): the stub has loaded an executable and starts it. Find its native
 * module by entry point and size, make its functions current, clear its BSS as its entry point would and
 * run its main. Returns to the stub when main does. */
u32 port_exec(u32 header, s32 argc, u32 argv) {
    const PortModule *prev = port_module, *m = NULL;
    u32 pc0 = *(u32 *)header, t_size = *(u32 *)(header + 0xC);
    int i;
    s32 r;
    for (i = 0; i < port_module_count; i++)
        if (port_modules[i]->pc0 == pc0 && port_modules[i]->text_size == t_size && port_modules[i]->main)
            m = port_modules[i];
    if (!m)
        port_fatal("Exec: no native module for the executable at %08X (%X bytes)\n", pc0, t_size);
    port_log("Exec %s\n", m->name);
    port_module = m;
    install_trampolines(m);
    memset((void *)m->bss_start, 0, m->bss_end - m->bss_start);
    r = m->main(argc, (void *)argv);
    port_log("%s returned %d\n", m->name, r);
    port_module = prev;
    return 1;
}

#define ARGS_ADDR 0x8001F164u

/* The mission each level folder holds (0 = Courier ... 10 = Meltdown), which the front end stores next to
 * the level number: the level opens its mission's speech and music stream, \Lnn\XA<mission + 1>.STR. */
static const s16 level_mission[12] = {-1, 2, 0, 8, 4, 7, 6, 5, 1, 9, 10, 3};

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
    *(s16 *)(a + 0x18A) = level_mission[level];
    *(u32 *)(a + 0x204) = 0x801FFF00; /* the stub's stack pointer when it starts a child */
    sprintf((char *)(a + 0x234), "\\L%02d\\LEVEL%02d.EXE;1", level, level);
}

/* The stub's entry (SLUS_012.72 func_80015900): clear its BSS, give the heap the RAM between the BSS and the
 * stack (whose top and size are in its data), initialise it and run the main loop. Its recompiled code keeps
 * its stack where all recompiled code does (recomp_rt.c), so the children's heaps can't reach it. */
static void boot_start(void) {
    u32 top = *(u32 *)0x8001D6A4 - 8, stack_size = *(u32 *)0x8001D6A0;
    u32 heap = port_boot->bss_end & 0x1FFFFFFF, size = top - stack_size - heap;
    memset((void *)port_boot->bss_start, 0, port_boot->bss_end - port_boot->bss_start);
    *(u32 *)0x8001D6B0 = size;
    *(u32 *)0x8001D6AC = heap | 0x80000000;
    ((void (*)(u32, u32))0x80019B00)((heap | 0x80000000) + 4, size);
    ((void (*)(void))0x80010154)();
}

/* TWINE_PROFILE=1: where the time goes. The program counter is sampled 1000 times a second of CPU time and
 * the counts are written to profile.txt when the program ends or is stopped (SIGTERM, SIGINT);
 * port/tools/profile.py names them. */
static u32 prof_pc[1 << 20];
static u32 prof_n;

static void on_prof(int sig, siginfo_t *si, void *ctx) {
    (void)sig, (void)si;
    if (prof_n < sizeof(prof_pc) / sizeof(prof_pc[0]))
        prof_pc[prof_n++] = ((ucontext_t *)ctx)->uc_mcontext.gregs[REG_EIP];
}

static void prof_write(void) {
    FILE *f = fopen("profile.txt", "w");
    u32 i;
    if (!f)
        return;
    for (i = 0; i < prof_n; i++)
        fprintf(f, "%08x\n", prof_pc[i]);
    fclose(f);
}

static void on_stop(int sig) {
    (void)sig;
    prof_write();
    _exit(0);
}

static void prof_start(void) {
    struct sigaction sa;
    struct itimerval it = {{0, 1000}, {0, 1000}};
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = on_prof;
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sigaction(SIGPROF, &sa, NULL);
    signal(SIGTERM, on_stop);
    signal(SIGINT, on_stop);
    atexit(prof_write);
    setitimer(ITIMER_PROF, &it, NULL);
}

int main(int argc, char **argv) {
    const char *level_env = getenv("TWINE_LEVEL");
    s32 level = level_env ? atoi(level_env) : 0, r;
    log_file = fopen("port.log", "w");
    if (getenv("TWINE_PROFILE"))
        prof_start();
    {
        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_sigaction = on_segv;
        sa.sa_flags = SA_SIGINFO | SA_NODEFER;
        sigaction(SIGSEGV, &sa, NULL);
    }
    port_video_init();
    port_audio_init();
    if (level < 1 || level > 11)
        port_intro(); /* the console's boot animation and the port's title card; they leave the machine as it was */
    kernel_tables();
    port_boot = port_module = find_module("boot");
    load_exe("SLUS_012.72"); /* the stub stays resident below the others (g_Args lives in its data) */
    install_trampolines(port_boot);
    if (level < 1 || level > 11) {
        port_irq_start();
        boot_start();
        port_log("the boot stub's main loop returned\n");
        return 0;
    }
    {
        char name[32];
        snprintf(name, sizeof(name), "level%02d", level);
        port_module = find_module(name);
    }
    load_exe(port_module->path);
    install_trampolines(port_module);
    boot_init_args(level);
    /* start(): clear the level's BSS */
    memset((void *)port_module->bss_start, 0, port_module->bss_end - port_module->bss_start);
    port_irq_start();
    r = port_module->main(1, (void *)ARGS_ADDR);
    port_log("game main returned %d\n", r);
    return 0;
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
