#include <stdio.h>
#include <sys/mman.h>
#include "allocator.h"


// void *ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);


//call mmap once to get 4096-byte slab set up first
//header at the start of the slab
void * heap_init() {
    //create a large arena of memory
    void *arena = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    //point a block header to the memory location as the enitre arena 
    block_header *first_header = arena;
    //set the fields
    first_header->size = 4096 - sizeof(block_header);
    first_header->free = 1;
    first_header->next = NULL;
    return arena;
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
void heap_dump(block_header *ptr){
    while (ptr) {
        printf("size is %zu bytes \n", ptr->size);
        printf("free is %d\n", ptr->free);
        ptr = ptr->next;
    }
    return;
}