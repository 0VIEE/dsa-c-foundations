#include <stdio.h>
#include <stdlib.h>

#define MAX 10

struct Element {
    int data;
    int priority;
};

struct PriorityQueue {
    struct Element arr[MAX];
    int size;
};

void init_pq(struct PriorityQueue *pq) {
    pq->size = 0;
}

// Enqueue maintaining descending order of priority - O(N)
void enqueue_pq(struct PriorityQueue *pq, int val, int priority) {
    if (pq->size == MAX) {
        printf("Priority Queue Overflow!\n");
        return;
    }

    int i = pq->size - 1;
    // Shift elements with lower priority to the right
    while (i >= 0 && pq->arr[i].priority < priority) {
        pq->arr[i + 1] = pq->arr[i];
        i--;
    }

    // Insert new element at correct position
    pq->arr[i + 1].data = val;
    pq->arr[i + 1].priority = priority;
    pq->size++;

    printf("Enqueued %d with Priority %d\n", val, priority);
}

// Dequeue highest priority element (at index 0) - O(N) shift or O(1) from front
int dequeue_pq(struct PriorityQueue *pq) {
    if (pq->size == 0) {
        printf("Priority Queue Underflow!\n");
        return -1;
    }

    int val = pq->arr[0].data;
    // Shift remaining elements left
    for (int i = 0; i < pq->size - 1; i++) {
        pq->arr[i] = pq->arr[i + 1];
    }
    pq->size--;
    return val;
}

int main() {
    struct PriorityQueue pq;
    init_pq(&pq);

    printf("=== Testing Priority Queue ===\n");
    enqueue_pq(&pq, 100, 1); // Low priority
    enqueue_pq(&pq, 500, 5); // Highest priority
    enqueue_pq(&pq, 300, 3); // Medium priority

    printf("\nDequeued (Highest Priority): %d\n", dequeue_pq(&pq)); // Should be 500
    printf("Dequeued (Next Highest): %d\n", dequeue_pq(&pq));     // Should be 300
    printf("Dequeued (Lowest): %d\n", dequeue_pq(&pq));           // Should be 100

    return 0;
}