#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

#define MIN_PAYLOAD 8

typedef struct block{
    size_t size;
    int free;
    struct block *next;
} block_t;

void *my_malloc(size_t size);
void my_free(void *ptr);

void print_blocks();
void check_heap();

int detect_cycles();

size_t align8(size_t size);

#endif