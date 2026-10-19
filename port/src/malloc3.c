/* PsyQ malloc3 family on the host, the same allocator as the console's (libc2: K&R's malloc): a circular
 * free list of 8-byte headers {next, size in 8-byte units} with a rover, blocks carved from the end of the
 * first free block big enough, and a zero-size header at the top of the heap that is never merged. The
 * levels size their heaps to fit this allocator's overhead, and every allocation lands at the address it
 * has on the console. */
#include <string.h>
#include "port.h"

typedef struct Header {
    struct Header *next;
    u32 size; /* in units of sizeof(Header), the header included */
} Header;

static Header *freep;

void InitHeap3(u32 *addr, u32 size) {
    Header *base = (Header *)addr, *top;
    if (!addr || !size)
        return;
    size &= ~7u;
    top = (Header *)((u8 *)addr + size) - 1;
    top->next = base;
    top->size = 0;
    base->next = top;
    base->size = size / sizeof(Header) - 1;
    freep = base;
    port_log("InitHeap3 %08X, %X bytes\n", (u32)addr, size);
}

void *malloc3(u32 nbytes) {
    Header *p, *prevp = freep;
    u32 nunits;
    if (!nbytes)
        return NULL;
    nunits = (nbytes + 7) / sizeof(Header) + 1;
    for (p = prevp->next;; prevp = p, p = p->next) {
        if (p->size >= nunits) {
            if (p->size == nunits) {
                prevp->next = p->next;
            } else {
                p->size -= nunits;
                p += p->size;
                p->size = nunits;
            }
            freep = prevp;
            return p + 1;
        }
        if (p == freep) {
            port_log("malloc3(%X): out of memory\n", nbytes);
            return NULL;
        }
    }
}

void free3(void *ap) {
    Header *bp, *p;
    if (!ap)
        return;
    bp = (Header *)ap - 1;
    for (p = freep; !(bp > p && bp < p->next); p = p->next)
        if (p >= p->next && (bp > p || bp < p->next))
            break;
    if (bp + bp->size == p->next && p->next->size != 0) {
        bp->size += p->next->size;
        bp->next = p->next->next;
    } else {
        bp->next = p->next;
    }
    if (p + p->size == bp) {
        p->size += bp->size;
        p->next = bp->next;
    } else {
        p->next = bp;
    }
    freep = p;
}

void *calloc3(u32 n, u32 size) {
    void *p = malloc3(n * size);
    if (p)
        memset(p, 0, n * size);
    return p;
}

void *realloc3(void *p, u32 size) {
    void *n;
    if (!p)
        return malloc3(size);
    n = malloc3(size);
    if (n) {
        u32 old = (((Header *)p - 1)->size - 1) * sizeof(Header);
        memcpy(n, p, old < size ? old : size);
        free3(p);
    }
    return n;
}
