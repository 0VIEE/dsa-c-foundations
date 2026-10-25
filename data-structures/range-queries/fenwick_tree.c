#include <stdio.h>
#include <stdlib.h>

struct FenwickTree {
    int size;
    int *tree;
};

// Create a 1-indexed Fenwick Tree
struct FenwickTree* create_bit(int n) {
    struct FenwickTree *p = (struct FenwickTree*)malloc(sizeof(struct FenwickTree));
    if (p == NULL) return NULL;

    p->size = n;
    p->tree = (int*)calloc(n + 1, sizeof(int));
    return p;
}

// Add delta to index i (1-indexed)
void update(struct FenwickTree *bit, int i, int delta) {
    while (i <= bit->size) {
        bit->tree[i] += delta;
        i += (i & (-i)); // Jump to next node responsible for this range
    }
}

// Prefix sum from 1 up to index i
int query(struct FenwickTree *bit, int i) {
    int sum = 0;
    while (i > 0) {
        sum += bit->tree[i];
        i -= (i & (-i)); // Drop lowest set bit to jump to previous segment
    }
    return sum;
}

// Range sum from index L to R (inclusive)
int range_query(struct FenwickTree *bit, int L, int R) {
    return query(bit, R) - query(bit, L - 1);
}

void free_bit(struct FenwickTree *bit) {
    if (bit != NULL) {
        if (bit->tree != NULL) {
            free(bit->tree);
        }
        free(bit);
    }
}

int main() {
    /*
        Original Array (1-indexed): {3, 2, -1, 6, 5, 4, -3, 3, 7, 2, 3}
        Indices:                     1  2   3  4  5  6   7  8  9 10 11
    */
    int arr[] = {0, 3, 2, -1, 6, 5, 4, -3, 3, 7, 2, 3};
    int n = sizeof(arr) / sizeof(arr[0]) - 1;

    struct FenwickTree *bit = create_bit(n);

    // Build Fenwick Tree by point updating each element
    for (int i = 1; i <= n; i++) {
        update(bit, i, arr[i]);
    }

    printf("=== Fenwick Tree (Binary Indexed Tree) ===\n");
    printf("Prefix Sum up to index 5: %d\n", query(bit, 5));          // 3 + 2 - 1 + 6 + 5 = 15
    printf("Range Sum [3, 6]:         %d\n", range_query(bit, 3, 6)); // -1 + 6 + 5 + 4 = 14
    printf("Range Sum [1, 11]:        %d\n", range_query(bit, 1, 11)); // Total = 31

    // Update index 3: arr[3] was -1, add +6 (new value = 5)
    printf("\nUpdating index 3 by adding +6...\n");
    update(bit, 3, 6);

    printf("New Range Sum [3, 6]:     %d\n", range_query(bit, 3, 6)); // Expected: 14 + 6 = 20
    printf("New Prefix Sum up to 5:   %d\n", query(bit, 5));          // Expected: 15 + 6 = 21

    free_bit(bit);
    return 0;
}