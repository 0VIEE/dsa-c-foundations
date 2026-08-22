#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 10

struct Node {
    int data;
    struct Node *link;
};

struct Graph {
    int num_vertices;
    struct Node *head[MAX_VERTICES];
};

struct Node* create_node(int val) {
    struct Node *p = (struct Node*)malloc(sizeof(struct Node));
    if (p == NULL) return NULL;
    p->data = val;
    p->link = NULL;
    return p;
}

struct Graph* create_graph(int vertices) {
    struct Graph *g = (struct Graph*)malloc(sizeof(struct Graph));
    if (g == NULL) return NULL;
    g->num_vertices = vertices;
    for (int i = 0; i < vertices; i++) {
        g->head[i] = NULL;
    }
    return g;
}

void add_edge(struct Graph *g, int u, int v) {
    // Add v to u's list
    struct Node *p = create_node(v);
    p->link = g->head[u];
    g->head[u] = p;

    // Add u to v's list (undirected graph)
    p = create_node(u);
    p->link = g->head[v];
    g->head[v] = p;
}

// DFS Helper to detect cycle
int is_cyclic_util(struct Graph *g, int vertex, int visited[], int parent) {
    visited[vertex] = 1;

    struct Node *p = g->head[vertex];
    while (p != NULL) {
        int adj_vertex = p->data;

        if (!visited[adj_vertex]) {
            if (is_cyclic_util(g, adj_vertex, visited, vertex)) {
                return 1;
            }
        } else if (adj_vertex != parent) {
            // Visited neighbor that is NOT parent -> Cycle exists!
            return 1;
        }
        p = p->link;
    }
    return 0;
}

// Main cycle detection handler (checks all components)
int is_cyclic(struct Graph *g) {
    int visited[MAX_VERTICES] = {0};

    for (int i = 0; i < g->num_vertices; i++) {
        if (!visited[i]) {
            if (is_cyclic_util(g, i, visited, -1)) {
                return 1;
            }
        }
    }
    return 0;
}

int main() {
    /*
        Graph with a cycle (0 - 1 - 2 - 0):
           0 --- 1
            \   /
              2 --- 3
    */
    struct Graph *g1 = create_graph(4);
    add_edge(g1, 0, 1);
    add_edge(g1, 1, 2);
    add_edge(g1, 2, 0);
    add_edge(g1, 2, 3);

    printf("=== Undirected Graph Cycle Check ===\n");
    if (is_cyclic(g1)) {
        printf("Graph 1: Cycle Detected! [YES]\n");
    } else {
        printf("Graph 1: No Cycle Detected. [NO]\n");
    }

    /*
        Graph without a cycle (Tree structure: 0-1, 1-2, 2-3):
           0 --- 1 --- 2 --- 3
    */
    struct Graph *g2 = create_graph(4);
    add_edge(g2, 0, 1);
    add_edge(g2, 1, 2);
    add_edge(g2, 2, 3);

    if (is_cyclic(g2)) {
        printf("Graph 2: Cycle Detected! [YES]\n");
    } else {
        printf("Graph 2: No Cycle Detected. [NO]\n");
    }

    return 0;
}