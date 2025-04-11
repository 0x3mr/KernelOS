#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdint.h>
#include "dlmall.h"

#define TRUE 1
#define FALSE 0
#define HEAD (sizeof(struct head))
#define MIN(size) (((size) > (8)) ? (size) : (8))
#define LIMIT(size) (MIN(0) + HEAD + size)
#define MAGIC(memory) ((struct head *)memory - 1)
#define HIDE(block) (void *)((struct head *)block + 1)
#define ALIGN 8
#define ARENA (64 * 1024)

struct head {
    uint16_t bfree;  // 2 bytes, the status of block before
    uint16_t bsize;  // 2 bytes, the size of block before
    uint16_t free;   // 2 bytes, the status of the block
    uint16_t size;   // 2 bytes, the size (max 2^16 i.e. 64 Ki byte)
    struct head *next; // 8 bytes pointer
    struct head *prev; // 8 bytes pointer
};

struct head *arena = NULL;
struct head *flist = NULL;

// Find the block after the given block
struct head *after(struct head *block) {
    return (struct head *)((char *)block + block->size + HEAD);
}

// Find the block before the given block
struct head *before(struct head *block) {
    return (struct head *)((char *)block - block->bsize - HEAD);
}

// Split a block into two, returning the second part
struct head *split(struct head *block, int size) {
    int rsize = block->size - size - HEAD;
    block->size = size;
    
    struct head *splt = after(block);
    splt->bsize = size;
    splt->bfree = block->free;
    splt->size = rsize;
    splt->free = TRUE;
    
    struct head *aft = after(splt);
    aft->bsize = rsize;
    
    return splt;
}

// Create a new block using mmap
struct head *new() {
    if (arena != NULL) {
        printf("one arena already allocated\n");
        return NULL;
    }
    
    // using mmap, but could have used sbrk
    struct head *new = mmap(NULL, ARENA,
                            PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (new == MAP_FAILED) {
        printf("mmap failed: error %d\n", errno);
        return NULL;
    }
    
    /* make room for head and dummy */
    uint16_t size = ARENA - 2*HEAD;
    new->bfree = FALSE;
    new->bsize = 0;
    new->free = TRUE;
    new->size = size;
    
    struct head *sentinel = after(new);
    /* only touch the status fields */
    sentinel->bfree = TRUE;
    sentinel->bsize = size;
    sentinel->free = FALSE;
    sentinel->size = 0;
    
    /* this is the only arena we have */
    arena = (struct head *)new;
    
    return new;
}

// Detach a block from the free list
void detach(struct head *block) {
    if (block->next != NULL)
        block->next->prev = block->prev;
    
    if (block->prev != NULL)
        block->prev->next = block->next;
    else
        flist = block->next;
}

// Insert a block into the free list
void insert(struct head *block) {
    block->next = flist;
    block->prev = NULL;
    
    if (flist != NULL)
        flist->prev = block;
    
    flist = block;
}

// Merge a block with adjacent free blocks
struct head *merge(struct head *block) {
    struct head *aft = after(block);
    
    if (block->bfree) {
        struct head *bef = before(block);
        detach(bef);
        
        bef->size = bef->size + block->size + HEAD;
        aft->bsize = bef->size;
        
        block = bef;
    }
    
    if (aft->free) {
        detach(aft);
        
        block->size = block->size + aft->size + HEAD;
        after(aft)->bsize = block->size;
    }
    
    return block;
}

// Adjust the requested size to be a multiple of ALIGN
int adjust(size_t request) {
    int size = MIN(request);
    int rem = size % ALIGN;
    
    if (rem != 0)
        size = size + ALIGN - rem;
    
    return size;
}

// Find a block in the free list that can accommodate the requested size
struct head *find(int size) {
    if (flist == NULL) {
        struct head *block = new();
        if (block == NULL)
            return NULL;
        insert(block);
    }
    
    struct head *current = flist;
    while (current != NULL) {
        if (current->size >= size) {
            detach(current);
            
            if (current->size >= LIMIT(size)) {
                struct head *remaining = split(current, size);
                insert(remaining);
            }
            
            current->free = FALSE;
            after(current)->bfree = FALSE;
            
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

// Check if the free list and arena are in a consistent state
void sanity() {
    // Check free list
    struct head *current = flist;
    while (current != NULL) {
        if (current->free != TRUE) {
            printf("Error: block in free list not marked as free\n");
        }
        
        if (current->size % ALIGN != 0) {
            printf("Error: block size not aligned\n");
        }
        
        if (current->size < MIN(0)) {
            printf("Error: block size below minimum\n");
        }
        
        if (current->next != NULL && current->next->prev != current) {
            printf("Error: free list has broken links\n");
        }
        
        current = current->next;
    }
    
    // Check all blocks in the arena
    if (arena != NULL) {
        current = arena;
        struct head *next;
        
        while (current->size != 0) {
            next = after(current);
            
            if (next->bfree != current->free || next->bsize != current->size) {
                printf("Error: inconsistent block metadata\n");
            }
            
            current = next;
        }
    }
}

// Allocate memory
void *dalloc(size_t request) {
    if (request <= 0) {
        return NULL;
    }
    
    int size = adjust(request);
    struct head *taken = find(size);
    
    if (taken == NULL)
        return NULL;
    else
        return HIDE(taken);
}

// Free memory
void dfree(void *memory) {
    if (memory != NULL) {
        struct head *block = MAGIC(memory);
        struct head *aft = after(block);
        
        block->free = TRUE;
        aft->bfree = TRUE;
        
        block = merge(block);
        insert(block);
    }
    
    return;
}