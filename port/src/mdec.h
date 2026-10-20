#ifndef MDEC_H
#define MDEC_H

/* The motion decoder (mdec.c): MDEC0 (0x1F801820) and MDEC1 (0x1F801824); io.c feeds DMA channels 0 and 1
 * through it. Plain state, no host dependencies: port/tools/mdectest.c runs it on its own too. */
#include "common.h"

void mdec_write(u32 v);    /* MDEC0 write: a command word or its next parameter word */
u32 mdec_read(void);       /* MDEC0 read: the next output word (0 when there's none) */
int mdec_out_ready(void);  /* an output word can be read (macroblocks whose input is in are decoded first) */
u32 mdec_status(void);     /* MDEC1 read */
void mdec_control(u32 v);  /* MDEC1 write: reset, DMA request enables */

#endif
