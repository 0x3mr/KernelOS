#include <stdio.h>
#include <stdlib.h>
#include "dlmall.h"

int main() {
    // Test basic allocation
    printf("Testing basic allocation...\n");
    void *mem1 = dalloc(10);
    void *mem2 = dalloc(20);
    void *mem3 = dalloc(30);
    
    if (mem1 != NULL && mem2 != NULL && mem3 != NULL) {
        printf("Allocation successful!\n");
    } else {
        printf("Allocation failed!\n");
        return 1;
    }
    
    // Test freeing memory
    printf("Testing memory freeing...\n");
    dfree(mem2);  // Free middle block
    
    // Try to allocate again
    void *mem4 = dalloc(15);
    if (mem4 != NULL) {
        printf("Reallocation successful!\n");
    } else {
        printf("Reallocation failed!\n");
        return 1;
    }
    
    // Free remaining blocks
    dfree(mem1);
    dfree(mem3);
    dfree(mem4);
    
    printf("All tests passed!\n");
    return 0;
}