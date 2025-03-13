#include <stdlib.h>
#include <stdio.h>

// If you allocate a data
// structure of 20 bytes every millisecond, and forget to free it you might run
// out of memory in a day or two but if you only run small benchmarks you
// will never notice.

// Using a pointer to a data structure that has been free has an undened
// behavior meaning that the compiler nor the execution need to preserve any
// predictable behavior. This said, we can play around with it and see what
// might happen. Try the following code and reason about what is actually
// happening.

int main() {
    char *heap = malloc(20);  // Allocate 20 bytes on the heap
    *heap = 0x61;             // Assign 'a' (ASCII 0x61) to the allocated memory

    printf("Heap pointing to: 0x%x\n", *heap);

    free(heap);  // Free the allocated memory

    char *foo = malloc(20);  // Allocate another 20 bytes
    *foo = 0x62;             // Assign 'b' (ASCII 0x62)

    printf("Foo pointing to: 0x%x\n", *foo);

    /* Danger ahead: accessing freed memory */
    *heap = 0x63;  // Undefined behavior: heap was freed
    printf("Or is it pointing to: 0x%x\n", *foo);

    return 0;
}

// If you experiment with freeing a data structure twice you will most cer-
// tainly run in to a segmentation fault and a core dump. The reason is that
// the underlying implementation of malloc and free assume that things are
// structured in a certain way and when they are not things break.