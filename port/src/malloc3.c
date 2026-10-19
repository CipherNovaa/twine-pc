/* PsyQ malloc3 family on the host: a first-fit heap inside the PS1 RAM region the game hands to InitHeap3,
 * so every allocation is a PS1 address like on the console. */
#include <string.h>
#include "port.h"

typedef struct Block {
    u32 size;          /* bytes after the header */
    u32 free;
    struct Block *next;
    u32 pad;
} Block;

static Block *heap_first;

void InitHeap3(u32 *addr, u32 size) {
    Block *b = (Block *)(((u32)addr + 15) & ~15u);
    size -= (u32)b - (u32)addr;
    b->size = (size & ~15u) - sizeof(Block);
    b->free = 1;
    b->next = NULL;
    heap_first = b;
    port_log("InitHeap3 %08X, %X bytes\n", (u32)addr, size);
}

void *malloc3(u32 size) {
    Block *b;
    size = (size + 15) & ~15u;
    for (b = heap_first; b; b = b->next) {
        if (b->free && b->size >= size) {
            if (b->size >= size + sizeof(Block) + 16) {
                Block *n = (Block *)((u8 *)(b + 1) + size);
                n->size = b->size - size - sizeof(Block);
                n->free = 1;
                n->next = b->next;
                b->next = n;
                b->size = size;
            }
            b->free = 0;
            return b + 1;
        }
    }
    port_log("malloc3(%X): out of memory\n", size);
    return NULL;
}

void free3(void *p) {
    Block *b;
    if (!p)
        return;
    ((Block *)p - 1)->free = 1;
    /* merge neighbouring free blocks */
    for (b = heap_first; b; b = b->next)
        while (b->free && b->next && b->next->free) {
            b->size += sizeof(Block) + b->next->size;
            b->next = b->next->next;
        }
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
        u32 old = ((Block *)p - 1)->size;
        memcpy(n, p, old < size ? old : size);
        free3(p);
    }
    return n;
}
