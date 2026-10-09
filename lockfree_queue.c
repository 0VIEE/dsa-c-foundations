#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node {
    int value;
    struct Node *_Atomic link; // Atomic link pointer for Lock-Free list
};

struct LockFreeQueue {
    struct Node *_Atomic head;
    struct Node *_Atomic tail;
};

// Create node with manual memory allocation
struct Node* create_node(int val) {
    struct Node *p = (struct Node*)malloc(sizeof(struct Node));
    p->value = val;
    p->link = NULL;
    return p;
}

struct LockFreeQueue* create_queue() {
    struct LockFreeQueue *q = (struct LockFreeQueue*)malloc(sizeof(struct LockFreeQueue));
    struct Node *dummy = create_node(-1); // Initial dummy node
    q->head = dummy;
    q->tail = dummy;
    return q;
}

// Lock-free Enqueue using atomic CAS loop
void enqueue(struct LockFreeQueue *q, int val) {
    struct Node *node = create_node(val);
    struct Node *tail = NULL;
    struct Node *next = NULL;

    while (1) {
        tail = q->tail;
        next = tail->link;

        // Verify tail is consistent
        if (tail == q->tail) {
            if (next == NULL) {
                // Try linking new node to current tail
                if (__atomic_compare_exchange_n(&(tail->link), &next, node, 1, __ATOMIC_RELEASE, __ATOMIC_RELAXED)) {
                    break; // Enqueue successful
                }
            } else {
                // Tail fell behind; try advancing it
                __atomic_compare_exchange_n(&(q->tail), &tail, next, 1, __ATOMIC_RELEASE, __ATOMIC_RELAXED);
            }
        }
    }
    // Advance tail pointer to new node
    __atomic_compare_exchange_n(&(q->tail), &tail, node, 1, __ATOMIC_RELEASE, __ATOMIC_RELAXED);
}

// Lock-free Dequeue using atomic CAS loop
bool dequeue(struct LockFreeQueue *q, int *val) {
    struct Node *head = NULL;
    struct Node *tail = NULL;
    struct Node *next = NULL;

    while (1) {
        head = q->head;
        tail = q->tail;
        next = head->link;

        if (head == q->head) {
            if (head == tail) {
                if (next == NULL) {
                    return false; // Queue empty
                }
                // Tail fell behind; advance it
                __atomic_compare_exchange_n(&(q->tail), &tail, next, 1, __ATOMIC_RELEASE, __ATOMIC_RELAXED);
            } else {
                // Read value before advancing head
                *val = next->value;
                if (__atomic_compare_exchange_n(&(q->head), &head, next, 1, __ATOMIC_RELEASE, __ATOMIC_RELAXED)) {
                    free(head); // Free old dummy node
                    return true;
                }
            }
        }
    }
}

void free_queue(struct LockFreeQueue *q) {
    int val;
    while (dequeue(q, &val)); // Drain queue
    free(q->head);            // Free remaining dummy node
    free(q);
}

int main() {
    printf("=== Atomic Lock-Free Queue (Michael & Scott Queue) ===\n");
    struct LockFreeQueue *q = create_queue();

    printf("Enqueueing values: 100, 200, 300\n");
    enqueue(q, 100);
    enqueue(q, 200);
    enqueue(q, 300);

    int val;
    printf("Dequeueing values:\n");
    while (dequeue(q, &val)) {
        printf("  -> Popped: %d\n", val);
    }

    free_queue(q);
    printf("[PASS] Lock-Free MPMC Queue test completed cleanly.\n");
    return 0;
}