#include <stdio.h>
#include <stdlib.h>

#define V 4
#define INF 99999

int min(int a, int b) {
    return (a < b) ? a : b;
}

// Floyd-Warshall All-Pairs Shortest Path
void floyd_warshall(int graph[V][V]) {
    int dist[V][V];

    // Initialize distance matrix with input graph values
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            dist[i][j] = graph[i][j];
        }
    }

    // k is the intermediate pivot vertex (OUTERMOST LOOP)
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    // Check for negative weight cycles along diagonals
    for (int i = 0; i < V; i++) {
        if (dist[i][i] < 0) {
            printf("\n[ALERT] Negative cycle detected involving vertex %d!\n", i);
            return;
        }
    }

    // Print resulting all-pairs shortest path matrix
    printf("=== Floyd-Warshall All-Pairs Shortest Path Matrix ===\n");
    printf("      ");
    for (int j = 0; j < V; j++) printf("To %d   ", j);
    printf("\n");

    for (int i = 0; i < V; i++) {
        printf("From %d: ", i);
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF) {
                printf(" INF   ");
            } else {
                printf("%4d   ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    /*
        4-Vertex Weighted Directed Graph:
        (0) --(5)--> (1)
         |            |
        (10)         (3)
         v            v
        (3) <--(1)-- (2)
    */
    int graph[V][V] = {
        {0,   5,   INF, 10},
        {INF, 0,   3,   INF},
        {INF, INF, 0,   1},
        {INF, INF, INF, 0}
    };

    floyd_warshall(graph);

    return 0;
}