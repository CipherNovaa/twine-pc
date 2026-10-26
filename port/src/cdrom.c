/* libcd on the host: the game's CD commands read sectors straight from the user's disc image (raw
 * 2352-byte sectors, .bin of a .cue), and CdSearchFile walks the disc's ISO 9660 directories. CdRead
 * completes at once.
 *
 * ReadN/ReadS stream like the drive (psx-spx "CDROM - Read Commands", "Data/ADPCM Sector Filtering/Delivery"):
 * after a short seek the read head passes 75 or 150 sectors per second of host time. Each sector it
 * passes is handled from the CD interrupt (port_cd_irq, every vblank): with XA-ADPCM on, real-time audio
 * sectors that pass the file/channel filter go to the drive's decoder (xa.c), which plays them into the
 * SPU's CD input; other sectors are data: they become the current sector (CdGetSector) and are announced to
 * the CdReadyCallback as DataReady, as libcd's interrupt handler does. GetlocP/GetlocL/Getstat report the
 * head, and CdSync returns the last command's result. libcd's movie streaming takes the data sectors through
 * the controller's ports and DMA channel 3 itself (port_cd_reg_read).
 *
 * Reads, pauses and stops are logged; TWINE_CD_LOG=1 logs every command (only every 60th GetlocP) and the
 * SPU's CD volume writes too. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "port.h"
#include "io.h"
#include "xa.h"

typedef struct {
    u8 minute, second, sector, track;
} CdlLOC;

typedef struct {
    CdlLOC pos;
    u32 size;
    char name[16];
} CdlFILE;

enum { CdlNoIntr, CdlDataReady, CdlComplete, CdlAcknowledge, CdlDataEnd, CdlDiskError };

static FILE *disc PORT_NOSAVE;
static u8 cd_mode;
static int motor = 1;
static u32 setloc_lba;   /* Setloc target (CdRead reads from here and moves it on) */
static int setloc_new;   /* a Setloc since the last read or seek: the next read seeks there */
static u8 filter_file, filter_chan;
static int muted;
static u8 last_result[8];  /* what CdSync hands back */
static u8 last_header[8];  /* GetlocL: header and subheader of the last sector read */
static void *ready_callback, *sync_callback, *read_callback, *data_callback;

/* (main.c, port_exe_leave) */
void port_cd_forget(void) {
    ready_callback = sync_callback = read_callback = data_callback = NULL;
}
static int cd_log = -1;

/* the read head: idle at head_lba, or reading from head_lba on (sector k of the run is read by
 * head_t0 + (k + 1) / head_rate seconds; head_t0 is after the seek) */
static int head_reading;
static u32 head_lba, head_rate, head_done; /* head_done: sectors of the run handled so far */
static u64 head_t0;
static u32 head_run; /* counts runs, for a ReadyCallback that starts or stops one */

/* the current data sector, for CdGetSector */
static u8 sector_buf[2340];
static u32 sector_size, sector_pos;
static int sector_ready;

/* the controller's ports, for code that reads sectors itself (port_cd_reg_read) */
static u8 port_index;
static int fifo_loaded; /* Request register bit 7 (BFRD): the data FIFO holds the current sector */
static int data_irq;    /* a DMA3 transfer with its interrupt enabled is over: the CdDataCallback is due */
static void data_done(void);

static const char *disc_path(void) {
    const char *p = getenv("TWINE_DISC");
    return p ? p : "workbench/disc/007 - The World Is Not Enough (USA).bin";
}

static u64 host_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (u64)ts.tv_sec * 1000000000ull + ts.tv_nsec;
}

static int read_raw(u32 lba, u8 *raw) {
    int ok;
    RT_ENTER();
    if (!disc)
        disc = fopen(disc_path(), "rb");
    if (!disc)
        port_fatal("cannot open the disc image %s (set TWINE_DISC)\n", disc_path());
    ok = !fseek(disc, (long)lba * 2352, SEEK_SET) && fread(raw, 1, 2352, disc) == 2352;
    RT_LEAVE();
    return ok;
}

/* Read the 2048 data bytes (or, with mode bit 0x20, the 2340 bytes after the sync field) of a sector. */
static int read_sector(u32 lba, u8 *buf, int whole) {
    u8 raw[2352];
    if (!read_raw(lba, raw))
        return 0;
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

static void msf(u32 i, u8 *p) {
    p[0] = bcd(i / 4500);
    p[1] = bcd((i / 75) % 60);
    p[2] = bcd(i % 75);
}

CdlLOC *CdIntToPos(s32 i, CdlLOC *p) {
    msf((u32)(i + 150), &p->minute);
    return p;
}

s32 CdPosToInt(CdlLOC *p) {
    return (unbcd(p->minute) * 60 + unbcd(p->second)) * 75 + unbcd(p->sector) - 150;
}

static void xa_mix_default(void) {
    static const u8 atv[4] = {0x80, 0, 0x80, 0};
    xa_set_mix(atv);
}

s32 CdInit(void) {
    u8 buf[2048];
    xa_mix_default(); /* CD_initvol's drive volumes: left to left, right to right */
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

/* ---- the read head ---- */

static u32 head_count(u64 now) { /* sectors of the run read by now */
    if (!head_reading || now < head_t0)
        return 0;
    return (u32)((now - head_t0) * head_rate / 1000000000ull);
}

static u32 head_position(u64 now) {
    return head_lba + head_count(now);
}

static u8 status(u64 now) {
    u8 s = motor ? 0x02 : 0;
    if (head_reading)
        s |= now < head_t0 ? 0x40 : 0x20; /* seeking, then reading */
    return s;
}

/* A sector the head has read, at host time t: to the XA decoder, or (when deliver) to the CPU as data. */
static void head_sector(u32 lba, u64 t, int deliver) {
    u8 raw[2352], result[8];
    if (!read_raw(lba, raw))
        return; /* past the end of the image */
    memcpy(last_header, raw + 12, 8);
    if (raw[15] == 2) { /* mode 2: XA subheader (file, channel, submode, coding info) */
        int rt_audio = (raw[18] & 0x44) == 0x44, filtered = cd_mode & 8;
        if ((cd_mode & 0x40) && rt_audio && (!filtered || (raw[16] == filter_file && raw[17] == filter_chan))) {
            xa_sector(raw, t);
            return;
        }
        if (filtered && rt_audio)
            return; /* the filter keeps other channels' audio from the CPU too */
    }
    if (!deliver)
        return;
    sector_size = (cd_mode & 0x20) ? 2340 : 2048;
    memcpy(sector_buf, raw + ((cd_mode & 0x20) ? 12 : 24), sector_size);
    sector_pos = 0;
    sector_ready = 1;
    if (ready_callback) {
        memset(result, 0, sizeof(result));
        result[0] = status(t);
        ((void (*)(u8, u8 *))ready_callback)(CdlDataReady, result);
        data_done();
    }
}

/* Handle the sectors read by now. More than a tenth of a second behind (the program was held up: interrupts
 * held off, or the PC busy, which a console never is), the drive is taken to have waited for it rather than
 * moved on: a movie that loses sectors loses a frame, and its player waits for that frame for good (it seeks
 * back to it, a sector late). */
static void head_advance(u64 now, int deliver) {
    u32 n = head_count(now), run = head_run, most = head_rate / 10 + 1;
    if (n - head_done > most) {
        head_t0 += (u64)(n - head_done - most) * 1000000000ull / head_rate;
        n = head_count(now);
    }
    while (head_done < n && head_reading && head_run == run) {
        u32 k = head_done++;
        head_sector(head_lba + k, head_t0 + (u64)(k + 1) * 1000000000ull / head_rate, deliver);
    }
}

/* End a run (Pause, Stop, seeks, CdRead): what was read so far still reaches the decoder. */
static void head_stop(u64 now) {
    if (!head_reading)
        return;
    head_advance(now, 0);
    head_lba = head_position(now);
    head_reading = 0;
    head_run++;
}

/* ReadN/ReadS: from the Setloc target (after a seek) when there is a new one, else on from the head. A
 * rough seek time, as psx-spx gives none: 20 ms plus 0.05 ms per sector of distance, at most 250 ms. */
static void head_read(u64 now) {
    u32 from, dist, seek_ms = 0;
    if (head_reading && !setloc_new)
        return;
    head_stop(now);
    from = setloc_new ? setloc_lba : head_lba;
    dist = from > head_lba ? from - head_lba : head_lba - from;
    if (dist) {
        seek_ms = 20 + dist / 20;
        if (seek_ms > 250)
            seek_ms = 250;
    }
    if (setloc_new)
        xa_flush();
    setloc_new = 0;
    head_reading = 1;
    head_lba = from;
    head_rate = (cd_mode & 0x80) ? 150 : 75;
    head_t0 = now + (u64)seek_ms * 1000000;
    head_done = 0;
    head_run++;
}

/* ---- commands ---- */

static const char *const com_names[0x20] = {
    "Sync", "Getstat", "Setloc", "Play", "Forward", "Backward", "ReadN", "Standby", "Stop", "Pause", "Init",
    "Mute", "Demute", "Setfilter", "Setmode", "Getparam", "GetlocL", "GetlocP", "SetSession", "GetTN",
    "GetTD", "SeekL", "SeekP", NULL, NULL, "Test", "GetID", "ReadS", "Reset", "GetQ", "ReadTOC", NULL,
};

static void log_command(u8 com, const u8 *param, const u8 *res, int len, u64 now) {
    static u32 getloc_calls;
    char text[64];
    int i, o = 0, np = com == 0x02 ? 3 : com == 0x0D ? 2 : com == 0x0E ? 1 : 0;
    if (com == 0x11 && getloc_calls++ % 60)
        return;
    for (i = 0; param && i < np; i++)
        o += snprintf(text + o, sizeof(text) - o, " %02X", param[i]);
    o += snprintf(text + o, sizeof(text) - o, " ->");
    for (i = 0; i < len; i++)
        o += snprintf(text + o, sizeof(text) - o, " %02X", res[i]);
    port_log("cd: %s%s (head %u%s)\n", com < 0x20 && com_names[com] ? com_names[com] : "?", text,
             head_position(now), head_reading ? (now < head_t0 ? ", seeking" : ", reading") : "");
}

/* One command: its effect, and its response bytes into last_result (and result). */
static s32 command(u8 com, const u8 *param, u8 *result) {
    u64 now;
    u8 res[8];
    int len = 1;
    RT_ENTER();
    now = host_ns();
    if (cd_log < 0)
        cd_log = getenv("TWINE_CD_LOG") != NULL;
    /* the position commands take a Setloc parameter first, as libcd sends it */
    if (param && (com == 0x02 || com == 0x03 || com == 0x06 || com == 0x15 || com == 0x16 || com == 0x1B)) {
        setloc_lba = CdPosToInt((CdlLOC *)param);
        setloc_new = 1;
    }
    switch (com) {
    case 0x03: /* Play: CD-DA, which this disc doesn't have */
        port_log("cd: Play (CD-DA) not emulated\n");
        break;
    case 0x06: /* ReadN */
    case 0x1B: /* ReadS */
        head_read(now);
        motor = 1;
        port_log("cd: %s from %u, mode %02X, filter %u/%u%s\n", com == 6 ? "ReadN" : "ReadS", head_lba, cd_mode,
                 filter_file, filter_chan, now < head_t0 ? " (seeking)" : "");
        break;
    case 0x07: /* Standby (MotorOn) */
        motor = 1;
        break;
    case 0x08: /* Stop */
    case 0x09: /* Pause */
        if (head_reading)
            port_log("cd: %s at %u\n", com == 8 ? "Stop" : "Pause", head_position(now));
        head_stop(now);
        if (com == 0x08)
            motor = 0;
        break;
    case 0x0A: /* Init */
        head_stop(now);
        cd_mode = 0x20;
        motor = 1;
        muted = 0;
        xa_set_mute(0);
        break;
    case 0x0B: /* Mute */
    case 0x0C: /* Demute */
        muted = com == 0x0B;
        xa_set_mute(muted);
        break;
    case 0x0D: /* Setfilter */
        if (param) {
            filter_file = param[0];
            filter_chan = param[1];
        }
        break;
    case 0x0E: /* Setmode: a new speed takes effect at once (a running read goes on from where it is) */
        if (param) {
            if (head_reading && ((param[0] ^ cd_mode) & 0x80) && now >= head_t0) {
                head_advance(now, 0);
                head_lba = head_position(now);
                head_t0 = now;
                head_done = 0;
                head_rate = (param[0] & 0x80) ? 150 : 75;
                head_run++;
            }
            if (head_reading && ((param[0] ^ cd_mode) & 0x40))
                port_log("cd: Setmode %02X while reading at %u: XA-ADPCM %s\n", param[0], head_position(now),
                         (param[0] & 0x40) ? "on" : "off");
            cd_mode = param[0];
        }
        break;
    case 0x0F: /* Getparam */
        res[1] = cd_mode;
        res[2] = 0;
        res[3] = filter_file;
        res[4] = filter_chan;
        len = 5;
        break;
    case 0x10: /* GetlocL: header and subheader of the last sector read */
        memcpy(res, last_header, 8);
        len = 8;
        break;
    case 0x11: /* GetlocP: track 1, index 1, position in the track (from 00:02:00) and on the disc */
        res[0] = 0x01;
        res[1] = 0x01;
        msf(head_position(now), res + 2);
        msf(head_position(now) + 150, res + 5);
        len = 8;
        break;
    case 0x13: /* GetTN: one track */
        res[1] = res[2] = 0x01;
        len = 3;
        break;
    case 0x14: /* GetTD: track 1 starts at 00:02 */
        res[1] = 0x00;
        res[2] = 0x02;
        len = 3;
        break;
    case 0x15: /* SeekL */
    case 0x16: /* SeekP */
        head_stop(now);
        if (setloc_new) {
            head_lba = setloc_lba;
            xa_flush();
        }
        setloc_new = 0;
        motor = 1;
        break;
    default:
        break;
    }
    if (com != 0x10 && com != 0x11)
        res[0] = status(now);
    memcpy(last_result, res, len);
    if (result)
        memcpy(result, res, len);
    if (cd_log)
        log_command(com, param, res, len, now);
    RT_LEAVE();
    return 1;
}

/* CdControl / CdControlB / CdControlF: the commands complete at once (a read's seek and its sectors follow
 * on host time). */
s32 CdControl(u8 com, u8 *param, u8 *result) {
    return command(com, param, result);
}

s32 CdControlB(u8 com, u8 *param, u8 *result) {
    return command(com, param, result);
}

s32 CdControlF(u8 com, u8 *param) {
    return command(com, param, NULL);
}

s32 CdMix(u8 *atv) {
    RT_ENTER();
    xa_set_mix(atv);
    RT_LEAVE();
    return 1;
}

static volatile int read_done; /* a CdRead finished: its callback is due */

/* CdRead: libcd sets the mode and reads from the Setloc position; here at once, ending any streaming. The
 * vblank may come between sectors, as before streaming existed (a long read must not hold it off). */
s32 CdRead(s32 sectors, u32 *buf, s32 mode) {
    int whole = (mode & 0x20) != 0;
    u8 *p = (u8 *)buf;
    s32 i;
    RT_ENTER();
    head_stop(host_ns());
    cd_mode = (u8)mode;
    RT_LEAVE();
    for (i = 0; i < sectors; i++) {
        if (!read_sector(setloc_lba + i, p, whole))
            return 0;
        p += whole ? 2340 : 2048;
    }
    RT_ENTER();
    setloc_lba += sectors;
    setloc_new = 1;
    head_lba = setloc_lba;
    motor = 1;
    RT_LEAVE();
    read_done = 1;
    return 1;
}

s32 CdReadSync(s32 mode, u8 *result) {
    (void)mode;
    if (result)
        result[0] = status(host_ns());
    return 0;
}

s32 CdSync(s32 mode, u8 *result) {
    (void)mode;
    if (result)
        memcpy(result, last_result, 8);
    return CdlComplete;
}

/* CdReady: whether a data sector has come in since the last call (mode 0 doesn't wait here). */
s32 CdReady(s32 mode, u8 *result) {
    s32 r = sector_ready ? CdlDataReady : CdlNoIntr;
    (void)mode;
    sector_ready = 0;
    if (result)
        result[0] = status(host_ns());
    return r;
}

/* CdGetSector: the next `words` 32-bit words of the current data sector (DMA from the sector buffer). */
s32 CdGetSector(void *madr, s32 words) {
    u32 n = (u32)words * 4;
    if (sector_pos + n > sector_size)
        n = sector_pos < sector_size ? sector_size - sector_pos : 0;
    memcpy(madr, sector_buf + sector_pos, n);
    sector_pos += n;
    return 1;
}

/* CdGetSector2: the same without waiting; the transfer is done at once, so its CdDataCallback runs now. */
s32 CdGetSector2(void *madr, s32 words) {
    CdGetSector(madr, words);
    if (data_callback)
        ((void (*)(void))data_callback)();
    return 1;
}

s32 CdDataSync(s32 mode) {
    (void)mode;
    return 0;
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

/* ---- the controller's ports (psx-spx "CDROM Controller I/O Ports") ----
 * libcd's stream reader (StCdInterrupt, run as the CdReadyCallback) takes each sector itself: it selects
 * port index 0, asks for the sector (Request register, BFRD), reads the header bytes from the data FIFO and
 * moves the rest with DMA channel 3 (io.c). With DMA3's interrupt enabled, libcd's DMA handler calls the
 * CdDataCallback; on the console that interrupt waits until the CD interrupt in progress is over, so here it
 * comes after the ReadyCallback returns. Commands and interrupts go through libcd on the host instead. */
u32 port_cd_reg_read(u32 reg) {
    int data = fifo_loaded && sector_pos < sector_size;
    switch (reg) {
    case 0: /* status: index, parameter FIFO empty and not full, data FIFO not empty */
        return port_index | 0x18 | (data ? 0x40 : 0);
    case 2: /* data FIFO */
        return data ? sector_buf[sector_pos++] : 0;
    }
    return 0;
}

void port_cd_reg_write(u32 reg, u32 v) {
    if (reg == 0)
        port_index = v & 3;
    else if (reg == 3 && port_index == 0) { /* Request: BFRD loads the sector into the data FIFO, 0 empties it */
        fifo_loaded = (v & 0x80) != 0;
        sector_pos = 0;
    }
}

/* DMA channel 3: `words` words from the data FIFO to RAM */
void port_cd_dma(u32 madr, u32 words, int irq) {
    CdGetSector((u8 *)PS1_RAM_BASE + (madr & PS1_RAM_MASK), (s32)words);
    if (irq)
        data_irq = 1;
}

static void data_done(void) {
    if (data_irq) {
        data_irq = 0;
        if (data_callback)
            ((void (*)(void))data_callback)();
    }
}

/* The CD interrupt (run with the vblank): a read issued with CdRead reports completion (CdlComplete) to the
 * CdReadCallback function, like libcd's interrupt handler does when the last sector has arrived. The game's
 * asynchronous loads (weapon models) wait for this. Then the sectors a streaming read has passed since the
 * last vblank are handled, and the XA decoder plays what is due. */
void port_cd_irq(void) {
    static u8 result[8];
    u64 now;
    data_done();
    if (read_done) {
        read_done = 0;
        result[0] = status(host_ns());
        if (read_callback)
            ((void (*)(u8, u8 *))read_callback)(CdlComplete, result);
    }
    if (!head_reading && !xa_playing())
        return;
    now = host_ns();
    if (head_reading)
        head_advance(now, 1);
    xa_run(now);
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

/* A file of the disc read straight from the image, past the drive (whose reads it leaves alone): for the runtime's
 * and mods' own use. name as "L08/DATA08.BIN"; returns its size and a malloc()ed copy, or 0 if it isn't there. */
static int disc_find(const char *name, u32 *at, u32 *bytes);

u32 port_disc_file(const char *name, u8 **data) {
    return port_disc_part(name, 0, 0xFFFFFFFFu, data);
}

/* len bytes of a disc file from offset (fewer at its end), malloc'd: how many (0: none) */
u32 port_disc_part(const char *name, u32 offset, u32 len, u8 **data) {
    u8 raw[2352];
    u32 lba, size, i, first, n;
    if (!disc_find(name, &lba, &size) || offset >= size)
        return 0;
    if (len > size - offset)
        len = size - offset;
    first = offset / 2048;
    n = (offset + len + 2047) / 2048 - first;
    if (!(*data = malloc(n * 2048 + 2048)))
        return 0;
    for (i = 0; i < n; i++) {
        if (!read_raw(lba + first + i, raw)) {
            free(*data);
            return 0;
        }
        memcpy(*data + i * 2048, raw + 24, 2048);
    }
    if (offset % 2048)
        memmove(*data, *data + offset % 2048, len);
    return len;
}

/* a file's first sector and size, from the disc's directories */
static int disc_find(const char *name, u32 *at, u32 *bytes) {
    u8 raw[2352], *rec;
    u32 lba, size, i, off;
    const char *part = name, *end;
    if (!read_raw(16, raw))
        return 0;
    rec = raw + 24 + 156; /* the volume descriptor's root directory record */
    lba = rec[2] | rec[3] << 8 | rec[4] << 16 | (u32)rec[5] << 24;
    size = rec[10] | rec[11] << 8 | rec[12] << 16 | (u32)rec[13] << 24;
    for (;;) {
        size_t len;
        int found = 0;
        end = strchr(part, '/');
        len = end ? (size_t)(end - part) : strlen(part);
        for (i = 0; i < (size + 2047) / 2048 && !found; i++) {
            if (!read_raw(lba + i, raw))
                return 0;
            for (off = 0; off < 2048 && raw[24 + off]; off += raw[24 + off]) {
                rec = raw + 24 + off;
                if (rec[32] >= len && !strncasecmp((char *)rec + 33, part, len) &&
                    (rec[32] == len || rec[33 + len] == ';')) {
                    lba = rec[2] | rec[3] << 8 | rec[4] << 16 | (u32)rec[5] << 24;
                    size = rec[10] | rec[11] << 8 | rec[12] << 16 | (u32)rec[13] << 24;
                    found = 1;
                    break;
                }
            }
        }
        if (!found)
            return 0;
        if (!end)
            break;
        part = end + 1;
    }
    *at = lba, *bytes = size;
    return 1;
}
