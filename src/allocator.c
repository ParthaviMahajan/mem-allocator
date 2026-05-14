#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/allocator.h"

#define MIN_PAYLOAD 8

//aligning all memory to multiples of 8 for CPU efficiency
size_t align8(size_t size){
    return (size + 7) & ~7;
}

//initialising the head pointer
block_t *head = NULL;

//function to print all blocks, much more easier to understand for user
void print_blocks(){

    block_t *current = head;

    int index = 0;

    size_t total_allocated = 0;
    size_t total_free = 0;
    size_t heap_footprint = 0;

    printf("\n=========== HEAP LAYOUT ===========\n");

    while(current != NULL){

        size_t total_block_size = current->size + sizeof(block_t);

        printf("\nBLOCK %d\n", index++);
        printf("Block Address      : %p\n", (void*)current);
        printf("Offset From Head   : %td bytes\n", (char*)current - (char*)head);
        printf("Aligned Payload    : %zu bytes\n", current->size);
        printf("Metadata Size      : %zu bytes\n", sizeof(block_t));
        printf("Total Block Size   : %zu bytes\n", total_block_size);
        printf("State              : %s\n", current->free ? "FREE" : "ALLOCATED");
        printf("Next Block         : %p\n", (void*)current->next);
        printf("User Memory Start  : %p\n", (void*)(current + 1));
        printf("-------------------------------------------\n");

        heap_footprint += total_block_size;

        if(current->free){
            total_free += current->size;
        }
        else{
            total_allocated += current->size;
        }

        current = current->next;
    }

    printf("\n=========== HEAP SUMMARY ===========\n");
    printf("Total Blocks       : %d\n", index);
    printf("Allocated Bytes    : %zu\n", total_allocated);
    printf("Free Bytes         : %zu\n", total_free);
    printf("Heap Footprint     : %zu\n", heap_footprint);
    printf("====================================\n");
}

//function to request space using sbrk()
block_t *request_space(size_t size){

    block_t *block = sbrk(sizeof(block_t) + size);

    if (block == (void*) - 1){
        return NULL;
    }

    block ->size = size;
    block ->next = NULL;
    block ->free = 0;

    return block;
}

// function to run the first fit strategy
block_t *find_free_block(size_t size){

    block_t *current = head;
    while(current != NULL){

        if(current ->free && current ->size >= size){
            return current;
        }
        current = current ->next;
    }
    return NULL;
}

// function for splitting
void split_block(block_t *block, size_t requested_size){

    size_t remaining_size = block ->size - requested_size;

    if(remaining_size < MIN_PAYLOAD + sizeof(block_t)){
        block ->free = 0;

        return;
    }

    block_t *new_block = (block_t*)((char*)block + sizeof(block_t) + requested_size);

    new_block ->size = remaining_size - sizeof(block_t);
    new_block ->next = block ->next;
    new_block ->free = 1;

    block ->free = 0;
    block ->next = new_block;
    block ->size = requested_size;
}

// actual custom my_malloc
void *my_malloc(size_t size){

    size = align8(size);

    block_t *block;

    if(head == NULL){
        block = request_space(size);
        if(block == NULL){
            return NULL;
        }
        head = block;
    }
    else{
        block = find_free_block(size);

        if(block == NULL){
            block= request_space(size);
            if(block == NULL){
                return NULL;
            }

            block_t *current = head;
            
            while ((current ->next != NULL))
            {
                current = current ->next;
            }

            current ->next = block;
            
        }
        else{
            split_block(block, size);
            block ->free=0;
        }
    }
    return(void*)(block+1);
}

//function to merge consecutive free blocks. always called when a block is freed.
void coalesce(){
    block_t *current = head;

    while(current != NULL && current ->next != NULL){

        if(current ->free && current ->next->free){

            current ->size += current ->next->size + sizeof(block_t);
            current ->next = current->next->next;
        }
        else{
            current = current->next;
        }
    }
}

//custom free function
void my_free(void *ptr){
    if (ptr == NULL){
        return;
    }

    block_t *block = (block_t*)ptr - 1;

    if(block->free){
        printf("ERROR: Double free detected!\n");
        return;
    }

    block ->free = 1;

    coalesce();
}

//function for linked list safety. uses floyd's cycle detection algorithm.
int detect_cycles(){
    
    block_t *slow = head;
    block_t *fast = head;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;

        if(slow == fast){
            return 1;
        }
    }
    return 0;
    
}

//overall heap genuinness checker
void check_heap(){

    block_t *current = head;
    printf("\n===== HEAP CHECK =====\n");

    if(detect_cycles()){
        printf("ERROR: Cycle detected in heap list!\n");
        return;
    }

    while (current != NULL)
    {
        if(current->size == 0){
            printf("ERROR: Zero-sized block detected!\n");
            return;
        }
        if(current->size > 1000000){
            printf("ERROR: Unrealistic block size detected!\n");
            return;
        }

        current = current->next;
    }
    printf("Heap check passed.\n");

}

