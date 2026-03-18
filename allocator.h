#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

typedef struct block_header {
    size_t size;
    int free;
    struct block_header *next;
} block_header;

void *heap_init();
void *my_malloc(size_t size);
void my_free(void *ptr);
void heap_dump(block_header *ptr);

#endif
