#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_V 16

struct AdjListNode {
    int dest;
    struct AdjListNode *link;
};

struct Tree {
    int V;
    struct AdjListNode *adj[MAX_V];
};

struct Tree* create_tree(int V) {
    struct Tree *t = (struct Tree*)malloc(sizeof(struct Tree));
    t->V = V;
    for (int i = 0; i < V; i++) {
        t->adj[i] = NULL;
    }
    return t;
}

void add_edge(struct Tree *t, int u, int v) {
    // Edge u -> v
    struct AdjListNode *p = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    p->dest = v;
    p->link = t->adj[u];
    t->adj[u] = p;

    // Edge v -> u
    p = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    p->dest = u;
    p->link = t->adj[v];
    t->adj[v] = p;
}

// Queue for BFS traversal
struct QueueNode {
    int val;
    struct QueueNode *link;
};

struct Queue {
    struct QueueNode *front;
    struct QueueNode *rear;
};

struct Queue* create_queue() {
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

void enqueue(struct Queue *q, int val) {
    struct QueueNode *p = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    p->val = val;
    p->link = NULL;
    if (q->rear == NULL) {
        q->front = p;
        q->rear = p;
        return;
    }
    q->rear->link = p;
    q->rear = p;
}

bool is_empty(struct Queue *q) {
    return q->front == NULL;
}

int dequeue(struct Queue *q) {
    if (is_empty(q)) return -1;
    struct QueueNode *p = q->front;
    int val = p->val;
    q->front = q->front->link;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(p);
    return val;
}

// BFS helper that returns the farthest node and records maximum distance
int bfs_farthest(struct Tree *t, int start_node, int *max_dist) {
    int dist[MAX_V];
    for (int i = 0; i < t->V; i++) dist[i] = -1;

    struct Queue *q = create_queue();
    enqueue(q, start_node);
    dist[start_node] = 0;

    int farthest_node = start_node;
    *max_dist = 0;

    while (!is_empty(q)) {
        int u = dequeue(q);

        if (dist[u] > *max_dist) {
            *max_dist = dist[u];
            farthest_node = u;
        }

        struct AdjListNode *p = t->adj[u];
        while (p != NULL) {
            int v = p->dest;
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                enqueue(q, v);
            }
            p = p->link;
        }
    }

    free(q);
    return farthest_node;
}

int get_tree_diameter(struct Tree *t, int *endpoint_a, int *endpoint_b) {
    int dist_dummy = 0;
    // Step 1: Run BFS from an arbitrary root (0) to find one peripheral endpoint
    *endpoint_a = bfs_farthest(t, 0, &dist_dummy);

    // Step 2: Run BFS from endpoint_a to find the other endpoint and exact diameter
    int diameter = 0;
    *endpoint_b = bfs_farthest(t, *endpoint_a, &diameter);

    return diameter;
}

void free_tree(struct Tree *t) {
    for (int i = 0; i < t->V; i++) {
        struct AdjListNode *p = t->adj[i];
        while (p != NULL) {
            struct AdjListNode *temp = p;
            p = p->link;
            free(temp);
        }
    }
    free(t);
}

int main() {
    /*
        Sample Tree (8 vertices, 0 to 7):
              0
             / \
            1   2
           /     \
          3       4
         /         \
        5           6
                     \
                      7
        Longest path is between 5 and 7:
        5 - 3 - 1 - 0 - 2 - 4 - 6 - 7
        Total edges (diameter): 7
    */
    int V = 8;
    struct Tree *t = create_tree(V);

    add_edge(t, 0, 1);
    add_edge(t, 0, 2);
    add_edge(t, 1, 3);
    add_edge(t, 2, 4);
    add_edge(t, 3, 5);
    add_edge(t, 4, 6);
    add_edge(t, 6, 7);

    int ep_a = 0, ep_b = 0;
    int diameter = get_tree_diameter(t, &ep_a, &ep_b);

    printf("=== Tree Diameter via Two-Pass BFS ===\n");
    printf("Endpoint A: Node %d\n", ep_a);
    printf("Endpoint B: Node %d\n", ep_b);
    printf("Calculated Tree Diameter: %d edges (Expected: 7)\n", diameter);

    free_tree(t);
    return 0;
}