#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 10
#define MAX_EDGES 20
#define INF 99999

struct Edge {
    int src;
    int dest;
    int weight;
};

struct Graph {
    int num_vertices;
    int num_edges;
    struct Edge edges[MAX_EDGES];
};

struct Graph* create_graph(int v, int e) {
    struct Graph *g = (struct Graph*)malloc(sizeof(struct Graph));
    if (g == NULL) return NULL;
    g->num_vertices = v;
    g->num_edges = e;
    return g;
}

void bellman_ford(struct Graph *g, int src) {
    int v = g->num_vertices;
    int e = g->num_edges;
    int dist[MAX_VERTICES];

    // Step 1: Initialize distances
    for (int i = 0; i < v; i++) {
        dist[i] = INF;
    }
    dist[src] = 0;

    // Step 2: Relax all edges (V - 1) times
    for (int i = 1; i <= v - 1; i++) {
        for (int j = 0; j < e; j++) {
            int u = g->edges[j].src;
            int dest = g->edges[j].dest;
            int weight = g->edges[j].weight;

            if (dist[u] != INF && dist[u] + weight < dist[dest]) {
                dist[dest] = dist[u] + weight;
            }
        }
    }

    // Step 3: Check for negative-weight cycles
    int has_negative_cycle = 0;
    for (int j = 0; j < e; j++) {
        int u = g->edges[j].src;
        int dest = g->edges[j].dest;
        int weight = g->edges[j].weight;

        if (dist[u] != INF && dist[u] + weight < dist[dest]) {
            has_negative_cycle = 1;
            break;
        }
    }

    printf("=== Bellman-Ford Shortest Path (Source %d) ===\n", src);
    if (has_negative_cycle) {
        printf("Warning: Graph contains a Negative Weight Cycle!\n");
        return;
    }

    printf("Vertex \t Distance from Source\n");
    for (int i = 0; i < v; i++) {
        printf("%d \t %d\n", i, dist[i]);
    }
}

int main() {
    /*
        Directed Graph with negative weights:
        0 -> 1 (-1), 0 -> 2 (4), 1 -> 2 (3), 1 -> 3 (2), 
        1 -> 4 (2), 3 -> 2 (5), 3 -> 1 (1), 4 -> 3 (-3)
    */
    int v = 5;
    int e = 8;
    struct Graph *g = create_graph(v, e);

    // Edge 0-1
    g->edges[0].src = 0; g->edges[0].dest = 1; g->edges[0].weight = -1;
    // Edge 0-2
    g->edges[1].src = 0; g->edges[1].dest = 2; g->edges[1].weight = 4;
    // Edge 1-2
    g->edges[2].src = 1; g->edges[2].dest = 2; g->edges[2].weight = 3;
    // Edge 1-3
    g->edges[3].src = 1; g->edges[3].dest = 3; g->edges[3].weight = 2;
    // Edge 1-4
    g->edges[4].src = 1; g->edges[4].dest = 4; g->edges[4].weight = 2;
    // Edge 3-2
    g->edges[5].src = 3; g->edges[5].dest = 2; g->edges[5].weight = 5;
    // Edge 3-1
    g->edges[6].src = 3; g->edges[6].dest = 1; g->edges[6].weight = 1;
    // Edge 4-3
    g->edges[7].src = 4; g->edges[7].dest = 3; g->edges[7].weight = -3;

    bellman_ford(g, 0);

    return 0;
}