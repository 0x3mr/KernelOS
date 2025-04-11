#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdint.h>
#include "dlmall.h"

#define TRUE 1
#define FALSE 0
#define HEAD (sizeof(head))
#define MIN(size) (((size) > (8)) ? (size) : (8))
#define LIMIT(size) (MIN(0) + HEAD + size)
#define MAGIC(memory) ((head *)memory - 1)
#define HIDE(block) (void *)((head *)block + 1)
#define ALIGN 8
#define ARENA (64 * 1024)   //64KB

typedef struct head {
    uint16_t bfree;  // 2 bytes, the status of block before
    uint16_t bsize;  // 2 bytes, the size of block before
    uint16_t free;   // 2 bytes, the status of the block
    uint16_t size;   // 2 bytes, the size (max 2^16 i.e. 64 Ki byte)
    struct head *next; // 8 bytes pointer
    struct head *prev; // 8 bytes pointer
}head;

head *arena = NULL;
head *flist = NULL;

//find the block after the given block
head *after(head *block) {
    return (head *)((char *)block + block->size + HEAD);
}

//find the block before the given block (find previous)
head *before(head *block) {
    return (head *)((char *)block - block->bsize - HEAD);
}

//split a block into two, returning the second part
head *split(head *block, int size) {
    int rsize = block->size - size - HEAD;
    block->size = size;
    
    head *splt = after(block);
    splt->bsize = size;
    splt->bfree = block->free;
    splt->size = rsize;
    splt->free = TRUE;
    
    head *aft = after(splt);
    aft->bsize = rsize;
    
    return splt;
}

//create a new block using mmap
head *new() {
    if (arena != NULL) {
        printf("one arena already allocated\n");
        return NULL;
    }
    
    // using mmap, but could have used sbrk
    head *new = mmap(NULL, ARENA,
                            PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (new == MAP_FAILED) {
        printf("mmap failed: error %d\n", errno);
        return NULL;
    }
    
    // make room for head and dummy
    uint16_t size = ARENA - 2*HEAD;
    new->bfree = FALSE;
    new->bsize = 0;
    new->free = TRUE;
    new->size = size;
    
    head *sentinel = after(new);
    // only touch the status fields
    sentinel->bfree = TRUE;
    sentinel->bsize = size;
    sentinel->free = FALSE;
    sentinel->size = 0;
    
    // this is the only arena we have
    arena = (head *)new;
    
    return new;
}

//detach a block from the free list
void detach(head *block) {
    if (block->next != NULL)
        block->next->prev = block->prev;
    
    if (block->prev != NULL)
        block->prev->next = block->next;
    else
        flist = block->next;
}

//insert some block into the free list
void insert(head *block) {
    block->next = flist;
    block->prev = NULL;
    
    if (flist != NULL)
        flist->prev = block;
    
    flist = block;
}

//merge certain block with adjacent free blocks
head *merge(head *block) {
    head *aft = after(block);
    
    if (block->bfree) {
        head *bef = before(block);
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

// Adjust the requested size to be a multiple of ALIGN macro
int adjust(size_t request) {
    int size = MIN(request);
    int rem = size % ALIGN;
    
    if (rem != 0)
        size = size + ALIGN - rem;
    
    return size;
}

//find a block in the free list that can accommodate the requested size
head *find(int size) {
    if (flist == NULL) {
        head *block = new();
        if (block == NULL)
            return NULL;
        insert(block);
    }
    
    head *current = flist;
    while (current != NULL) {
        if (current->size >= size) {
            detach(current);
            
            if (current->size >= LIMIT(size)) {
                head *remaining = split(current, size);
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

//check if the free list and arena are in a consistent state
void sanity() {
    // Check free list
    head *current = flist;
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
        head *next;
        
        while (current->size != 0) {
            next = after(current);
            
            if (next->bfree != current->free || next->bsize != current->size) {
                printf("Error: inconsistent block metadata\n");
            }
            
            current = next;
        }
    }
}

//allocate 
void *dalloc(size_t request) {
    if (request <= 0) {
        return NULL;
    }
    
    int size = adjust(request);
    head *taken = find(size);
    
    if (taken == NULL)
        return NULL;
    else
        return HIDE(taken);
}

//free
void dfree(void *memory) {
    if (memory != NULL) {
        head *block = MAGIC(memory);
        head *aft = after(block);
        
        block->free = TRUE;
        aft->bfree = TRUE;
        
        block = merge(block);
        insert(block);
    }
    
    return;
}