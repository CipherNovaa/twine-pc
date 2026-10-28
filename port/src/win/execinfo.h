#ifndef PLAT_WIN_EXECINFO_H
#define PLAT_WIN_EXECINFO_H
/* Windows: glibc's backtrace (the frame pointers' chain, RtlCaptureStackBackTrace); its symbols are the module
 * and offset (addr2line -f -e twine.exe <address>) */
int backtrace(void **at, int n);
void backtrace_symbols_fd(void *const *at, int n, int fd);
#endif
