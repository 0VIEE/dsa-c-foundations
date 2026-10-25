#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_N 16
#define MAX_TOUR (2 * MAX_N)

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

int euler[MAX_TOUR];
int depth_arr[MAX_TOUR];
int first_occ[MAX_N];
int tour_idx = 0;

// DFS to record Euler Tour and node depths
void dfs_euler(struct Tree *t, int u, int p, int d) {
    first_occ[u] = tour_idx;
    euler[tour_idx] = u;
    depth_arr[tour_idx] = d;
    tour_idx++;

    struct AdjListNode *curr = t->adj[u];
    while (curr != NULL) {
        int v = curr->dest;
        if (v != p) {
            dfs_euler(t, v, u, d + 1);
            // Record step returning back to current node
            euler[tour_idx] = u;
            depth_arr[tour_idx] = d;
            tour_idx++;
        }
        curr = curr->link;
    }
}

// Segment Tree for Range Minimum Query (storing index of minimum depth)
int seg_tree[4 * MAX_TOUR];

void build_rmq_tree(int node, int start, int end) {
    if (start == end) {
        seg_tree[node] = start;
        return;
    }
    int mid = (start + end) / 2;
    build_rmq_tree(2 * node, start, mid);
    build_rmq_tree(2 * node + 1, mid + 1, end);

    int idx_left = seg_tree[2 * node];
    int idx_right = seg_tree[2 * node + 1];
    seg_tree[node] = (depth_arr[idx_left] <= depth_arr[idx_right]) ? idx_left : idx_right;
}

int query_rmq(int node, int start, int end, int l, int r) {
    if (r < start || end < l) return -1;
    if (l <= start && end <= r) return seg_tree[node];

    int mid = (start + end) / 2;
    int idx_left = query_rmq(2 * node, start, mid, l, r);
    int idx_right = query_rmq(2 * node + 1, mid + 1, end, l, r);

    if (idx_left == -1) return idx_right;
    if (idx_right == -1) return idx_left;

    return (depth_arr[idx_left] <= depth_arr[idx_right]) ? idx_left : idx_right;
}

int query_lca(int u, int v) {
    int l = first_occ[u];
    int r = first_occ[v];
    if (l > r) {
        int temp = l;
        l = r;
        r = temp;
    }
    int min_depth_idx = query_rmq(1, 0, tour_idx - 1, l, r);
    return euler[min_depth_idx];
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

    tour_idx = 0;
    dfs_euler(t, 0, -1, 0);
    build_rmq_tree(1, 0, tour_idx - 1);

    printf("=== LCA via Euler Tour + RMQ Segment Tree ===\n");
    printf("Euler Tour Steps: %d\n", tour_idx);
    printf("LCA(3, 7): Node %d (Expected: 1)\n", query_lca(3, 7));
    printf("LCA(7, 2): Node %d (Expected: 0)\n", query_lca(7, 2));
    printf("LCA(5, 6): Node %d (Expected: 2)\n", query_lca(5, 6));
    printf("LCA(4, 7): Node %d (Expected: 4)\n", query_lca(4, 7));

    free_tree(t);
    return 0;
}