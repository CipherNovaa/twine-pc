#ifndef PLAT_WIN_SYS_MMAN_H
#define PLAT_WIN_SYS_MMAN_H
/* Windows: mmap as the runtime uses it (platform_win.c): anonymous memory, or views of a memfd_create'd file (a
 * section in the page file) at fixed addresses, the same memory seen at several (PS1 RAM's mirrors). Addresses
 * and offsets are multiples of 64 KB (Windows' allocation granularity). */
#include <stddef.h>
#include <sys/types.h>

#define PROT_NONE 0
#define PROT_READ 1
#define PROT_WRITE 2
#define PROT_EXEC 4
#define MAP_SHARED 0x01
#define MAP_PRIVATE 0x02
#define MAP_FIXED 0x10
#define MAP_ANONYMOUS 0x20
#define MAP_ANON MAP_ANONYMOUS
#define MAP_FIXED_NOREPLACE 0x100000
#define MAP_NORESERVE 0x4000 /* (ignored: the memory is committed) */
#define MAP_FAILED ((void *)-1)

void *mmap(void *addr, size_t len, int prot, int flags, int fd, long long offset);
int munmap(void *addr, size_t len);
int mprotect(void *addr, size_t len, int prot);
int memfd_create(const char *name, unsigned int flags);

#endif
