/* Windows: the CRT's setjmp.h, and sigsetjmp/siglongjmp (no signal mask to keep: a jump never leaves a signal
 * handler here) */
#include_next <setjmp.h>

#ifndef PLAT_WIN_SETJMP_H
#define PLAT_WIN_SETJMP_H
typedef jmp_buf sigjmp_buf;
#define sigsetjmp(buf, savemask) setjmp(buf)
#define siglongjmp longjmp
#endif
