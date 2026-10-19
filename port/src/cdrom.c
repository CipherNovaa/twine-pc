/* libcd on the host: the game's CD commands read sectors straight from the user's disc image (raw
 * 2352-byte sectors, .bin of a .cue), and CdSearchFile walks the disc's ISO 9660 directories. Reads
 * complete immediately. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "port.h"

typedef struct {
    u8 minute, second, sector, track;
} CdlLOC;

typedef struct {
    CdlLOC pos;
    u32 size;
    char name[16];
} CdlFILE;

static FILE *disc;
static u32 cd_lba;  /* current read position (sector index from the start of the data track) */
static u8 cd_mode;
static u8 cd_status = 0x02; /* motor on */
static void *ready_callback, *sync_callback, *read_callback, *data_callback;

static const char *disc_path(void) {
    const char *p = getenv("TWINE_DISC");
    return p ? p : "workbench/disc/007 - The World Is Not Enough (USA).bin";
}

/* Read the 2048 data bytes (or, with mode bit 0x20, the 2340 bytes after the sync field) of a sector. */
static int read_sector(u32 lba, u8 *buf, int whole) {
    u8 raw[2352];
    RT_ENTER();
    if (!disc)
        disc = fopen(disc_path(), "rb");
    if (!disc)
        port_fatal("cannot open the disc image %s (set TWINE_DISC)\n", disc_path());
    if (fseek(disc, (long)lba * 2352, SEEK_SET) || fread(raw, 1, sizeof(raw), disc) != sizeof(raw)) {
        RT_LEAVE();
        return 0;
    }
    RT_LEAVE();
    if (whole)
        memcpy(buf, raw + 12, 2340);
    else
        memcpy(buf, raw + 24, 2048);
    return 1;
}

static u8 bcd(u32 v) {
    return (u8)(((v / 10) << 4) | (v % 10));
}

static u32 unbcd(u8 v) {
    return (v >> 4) * 10 + (v & 15);
}

CdlLOC *CdIntToPos(s32 i, CdlLOC *p) {
    i += 150;
    p->minute = bcd(i / 4500);
    p->second = bcd((i / 75) % 60);
    p->sector = bcd(i % 75);
    return p;
}

s32 CdPosToInt(CdlLOC *p) {
    return (unbcd(p->minute) * 60 + unbcd(p->second)) * 75 + unbcd(p->sector) - 150;
}

s32 CdInit(void) {
    u8 buf[2048];
    return read_sector(16, buf, 0);
}

s32 CdSetDebug(s32 level) {
    (void)level;
    return 0;
}

/* Directory search: "\\DIR\\FILE.EXT;1" */
static int find_in_dir(u32 lba, u32 size, const char *name, int namelen, u32 *out_lba, u32 *out_size) {
    u8 buf[2048];
    u32 off;
    for (off = 0; off < size; off += 2048) {
        u32 p = 0;
        if (!read_sector(lba + off / 2048, buf, 0))
            return 0;
        while (p < 2048 && buf[p]) {
            u8 len = buf[p], nl = buf[p + 32];
            const char *n = (const char *)&buf[p + 33];
            if (nl == namelen && !strncasecmp(n, name, namelen)) {
                *out_lba = buf[p + 2] | (buf[p + 3] << 8) | (buf[p + 4] << 16) | ((u32)buf[p + 5] << 24);
                *out_size = buf[p + 10] | (buf[p + 11] << 8) | (buf[p + 12] << 16) | ((u32)buf[p + 13] << 24);
                return 1;
            }
            p += len;
        }
    }
    return 0;
}

CdlFILE *CdSearchFile(CdlFILE *fp, char *name) {
    u8 pvd[2048];
    u32 lba, size;
    const char *s = name, *e;
    if (!read_sector(16, pvd, 0))
        return NULL;
    lba = pvd[156 + 2] | (pvd[156 + 3] << 8) | (pvd[156 + 4] << 16) | ((u32)pvd[156 + 5] << 24);
    size = pvd[156 + 10] | (pvd[156 + 11] << 8) | (pvd[156 + 12] << 16) | ((u32)pvd[156 + 13] << 24);
    while (*s == '\\')
        s++;
    for (;;) {
        e = strchr(s, '\\');
        if (!e) {
            if (!find_in_dir(lba, size, s, (int)strlen(s), &lba, &size))
                break;
            CdIntToPos(lba, &fp->pos);
            fp->size = size;
            memset(fp->name, 0, sizeof(fp->name));
            strncpy(fp->name, s, sizeof(fp->name) - 1);
            port_log("CdSearchFile %s -> sector %u, %u bytes\n", name, lba, size);
            return fp;
        }
        if (!find_in_dir(lba, size, s, (int)(e - s), &lba, &size))
            break;
        s = e + 1;
    }
    port_log("CdSearchFile %s: not found\n", name);
    return NULL;
}

/* CdControl / CdControlB / CdControlF: Setloc, Setmode; reads and seeks complete at once. */
s32 CdControl(u8 com, u8 *param, u8 *result) {
    switch (com) {
    case 0x02: /* CdlSetloc */
        if (param)
            cd_lba = CdPosToInt((CdlLOC *)param);
        break;
    case 0x0E: /* CdlSetmode */
        if (param)
            cd_mode = param[0];
        break;
    case 0x15: /* CdlSeekL */
    case 0x16: /* CdlSeekP */
        if (param)
            cd_lba = CdPosToInt((CdlLOC *)param);
        break;
    case 0x06: /* CdlReadN */
    case 0x1B: /* CdlReadS */
        if (param)
            cd_lba = CdPosToInt((CdlLOC *)param);
        port_log("CdControl read %02X at %u (streaming not implemented)\n", com, cd_lba);
        break;
    default:
        break;
    }
    if (result)
        result[0] = cd_status;
    return 1;
}

s32 CdControlB(u8 com, u8 *param, u8 *result) {
    return CdControl(com, param, result);
}

s32 CdControlF(u8 com, u8 *param) {
    return CdControl(com, param, NULL);
}

static volatile int read_done; /* a CdRead finished: its callback is due */

s32 CdRead(s32 sectors, u32 *buf, s32 mode) {
    int whole = (mode & 0x20) != 0;
    u8 *p = (u8 *)buf;
    s32 i;
    for (i = 0; i < sectors; i++) {
        if (!read_sector(cd_lba + i, p, whole))
            return 0;
        p += whole ? 2340 : 2048;
    }
    cd_lba += sectors;
    read_done = 1;
    return 1;
}

s32 CdReadSync(s32 mode, u8 *result) {
    (void)mode;
    if (result)
        result[0] = cd_status;
    return 0;
}

s32 CdSync(s32 mode, u8 *result) {
    (void)mode;
    if (result)
        result[0] = cd_status;
    return 2; /* CdlComplete */
}

void *CdReadyCallback(void *fn) {
    void *old = ready_callback;
    ready_callback = fn;
    return old;
}

void *CdSyncCallback(void *fn) {
    void *old = sync_callback;
    sync_callback = fn;
    return old;
}

void *CdReadCallback(void *fn) {
    void *old = read_callback;
    read_callback = fn;
    return old;
}

void *CdDataCallback(void *fn) {
    void *old = data_callback;
    data_callback = fn;
    return old;
}

/* The CD interrupt (run with the vblank): a read issued with CdRead reports completion (CdlComplete) to the
 * CdReadCallback function, like libcd's interrupt handler does when the last sector has arrived. The game's
 * asynchronous loads (weapon models) wait for this. */
void port_cd_irq(void) {
    static u8 result[8];
    if (!read_done)
        return;
    read_done = 0;
    result[0] = cd_status;
    if (read_callback)
        ((void (*)(u8, u8 *))read_callback)(2, result);
}

/* PC file server (development builds only; the retail game reads the CD) */
s32 PCopen(char *name, s32 flags, s32 perms) {
    port_log("PCopen %s\n", name);
    return -1;
}

s32 PCread(s32 fd, void *buf, s32 len) {
    return -1;
}

s32 PClseek(s32 fd, s32 offset, s32 mode) {
    return -1;
}

s32 PCcreat(char *name, s32 perms) {
    return -1;
}

s32 PCclose(s32 fd) {
    return -1;
}
