#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 32
#define INF 1000000000

struct Edge {
    int to;
    int capacity;
    int flow;
    struct Edge *rev; // Paired residual edge
    struct Edge *link;
};

struct Graph {
    int n;
    struct Edge *adj[MAX_NODES];
};

struct Graph* create_graph(int n) {
    if (n <= 0 || n > MAX_NODES) return NULL;

    struct Graph *g = (struct Graph*)malloc(sizeof(struct Graph));
    if (g == NULL) return NULL;

    g->n = n;
    for (int i = 0; i < n; i++) {
        g->adj[i] = NULL;
    }
    return g;
}

// Add a directed edge and its paired reverse residual edge.
int add_edge(struct Graph *g, int u, int v, int cap) {
    if (g == NULL || u < 0 || u >= g->n || v < 0 || v >= g->n ||
        u == v || cap < 0) {
        return 0;
    }

    struct Edge *a = (struct Edge*)malloc(sizeof(struct Edge));
    struct Edge *b = (struct Edge*)malloc(sizeof(struct Edge));
    if (a == NULL || b == NULL) {
        free(a);
        free(b);
        return 0;
    }

    a->to = v;
    a->capacity = cap;
    a->flow = 0;
    a->rev = b;
    a->link = g->adj[u];

    b->to = u;
    b->capacity = 0;
    b->flow = 0;
    b->rev = a;
    b->link = g->adj[v];

    g->adj[u] = a;
    g->adj[v] = b;
    return 1;
}

int level[MAX_NODES];
struct Edge* ptr[MAX_NODES]; // Dinic's pointer optimization for DFS

int bfs(struct Graph *g, int s, int t) {
    for (int i = 0; i < g->n; i++) level[i] = -1;
    level[s] = 0;

    int queue[MAX_NODES];
    int head = 0, tail = 0;
    queue[tail++] = s;

    while (head < tail) {
        int u = queue[head++];
        struct Edge *e = g->adj[u];
        while (e != NULL) {
            if (e->capacity - e->flow > 0 && level[e->to] == -1) {
                level[e->to] = level[u] + 1;
                queue[tail++] = e->to;
            }
            e = e->link;
        }
    }
    return level[t] != -1;
}

int dfs(struct Graph *g, int u, int t, int pushed) {
    if (pushed == 0) return 0;
    if (u == t) return pushed;

    for (struct Edge **e_ptr = &ptr[u]; *e_ptr != NULL; e_ptr = &((*e_ptr)->link)) {
        struct Edge *e = *e_ptr;
        int v = e->to;

        if (level[u] + 1 != level[v] || e->capacity - e->flow == 0) continue;

        int tr = dfs(g, v, t, pushed < (e->capacity - e->flow) ? pushed : (e->capacity - e->flow));
        if (tr == 0) continue;

        e->flow += tr;
        e->rev->flow -= tr;
        return tr;
    }
    return 0;
}

int dinic_max_flow(struct Graph *g, int s, int t) {
    if (g == NULL || s < 0 || s >= g->n || t < 0 || t >= g->n || s == t) {
        return 0;
    }

    int flow = 0;
    while (bfs(g, s, t)) {
        for (int i = 0; i < g->n; i++) {
            ptr[i] = g->adj[i];
        }
        for (;;) {
            int pushed = dfs(g, s, t, INF);
            if (pushed == 0) break;
            flow += pushed;
        }
    }
    return flow;
}

void free_graph(struct Graph *g) {
    if (g == NULL) return;

    for (int i = 0; i < g->n; i++) {
        struct Edge *e = g->adj[i];
        while (e != NULL) {
            struct Edge *temp = e;
            e = e->link;
            free(temp);
        }
    }
    free(g);
}

int main() {
    /*
        Network Flow Graph:
        Source = 0, Sink = 5
        Edges with capacities:
        0 -> 1 (10), 0 -> 2 (10)
        1 -> 3 (4),  1 -> 4 (8), 1 -> 2 (2)
        2 -> 4 (9),  2 -> 5 (10)
        3 -> 5 (10)
        4 -> 3 (6),  4 -> 5 (10)
        Expected Max Flow: 19
    */
    int n = 6;
    struct Graph *g = create_graph(n);
    if (g == NULL) {
        fprintf(stderr, "Failed to create graph.\n");
        return EXIT_FAILURE;
    }

    if (!add_edge(g, 0, 1, 10) ||
        !add_edge(g, 0, 2, 10) ||
        !add_edge(g, 1, 3, 4) ||
        !add_edge(g, 1, 4, 8) ||
        !add_edge(g, 1, 2, 2) ||
        !add_edge(g, 2, 4, 9) ||
        !add_edge(g, 2, 5, 10) ||
        !add_edge(g, 3, 5, 10) ||
        !add_edge(g, 4, 3, 6) ||
        !add_edge(g, 4, 5, 10)) {
        fprintf(stderr, "Failed to add graph edge.\n");
        free_graph(g);
        return EXIT_FAILURE;
    }

    int max_flow = dinic_max_flow(g, 0, 5);

    printf("=== Dinic's Algorithm Maximum Flow ===\n");
    printf("Calculated Maximum Flow: %d (Expected: 19)\n", max_flow);

    free_graph(g);
    return 0;
}