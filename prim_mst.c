#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_V 10
#define INF INT_MAX

struct AdjListNode {
    int dest;
    int weight;
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

void add_edge(struct Graph *g, int src, int dest, int weight) {
    struct AdjListNode *p = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    p->dest = dest;
    p->weight = weight;
    p->link = g->adj[src];
    g->adj[src] = p;

    // Undirected edge
    p = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    p->dest = src;
    p->weight = weight;
    p->link = g->adj[dest];
    g->adj[dest] = p;
}

struct HeapNode {
    int v;
    int key;
};

struct MinHeap {
    int size;
    struct HeapNode array[MAX_V * MAX_V];
};

struct MinHeap* create_min_heap() {
    struct MinHeap *h = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    h->size = 0;
    return h;
}

void swap_heap_nodes(struct HeapNode *a, struct HeapNode *b) {
    struct HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void min_heapify(struct MinHeap *h, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < h->size && h->array[left].key < h->array[smallest].key)
        smallest = left;
    if (right < h->size && h->array[right].key < h->array[smallest].key)
        smallest = right;

    if (smallest != idx) {
        swap_heap_nodes(&h->array[smallest], &h->array[idx]);
        min_heapify(h, smallest);
    }
}

void push_heap(struct MinHeap *h, int v, int key) {
    int i = h->size++;
    h->array[i].v = v;
    h->array[i].key = key;

    while (i != 0 && h->array[(i - 1) / 2].key > h->array[i].key) {
        swap_heap_nodes(&h->array[i], &h->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

struct HeapNode pop_heap(struct MinHeap *h) {
    struct HeapNode root = h->array[0];
    h->array[0] = h->array[--h->size];
    min_heapify(h, 0);
    return root;
}

bool is_heap_empty(struct MinHeap *h) {
    return h->size == 0;
}

void prim_mst(struct Graph *g) {
    int V = g->V;
    int parent[MAX_V];
    int key[MAX_V];
    bool in_mst[MAX_V];

    for (int i = 0; i < V; i++) {
        key[i] = INF;
        parent[i] = -1;
        in_mst[i] = false;
    }

    struct MinHeap *h = create_min_heap();

    // Start with root vertex 0
    key[0] = 0;
    push_heap(h, 0, 0);

    while (!is_heap_empty(h)) {
        struct HeapNode min_node = pop_heap(h);
        int u = min_node.v;

        if (in_mst[u]) continue;
        in_mst[u] = true;

        struct AdjListNode *p = g->adj[u];
        while (p != NULL) {
            int v = p->dest;
            int weight = p->weight;

            if (!in_mst[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                push_heap(h, v, key[v]);
            }
            p = p->link;
        }
    }

    printf("=== Prim's Minimum Spanning Tree (MST) ===\n");
    printf("Edge \tWeight\n");
    int total_weight = 0;
    for (int i = 1; i < V; i++) {
        printf("%d -- %d \t%d\n", parent[i], i, key[i]);
        total_weight += key[i];
    }
    printf("Total MST Weight: %d\n", total_weight);

    free(h);
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
        Same 5-edge, 4-vertex graph from Day 69:
        0 - 1 (10), 0 - 2 (6), 0 - 3 (5), 1 - 3 (15), 2 - 3 (4)
    */
    int V = 4;
    struct Graph *g = create_graph(V);

    add_edge(g, 0, 1, 10);
    add_edge(g, 0, 2, 6);
    add_edge(g, 0, 3, 5);
    add_edge(g, 1, 3, 15);
    add_edge(g, 2, 3, 4);

    prim_mst(g);

    free_graph(g);
    return 0;
}