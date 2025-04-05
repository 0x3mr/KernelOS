#include <stdio.h>
#include <stddef.h>
#define MEMORY 100

int memory [MEMORY];

typedef struct array {
    int size;
    int *segment;
    struct array* next;
} array;

array sentinel = {5, &memory[MEMORY], NULL};
array dummy = {1, &memory[-1], &sentinel};

array *allocated = &dummy;

void check() {
    array *nxt = allocated;
    while (nxt != NULL) {
        printf("array (%p) : size %2d, segment %3d, next %14p\n", nxt, nxt->size, nxt->next, (int)(nxt->segment - memory));
        nxt = nxt->next;
    }
    return;
}

int main() {
    check();
    return 0;
}