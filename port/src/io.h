#ifndef IO_H
#define IO_H

/* Hardware register model (io.c) and the SPU (spu.c) */
#define M32_RAM(a) (*(u32 *)(PS1_RAM_BASE + ((a) & PS1_RAM_MASK)))

extern u16 spu_regs[0x200];
extern u8 spu_ram[512 * 1024];
extern u32 spu_xfer_addr;
u16 spu_read(u32 reg);           /* reg = halfword index from 0x1F801C00 */
void spu_write(u32 reg, u16 v);
void port_dma_callback(int ch, void *fn);
u32 port_io_read(u32 addr, int size);
void port_io_write(u32 addr, u32 value, int size);
void port_io_reset(void);        /* GPU/SPU port state back to power-on (after the intro) */
u32 port_cd_reg_read(u32 reg);   /* cdrom.c: the CD-ROM controller's ports 0-3 */
void port_cd_reg_write(u32 reg, u32 v);
void port_cd_dma(u32 madr, u32 words, int irq);

#endif
