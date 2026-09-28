#include <stdio.h>
#include <stdlib.h>

#define MAX_VAL 100

struct FenwickTree {
    int size;
    int *tree;
};

// Create a Fenwick Tree of size max_val (1-based index)
struct FenwickTree* create_fenwick(int size) {
    struct FenwickTree *bit = (struct FenwickTree*)malloc(sizeof(struct FenwickTree));
    bit->size = size;
    bit->tree = (int*)calloc(size + 1, sizeof(int));
    return bit;
}

// Add delta to index i (i must be >= 1)
void update(struct FenwickTree *bit, int i, int delta) {
    while (i <= bit->size) {
        bit->tree[i] += delta;
        i += (i & -i); // Add least significant set bit
    }
}

// Query prefix sum from 1 to i
int query(struct FenwickTree *bit, int i) {
    int sum = 0;
    while (i > 0) {
        sum += bit->tree[i];
        i -= (i & -i); // Subtract least significant set bit
    }
    return sum;
}

void free_fenwick(struct FenwickTree *bit) {
    free(bit->tree);
    free(bit);
}

// Count inversions: pairs (i, j) where i < j and arr[i] > arr[j]
long long count_inversions(int arr[], int n) {
    // Determine maximum value for BIT size
    int max_elem = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] > max_elem) {
            max_elem = arr[i];
        }
    }

    struct FenwickTree *bit = create_fenwick(max_elem);
    long long inv_count = 0;

    // Traverse from right to left
    // Elements to the right of current element that are strictly smaller form inversions
    for (int i = n - 1; i >= 0; i--) {
        // Query count of elements strictly smaller than arr[i] already inserted
        inv_count += query(bit, arr[i] - 1);

        // Insert current element frequency into BIT
        update(bit, arr[i], 1);
    }

    free_fenwick(bit);
    return inv_count;
}

int main() {
    /*
        Array: [8, 4, 2, 1]
        Inversions:
        (8,4), (8,2), (8,1)
        (4,2), (4,1)
        (2,1)
        Total Inversions = 6
    */
    int arr[] = {8, 4, 2, 1};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("=== Fenwick Tree (Binary Indexed Tree): Inversion Counter ===\n");
    printf("Input Array: [ ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("]\n");

    long long result = count_inversions(arr, n);
    printf("Calculated Inversions: %lld (Expected: 6)\n", result);

    // Test Case 2: Partially sorted array [3, 1, 2] -> (3,1), (3,2) = 2 inversions
    int arr2[] = {3, 1, 2};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    printf("\nTest Case 2: [ 3 1 2 ]\n");
    printf("Calculated Inversions: %lld (Expected: 2)\n", count_inversions(arr2, n2));

    return 0;
}