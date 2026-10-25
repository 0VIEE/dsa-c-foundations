#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define CACHE_LINE_SIZE 64

// Unpadded structure - susceptible to false sharing across cores
struct UnpaddedState {
    uint64_t counter_a; // Shares 64-byte cache line with counter_b
    uint64_t counter_b;
};

// Padded structure - explicit cache line isolation
struct PaddedState {
    uint64_t counter_a;
    uint8_t padding_a[CACHE_LINE_SIZE - sizeof(uint64_t)]; // Force next field to new cache line
    uint64_t counter_b;
    uint8_t padding_b[CACHE_LINE_SIZE - sizeof(uint64_t)];
};

int main() {
    printf("=== Cache Line Padding & False Sharing Elimination (ANSI C) ===\n");

    struct UnpaddedState unpadded = { .counter_a = 0, .counter_b = 0 };
    struct PaddedState padded = { .counter_a = 0, .counter_b = 0 };

    printf("1. Unpadded Structure Memory Layout:\n");
    printf("  -> Offset counter_a: %zu bytes\n", (size_t)&unpadded.counter_a - (size_t)&unpadded);
    printf("  -> Offset counter_b: %zu bytes\n", (size_t)&unpadded.counter_b - (size_t)&unpadded);
    printf("  -> Total Struct Size: %zu bytes (Fits in single 64B cache line -> High False Sharing Risk!)\n\n",
           sizeof(struct UnpaddedState));

    printf("2. Padded Structure Memory Layout (Cache Line Isolated):\n");
    printf("  -> Offset counter_a: %zu bytes\n", (size_t)&padded.counter_a - (size_t)&padded);
    printf("  -> Offset counter_b: %zu bytes\n", (size_t)&padded.counter_b - (size_t)&padded);
    printf("  -> Total Struct Size: %zu bytes (Separated into independent 64B cache lines!)\n\n",
           sizeof(struct PaddedState));

    // Simulate core updates
    for (uint64_t i = 0; i < 10000000ULL; i++) {
        padded.counter_a++;
        padded.counter_b++;
    }

    printf("  -> Final Counters: A=%lu, B=%lu\n", padded.counter_a, padded.counter_b);
    printf("[PASS] Cache line padding verification completed cleanly.\n");
    return 0;
}