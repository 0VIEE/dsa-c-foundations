#include <stdio.h>
#include <stdlib.h>

#define MAX_N 20

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
    // Edge u -> v
    struct AdjListNode *p = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    p->dest = v;
    p->link = t->adj[u];
    t->adj[u] = p;

    // Edge v -> u
    p = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    p->dest = u;
    p->link = t->adj[v];
    t->adj[v] = p;
}

int min_val(int a, int b) {
    return (a < b) ? a : b;
}

int dp[MAX_N][2];

// DFS computing min vertex cover values
void dfs_vertex_cover(struct Tree *t, int u, int parent) {
    dp[u][0] = 0; // u not taken
    dp[u][1] = 1; // u taken (counts as 1 node)

    struct AdjListNode *p = t->adj[u];
    while (p != NULL) {
        int v = p->dest;
        if (v != parent) {
            dfs_vertex_cover(t, v, u);

            // If u is not taken, child v MUST be taken
            dp[u][0] += dp[v][1];

            // If u is taken, child v can either be taken or not taken
            dp[u][1] += min_val(dp[v][0], dp[v][1]);
        }
        p = p->link;
    }
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
        Sample Tree (7 nodes: 0 to 6):
               0
             /   \
            1     2
           / \   / \
          3   4 5   6

        Optimal Cover: Nodes {0, 1, 2} covers all edges -> Size = 3
        Alternative Cover: Nodes {1, 2} covers edges (1,3),(1,4),(2,5),(2,6), 
        plus node 0 to cover (0,1),(0,2) -> Size = 3
    */
    int n = 7;
    struct Tree *t = create_tree(n);

    add_edge(t, 0, 1);
    add_edge(t, 0, 2);
    add_edge(t, 1, 3);
    add_edge(t, 1, 4);
    add_edge(t, 2, 5);
    add_edge(t, 2, 6);

    dfs_vertex_cover(t, 0, -1);

    int min_cover = min_val(dp[0][0], dp[0][1]);

    printf("=== Minimum Vertex Cover on Tree via DP ===\n");
    printf("Total Nodes: %d\n", n);
    printf("dp[root=0][exclude]: %d\n", dp[0][0]);
    printf("dp[root=0][include]: %d\n", dp[0][1]);
    printf("Minimum Vertex Cover Size: %d (Expected: 3)\n", min_cover);

    free_tree(t);
    return 0;
}