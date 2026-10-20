/* libpad on the host: PadInitDirect's receive buffers get the controller state every vblank (from SDL
 * input once the window is in; idle pads until then), mode queries answer for a DualShock, and the motor
 * values the game keeps in its PadSetAct buffer drive a real gamepad's rumble (video.c). */
#include <string.h>
#include "port.h"

static u8 *pad_buf[2];
static int pad_analog[2] = {1, 1};
static u8 *pad_act[2]; /* PadSetAct: where the game keeps the motor values */
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

/* The game names a buffer once and changes the values in it; the pad reads them every vblank (byte 0 the
 * small motor, on or off; byte 1 the large one's strength, per PadSetActAlign's usual 0, 1). */
void PadSetAct(s32 port, u8 *data, s32 len) {
    pad_act[port ? 1 : 0] = data && len >= 2 ? data : NULL;
}

/* the first pad's motors now: small (0 off) and large (0-255) */
void port_pad_motors(u8 *small, u8 *large) {
    u8 *a = pad_act[0];
    int ok = a && (u32)a >= PS1_RAM_BASE && (u32)a < PS1_RAM_BASE + PS1_RAM_SIZE - 1 && pad_analog[0];
    *small = ok ? a[0] : 0;
    *large = ok ? a[1] : 0;
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
