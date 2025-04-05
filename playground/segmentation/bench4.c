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

// void set(array *arr, int pos, int val) {
//     if (pos <= 0 || pos > arr->size) {
//         printf("segmentation fault (set): illegal access at pos %d (size: %d)\n", pos, arr->size);
//         exit(-1);
//     }
//     arr->segment[pos - 1] = val;
// }

// int get(array *arr, int pos) {
//     if (pos <= 0 || pos > arr->size) {
//         printf("segmentation fault (get): illegal access at pos %d (size: %d)\n", pos, arr->size);
//         exit(-1);
//     }
//     return arr->segment[pos - 1];
// }

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

// let's try to access illegal values (that are larger than the actual size)
void bench3() {
    array *a = create(20);
    array *b = create(30);

    set(a, 22, 100);
    set(b, 0, 200);

    printf("a[20] + b[2] = %d\n", get(a, 20) + get(b, 2));

    delete(a);
    delete(b);
}

void bench4() {
    array *a = create(20);
    array *b = create(30);
    array *c = create(30);

    delete(b);
    check();
    array *d = create(50);
    // array *d = create(29);

    delete(a);
    delete(c);
    delete(d);
}

int main() {
    // bench1();
    // bench2();
    // bench3();
    bench4();
    return 0;
}

// what happened visually?
//
// memory array:
// After creation:
// [ [0 ... size 20] [size 30] [size 30] [ ... 100] ]
//
// After deleting b:
// [ [0 ... size 20] [deleted b: free 30 slots] [size 30: 50 to 80] [ 80 ... free slots ... 100] ]
//
// After d creation:
// There's no space..
// How?
// There's no space for 50 consecutive slots, the 30 freed slots in the middle will not fit
// And the 20 free slots at the end will not fit either
// Therefore out of memory safety check is triggered (return array was equal to NULL)