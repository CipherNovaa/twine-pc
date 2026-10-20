/* The PS1's own boot animation, from the user's BIOS ROM: a small R3000 interpreter runs the BIOS from
 * reset in the port's PS1 RAM, with the hardware the boot needs around it (interrupt controller, DMA,
 * root counters, the CD-ROM controller in bios_cdrom.c, and controller/memory card ports with nothing
 * plugged in). It draws through the software GPU (gpu.c, via io.c's GPU ports), plays the chime through
 * the SPU (spu.c, via io.c) and runs GTE commands in gte.c. It stops when the BIOS has loaded the disc's
 * boot executable and jumps to its entry point; intro.c then puts the machine back the way the native game
 * expects it. Nothing here touches the game's own interrupt machinery (irq.c): it runs before that starts.
 *
 * Written from the psx-spx documentation ("CPU Specifications", "COP0 - Exception Handling", "Memory Map",
 * "Interrupts", "DMA Channels", "Timers", "Controllers and Memory Cards", "GPU Status Register").
 *
 * Timing is approximate: about one cycle per instruction from cached RAM, more for ROM fetches and RAM
 * loads, a vblank every 33868800 / 59.94 cycles. At each vblank the frame is shown and (with a window) the
 * run waits for real time, so the animation and the chime keep the console's speed; headless, the SPU is
 * run for the frame's samples instead (intro.c). TWINE_BIOS_TTY=1 logs the BIOS's text output. */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "port.h"
#include "gpu.h"
#include "gte_native.h"
#include "io.h"
#include "intro.h"
#include "spu.h"

#define FRAME_CYCLES 565045u /* 33868800 Hz / 59.94 Hz */
#define LINE_CYCLES 2152u    /* 263 lines per NTSC field */
#define VBLANK_LINES 22u
#define GIVE_UP_FRAMES (60 * 60)
#define PLAY_OUT_FRAMES (8 * 60) /* the most the boot sound is given to play out after the handover */
#define QUIET_PEAK 128            /* output this quiet (-48 dB; the chord rings on for some 6 s)... */
#define QUIET_FRAMES 15           /* ...for this many frames in a row is the end of it */

#define RAM ((u8 *)PS1_RAM_BASE)
#define SCRATCH ((u8 *)PS1_SCRATCH_BASE)

enum { C0_BADV = 8, C0_SR = 12, C0_CAUSE = 13, C0_EPC = 14, C0_PRID = 15 };
enum { EX_INT = 0, EX_ADEL = 4, EX_ADES = 5, EX_SYS = 8, EX_BP = 9, EX_RI = 10, EX_OV = 12 };
#define SR_IEC 0x1u
#define SR_ISC 0x10000u /* isolate cache: stores don't reach memory */
#define SR_BEV 0x400000u

u64 bios_boot_now;
static u64 next_event, next_vblank, last_vblank;
static const u8 *rom;
static u32 stop_pc;
static int done, skipped, tty;
static u32 total_frames, paced_frames;

/* ---- CPU state ---- */

static u32 R[32], hi, lo, pc, next_pc, cop0[16], cache_ctrl;
static u32 ld_reg, ld_val, nld_reg, nld_val; /* load delay: last instruction's load, this one's */
static int delay_slot;                      /* the next instruction is a taken branch's delay slot */
static u32 cur_pc;
static int cur_ds;

/* ---- devices ---- */

static u32 i_stat, i_mask;
static u32 dma_madr[7], dma_bcr[7], dma_chcr[7], dma_dpcr, dma_dicr;
static u32 misc[0x2000 / 4]; /* registers that only need to read back (memory control, SIO1, ...) */
static u8 logged[0x40];

typedef struct {
    u32 count, mode, target;
    u64 last;
    int fired;
} Timer;
static Timer timers[3];
static u64 timer_next = ~0ull;

static u8 sio_rx;
static int sio_rx_full;
static u16 sio_mode, sio_ctrl, sio_baud;

static u32 gpu_mode, gpu_dir, gpu_off = 1, gpu_info, gpu_vrange;
static int gpu_info_valid;

static void log_once(int slot, const char *what, u32 addr, u32 v) {
    if (slot >= 0 && slot < (int)sizeof(logged) && !logged[slot]) {
        logged[slot] = 1;
        port_log("bios: %s %08X (%08X) at pc %08X\n", what, addr, v, cur_pc);
    }
}

void bios_boot_schedule(u64 when) {
    if (when < next_event)
        next_event = when;
}

static void irq_update(void) {
    if (i_stat & i_mask)
        cop0[C0_CAUSE] |= 0x400;
    else
        cop0[C0_CAUSE] &= ~0x400u;
}

void bios_boot_irq(int n) {
    i_stat |= 1u << n;
    irq_update();
}

/* ---- root counters ---- */

/* clock of counter n as num/den of the CPU clock */
static void timer_rate(int n, u32 *num, u32 *den) {
    u32 m = timers[n].mode;
    *num = *den = 1;
    if (n == 0 && (m & 0x100)) { /* dot clock: the video clock (CPU * 11/7) over the mode's divider */
        static const u32 div[4] = {10, 8, 5, 4};
        *num = 11;
        *den = 7 * ((gpu_mode & 0x40) ? 7 : div[gpu_mode & 3]);
    } else if (n == 1 && (m & 0x100)) { /* hblank */
        *num = 5;
        *den = 10763;
    } else if (n == 2 && (m & 0x200)) {
        *den = 8;
    }
}

static int timer_paused(int n) {
    /* counter 2's sync modes 0 and 3 stop it */
    return n == 2 && (timers[2].mode & 1) && (((timers[2].mode >> 1) & 3) % 3) == 0;
}

static void timer_irq(int n) {
    Timer *t = &timers[n];
    if (!(t->mode & 0x40) && t->fired) /* one-shot */
        return;
    t->fired = 1;
    if (t->mode & 0x80) { /* toggle bit 10; the IRQ is its falling edge */
        t->mode ^= 0x400;
        if (!(t->mode & 0x400))
            bios_boot_irq(4 + n);
    } else
        bios_boot_irq(4 + n);
}

static void timer_reach(int n, u32 v) {
    Timer *t = &timers[n];
    if (v == t->target) {
        t->mode |= 0x800;
        if (t->mode & 0x10)
            timer_irq(n);
    }
    if (v == 0xFFFF) {
        t->mode |= 0x1000;
        if (t->mode & 0x20)
            timer_irq(n);
    }
}

/* the value counter n wraps to 0 after, from count c */
static u32 timer_wrap(const Timer *t) {
    return (t->mode & 8) && t->count <= t->target ? t->target : 0xFFFF;
}

static void timer_advance(int n) {
    Timer *t = &timers[n];
    u32 num, den;
    u64 ticks;
    timer_rate(n, &num, &den);
    ticks = timer_paused(n) ? 0 : bios_boot_now * num / den - t->last * num / den;
    t->last = bios_boot_now;
    while (ticks) {
        u32 c = t->count, wrap = timer_wrap(t), stop;
        if (c == wrap) {
            t->count = 0;
            ticks--;
            if (t->target == 0)
                timer_reach(n, 0);
            if (ticks > 0x30000) /* whole periods change nothing more */
                ticks = 0x10000 + ticks % ((u64)wrap + 1);
            continue;
        }
        stop = wrap;
        if (t->target > c && t->target < stop)
            stop = t->target;
        if (ticks < stop - c) {
            t->count = c + (u32)ticks;
            break;
        }
        ticks -= stop - c;
        t->count = stop;
        timer_reach(n, stop);
    }
}

/* Bring the counters up to date and schedule the next point where one could raise an interrupt. */
static void timers_sync(void) {
    int n;
    timer_next = ~0ull;
    for (n = 0; n < 3; n++) {
        Timer *t = &timers[n];
        u32 num, den, c, wrap, stop;
        u64 when;
        timer_advance(n);
        if (!(t->mode & 0x30) || timer_paused(n) || (!(t->mode & 0x40) && t->fired))
            continue;
        c = t->count;
        wrap = timer_wrap(t);
        stop = wrap;
        if (t->target > c && t->target < stop)
            stop = t->target;
        timer_rate(n, &num, &den);
        when = bios_boot_now + ((u64)(stop > c ? stop - c : 1) * den + num - 1) / num + 1;
        if (when < timer_next)
            timer_next = when;
    }
    bios_boot_schedule(timer_next);
}

static u32 timer_read(int n, u32 reg) {
    Timer *t = &timers[n];
    timer_advance(n);
    switch (reg) {
    case 0:
        return t->count;
    case 4: {
        u32 m = t->mode;
        t->mode &= ~0x1800u; /* reached-target/FFFF flags clear when read */
        return m;
    }
    case 8:
        return t->target;
    }
    return 0;
}

static void timer_write(int n, u32 reg, u32 v) {
    Timer *t = &timers[n];
    timer_advance(n);
    switch (reg) {
    case 0:
        t->count = v & 0xFFFF;
        break;
    case 4:
        t->mode = (v & 0x3FF) | 0x400;
        t->count = 0;
        t->fired = 0;
        break;
    case 8:
        t->target = v & 0xFFFF;
        break;
    }
    timers_sync();
}

/* ---- GPU (the drawing itself is gpu.c, through io.c's GP0/GP1 ports) ---- */

static void gp0(u32 v) {
    gpu_info_valid = 0;
    port_io_write(0x1F801810, v, 4);
}

static void gp1(u32 v) {
    switch ((v >> 24) & 0x3F) {
    case 0x00:
        gpu_mode = gpu_dir = 0;
        gpu_off = 1;
        break;
    case 0x03:
        gpu_off = v & 1;
        break;
    case 0x04:
        gpu_dir = v & 3;
        break;
    case 0x07:
        gpu_vrange = v & 0xFFFFF;
        break;
    case 0x08:
        gpu_mode = v & 0xFF;
        break;
    default:
        if ((v >> 24) >= 0x10 && (v >> 24) < 0x20) { /* GPU info: only the version is answered */
            gpu_info = (v & 7) == 7 ? 2 : 0;
            gpu_info_valid = 1;
            return;
        }
        break;
    }
    port_io_write(0x1F801814, v, 4);
    { /* the vertical display range shows fewer lines than the mode (the BIOS: 239 per field) */
        s32 lines = (s32)((gpu_vrange >> 10) & 0x3FF) - (s32)(gpu_vrange & 0x3FF);
        if ((gpu_mode & 0x24) == 0x24)
            lines *= 2;
        if (gpu_vrange && lines > 0 && lines < gpu_display.h)
            gpu_display.h = lines;
    }
}

static u32 gpu_status(void) {
    u32 s = 0x1C000000 | (gpu_off << 23) | (gpu_dir << 29);
    u32 line = (u32)((bios_boot_now - last_vblank) / LINE_CYCLES);
    int interlace = (gpu_mode & 0x24) == 0x24;
    s |= ((gpu_mode & 0x3F) << 17) | ((gpu_mode & 0x40) << 10) | ((gpu_mode & 0x80) << 7);
    if (!interlace || (total_frames & 1))
        s |= 0x2000;
    if (gpu_dir == 1 || gpu_dir >= 2)
        s |= 0x02000000;
    /* bit 31: the field in 480-line mode, the line otherwise; 0 in vblank */
    if (line >= VBLANK_LINES && (interlace ? (total_frames & 1) : (line & 1)))
        s |= 0x80000000u;
    return s;
}

/* ---- DMA ---- */

static void dma_irq_update(u32 old) {
    int on = (dma_dicr & 0x8000) || ((dma_dicr & 0x800000) && ((dma_dicr >> 16) & (dma_dicr >> 24) & 0x7F));
    dma_dicr = on ? dma_dicr | 0x80000000u : dma_dicr & 0x7FFFFFFFu;
    if (on && !(old & 0x80000000u))
        bios_boot_irq(3);
}

#define RAM32(a) (*(u32 *)(RAM + ((a) & 0x1FFFFC)))

static void dma_run(int ch) {
    u32 madr = dma_madr[ch] & 0x1FFFFC, chcr = dma_chcr[ch], bcr = dma_bcr[ch], i, old = dma_dicr;
    u32 sync = (chcr >> 9) & 3, words;
    int to_dev = chcr & 1;
    if (sync == 0)
        words = (bcr & 0xFFFF) ? bcr & 0xFFFF : 0x10000;
    else
        words = ((bcr & 0xFFFF) ? bcr & 0xFFFF : 0x10000) * ((bcr >> 16) ? bcr >> 16 : 0x10000);
    switch (ch) {
    case 2: /* GPU */
        if (sync == 2) {
            u32 a = madr, n;
            for (n = 0; n < 0x40000; n++) {
                u32 tag = RAM32(a), len = tag >> 24;
                for (i = 0; i < len; i++)
                    gp0(RAM32(a + 4 + i * 4));
                if (tag & 0x800000)
                    break;
                a = tag & 0x1FFFFC;
            }
            dma_madr[ch] = 0x00FFFFFF;
            words = 0;
        } else if (to_dev) {
            for (i = 0; i < words; i++)
                gp0(RAM32(madr + i * 4));
        } else {
            for (i = 0; i < words; i++)
                RAM32(madr + i * 4) = gpu_info_valid ? gpu_info : port_io_read(0x1F801810, 4);
        }
        break;
    case 3: /* CD-ROM: the data FIFO into RAM */
        for (i = 0; i < words; i++)
            RAM32(madr + i * 4) = cdc_dma_word();
        break;
    case 4: /* SPU: sound RAM at the transfer address */
        for (i = 0; i < words; i++) {
            u32 a = (spu_xfer_addr + i * 4) & (sizeof(spu_ram) - 4);
            if (to_dev)
                memcpy(&spu_ram[a], RAM + ((madr + i * 4) & 0x1FFFFC), 4);
            else
                memcpy(RAM + ((madr + i * 4) & 0x1FFFFC), &spu_ram[a], 4);
        }
        spu_xfer_addr = (spu_xfer_addr + words * 4) & (sizeof(spu_ram) - 1);
        break;
    case 6: /* OTC: an empty ordering table, linked backwards */
        for (i = 0; i < words; i++) {
            u32 a = madr - i * 4;
            RAM32(a) = i == words - 1 ? 0xFFFFFF : ((a - 4) & 0xFFFFFF);
        }
        words = 0;
        break;
    default:
        log_once(0x30 + ch, "DMA channel not emulated", ch, chcr);
        break;
    }
    if (sync == 1) {
        dma_madr[ch] = (madr + words * 4) & 0xFFFFFF;
        dma_bcr[ch] &= 0xFFFF;
    }
    dma_chcr[ch] &= ~0x11000000u;
    if (dma_dicr & (0x10000u << ch))
        dma_dicr |= 0x1000000u << ch;
    dma_irq_update(old);
}

/* ---- controller and memory card port (SIO0): nothing is plugged in ---- */

static u32 sio_read(u32 off, int size) {
    u32 v;
    switch (off) {
    case 0x40:
        v = sio_rx_full ? sio_rx : 0xFF;
        sio_rx_full = 0;
        return size == 1 ? v : v | 0xFFFFFF00u;
    case 0x44: /* TX ready and finished; no /ACK, no IRQ */
        return 0x5 | (sio_rx_full ? 2 : 0);
    case 0x48:
        return size == 4 ? sio_mode | (u32)sio_ctrl << 16 : sio_mode;
    case 0x4A:
        return sio_ctrl;
    case 0x4E:
        return sio_baud;
    }
    return 0;
}

static void sio_write(u32 off, u32 v) {
    switch (off) {
    case 0x40: /* a byte goes out; the open line answers FFh and nobody acknowledges */
        sio_rx = 0xFF;
        sio_rx_full = 1;
        break;
    case 0x48:
        sio_mode = (u16)v;
        break;
    case 0x4A:
        if (v & 0x40) { /* reset */
            sio_rx_full = 0;
            sio_mode = 0;
            v = 0;
        }
        sio_ctrl = (u16)(v & ~0x50u);
        break;
    case 0x4E:
        sio_baud = (u16)v;
        break;
    }
}

/* ---- I/O ports ---- */

static u32 io_read(u32 p, int size) {
    u32 off = p - 0x1F801000, v;
    if (off >= 0xC00 && off < 0x1000)
        return port_io_read(p, size);
    if (off >= 0x800 && off < 0x804) {
        if (off == 0x802 && size > 1) { /* the data FIFO also reads 16 bits at a time */
            v = cdc_read(2);
            v |= (u32)cdc_read(2) << 8;
            if (size == 4) {
                v |= (u32)cdc_read(2) << 16;
                v |= (u32)cdc_read(2) << 24;
            }
            return v;
        }
        return cdc_read(off & 3);
    }
    if (off >= 0x40 && off < 0x50)
        return sio_read(off, size);
    if (off >= 0x100 && off < 0x130) {
        v = timer_read((off - 0x100) >> 4, off & 0xC);
        return size == 4 ? v : v & 0xFFFF;
    }
    switch (off & ~3u) {
    case 0x70:
        v = i_stat;
        break;
    case 0x74:
        v = i_mask;
        break;
    case 0xF0:
        v = dma_dpcr;
        break;
    case 0xF4:
        v = dma_dicr;
        break;
    case 0x810:
        v = gpu_info_valid ? gpu_info : port_io_read(p, 4);
        break;
    case 0x814:
        v = gpu_status();
        break;
    case 0x824: /* MDEC: idle, FIFOs empty */
        v = 0x80040000;
        break;
    default:
        if (off >= 0x80 && off < 0xF0) {
            int ch = (off - 0x80) >> 4;
            v = (off & 0xC) == 0 ? dma_madr[ch] : (off & 0xC) == 4 ? dma_bcr[ch] : dma_chcr[ch];
        } else {
            v = misc[off / 4];
            if (off >= 0x1000) /* expansion 2 */
                log_once(0x20, "expansion 2 read", p, v);
        }
        break;
    }
    v >>= (off & 3) * 8;
    return size == 4 ? v : size == 2 ? v & 0xFFFF : v & 0xFF;
}

static void io_write(u32 p, u32 v, int size) {
    u32 off = p - 0x1F801000, sh = (off & 3) * 8;
    u32 wmask = size == 4 ? 0xFFFFFFFFu : size == 2 ? 0xFFFFu << sh : 0xFFu << sh;
    if (off >= 0xC00 && off < 0x1000) {
        port_io_write(p, v, size);
        return;
    }
    if (off >= 0x800 && off < 0x804) {
        cdc_write(off & 3, (u8)v);
        return;
    }
    if (off >= 0x40 && off < 0x50) {
        sio_write(off, v);
        return;
    }
    if (off >= 0x100 && off < 0x130) {
        timer_write((off - 0x100) >> 4, off & 0xC, v);
        return;
    }
    v = (v << sh) & wmask;
    off &= ~3u;
    switch (off) {
    case 0x70:
        i_stat &= v | ~wmask;
        irq_update();
        return;
    case 0x74:
        i_mask = ((i_mask & ~wmask) | v) & 0x7FF;
        irq_update();
        return;
    case 0xF0:
        dma_dpcr = (dma_dpcr & ~wmask) | v;
        return;
    case 0xF4: { /* bits 24-30 are acknowledged by writing 1 */
        u32 old = dma_dicr, full = (dma_dicr & ~wmask) | v;
        dma_dicr = (dma_dicr & 0x7F000000 & ~(v & 0x7F000000)) | (full & 0x00FF803F);
        dma_irq_update(old);
        return;
    }
    case 0x810:
        gp0(v);
        return;
    case 0x814:
        gp1(v);
        return;
    }
    if (off >= 0x80 && off < 0xF0) {
        int ch = (off - 0x80) >> 4;
        switch (off & 0xC) {
        case 0:
            dma_madr[ch] = ((dma_madr[ch] & ~wmask) | v) & 0xFFFFFF;
            break;
        case 4:
            dma_bcr[ch] = (dma_bcr[ch] & ~wmask) | v;
            break;
        case 8:
            dma_chcr[ch] = (dma_chcr[ch] & ~wmask) | v;
            if (ch == 6)
                dma_chcr[6] = (dma_chcr[6] & 0x51000000) | 2;
            /* start: bit 24, and in manual (sync 0) mode the trigger bit 28 too */
            if ((dma_chcr[ch] & 0x01000000) && (((dma_chcr[ch] >> 9) & 3) || (dma_chcr[ch] & 0x10000000)))
                dma_run(ch);
            break;
        }
        return;
    }
    if (off >= 0x1000 && off != 0x1040) /* expansion 2 (1F802041 is the POST display) */
        log_once(0x21, "expansion 2 write", p, v);
    misc[off / 4] = (misc[off / 4] & ~wmask) | v;
}

/* ---- memory ---- */

static void bad_access(const char *what, u32 a) {
    log_once(0x22 + (what[0] == 's'), what, a, 0);
}

static inline u32 load(u32 a, int size) {
    u32 p;
    const u8 *m;
    if (a >= 0xC0000000u)
        return a == 0xFFFE0130u ? cache_ctrl : 0;
    p = a & 0x1FFFFFFF;
    if (p < 0x00800000u) {
        m = RAM + (p & 0x1FFFFF);
        bios_boot_now += 4;
    } else if (p >= 0x1FC00000u && p < 0x1FC80000u) {
        m = rom + (p & 0x7FFFF);
        bios_boot_now += 20;
    } else if (p >= 0x1F800000u && p < 0x1F800400u) {
        m = SCRATCH + (p & 0x3FF);
    } else if (p >= 0x1F801000u && p < 0x1F803000u) {
        bios_boot_now += 3;
        return io_read(p, size);
    } else if (p >= 0x1F000000u && p < 0x1F800000u) { /* expansion 1: nothing there */
        return size == 4 ? 0xFFFFFFFFu : size == 2 ? 0xFFFF : 0xFF;
    } else {
        bad_access("load from", a);
        return 0;
    }
    return size == 4 ? *(const u32 *)m : size == 2 ? *(const u16 *)m : *m;
}

static inline void store(u32 a, u32 v, int size) {
    u32 p;
    u8 *m;
    if (a >= 0xC0000000u) {
        if (a == 0xFFFE0130u)
            cache_ctrl = v;
        return;
    }
    if (cop0[C0_SR] & SR_ISC)
        return;
    p = a & 0x1FFFFFFF;
    if (p < 0x00800000u)
        m = RAM + (p & 0x1FFFFF);
    else if (p >= 0x1F800000u && p < 0x1F800400u)
        m = SCRATCH + (p & 0x3FF);
    else if (p >= 0x1F801000u && p < 0x1F803000u) {
        io_write(p, v, size);
        return;
    } else {
        if (p < 0x1F000000u || p >= 0x1FC80000u)
            bad_access("store to", a);
        return;
    }
    if (size == 4)
        *(u32 *)m = v;
    else if (size == 2)
        *(u16 *)m = (u16)v;
    else
        *m = (u8)v;
}

static inline u32 fetch(u32 a) {
    u32 p = a & 0x1FFFFFFF;
    if (p < 0x00800000u) {
        bios_boot_now += a >= 0xA0000000u ? 4 : 1; /* KSEG1 is uncached */
        return *(const u32 *)(RAM + (p & 0x1FFFFC));
    }
    if (p >= 0x1FC00000u && p < 0x1FC80000u) {
        bios_boot_now += 20;
        return *(const u32 *)(rom + (p & 0x7FFFC));
    }
    bad_access("fetch from", a);
    return 0;
}

/* ---- CPU ---- */

static void exception(u32 code) {
    u32 sr = cop0[C0_SR];
    cop0[C0_CAUSE] = (cop0[C0_CAUSE] & ~0x8000007Cu) | (code << 2);
    if (cur_ds) {
        cop0[C0_EPC] = cur_pc - 4;
        cop0[C0_CAUSE] |= 0x80000000u;
    } else
        cop0[C0_EPC] = cur_pc;
    cop0[C0_SR] = (sr & ~0x3Fu) | ((sr << 2) & 0x3Fu);
    pc = (sr & SR_BEV) ? 0xBFC00180u : 0x80000080u;
    next_pc = pc + 4;
    delay_slot = 0;
    nld_reg = 0;
}

static void address_error(u32 code, u32 a) {
    cop0[C0_BADV] = a;
    exception(code);
}

static inline void set_reg(u32 r, u32 v) {
    if (r) {
        R[r] = v;
        if (ld_reg == r)
            ld_reg = 0; /* the instruction's write wins over the load in its delay slot */
    }
}

static inline void load_reg(u32 r, u32 v) {
    if (r) {
        nld_reg = r;
        nld_val = v;
    }
}

static inline void branch(u32 target) {
    next_pc = target;
    delay_slot = 1;
}

/* the BIOS's interrupt handler skips a GTE command at EPC, as the hardware has already run it */
static int is_gte_command(u32 a) {
    u32 p = a & 0x1FFFFFFF, op;
    if (p < 0x00800000u)
        op = *(const u32 *)(RAM + (p & 0x1FFFFC));
    else if (p >= 0x1FC00000u && p < 0x1FC80000u)
        op = *(const u32 *)(rom + (p & 0x7FFFC));
    else
        return 0;
    return (op >> 25) == 0x25; /* COP2 with the command bit */
}

/* BIOS text output (A0h:3Ch/3Fh, B0h:3Dh), for TWINE_BIOS_TTY=1 */
static void tty_trace(void) {
    static char line[256];
    static int n;
    u32 fn = R[9];
    int c = -1;
    if ((cur_pc == 0xA0 && fn == 0x3C) || (cur_pc == 0xB0 && fn == 0x3D))
        c = R[4] & 0xFF;
    else if (cur_pc == 0xA0 && fn == 0x3F) { /* printf: the format string */
        u32 a = R[4];
        int i;
        for (i = 0; i < 200; i++) {
            u8 ch = (u8)load(a + i, 1);
            if (!ch)
                break;
            if (ch == '\n' || n >= (int)sizeof(line) - 1) {
                line[n] = 0;
                port_log("bios tty: %s\n", line);
                n = 0;
            } else
                line[n++] = ch;
        }
        return;
    }
    if (c < 0)
        return;
    if (c == '\n' || n >= (int)sizeof(line) - 1) {
        line[n] = 0;
        port_log("bios tty: %s\n", line);
        n = 0;
    } else if (c != '\r')
        line[n++] = (char)c;
}

static void execute(u32 op) {
    u32 rs = (op >> 21) & 31, rt = (op >> 16) & 31, rd = (op >> 11) & 31;
    u32 imm = (u32)(s32)(s16)op, a;
    switch (op >> 26) {
    case 0x00:
        switch (op & 63) {
        case 0x00: set_reg(rd, R[rt] << ((op >> 6) & 31)); break;
        case 0x02: set_reg(rd, R[rt] >> ((op >> 6) & 31)); break;
        case 0x03: set_reg(rd, (u32)((s32)R[rt] >> ((op >> 6) & 31))); break;
        case 0x04: set_reg(rd, R[rt] << (R[rs] & 31)); break;
        case 0x06: set_reg(rd, R[rt] >> (R[rs] & 31)); break;
        case 0x07: set_reg(rd, (u32)((s32)R[rt] >> (R[rs] & 31))); break;
        case 0x08: branch(R[rs]); break;
        case 0x09: {
            u32 t = R[rs];
            set_reg(rd, pc + 4);
            branch(t);
            break;
        }
        case 0x0C: exception(EX_SYS); break;
        case 0x0D: exception(EX_BP); break;
        case 0x10: set_reg(rd, hi); break;
        case 0x11: hi = R[rs]; break;
        case 0x12: set_reg(rd, lo); break;
        case 0x13: lo = R[rs]; break;
        case 0x18: {
            s64 m = (s64)(s32)R[rs] * (s32)R[rt];
            lo = (u32)m;
            hi = (u32)((u64)m >> 32);
            bios_boot_now += 6;
            break;
        }
        case 0x19: {
            u64 m = (u64)R[rs] * R[rt];
            lo = (u32)m;
            hi = (u32)(m >> 32);
            bios_boot_now += 6;
            break;
        }
        case 0x1A: {
            s32 n = (s32)R[rs], d = (s32)R[rt];
            if (d == 0) {
                lo = n >= 0 ? 0xFFFFFFFFu : 1;
                hi = (u32)n;
            } else if ((u32)n == 0x80000000u && d == -1) {
                lo = 0x80000000u;
                hi = 0;
            } else {
                lo = (u32)(n / d);
                hi = (u32)(n % d);
            }
            bios_boot_now += 30;
            break;
        }
        case 0x1B:
            if (R[rt] == 0) {
                lo = 0xFFFFFFFFu;
                hi = R[rs];
            } else {
                lo = R[rs] / R[rt];
                hi = R[rs] % R[rt];
            }
            bios_boot_now += 30;
            break;
        case 0x20: {
            u32 r = R[rs] + R[rt];
            if ((s32)((R[rs] ^ r) & (R[rt] ^ r)) < 0)
                exception(EX_OV);
            else
                set_reg(rd, r);
            break;
        }
        case 0x21: set_reg(rd, R[rs] + R[rt]); break;
        case 0x22: {
            u32 r = R[rs] - R[rt];
            if ((s32)((R[rs] ^ R[rt]) & (R[rs] ^ r)) < 0)
                exception(EX_OV);
            else
                set_reg(rd, r);
            break;
        }
        case 0x23: set_reg(rd, R[rs] - R[rt]); break;
        case 0x24: set_reg(rd, R[rs] & R[rt]); break;
        case 0x25: set_reg(rd, R[rs] | R[rt]); break;
        case 0x26: set_reg(rd, R[rs] ^ R[rt]); break;
        case 0x27: set_reg(rd, ~(R[rs] | R[rt])); break;
        case 0x2A: set_reg(rd, (s32)R[rs] < (s32)R[rt]); break;
        case 0x2B: set_reg(rd, R[rs] < R[rt]); break;
        default: exception(EX_RI); break;
        }
        break;
    case 0x01: { /* BLTZ/BGEZ, and the linking forms when rt bits 4-1 are 1000 */
        s32 v = (s32)R[rs];
        int take = (rt & 1) ? v >= 0 : v < 0;
        if ((rt & 0x1E) == 0x10)
            set_reg(31, pc + 4);
        if (take)
            branch(pc + (imm << 2));
        break;
    }
    case 0x02: branch((pc & 0xF0000000u) | ((op & 0x3FFFFFF) << 2)); break;
    case 0x03:
        set_reg(31, pc + 4);
        branch((pc & 0xF0000000u) | ((op & 0x3FFFFFF) << 2));
        break;
    case 0x04: if (R[rs] == R[rt]) branch(pc + (imm << 2)); break;
    case 0x05: if (R[rs] != R[rt]) branch(pc + (imm << 2)); break;
    case 0x06: if ((s32)R[rs] <= 0) branch(pc + (imm << 2)); break;
    case 0x07: if ((s32)R[rs] > 0) branch(pc + (imm << 2)); break;
    case 0x08: {
        u32 r = R[rs] + imm;
        if ((s32)((R[rs] ^ r) & (imm ^ r)) < 0)
            exception(EX_OV);
        else
            set_reg(rt, r);
        break;
    }
    case 0x09: set_reg(rt, R[rs] + imm); break;
    case 0x0A: set_reg(rt, (s32)R[rs] < (s32)imm); break;
    case 0x0B: set_reg(rt, R[rs] < imm); break;
    case 0x0C: set_reg(rt, R[rs] & (op & 0xFFFF)); break;
    case 0x0D: set_reg(rt, R[rs] | (op & 0xFFFF)); break;
    case 0x0E: set_reg(rt, R[rs] ^ (op & 0xFFFF)); break;
    case 0x0F: set_reg(rt, op << 16); break;
    case 0x10: /* COP0 */
        switch (rs) {
        case 0x00:
            load_reg(rt, cop0[rd & 15]);
            break;
        case 0x04:
            if (rd == C0_CAUSE)
                cop0[C0_CAUSE] = (cop0[C0_CAUSE] & ~0x300u) | (R[rt] & 0x300);
            else if (rd != C0_PRID)
                cop0[rd & 15] = R[rt];
            break;
        case 0x10:
            if ((op & 63) == 0x10) /* RFE */
                cop0[C0_SR] = (cop0[C0_SR] & ~0xFu) | ((cop0[C0_SR] >> 2) & 0xF);
            break;
        }
        break;
    case 0x12: /* COP2 (GTE) */
        if (op & 0x02000000) {
            GTE_cmd(op);
            bios_boot_now += 12;
        } else
            switch (rs) {
            case 0x00: load_reg(rt, GTE_mfc2(rd)); break;
            case 0x02: load_reg(rt, GTE_cfc2(rd)); break;
            case 0x04: GTE_mtc2(rd, R[rt]); break;
            case 0x06: GTE_ctc2(rd, R[rt]); break;
            }
        break;
    case 0x11:
    case 0x13:
        break; /* no COP1/COP3 */
    case 0x20: load_reg(rt, (u32)(s32)(s8)load(R[rs] + imm, 1)); break;
    case 0x21:
        a = R[rs] + imm;
        if (a & 1)
            address_error(EX_ADEL, a);
        else
            load_reg(rt, (u32)(s32)(s16)load(a, 2));
        break;
    case 0x22: { /* LWL: merges with a load still in flight to the same register */
        u32 w, cur, sh;
        a = R[rs] + imm;
        w = load(a & ~3u, 4);
        cur = ld_reg == rt ? ld_val : R[rt];
        sh = (a & 3) * 8;
        load_reg(rt, (cur & (0x00FFFFFFu >> sh)) | (w << (24 - sh)));
        break;
    }
    case 0x23:
        a = R[rs] + imm;
        if (a & 3)
            address_error(EX_ADEL, a);
        else
            load_reg(rt, load(a, 4));
        break;
    case 0x24: load_reg(rt, load(R[rs] + imm, 1)); break;
    case 0x25:
        a = R[rs] + imm;
        if (a & 1)
            address_error(EX_ADEL, a);
        else
            load_reg(rt, load(a, 2));
        break;
    case 0x26: { /* LWR */
        u32 w, cur, sh;
        a = R[rs] + imm;
        w = load(a & ~3u, 4);
        cur = ld_reg == rt ? ld_val : R[rt];
        sh = (a & 3) * 8;
        load_reg(rt, (cur & ~(0xFFFFFFFFu >> sh)) | (w >> sh));
        break;
    }
    case 0x28: store(R[rs] + imm, R[rt], 1); break;
    case 0x29:
        a = R[rs] + imm;
        if (a & 1)
            address_error(EX_ADES, a);
        else
            store(a, R[rt], 2);
        break;
    case 0x2A: { /* SWL */
        u32 w, sh;
        a = R[rs] + imm;
        w = load(a & ~3u, 4);
        sh = (a & 3) * 8;
        store(a & ~3u, (w & ~(0xFFFFFFFFu >> (24 - sh))) | (R[rt] >> (24 - sh)), 4);
        break;
    }
    case 0x2B:
        a = R[rs] + imm;
        if (a & 3)
            address_error(EX_ADES, a);
        else
            store(a, R[rt], 4);
        break;
    case 0x2E: { /* SWR */
        u32 w, sh;
        a = R[rs] + imm;
        w = load(a & ~3u, 4);
        sh = (a & 3) * 8;
        store(a & ~3u, (w & ~(0xFFFFFFFFu << sh)) | (R[rt] << sh), 4);
        break;
    }
    case 0x32: /* LWC2 */
        a = R[rs] + imm;
        if (a & 3)
            address_error(EX_ADEL, a);
        else
            GTE_mtc2(rt, load(a, 4));
        break;
    case 0x3A: /* SWC2 */
        a = R[rs] + imm;
        if (a & 3)
            address_error(EX_ADES, a);
        else
            store(a, GTE_mfc2(rt), 4);
        break;
    default:
        exception(EX_RI);
        break;
    }
}

/* ---- frames ---- */

static struct timespec pace_start;

/* Wait until this frame's real time (59.94 Hz from the first frame). */
static void pace(void) {
    struct timespec now, due;
    u64 due_ns, now_ns, start_ns;
    clock_gettime(CLOCK_MONOTONIC, &now);
    if (paced_frames == 0)
        pace_start = now;
    start_ns = (u64)pace_start.tv_sec * 1000000000ull + pace_start.tv_nsec;
    due_ns = start_ns + (u64)paced_frames * 100000000000ull / 5994;
    now_ns = (u64)now.tv_sec * 1000000000ull + now.tv_nsec;
    paced_frames++;
    if (now_ns > due_ns + 100000000ull) { /* far behind (the host stalled): don't rush to catch up */
        pace_start = now;
        paced_frames = 1;
        return;
    }
    if (due_ns > now_ns) {
        due.tv_sec = (time_t)(due_ns / 1000000000ull);
        due.tv_nsec = (long)(due_ns % 1000000000ull);
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &due, NULL);
    }
}

static u64 samples_done;

static void vblank(void) {
    u32 n = intro_next_frame();
    u64 samples;
    last_vblank = bios_boot_now;
    total_frames++;
    bios_boot_irq(0);
    samples = (u64)total_frames * 4410000 / 5994; /* 44100 Hz at 59.94 frames/s */
    if (port_video_headless())
        intro_audio((int)(samples - samples_done));
    samples_done = samples;
    if (intro_shot_wanted(n)) {
        char path[64];
        snprintf(path, sizeof(path), "intro_%04u.ppm", n);
        port_video_dump_gpu(path);
        port_log("intro frame %u (BIOS): wrote %s (%dx%d at %d,%d)\n", n, path, gpu_display.w, gpu_display.h,
                 gpu_display.x, gpu_display.y);
    }
    if (!port_video_headless()) {
        port_video_show_gpu();
        if (port_video_poll_skip())
            skipped = 1;
        pace();
    }
    if (total_frames >= GIVE_UP_FRAMES) {
        port_log("bios: the boot hasn't reached the game after %u frames; giving up\n", total_frames);
        done = 1;
    }
}

/* The BIOS has handed over, but its sound (the PlayStation logo's chord and its echo) is still playing: on the
 * console it goes on until the game resets the SPU, with the logo up until the game shows a picture. Here the
 * machine is reset for the game as soon as the BIOS is done, so the logo stays up and the sound runs until it
 * has died away. */
static void play_out(void) {
    u32 n, quiet = 0;
    u64 samples;
    spu_take_peak();
    for (n = 0; n < PLAY_OUT_FRAMES && quiet < QUIET_FRAMES && !skipped; n++) {
        total_frames++;
        samples = (u64)total_frames * 4410000 / 5994;
        if (port_video_headless())
            intro_audio((int)(samples - samples_done));
        samples_done = samples;
        if (!port_video_headless()) {
            port_video_show_gpu();
            if (port_video_poll_skip())
                skipped = 1;
            pace();
        }
        quiet = spu_take_peak() < QUIET_PEAK ? quiet + 1 : 0;
    }
    port_log("bios: the boot sound played out in %u more frames\n", n);
}

static void events(void) {
    if (bios_boot_now >= next_vblank) {
        next_vblank += FRAME_CYCLES;
        vblank();
    }
    if (bios_boot_now >= cdc_next)
        cdc_events();
    if (bios_boot_now >= timer_next)
        timers_sync();
    next_event = next_vblank;
    if (cdc_next < next_event)
        next_event = cdc_next;
    if (timer_next < next_event)
        next_event = timer_next;
}

static void reset_state(void) {
    memset(R, 0, sizeof(R));
    hi = lo = 0;
    memset(cop0, 0, sizeof(cop0));
    cop0[C0_SR] = SR_BEV;
    cop0[C0_PRID] = 2;
    cache_ctrl = 0;
    ld_reg = nld_reg = 0;
    delay_slot = 0;
    pc = 0xBFC00000u;
    next_pc = pc + 4;
    i_stat = i_mask = 0;
    memset(dma_madr, 0, sizeof(dma_madr));
    memset(dma_bcr, 0, sizeof(dma_bcr));
    memset(dma_chcr, 0, sizeof(dma_chcr));
    dma_dpcr = 0x07654321;
    dma_dicr = 0;
    memset(misc, 0, sizeof(misc));
    memset(timers, 0, sizeof(timers));
    timers[0].mode = timers[1].mode = timers[2].mode = 0x400;
    timer_next = ~0ull;
    sio_rx_full = 0;
    sio_mode = sio_ctrl = sio_baud = 0;
    gpu_mode = gpu_dir = gpu_vrange = 0;
    gpu_off = 1;
    gpu_info_valid = 0;
    bios_boot_now = 0;
    last_vblank = 0;
    next_vblank = FRAME_CYCLES;
    next_event = next_vblank;
    paced_frames = total_frames = 0;
    samples_done = 0;
    done = skipped = 0;
}

int bios_boot_run(const u8 *rom_image, u32 entry) {
    struct timespec t0, t1;
    u32 shell_seen = 0;
    rom = rom_image;
    stop_pc = entry;
    tty = getenv("TWINE_BIOS_TTY") != NULL;
    reset_state();
    if (!cdc_open())
        return 0;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    while (!done && !skipped) {
        if (bios_boot_now >= next_event) {
            events();
            continue;
        }
        cur_pc = pc;
        cur_ds = delay_slot;
        delay_slot = 0;
        if (cur_pc == stop_pc) {
            done = 2;
            break;
        }
        if (tty && (cur_pc == 0xA0 || cur_pc == 0xB0))
            tty_trace();
        if (cur_pc == 0x80030000u && !shell_seen++)
            port_log("bios: shell started at frame %u\n", total_frames);
        if ((cop0[C0_SR] & SR_IEC) && (cop0[C0_SR] & cop0[C0_CAUSE] & 0xFF00) && !is_gte_command(cur_pc)) {
            if (ld_reg)
                R[ld_reg] = ld_val;
            ld_reg = 0;
            exception(EX_INT);
            continue;
        }
        if (cur_pc & 3) {
            address_error(EX_ADEL, cur_pc);
            continue;
        }
        pc = next_pc;
        next_pc += 4;
        execute(fetch(cur_pc));
        if (ld_reg)
            R[ld_reg] = ld_val;
        ld_reg = nld_reg;
        ld_val = nld_val;
        nld_reg = 0;
    }
    cdc_close();
    clock_gettime(CLOCK_MONOTONIC, &t1);
    port_log("bios: %s after %u frames (%.2f s of console time, %.2f s on the host)\n",
             done == 2 ? "reached the boot executable" : skipped ? "skipped" : "stopped", total_frames,
             (double)bios_boot_now / 33868800.0, (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9);
    if (done == 2)
        play_out();
    return done == 2;
}
