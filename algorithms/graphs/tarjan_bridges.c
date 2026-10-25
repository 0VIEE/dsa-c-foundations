#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_V 10

struct AdjListNode {
    int dest;
    struct AdjListNode *link;
};

struct Graph {
    int V;
    struct AdjListNode *adj[MAX_V];
};

struct Graph* create_graph(int V) {
    struct Graph *g = (struct Graph*)malloc(sizeof(struct Graph));
    g->V = V;
    for (int i = 0; i < V; i++) {
        g->adj[i] = NULL;
    }
    return g;
}

void add_edge(struct Graph *g, int src, int dest) {
    // Add edge src -> dest
    struct AdjListNode *p = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    p->dest = dest;
    p->link = g->adj[src];
    g->adj[src] = p;

    // Add edge dest -> src (Undirected graph)
    p = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    p->dest = src;
    p->link = g->adj[dest];
    g->adj[dest] = p;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

// DFS to find bridges using tin and low arrays
void dfs_bridges(struct Graph *g, int u, int parent, bool visited[], int tin[], int low[], int *timer) {
    visited[u] = true;
    tin[u] = low[u] = ++(*timer);

    struct AdjListNode *p = g->adj[u];
    while (p != NULL) {
        int v = p->dest;
        if (v == parent) {
            p = p->link;
            continue;
        }

        if (visited[v]) {
            // Back-edge found: update low of current vertex
            low[u] = min(low[u], tin[v]);
        } else {
            // Forward tree-edge: recurse on child
            dfs_bridges(g, v, u, visited, tin, low, timer);
            low[u] = min(low[u], low[v]);

            // Bridge condition check
            if (low[v] > tin[u]) {
                printf("Bridge detected: %d -- %d\n", u, v);
            }
        }
        p = p->link;
    }
}

void find_bridges(struct Graph *g) {
    bool visited[MAX_V] = {false};
    int tin[MAX_V] = {0};
    int low[MAX_V] = {0};
    int timer = 0;

    printf("=== Tarjan's Bridge-Finding Algorithm ===\n");
    for (int i = 0; i < g->V; i++) {
        if (!visited[i]) {
            dfs_bridges(g, i, -1, visited, tin, low, &timer);
        }
    }
}

void free_graph(struct Graph *g) {
    for (int i = 0; i < g->V; i++) {
        struct AdjListNode *p = g->adj[i];
        while (p != NULL) {
            struct AdjListNode *temp = p;
            p = p->link;
            free(temp);
        }
    }
    free(g);
}

int main() {
    /*
        Graph structure:
        0 --- 1
        |   / |
        |  /  |
        2 --- 3
              |
              4 --- 5
        Bridges should be: 3 -- 4 and 4 -- 5
    */
    int V = 6;
    struct Graph *g = create_graph(V);

    add_edge(g, 0, 1);
    add_edge(g, 1, 2);
    add_edge(g, 2, 0);
    add_edge(g, 1, 3);
    add_edge(g, 2, 3);
    add_edge(g, 3, 4);
    add_edge(g, 4, 5);

    find_bridges(g);

    free_graph(g);
    return 0;
}