#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX

int min(int a, int b) {
    return (a < b) ? a : b;
}

// Build the Segment Tree
void build_tree(int arr[], int tree[], int node, int start, int end) {
    if (start == end) {
        tree[node] = arr[start];
        return;
    }

    int mid = start + (end - start) / 2;
    int left_node = 2 * node + 1;
    int right_node = 2 * node + 2;

    build_tree(arr, tree, left_node, start, mid);
    build_tree(arr, tree, right_node, mid + 1, end);

    tree[node] = min(tree[left_node], tree[right_node]);
}

// Range Minimum Query for range [qs, qe]
int query_min(int tree[], int node, int start, int end, int qs, int qe) {
    // Case 1: Out of range (No overlap)
    if (qs > end || qe < start) {
        return INF;
    }

    // Case 2: Complete overlap
    if (qs <= start && end <= qe) {
        return tree[node];
    }

    // Case 3: Partial overlap
    int mid = start + (end - start) / 2;
    int left_min = query_min(tree, 2 * node + 1, start, mid, qs, qe);
    int right_min = query_min(tree, 2 * node + 2, mid + 1, end, qs, qe);

    return min(left_min, right_min);
}

// Point Update: arr[idx] = val
void update_value(int tree[], int node, int start, int end, int idx, int val) {
    if (start == end) {
        tree[node] = val;
        return;
    }

    int mid = start + (end - start) / 2;
    int left_node = 2 * node + 1;
    int right_node = 2 * node + 2;

    if (idx <= mid) {
        update_value(tree, left_node, start, mid, idx, val);
    } else {
        update_value(tree, right_node, mid + 1, end, idx, val);
    }

    tree[node] = min(tree[left_node], tree[right_node]);
}

int main() {
    /*
        Input Array: {2, 5, 1, 4, 9, 3}
        Indices:      0  1  2  3  4  5
    */
    int arr[] = {2, 5, 1, 4, 9, 3};
    int n = sizeof(arr) / sizeof(arr[0]);

    // Segment tree array requires up to 4 * n space
    int *tree = (int*)malloc(4 * n * sizeof(int));
    if (tree == NULL) return -1;

    build_tree(arr, tree, 0, 0, n - 1);

    printf("=== Segment Tree: Range Minimum Query (RMQ) ===\n");
    printf("Array: {2, 5, 1, 4, 9, 3}\n");

    printf("Min in range [1, 4]: %d\n", query_min(tree, 0, 0, n - 1, 1, 4)); // Expected: 1
    printf("Min in range [3, 5]: %d\n", query_min(tree, 0, 0, n - 1, 3, 5)); // Expected: 3
    printf("Min in range [0, 1]: %d\n", query_min(tree, 0, 0, n - 1, 0, 1)); // Expected: 2

    // Update arr[2] from 1 -> 10
    printf("\nUpdating index 2 to value 10...\n");
    update_value(tree, 0, 0, n - 1, 2, 10);

    printf("New Min in range [1, 4]: %d\n", query_min(tree, 0, 0, n - 1, 1, 4)); // Expected: 4
    printf("New Min in range [0, 5]: %d\n", query_min(tree, 0, 0, n - 1, 0, 5)); // Expected: 2

    free(tree);
    return 0;
}