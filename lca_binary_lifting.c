#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_N 10
#define LOGN 4 // ceil(log2(10))

struct AdjListNode {
    int dest;
    struct AdjListNode *link;
};

struct Tree {
    int n;
    struct AdjListNode *adj[MAX_N];
};

struct Tree* create_tree(int n) {
    struct Tree *t = (struct Tree*)malloc(sizeof(struct Tree));
    t->n = n;
    for (int i = 0; i < n; i++) {
        t->adj[i] = NULL;
    }
    return t;
}

void add_edge(struct Tree *t, int u, int v) {
    // Tree edge: u <-> v
    struct AdjListNode *p = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    p->dest = v;
    p->link = t->adj[u];
    t->adj[u] = p;

    p = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    p->dest = u;
    p->link = t->adj[v];
    t->adj[v] = p;
}

int depth[MAX_N];
int up[MAX_N][LOGN];

// DFS to compute depths and 2^0 ancestors (direct parent)
void dfs_tree(struct Tree *t, int u, int p, int d) {
    depth[u] = d;
    up[u][0] = p;

    struct AdjListNode *curr = t->adj[u];
    while (curr != NULL) {
        int v = curr->dest;
        if (v != p) {
            dfs_tree(t, v, u, d + 1);
        }
        curr = curr->link;
    }
}

// Precompute binary lifting table up[u][k]
void preprocess_lca(struct Tree *t, int root) {
    dfs_tree(t, root, root, 0);

    for (int k = 1; k < LOGN; k++) {
        for (int i = 0; i < t->n; i++) {
            up[i][k] = up[up[i][k - 1]][k - 1];
        }
    }
}

// Compute LCA in O(log N)
int query_lca(int u, int v) {
    // Ensure u is deeper or at same level as v
    if (depth[u] < depth[v]) {
        int temp = u;
        u = v;
        v = temp;
    }

    // Step 1: Lift u to the same depth as v
    int diff = depth[u] - depth[v];
    for (int k = 0; k < LOGN; k++) {
        if ((diff >> k) & 1) {
            u = up[u][k];
        }
    }

    if (u == v) return u;

    // Step 2: Lift both u and v simultaneously
    for (int k = LOGN - 1; k >= 0; k--) {
        if (up[u][k] != up[v][k]) {
            u = up[u][k];
            v = up[v][k];
        }
    }

    return up[u][0];
}

void free_tree(struct Tree *t) {
    for (int i = 0; i < t->n; i++) {
        struct AdjListNode *p = t->adj[i];
        while (p != NULL) {
            struct AdjListNode *temp = p;
            p = p->link;
            free(temp);
        }
    }
    free(t);
}

int main() {
    /*
        Sample Tree rooted at 0:
              0
            /   \
           1     2
          / \   / \
         3   4 5   6
            /
           7
    */
    int n = 8;
    struct Tree *t = create_tree(n);

    add_edge(t, 0, 1);
    add_edge(t, 0, 2);
    add_edge(t, 1, 3);
    add_edge(t, 1, 4);
    add_edge(t, 2, 5);
    add_edge(t, 2, 6);
    add_edge(t, 4, 7);

    preprocess_lca(t, 0);

    printf("=== Lowest Common Ancestor (Binary Lifting) ===\n");
    printf("LCA(3, 7): %d (Expected: 1)\n", query_lca(3, 7));
    printf("LCA(7, 2): %d (Expected: 0)\n", query_lca(7, 2));
    printf("LCA(5, 6): %d (Expected: 2)\n", query_lca(5, 6));
    printf("LCA(4, 7): %d (Expected: 4)\n", query_lca(4, 7));

    free_tree(t);
    return 0;
}