#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 10

struct Graph {
    int num_vertices;
    int adj[MAX_VERTICES][MAX_VERTICES];
};

// Initialize graph with 0 edges
struct Graph* create_graph(int vertices) {
    struct Graph* g = (struct Graph*)malloc(sizeof(struct Graph));
    if (g == NULL) return NULL;
    
    g->num_vertices = vertices;
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            g->adj[i][j] = 0;
        }
    }
    return g;
}

// Add undirected edge between u and v
void add_edge(struct Graph* g, int u, int v) {
    if (u >= 0 && u < g->num_vertices && v >= 0 && v < g->num_vertices) {
        g->adj[u][v] = 1;
        g->adj[v][u] = 1; // Symmetric for undirected graph
    }
}

// Print Adjacency Matrix
void print_graph(struct Graph* g) {
    printf("=== Graph Adjacency Matrix ===\n   ");
    for (int i = 0; i < g->num_vertices; i++) {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < g->num_vertices; i++) {
        printf("%d: ", i);
        for (int j = 0; j < g->num_vertices; j++) {
            printf("%d ", g->adj[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // 4 Vertices: 0, 1, 2, 3
    struct Graph* g = create_graph(4);

    add_edge(g, 0, 1);
    add_edge(g, 0, 2);
    add_edge(g, 1, 2);
    add_edge(g, 2, 3);

    print_graph(g);

    return 0;
}