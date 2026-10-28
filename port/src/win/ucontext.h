#ifndef PLAT_WIN_UCONTEXT_H
#define PLAT_WIN_UCONTEXT_H
/* Windows: Linux's i386 ucontext, which the platform layer's signals hand their handlers (platform_win.c): the
 * registers of the interrupted thread, in glibc's order; what a handler changes there is what the thread resumes
 * with. */
#include <signal.h>

typedef int greg_t;
#define NGREG 19
typedef greg_t gregset_t[NGREG];
enum {
    REG_GS, REG_FS, REG_ES, REG_DS, REG_EDI, REG_ESI, REG_EBP, REG_ESP, REG_EBX, REG_EDX, REG_ECX, REG_EAX,
    REG_TRAPNO, REG_ERR, REG_EIP, REG_CS, REG_EFL, REG_UESP, REG_SS
};
typedef struct {
    gregset_t gregs;
} mcontext_t;
typedef struct ucontext_t {
    unsigned long uc_flags;
    struct ucontext_t *uc_link;
    stack_t uc_stack;
    mcontext_t uc_mcontext;
    sigset_t uc_sigmask;
} ucontext_t;

#endif
