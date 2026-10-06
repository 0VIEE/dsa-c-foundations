#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_LEVEL 4 // Represents 2^4 = 16 units of base size (e.g., 16 bytes per unit)
#define UNIT_SIZE 16

struct BuddyBlock {
    int level;
    bool is_free;
    struct BuddyBlock *link;
};

struct BuddyAllocator {
    void *memory_pool;
    size_t total_size;
    struct BuddyBlock *free_lists[MAX_LEVEL + 1];
};

struct BuddyAllocator* create_buddy_allocator(size_t pool_bytes) {
    struct BuddyAllocator *allocator = (struct BuddyAllocator*)malloc(sizeof(struct BuddyAllocator));
    allocator->total_size = pool_bytes;
    allocator->memory_pool = malloc(pool_bytes);

    for (int i = 0; i <= MAX_LEVEL; i++) {
        allocator->free_lists[i] = NULL;
    }

    struct BuddyBlock *root = (struct BuddyBlock*)allocator->memory_pool;
    root->level = MAX_LEVEL;
    root->is_free = true;
    root->link = NULL;
    allocator->free_lists[MAX_LEVEL] = root;

    return allocator;
}

// Find block level required for requested size
int size_to_level(size_t size) {
    size_t needed = size + sizeof(struct BuddyBlock);
    int level = 0;
    size_t cap = UNIT_SIZE;
    while (cap < needed && level < MAX_LEVEL) {
        cap *= 2;
        level++;
    }
    return level;
}

// Remove block from its respective free list
void remove_from_freelist(struct BuddyAllocator *allocator, struct BuddyBlock *block, int level) {
    struct BuddyBlock **p = &(allocator->free_lists[level]);
    while (*p != NULL) {
        if (*p == block) {
            *p = block->link;
            break;
        }
        p = &((*p)->link);
    }
}

// Add block to its respective free list
void add_to_freelist(struct BuddyAllocator *allocator, struct BuddyBlock *block, int level) {
    block->link = allocator->free_lists[level];
    block->is_free = true;
    allocator->free_lists[level] = block;
}

// Allocate memory block using Buddy Splitting
void* buddy_alloc(struct BuddyAllocator *allocator, size_t size) {
    int target_level = size_to_level(size);
    int current_level = target_level;

    // Find smallest available free block at or above target level
    while (current_level <= MAX_LEVEL && allocator->free_lists[current_level] == NULL) {
        current_level++;
    }

    if (current_level > MAX_LEVEL) {
        return NULL; // Out of memory
    }

    // Pop block from free list
    struct BuddyBlock *block = allocator->free_lists[current_level];
    allocator->free_lists[current_level] = block->link;

    // Recursively split block down to target level
    while (current_level > target_level) {
        current_level--;
        size_t half_size = (UNIT_SIZE << current_level);
        struct BuddyBlock *buddy = (struct BuddyBlock*)((uint8_t*)block + half_size);
        
        block->level = current_level;
        buddy->level = current_level;
        buddy->is_free = true;

        // Add buddy to the lower level free list
        add_to_freelist(allocator, buddy, current_level);
    }

    block->is_free = false;
    block->link = NULL;
    return (void*)((uint8_t*)block + sizeof(struct BuddyBlock));
}

// Free memory block with recursive buddy coalescing
void buddy_free(struct BuddyAllocator *allocator, void *ptr) {
    if (ptr == NULL) return;

    struct BuddyBlock *block = (struct BuddyBlock*)((uint8_t*)ptr - sizeof(struct BuddyBlock));
    int level = block->level;

    uint8_t *pool_start = (uint8_t*)allocator->memory_pool;

    // Coalesce upwards with buddy if available
    while (level < MAX_LEVEL) {
        size_t block_size = (UNIT_SIZE << level);
        size_t offset = (uint8_t*)block - pool_start;
        size_t buddy_offset = offset ^ block_size;
        struct BuddyBlock *buddy = (struct BuddyBlock*)(pool_start + buddy_offset);

        // Check if buddy is free and at the same level
        bool buddy_is_free_at_level = false;
        struct BuddyBlock *p = allocator->free_lists[level];
        while (p != NULL) {
            if (p == buddy) {
                buddy_is_free_at_level = true;
                break;
            }
            p = p->link;
        }

        if (!buddy_is_free_at_level) {
            break; // Buddy is not free; stop coalescing
        }

        // Remove buddy from free list
        remove_from_freelist(allocator, buddy, level);

        // Merge with buddy (choose the lower memory address as the combined block)
        if (buddy_offset < offset) {
            block = buddy;
        }
        level++;
        block->level = level;
    }

    add_to_freelist(allocator, block, level);
}

void free_buddy_allocator(struct BuddyAllocator *allocator) {
    if (allocator != NULL) {
        free(allocator->memory_pool);
        free(allocator);
    }
}

int main() {
    printf("=== Buddy Allocator Splitting & Coalescing Test ===\n");
    size_t pool_size = 256; // 256 bytes pool (UNIT_SIZE 16 * 2^MAX_LEVEL 16 = 256 bytes)
    struct BuddyAllocator *allocator = create_buddy_allocator(pool_size);

    void *p1 = buddy_alloc(allocator, 30);
    void *p2 = buddy_alloc(allocator, 30);

    if (p1 && p2) {
        printf("[SUCCESS] Allocated two blocks successfully: p1=%p, p2=%p\n", p1, p2);
    } else {
        printf("[FAIL] Allocation failed\n");
    }

    printf("Freeing p1 and p2 (triggering recursive coalescing)...\n");
    buddy_free(allocator, p1);
    buddy_free(allocator, p2);

    // Verify root is restored at max level
    if (allocator->free_lists[MAX_LEVEL] != NULL) {
        printf("[PASS] Memory fully coalesced back to root block at level %d.\n", MAX_LEVEL);
    } else {
        printf("[FAIL] Coalescing incomplete.\n");
    }

    free_buddy_allocator(allocator);
    return 0;
}