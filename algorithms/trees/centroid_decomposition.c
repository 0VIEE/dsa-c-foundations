#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_N 16

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
    struct AdjListNode *p = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    p->dest = v;
    p->link = t->adj[u];
    t->adj[u] = p;

    p = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    p->dest = u;
    p->link = t->adj[v];
    t->adj[v] = p;
}

int sz[MAX_N];
bool removed[MAX_N];
int centroid_parent[MAX_N];

// DFS 1: Subtree sizes inside current active component
void get_subtree_sizes(struct Tree *t, int u, int p) {
    sz[u] = 1;
    struct AdjListNode *curr = t->adj[u];
    while (curr != NULL) {
        int v = curr->dest;
        if (v != p && !removed[v]) {
            get_subtree_sizes(t, v, u);
            sz[u] += sz[v];
        }
        curr = curr->link;
    }
}

// DFS 2: Locate centroid within active component
int get_centroid(struct Tree *t, int u, int p, int total_nodes) {
    struct AdjListNode *curr = t->adj[u];
    while (curr != NULL) {
        int v = curr->dest;
        if (v != p && !removed[v]) {
            if (sz[v] > total_nodes / 2) {
                return get_centroid(t, v, u, total_nodes);
            }
        }
        curr = curr->link;
    }
    return u;
}

// Recursive Centroid Tree builder
int decompose_tree(struct Tree *t, int u, int p) {
    get_subtree_sizes(t, u, -1);
    int total_nodes = sz[u];
    int c = get_centroid(t, u, -1, total_nodes);

    removed[c] = true;
    centroid_parent[c] = p;

    // Recurse on remaining connected subtrees
    struct AdjListNode *curr = t->adj[c];
    while (curr != NULL) {
        int v = curr->dest;
        if (!removed[v]) {
            decompose_tree(t, v, c);
        }
        curr = curr->link;
    }

    return c;
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
        Sample Tree with 8 nodes (0 to 7):
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

    for (int i = 0; i < n; i++) {
        removed[i] = false;
        centroid_parent[i] = -1;
    }

    int centroid_root = decompose_tree(t, 0, -1);

    printf("=== Centroid Tree Decomposition Results ===\n");
    printf("Root of Centroid Tree: Node %d\n", centroid_root);
    printf("Node\tCentroid Parent\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\n", i, centroid_parent[i]);
    }

    free_tree(t);
    return 0;
}