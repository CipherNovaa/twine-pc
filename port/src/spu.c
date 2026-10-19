/* SPU (sound) registers, written from the psx-spx documentation ("Sound Processing Unit (SPU)"). The
 * recompiled libspu/libsnd program these; manual transfers go to sound RAM here, DMA in io.c. Voice
 * playback (ADPCM decoding, ADSR, reverb, mixing to SDL audio) comes later. */
#include "port.h"
#include "io.h"

#define REG_KON 0xC4     /* 0x1F801D88 */
#define REG_KOFF 0xC6    /* 0x1F801D8C */
#define REG_ENDX 0xCE    /* 0x1F801D9C */
#define REG_XADDR 0xD3   /* 0x1F801DA6 */
#define REG_FIFO 0xD4    /* 0x1F801DA8 */

static u32 key_on, key_off, endx;

u16 spu_read(u32 reg) {
    if (reg == REG_ENDX)
        return endx & 0xFFFF;
    if (reg == REG_ENDX + 1)
        return endx >> 16;
    if (reg < 0xC0 && (reg & 7) == 6)  /* voice ADSR volume: silent until voices are mixed */
        return 0;
    return spu_regs[reg & 0x1FF];
}

void spu_write(u32 reg, u16 v) {
    reg &= 0x1FF;
    spu_regs[reg] = v;
    switch (reg) {
    case REG_XADDR:
        spu_xfer_addr = (u32)v * 8;
        break;
    case REG_FIFO:
        spu_ram[spu_xfer_addr & 0x7FFFF] = v & 0xFF;
        spu_ram[(spu_xfer_addr + 1) & 0x7FFFF] = v >> 8;
        spu_xfer_addr = (spu_xfer_addr + 2) & 0x7FFFF;
        break;
    case REG_KON:
        key_on |= v;
        endx &= ~(u32)v;
        break;
    case REG_KON + 1:
        key_on |= (u32)v << 16;
        endx &= ~((u32)v << 16);
        break;
    case REG_KOFF:
        key_off |= v;
        break;
    case REG_KOFF + 1:
        key_off |= (u32)v << 16;
        break;
    }
}
