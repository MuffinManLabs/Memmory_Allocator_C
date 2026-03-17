#include <stdio.h>
#include <sys/mman.h>
#include "allocator.h"


// void *ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);


//call mmap once to get 4096-byte slab set up first
//header at the start of the slab
void * heap_init() {
    //create a large slab of memory
    void *slab_ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    //create the header you will attach to the beginning
    block_header *first_header = calloc(1,sizeof(block_header));

    return slab_ptr;
} 

//walk the linked list of headers
//find first free block big enough (first-fit)
//split the block if it's much bigger than needed
//mark it as in use (free = 0)
//return pointer to memory AFTER the header
// my_malloc(size)


//step back from ptr to find the header
//mark it as free (free = 1)
//coalesce with next block if it's also free
// my_free(ptr)


//walk the linked list
//print each block's size, free/used status
//(debugging tool to see the state of the slab)
// heap_dump()