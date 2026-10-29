/* First run: the disc image (and the optional BIOS) the port plays, chosen and checked in the game's window, and
 * the notices of what didn't start as asked (no hardware renderer, no sound, a folder it can't write to).
 *
 * port_setup (main.c, after port_video_init) looks at the disc twine.ini or the environment name (TWINE_DISC; a .cue
 * sheet stands for its .bin): when it isn't there, can't be read or isn't the right disc, or with --setup on the
 * command line, the setup screen shows. It says what is wrong in words, opens the system's file chooser
 * (SDL_ShowOpenFileDialog), takes a file dropped on the window or a path pasted with Ctrl+V, and saves a good choice
 * in twine.ini (disc = ..., bios = ...). The right disc is the American release, SLUS-01272, as a raw image (2352-byte
 * sectors: .bin, or the .cue that names it); its SYSTEM.CNF, region and boot program are checked, so another region,
 * another game, a prototype or a cut-short copy are each named as such. The BIOS only plays the console's boot
 * animation (intro.c): optional, and said so.
 *
 * port_notice collects what the player should know before playing; port_setup_notices (main.c, after
 * port_audio_init) shows them on one screen, each with "don't show again" (twine.ini: quiet_<key> = 1).
 *
 * Headless (TWINE_HEADLESS) nothing shows: a missing or wrong disc ends the run with the same words (port_fatal), and
 * notices only go to the log. TWINE_SETUP_SHOT=<file.bmp> (headless) draws the screen as a 1280x960 window would
 * show it (TWINE_SETUP_SHOT_SIZE=<w>x<h>: another size), for the disc and BIOS the environment names
 * (TWINE_SETUP_SHOT_PAGE=notices: the notices), writes it and ends the run.
 *
 * Only SDL here (files, dialogs, drawing), so it is the same on every system. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "port.h"
#include "intro.h"

void port_setting_save(const char *key, const char *value); /* main.c */
void *port_video_window(void);                              /* video.c */

#define RETAIL_SERIAL "SLUS_012.72"
#define RETAIL_EXE_SIZE 65536u
#define RETAIL_EXE_CRC 0xD25CDB2Bu /* SDL_crc32 of the retail disc's SLUS_012.72 */
#define DEV_DISC "workbench/disc/007 - The World Is Not Enough (USA).bin" /* (cdrom.c's default, for development) */

/* ---------------------------------------------------------------- the disc */

typedef struct {
    int ok;
    char bin[1024];  /* the image file (a .cue sheet's .bin) */
    char what[480];  /* ok: what it is; otherwise why it can't be used, for people */
} DiscInfo;

typedef struct {
    SDL_IOStream *io;
    Sint64 size;
} Image;

static int image_sector(Image *im, u32 lba, u8 *out) {
    u8 raw[2352];
    if ((Sint64)(lba + 1) * 2352 > im->size || SDL_SeekIO(im->io, (Sint64)lba * 2352, SDL_IO_SEEK_SET) < 0 ||
        SDL_ReadIO(im->io, raw, sizeof(raw)) != sizeof(raw))
        return 0;
    memcpy(out, raw + 24, 2048); /* (mode 2 form 1: the data after sync, header and subheader) */
    return 1;
}

static u32 le32(const u8 *p) {
    return p[0] | p[1] << 8 | p[2] << 16 | (u32)p[3] << 24;
}

/* the file or directory `name` (";1" not needed, any case) in the directory at lba */
static int iso_lookup(Image *im, u32 lba, u32 size, const char *name, u32 *out_lba, u32 *out_size) {
    u8 sec[2048];
    u32 s, n = strlen(name);
    if (size > 1024 * 1024)
        return 0;
    for (s = 0; s < (size + 2047) / 2048; s++) {
        u32 i = 0;
        if (!image_sector(im, lba + s, sec))
            return 0;
        while (i + 33 < 2048 && sec[i]) {
            const u8 *r = sec + i;
            u32 len = r[32];
            if (i + 33 + len > 2048)
                break;
            if ((len == n || (len > n && r[33 + n] == ';')) && !SDL_strncasecmp((const char *)r + 33, name, n)) {
                *out_lba = le32(r + 2);
                *out_size = le32(r + 10);
                return 1;
            }
            i += r[0];
        }
    }
    return 0;
}

/* a path ("L01/LEVEL01.EXE") from the root directory of the volume whose descriptor is pvd */
static int iso_path(Image *im, const u8 *pvd, const char *path, u32 *lba, u32 *size) {
    char part[64];
    *lba = le32(pvd + 156 + 2);
    *size = le32(pvd + 156 + 10);
    while (*path) {
        size_t n = strcspn(path, "/");
        if (n >= sizeof(part))
            return 0;
        memcpy(part, path, n);
        part[n] = 0;
        if (!iso_lookup(im, *lba, *size, part, lba, size))
            return 0;
        path += n + (path[n] == '/');
    }
    return 1;
}

static u8 *iso_read(Image *im, u32 lba, u32 size) {
    u8 *data;
    u32 s;
    if (size > 4 * 1024 * 1024 || !(data = SDL_malloc((size + 2047) & ~2047u)))
        return NULL;
    for (s = 0; s < (size + 2047) / 2048; s++)
        if (!image_sector(im, lba + s, data + s * 2048)) {
            SDL_free(data);
            return NULL;
        }
    return data;
}

static const char *file_name(const char *path) {
    const char *a = strrchr(path, '/'), *b = strrchr(path, '\\');
    if (b > a)
        a = b;
    return a ? a + 1 : path;
}

static int has_ext(const char *path, const char *ext) {
    size_t n = strlen(path), e = strlen(ext);
    return n > e && !SDL_strcasecmp(path + n - e, ext);
}

static int is_dir(const char *path) {
    SDL_PathInfo info;
    return SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_DIRECTORY;
}

static int exists(const char *path) {
    return SDL_GetPathInfo(path, NULL);
}

/* the .bin a .cue sheet names (its first FILE: track 1, the data), next to the sheet */
static int cue_bin(const char *cue, char *bin, size_t len, char *why, size_t wlen) {
    size_t size;
    char *text = SDL_LoadFile(cue, &size), *p, name[512];
    int found = 0;
    if (!text) {
        snprintf(why, wlen, "Can't read \"%s\": %s.", file_name(cue), SDL_GetError());
        return 0;
    }
    for (p = text; p && *p && !found;) {
        while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n' || (unsigned char)*p == 0xEF ||
               (unsigned char)*p == 0xBB || (unsigned char)*p == 0xBF)
            p++;
        if (!SDL_strncasecmp(p, "FILE", 4) && (p[4] == ' ' || p[4] == '\t')) {
            char *s = p + 5, *e;
            while (*s == ' ' || *s == '\t')
                s++;
            if (*s == '"') {
                e = strchr(++s, '"');
            } else {
                e = s + strcspn(s, " \t\r\n");
            }
            if (e && e > s && (size_t)(e - s) < sizeof(name)) {
                memcpy(name, s, e - s);
                name[e - s] = 0;
                found = 1;
            }
        }
        p = strchr(p, '\n');
    }
    SDL_free(text);
    if (!found) {
        snprintf(why, wlen, "This .cue sheet doesn't name its disc image (.bin).");
        return 0;
    }
    if (name[0] == '/' || (name[0] && name[1] == ':'))
        snprintf(bin, len, "%s", name);
    else
        snprintf(bin, len, "%.*s%s", (int)(file_name(cue) - cue), cue, name);
    if (!exists(bin)) {
        snprintf(why, wlen, "This .cue sheet's disc image, \"%.200s\", isn't next to it. Keep the .cue and the .bin in "
                            "one folder.", name);
        return 0;
    }
    return 1;
}

static const char *region_of(const char *serial) {
    if (!SDL_strncasecmp(serial, "SLUS", 4) || !SDL_strncasecmp(serial, "SCUS", 4))
        return "American (NTSC-U)";
    if (!SDL_strncasecmp(serial, "SLES", 4) || !SDL_strncasecmp(serial, "SCES", 4) ||
        !SDL_strncasecmp(serial, "SCED", 4) || !SDL_strncasecmp(serial, "SLED", 4))
        return "European (PAL)";
    if (!SDL_strncasecmp(serial, "SLPS", 4) || !SDL_strncasecmp(serial, "SLPM", 4) ||
        !SDL_strncasecmp(serial, "SCPS", 4) || !SDL_strncasecmp(serial, "SCPM", 4) ||
        !SDL_strncasecmp(serial, "SIPS", 4) || !SDL_strncasecmp(serial, "SLKA", 4))
        return "Japanese or Asian (NTSC-J)";
    return NULL;
}

/* "SLES_031.60" -> "SLES-03160" */
static void serial_text(const char *boot, char *out, size_t len) {
    size_t n = 0;
    for (; *boot && n + 1 < len; boot++)
        if (*boot == '_')
            out[n++] = '-';
        else if (*boot != '.')
            out[n++] = (char)SDL_toupper((unsigned char)*boot);
    out[n] = 0;
}

static void disc_check(const char *path, DiscInfo *d) {
    Image im = {0};
    u8 head[16] = {0}, sec[2048], pvd[2048], *cnf = NULL, *exe = NULL;
    u32 lba, size, volume;
    char boot[64] = "", *b;
    memset(d, 0, sizeof(*d));
    if (!path || !*path) {
        snprintf(d->what, sizeof(d->what), "No disc image chosen yet.");
        return;
    }
    if (is_dir(path)) {
        snprintf(d->what, sizeof(d->what), "That's a folder: choose the disc image in it (its .cue or .bin file).");
        return;
    }
    if (!exists(path)) {
        snprintf(d->what, sizeof(d->what), "Can't find \"%s\" (moved or renamed?). Choose it again.",
                 file_name(path));
        return;
    }
    if (has_ext(path, ".cue")) {
        if (!cue_bin(path, d->bin, sizeof(d->bin), d->what, sizeof(d->what)))
            return;
    } else
        snprintf(d->bin, sizeof(d->bin), "%s", path);
    if (!(im.io = SDL_IOFromFile(d->bin, "rb"))) {
        snprintf(d->what, sizeof(d->what), "Can't read \"%s\": %s.", file_name(d->bin), SDL_GetError());
        return;
    }
    im.size = SDL_GetIOSize(im.io);
    SDL_ReadIO(im.io, head, sizeof(head));
    if (!memcmp(head, "MComprHD", 8) || has_ext(path, ".chd")) {
        snprintf(d->what, sizeof(d->what), "CHD images aren't supported yet. Turn it into .bin/.cue with MAME's chdman "
                                           "(chdman extractcd -i game.chd -o game.cue), then choose the .cue.");
        goto done;
    }
    if (!memcmp(head, "PK\3\4", 4) || !memcmp(head, "7z\xBC\xAF", 4) || !memcmp(head, "Rar!", 4) ||
        (head[0] == 0x1F && head[1] == 0x8B)) {
        snprintf(d->what, sizeof(d->what), "This is a compressed archive: unpack it first, then choose the .cue or "
                                           ".bin inside.");
        goto done;
    }
    if (!memcmp(head, "\0PBP", 4) || !memcmp(head, "ECM", 3)) {
        snprintf(d->what, sizeof(d->what), "%s images aren't supported: use a plain .bin/.cue copy of the disc.",
                 head[0] ? "ECM" : "PBP");
        goto done;
    }
    if (im.size >= 17 * 2352 && SDL_SeekIO(im.io, 16 * 2352, SDL_IO_SEEK_SET) >= 0 &&
        SDL_ReadIO(im.io, head, 12) == 12 && !memcmp(head, "\0\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0", 12)) {
        /* raw sectors: the kind the port reads */
    } else {
        if (im.size >= 17 * 2048 && SDL_SeekIO(im.io, 16 * 2048, SDL_IO_SEEK_SET) >= 0 &&
            SDL_ReadIO(im.io, sec, 6) == 6 && !memcmp(sec + 1, "CD001", 5))
            snprintf(d->what, sizeof(d->what), "This is a 2048-byte .iso, which leaves out the sectors the game's "
                                               "speech, music and movies are in. Use a raw .bin/.cue copy of the "
                                               "disc (as DuckStation and disc rippers make).");
        else
            snprintf(d->what, sizeof(d->what), "This file isn't a CD image. Choose the game's .cue or .bin file.");
        goto done;
    }
    if (!image_sector(&im, 16, pvd) || pvd[0] != 1 || memcmp(pvd + 1, "CD001", 5)) {
        snprintf(d->what, sizeof(d->what), "This disc image has no readable file system (damaged, or not a game "
                                           "disc's first track?).");
        goto done;
    }
    if (memcmp(pvd + 8, "PLAYSTATION", 11)) {
        snprintf(d->what, sizeof(d->what), "This is a CD image, but not a PlayStation disc.");
        goto done;
    }
    if (!iso_path(&im, pvd, "SYSTEM.CNF", &lba, &size) || size > 4096 || !(cnf = iso_read(&im, lba, size))) {
        snprintf(d->what, sizeof(d->what), "This PlayStation disc has no game program on it (no SYSTEM.CNF).");
        goto done;
    }
    cnf[size < 2047 ? size : 2047] = 0;
    if ((b = SDL_strcasestr((char *)cnf, "BOOT"))) {
        char *s = SDL_strcasestr(b, "cdrom:"), *e;
        if (s) {
            for (s += 6; *s == '\\' || *s == '/'; s++)
                ;
            e = s + strcspn(s, ";\r\n \t");
            snprintf(boot, sizeof(boot), "%.*s", (int)(e - s), s);
        }
    }
    volume = le32(pvd + 80);
    if (SDL_strcasecmp(boot, RETAIL_SERIAL)) {
        char serial[64];
        const char *region = region_of(file_name(boot));
        int twine = !SDL_strncasecmp((const char *)pvd + 40, "TWINE", 5) || iso_path(&im, pvd, "L01/LEVEL01.EXE",
                                                                                       &lba, &size);
        serial_text(file_name(boot), serial, sizeof(serial));
        if (twine)
            snprintf(d->what, sizeof(d->what), "This is The World Is Not Enough, but the %s release (%s). The port is "
                     "made from the American release, SLUS-01272, and runs only that one.",
                     region ? region : "another", *serial ? serial : "unknown serial");
        else
            snprintf(d->what, sizeof(d->what), "This is another PlayStation game (%s%s%s). The port needs 007: The "
                     "World Is Not Enough, the American release (SLUS-01272).", *serial ? serial : "unknown serial",
                     region ? ", " : "", region ? region : "");
        goto done;
    }
    if ((Sint64)volume * 2352 > im.size) {
        snprintf(d->what, sizeof(d->what), "This disc image is incomplete: it ends early (%d of %d MB). Copy or rip "
                 "the disc again.", (int)(im.size >> 20), (int)(((Sint64)volume * 2352) >> 20));
        goto done;
    }
    if (!iso_path(&im, pvd, RETAIL_SERIAL, &lba, &size) || size != RETAIL_EXE_SIZE || !(exe = iso_read(&im, lba, size)) ||
        SDL_crc32(0, exe, size) != RETAIL_EXE_CRC) {
        snprintf(d->what, sizeof(d->what), "This is SLUS-01272, but its game program isn't the retail one (a "
                                           "prototype, a patched or a damaged image?). The port needs the retail "
                                           "disc.");
        goto done;
    }
    d->ok = 1;
    snprintf(d->what, sizeof(d->what), "007: The World Is Not Enough, American release (SLUS-01272).");
done:
    SDL_free(cnf);
    SDL_free(exe);
    SDL_CloseIO(im.io);
}

/* ---------------------------------------------------------------- notices */

typedef struct {
    char key[16];
    char text[400];
} Notice;
static Notice notices[8];
static int notice_count;

void port_notice(const char *key, const char *fmt, ...) {
    char name[64];
    const char *quiet;
    va_list ap;
    Notice *n;
    snprintf(name, sizeof(name), "TWINE_QUIET_%s", key);
    for (char *c = name; *c; c++)
        *c = (char)SDL_toupper((unsigned char)*c);
    quiet = getenv(name);
    if (notice_count >= (int)(sizeof(notices) / sizeof(notices[0])))
        return;
    n = &notices[notice_count];
    snprintf(n->key, sizeof(n->key), "%s", key);
    va_start(ap, fmt);
    vsnprintf(n->text, sizeof(n->text), fmt, ap);
    va_end(ap);
    port_log("notice: %s\n", n->text);
    if (!(quiet && atoi(quiet)))
        notice_count++;
}

/* the folders the port writes to (settings; memory cards, save states, scores): there, and writable */
static void check_folder(const char *env, const char *fallback, const char *what) {
    const char *base = getenv(env), *home = getenv("HOME");
    char dir[1024], probe[1100];
    SDL_IOStream *io;
    if (base && *base)
        snprintf(dir, sizeof(dir), "%s/twine", base);
    else if (home && *home)
        snprintf(dir, sizeof(dir), "%s/%s/twine", home, fallback);
    else
        return;
    snprintf(probe, sizeof(probe), "%s/.write-test", dir);
    if (SDL_CreateDirectory(dir) && (io = SDL_IOFromFile(probe, "wb"))) {
        int ok = SDL_WriteIO(io, "ok", 2) == 2;
        ok &= SDL_CloseIO(io);
        SDL_RemovePath(probe);
        if (ok)
            return;
    }
    port_notice("folders", "Can't write to %s (%s): %s won't be kept. Check that folder's owner and permissions.",
                dir, SDL_GetError(), what);
}

/* ---------------------------------------------------------------- drawing */

static SDL_Renderer *R;
static float K, OX, OY; /* the 640x480 layout's scale (of the 8x8 font's pixels) and origin */

typedef struct {
    Uint8 r, g, b;
} Rgb;
static const Rgb GOLD = {255, 210, 80}, IVORY = {236, 232, 218}, DIM = {150, 148, 140}, GREEN = {130, 215, 130},
                 RED = {245, 120, 95}, BLACK = {0, 0, 0}, PANEL = {20, 20, 24}, EDGE = {64, 62, 58};

static void colour(Rgb c, Uint8 a) {
    SDL_SetRenderDrawColor(R, c.r, c.g, c.b, a);
}

static void text(float x, float y, int s, Rgb c, const char *t) {
    colour(c, 255);
    SDL_SetRenderScale(R, K * s, K * s);
    SDL_RenderDebugText(R, (OX + x * K) / (K * s), (OY + y * K) / (K * s), t);
    SDL_SetRenderScale(R, 1, 1);
}

static void text_centred(float y, int s, Rgb c, const char *t) {
    text((640 - (float)strlen(t) * 8 * s) / 2, y, s, c, t);
}

static void box(float x, float y, float w, float h, Rgb c, int fill) {
    SDL_FRect r = {OX + x * K, OY + y * K, w * K, h * K};
    colour(c, 255);
    if (fill)
        SDL_RenderFillRect(R, &r);
    else {
        SDL_FRect e[4] = {{r.x, r.y, r.w, K}, {r.x, r.y + r.h - K, r.w, K}, {r.x, r.y, K, r.h},
                          {r.x + r.w - K, r.y, K, r.h}};
        SDL_RenderFillRects(R, e, 4);
    }
}

/* text broken into lines of at most `cols` characters, at y, line after line; returns the y after it */
static float para(float x, float y, int cols, Rgb c, const char *t) {
    char line[128];
    while (*t) {
        int n = (int)strlen(t), cut = n;
        if (n > cols) {
            for (cut = cols; cut > 0 && t[cut] != ' '; cut--)
                ;
            if (cut == 0)
                cut = cols;
        }
        snprintf(line, sizeof(line), "%.*s", cut, t);
        text(x, y, 1, c, line);
        y += 11;
        t += cut;
        while (*t == ' ')
            t++;
    }
    return y;
}

/* a path shown in at most `cols` characters: its end, which names it */
static void path_line(float x, float y, int cols, Rgb c, const char *path) {
    char line[128];
    size_t n = strlen(path);
    if ((int)n <= cols)
        snprintf(line, sizeof(line), "%s", path);
    else
        snprintf(line, sizeof(line), "...%s", path + n - (cols - 3));
    for (char *p = line; *p; p++)
        if ((unsigned char)*p >= 0x80)
            *p = '?';
    text(x, y, 1, c, line);
}

typedef struct {
    float x, y, w, h;
    const char *label;
    int enabled;
} Button;

static void button(const Button *b, int focused) {
    float tx = b->x + (b->w - (float)strlen(b->label) * 8) / 2, ty = b->y + (b->h - 8) / 2;
    if (!b->enabled) {
        box(b->x, b->y, b->w, b->h, EDGE, 0);
        text(tx, ty, 1, EDGE, b->label);
    } else if (focused) {
        box(b->x, b->y, b->w, b->h, GOLD, 1);
        text(tx, ty, 1, BLACK, b->label);
    } else {
        box(b->x, b->y, b->w, b->h, DIM, 0);
        text(tx, ty, 1, IVORY, b->label);
    }
}

static void begin(int w, int h) {
    float k = SDL_min(w / 640.0f, h / 480.0f);
    /* whole multiples of the font's pixels, or halves where a whole one would leave much unused (a Steam Deck's
     * 1280x800: 1.5) */
    K = SDL_floorf(k) >= 2 || k < 1.5f ? SDL_max(1.0f, SDL_floorf(k)) : SDL_floorf(k * 2) / 2;
    OX = SDL_floorf((w - 640 * K) / 2);
    OY = SDL_floorf((h - 480 * K) / 2);
    SDL_SetRenderScale(R, 1, 1);
    SDL_SetRenderDrawBlendMode(R, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(R, 8, 8, 10, 255);
    SDL_RenderClear(R);
    text_centred(30, 3, GOLD, "007");
    text_centred(62, 2, IVORY, "THE WORLD IS NOT ENOUGH");
    box(160, 84, 320, 2, GOLD, 1);
    text_centred(94, 1, DIM, "PC PORT");
}

/* ---------------------------------------------------------------- the screens */

enum { PAGE_SETUP, PAGE_NOTICES };
enum { B_DISC, B_BIOS, B_PLAY, B_QUIT, B_OK = 0, B_QUIET, B_COUNT = 4 };

static struct {
    int page;
    DiscInfo disc;
    char disc_path[1024]; /* what the player chose (a .cue stays a .cue) */
    int bios_rank;
    char bios_desc[1200], bios_note[300];
    char note[400]; /* the last thing done or refused, under the buttons */
    int note_bad;
    Button b[B_COUNT];
    int nb, focus;
    char ini[1100];
} S;

static void layout(void) {
    memset(S.b, 0, sizeof(S.b));
    if (S.page == PAGE_SETUP) {
        S.b[B_DISC] = (Button){40, 236, 220, 20, "CHOOSE DISC IMAGE...", 1};
        S.b[B_BIOS] = (Button){40, 338, 220, 20, "CHOOSE BIOS...", 1};
        S.b[B_PLAY] = (Button){196, 394, 120, 22, "PLAY", S.disc.ok};
        S.b[B_QUIT] = (Button){324, 394, 120, 22, "QUIT", 1};
        S.nb = 4;
    } else {
        S.b[B_OK] = (Button){156, 394, 140, 22, "CONTINUE", 1};
        S.b[B_QUIET] = (Button){304, 394, 180, 22, "DON'T SHOW AGAIN", 1};
        S.nb = 2;
    }
    if (S.focus >= S.nb || !S.b[S.focus].enabled)
        S.focus = S.page == PAGE_SETUP ? (S.disc.ok ? B_PLAY : B_DISC) : B_OK;
}

static void draw(int w, int h) {
    int i;
    float y;
    layout();
    begin(w, h);
    if (S.page == PAGE_SETUP) {
        para(40, 116, 70, IVORY, "This port plays your own copy of the game: the American PlayStation release, "
                                 "SLUS-01272, as a disc image (.cue and .bin).");
        box(24, 146, 592, 118, PANEL, 1);
        box(24, 146, 592, 118, EDGE, 0);
        text(40, 156, 1, GOLD, "DISC IMAGE");
        path_line(40, 172, 70, S.disc_path[0] ? IVORY : DIM, S.disc_path[0] ? S.disc_path : "(none chosen)");
        y = para(40, 190, 70, S.disc.ok ? GREEN : RED, S.disc.what);
        (void)y;
        box(24, 272, 592, 94, PANEL, 1);
        box(24, 272, 592, 94, EDGE, 0);
        text(40, 282, 1, GOLD, "PS1 BIOS");
        text(112, 282, 1, DIM, "(OPTIONAL)");
        text(40, 298, 1, DIM, "Only plays the console's start-up animation; the game doesn't need it.");
        if (S.bios_rank)
            path_line(40, 314, 70, S.bios_rank ? GREEN : DIM, S.bios_desc);
        else
            text(40, 314, 1, DIM, "None found: the game starts with the port's title card.");
        if (S.bios_note[0])
            text(272, 344, 1, S.bios_rank ? DIM : RED, S.bios_note);
        if (S.note[0])
            para(40, 426, 70, S.note_bad ? RED : GREEN, S.note);
        else
            text_centred(426, 1, DIM, "You can also drop the file on this window, or paste its path (Ctrl+V).");
        text(40, 458, 1, EDGE, "Settings:");
        path_line(120, 458, 60, EDGE, S.ini);
    } else {
        text(40, 116, 1, GOLD, "BEFORE YOU PLAY");
        y = 136;
        for (i = 0; i < notice_count; i++) {
            box(32, y + 3, 3, 3, GOLD, 1);
            y = para(44, y, 68, IVORY, notices[i].text) + 8;
        }
    }
    for (i = 0; i < S.nb; i++)
        button(&S.b[i], i == S.focus);
}

/* ---- choosing files ---- */

static SDL_AtomicInt picked_kind; /* 0 none; B_DISC or B_BIOS + 1: picked_path holds the choice */
static char picked_path[1024], picked_error[300];
static SDL_AtomicInt dialog_open;

static void SDLCALL picked(void *userdata, const char *const *files, int filter) {
    int kind = (int)(intptr_t)userdata;
    (void)filter;
    if (!files) {
        snprintf(picked_error, sizeof(picked_error), "%s", SDL_GetError());
        picked_path[0] = 0;
    } else if (!files[0]) {
        picked_path[0] = picked_error[0] = 0; /* cancelled */
        kind = -1;
    } else {
        snprintf(picked_path, sizeof(picked_path), "%s", files[0]);
        picked_error[0] = 0;
    }
    SDL_SetAtomicInt(&picked_kind, kind < 0 ? -1 : kind + 1);
    SDL_SetAtomicInt(&dialog_open, 0);
}

static void open_dialog(int kind) {
    static const SDL_DialogFileFilter disc_filters[] = {{"PlayStation disc image (.cue, .bin)", "cue;bin;CUE;BIN"},
                                                        {"All files", "*"}};
    static const SDL_DialogFileFilter bios_filters[] = {{"PlayStation BIOS (.bin, .rom)", "bin;rom;BIN;ROM"},
                                                        {"All files", "*"}};
    static char where[1024];
    const char *from = kind == B_DISC ? S.disc_path : S.bios_desc;
    if (SDL_GetAtomicInt(&dialog_open))
        return;
    where[0] = 0;
    if (*from) {
        snprintf(where, sizeof(where), "%.*s", (int)(file_name(from) - from), from);
        if (!is_dir(where))
            where[0] = 0;
    }
    SDL_SetAtomicInt(&dialog_open, 1);
    SDL_ShowOpenFileDialog(picked, (void *)(intptr_t)kind, (SDL_Window *)port_video_window(),
                           kind == B_DISC ? disc_filters : bios_filters, 2, where[0] ? where : NULL, false);
}

static void look_at_bios(void) {
    S.bios_rank = port_intro_bios(NULL, S.bios_desc, sizeof(S.bios_desc));
}

static void choose_disc(const char *path) {
    DiscInfo d;
    disc_check(path, &d);
    if (!d.ok) {
        /* (shown as the disc's state only when nothing usable was chosen before) */
        if (!S.disc.ok) {
            S.disc = d;
            snprintf(S.disc_path, sizeof(S.disc_path), "%s", path);
            S.note[0] = 0;
        } else {
            snprintf(S.note, sizeof(S.note), "Not used: %.380s", d.what);
            S.note_bad = 1;
        }
        port_log("setup: %s: %s\n", path, d.what);
        return;
    }
    S.disc = d;
    snprintf(S.disc_path, sizeof(S.disc_path), "%s", path);
    port_setting_save("disc", path);
    setenv("TWINE_DISC", d.bin, 1); /* (the C library's: what cdrom.c's getenv reads; SDL's is Windows' own) */
    snprintf(S.note, sizeof(S.note), "Saved. Press PLAY.");
    S.note_bad = 0;
    S.focus = B_PLAY;
}

static void choose_bios(const char *path) {
    char desc[1200];
    int rank = port_intro_bios(path, desc, sizeof(desc));
    if (!rank) {
        snprintf(S.bios_note, sizeof(S.bios_note), "Not a PS1 BIOS (a 512 KB file).");
        port_log("setup: %s: not a PS1 BIOS\n", path);
        return;
    }
    port_setting_save("bios", path);
    look_at_bios();
    snprintf(S.bios_note, sizeof(S.bios_note), "%s", rank == 3 ? "Saved." : "Saved (not the American one: fine).");
}

/* a file named some other way than the chooser: by size, a BIOS (512 KB) or a disc */
static void choose_any(const char *given) {
    char path[1024];
    size_t n;
    SDL_PathInfo info;
    while (*given == ' ' || *given == '\t' || *given == '"' || *given == '\'')
        given++;
    if (!SDL_strncasecmp(given, "file://", 7)) { /* a file manager's copy: a URL */
        const char *s = given + 7;
        for (n = 0; *s && n < sizeof(path) - 1; s++)
            if (*s == '%' && SDL_isxdigit((unsigned char)s[1]) && SDL_isxdigit((unsigned char)s[2])) {
                char hex[3] = {s[1], s[2], 0};
                path[n++] = (char)strtol(hex, NULL, 16);
                s += 2;
            } else
                path[n++] = *s;
        path[n] = 0;
    } else
        snprintf(path, sizeof(path), "%s", given);
    path[strcspn(path, "\r\n")] = 0; /* (the first of a list) */
    for (n = strlen(path); n && strchr(" \t\"'", path[n - 1]); n--)
        path[n - 1] = 0;
    if (!*path)
        return;
    if (SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_FILE && info.size == 512 * 1024)
        choose_bios(path);
    else
        choose_disc(path);
}

static void activate(int i) {
    if (S.page == PAGE_NOTICES) {
        if (i == B_QUIET)
            for (int k = 0; k < notice_count; k++) {
                char key[64];
                snprintf(key, sizeof(key), "quiet_%.15s", notices[k].key);
                port_setting_save(key, "1");
            }
        S.page = -1; /* done */
        return;
    }
    switch (i) {
    case B_DISC:
    case B_BIOS:
        open_dialog(i);
        break;
    case B_PLAY:
        if (S.disc.ok)
            S.page = -1;
        break;
    case B_QUIT:
        port_log("setup: quit\n");
        SDL_Quit();
        exit(0);
    }
}

static void step_focus(int d) {
    int i, f = S.focus;
    for (i = 0; i < S.nb; i++) {
        f = (f + d + S.nb) % S.nb;
        if (S.b[f].enabled) {
            S.focus = f;
            return;
        }
    }
}

static int hit(float x, float y) {
    int i;
    for (i = 0; i < S.nb; i++) {
        const Button *b = &S.b[i];
        float bx = OX + b->x * K, by = OY + b->y * K;
        if (b->enabled && x >= bx && y >= by && x < bx + b->w * K && y < by + b->h * K)
            return i;
    }
    return -1;
}

/* the screen in the window until PLAY, CONTINUE or QUIT (the window's close button quits too) */
static void run(void) {
    SDL_Event e, kept[16];
    SDL_Gamepad *pads[8] = {0};
    int nkept = 0, i;
    R = port_video_renderer();
    if (!R)
        return;
    while (S.page >= 0) {
        int w = 0, h = 0, kind;
        SDL_GetCurrentRenderOutputSize(R, &w, &h);
        draw(w, h);
        SDL_RenderPresent(R);
        if ((kind = SDL_GetAtomicInt(&picked_kind))) {
            SDL_SetAtomicInt(&picked_kind, 0);
            if (kind > 0 && picked_path[0])
                kind - 1 == B_BIOS ? choose_bios(picked_path) : choose_disc(picked_path);
            else if (kind > 0) {
                snprintf(S.note, sizeof(S.note), "No file chooser here (%s). Drop the file on this window, or paste "
                         "its path with Ctrl+V.", picked_error[0] ? picked_error : "none installed");
                S.note_bad = 1;
                port_log("setup: no file chooser: %s\n", picked_error);
            }
        }
        if (!SDL_WaitEventTimeout(&e, 100))
            continue;
        do {
            switch (e.type) {
            case SDL_EVENT_QUIT:
                port_log("setup: window closed\n");
                SDL_Quit();
                exit(0);
            case SDL_EVENT_GAMEPAD_ADDED:
            case SDL_EVENT_GAMEPAD_REMOVED: /* (video.c's to keep: put back afterwards) */
                if (nkept < 16)
                    kept[nkept++] = e;
                if (e.type == SDL_EVENT_GAMEPAD_ADDED)
                    for (i = 0; i < 8; i++)
                        if (!pads[i]) {
                            pads[i] = SDL_OpenGamepad(e.gdevice.which);
                            break;
                        }
                break;
            case SDL_EVENT_DROP_FILE:
                if (e.drop.data)
                    choose_any(e.drop.data);
                break;
            case SDL_EVENT_MOUSE_MOTION:
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                SDL_ConvertEventToRenderCoordinates(R, &e);
                if ((i = hit(e.type == SDL_EVENT_MOUSE_MOTION ? e.motion.x : e.button.x,
                             e.type == SDL_EVENT_MOUSE_MOTION ? e.motion.y : e.button.y)) >= 0) {
                    S.focus = i;
                    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
                        activate(i);
                }
                break;
            case SDL_EVENT_KEY_DOWN:
                if (e.key.key == SDLK_V && (e.key.mod & (SDL_KMOD_CTRL | SDL_KMOD_GUI))) {
                    char *clip = SDL_GetClipboardText();
                    if (clip && *clip)
                        choose_any(clip);
                    SDL_free(clip);
                } else if (e.key.key == SDLK_TAB)
                    step_focus(e.key.mod & SDL_KMOD_SHIFT ? -1 : 1);
                else if (e.key.key == SDLK_RIGHT || e.key.key == SDLK_DOWN)
                    step_focus(1);
                else if (e.key.key == SDLK_LEFT || e.key.key == SDLK_UP)
                    step_focus(-1);
                else if (e.key.key == SDLK_RETURN || e.key.key == SDLK_KP_ENTER || e.key.key == SDLK_SPACE)
                    activate(S.focus);
                else if (e.key.key == SDLK_ESCAPE)
                    activate(S.page == PAGE_SETUP ? B_QUIT : B_OK);
                break;
            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                if (e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT || e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN)
                    step_focus(1);
                else if (e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_LEFT ||
                         e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP)
                    step_focus(-1);
                else if (e.gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH || e.gbutton.button == SDL_GAMEPAD_BUTTON_START)
                    activate(S.focus);
                break;
            }
        } while (S.page >= 0 && SDL_PollEvent(&e));
    }
    for (i = 0; i < 8; i++)
        if (pads[i])
            SDL_CloseGamepad(pads[i]);
    for (i = 0; i < nkept; i++)
        SDL_PushEvent(&kept[i]);
    /* (a clean frame for what comes next) */
    SDL_SetRenderDrawColor(R, 0, 0, 0, 255);
    SDL_RenderClear(R);
    SDL_RenderPresent(R);
}

/* TWINE_SETUP_SHOT: the page as a 1280x960 window shows it, to a .bmp, and the end of the run */
static void shot(const char *path) {
    const char *size = getenv("TWINE_SETUP_SHOT_SIZE"); /* (<w>x<h>, another window's size) */
    int w = 1280, h = 960;
    SDL_Surface *s;
    if (size && SDL_sscanf(size, "%dx%d", &w, &h) != 2)
        w = 1280, h = 960;
    if (!(s = SDL_CreateSurface(w, h, SDL_PIXELFORMAT_XRGB8888)) || !(R = SDL_CreateSoftwareRenderer(s)))
        port_fatal("setup shot: %s\n", SDL_GetError());
    draw(w, h);
    SDL_FlushRenderer(R);
    if (!SDL_SaveBMP(s, path))
        port_fatal("setup shot: %s: %s\n", path, SDL_GetError());
    port_log("setup: wrote %s\n", path);
    SDL_DestroyRenderer(R);
    SDL_DestroySurface(s);
    exit(0);
}

static void ini_path(void) {
    const char *xdg = getenv("XDG_CONFIG_HOME"), *home = getenv("HOME");
    if (xdg && *xdg)
        snprintf(S.ini, sizeof(S.ini), "%s/twine/twine.ini", xdg);
    else
        snprintf(S.ini, sizeof(S.ini), "%s/.config/twine/twine.ini", home ? home : ".");
}

void port_setup(int argc, char **argv) {
    const char *disc = getenv("TWINE_DISC"), *shot_path = getenv("TWINE_SETUP_SHOT");
    int asked = 0, i;
    for (i = 1; i < argc; i++)
        if (!strcmp(argv[i], "--setup"))
            asked = 1;
    if (!disc && exists(DEV_DISC))
        disc = DEV_DISC;
    S.page = PAGE_SETUP;
    snprintf(S.disc_path, sizeof(S.disc_path), "%s", disc ? disc : "");
    disc_check(disc, &S.disc);
    port_log("setup: disc %s: %s\n", disc ? disc : "(none)", S.disc.what);
    if (S.disc.ok)
        setenv("TWINE_DISC", S.disc.bin, 1);
    ini_path();
    S.focus = S.disc.ok ? B_PLAY : B_DISC;
    if (port_video_headless()) {
        if (shot_path && getenv("TWINE_SETUP_SHOT_PAGE") && !strcmp(getenv("TWINE_SETUP_SHOT_PAGE"), "notices")) {
            check_folder("XDG_CONFIG_HOME", ".config", "settings");
            check_folder("XDG_DATA_HOME", ".local/share", "memory card saves, save states and scores");
            return; /* (port_setup_notices takes it) */
        }
        if (shot_path) {
            look_at_bios();
            shot(shot_path);
        }
        if (!S.disc.ok)
            port_fatal("the disc image: %s%s%s\n", S.disc.what, disc ? "\n  " : "", disc ? disc : "");
        return;
    }
    check_folder("XDG_CONFIG_HOME", ".config", "settings");
    check_folder("XDG_DATA_HOME", ".local/share", "memory card saves, save states and scores");
    if (S.disc.ok && !asked)
        return;
    look_at_bios();
    port_log("setup: the setup screen\n");
    run();
    port_log("setup: playing %s\n", getenv("TWINE_DISC"));
}

void port_setup_notices(void) {
    const char *shot_path = getenv("TWINE_SETUP_SHOT");
    S.page = PAGE_NOTICES;
    S.focus = B_OK;
    if (port_video_headless()) {
        if (shot_path && getenv("TWINE_SETUP_SHOT_PAGE") && !strcmp(getenv("TWINE_SETUP_SHOT_PAGE"), "notices"))
            shot(shot_path);
        return;
    }
    if (notice_count)
        run();
}

/* the launcher's word after the game crashed (main.c's --crash-notice: twine.sh runs the program again for it, so
 * the box comes from a sound process) */
void port_crash_notice(const char *log, const char *code) {
    char text[1600];
    snprintf(text, sizeof(text), "TWINE stopped unexpectedly (%s). Sorry about that.\n\nWhat happened is in the log:\n"
                                 "%s\n\nIf you report the problem, please attach that file.", code, log);
    fprintf(stderr, "%s\n", text);
    if (!getenv("TWINE_HEADLESS"))
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "TWINE", text, NULL);
}
