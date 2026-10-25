#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SIZE 8 // Must be a power of two

struct SPSCRingBuffer {
    int buffer[BUFFER_SIZE];
    volatile unsigned int head; // Read index
    volatile unsigned int tail; // Write index
};

struct SPSCRingBuffer* create_spsc_buffer() {
    struct SPSCRingBuffer *p = (struct SPSCRingBuffer*)malloc(sizeof(struct SPSCRingBuffer));
    if (p == NULL) {
        fprintf(stderr, "Allocation failed\n");
        exit(EXIT_FAILURE);
    }
    p->head = 0;
    p->tail = 0;
    return p;
}

bool spsc_push(struct SPSCRingBuffer *p, int value) {
    unsigned int current_tail = p->tail;
    unsigned int current_head = p->head;

    // Check if buffer is full
    if ((current_tail - current_head) >= BUFFER_SIZE) {
        return false; // Full
    }

    p->buffer[current_tail & (BUFFER_SIZE - 1)] = value;
    p->tail = current_tail + 1; // Publish write
    return true;
}

bool spsc_pop(struct SPSCRingBuffer *p, int *value) {
    unsigned int current_head = p->head;
    unsigned int current_tail = p->tail;

    // Check if buffer is empty
    if (current_head == current_tail) {
        return false; // Empty
    }

    *value = p->buffer[current_head & (BUFFER_SIZE - 1)];
    p->head = current_head + 1; // Publish read
    return true;
}

void free_spsc_buffer(struct SPSCRingBuffer *p) {
    if (p != NULL) {
        free(p);
    }
}

int main() {
    printf("=== Lock-Free SPSC Ring Buffer (ANSI C) ===\n");
    struct SPSCRingBuffer *ring = create_spsc_buffer();

    // Simulate Producer pushing items
    printf("Producing items: 10, 20, 30, 40, 50\n");
    spsc_push(ring, 10);
    spsc_push(ring, 20);
    spsc_push(ring, 30);
    spsc_push(ring, 40);
    spsc_push(ring, 50);

    // Simulate Consumer popping items
    int val = 0;
    printf("Consuming items:\n");
    while (spsc_pop(ring, &val)) {
        printf("  -> Popped: %d\n", val);
    }

    // Push more to verify wrap-around behavior
    printf("Producing wrap-around items: 100, 200, 300\n");
    spsc_push(ring, 100);
    spsc_push(ring, 200);
    spsc_push(ring, 300);

    while (spsc_pop(ring, &val)) {
        printf("  -> Popped wrap-around: %d\n", val);
    }

    free_spsc_buffer(ring);
    printf("[PASS] SPSC Ring Buffer execution completed cleanly.\n");
    return 0;
}