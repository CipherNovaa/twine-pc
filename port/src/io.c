/* Hardware registers for recompiled library code (0x1F801000-0x1F802FFF), written from the psx-spx
 * documentation: DMA (GPU/SPU/OTC channels run at once), the GPU ports (into the software GPU), the MDEC
 * (mdec.c) with its DMA channels 0 and 1, root counters (from host time), interrupt registers, and the SPU
 * register file with its 512 KB sound RAM (the mixer in spu.c reads them). Anything else is stored and read
 * back, and logged once. */
#include <string.h>
#include <time.h>
#include "port.h"
#include "gpu.h"
#include "io.h"
#include "mdec.h"

u16 spu_regs[0x200];       /* 0x1F801C00-0x1F801FFF */
u8 spu_ram[512 * 1024];
u32 spu_xfer_addr;         /* byte address in sound RAM */
static u32 regs[0x800];    /* everything else, by word */
static u8 logged[0x2000 / 16];

static u32 dma_madr[7], dma_bcr[7], dma_chcr[7], dma_dpcr = 0x07654321, dma_dicr;
static u32 i_stat, i_mask;
static u32 timer_mode[3], timer_target[3];
static u64 timer_base[3];
static void *dma_callback[7];
extern int port_subframe; /* an extra drawing between ticks (interp.c) */

/* MDEC transfers (channels 0 and 1) move their data at once but finish later, as on the console, where the
 * MDEC takes a while to decode: MDEC_TIME after they started, they are signalled (CHCR no longer busy, then
 * the DMA callback) at the next vblank, or when the game polls the channel. Movie players start the next
 * frame's work after DecDCTout and expect the callback only later (the shell's clears its frame-done flag
 * right after the call). What a callback starts (the next slice of a frame) is signalled right after it, so
 * a frame decoded slice by slice is done in one go: libcd's stream reader, which leaves a sector for later
 * while DMA1 is busy, sees it busy only briefly. DMA1 can also start before DMA0 has delivered the MDEC's
 * input: it then waits for the decoded words. */
#define MDEC_TIME (33868800 / 4000) /* 0.25 ms in system clock ticks */
static u32 dma_done;               /* channels whose transfer is complete but not yet signalled */
static u64 dma_due[2];             /* ...from this time on (sysclock) */
static int dma_in_callback;
static u32 mdec_out_madr, mdec_out_left; /* DMA1 waiting for decoded words */

/* GPU ports */
static u32 gp0_buf[0x10000];
static int gp0_have, gp0_need;
static u32 gpu_read_x, gpu_read_y, gpu_read_w, gpu_read_h, gpu_read_pos;
static u32 gpustat_disp = 0x00800000;  /* display off until GP1(03) */

static void log_once(u32 off, const char *what, u32 v) {
    if (!logged[off / 16]) {
        logged[off / 16] = 1;
        port_log("io: %s %08X = %08X (first access to this register block)\n", what, 0x1F801000 + off, v);
    }
}

static u64 sysclock(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ((u64)ts.tv_sec * 1000000000ull + ts.tv_nsec) * 33868800ull / 1000000000ull;
}

/* ---- GPU ---- */

static int gp0_length(const u32 *w, int have) {
    u32 cmd = w[0] >> 24;
    if (cmd >= 0x20 && cmd < 0x40) {
        int n = (cmd & 8) ? 4 : 3, per = 1 + ((cmd & 4) ? 1 : 0) + ((cmd & 0x10) ? 1 : 0);
        return 1 + n * per - ((cmd & 0x10) ? 1 : 0);
    }
    if (cmd >= 0x40 && cmd < 0x60) {
        int g = (cmd & 0x10) != 0, i;
        if (!(cmd & 8))
            return g ? 4 : 3;
        for (i = 3; i < have; i++)
            if ((w[i] & 0xF000F000) == 0x50005000 && ((i - 1) % (g ? 2 : 1)) == 0)
                return i + 1;
        return 0x7FFFFFFF;
    }
    if (cmd >= 0x60 && cmd < 0x80)
        return 2 + ((cmd & 4) ? 1 : 0) + (((cmd >> 3) & 3) == 0 ? 1 : 0);
    if (cmd == 0x02 || (cmd >= 0x80 && cmd < 0xA0))
        return cmd == 0x02 ? 3 : 4;
    if (cmd >= 0xA0 && cmd < 0xC0) {
        u32 wd, ht;
        if (have < 3)
            return 3;
        wd = ((w[2] - 1) & 1023) + 1;
        ht = (((w[2] >> 16) - 1) & 511) + 1;
        return 3 + (wd * ht + 1) / 2;
    }
    if (cmd >= 0xC0 && cmd < 0xE0)
        return 3;
    return 1;
}

static void gp0_write(u32 v) {
    gp0_buf[gp0_have++] = v;
    gp0_need = gp0_length(gp0_buf, gp0_have);
    if (gp0_have >= gp0_need || gp0_have >= (int)(sizeof(gp0_buf) / 4)) {
        u32 cmd = gp0_buf[0] >> 24;
        if (cmd >= 0xC0 && cmd < 0xE0) {
            gpu_read_x = gp0_buf[1] & 1023;
            gpu_read_y = (gp0_buf[1] >> 16) & 511;
            gpu_read_w = ((gp0_buf[2] - 1) & 1023) + 1;
            gpu_read_h = (((gp0_buf[2] >> 16) - 1) & 511) + 1;
            gpu_read_pos = 0;
        } else
            gpu_gp0(gp0_buf, gp0_have);
        gp0_have = 0;
    }
}

static u16 gpuread_pixel(void) {
    u32 x = gpu_read_x + gpu_read_pos % gpu_read_w, y = gpu_read_y + gpu_read_pos / gpu_read_w;
    gpu_read_pos++;
    return gpu_vram[(y & 511) * 1024 + (x & 1023)];
}

static void gp1_write(u32 v) {
    switch (v >> 24) {
    case 0x00:
        gp0_have = 0;
        gpustat_disp = 0x00800000;
        break;
    case 0x01:
        gp0_have = 0;
        break;
    case 0x03:
        gpustat_disp = (v & 1) << 23;
        gpu_display.blank = v & 1;
        break;
    case 0x05:
        gpu_display.x = v & 0x3FF;
        gpu_display.y = (v >> 10) & 0x1FF;
        break;
    case 0x08: {
        static const s32 widths[4] = {256, 320, 512, 640};
        gpu_display.w = (v & 0x40) ? 368 : widths[v & 3];
        gpu_display.h = (v & 4) ? 480 : 240;
        gpu_display.rgb24 = (v >> 4) & 1;
        break;
    }
    }
}

/* ---- DMA ---- */

/* An MDEC transfer has moved its data (under RT_ENTER). */
static void mdec_dma_done(int ch) {
    dma_done |= 1u << ch;
    dma_due[ch] = dma_in_callback ? 0 : sysclock() + MDEC_TIME;
}

/* DMA1: copy what the MDEC has decoded; the transfer is done once all its words are out. */
static void mdec_dma_out(void) {
    if (!mdec_out_left)
        return;
    while (mdec_out_left && mdec_out_ready()) {
        M32_RAM(mdec_out_madr) = mdec_read();
        mdec_out_madr += 4;
        mdec_out_left--;
    }
    if (!mdec_out_left)
        mdec_dma_done(1);
}

/* Signal the channel's done MDEC transfer, if there is one and it's due (or forced): CHCR not busy, then
 * the callback; then the ones the callback starts. (Taking one is guarded against the vblank, which signals
 * them too.) */
static void dma_finish(int ch, int force) {
    int n;
    for (n = 0; n < 1024; n++) {
        u32 had;
        RT_ENTER();
        had = dma_done & (1u << ch);
        if (had && !force && dma_due[ch] > sysclock())
            had = 0;
        if (had) {
            dma_done &= ~had;
            dma_chcr[ch] &= ~0x11000000u;
        }
        RT_LEAVE();
        if (!had)
            return;
        if (dma_callback[ch]) {
            dma_in_callback++;
            ((void (*)(void))dma_callback[ch])();
            dma_in_callback--;
        }
        force = 0;
    }
}

/* Every vblank (irq.c): signal the MDEC transfers that are due. */
void port_dma_irq(void) {
    int n;
    for (n = 0; n < 16 && dma_done; n++) {
        u32 before = dma_done;
        dma_finish(0, 0);
        dma_finish(1, 0);
        if (dma_done == before)
            break;
    }
}

static void dma_run(int ch) {
    u32 madr = dma_madr[ch] & 0xFFFFFC, chcr = dma_chcr[ch];
    u32 words = (dma_bcr[ch] & 0xFFFF) * ((dma_bcr[ch] >> 16) ? (dma_bcr[ch] >> 16) : 1), i;
    int to_dev = chcr & 1;
    switch (ch) {
    case 0: /* MDEC in: command parameters (sync mode 1, 20h-word blocks) */
        RT_ENTER();
        for (i = 0; i < words; i++)
            mdec_write(M32_RAM(madr + i * 4));
        mdec_dma_done(0);
        mdec_dma_out(); /* a DMA1 waiting for this input */
        RT_LEAVE();
        return;
    case 1: /* MDEC out */
        RT_ENTER();
        mdec_out_madr = madr;
        mdec_out_left = words;
        mdec_dma_out();
        RT_LEAVE();
        return;
    case 2: /* GPU */
        if (((chcr >> 9) & 3) == 2) {
            u32 a = madr, n = 0;
            for (;;) {
                u32 tag = M32_RAM(a), len = tag >> 24;
                for (i = 0; i < len; i++)
                    gp0_write(M32_RAM(a + 4 + i * 4));
                if ((tag & 0xFFFFFF) == 0xFFFFFF || ++n > 0x100000)
                    break;
                a = tag & 0xFFFFFC;
            }
        } else if (to_dev) {
            for (i = 0; i < words; i++)
                gp0_write(M32_RAM(madr + i * 4));
        } else {
            for (i = 0; i < words; i++) {
                u32 lo = gpuread_pixel(), hi = gpuread_pixel();
                M32_RAM(madr + i * 4) = lo | (hi << 16);
            }
        }
        break;
    case 4: /* SPU */
        if (port_subframe)
            break;
        for (i = 0; i < words; i++) {
            u32 a = (spu_xfer_addr + i * 4) & (sizeof(spu_ram) - 4);
            if (to_dev)
                memcpy(&spu_ram[a], (u8 *)PS1_RAM_BASE + ((madr + i * 4) & 0x1FFFFC), 4);
            else
                memcpy((u8 *)PS1_RAM_BASE + ((madr + i * 4) & 0x1FFFFC), &spu_ram[a], 4);
        }
        spu_xfer_addr = (spu_xfer_addr + words * 4) & (sizeof(spu_ram) - 1);
        break;
    case 6: { /* OTC: reverse-linked empty ordering table */
        u32 n = dma_bcr[ch] & 0xFFFF ? dma_bcr[ch] & 0xFFFF : 0x10000;
        for (i = 0; i < n; i++) {
            u32 a = madr - i * 4;
            M32_RAM(a) = i == n - 1 ? 0xFFFFFF : ((a - 4) & 0xFFFFFF);
        }
        break;
    }
    default:
        port_log("io: DMA channel %d (CHCR %08X) not emulated\n", ch, chcr);
        break;
    }
    dma_chcr[ch] &= ~0x11000000u;
    if (dma_callback[ch])
        ((void (*)(void))dma_callback[ch])();
}

void port_dma_callback(int ch, void *fn) {
    if (ch >= 0 && ch < 7)
        dma_callback[ch] = fn;
}

/* The BIOS boot animation (intro.c) drives the GPU and SPU through these ports before the game starts;
 * afterwards their state goes back to power-on. */
void port_io_reset(void) {
    gp0_have = gp0_need = 0;
    gpu_read_x = gpu_read_y = gpu_read_w = gpu_read_h = gpu_read_pos = 0;
    gpustat_disp = 0x00800000;
    spu_xfer_addr = 0;
}

/* ---- register access ---- */

u32 port_io_read(u32 addr, int size) {
    u32 off = (addr & 0x1FFFFFFF) - 0x1F801000, v;
    if (off >= 0xC00 && off < 0x1000) {
        u32 r = (off - 0xC00) / 2;
        if (off == 0xDAE) /* SPUSTAT: mode bits follow SPUCNT, never busy */
            v = spu_regs[(0xDAA - 0xC00) / 2] & 0x3F;
        else
            v = spu_read(r);
        if (size == 4)
            v |= (u32)spu_read(r + 1) << 16;
        return v;
    }
    if (off >= 0x80 && off < 0xF0) {
        int ch = (off - 0x80) >> 4;
        switch (off & 0xC) {
        case 0:
            return dma_madr[ch];
        case 4:
            return dma_bcr[ch];
        case 8:
            if (ch < 2 && (dma_done >> ch) & 1) /* polling an MDEC transfer: done once it's due */
                dma_finish(ch, 0);
            return dma_chcr[ch];
        }
        return 0;
    }
    switch (off & ~3u) {
    case 0xF0:
        return dma_dpcr;
    case 0xF4:
        return dma_dicr;
    case 0x70:
        return i_stat;
    case 0x74:
        return i_mask;
    case 0x820: /* MDEC0: decoded data */
        RT_ENTER();
        v = mdec_read();
        RT_LEAVE();
        return v;
    case 0x824: /* MDEC1: status */
        RT_ENTER();
        v = mdec_status();
        RT_LEAVE();
        return size == 2 ? (off & 2 ? v >> 16 : v & 0xFFFF) : v;
    case 0x810: /* GPUREAD */
        if (gpu_read_w) {
            u32 lo = gpuread_pixel(), hi = gpuread_pixel();
            return lo | (hi << 16);
        }
        return 0;
    case 0x814: /* GPUSTAT: ready for commands, VRAM reads and DMA */
        return 0x1C000000 | gpustat_disp | ((port_vblank_count() & 1) << 31);
    }
    if (off >= 0x100 && off < 0x130) {
        int n = (off - 0x100) >> 4;
        switch (off & 0xC) {
        case 0: {
            u64 t = sysclock() - timer_base[n];
            u32 div = n == 2 && (timer_mode[n] & 0x200) ? 8 : n == 1 && (timer_mode[n] & 0x100) ? 2152 : 1;
            u32 top = (timer_mode[n] & 8) && timer_target[n] ? timer_target[n] : 0x10000;
            return (u32)((t / div) % top);
        }
        case 4:
            return timer_mode[n];
        case 8:
            return timer_target[n];
        }
    }
    v = regs[off / 4];
    log_once(off, "read", v);
    if (size == 2)
        v = (v >> ((off & 2) * 8)) & 0xFFFF;
    else if (size == 1)
        v = (v >> ((off & 3) * 8)) & 0xFF;
    return v;
}

void port_io_write(u32 addr, u32 v, int size) {
    u32 off = (addr & 0x1FFFFFFF) - 0x1F801000;
    if (off >= 0xC00 && off < 0x1000) {
        if (port_subframe) /* an extra drawing between ticks (interp.c) makes no sound */
            return;
        u32 r = (off - 0xC00) / 2;
        spu_write(r, v & 0xFFFF);
        if (size == 4)
            spu_write(r + 1, v >> 16);
        return;
    }
    if (off >= 0x80 && off < 0xF0) {
        int ch = (off - 0x80) >> 4;
        switch (off & 0xC) {
        case 0:
            dma_madr[ch] = v & 0xFFFFFF;
            break;
        case 4:
            dma_bcr[ch] = v;
            break;
        case 8:
            if (ch < 2) { /* the last MDEC transfer is over (its data has moved), or stopped waiting */
                dma_finish(ch, 1);
                if (ch == 1) {
                    RT_ENTER();
                    mdec_out_left = 0;
                    RT_LEAVE();
                }
            }
            dma_chcr[ch] = v;
            if (v & 0x01000000)
                dma_run(ch);
            break;
        }
        return;
    }
    switch (off & ~3u) {
    case 0xF0:
        dma_dpcr = v;
        return;
    case 0xF4:
        dma_dicr = (dma_dicr & 0xFF000000 & ~v) | (v & 0x00FFFFFF);
        return;
    case 0x70:
        i_stat &= v;
        return;
    case 0x74:
        i_mask = v;
        return;
    case 0x810:
        gp0_write(v);
        return;
    case 0x814:
        gp1_write(v);
        return;
    case 0x820: /* MDEC0: command or parameter */
        RT_ENTER();
        mdec_write(v);
        mdec_dma_out();
        RT_LEAVE();
        return;
    case 0x824: /* MDEC1: control */
        RT_ENTER();
        mdec_control(v);
        RT_LEAVE();
        return;
    }
    if (off >= 0x100 && off < 0x130) {
        int n = (off - 0x100) >> 4;
        switch (off & 0xC) {
        case 0:
            timer_base[n] = sysclock();
            break;
        case 4:
            timer_mode[n] = v;
            timer_base[n] = sysclock();
            break;
        case 8:
            timer_target[n] = v & 0xFFFF;
            break;
        }
        return;
    }
    log_once(off, "write", v);
    if (size == 4)
        regs[off / 4] = v;
    else if (size == 2) {
        u32 sh = (off & 2) * 8;
        regs[off / 4] = (regs[off / 4] & ~(0xFFFFu << sh)) | ((v & 0xFFFF) << sh);
    } else {
        u32 sh = (off & 3) * 8;
        regs[off / 4] = (regs[off / 4] & ~(0xFFu << sh)) | ((v & 0xFF) << sh);
    }
}
