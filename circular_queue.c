#include <stdio.h>
#include <stdlib.h>

#define MAX 5

struct CircularQueue {
    int arr[MAX];
    int front;
    int rear;
};

void init_queue(struct CircularQueue *q) {
    q->front = -1;
    q->rear = -1;
}

// Check if queue is full
int is_full(struct CircularQueue *q) {
    return ((q->rear + 1) % MAX == q->front);
}

// Check if queue is empty
int is_empty(struct CircularQueue *q) {
    return (q->front == -1);
}

// Enqueue - O(1)
void enqueue_cq(struct CircularQueue *q, int val) {
    if (is_full(q)) {
        printf("Circular Queue Overflow! Cannot enqueue %d\n", val);
        return;
    }
    if (is_empty(q)) {
        q->front = 0;
        q->rear = 0;
    } else {
        q->rear = (q->rear + 1) % MAX; // Wrap around using modulo
    }
    q->arr[q->rear] = val;
    printf("Enqueued %d at index %d\n", val, q->rear);
}

// Dequeue - O(1)
int dequeue_cq(struct CircularQueue *q) {
    if (is_empty(q)) {
        printf("Circular Queue Underflow!\n");
        return -1;
    }
    int val = q->arr[q->front];
    
    if (q->front == q->rear) { // Queue had only one element
        q->front = -1;
        q->rear = -1;
    } else {
        q->front = (q->front + 1) % MAX; // Wrap around using modulo
    }
    return val;
}

int main() {
    struct CircularQueue q;
    init_queue(&q);

    printf("=== Filling Circular Queue (MAX = 5) ===\n");
    enqueue_cq(&q, 10); // index 0
    enqueue_cq(&q, 20); // index 1
    enqueue_cq(&q, 30); // index 2
    enqueue_cq(&q, 40); // index 3

    printf("\n=== Dequeuing Two Elements ===\n");
    printf("Dequeued: %d\n", dequeue_cq(&q)); // Removes 10 (front moves to 1)
    printf("Dequeued: %d\n", dequeue_cq(&q)); // Removes 20 (front moves to 2)

    printf("\n=== Enqueuing New Elements (Testing Modulo Wrap-Around) ===\n");
    enqueue_cq(&q, 50); // index 4
    enqueue_cq(&q, 60); // index 0  <-- WATCH THIS! (4 + 1) % 5 = 0
    enqueue_cq(&q, 70); // index 1  <-- WATCH THIS! (0 + 1) % 5 = 1

    printf("\n=== Trying Overflow ===\n");
    enqueue_cq(&q, 80); // Should trigger Circular Queue Overflow!

    return 0;
}