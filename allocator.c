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
    block_header * current = head;

    while (current) {
        if (current->free == 1 && current->size >= size) {
            //keep track of the original size of this chunk of memory
            size_t original_size = current->size;
            //keep track of what this free chunk of memory was pointing to before you split it up
            block_header * original_next = current->next;
            current->free = 0;


            //split that chunk of memory into 2 (only if you can still fit a header into the remaining memory in that block)
            if (sizeof(block_header) < original_size - size ){ 
                block_header * new_block = (block_header *) (((char *) current) + sizeof(block_header) + size);
                //shrink the size if you split the memory into 2
                current->size = size;
                //now the new block header points to exactly where we need it.
                new_block->free = 1;
                new_block->size = original_size - sizeof(block_header) - current->size;
                new_block->next = original_next;
                //connect the current header to the new block
                current->next = new_block;
                
            }
            
            //Pointer arithmetic works in units of the type it points to
            return current+1;
        }
        current = current->next;
    }
    return NULL;
}


//step back from ptr to find the header
//mark it as free (free = 1)
//coalesce (merge) with next block if it's also free
void my_free(void *ptr) {
    //relocate the pointer from after the header to beginning of it
    block_header * new_ptr = (block_header *)ptr - 1;
    new_ptr->free = 1;

    //merge with block ahead if possible
    if (new_ptr->next != NULL && new_ptr->next->free == 1) {
        //add the usable space of the current block, then the header of next block and the usable space of next block
        new_ptr->size = new_ptr->size + sizeof(block_header) + new_ptr->next->size;
        //remove the next header out of the linked list 
        new_ptr->next = new_ptr->next->next;
    }
    
}


//walk the linked list
//print each block's size, free/used status
//(this is a debugging tool to see the state of the memory chunks in arena)
void heap_dump(){
    block_header * current = head;
    while (current) {
        printf("size is %zu bytes \n", current->size);
        printf("free is %d\n", current->free);
        current = current->next;
    }
    return;
}