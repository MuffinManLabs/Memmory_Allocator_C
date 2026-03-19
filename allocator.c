#include <stdio.h>
#include <sys/mman.h>
#include "allocator.h"

block_header *head = NULL;

//call mmap once to get 4096-byte slab set up first
//header at the start of the arena.
//return pointer to beginning of arena
void * heap_init() {
    //create a large arena of memory
    void *arena = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    //set the global head to point to the beginnning of arena 
    head = arena;
  
    //set the fields
    head->size = 4096 - sizeof(block_header);
    head->free = 1;
    head->next = NULL;
    return arena;
} 

//walk the linked list of headers
//find first free block big enough (first-fit)
//split the block if it's much bigger than needed
//mark it as in use (free = 0)
//return pointer to memory AFTER the header
void *my_malloc(size_t size) {
    // walk the linked list of headers until 
    // you find a free one of the correct size 
    //make a copy of the global head
    block_header * head_cpy = head;
    while (head_cpy) {
        if (head_cpy->free == 1 && head_cpy->size >= size) {
            head_cpy->free = 0;
            head_cpy->size = size;

            //split that chunk of memory into 2
            block_header * extra_header = (block_header *) (((char *) head_cpy) + sizeof(block_header) + size);
            //now the extra header points to exactly where we need it. 

            //Pointer arithmetic works in units of the type it points to
            return head_cpy+1;
        }
        head_cpy = head_cpy->next;
    }
    return NULL;
}


//step back from ptr to find the header
//mark it as free (free = 1)
//coalesce with next block if it's also free
// my_free(ptr)


//walk the linked list
//print each block's size, free/used status
//(this is a debugging tool to see the state of the memory chunks in arena)
void heap_dump(){
    //make a copy of the global head
    block_header * head_cpy = head;
    while (head_cpy) {
        printf("size is %zu bytes \n", head_cpy->size);
        printf("free is %d\n", head_cpy->free);
        head_cpy = head_cpy->next;
    }
    return;
}