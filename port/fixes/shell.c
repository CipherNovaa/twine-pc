/* The front end (SHELL.EXE) on the PC: what the native build changes in its code (port/mods.py builds this into
 * the shell's module). Names are the shell's own.
 *
 * MOD_REPLACES_NATIVE: func_8006CDA8 func_8002ABF0
 */
#include "common.h"

u32 func_8006CDA8_orig(u32 **addr, u32 **header);
void func_8002ABF0_orig(void);

s32 func_8004977C(s32 code);
void func_80049938(s32 code);
void port_spin(u32 ns);

/* libcd's StGetNext: the next whole movie frame in the stream ring, or 1 if none has come in yet. The movie
 * players wait for a frame by calling it a fixed number of times (2000 x 2000 for the first frame), some
 * five seconds on the console but a few milliseconds here, so a call that finds nothing takes as long as
 * such a pass of their loops does on the console. */
u32 func_8006CDA8(u32 **addr, u32 **header) {
    u32 r = func_8006CDA8_orig(addr, header);
    if (r)
        port_spin(1300);
    return r;
}

/* The front end's own developer codes, which nothing calls in the retail game (the loop at 0x800496F8, left
 * after the end of func_800495B0): each frame, every code that has just been completed takes effect. With the
 * devcheats setting they work again. All start with SELECT SELECT CIRCLE CIRCLE, then:
 *   L2 L2 R2 R2                 the sprite position editor (D_80073E28; it prints positions)
 *   L1 L1 L1 L1 L1 L1 L1        every movie in the Movies menu (D_80073E2C)
 *   L1 L1 R1 R1                 any mission can be chosen (D_80073E30)
 *   SELECT R1 L2, five times    D_80073E34, which skips ahead in the front end
 * func_8002ABF0 is the front end's frame (drawing, then waiting for the vblank's buffer swap). */
void func_8002ABF0(void) {
    s32 i;
    if (port_dev_cheats)
        for (i = 0; i < 4; i++)
            if (func_8004977C(i))
                func_80049938(i);
    func_8002ABF0_orig();
}

