/* libpad on the host: PadInitDirect's receive buffers get the controller state every vblank (from SDL
 * input once the window is in; idle pads until then), mode queries answer for a DualShock. */
#include <string.h>
#include "port.h"

static u8 *pad_buf[2];
static int pad_analog[2] = {1, 1};
static u8 pad_motor[2][2];
u16 port_pad_buttons[2] = {0xFFFF, 0xFFFF};          /* active low, PsyQ bit order */
u8 port_pad_sticks[2][4] = {{0x80, 0x80, 0x80, 0x80}, {0x80, 0x80, 0x80, 0x80}}; /* RX RY LX LY */

void PadInitDirect(u8 *a, u8 *b) {
    pad_buf[0] = a;
    pad_buf[1] = b;
}

void PadStartCom(void) {
}

void PadStopCom(void) {
}

s32 PadEnableCom(s32 mode) {
    (void)mode;
    return 3;
}

/* PadStateStable for the first port, nothing on the second */
s32 PadGetState(s32 port) {
    return (port & 0x10) || port > 0 ? 0 : 6;
}

/* InfoModeCurID 1, InfoModeCurExID 2, InfoModeCurExOffs 3, InfoModeIdTable 4 */
s32 PadInfoMode(s32 port, s32 term, s32 offs) {
    int p = port ? 1 : 0;
    switch (term) {
    case 1:
        return pad_analog[p] ? 7 : 4;
    case 2:
        return pad_analog[p] ? 7 : 4;
    case 3:
        return pad_analog[p] ? 1 : 0;
    case 4:
        return offs < 0 ? 2 : offs == 0 ? 4 : 7;
    }
    return 0;
}

s32 PadSetMainMode(s32 port, s32 offs, s32 lock) {
    (void)lock;
    pad_analog[port ? 1 : 0] = offs == 1;
    return 1;
}

s32 PadSetActAlign(s32 port, u8 *data) {
    (void)port;
    (void)data;
    return 1;
}

void PadSetAct(s32 port, u8 *data, s32 len) {
    int p = port ? 1 : 0;
    if (data && len >= 2) {
        pad_motor[p][0] = data[0];
        pad_motor[p][1] = data[1];
    }
}

void port_pad_vblank(void) {
    int p;
    for (p = 0; p < 2; p++) {
        u8 *b = pad_buf[p];
        if (!b)
            continue;
        if (p == 1) {
            b[0] = 0xFF;
            continue;
        }
        b[0] = 0;
        b[1] = pad_analog[p] ? 0x73 : 0x41;
        b[2] = port_pad_buttons[p] & 0xFF;
        b[3] = port_pad_buttons[p] >> 8;
        memcpy(b + 4, port_pad_sticks[p], 4);
    }
}
