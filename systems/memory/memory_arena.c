#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define ARENA_CAPACITY (1024 * 1024) // 1 MB Pool

// Memory Arena Structure
struct Arena {
    uint8_t *buffer;
    size_t capacity;
    size_t offset;
};

struct Arena* create_arena(size_t capacity) {
    struct Arena *arena = (struct Arena*)malloc(sizeof(struct Arena));
    arena->buffer = (uint8_t*)malloc(capacity);
    arena->capacity = capacity;
    arena->offset = 0;
    return arena;
}

// O(1) Allocation with 8-byte alignment
void* arena_alloc(struct Arena *arena, size_t size) {
    // Align to 8 bytes
    size_t aligned_size = (size + 7) & ~7;

    if (arena->offset + aligned_size > arena->capacity) {
        fprintf(stderr, "Arena out of memory!\n");
        return NULL;
    }

    void *ptr = &arena->buffer[arena->offset];
    arena->offset += aligned_size;
    return ptr;
}

// Reset entire arena in O(1)
void arena_reset(struct Arena *arena) {
    arena->offset = 0;
}

void free_arena(struct Arena *arena) {
    free(arena->buffer);
    free(arena);
}

// Demonstration: Linked list node using arena allocation
struct Node {
    int val;
    struct Node *link;
};

// Fast Integer Reader (POSIX getchar_unlocked or fallback)
#ifdef _WIN32
#define GETCHAR getchar
#else
#define GETCHAR getchar_unlocked
#endif

int fast_read_int() {
    int x = 0;
    int ch = GETCHAR();
    while (ch < '0' || ch > '9') {
        if (ch == EOF) return 0;
        ch = GETCHAR();
    }
    while (ch >= '0' && ch <= '9') {
        x = (x << 3) + (x << 1) + (ch - '0');
        ch = GETCHAR();
    }
    return x;
}

int main() {
    struct Arena *arena = create_arena(ARENA_CAPACITY);

    printf("=== Memory Arena Allocator & Fast I/O ===\n");
    printf("Arena Pool Capacity: %zu bytes (1 MB)\n", arena->capacity);

    // Allocate 5 linked nodes using our arena
    struct Node *head = NULL;
    int sample_data[] = {10, 20, 30, 40, 50};
    int n = sizeof(sample_data) / sizeof(sample_data[0]);

    for (int i = 0; i < n; i++) {
        struct Node *p = (struct Node*)arena_alloc(arena, sizeof(struct Node));
        p->val = sample_data[i];
        p->link = head;
        head = p;
    }

    printf("Current Arena Offset (Allocated): %zu bytes\n", arena->offset);

    // Traverse list using pointer 'p'
    printf("Traversing Arena-Allocated List: [ ");
    struct Node *p = head;
    while (p != NULL) {
        printf("%d ", p->val);
        p = p->link;
    }
    printf("]\n");

    // Instant O(1) bulk deallocation
    arena_reset(arena);
    printf("Arena Reset. New Offset: %zu bytes (Instantly deallocated all nodes)\n", arena->offset);

    free_arena(arena);
    return 0;
}