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
    /* (getcontext/makecontext/swapcontext, platform_win.c: the stack pointer it was left at, its function) */
    unsigned int plat_sp;
    void (*plat_fn)(void);
} ucontext_t;

/* Contexts as main.c uses them: getcontext, makecontext with a stack and no arguments, swapcontext to it, its uc_link
 * when the function returns. A switch of stack (the thread's stack bounds in its TEB with it), registers kept as a
 * call keeps them. */
int getcontext(ucontext_t *uc);
void makecontext(ucontext_t *uc, void (*fn)(), int argc, ...);
int swapcontext(ucontext_t *from, const ucontext_t *to);
int setcontext(const ucontext_t *uc);

#endif
