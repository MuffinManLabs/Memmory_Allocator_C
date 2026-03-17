//testing

/*
================================================================================
                        MEMORY ALLOCATOR - CONCEPT REFERENCE
================================================================================

OVERALL DESIGN:
    - 3 files: allocator.h, allocator.c, main.c
    - 1 struct: block_header (size, free, *next)
    - 4 functions: heap_init(), my_malloc(), my_free(), heap_dump()

THE STRUCT (label that sits before each chunk):

    typedef struct block_header {
        size_t size;                // how many usable bytes after this header
        int free;                   // 1 = free, 0 = in use
        struct block_header *next;  // pointer to next header (linked list)
    } block_header;

================================================================================
                              MEMORY DIAGRAMS
================================================================================

AFTER heap_init() — one big free block:

    [header         ][              4072 bytes free                        ]
     size: 4072
     free: 1
     next: NULL

AFTER my_malloc(100) — split into used + free:

    [hdr A ][ 100 bytes ][hdr B         ][        3948 bytes free         ]
     size:100  (in use)   size: 3948
     free: 0              free: 1
     next: B              next: NULL
             ^
             |
             pointer returned to user (right AFTER the header)

AFTER my_malloc(200) — another split:

    [hdr A][100 used][hdr B][200 used][hdr C][       3724 free            ]

AFTER my_free(ptr) — flip the free flag:

    step backward from ptr by sizeof(block_header) to find the header
    set header->free = 1

COALESCING — merge adjacent free blocks:

    Before:  [hdr A][ 100 free ][hdr B][ 3948 free          ]
    After:   [hdr A][              4072 free                 ]
    (hdr B disappears, its size + header size added to hdr A)

================================================================================
                           FUNCTION OVERVIEW
================================================================================

heap_init()         — call mmap once to get 4096-byte slab
                      set up first header at the start of the slab

my_malloc(size)     — walk the linked list of headers
                      find first free block big enough (first-fit)
                      split the block if it's much bigger than needed
                      mark it as in use (free = 0)
                      return pointer to memory AFTER the header

my_free(ptr)        — step back from ptr to find the header
                      mark it as free (free = 1)
                      coalesce with next block if it's also free

heap_dump()         — walk the linked list
                      print each block's size, free/used status
                      (debugging tool to see the state of the slab)

================================================================================
*/

#include <stdio.h>
#include "allocator.h"
#include <sys/mman.h>

int main() {
    void *slab = heap_init();
    if (slab == MAP_FAILED) {
        printf("mmap failed\n");
    } else {
        printf("heap initialized at address: %p\n", slab);
    }
    return 0;
}
