/* Startup sequence, before main() loads the game: the PS1's own boot animation from the user's BIOS
 * (bios_boot.c), then the port's title card (intro_card.c). Afterwards the machine goes back to exactly
 * what main() has at startup (PS1 RAM and scratchpad zero, GPU, SPU, GTE and the I/O ports at power-on),
 * so the game runs the same with or without the intro.
 *
 * TWINE_NO_INTRO=1 skips both; headless runs (TWINE_HEADLESS) skip both unless TWINE_INTRO=1.
 * TWINE_INTRO_SHOT=<n>[,<n>...] dumps intro frames (BIOS vblanks, then card frames) as intro_NNNN.ppm;
 * they don't count towards the game's TWINE_SHOT frame numbers.
 *
 * The BIOS image is the user's own and is only ever read: TWINE_BIOS=<file or folder> (only there), else a
 * bios/ folder in the current directory, else DuckStation's BIOS search directory (its settings.ini,
 * read-only). An American one (by its "System ROM Version ... A" string) is preferred, as the disc is
 * NTSC-U. Without one, the card still shows. */
#define _GNU_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "port.h"
#include "gpu.h"
#include "io.h"
#include "spu.h"
#include "intro.h"

#define BIOS_SIZE (512 * 1024)

void GTE_reset(void);

static u32 intro_frames;
static u32 shots[64];
static int nshots = -1;

u32 intro_next_frame(void) {
    return intro_frames++;
}

int intro_shot_wanted(u32 frame) {
    int i;
    if (nshots < 0) {
        const char *s = getenv("TWINE_INTRO_SHOT");
        nshots = 0;
        while (s && *s && nshots < 64) {
            char *end;
            shots[nshots++] = (u32)strtoul(s, &end, 10);
            s = *end ? end + 1 : end;
        }
    }
    for (i = 0; i < nshots; i++)
        if (shots[i] == frame)
            return 1;
    return 0;
}

void intro_audio(int frames) {
    s16 buf[1024 * 2];
    while (frames > 0) {
        int n = frames < 1024 ? frames : 1024;
        spu_render(buf, n);
        port_audio_wav(buf, n);
        frames -= n;
    }
}

/* ---- finding the BIOS ---- */

/* How much we want this file as the BIOS: 0 = not a PS1 BIOS, 1 = unknown region, 2 = not American,
 * 3 = American (NTSC-U: SCPH-1001, 5001, 5501, 7001, 7501, 9001, 101...). */
static int bios_rank(const u8 *rom, char *version, size_t vlen) {
    const char *key = "System ROM Version ";
    size_t klen = strlen(key), i;
    int rank = 0;
    for (i = 0; i < 0x200 - 27; i++)
        if (!memcmp(rom + i, "Sony Computer Entertainment", 27))
            rank = 1;
    if (!rank)
        return 0;
    version[0] = 0;
    for (i = 0; i + klen + 20 < BIOS_SIZE; i++) {
        unsigned maj, min, mm, dd, yy;
        char region;
        if (memcmp(rom + i, key, klen))
            continue;
        if (sscanf((const char *)rom + i + klen, "%u.%u %u/%u/%u %c", &maj, &min, &mm, &dd, &yy, &region) == 6) {
            snprintf(version, vlen, "%u.%u %02u/%02u/%02u %c", maj, min, mm, dd, yy, region);
            rank = region == 'A' ? 3 : 2;
        }
    }
    return rank;
}

static u8 *best_rom;
static int best_rank;
static char best_path[1024];

static void consider(const char *path) {
    struct stat st;
    FILE *f;
    u8 *rom;
    char version[64];
    int rank;
    if (stat(path, &st) || !S_ISREG(st.st_mode) || st.st_size != BIOS_SIZE)
        return;
    if (!(f = fopen(path, "rb")))
        return;
    rom = malloc(BIOS_SIZE);
    if (fread(rom, 1, BIOS_SIZE, f) != BIOS_SIZE)
        rank = 0;
    else
        rank = bios_rank(rom, version, sizeof(version));
    fclose(f);
    if (rank > best_rank) {
        free(best_rom);
        best_rom = rom;
        best_rank = rank;
        snprintf(best_path, sizeof(best_path), "%s%s%s%s", path, version[0] ? " (version " : "", version,
                 version[0] ? ")" : "");
    } else
        free(rom);
}

static void consider_dir(const char *dir) {
    DIR *d = opendir(dir);
    struct dirent *e;
    char **names = NULL;
    int n = 0, i;
    if (!d)
        return;
    while ((e = readdir(d)))
        if (e->d_name[0] != '.') {
            names = realloc(names, (n + 1) * sizeof(*names));
            names[n++] = strdup(e->d_name);
        }
    closedir(d);
    for (i = 0; i < n; i++) { /* in name order, so the choice among equals is stable */
        int j;
        for (j = i + 1; j < n; j++)
            if (strcmp(names[j], names[i]) < 0) {
                char *t = names[i];
                names[i] = names[j];
                names[j] = t;
            }
    }
    for (i = 0; i < n; i++) {
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir, names[i]);
        consider(path);
        free(names[i]);
    }
    free(names);
}

/* [BIOS] SearchDirectory from DuckStation's settings.ini (relative paths are from its config folder) */
static int duckstation_dir(char *out, size_t len) {
    const char *home = getenv("HOME"), *xdg = getenv("XDG_CONFIG_HOME");
    char conf[1024], ini[1100], line[1024];
    int in_bios = 0, found = 0;
    FILE *f;
    if (xdg && *xdg)
        snprintf(conf, sizeof(conf), "%s/duckstation", xdg);
    else if (home)
        snprintf(conf, sizeof(conf), "%s/.config/duckstation", home);
    else
        return 0;
    snprintf(ini, sizeof(ini), "%s/settings.ini", conf);
    if (!(f = fopen(ini, "r")))
        return 0;
    while (!found && fgets(line, sizeof(line), f)) {
        char *p = line, *v, *end;
        while (*p == ' ' || *p == '\t')
            p++;
        if (*p == '[') {
            in_bios = !strncmp(p, "[BIOS]", 6);
            continue;
        }
        if (!in_bios || strncmp(p, "SearchDirectory", 15))
            continue;
        v = strchr(p, '=');
        if (!v)
            continue;
        for (v++; *v == ' ' || *v == '\t'; v++)
            ;
        for (end = v + strlen(v); end > v && (end[-1] == '\n' || end[-1] == '\r' || end[-1] == ' '); end--)
            ;
        *end = 0;
        if ((size_t)snprintf(out, len, "%s%s%s", *v == '/' ? "" : conf, *v == '/' ? "" : "/", v) < len)
            found = *v != 0;
    }
    fclose(f);
    return found;
}

static u8 *find_bios(void) {
    const char *env = getenv("TWINE_BIOS");
    char dir[1024];
    struct stat st;
    if (env) { /* an explicit choice: no searching elsewhere */
        if (!stat(env, &st) && S_ISDIR(st.st_mode))
            consider_dir(env);
        else
            consider(env);
    } else {
        consider_dir("bios");
        if (!best_rom && duckstation_dir(dir, sizeof(dir)))
            consider_dir(dir);
    }
    if (best_rom)
        port_log("intro: BIOS %s\n", best_path);
    return best_rom;
}

/* Everything the BIOS run touched, back to how the process started. */
static void restore_machine(void) {
    memset((void *)PS1_RAM_BASE, 0, PS1_RAM_SIZE);
    memset((void *)PS1_SCRATCH_BASE, 0, PS1_SCRATCH_SIZE);
    gpu_reset();
    port_io_reset();
    spu_reset();
    GTE_reset();
}

void port_intro(void) {
    int headless = port_video_headless();
    u8 *rom;
    u32 entry;
    if (getenv("TWINE_NO_INTRO") || (headless && !getenv("TWINE_INTRO")))
        return;
    if (headless) /* the boot's sound runs on console time (bios_boot.c feeds it per frame) */
        spu_set_clock(SPU_CLOCK_MANUAL);
    rom = find_bios();
    entry = rom ? cdc_boot_entry() : 0; /* where the BIOS will hand over to the disc */
    if (!rom)
        port_log("intro: no PS1 BIOS found (TWINE_BIOS, bios/, DuckStation's BIOS folder); no boot animation\n");
    else if (!entry)
        port_log("intro: no bootable executable on the disc (SYSTEM.CNF); no boot animation\n");
    else {
        port_log("intro: the BIOS hands over at %08X\n", entry);
        bios_boot_run(rom, entry);
        restore_machine();
    }
    free(rom);
    best_rom = NULL;
    intro_card_run();
    if (headless)
        spu_set_clock(SPU_CLOCK_HOST);
}
