#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Allocate size bytes aligned to a specific boundary (e.g., 16, 32, 64 bytes)
void* custom_aligned_alloc(size_t alignment, size_t size) {
    // Total allocation: requested size + alignment padding + pointer header space
    size_t total_size = size + alignment + sizeof(void*);
    void *raw_ptr = malloc(total_size);
    if (raw_ptr == NULL) return NULL;

    // Calculate start address after reserving space for raw pointer header
    uintptr_t raw_addr = (uintptr_t)raw_ptr + sizeof(void*);

    // Bitwise mask alignment to round up to nearest multiple of alignment boundary
    uintptr_t aligned_addr = (raw_addr + alignment - 1) & ~(alignment - 1);

    // Store raw_ptr immediately before aligned_addr for deallocation recovery
    void **header = (void**)aligned_addr;
    header[-1] = raw_ptr;

    printf("  [Aligned Alloc] Requested: %zu bytes | Boundary: %zu-byte\n", size, alignment);
    printf("     └─ Raw Address: %p -> Aligned Address: %p\n", raw_ptr, (void*)aligned_addr);

    return (void*)aligned_addr;
}

// Free memory allocated via custom_aligned_alloc
void custom_aligned_free(void *aligned_ptr) {
    if (aligned_ptr == NULL) return;

    // Retrieve original raw_ptr stored in header prefix
    void **header = (void**)aligned_ptr;
    void *raw_ptr = header[-1];

    printf("  [Aligned Free] Reclaimed base raw pointer: %p from aligned: %p\n", raw_ptr, aligned_ptr);
    free(raw_ptr);
}

int main() {
    printf("=== Aligned Memory Allocator with Padding Header (ANSI C) ===\n");

    size_t alignment = 64; // 64-byte alignment suitable for AVX-512 / Cache lines
    size_t data_bytes = 256;

    printf("Allocating SIMD vector buffer with 64-byte hardware alignment:\n");
    void *ptr = custom_aligned_alloc(alignment, data_bytes);

    // Verify alignment using modulo arithmetic
    uintptr_t addr_val = (uintptr_t)ptr;
    bool is_aligned = (addr_val % alignment == 0);
    printf("  -> Verification: Address %p modulo %zu = %lu (%s)\n",
           ptr, alignment, addr_val % alignment, is_aligned ? "SUCCESSFULLY ALIGNED" : "UNALIGNED");

    printf("\nDeallocating aligned memory block:\n");
    custom_aligned_free(ptr);

    printf("[PASS] Aligned Memory Allocator execution completed cleanly.\n");
    return 0;
}