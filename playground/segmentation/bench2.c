#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#define MEMORY 100

int memory [MEMORY];

typedef struct array {
    int size;
    int *segment;
    struct array* next;
} array;

array sentinel = {0, &memory[MEMORY], NULL};
array dummy = {1, &memory[-1], &sentinel};

array *allocated = &dummy;

void check() {
    array *nxt = allocated;
    printf("=============== PRINTING =============\n");
    while (nxt != NULL) {
        printf("array (%p) : size %2d, segment %3d, next %14p\n", nxt, nxt->size, nxt->next, (int)(nxt->segment - memory));
        nxt = nxt->next;
    }
    printf("=======================================\n");
    return;
}

array *allocate(int size) {
    array *nxt = allocated;

    while (nxt->size != 0) {
        if ((nxt->next->segment - (nxt->segment + nxt->size)) >= size) {
            // once u find space
            // allocate immediately 
            array *new = (array*)malloc(sizeof(array));

            new->size = size;
            new->segment = (nxt->segment + nxt->size);
            new->next = nxt->next;
            nxt->next = new;
            // and return
            return new;
        }
        nxt = nxt->next;
    }
    return NULL;
}

// wrapper
array *create(int size) {
    printf("create an array of size %4d ..\n", size);
    array *new = allocate(size);
    if (new == NULL) {
        printf("out of memory!!\n");
        exit(-1);
    }
    printf(" > done!\n");
    return new;
}

void delete(array *arr) {
    printf("delete array (%p) of size %4d", arr, arr->size);
    array *nxt = allocated;
    array *prev = NULL;

    while (nxt != arr) {
        prev = nxt; // blank
        nxt = nxt->next; // blank
    }
    // after u're done unlink it
    prev->next = nxt->next; // blank
    // don't forget to free the memory
    free(nxt); // blank
    printf(" < done\n");
    return;
}

void set(array *arr, int pos, int val) {
    arr->segment[pos-1] = val;
}

int get(array *arr, int pos) {
    return (arr->segment[pos-1]);
}

// let's run through the tests, shall we?

void bench1() {
    check();
    array *a = create(20);

    check();
    array *b = create(30);

    check();
    set(a, 10, 110);
    set(a, 14, 114);

    set(b, 8, 208);
    set(b, 12, 212);

    printf(" a[10] + a[14] = %d\n", get(a, 10) + get(a, 14));
    printf("  b[8] + b[12] = %d\n", get(b, 8) + get(b, 12));

    delete(a);
    check();
    delete(b);
    check();
}

// Let's see if we can
// find room for a small segment in between two segments. We will first create
// three arrays, delete the middle one and then create a new array
void bench2() {
    array *a = create(20);
    array *b = create(30);
    array *c = create(30);

    check();
    delete(b);
    check();

    // array *d = create(20);
    array *d = create(5);
    check();

    delete(a);
    delete(c);
    delete(d);
}

int main() {
    // bench1();
    bench2();
    return 0;
}