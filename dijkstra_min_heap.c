#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_V 10
#define INF INT_MAX

// Adjacency List Node
struct AdjListNode {
    int dest;
    int weight;
    struct AdjListNode *link;
};

// Graph Representation
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

    // Undirected graph: add edge in reverse direction
    p = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    p->dest = src;
    p->weight = weight;
    p->link = g->adj[dest];
    g->adj[dest] = p;
}

// Min-Heap Node
struct HeapNode {
    int v;
    int dist;
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
    struct HeapNode t = *a;
    *a = *b;
    *b = t;
}

void min_heapify(struct MinHeap *h, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < h->size && h->array[left].dist < h->array[smallest].dist)
        smallest = left;
    if (right < h->size && h->array[right].dist < h->array[smallest].dist)
        smallest = right;

    if (smallest != idx) {
        swap_heap_nodes(&h->array[smallest], &h->array[idx]);
        min_heapify(h, smallest);
    }
}

void push_heap(struct MinHeap *h, int v, int dist) {
    int i = h->size++;
    h->array[i].v = v;
    h->array[i].dist = dist;

    while (i != 0 && h->array[(i - 1) / 2].dist > h->array[i].dist) {
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

// Dijkstra's Algorithm
void dijkstra(struct Graph *g, int src) {
    int dist[MAX_V];
    for (int i = 0; i < g->V; i++) {
        dist[i] = INF;
    }

    struct MinHeap *h = create_min_heap();

    dist[src] = 0;
    push_heap(h, src, 0);

    while (!is_heap_empty(h)) {
        struct HeapNode curr = pop_heap(h);
        int u = curr.v;
        int d = curr.dist;

        // Skip outdated relaxed entries
        if (d > dist[u]) continue;

        struct AdjListNode *p = g->adj[u];
        while (p != NULL) {
            int v = p->dest;
            int weight = p->weight;

            if (dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                push_heap(h, v, dist[v]);
            }
            p = p->link;
        }
    }

    printf("=== Dijkstra's Shortest Paths (Source = %d) ===\n", src);
    printf("Vertex\tDistance from Source\n");
    for (int i = 0; i < g->V; i++) {
        printf("%d\t%d\n", i, dist[i]);
    }

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
        Graph:
        0 - (4) - 1 - (8) - 2
        |         |         |
       (8)       (11)      (2)
        |         |         |
        7 - (7) - 6 - (1) - 5
    */
    int V = 6;
    struct Graph *g = create_graph(V);

    add_edge(g, 0, 1, 4);
    add_edge(g, 0, 2, 8);
    add_edge(g, 1, 2, 2);
    add_edge(g, 1, 3, 5);
    add_edge(g, 2, 3, 5);
    add_edge(g, 2, 4, 9);
    add_edge(g, 3, 4, 4);
    add_edge(g, 3, 5, 2);
    add_edge(g, 4, 5, 6);

    dijkstra(g, 0);

    free_graph(g);
    return 0;
}