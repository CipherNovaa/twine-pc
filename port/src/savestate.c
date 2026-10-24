/* Save states: F5 saves the mission as it is, F6 loads what F5 saved (in this session or an earlier one).
 *
 * The game's code is native here, so a state can't be taken anywhere: it is taken where the mission's frame
 * loop reads the controller at its top (the savestate mod's Pad_Update, port/mods/savestate.c), where nothing
 * of the frame is under way and the native frames under it (the level's main and the loop) hold nothing live.
 * There the state is the machine: PS1 RAM and the scratchpad, and the runtime's and the game modules' own
 * memory, everything in the program's data and BSS (VRAM, the GTE, the sound chip and its RAM, the CD drive
 * and its XA stream, libgs and libgpu, the mods' memory, ...) but what belongs to the host or to this run:
 * the objects port/Makefile builds into the nosave sections (the window and input, the audio device, the
 * memory cards, the hardware renderer, the recompiled code's stack, ...) and variables marked PORT_NOSAVE
 * (pointers to the heap, host clocks). The program's own pointers into its data are the same in every run of
 * the same build; a state says which build made it and loads only into that one.
 *
 * Loading a state of the mission that is running puts it all back at the same place. A state of another
 * mission (or loaded from the front end) has that mission started first, the way the console does it, with
 * the state's RAM (so g_Args says which mission it is); its first pass through the same place then puts the
 * rest back. From a mission that is the executable started over in its place (main.c, port_state_target);
 * from the front end it is returning to the boot stub, which then starts the mission g_Args names. */
#define _GNU_SOURCE
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "port.h"
#include "module.h"
#include "gpu.h"
#include "gpu_hw.h"

#define MAGIC "TWINESTA"
#define VERSION 1

typedef struct {
    char magic[8];
    u32 version, build;
    char module[16];
    s32 argc;
    u32 argv;
    u32 ram, native; /* sizes */
} Header;

extern char __data_start[], _end[];
extern char __start_nosave_data[] __attribute__((weak)), __stop_nosave_data[] __attribute__((weak));
extern char __start_nosave_bss[] __attribute__((weak)), __stop_nosave_bss[] __attribute__((weak));
extern char __executable_start[], etext[];

extern const PortModule *port_module, *port_boot;
void port_video_message(const char *text);
void port_irq_now(int *v, int put);
u32 port_video_frames(void);
void spu_hold(int on);

/* the program's memory a state holds: [__data_start, _end) less the nosave sections */
static struct {
    char *at;
    u32 size;
} range[3];
static int ranges;
static u32 native_size, build;

static int request;           /* 1 save, 2 load */
static Header *pending;       /* a load under way: the state, read */
static const PortModule *pending_module;
static u32 loop_frame;        /* the frame loop's call's native frame (port_state_point) */
static s32 run_argc;          /* how the running executable was started */
static u32 run_argv;
void *port_state_jump;        /* main.c's sigjmp_buf of the executable running (port_state_target) */

static void cut(char *lo, char *hi) {
    int i;
    if (!lo || hi <= lo)
        return;
    for (i = 0; i < ranges; i++) {
        char *a = range[i].at, *b = a + range[i].size;
        if (hi <= a || lo >= b)
            continue;
        range[i].size = (u32)(lo > a ? lo - a : 0);
        if (hi < b) {
            range[ranges].at = hi;
            range[ranges].size = (u32)(b - hi);
            ranges++;
        }
        if (!range[i].size)
            range[i] = range[--ranges];
        return;
    }
}

static void init(void) {
    const u8 *p;
    int i;
    if (ranges)
        return;
    range[0].at = __data_start;
    range[0].size = (u32)(_end - __data_start);
    ranges = 1;
    cut(__start_nosave_data, __stop_nosave_data);
    cut(__start_nosave_bss, __stop_nosave_bss);
    for (i = 0; i < ranges; i++)
        native_size += range[i].size;
    build = 2166136261u;
    for (p = (const u8 *)__executable_start; p < (const u8 *)etext; p++)
        build = (build ^ *p) * 16777619u;
    port_log("savestate: %u bytes of the program's memory in %d ranges, build %08X\n", native_size, ranges, build);
}

static void state_path(char *out, size_t len, int dir_only) {
    const char *xdg = getenv("XDG_DATA_HOME"), *home = getenv("HOME");
    if (xdg && *xdg)
        snprintf(out, len, "%s/twine/states%s", xdg, dir_only ? "" : "/quick.state");
    else
        snprintf(out, len, "%s/.local/share/twine/states%s", home ? home : ".", dir_only ? "" : "/quick.state");
}

static void mkdirs(char *path) {
    char *s;
    for (s = path + 1; *s; s++)
        if (*s == '/') {
            *s = 0;
            mkdir(path, 0755);
            *s = '/';
        }
    mkdir(path, 0755);
}

/* everything that changes outside the game thread held still: the vblank signal and the sound chip */
static void hold(int on, sigset_t *old) {
    sigset_t all;
    if (on) {
        sigfillset(&all);
        pthread_sigmask(SIG_BLOCK, &all, old);
        spu_hold(1);
    } else {
        spu_hold(0);
        pthread_sigmask(SIG_SETMASK, old, NULL);
    }
}

static void save(void) {
    char path[1024];
    Header h;
    FILE *f;
    sigset_t old;
    u8 *buf, *p;
    int i, ok;
    init();
    memset(&h, 0, sizeof(h));
    memcpy(h.magic, MAGIC, 8);
    h.version = VERSION;
    h.build = build;
    snprintf(h.module, sizeof(h.module), "%s", port_module->name);
    h.argc = run_argc;
    h.argv = run_argv;
    h.ram = PS1_RAM_SIZE + PS1_SCRATCH_SIZE;
    h.native = native_size;
    if (!(buf = malloc(h.ram + h.native))) {
        port_video_message("Not saved: out of memory");
        return;
    }
    hold(1, &old);
    memcpy(buf, (void *)PS1_RAM_BASE, PS1_RAM_SIZE);
    memcpy(buf + PS1_RAM_SIZE, (void *)PS1_SCRATCH_BASE, PS1_SCRATCH_SIZE);
    for (p = buf + h.ram, i = 0; i < ranges; p += range[i].size, i++)
        memcpy(p, range[i].at, range[i].size);
    hold(0, &old);
    state_path(path, sizeof(path), 1);
    mkdirs(path);
    state_path(path, sizeof(path), 0);
    ok = (f = fopen(path, "wb")) && fwrite(&h, sizeof(h), 1, f) == 1 &&
         fwrite(buf, 1, h.ram + h.native, f) == h.ram + h.native;
    if (f && fclose(f))
        ok = 0;
    free(buf);
    port_log("savestate: %s %s (%s, frame %u)\n", ok ? "saved" : "cannot write", path, h.module,
             port_video_frames());
    port_video_message(ok ? "State saved" : "Not saved: cannot write the file");
}

/* read the state; NULL (and a message) if there is none or it won't do */
static Header *read_state(void) {
    char path[1024];
    Header h, *s;
    FILE *f;
    init();
    state_path(path, sizeof(path), 0);
    if (!(f = fopen(path, "rb")) || fread(&h, sizeof(h), 1, f) != 1 || memcmp(h.magic, MAGIC, 8) ||
        h.version != VERSION) {
        if (f)
            fclose(f);
        port_video_message("No saved state (F5 saves one in a mission)");
        return NULL;
    }
    if (h.build != build || h.native != native_size || h.ram != PS1_RAM_SIZE + PS1_SCRATCH_SIZE) {
        fclose(f);
        port_video_message("The saved state is from another version of the port");
        return NULL;
    }
    if (!(s = malloc(sizeof(h) + h.ram + h.native)) || fread(s + 1, 1, h.ram + h.native, f) != h.ram + h.native) {
        free(s);
        fclose(f);
        port_video_message("Cannot read the saved state");
        return NULL;
    }
    fclose(f);
    *s = h;
    return s;
}

static const PortModule *module_named(const char *name) {
    int i;
    for (i = 0; i < port_module_count; i++)
        if (!strcmp(port_modules[i]->name, name))
            return port_modules[i];
    return NULL;
}

/* PS1 RAM and the scratchpad from the state */
static void put_ram(const Header *s) {
    const u8 *p = (const u8 *)(s + 1);
    memcpy((void *)PS1_RAM_BASE, p, PS1_RAM_SIZE);
    memcpy((void *)PS1_SCRATCH_BASE, p + PS1_RAM_SIZE, PS1_SCRATCH_SIZE);
}

extern int port_endless, port_endless_unranked;

/* all of it */
static void put_all(const Header *s, int loaded) {
    const u8 *p = (const u8 *)(s + 1) + s->ram;
    sigset_t old;
    int now[4], i;
    hold(1, &old);
    port_irq_now(now, 0);
    put_ram(s);
    for (i = 0; i < ranges; p += range[i].size, i++)
        memcpy(range[i].at, p, range[i].size);
    port_irq_now(now, 1);
    hold(0, &old);
    if (gpu_hw_on) { /* the renderer's copy of VRAM, from gpu.c's */
        gpu_hw_reset();
        gpu_hw_load_image(0, 0, 1024, 512, gpu_vram);
    }
    if (loaded) {
        port_endless_unranked |= port_endless; /* (endless.c: a run with a state loaded isn't ranked) */
        port_log("savestate: loaded (%s, frame %u)\n", s->module, port_video_frames());
        port_video_message("State loaded");
    }
}

/* F5 / F6 (video.c) */
void port_state_request(int load) {
    request = load ? 2 : 1;
}

/* main.c: an executable starts (argc, argv as Exec passed them) */
void port_state_started(s32 argc, u32 argv) {
    run_argc = argc;
    run_argv = argv;
    loop_frame = 0;
}

/* main.c, after a jump to port_state_jump: the executable to start in the one that ran. The state is put back
 * (its RAM has the executable's image and g_Args, and the callbacks the runtime holds are then the state's,
 * into that code), the executable's BSS is cleared as its start would, and its first pass through the frame
 * loop's top puts the state back again. */
const PortModule *port_state_target(s32 *argc, u32 *argv) {
    const PortModule *m = pending_module;
    put_all(pending, 0);
    *argc = pending->argc;
    *argv = pending->argv;
    return m;
}

/* A load that has to start another executable: from a mission, start it over in this one's place (main.c);
 * from the front end, return to the boot stub with the state's RAM, which has it start the mission. */
static void leave(void) {
    if (!port_state_jump)
        return;
    siglongjmp(*(sigjmp_buf *)port_state_jump, port_module->level ? 1 : 2);
}

/* where the mission reads the controller (the savestate mod, its native frame): the frame loop's call is the
 * one from the shallowest frame (the first, every frame) */
void port_state_point(void *frame) {
    static int tick, save_at = -1, load_at = -1;
    int r;
    if ((u32)frame < loop_frame)
        return;
    loop_frame = (u32)frame;
    if (save_at < 0) { /* headless tests: TWINE_TEST_SAVE=tick, TWINE_TEST_LOAD=tick (of this mission's loop) */
        save_at = getenv("TWINE_TEST_SAVE") ? atoi(getenv("TWINE_TEST_SAVE")) : 0;
        load_at = getenv("TWINE_TEST_LOAD") ? atoi(getenv("TWINE_TEST_LOAD")) : 0;
    }
    tick++;
    if (tick == save_at)
        request = 1;
    if (tick == load_at)
        request = 2;
    r = request;
    request = 0;
    RT_ENTER();
    if (pending && pending_module == port_module) {
        put_all(pending, 1);
        free(pending);
        pending = NULL;
    } else if (r == 1) {
        save();
    } else if (r == 2 && (pending = read_state())) {
        pending_module = module_named(pending->module);
        if (pending_module == port_module) {
            put_all(pending, 1);
            free(pending);
            pending = NULL;
        } else if (!pending_module) {
            free(pending);
            pending = NULL;
        } else {
            port_rt_depth = 0;
            leave();
        }
    }
    RT_LEAVE();
}

/* At VSync and the front end's waits (irq.c), from game code: a load asked for in the front end, or one
 * still to come in a mission; and a save asked for where there is no state to take. */
void port_state_frame(void) {
    static int test_at = -1;
    int r;
    if (test_at < 0) /* headless tests: TWINE_TEST_LOAD_FRAME=frame loads there (as F6 would) */
        test_at = getenv("TWINE_TEST_LOAD_FRAME") ? atoi(getenv("TWINE_TEST_LOAD_FRAME")) : 0;
    if (test_at && port_video_frames() == (u32)test_at)
        request = 2;
    r = request;
    if (!r && !pending)
        return;
    if (port_module && port_module->level)
        return; /* the frame loop's top will see to it */
    if (!port_module || strcmp(port_module->name, "shell")) {
        if (r == 1) {
            request = 0;
            port_video_message("States can be saved in a mission");
        }
        return; /* the boot stub, a film: once the front end or a mission runs */
    }
    request = 0;
    if (r == 1) {
        port_video_message("States can be saved in a mission");
        return;
    }
    if (pending || !(pending = read_state()))
        return;
    if (!(pending_module = module_named(pending->module))) {
        free(pending);
        pending = NULL;
        return;
    }
    port_video_message("Loading the state...");
    leave();
}

/* Leave the running executable at once, as if its main had returned (the front end's BLACKJACK: the boot stub
 * then starts the mission g_Args names) */
void port_exe_leave(void) {
    if (port_state_jump)
        siglongjmp(*(sigjmp_buf *)port_state_jump, 3);
}

/* main.c, back from the front end for a load: the state put back, and the boot stub carries on from there */
void port_state_return(void) {
    put_all(pending, 0);
}
