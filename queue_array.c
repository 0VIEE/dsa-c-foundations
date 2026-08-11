#include <stdio.h>
#include <stdlib.h>

#define MAX 100

struct Queue {
    int arr[MAX];
    int front;
    int rear;
};

// Initialize empty queue
void init_queue(struct Queue *q) {
    q->front = -1;
    q->rear = -1;
}

// Enqueue operation - O(1)
void enqueue(struct Queue *q, int val) {
    if (q->rear == MAX - 1) {
        printf("Queue Overflow! Cannot enqueue %d\n", val);
        return;
    }
    if (q->front == -1) q->front = 0; // First element added
    q->rear++;
    q->arr[q->rear] = val;
    printf("Enqueued %d into queue\n", val);
}

// Dequeue operation - O(1)
int dequeue(struct Queue *q) {
    if (q->front == -1 || q->front > q->rear) {
        printf("Queue Underflow! Queue is empty\n");
        return -1;
    }
    int val = q->arr[q->front];
    q->front++;
    return val;
}

// Peek front element - O(1)
int peek_front(struct Queue *q) {
    if (q->front == -1 || q->front > q->rear) {
        printf("Queue is empty!\n");
        return -1;
    }
    return q->arr[q->front];
}

int main() {
    printf("=== Testing Array-Based Queue ===\n");
    struct Queue q;
    init_queue(&q);

    enqueue(&q, 10);
    enqueue(&q, 20);
    enqueue(&q, 30);

    printf("Front Element (Peek): %d\n", peek_front(&q)); // Should print 10

    printf("Dequeued Element: %d\n", dequeue(&q));        // Removes 10
    printf("New Front Element (Peek): %d\n", peek_front(&q)); // Should print 20

    return 0;
}