#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Example unoptimized structure with explicit compiler padding
struct UnalignedHeader {
    uint8_t  flag;       // 1 byte
    uint64_t timestamp;  // 8 bytes (7 padding bytes inserted before timestamp)
    uint16_t packet_id;  // 2 bytes
};

// Optimized structure rearranged by field size (descending)
struct OptimizedHeader {
    uint64_t timestamp;  // 8 bytes
    uint16_t packet_id;  // 2 bytes
    uint8_t  flag;       // 1 byte
                         // 5 padding bytes at end for 8-byte struct alignment
};

// Check if a memory pointer is aligned to a given boundary (must be power of 2)
bool is_address_aligned(const void *p, size_t alignment) {
    return ((uintptr_t)p & (alignment - 1)) == 0;
}

void inspect_structure_layout() {
    printf("--- Unoptimized Structure Layout Analysis ---\n");
    printf("  Total Size: %zu bytes (Payload: 11 bytes, Padding: %zu bytes)\n",
           sizeof(struct UnalignedHeader), sizeof(struct UnalignedHeader) - 11);
    printf("  Offset of 'flag'     : %zu bytes\n", offsetof(struct UnalignedHeader, flag));
    printf("  Offset of 'timestamp': %zu bytes\n", offsetof(struct UnalignedHeader, timestamp));
    printf("  Offset of 'packet_id': %zu bytes\n", offsetof(struct UnalignedHeader, packet_id));

    printf("\n--- Optimized Structure Layout Analysis ---\n");
    printf("  Total Size: %zu bytes (Payload: 11 bytes, Padding: %zu bytes)\n",
           sizeof(struct OptimizedHeader), sizeof(struct OptimizedHeader) - 11);
    printf("  Offset of 'timestamp': %zu bytes\n", offsetof(struct OptimizedHeader, timestamp));
    printf("  Offset of 'packet_id': %zu bytes\n", offsetof(struct OptimizedHeader, packet_id));
    printf("  Offset of 'flag'     : %zu bytes\n", offsetof(struct OptimizedHeader, flag));
}

int main() {
    printf("=== Static Binary Memory Alignment & Padding Inspector (ANSI C) ===\n\n");

    inspect_structure_layout();

    printf("\n--- Pointer Address Alignment Verification ---\n");
    void *heap_buffer = malloc(64);
    if (heap_buffer == NULL) return 1;

    printf("  Heap Buffer Address: %p\n", heap_buffer);
    printf("  Is 8-byte aligned?  : %s\n", is_address_aligned(heap_buffer, 8) ? "YES" : "NO");
    printf("  Is 16-byte aligned? : %s\n", is_address_aligned(heap_buffer, 16) ? "YES" : "NO");
    printf("  Is 64-byte aligned? : %s\n", is_address_aligned(heap_buffer, 64) ? "YES" : "NO");

    free(heap_buffer);
    printf("\n[PASS] Memory alignment inspection executed cleanly.\n");
    return 0;
}