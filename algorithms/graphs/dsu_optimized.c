#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct DSU {
    int *parent;
    int *rank;
    int size;
};

struct DSU* create_dsu(int n) {
    struct DSU *dsu = (struct DSU*)malloc(sizeof(struct DSU));
    dsu->size = n;
    dsu->parent = (int*)malloc(n * sizeof(int));
    dsu->rank = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        dsu->parent[i] = i; // Every element is its own parent
        dsu->rank[i] = 0;   // Initial tree depth is 0
    }
    return dsu;
}

// Find with Path Compression: O(alpha(N))
int find_set(struct DSU *dsu, int i) {
    if (dsu->parent[i] != i) {
        dsu->parent[i] = find_set(dsu, dsu->parent[i]);
    }
    return dsu->parent[i];
}

// Union by Rank: O(alpha(N))
bool union_sets(struct DSU *dsu, int x, int y) {
    int root_x = find_set(dsu, x);
    int root_y = find_set(dsu, y);

    if (root_x == root_y) {
        return false; // Already in the same set (cycle/redundant edge)
    }

    if (dsu->rank[root_x] < dsu->rank[root_y]) {
        dsu->parent[root_x] = root_y;
    } else if (dsu->rank[root_x] > dsu->rank[root_y]) {
        dsu->parent[root_y] = root_x;
    } else {
        dsu->parent[root_y] = root_x;
        dsu->rank[root_x]++;
    }

    return true;
}

void free_dsu(struct DSU *dsu) {
    if (dsu) {
        free(dsu->parent);
        free(dsu->rank);
        free(dsu);
    }
}

int main() {
    int n = 6;
    struct DSU *dsu = create_dsu(n);

    // Connect components: (0-1), (1-2), (3-4)
    union_sets(dsu, 0, 1);
    union_sets(dsu, 1, 2);
    union_sets(dsu, 3, 4);

    printf("=== Disjoint Set Union (Rank + Path Compression) ===\n");
    printf("Is 0 connected to 2? %s\n", (find_set(dsu, 0) == find_set(dsu, 2)) ? "YES" : "NO");
    printf("Is 0 connected to 4? %s\n", (find_set(dsu, 0) == find_set(dsu, 4)) ? "YES" : "NO");

    // Add redundant edge to trigger cycle detection
    bool added = union_sets(dsu, 0, 2);
    printf("Adding edge 0-2: %s\n", added ? "Merged" : "Redundant (Cycle detected!)");

    free_dsu(dsu);
    return 0;
}