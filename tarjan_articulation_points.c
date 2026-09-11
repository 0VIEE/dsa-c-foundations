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
    // Edge src -> dest
    struct AdjListNode *p = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    p->dest = dest;
    p->link = g->adj[src];
    g->adj[src] = p;

    // Edge dest -> src (Undirected graph)
    p = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    p->dest = src;
    p->link = g->adj[dest];
    g->adj[dest] = p;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

void dfs_articulation(struct Graph *g, int u, int parent, bool visited[], 
                      int tin[], int low[], bool is_ap[], int *timer) {
    visited[u] = true;
    tin[u] = low[u] = ++(*timer);
    int children = 0;

    struct AdjListNode *p = g->adj[u];
    while (p != NULL) {
        int v = p->dest;
        if (v == parent) {
            p = p->link;
            continue;
        }

        if (visited[v]) {
            // Back-edge to an ancestor
            low[u] = min(low[u], tin[v]);
        } else {
            // Forward tree-edge
            children++;
            dfs_articulation(g, v, u, visited, tin, low, is_ap, timer);
            low[u] = min(low[u], low[v]);

            // Condition 1: Non-root vertex
            if (parent != -1 && low[v] >= tin[u]) {
                is_ap[u] = true;
            }
        }
        p = p->link;
    }

    // Condition 2: Root vertex with 2 or more separate children
    if (parent == -1 && children > 1) {
        is_ap[u] = true;
    }
}

void find_articulation_points(struct Graph *g) {
    bool visited[MAX_V] = {false};
    int tin[MAX_V] = {0};
    int low[MAX_V] = {0};
    bool is_ap[MAX_V] = {false};
    int timer = 0;

    for (int i = 0; i < g->V; i++) {
        if (!visited[i]) {
            dfs_articulation(g, i, -1, visited, tin, low, is_ap, &timer);
        }
    }

    printf("=== Tarjan's Articulation Points (Cut Vertices) ===\n");
    printf("Articulation Points: ");
    bool found = false;
    for (int i = 0; i < g->V; i++) {
        if (is_ap[i]) {
            printf("%d ", i);
            found = true;
        }
    }
    if (!found) {
        printf("None (Graph is Biconnected)");
    }
    printf("\n");
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
        Removing vertex 3 isolates {4, 5}.
        Removing vertex 4 isolates {5}.
        Expected Cut Vertices: 3, 4
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

    find_articulation_points(g);

    free_graph(g);
    return 0;
}