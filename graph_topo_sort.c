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

// Add DIRECTED edge: u -> v
void add_directed_edge(struct Graph *g, int u, int v) {
    struct Node *p = create_node(v);
    p->link = g->head[u];
    g->head[u] = p;
}

// DFS helper for Topological Sort
void topo_sort_util(struct Graph *g, int v, int visited[], int stack[], int *top) {
    visited[v] = 1;

    struct Node *p = g->head[v];
    while (p != NULL) {
        int adj_v = p->data;
        if (!visited[adj_v]) {
            topo_sort_util(g, adj_v, visited, stack, top);
        }
        p = p->link;
    }

    // Push to stack after visiting all neighbors
    stack[++(*top)] = v;
}

// Main Topological Sort function
void topological_sort(struct Graph *g) {
    int visited[MAX_VERTICES] = {0};
    int stack[MAX_VERTICES];
    int top = -1;

    for (int i = 0; i < g->num_vertices; i++) {
        if (!visited[i]) {
            topo_sort_util(g, i, visited, stack, &top);
        }
    }

    printf("Topological Sort Order: ");
    while (top >= 0) {
        printf("%d ", stack[top--]);
    }
    printf("\n");
}

int main() {
    /*
        DAG dependencies:
        5 -> 0, 5 -> 2
        4 -> 0, 4 -> 1
        2 -> 3
        3 -> 1
    */
    struct Graph *g = create_graph(6);
    add_directed_edge(g, 5, 0);
    add_directed_edge(g, 5, 2);
    add_directed_edge(g, 4, 0);
    add_directed_edge(g, 4, 1);
    add_directed_edge(g, 2, 3);
    add_directed_edge(g, 3, 1);

    topological_sort(g);

    return 0;
}