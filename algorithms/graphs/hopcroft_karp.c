#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 32
#define NIL 0
#define INF 1e9

struct AdjNode {
    int dest;
    struct AdjNode *link;
};

struct BipartiteGraph {
    int n1, n2; // Size of partitions U and V
    struct AdjNode *adj[MAX_NODES];
};

struct BipartiteGraph* create_graph(int n1, int n2) {
    struct BipartiteGraph *g = (struct BipartiteGraph*)malloc(sizeof(struct BipartiteGraph));
    g->n1 = n1;
    g->n2 = n2;
    for (int i = 0; i < MAX_NODES; i++) {
        g->adj[i] = NULL;
    }
    return g;
}

void add_edge(struct BipartiteGraph *g, int u, int v) {
    struct AdjNode *p = (struct AdjNode*)malloc(sizeof(struct AdjNode));
    p->dest = v;
    p->link = g->adj[u];
    g->adj[u] = p;
}

int pair_u[MAX_NODES];
int pair_v[MAX_NODES];
int dist[MAX_NODES];

int bfs(struct BipartiteGraph *g) {
    int queue[MAX_NODES];
    int head = 0, tail = 0;

    for (int u = 1; u <= g->n1; u++) {
        if (pair_u[u] == NIL) {
            dist[u] = 0;
            queue[tail++] = u;
        } else {
            dist[u] = INF;
        }
    }
    dist[NIL] = INF;

    while (head < tail) {
        int u = queue[head++];
        if (dist[u] < dist[NIL]) {
            struct AdjNode *p = g->adj[u];
            while (p != NULL) {
                int v = p->dest;
                if (dist[pair_v[v]] == INF) {
                    dist[pair_v[v]] = dist[u] + 1;
                    queue[tail++] = pair_v[v];
                }
                p = p->link;
            }
        }
    }
    return dist[NIL] != INF;
}

int dfs(struct BipartiteGraph *g, int u) {
    if (u != NIL) {
        struct AdjNode *p = g->adj[u];
        while (p != NULL) {
            int v = p->dest;
            if (dist[pair_v[v]] == dist[u] + 1) {
                if (dfs(g, pair_v[v])) {
                    pair_v[v] = u;
                    pair_u[u] = v;
                    return 1;
                }
            }
            p = p->link;
        }
        dist[u] = INF;
        return 0;
    }
    return 1;
}

int hopcroft_karp(struct BipartiteGraph *g) {
    for (int i = 0; i < MAX_NODES; i++) {
        pair_u[i] = NIL;
        pair_v[i] = NIL;
    }
    
    int max_matching = 0;
    while (bfs(g)) {
        for (int u = 1; u <= g->n1; u++) {
            if (pair_u[u] == NIL && dfs(g, u)) {
                max_matching++;
            }
        }
    }
    return max_matching;
}

void free_graph(struct BipartiteGraph *g) {
    for (int i = 0; i < MAX_NODES; i++) {
        struct AdjNode *p = g->adj[i];
        while (p != NULL) {
            struct AdjNode *temp = p;
            p = p->link;
            free(temp);
        }
    }
    free(g);
}

int main() {
    /*
        Bipartite Graph:
        Set U: {1, 2, 3}
        Set V: {1, 2, 3} (represented as indices in V)
        Edges: 1->1, 1->2, 2->1, 3->2, 3->3
        Expected Maximum Matching: 3
    */
    struct BipartiteGraph *g = create_graph(3, 3);

    add_edge(g, 1, 1);
    add_edge(g, 1, 2);
    add_edge(g, 2, 1);
    add_edge(g, 3, 2);
    add_edge(g, 3, 3);

    int matching = hopcroft_karp(g);

    printf("=== Hopcroft-Karp Maximum Bipartite Matching ===\n");
    printf("Maximum Matching Size: %d (Expected: 3)\n", matching);

    free_graph(g);
    return 0;
}