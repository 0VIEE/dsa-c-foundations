#include <stdio.h>
#include <stdlib.h>

struct FenwickTree {
    int size;
    int *tree;
};

struct FenwickTree *create_bit(int n) {
    if (n <= 0) {
        return NULL;
    }

    struct FenwickTree *bit = (struct FenwickTree *)malloc(sizeof(*bit));
    if (bit == NULL) {
        return NULL;
    }

    bit->size = n;
    bit->tree = (int *)calloc((size_t)n + 1, sizeof(*bit->tree));
    if (bit->tree == NULL) {
        free(bit);
        return NULL;
    }
    return bit;
}

/* Add delta at a 1-based index. Invalid indices are ignored. */
void update(struct FenwickTree *bit, int i, int delta) {
    if (bit == NULL || bit->tree == NULL || i < 1 || i > bit->size) {
        return;
    }

    while (i <= bit->size) {
        bit->tree[i] += delta;
        i += i & -i;
    }
}

/* Return the sum from index 1 through i; values above size are clamped. */
int query(struct FenwickTree *bit, int i) {
    if (bit == NULL || bit->tree == NULL || i <= 0) {
        return 0;
    }
    if (i > bit->size) {
        i = bit->size;
    }

    int sum = 0;
    while (i > 0) {
        sum += bit->tree[i];
        i -= i & -i;
    }
    return sum;
}

/* Return the inclusive range sum; invalid ranges return zero. */
int range_query(struct FenwickTree *bit, int left, int right) {
    if (bit == NULL || bit->tree == NULL ||
        left < 1 || right > bit->size || left > right) {
        return 0;
    }
    return query(bit, right) - query(bit, left - 1);
}

void free_bit(struct FenwickTree *bit) {
    if (bit == NULL) {
        return;
    }
    free(bit->tree);
    free(bit);
}

#ifndef FENWICK_TREE_NO_MAIN
int main(void) {
    /*
        Original Array (1-indexed): {3, 2, -1, 6, 5, 4, -3, 3, 7, 2, 3}
        Indices:                     1  2   3  4  5  6   7  8  9 10 11
    */
    int arr[] = {0, 3, 2, -1, 6, 5, 4, -3, 3, 7, 2, 3};
    int n = (int)(sizeof(arr) / sizeof(arr[0])) - 1;

    struct FenwickTree *bit = create_bit(n);
    if (bit == NULL) {
        fputs("Failed to allocate the Fenwick tree.\n", stderr);
        return EXIT_FAILURE;
    }

    for (int i = 1; i <= n; i++) {
        update(bit, i, arr[i]);
    }

    printf("=== Fenwick Tree (Binary Indexed Tree) ===\n");
    printf("Prefix Sum up to index 5: %d\n", query(bit, 5));
    printf("Range Sum [3, 6]:         %d\n", range_query(bit, 3, 6));
    printf("Range Sum [1, 11]:        %d\n", range_query(bit, 1, 11));

    printf("\nUpdating index 3 by adding +6...\n");
    update(bit, 3, 6);

    printf("New Range Sum [3, 6]:     %d\n", range_query(bit, 3, 6));
    printf("New Prefix Sum up to 5:   %d\n", query(bit, 5));

    free_bit(bit);
    return EXIT_SUCCESS;
}
#endif
