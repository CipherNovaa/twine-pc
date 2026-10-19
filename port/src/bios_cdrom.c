/* CD-ROM controller (0x1F801800-0x1F801803) for the BIOS boot (bios_boot.c), written from the psx-spx
 * documentation ("CDROM Controller I/O Ports", "CDROM Controller Command Summary", "CDROM - Response
 * Timings"): the index register, the parameter, response and data FIFOs, the interrupt flags INT1-INT5
 * with their enable mask, and the commands the BIOS uses. Responses come after drive-like delays and are
 * queued: a response waits until the previous interrupt has been acknowledged. Sectors are read from the
 * user's disc image (raw 2352-byte sectors, the same file cdrom.c reads); GetID reports a licensed NTSC-U
 * disc ("SCEA"). Not emulated: audio playback (CD-DA, XA-ADPCM) and the sound map.
 * TWINE_CDC_LOG=1 logs commands and responses. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "port.h"
#include "intro.h"

#define RAW 2352
#define NEVER (~0ull)

/* delays in CPU cycles (psx-spx "Response Timings"; seeks are a guess: ~0.3 s across the whole disc) */
#define T_FIRST 0xC4E1       /* first response of most commands */
#define T_FIRST_STOPPED 0x5CF4
#define T_INIT 0x13CCE       /* Init (and ReadTOC) answer late */
#define T_GETID 0x4A00       /* GetID's second response */
#define T_SECTOR_1X 0x6E1CD  /* INT1 rate */
#define T_SECTOR_2X 0x36CD2
#define T_RETRY 0x400        /* a response waiting for the CPU to acknowledge the last one */

static FILE *disc;
static u32 disc_sectors;
static int trace;

static u8 index_reg;
static u8 params[16], nparams;
static u8 resp[16], resp_len, resp_pos;
static u8 ie, iflag;
static u8 stat, mode;
static int busy;           /* a command is waiting for its first response */
static u8 command;

static u32 head, setloc;   /* LBA (sector 0 = 00:02:00) */
static int setloc_pending, reading;
static u8 sector[RAW];
static u8 fifo[RAW];
static u32 fifo_len, fifo_pos;

u64 cdc_next = NEVER;
static u64 first_at = NEVER, second_at = NEVER, sector_at = NEVER;
static u8 second_type, second_data[8], second_len;

static u8 bcd(u32 v) {
    return (u8)(((v / 10) << 4) | (v % 10));
}

static u32 unbcd(u8 v) {
    return (v >> 4) * 10 + (v & 15);
}

static void reschedule(void) {
    cdc_next = first_at;
    if (second_at < cdc_next)
        cdc_next = second_at;
    if (sector_at < cdc_next)
        cdc_next = sector_at;
    bios_boot_schedule(cdc_next);
}

int cdc_open(void) {
    const char *path = getenv("TWINE_DISC");
    if (!path)
        path = "workbench/disc/007 - The World Is Not Enough (USA).bin";
    disc = fopen(path, "rb");
    if (!disc) {
        port_log("intro: no disc image at %s\n", path);
        return 0;
    }
    fseek(disc, 0, SEEK_END);
    disc_sectors = (u32)(ftell(disc) / RAW);
    trace = getenv("TWINE_CDC_LOG") != NULL;
    index_reg = nparams = resp_len = resp_pos = 0;
    ie = iflag = 0;
    stat = 0x02; /* the drive has spun up with the disc */
    mode = 0;
    busy = reading = setloc_pending = 0;
    head = setloc = 0;
    fifo_len = fifo_pos = 0;
    first_at = second_at = sector_at = NEVER;
    reschedule();
    return 1;
}

void cdc_close(void) {
    if (disc)
        fclose(disc);
    disc = NULL;
    first_at = second_at = sector_at = cdc_next = NEVER;
}

static void read_raw(u32 lba, u8 *out) {
    if (lba >= disc_sectors || fseek(disc, (long)lba * RAW, SEEK_SET) || fread(out, 1, RAW, disc) != RAW)
        memset(out, 0, RAW);
}

static u32 le32(const u8 *p) {
    return p[0] | p[1] << 8 | p[2] << 16 | (u32)p[3] << 24;
}

/* ISO 9660: find `name` (up to `len` characters, no version) in the directory at lba/size. */
static int find_entry(u32 *lba, u32 *size, const char *name, int len) {
    u8 raw[RAW];
    u32 off;
    for (off = 0; off < *size; off += 2048) {
        const u8 *d = raw + 24;
        u32 p = 0;
        read_raw(*lba + off / 2048, raw);
        while (p + 33 < 2048 && d[p]) {
            int n = d[p + 32];
            const char *s = (const char *)d + p + 33;
            if ((n == len || (n > len && s[len] == ';')) && !strncasecmp(s, name, len)) {
                *lba = le32(d + p + 2);
                *size = le32(d + p + 10);
                return 1;
            }
            p += d[p];
        }
    }
    return 0;
}

/* Where the BIOS will hand over: the entry point (pc0) of the executable SYSTEM.CNF's BOOT line names. */
u32 cdc_boot_entry(void) {
    u8 raw[RAW];
    const u8 *d = raw + 24;
    u32 root_lba, root_size, lba, size, pc0 = 0;
    char cnf[2049], *p;
    int opened = !disc;
    if (opened && !cdc_open())
        return 0;
    read_raw(16, raw); /* the primary volume descriptor: its root directory record */
    lba = root_lba = le32(d + 156 + 2);
    size = root_size = le32(d + 156 + 10);
    if (find_entry(&lba, &size, "SYSTEM.CNF", 10)) {
        read_raw(lba, raw);
        memcpy(cnf, d, 2048);
        cnf[2048] = 0;
        p = strstr(cnf, "BOOT");
        p = p ? strchr(p, '=') : NULL;
        if (p && (p = strstr(p, "cdrom:"))) {
            lba = root_lba;
            size = root_size;
            for (p += 6; *p == '\\'; p++)
                ;
            for (;;) { /* each path component, up to the ";1" or the end of the line */
                int n = (int)strcspn(p, "\\;\r\n\t ");
                if (!n || !find_entry(&lba, &size, p, n))
                    break;
                if (p[n] != '\\') {
                    read_raw(lba, raw);
                    if (!memcmp(d, "PS-X EXE", 8))
                        pc0 = le32(d + 0x10);
                    break;
                }
                p += n + 1;
            }
        }
    }
    if (opened)
        cdc_close();
    return pc0;
}

/* Put a response in the FIFO and raise its interrupt. */
static void respond(u8 type, const u8 *data, int n) {
    memcpy(resp, data, n);
    resp_len = (u8)n;
    resp_pos = 0;
    iflag = type;
    if (trace) {
        char hex[64] = "";
        int i, k = 0;
        for (i = 0; i < n && k < 60; i++)
            k += snprintf(hex + k, sizeof(hex) - k, " %02X", data[i]);
        port_log("cdc: INT%d%s (cmd %02X) at %llu\n", type, hex, command, (unsigned long long)bios_boot_now);
    }
    if (iflag & ie)
        bios_boot_irq(2);
}

static void respond_stat(u8 type) {
    respond(type, &stat, 1);
}

static void later(u8 type, const u8 *data, int n, u64 delay) {
    memcpy(second_data, data, n);
    second_len = (u8)n;
    second_type = type;
    second_at = bios_boot_now + delay;
}

static void later_stat(u8 type, u64 delay) {
    later(type, &stat, 1, delay);
}

static void error(u8 code) {
    u8 r[2] = {(u8)(stat | 1), code};
    respond(5, r, 2);
}

static u64 seek_time(u32 to) {
    u32 d = to > head ? to - head : head - to;
    return 100000 + (u64)d * 32;
}

static u64 sector_time(void) {
    return (mode & 0x80) ? T_SECTOR_2X : T_SECTOR_1X;
}

/* The command's first response, and what it sets going. */
static void execute(void) {
    u8 r[8];
    switch (command) {
    case 0x01: /* Getstat */
        respond_stat(3);
        stat &= ~0x10; /* "shell was opened" clears once reported */
        break;
    case 0x02: /* Setloc mm ss ff */
        if (nparams < 3) {
            error(0x20);
            break;
        }
        setloc = (unbcd(params[0]) * 60 + unbcd(params[1])) * 75 + unbcd(params[2]);
        setloc = setloc >= 150 ? setloc - 150 : 0;
        setloc_pending = 1;
        respond_stat(3);
        break;
    case 0x06: /* ReadN */
    case 0x1B: { /* ReadS */
        u64 t = 0;
        respond_stat(3);
        if (setloc_pending) {
            t = seek_time(setloc);
            head = setloc;
            setloc_pending = 0;
        }
        reading = 1;
        stat = (stat | 0x02 | 0x20) & ~0x40;
        sector_at = bios_boot_now + t + sector_time();
        break;
    }
    case 0x07: /* MotorOn */
        respond_stat(3);
        stat |= 0x02;
        later_stat(2, T_FIRST);
        break;
    case 0x08: /* Stop */
        respond_stat(3);
        reading = 0;
        sector_at = NEVER;
        later_stat(2, (stat & 0x02) ? ((mode & 0x80) ? 0x18A6076 : 0xD38ACA) : 0x1D7B);
        stat = 0x00;
        second_data[0] = stat;
        break;
    case 0x09: /* Pause */
        respond_stat(3);
        stat &= ~0x60;
        later_stat(2, reading ? ((mode & 0x80) ? 0x10BD93 : 0x21181C) : 0x1DF2);
        reading = 0;
        sector_at = NEVER;
        break;
    case 0x0A: /* Init: mode 20h, motor on, abort everything */
        mode = 0x20;
        reading = 0;
        sector_at = NEVER;
        stat = 0x02;
        respond_stat(3);
        later_stat(2, T_INIT);
        break;
    case 0x0B: /* Mute */
    case 0x0C: /* Demute */
    case 0x0D: /* Setfilter file, channel */
        respond_stat(3);
        break;
    case 0x0E: /* Setmode */
        if (nparams)
            mode = params[0];
        respond_stat(3);
        break;
    case 0x0F: /* Getparam */
        r[0] = stat;
        r[1] = mode;
        r[2] = r[3] = r[4] = 0;
        respond(3, r, 5);
        break;
    case 0x10: /* GetlocL: header and subheader of the last sector read */
        respond(3, sector + 12, 8);
        break;
    case 0x11: { /* GetlocP */
        u32 rel = head, abs = head + 150;
        r[0] = 1;
        r[1] = 1;
        r[2] = bcd(rel / 4500);
        r[3] = bcd((rel / 75) % 60);
        r[4] = bcd(rel % 75);
        r[5] = bcd(abs / 4500);
        r[6] = bcd((abs / 75) % 60);
        r[7] = bcd(abs % 75);
        respond(3, r, 8);
        break;
    }
    case 0x12: /* SetSession */
        respond_stat(3);
        later_stat(2, T_INIT);
        break;
    case 0x13: /* GetTN: one track */
        r[0] = stat;
        r[1] = 1;
        r[2] = 1;
        respond(3, r, 3);
        break;
    case 0x14: { /* GetTD track: 0 = the lead-out */
        u32 t = nparams ? unbcd(params[0]) : 0, at = t == 0 ? disc_sectors + 150 : 150;
        if (t > 1) {
            error(0x10);
            break;
        }
        r[0] = stat;
        r[1] = bcd(at / 4500);
        r[2] = bcd((at / 75) % 60);
        respond(3, r, 3);
        break;
    }
    case 0x15: /* SeekL */
    case 0x16: /* SeekP */
        respond_stat(3);
        reading = 0;
        sector_at = NEVER;
        stat = (stat & ~0x20) | 0x02;
        later_stat(2, seek_time(setloc));
        head = setloc;
        setloc_pending = 0;
        break;
    case 0x19: /* Test */
        switch (nparams ? params[0] : 0xFF) {
        case 0x20: { /* controller BIOS date and version (a PU-8 board's) */
            static const u8 v[4] = {0x95, 0x07, 0x24, 0xC1};
            respond(3, v, 4);
            break;
        }
        case 0x22: /* region */
            respond(3, (const u8 *)"for U/C", 7);
            break;
        default:
            respond_stat(3);
            break;
        }
        break;
    case 0x1A: { /* GetID: a licensed CD-ROM XA disc for America */
        static const u8 id[8] = {0x02, 0x00, 0x20, 0x00, 'S', 'C', 'E', 'A'};
        respond_stat(3);
        later(2, id, 8, T_GETID);
        break;
    }
    case 0x1C: /* Reset */
    case 0x1D: /* GetQ */
        respond_stat(3);
        break;
    case 0x1E: /* ReadTOC */
        respond_stat(3);
        later_stat(2, 0x400000);
        break;
    default:
        error(0x40);
        break;
    }
    nparams = 0;
}

void cdc_events(void) {
    if (bios_boot_now >= first_at) {
        if (iflag)
            first_at = bios_boot_now + T_RETRY;
        else {
            first_at = NEVER;
            busy = 0;
            execute();
        }
    }
    if (bios_boot_now >= second_at) {
        if (iflag)
            second_at = bios_boot_now + T_RETRY;
        else {
            second_at = NEVER;
            respond(second_type, second_data, second_len);
        }
    }
    if (bios_boot_now >= sector_at) {
        if (iflag || busy)
            sector_at = bios_boot_now + T_RETRY;
        else {
            read_raw(head++, sector);
            stat = (stat | 0x22) & ~0x40;
            respond_stat(1);
            sector_at = bios_boot_now + sector_time();
        }
    }
    reschedule();
}

u8 cdc_read(u32 reg) {
    switch (reg) {
    case 0: /* status */
        return index_reg | (nparams == 0 ? 0x08 : 0) | (nparams < 16 ? 0x10 : 0) | (resp_pos < resp_len ? 0x20 : 0) |
               (fifo_pos < fifo_len ? 0x40 : 0) | (busy ? 0x80 : 0);
    case 1: /* response FIFO */
        return resp_pos < resp_len ? resp[resp_pos++] : 0;
    case 2: /* data FIFO */
        return fifo_pos < fifo_len ? fifo[fifo_pos++] : 0;
    default: /* interrupt enable (index 0, 2) or flags (1, 3) */
        return 0xE0 | ((index_reg & 1) ? iflag : ie);
    }
}

void cdc_write(u32 reg, u8 v) {
    if (reg == 0) {
        index_reg = v & 3;
        return;
    }
    switch (reg * 4 + index_reg) {
    case 1 * 4 + 0: /* command */
        command = v;
        busy = 1;
        if (trace)
            port_log("cdc: command %02X (%d params: %02X %02X %02X)\n", v, nparams, params[0], params[1], params[2]);
        first_at = bios_boot_now + (v == 0x0A || v == 0x1E ? T_INIT : (stat & 0x02) ? T_FIRST : T_FIRST_STOPPED);
        reschedule();
        break;
    case 2 * 4 + 0: /* parameter */
        if (nparams < 16)
            params[nparams++] = v;
        break;
    case 2 * 4 + 1: { /* interrupt enable */
        u8 old = ie;
        ie = v & 0x1F;
        if ((iflag & ie) && !(iflag & old))
            bios_boot_irq(2);
        break;
    }
    case 3 * 4 + 0: /* request: BFRD loads the sector buffer into the data FIFO */
        if (v & 0x80) {
            int whole = (mode & 0x20) != 0; /* 924h bytes: all after the sync field; else 800h of data */
            fifo_len = whole ? 2340 : 2048;
            memcpy(fifo, sector + (whole ? 12 : 24), fifo_len);
            fifo_pos = 0;
        } else
            fifo_len = fifo_pos = 0;
        break;
    case 3 * 4 + 1: /* acknowledge; the response FIFO drains */
        iflag &= ~(v & 0x1F);
        if (v & 0x07)
            resp_len = resp_pos = 0;
        if (v & 0x40)
            nparams = 0;
        if (!iflag)
            reschedule();
        break;
    default: /* sound map and CD audio volumes */
        break;
    }
}

u32 cdc_dma_word(void) {
    u32 w = 0;
    int i;
    for (i = 0; i < 4; i++)
        w |= (u32)(fifo_pos < fifo_len ? fifo[fifo_pos++] : 0) << (i * 8);
    return w;
}
