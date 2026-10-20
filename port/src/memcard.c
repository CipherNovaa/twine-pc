/* Memory cards: the BIOS's memory card driver on the host, over card images in the usual raw 128 KB format
 * (psx-spx "Memory Card Data Format", "BIOS Memory Card Functions", "BIOS File Functions"), so cards made here
 * also work in emulators and the other way round.
 *
 * The game saves through PsyQ's libmcrd, which uses the BIOS's file functions on "bu00:" (slot 1) and "bu10:"
 * (slot 2), its raw sector access and _card_info/_card_load, and learns how each request went from the BIOS's
 * memory card events (SwCARD/HwCARD: done, error, no card, new card). Requests complete at once here: the
 * events are delivered before the call returns, as libmcrd clears them before asking.
 *
 * Slot 1's card is $TWINE_CARD1, else card1.mcd in $XDG_DATA_HOME/twine (~/.local/share/twine), made and
 * formatted on first use; slot 2's is $TWINE_CARD2, else card2.mcd there when it exists. "none" leaves a slot
 * empty. Every change is written back to the file at once. */
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "port.h"

#define CARD_SIZE 0x20000
#define FRAME 128
#define BLOCK 0x2000

/* the BIOS's memory card events */
#define SW_CARD 0xF4000001u
#define HW_CARD 0xF0000011u
#define EV_DONE 0x0004
#define EV_ERROR 0x8000
#define EV_NO_CARD 0x0100

/* file open modes */
#define F_CREATE 0x200
#define F_ASYNC 0x8000

void port_deliver_event(u32 cls, u32 spec);

typedef struct {
    int loaded, present;
    char path[1024];
    u8 data[CARD_SIZE];
} Card;

typedef struct {
    int open, slot, first, mode;
    u32 size, pos;
} File;

static Card cards[2];
static File files[16];
static struct {
    int slot, next;
    char pattern[32];
} search;

static void checksum(u8 *frame) {
    u8 x = 0;
    int i;
    for (i = 0; i < FRAME - 1; i++)
        x ^= frame[i];
    frame[FRAME - 1] = x;
}

/* a newly formatted card: the header frame, 15 free directory entries, an empty broken sector list, and the
 * write test frame */
static void format(u8 *d) {
    int i;
    memset(d, 0, CARD_SIZE);
    memset(d + 36 * FRAME, 0xFF, 27 * FRAME);
    d[0] = 'M';
    d[1] = 'C';
    checksum(d);
    for (i = 1; i < 16; i++) {
        u8 *f = d + i * FRAME;
        f[0] = 0xA0;
        f[8] = f[9] = 0xFF;
        checksum(f);
    }
    for (i = 16; i < 36; i++) {
        u8 *f = d + i * FRAME;
        memset(f, 0xFF, 4);
        f[8] = f[9] = 0xFF;
        checksum(f);
    }
    memcpy(d + 63 * FRAME, d, FRAME);
}

static void card_path(int slot, char *out, size_t len) {
    const char *env = getenv(slot ? "TWINE_CARD2" : "TWINE_CARD1"), *xdg = getenv("XDG_DATA_HOME"),
               *home = getenv("HOME");
    if (env) {
        snprintf(out, len, "%s", env);
        return;
    }
    if (xdg && *xdg)
        snprintf(out, len, "%s/twine/card%d.mcd", xdg, slot + 1);
    else
        snprintf(out, len, "%s/.local/share/twine/card%d.mcd", home ? home : ".", slot + 1);
}

/* mkdir -p for the folder of path */
static void make_folder(const char *path) {
    char dir[1024], *p;
    snprintf(dir, sizeof(dir), "%s", path);
    p = strrchr(dir, '/');
    if (!p)
        return;
    *p = 0;
    for (p = dir + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            mkdir(dir, 0755);
            *p = '/';
        }
    }
    mkdir(dir, 0755);
}

static int save(int slot) {
    Card *c = &cards[slot];
    char tmp[1100];
    FILE *f;
    snprintf(tmp, sizeof(tmp), "%s.new", c->path);
    f = fopen(tmp, "wb");
    if (!f || fwrite(c->data, 1, CARD_SIZE, f) != CARD_SIZE || fclose(f) || rename(tmp, c->path)) {
        port_log("memory card: cannot write %s (%s)\n", c->path, strerror(errno));
        return 0;
    }
    return 1;
}

/* the card in a slot (loaded on first use), or NULL for an empty slot */
static Card *card(int slot) {
    Card *c = &cards[slot & 1];
    FILE *f;
    if (c->loaded)
        return c->present ? c : NULL;
    c->loaded = 1;
    card_path(slot & 1, c->path, sizeof(c->path));
    if (!strcmp(c->path, "none"))
        return NULL;
    f = fopen(c->path, "rb");
    if (f) {
        c->present = fread(c->data, 1, CARD_SIZE, f) == CARD_SIZE;
        fclose(f);
        if (!c->present)
            port_log("memory card: %s is not a 128 KB card image; slot %d left empty\n", c->path, slot + 1);
        else
            port_log("memory card: slot %d is %s\n", slot + 1, c->path);
    } else if (!(slot & 1) || getenv("TWINE_CARD2")) {
        format(c->data);
        make_folder(c->path);
        c->present = save(slot & 1);
        if (c->present)
            port_log("memory card: slot %d is a new card, %s\n", slot + 1, c->path);
    }
    return c->present ? c : NULL;
}

static void events(u32 spec) {
    port_deliver_event(SW_CARD, spec);
    port_deliver_event(HW_CARD, spec);
}

/* "bu00:NAME" / "bu10:NAME": the slot (0 or 1), and the file name after the colon; -1 if it isn't a card */
static int device(const char *name, const char **file) {
    if (strncasecmp(name, "bu", 2) || !name[2] || !name[3] || name[4] != ':')
        return -1;
    *file = name + 5;
    return name[2] == '1';
}

static u8 *entry(Card *c, int block) {
    return c->data + block * FRAME;
}

static int names_match(const char *pattern, const char *name) {
    for (; *pattern; pattern++, name++) {
        if (*pattern == '*')
            return 1;
        if (!*name || (*pattern != '?' && toupper((u8)*pattern) != toupper((u8)*name)))
            return 0;
    }
    return !*name;
}

/* the directory block (1-15) where the file starts, or 0 */
static int find(Card *c, const char *name) {
    int b;
    for (b = 1; b < 16; b++) {
        u8 *e = entry(c, b);
        if (e[0] == 0x51 && strlen(name) <= 20 && !strncmp((char *)e + 10, name, 21))
            return b;
    }
    return 0;
}

/* the byte offset in the card of position pos of the file starting at block first */
static u32 offset(Card *c, int first, u32 pos) {
    int b = first, n = pos / BLOCK;
    while (n-- > 0) {
        u8 *e = entry(c, b);
        b = e[8] | e[9] << 8;
        if (b == 0xFFFF)
            return 0;
        b++;
    }
    return b * BLOCK + pos % BLOCK;
}

/* B0:32 open: a file on a card; with F_CREATE, a new file of mode>>16 blocks */
s32 card_open(const char *name, u32 mode) {
    const char *fname;
    int slot = device(name, &fname), fd, first = 0, b, n;
    Card *c;
    if (slot < 0) {
        port_log("BIOS open %s: only memory cards here\n", name);
        return -1;
    }
    c = card(slot);
    if (!c || !*fname || strlen(fname) > 20)
        return -1;
    for (fd = 2; fd < 16 && files[fd].open; fd++)
        ;
    if (fd == 16)
        return -1;
    if (mode & F_CREATE) {
        int blocks[15], i;
        n = mode >> 16 ? (int)(mode >> 16) : 1;
        if (find(c, fname))
            return -1;
        for (b = 1, i = 0; b < 16 && i < n; b++)
            if ((entry(c, b)[0] & 0xF0) == 0xA0) /* free, or a deleted file's */
                blocks[i++] = b;
        if (i < n)
            return -1;
        /* states 51h (first or only), 52h (middle), 53h (last); each points to the next block, less one */
        for (i = 0; i < n; i++) {
            u8 *e = entry(c, blocks[i]);
            u32 next = i < n - 1 ? (u32)blocks[i + 1] - 1 : 0xFFFF;
            memset(e, 0, FRAME);
            e[0] = i == 0 ? 0x51 : i == n - 1 ? 0x53 : 0x52;
            if (i == 0) {
                u32 size = (u32)n * BLOCK;
                e[4] = (u8)size;
                e[5] = (u8)(size >> 8);
                e[6] = (u8)(size >> 16);
                strncpy((char *)e + 10, fname, 20);
            }
            e[8] = (u8)next;
            e[9] = (u8)(next >> 8);
            checksum(e);
        }
        first = blocks[0];
        save(slot);
        port_log("memory card: slot %d: created %s (%d blocks)\n", slot + 1, fname, n);
    } else {
        first = find(c, fname);
        if (!first)
            return -1;
    }
    files[fd].open = 1;
    files[fd].slot = slot;
    files[fd].first = first;
    files[fd].mode = (s32)mode;
    files[fd].size = entry(c, first)[4] | entry(c, first)[5] << 8 | entry(c, first)[6] << 16;
    files[fd].pos = 0;
    return fd;
}

static File *file(s32 fd) {
    return fd >= 2 && fd < 16 && files[fd].open ? &files[fd] : NULL;
}

/* B0:33 lseek: from the start (0) or from where it is (1) */
s32 card_lseek(s32 fd, s32 off, s32 whence) {
    File *f = file(fd);
    if (!f)
        return -1;
    f->pos = whence == 1 ? f->pos + off : (u32)off;
    return (s32)f->pos;
}

/* B0:34 read / B0:35 write, in whole frames. Asynchronous ones (F_ASYNC) return 0, done, and the events say so;
 * others return the bytes moved. */
s32 card_rw(s32 fd, u8 *buf, s32 len, int write) {
    File *f = file(fd);
    Card *c = f ? card(f->slot) : NULL;
    u32 done = 0;
    if (!c || len < 0) {
        if (f && (f->mode & F_ASYNC))
            events(EV_ERROR);
        return -1;
    }
    while (done < (u32)len && f->pos < f->size) {
        u32 at = offset(c, f->first, f->pos), n = BLOCK - f->pos % BLOCK;
        if (!at)
            break;
        if (n > (u32)len - done)
            n = (u32)len - done;
        if (write)
            memcpy(c->data + at, buf + done, n);
        else
            memcpy(buf + done, c->data + at, n);
        done += n;
        f->pos += n;
    }
    if (write)
        save(f->slot);
    if (f->mode & F_ASYNC) {
        events(EV_DONE);
        return 0;
    }
    return (s32)done;
}

/* B0:36 close */
s32 card_close(s32 fd) {
    File *f = file(fd);
    if (!f)
        return -1;
    f->open = 0;
    return fd;
}

/* the next file of the search into the directory entry at dir (PS1's DIRENTRY: name, attribute, size, next,
 * first block), or 0 when there are no more: the BIOS has then read the card's directory, which the events say
 * (libmcrd waits for them to tell no more files from a failed read) */
static u32 next_match(u32 dir) {
    Card *c = card(search.slot);
    u8 *d = (u8 *)dir;
    if (!c) {
        events(EV_NO_CARD);
        return 0;
    }
    while (search.next < 16) {
        u8 *e = entry(c, search.next++);
        if (e[0] != 0x51 || !names_match(search.pattern, (char *)e + 10))
            continue;
        memset(d, 0, 40);
        memcpy(d, e + 10, 20);
        *(u32 *)(d + 0x14) = e[0] & 0xF0;
        *(u32 *)(d + 0x18) = e[4] | e[5] << 8 | e[6] << 16;
        *(u32 *)(d + 0x20) = (u32)(search.next - 1);
        return dir;
    }
    events(EV_DONE);
    return 0;
}

/* B0:42 firstfile, B0:43 nextfile: the files whose names match a pattern (? one character, * the rest) */
u32 card_firstfile(const char *pattern, u32 dir) {
    const char *p;
    int slot = device(pattern, &p);
    if (slot < 0)
        return 0;
    search.slot = slot;
    search.next = 1;
    snprintf(search.pattern, sizeof(search.pattern), "%s", *p ? p : "*");
    return next_match(dir);
}

u32 card_nextfile(u32 dir) {
    return next_match(dir);
}

/* B0:45 erase: 1 if the file was there */
s32 card_erase(const char *name) {
    const char *fname;
    int slot = device(name, &fname), b;
    Card *c = slot < 0 ? NULL : card(slot);
    if (!c || !(b = find(c, fname)))
        return 0;
    for (;;) { /* its blocks become deleted (A1h/A2h/A3h), which are free */
        u8 *e = entry(c, b);
        int next = e[8] | e[9] << 8;
        e[0] = (u8)(0xA0 | (e[0] & 0x0F));
        checksum(e);
        if (next > 14)
            break;
        b = next + 1;
    }
    save(slot);
    port_log("memory card: slot %d: erased %s\n", slot + 1, fname);
    return 1;
}

/* A0:AB _card_info, A0:AC _card_load (port 00h = slot 1, 10h = slot 2): whether there's a card */
s32 card_info(u32 port) {
    events(card(port >> 4 & 1) ? EV_DONE : EV_NO_CARD);
    return 1;
}

/* B0:4E _card_write, B0:4F _card_read: one 128-byte frame */
s32 card_sector(u32 port, u32 sector, u8 *buf, int write) {
    int slot = port >> 4 & 1;
    Card *c = card(slot);
    if (!c) {
        events(EV_NO_CARD);
        return 1;
    }
    if (sector >= CARD_SIZE / FRAME) {
        events(EV_ERROR);
        return 0;
    }
    if (write) {
        memcpy(c->data + sector * FRAME, buf, FRAME);
        save(slot);
    } else
        memcpy(buf, c->data + sector * FRAME, FRAME);
    events(EV_DONE);
    return 1;
}
