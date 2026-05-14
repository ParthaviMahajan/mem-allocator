#include <stdio.h>

#include "../include/allocator.h"

int main(){

    printf("\n===== INITIAL ALLOCATIONS =====\n");

    void *a = my_malloc(100);
    void *b = my_malloc(200);
    void *c = my_malloc(150);

    print_blocks();
    
    printf("\n===== FREEING BLOCK B =====\n");
    my_free(b);
    print_blocks();

    printf("\n===== FREEING BLOCK C =====\n");
    my_free(c);
    print_blocks();

    printf("\n===== ALLOCATING 250 BYTES =====\n");
    void *d = my_malloc(250);
    print_blocks();

    printf("\n===== DOUBLE FREE TEST =====\n");
    my_free(a);
    my_free(a);

    printf("\n===== HEAP CHECK =====\n");
    check_heap();

    return 0;
}