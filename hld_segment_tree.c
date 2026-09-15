#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_N 10

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

int parent_node[MAX_N];
int depth[MAX_N];
int sz[MAX_N];
int heavy[MAX_N];
int head[MAX_N];
int pos[MAX_N];
int cur_pos = 0;

int node_val[MAX_N];
int base_arr[MAX_N];
int seg_tree[4 * MAX_N];

int max(int a, int b) {
    return (a > b) ? a : b;
}

// DFS 1: Subtree sizes, parent, depth, and heavy child identification
void dfs_sz(struct Tree *t, int u, int p, int d) {
    sz[u] = 1;
    parent_node[u] = p;
    depth[u] = d;
    heavy[u] = -1;
    int max_child_sz = 0;

    struct AdjListNode *p_node = t->adj[u];
    while (p_node != NULL) {
        int v = p_node->dest;
        if (v != p) {
            dfs_sz(t, v, u, d + 1);
            sz[u] += sz[v];
            if (sz[v] > max_child_sz) {
                max_child_sz = sz[v];
                heavy[u] = v;
            }
        }
        p_node = p_node->link;
    }
}

// DFS 2: Flatten tree into contiguous ranges on heavy paths
void dfs_hld(struct Tree *t, int u, int h) {
    head[u] = h;
    pos[u] = cur_pos;
    base_arr[cur_pos++] = node_val[u];

    if (heavy[u] != -1) {
        dfs_hld(t, heavy[u], h);
    }

    struct AdjListNode *p_node = t->adj[u];
    while (p_node != NULL) {
        int v = p_node->dest;
        if (v != parent_node[u] && v != heavy[u]) {
            dfs_hld(t, v, v);
        }
        p_node = p_node->link;
    }
}

// Segment Tree for Range Maximum Queries
void build_seg_tree(int node, int start, int end) {
    if (start == end) {
        seg_tree[node] = base_arr[start];
        return;
    }
    int mid = (start + end) / 2;
    build_seg_tree(2 * node, start, mid);
    build_seg_tree(2 * node + 1, mid + 1, end);
    seg_tree[node] = max(seg_tree[2 * node], seg_tree[2 * node + 1]);
}

int query_seg_tree(int node, int start, int end, int l, int r) {
    if (r < start || end < l) return -1;
    if (l <= start && end <= r) return seg_tree[node];

    int mid = (start + end) / 2;
    return max(query_seg_tree(2 * node, start, mid, l, r),
               query_seg_tree(2 * node + 1, mid + 1, end, l, r));
}

// Query maximum node value on the path between u and v
int query_path_max(int u, int v, int n) {
    int ans = -1;
    while (head[u] != head[v]) {
        if (depth[head[u]] < depth[head[v]]) {
            int temp = u;
            u = v;
            v = temp;
        }
        ans = max(ans, query_seg_tree(1, 0, n - 1, pos[head[u]], pos[u]));
        u = parent_node[head[u]];
    }

    if (depth[u] > depth[v]) {
        int temp = u;
        u = v;
        v = temp;
    }
    ans = max(ans, query_seg_tree(1, 0, n - 1, pos[u], pos[v]));
    return ans;
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
        Sample Tree rooted at 0 with node values:
              0 (val: 5)
            /   \
        1 (12)   2 (9)
        / \       \
      3(8) 4(20)   5 (3)
          /
         6 (15)
    */
    int n = 7;
    struct Tree *t = create_tree(n);

    node_val[0] = 5;
    node_val[1] = 12;
    node_val[2] = 9;
    node_val[3] = 8;
    node_val[4] = 20;
    node_val[5] = 3;
    node_val[6] = 15;

    add_edge(t, 0, 1);
    add_edge(t, 0, 2);
    add_edge(t, 1, 3);
    add_edge(t, 1, 4);
    add_edge(t, 2, 5);
    add_edge(t, 4, 6);

    cur_pos = 0;
    dfs_sz(t, 0, 0, 0);
    dfs_hld(t, 0, 0);
    build_seg_tree(1, 0, n - 1);

    printf("=== HLD + Segment Tree Path Queries ===\n");
    printf("Max val on path (3 -> 6): %d (Expected: 20)\n", query_path_max(3, 6, n));
    printf("Max val on path (6 -> 5): %d (Expected: 20)\n", query_path_max(6, 5, n));
    printf("Max val on path (2 -> 5): %d (Expected: 9)\n", query_path_max(2, 5, n));

    free_tree(t);
    return 0;
}