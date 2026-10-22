/* PsyQ malloc3 family on the host, the same allocator as the console's (libc2: K&R's malloc): a circular
 * free list of 8-byte headers {next, size in 8-byte units} with a rover, blocks carved from the end of the
 * first free block big enough, and a zero-size header at the top of the heap that is never merged. The
 * levels size their heaps to fit this allocator's overhead, and every allocation lands at the address it
 * has on the console.
 *
 * The BIOS heap (InitHeap/malloc/free, A0:33-39, which the boot stub uses) is a second heap of the same
 * kind, but it allocates first-fit from the bottom like the BIOS's allocator: the stub sizes it for 8 MB of
 * RAM before it learns there are 2 (its top is in RAM's mirrors), so only its bottom is safe to use. */
#include <string.h>
#include "port.h"

typedef struct Header {
    struct Header *next;
    u32 size; /* in units of sizeof(Header), the header included */
} Header;

typedef struct {
    Header *freep;
    Header *top; /* the zero-size header at the top, whose next is the lowest free block */
    int low;     /* allocate first-fit from the bottom */
} Heap;

static Heap heap3, bios_heap = {NULL, NULL, 1};

static void heap_init(Heap *h, u32 *addr, u32 size) {
    Header *base = (Header *)addr, *top;
    if (!addr || !size)
        return;
    size &= ~7u;
    top = (Header *)((u8 *)addr + size) - 1;
    top->next = base;
    top->size = 0;
    base->next = top;
    base->size = size / sizeof(Header) - 1;
    h->freep = base;
    h->top = top;
}

static void *heap_alloc(Heap *h, u32 nbytes) {
    Header *p, *prevp = h->low ? h->top : h->freep;
    u32 nunits;
    if (!nbytes || !prevp)
        return NULL;
    nunits = (nbytes + 7) / sizeof(Header) + 1;
    for (p = prevp->next;; prevp = p, p = p->next) {
        if (p->size >= nunits) {
            if (p->size == nunits) {
                prevp->next = p->next;
            } else if (h->low) {
                Header *rest = p + nunits;
                rest->size = p->size - nunits;
                rest->next = p->next;
                prevp->next = rest;
                p->size = nunits;
            } else {
                p->size -= nunits;
                p += p->size;
                p->size = nunits;
            }
            h->freep = prevp;
            return p + 1;
        }
        if (p == (h->low ? h->top : h->freep)) {
            port_log("malloc(%X): out of memory\n", nbytes);
            return NULL;
        }
    }
}

static void heap_free(Heap *h, void *ap) {
    Header *bp, *p;
    if (!ap)
        return;
    bp = (Header *)ap - 1;
    for (p = h->freep; !(bp > p && bp < p->next); p = p->next)
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
    h->freep = p;
}

static void *heap_realloc(Heap *h, void *p, u32 size) {
    void *n;
    if (!p)
        return heap_alloc(h, size);
    n = heap_alloc(h, size);
    if (n) {
        u32 old = (((Header *)p - 1)->size - 1) * sizeof(Header);
        memcpy(n, p, old < size ? old : size);
        heap_free(h, p);
    }
    return n;
}

void InitHeap3(u32 *addr, u32 size) {
    heap_init(&heap3, addr, size);
    port_log("InitHeap3 %08X, %X bytes\n", (u32)addr, size);
}

/* A heap of the runtime's own, which malloc3 and co. use instead while port_heap3_own(1) holds: for what a
 * mod has the game make that the level's heap has no room for (it is sized to the level's own needs). */
static Heap own_heap, *heap = &heap3;
static u32 own_arena[0x10000 / 4];

/* on: the own heap from now on (fresh: emptied first), else the level's again */
void port_heap3_own(int on, int fresh) {
    if (fresh)
        heap_init(&own_heap, own_arena, sizeof(own_arena));
    heap = on ? &own_heap : &heap3;
}

void *malloc3(u32 size) {
    return heap_alloc(heap, size);
}

void free3(void *p) {
    heap_free(heap, p);
}

void *calloc3(u32 n, u32 size) {
    void *p = heap_alloc(heap, n * size);
    if (p)
        memset(p, 0, n * size);
    return p;
}

void *realloc3(void *p, u32 size) {
    return heap_realloc(heap, p, size);
}

/* BIOS A0:39 InitHeap, A0:33 malloc, A0:34 free, A0:37 calloc, A0:38 realloc (recomp_rt.c bios_call) */
void bios_init_heap(u32 addr, u32 size) {
    heap_init(&bios_heap, (u32 *)addr, size);
    port_log("InitHeap %08X, %X bytes\n", addr, size);
}

u32 bios_malloc(u32 size) {
    return (u32)heap_alloc(&bios_heap, size);
}

void bios_free(u32 p) {
    heap_free(&bios_heap, (void *)p);
}

u32 bios_calloc(u32 n, u32 size) {
    void *p = heap_alloc(&bios_heap, n * size);
    if (p)
        memset(p, 0, n * size);
    return (u32)p;
}

u32 bios_realloc(u32 p, u32 size) {
    return (u32)heap_realloc(&bios_heap, (void *)p, size);
}
