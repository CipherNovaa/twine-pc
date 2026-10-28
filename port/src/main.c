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
#include <ctype.h>
#include <setjmp.h>
#include <dirent.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <ucontext.h>
#include <unistd.h>
#include <pthread.h>
#include <execinfo.h>
#include "port.h"
#include "module.h"
#include "spu.h"
#include "intro.h"
#include <SDL3/SDL.h>

const PortModule *port_module, *port_boot;

static FILE *log_file;
static char log_path[1100]; /* where it is, whole (for the player: errors and crashes name it) */

/* setup.c */
void port_setup(int argc, char **argv);
void port_setup_notices(void);
void port_crash_notice(const char *log, const char *code);

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

/* a mod's last word before the game ends (what it knows of how it came to that: endless.c's texts) */
void (*port_fatal_hook)(void);
void port_smooth_forget(void);
void port_spu_exe_started(void);
int port_spu_sounding(char *buf, int size);

/* TWINE_CORE=1: abort (for a core dump) rather than exit */
void port_fatal(const char *fmt, ...) {
    va_list ap;
    char msg[512];
    void (*hook)(void) = port_fatal_hook;
    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);
    fprintf(stderr, "fatal: %s", msg);
    if (log_file)
        fprintf(log_file, "fatal: %s", msg);
    port_fatal_hook = NULL;
    if (hook)
        hook();
    if (log_file)
        fflush(log_file);
    if (!getenv("TWINE_HEADLESS")) { /* (a player started from a desktop sees no terminal) */
        char text[1800];
        void *port_video_window(void);
        snprintf(text, sizeof(text), "%s%s%s", msg, *log_path ? "\nThe log is at " : "", log_path);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "TWINE", text, port_video_window());
    }
    if (getenv("TWINE_CORE") && atoi(getenv("TWINE_CORE")))
        abort();
    exit(1);
}

static void map_fixed(int fd, u32 offset, u32 addr, u32 size, int prot) {
#ifdef _WIN32
    if (addr < 0x00800000u) /* KUSEG: Windows has it taken; the image's first section keeps it (port/tools/win_link.py) */
        return;
#endif
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
u32 port_ram_size = PS1_RAM_CONSOLE;

/* the ram_8mb setting, read before anything (the environment, else twine.ini's line: load_settings comes later) */
static int ram_8mb_wanted(void) {
    const char *e = getenv("TWINE_RAM_8MB"), *xdg = getenv("XDG_CONFIG_HOME"), *home = getenv("HOME");
    char path[1024], line[256];
    FILE *f;
    int on = 0;
    if (e)
        return atoi(e) != 0;
    if (xdg && *xdg)
        snprintf(path, sizeof(path), "%s/twine/twine.ini", xdg);
    else
        snprintf(path, sizeof(path), "%s/.config/twine/twine.ini", home ? home : ".");
    if (!(f = fopen(path, "r")))
        return 0;
    while (fgets(line, sizeof(line), f)) {
        char *k = line;
        while (*k == ' ' || *k == '\t')
            k++;
        if (!strncasecmp(k, "ram_8mb", 7) && strchr(" \t=", k[7]) && strchr(k, '='))
            on = atoi(strchr(k, '=') + 1) != 0;
    }
    fclose(f);
    return on;
}

__attribute__((constructor(101))) static void map_ps1_memory(void) {
    int fd = memfd_create("ps1-ram", 0), i;
    if (ram_8mb_wanted()) { /* (8 MB, once in each segment: no mirrors) */
        port_ram_size = 0x00800000u;
        if (fd < 0 || ftruncate(fd, PS1_RAM_SIZE))
            port_fatal("cannot create PS1 RAM\n");
        map_fixed(fd, 0, PS1_RAM_BASE, PS1_RAM_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
        map_fixed(fd, 0, 0xA0000000u, PS1_RAM_SIZE, PROT_READ | PROT_WRITE);
        map_fixed(fd, 0x10000, 0x00010000u, PS1_RAM_SIZE - 0x10000, PROT_READ | PROT_WRITE);
        map_fixed(-1, 0, PS1_SCRATCH_BASE, PS1_SCRATCH_SIZE, PROT_READ | PROT_WRITE);
        return;
    }
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
 * that code here; the patches land in otherwise unused kernel RAM.
 *
 * The device table (address at 0x150, size at 0x154, 50h bytes per device): libmcrd looks for the memory card
 * device "bu" there and puts its own directory reader into its firstfile entry before calling firstfile, which
 * the memory card driver here (memcard.c) answers itself. */
static void kernel_tables(void) {
    *(u32 *)(0x80000674u + 6 * 4) = 0x80000C80u;
    memset((void *)0x8000F000u, 0, 0x60);
    strcpy((char *)0x8000F050u, "bu");
    *(u32 *)0x8000F000u = 0x8000F050u; /* the device's name */
    *(u32 *)0x80000150u = 0x8000F000u;
    *(u32 *)0x80000154u = 0x50;
}

/* The PS1 maps RAM at 0x00000000 too (KUSEG); its first 64 KB is kernel workspace, and the game does
 * touch it through NULL-based pointers (e.g. `*p->e &= ~0x100` with e still NULL at level load). Linux
 * can't map that page range, so an access there faults: decode the instruction's address registers and
 * move the small one onto the KSEG0 mirror at 0x80000000 (the same RAM on the PS1), then retry. */
#ifdef _WIN32
#define LOW_END 0x00800000u /* (the KUSEG Windows can't map either: port/tools/win_link.py) */
#else
#define LOW_END 0x10000
#endif

static int x86reg_to_greg(int r) {
    static const int map[8] = {REG_EAX, REG_ECX, REG_EDX, REG_EBX, REG_ESP, REG_EBP, REG_ESI, REG_EDI};
    return map[r & 7];
}

static void crash_report(u32 fault, u32 eip) {
    char cmd[256];
    void *at[64];
    int n;
    fprintf(stderr, "crash: access to %08X from %08X\n", fault, eip);
    port_log("crash: access to %08X from %08X (in %s)\n", fault, eip, port_module ? port_module->name : "startup");
    if (log_file) { /* (the stack, as addresses in this executable: addr2line -f -e twine) */
        n = backtrace(at, 64);
        fprintf(log_file, "crash: the stack:\n");
        fflush(log_file);
        backtrace_symbols_fd(at, n, fileno(log_file));
    }
    if (*log_path)
        fprintf(stderr, "TWINE crashed. The log is at %s\n", log_path);
#ifndef _WIN32 /* (Windows: addr2line -f -e twine.exe <address> on the build's twine.exe) */
    snprintf(cmd, sizeof(cmd), "addr2line -f -e /proc/%d/exe %08X >&2", (int)getpid(), eip);
    if (system(cmd)) {
    }
#else
    (void)cmd;
#endif
}

static void on_segv(int sig, siginfo_t *si, void *ctx) {
    ucontext_t *uc = ctx;
    greg_t *g = uc->uc_mcontext.gregs;
    u8 *ip = (u8 *)g[REG_EIP];
    u32 fault = (u32)si->si_addr;
    static u32 logged[64];
    int n, base = -1, index = -1, scale = 0;
    u8 op;
    if (fault >= LOW_END) {
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
        if ((u32)g[REG_ESI] < LOW_END)
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
    if (base >= 0 && (u32)g[x86reg_to_greg(base)] < LOW_END)
        g[x86reg_to_greg(base)] += 0x80000000;
    else if (index >= 0 && ((u32)g[x86reg_to_greg(index)] << scale) < LOW_END)
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

/* SIGBUS, SIGFPE, SIGILL: said in the log, where and with the stack (when glibc finds libgcc_s to walk it), then
 * the default action (the handler is reset) */
static void on_fatal_signal(int sig, siginfo_t *si, void *ctx) {
    void *at[64];
    int n = backtrace(at, 64);
    if (log_file) {
        fprintf(log_file, "crash: signal %d at %08X, address %08X (in %s); the stack:\n", sig,
                (u32)((ucontext_t *)ctx)->uc_mcontext.gregs[REG_EIP], (u32)si->si_addr,
                port_module ? port_module->name : "startup");
        fflush(log_file);
        backtrace_symbols_fd(at, n, fileno(log_file));
    }
    if (*log_path)
        fprintf(stderr, "TWINE crashed (signal %d). The log is at %s\n", sig, log_path);
    raise(sig);
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

/* The PS1 address of a function of the executable running now (0: none). Past its 5-byte jump (below) the bytes
 * of its MIPS code are neither run nor read: what a mod may use for PS1 memory of its own (GPU primitives). */
u32 port_ps1_addr(void *fn) {
    int i;
    for (i = 0; port_module && i < port_module->func_count; i++)
        if (port_module->funcs[i].fn == fn)
            return port_module->funcs[i].addr;
    return 0;
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

#define ARGS_ADDR 0x8001F164u

extern void *port_state_jump;
const PortModule *port_state_target(s32 *argc, u32 *argv);
void port_state_return(void);
void port_state_started(s32 argc, u32 argv);
u32 port_rc_sp(void);
void port_irq_forget(const PortModule *m);
void port_io_forget(void);
void port_cd_forget(void);
void port_rc_sp_set(u32 sp);
void port_endless_level(const PortModule *m);
int port_endless_start(void);
int port_endless_after(void);
void port_endless_returned(const PortModule *m, u8 *args);
extern int port_endless_stats;

/* Run an executable: make its functions current, clear its BSS as its entry point would and run its main.
 * A save state of another mission (savestate.c) comes back here to run that one instead; one loaded in the
 * front end comes back here to return to the boot stub (0). */
static s32 run_main(const PortModule *m, s32 argc, u32 argv) {
    sigjmp_buf here;
    void *outer = port_state_jump;
    u32 sp = port_rc_sp();
    s32 r;
    switch (sigsetjmp(here, 1)) {
    case 1:
        port_rt_depth = 0;
        port_rc_sp_set(sp);
        m = port_state_target(&argc, &argv);
        break;
    case 2:
        port_rt_depth = 0;
        port_rc_sp_set(sp);
        port_state_jump = outer;
        port_state_return();
        return 0;
    case 3: /* port_exe_leave: as if its main had returned, its callbacks forgotten */
        port_rt_depth = 0;
        port_rc_sp_set(sp);
        port_state_jump = outer;
        port_irq_forget(port_module);
        port_io_forget();
        port_cd_forget();
        return 0;
    }
    port_state_jump = &here;
    port_log("Exec %s\n", m->name);
    port_module = m;
    port_smooth_forget(); /* (interp.c: the last executable's mods' memory) */
    if (port_trace("voices")) { /* (what still sounds of the last executable's, before and after its own stopped) */
        char what[640];
        port_spu_sounding(what, sizeof(what));
        port_log("trace: voices: as %s starts, before: %s\n", m->name, what);
    }
    port_spu_exe_started(); /* (spu.c: the runtime's own sounds of the last one stopped: their handles are gone) */
    install_trampolines(m);
    memset((void *)m->bss_start, 0, m->bss_end - m->bss_start);
    port_state_started(argc, argv);
    port_endless_level(m);
    r = m->main(argc, (void *)argv);
    port_endless_returned(m, (u8 *)ARGS_ADDR);
    port_state_jump = outer;
    return r;
}

/* BIOS Exec(header, argc, argv) (A0:43): the stub has loaded an executable and starts it. Find its native
 * module by entry point and size and run it. Returns to the stub when its main does. */
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
    r = run_main(m, argc, argv);
    port_log("%s returned %d\n", port_module->name, r);
    port_module = prev;
    return 1;
}


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

/* Settings: twine.ini in $XDG_CONFIG_HOME/twine (~/.config/twine), `key = value` lines (# comments) that stand
 * for the TWINE_<KEY> environment variables (fullscreen = 1, display = NEC, aspect = fill, ...); the
 * environment wins. */
static void load_settings(void) {
    const char *xdg = getenv("XDG_CONFIG_HOME"), *home = getenv("HOME");
    char path[1024], line[512];
    FILE *f;
    if (xdg && *xdg)
        snprintf(path, sizeof(path), "%s/twine/twine.ini", xdg);
    else
        snprintf(path, sizeof(path), "%s/.config/twine/twine.ini", home ? home : ".");
    f = fopen(path, "r");
    if (!f)
        return;
    while (fgets(line, sizeof(line), f)) {
        char name[128] = "TWINE_", *eq = strchr(line, '='), *k = line, *v, *e;
        size_t n = 6;
        if (!eq || *line == '#' || *line == ';')
            continue;
        *eq = 0;
        for (v = eq + 1; *v == ' ' || *v == '\t'; v++)
            ;
        for (e = v + strlen(v); e > v && (e[-1] == '\n' || e[-1] == '\r' || e[-1] == ' ' || e[-1] == '\t'); e--)
            ;
        *e = 0;
        for (; *k && n < sizeof(name) - 1; k++)
            if (*k != ' ' && *k != '\t')
                name[n++] = (char)toupper((unsigned char)*k);
        name[n] = 0;
        if (n > 6)
            setenv(name, v, 0);
    }
    fclose(f);
}

/* A setting changed in the game (the front end's OPTIONS): its line in twine.ini rewritten, or added, and the
 * environment variable it stands for set to it. */
void port_setting_save(const char *key, const char *value) {
    const char *xdg = getenv("XDG_CONFIG_HOME"), *home = getenv("HOME");
    char path[1024], tmp[1100], line[512], name[128] = "TWINE_";
    FILE *in, *out;
    int done = 0;
    size_t i, n = 6;
    RT_ENTER();
    for (i = 0; key[i] && n < sizeof(name) - 1; i++)
        name[n++] = (char)toupper((unsigned char)key[i]);
    name[n] = 0;
    setenv(name, value, 1);
    if (xdg && *xdg)
        snprintf(path, sizeof(path), "%s/twine", xdg);
    else
        snprintf(path, sizeof(path), "%s/.config/twine", home ? home : ".");
    mkdir(path, 0755);
    strcat(path, "/twine.ini");
    snprintf(tmp, sizeof(tmp), "%s.new", path);
    in = fopen(path, "r");
    if (!(out = fopen(tmp, "w"))) {
        if (in)
            fclose(in);
        RT_LEAVE();
        return;
    }
    while (in && fgets(line, sizeof(line), in)) {
        const char *k = line;
        while (*k == ' ' || *k == '\t')
            k++;
        if (!done && !strncasecmp(k, key, strlen(key)) && strchr(" \t=", k[strlen(key)])) {
            fprintf(out, "%s = %s\n", key, value);
            done = 1;
        } else
            fputs(line, out);
    }
    if (!done)
        fprintf(out, "%s = %s\n", key, value);
    if (in)
        fclose(in);
    if (fclose(out) == 0)
        rename(tmp, path);
    port_log("settings: %s = %s\n", key, value);
    RT_LEAVE();
}

/* The game's folder (where the executable is, as make dist lays it out) can hold what the player supplies: a
 * disc/ folder with the disc image (the largest .bin in it) and a bios/ folder; they're used when twine.ini and
 * the environment don't name others. */
static void game_folder_files(void) {
    char dir[1024], path[1300], best[1300] = "";
    long best_size = 0;
    const char *home = getenv("TWINE_HOME"), *base; /* (the launcher's: the folder it is in) */
    size_t n;
    DIR *d;
    struct dirent *e;
    if (!home && !(base = SDL_GetBasePath()))
        return;
    snprintf(dir, sizeof(dir), "%s", home ? home : base);
    for (n = strlen(dir); n > 1 && dir[n - 1] == '/'; n--)
        dir[n - 1] = 0;
    snprintf(path, sizeof(path), "%s/bios", dir);
    if (!getenv("TWINE_BIOS") && access(path, F_OK) == 0)
        setenv("TWINE_BIOS", path, 0);
    snprintf(path, sizeof(path), "%s/disc", dir);
    if (getenv("TWINE_DISC") || !(d = opendir(path)))
        return;
    while ((e = readdir(d))) {
        size_t len = strlen(e->d_name);
        FILE *f;
        if (len < 4 || (strcasecmp(e->d_name + len - 4, ".bin") && strcasecmp(e->d_name + len - 4, ".cue")))
            continue;
        snprintf(path, sizeof(path), "%s/disc/%s", dir, e->d_name);
        if ((f = fopen(path, "rb"))) {
            fseek(f, 0, SEEK_END);
            if (!strcasecmp(e->d_name + len - 4, ".cue")) { /* (a sheet: the disc it names, before any .bin) */
                if (best_size < 0x7FFFFFFF) {
                    best_size = 0x7FFFFFFF;
                    strcpy(best, path);
                }
            } else if (ftell(f) > best_size) {
                best_size = ftell(f);
                strcpy(best, path);
            }
            fclose(f);
        }
    }
    closedir(d);
    if (*best)
        setenv("TWINE_DISC", best, 0);
}

int port_dev_cheats;
int port_blackjack; /* the front end's BLACKJACK: the casino mission at its table (fixes/shell.c, fixes/level11.c) */

/* The watchdog: no frame shown for six seconds (the game stuck in a loop, or waiting on something that never
 * comes) logs where the main thread is (its stack, as addresses in this executable: addr2line -f -e twine) */
static pthread_t main_thread;
u32 port_video_frames(void);

static void on_stall(int sig) {
    void *at[64];
    int n = backtrace(at, 64);
    (void)sig;
    if (log_file) {
        fprintf(log_file, "watchdog: the main thread's stack:\n");
        fflush(log_file);
        backtrace_symbols_fd(at, n, fileno(log_file));
    }
}

/* (TWINE_TURBO: 6 s of the game thread's own CPU time, or 60 s: a run on game time in a loaded machine may well
 * wait its turn for seconds, its game not stuck at all) */
static u64 thread_cpu_ns(clockid_t c) {
    struct timespec ts;
    return clock_gettime(c, &ts) ? 0 : (u64)ts.tv_sec * 1000000000ull + (u64)ts.tv_nsec;
}

static void *watchdog(void *arg) {
    u32 last = port_video_frames();
    int still = 0, told = 0;
    clockid_t cpu;
    u64 cpu_from = 0;
    int have_cpu = port_turbo && !pthread_getcpuclockid(main_thread, &cpu);
    (void)arg;
    if (have_cpu)
        cpu_from = thread_cpu_ns(cpu);
    for (;;) {
        u32 v;
        int stuck;
        sleep(1);
        v = port_video_frames();
        if (v != last) {
            if (told && log_file) {
                fprintf(log_file, "watchdog: frames again (frame %u)\n", v);
                fflush(log_file);
            }
            last = v, still = told = 0;
            cpu_from = have_cpu ? thread_cpu_ns(cpu) : 0;
            continue;
        }
        ++still;
        stuck = have_cpu ? thread_cpu_ns(cpu) - cpu_from >= 6000000000ull || still >= 60 : still >= 6;
        if (stuck && !told) {
            told = 1;
            if (log_file) {
                fprintf(log_file, "watchdog: no frame shown for 6 s (frame %u%s)\n", v,
                        have_cpu ? ", the game's own time" : "");
                fflush(log_file);
            }
            pthread_kill(main_thread, SIGUSR2);
        }
    }
    return NULL;
}

static void watchdog_start(void) {
    pthread_t t;
    void *at[4];
    struct sigaction sa;
    backtrace(at, 4); /* (loads what it needs now, not in the signal) */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = on_stall;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR2, &sa, NULL);
    main_thread = pthread_self();
    if (!getenv("TWINE_NO_WATCHDOG"))
        pthread_create(&t, NULL, watchdog, NULL);
}

int main(int argc, char **argv) {
    const char *level_env;
    s32 level, r;
    if (argc == 4 && !strcmp(argv[1], "--crash-notice")) { /* (the launcher's, after a crash: twine.sh) */
        port_crash_notice(argv[2], argv[3]);
        return 0;
    }
    load_settings();
    game_folder_files();
    port_dev_cheats = getenv("TWINE_DEVCHEATS") && atoi(getenv("TWINE_DEVCHEATS"));
    port_blackjack = getenv("TWINE_BLACKJACK") && atoi(getenv("TWINE_BLACKJACK")); /* (with level = 11, for tests) */
    level_env = getenv("TWINE_LEVEL");
    level = level_env ? atoi(level_env) : 0;
    /* the log: TWINE_LOG (the launcher's: in the user's state folder), else port.log here */
    {
        const char *want = getenv("TWINE_LOG") && *getenv("TWINE_LOG") ? getenv("TWINE_LOG") : "port.log";
        char *cwd;
        log_file = fopen(want, "w");
        if (!log_file)
            fprintf(stderr, "cannot write the log %s; logging here instead\n", want);
        else if (want[0] == '/' || !(cwd = SDL_GetCurrentDirectory()))
            snprintf(log_path, sizeof(log_path), "%s", want);
        else {
            snprintf(log_path, sizeof(log_path), "%s%s", cwd, want);
            SDL_free(cwd);
        }
    }
    if ((r = port_endless_start())) /* (TWINE_ENDLESS: straight into a run) */
        level = r;
    if (getenv("TWINE_PROFILE"))
        prof_start();
    {
        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_sigaction = on_segv;
        sa.sa_flags = SA_SIGINFO | SA_NODEFER;
        sigaction(SIGSEGV, &sa, NULL);
        sa.sa_sigaction = on_fatal_signal;
        sa.sa_flags = SA_SIGINFO | SA_RESETHAND;
        sigaction(SIGBUS, &sa, NULL);
        sigaction(SIGFPE, &sa, NULL);
        sigaction(SIGILL, &sa, NULL);
    }
    port_video_init();
    port_setup(argc, argv); /* the disc (and BIOS): checked, and chosen on the setup screen when it isn't right */
    port_audio_init();
    port_setup_notices(); /* what didn't start as asked (renderer, sound, folders), before the game */
    watchdog_start();
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
    boot_init_args(level);
    port_irq_start();
    r = run_main(port_module, 1, ARGS_ADDR);
    port_log("game main returned %d\n", r);
    /* endless mode, with no boot stub running: the next room's level in this one's place, as the stub would
     * (g_Args kept: the level named it) */
    while ((level = port_endless_after())) {
        char name[32];
        snprintf(name, sizeof(name), "level%02d", level);
        port_module = find_module(name);
        load_exe(port_module->path);
        r = run_main(port_module, 1, ARGS_ADDR);
        port_log("game main returned %d\n", r);
    }
    if (port_endless_stats) { /* a run straight from the start is over: the front end, for its stats */
        port_module = find_module("shell");
        load_exe(port_module->path);
        *(u32 *)ARGS_ADDR |= 1;
        r = run_main(port_module, 1, ARGS_ADDR);
        port_log("shell returned %d\n", r);
    }
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
