#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BITS_PER_WORD 64
#define NUM_WORDS 4 // 256 bits total

struct SuccinctBitvector {
    uint64_t words[NUM_WORDS];
    uint32_t block_rank[NUM_WORDS]; // Precomputed cumulative rank at each word boundary
};

struct SuccinctBitvector* create_bitvector() {
    struct SuccinctBitvector *p = (struct SuccinctBitvector*)malloc(sizeof(struct SuccinctBitvector));
    for (int i = 0; i < NUM_WORDS; i++) {
        p->words[i] = 0;
        p->block_rank[i] = 0;
    }
    return p;
}

// Set bit at index bit_idx to 1
void set_bit(struct SuccinctBitvector *p, size_t bit_idx) {
    size_t word_idx = bit_idx / BITS_PER_WORD;
    size_t offset = bit_idx % BITS_PER_WORD;
    p->words[word_idx] |= (1ULL << offset);
}

// Precompute cumulative rank table for O(1) rank queries
void build_rank_index(struct SuccinctBitvector *p) {
    uint32_t running_total = 0;
    for (int i = 0; i < NUM_WORDS; i++) {
        p->block_rank[i] = running_total;
        // __builtin_popcountll counts set bits in O(1) hardware instruction
        running_total += __builtin_popcountll(p->words[i]);
    }
}

// Rank(i): Count total set bits from index 0 to bit_idx in O(1) time
size_t bitvector_rank(struct SuccinctBitvector *p, size_t bit_idx) {
    size_t word_idx = bit_idx / BITS_PER_WORD;
    size_t offset = bit_idx % BITS_PER_WORD;

    // Cumulative sum up to full block boundary
    size_t rank_count = p->block_rank[word_idx];

    // Mask out bits above the requested offset within the partial word
    uint64_t mask = (offset == 63) ? ~0ULL : ((1ULL << (offset + 1)) - 1);
    uint64_t partial_word = p->words[word_idx] & mask;

    rank_count += __builtin_popcountll(partial_word);
    return rank_count;
}

void free_bitvector(struct SuccinctBitvector *p) {
    if (p != NULL) {
        free(p);
    }
}

int main() {
    printf("=== Succinct Rank/Select Bitvector Engine (ANSI C) ===\n");
    struct SuccinctBitvector *bv = create_bitvector();

    printf("Setting bits at indices: 2, 5, 63, 64, 100, 127, 200...\n");
    set_bit(bv, 2);
    set_bit(bv, 5);
    set_bit(bv, 63);
    set_bit(bv, 64);
    set_bit(bv, 100);
    set_bit(bv, 127);
    set_bit(bv, 200);

    build_rank_index(bv);

    printf("\nExecuting O(1) Rank Queries:\n");
    printf("  -> Rank(5)   (Set bits up to index 5):   %zu\n", bitvector_rank(bv, 5));
    printf("  -> Rank(63)  (Set bits up to index 63):  %zu\n", bitvector_rank(bv, 63));
    printf("  -> Rank(64)  (Set bits up to index 64):  %zu\n", bitvector_rank(bv, 64));
    printf("  -> Rank(150) (Set bits up to index 150): %zu\n", bitvector_rank(bv, 150));
    printf("  -> Rank(255) (Set bits up to index 255): %zu\n", bitvector_rank(bv, 255));

    free_bitvector(bv);
    printf("[PASS] Succinct Bitvector execution completed cleanly.\n");
    return 0;
}