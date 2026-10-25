#include <stdio.h>
#include <stdlib.h>

#define V 5
#define INF 99999

int min_key(int key[], int mst_set[]) {
    int min = INF, min_index = -1;

    for (int v = 0; v < V; v++) {
        if (!mst_set[v] && key[v] < min) {
            min = key[v];
            min_index = v;
        }
    }
    return min_index;
}

void print_mst(int parent[], int graph[V][V]) {
    int total_weight = 0;
    printf("=== Prim's Minimum Spanning Tree ===\n");
    printf("Edge \tWeight\n");
    for (int i = 1; i < V; i++) {
        printf("%d - %d \t%d\n", parent[i], i, graph[i][parent[i]]);
        total_weight += graph[i][parent[i]];
    }
    printf("Total MST Weight: %d\n", total_weight);
}

void prim_mst(int graph[V][V]) {
    int parent[V];
    int key[V];
    int mst_set[V];

    for (int i = 0; i < V; i++) {
        key[i] = INF;
        mst_set[i] = 0;
    }

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < V - 1; count++) {
        int u = min_key(key, mst_set);
        if (u == -1) break;

        mst_set[u] = 1;

        for (int v = 0; v < V; v++) {
            if (graph[u][v] && !mst_set[v] && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
            }
        }
    }

    print_mst(parent, graph);
}

int main() {
    /*
        Weighted Undirected Graph:
            (0)---2---(1)
             | \       |
             6   8     3
             |     \   |
            (2)---5---(3)---9---(4)
                  \         /
                   7-------(4)
    */
    int graph[V][V] = {
        {0, 2, 6, 8, 0},
        {2, 0, 0, 3, 0},
        {6, 0, 0, 5, 7},
        {8, 3, 5, 0, 9},
        {0, 0, 7, 9, 0}
    };

    prim_mst(graph);

    return 0;
}