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

struct Queue {
    int items[MAX_VERTICES];
    int front;
    int rear;
};

struct Queue* create_queue() {
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
    if (q == NULL) return NULL;
    q->front = -1;
    q->rear = -1;
    return q;
}

int is_empty(struct Queue *q) {
    return q->front == -1;
}

void enqueue(struct Queue *q, int val) {
    if (q->rear == MAX_VERTICES - 1) return;
    if (q->front == -1) q->front = 0;
    q->rear++;
    q->items[q->rear] = val;
}

int dequeue(struct Queue *q) {
    if (is_empty(q)) return -1;
    int val = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
        q->front = -1;
        q->rear = -1;
    }
    return val;
}

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

// Breadth-First Search Traversal
void bfs(struct Graph *g, int start_vertex) {
    int visited[MAX_VERTICES] = {0};
    struct Queue *q = create_queue();

    visited[start_vertex] = 1;
    enqueue(q, start_vertex);

    printf("BFS Traversal starting from vertex %d: ", start_vertex);

    while (!is_empty(q)) {
        int current = dequeue(q);
        printf("%d ", current);

        struct Node *p = g->head[current];
        while (p != NULL) {
            int adj_vertex = p->data;
            if (!visited[adj_vertex]) {
                visited[adj_vertex] = 1;
                enqueue(q, adj_vertex);
            }
            p = p->link;
        }
    }
    printf("\n");
    free(q);
}

int main() {
    /*
        Graph structure:
           0 --- 1
           | \   |
           |  \  |
           2 --- 3 --- 4
    */
    struct Graph *g = create_graph(5);

    add_edge(g, 0, 1);
    add_edge(g, 0, 2);
    add_edge(g, 0, 3);
    add_edge(g, 1, 3);
    add_edge(g, 2, 3);
    add_edge(g, 3, 4);

    bfs(g, 0);

    return 0;
}