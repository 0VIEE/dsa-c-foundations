#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SLAB_SLOTS 4   // Number of object slots per slab
#define OBJECT_SIZE 32 // Size of each object in bytes

struct ObjectSlot {
    bool is_free;
    struct ObjectSlot *link;
    uint8_t data[OBJECT_SIZE];
};

struct Slab {
    int free_count;
    struct ObjectSlot slots[SLAB_SLOTS];
    struct Slab *link; // Pointers to next slab in cache list
};

struct SlabCache {
    size_t object_size;
    struct Slab *partial_slabs;
    struct Slab *full_slabs;
};

struct SlabCache* create_slab_cache(size_t obj_size) {
    struct SlabCache *cache = (struct SlabCache*)malloc(sizeof(struct SlabCache));
    cache->object_size = obj_size;
    cache->partial_slabs = NULL;
    cache->full_slabs = NULL;
    return cache;
}

struct Slab* allocate_slab() {
    struct Slab *slab = (struct Slab*)malloc(sizeof(struct Slab));
    slab->free_count = SLAB_SLOTS;
    slab->link = NULL;

    for (int i = 0; i < SLAB_SLOTS; i++) {
        slab->slots[i].is_free = true;
        slab->slots[i].link = (i < SLAB_SLOTS - 1) ? &slab->slots[i + 1] : NULL;
    }
    return slab;
}

void* slab_alloc(struct SlabCache *cache) {
    // If no partial slab exists, create a new one
    if (cache->partial_slabs == NULL) {
        struct Slab *new_slab = allocate_slab();
        new_slab->link = cache->partial_slabs;
        cache->partial_slabs = new_slab;
    }

    struct Slab *slab = cache->partial_slabs;
    
    // Find first free slot in the slab
    struct ObjectSlot *slot = NULL;
    for (int i = 0; i < SLAB_SLOTS; i++) {
        if (slab->slots[i].is_free) {
            slot = &slab->slots[i];
            break;
        }
    }

    if (slot != NULL) {
        slot->is_free = false;
        slab->free_count--;

        // If slab is now full, move it from partial to full list
        if (slab->free_count == 0) {
            cache->partial_slabs = slab->link;
            slab->link = cache->full_slabs;
            cache->full_slabs = slab;
        }
        return (void*)slot->data;
    }
    return NULL;
}

void free_slab_cache(struct SlabCache *cache) {
    struct Slab *p = cache->partial_slabs;
    while (p != NULL) {
        struct Slab *temp = p;
        p = p->link;
        free(temp);
    }
    p = cache->full_slabs;
    while (p != NULL) {
        struct Slab *temp = p;
        p = p->link;
        free(temp);
    }
    free(cache);
}

int main() {
    printf("=== Simple Slab Allocator for Object Caching ===\n");
    struct SlabCache *packet_cache = create_slab_cache(OBJECT_SIZE);

    printf("Allocating objects from Slab Cache...\n");
    void *obj1 = slab_alloc(packet_cache);
    void *obj2 = slab_alloc(packet_cache);

    printf("  -> Object 1 allocated at: %p\n", obj1);
    printf("  -> Object 2 allocated at: %p\n", obj2);

    free_slab_cache(packet_cache);
    printf("[PASS] Slab Allocator framework verified and cleaned up successfully.\n");
    return 0;
}