#include <stdlib.h>
#include <stdio.h>

int main() {
    long *heap = (unsigned long *)calloc(40, sizeof(unsigned long));  // Allocate and zero-initialize

    printf("heap[2]: 0x%lx\n", heap[2]);
    printf("heap[1]: 0x%lx\n", heap[1]);
    printf("heap[0]: 0x%lx\n", heap[0]);
    printf("heap[-1]: 0x%lx\n", heap[-1]);  // Accessing out-of-bounds memory (dangerous!)
    printf("heap[-2]: 0x%lx\n", heap[-2]);

    free(heap);  // Free allocated memory

    printf("\nAfter free:\n");
    printf("heap[2]: 0x%lx\n", heap[2]);
    printf("heap[1]: 0x%lx\n", heap[1]);
    printf("heap[0]: 0x%lx\n", heap[0]);
    printf("heap[-1]: 0x%lx\n", heap[-1]);
    printf("heap[-2]: 0x%lx\n", heap[-2]);

    return 0;
}
