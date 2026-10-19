/* libgte functions with host behaviour (the rest of libgte is recompiled and runs on the software GTE). */
#include "port.h"
#include "gte_native.h"

/* InitGeom: GTE defaults. The PS1 version also patches the BIOS exception handler (_patch_gte) and enables
 * cop2, which the host doesn't need. */
void InitGeom(void) {
    GTE_ctc2(29, 0x155);    /* ZSF3 */
    GTE_ctc2(30, 0x100);    /* ZSF4 */
    GTE_ctc2(26, 0x3E8);    /* H = 1000 */
    GTE_ctc2(27, -0x1062);  /* DQA */
    GTE_ctc2(28, 0x1400000); /* DQB */
    GTE_ctc2(24, 0);        /* OFX */
    GTE_ctc2(25, 0);        /* OFY */
}
