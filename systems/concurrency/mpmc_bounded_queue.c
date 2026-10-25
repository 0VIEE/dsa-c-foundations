#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define BUFFER_SIZE 8 // Must be a power of 2

struct Cell {
    _Atomic size_t sequence;
    int data;
};

struct MPMCBoundedQueue {
    struct Cell buffer[BUFFER_SIZE];
    size_t buffer_mask;
    _Atomic size_t enqueue_pos;
    _Atomic size_t dequeue_pos;
};

struct MPMCBoundedQueue* create_mpmc_queue() {
    struct MPMCBoundedQueue *p = (struct MPMCBoundedQueue*)malloc(sizeof(struct MPMCBoundedQueue));
    p->buffer_mask = BUFFER_SIZE - 1;
    p->enqueue_pos = 0;
    p->dequeue_pos = 0;

    for (size_t i = 0; i < BUFFER_SIZE; i++) {
        p->buffer[i].sequence = i;
        p->buffer[i].data = 0;
    }
    return p;
}

// Lock-Free MPMC Enqueue
bool mpmc_enqueue(struct MPMCBoundedQueue *p, int data) {
    struct Cell *cell;
    size_t pos = __atomic_load_n(&p->enqueue_pos, __ATOMIC_RELAXED);

    while (1) {
        cell = &p->buffer[pos & p->buffer_mask];
        size_t seq = __atomic_load_n(&cell->sequence, __ATOMIC_ACQUIRE);
        intptr_t diff = (intptr_t)seq - (intptr_t)pos;

        if (diff == 0) {
            // Slot is ready for enqueue; attempt CAS to claim slot
            if (__atomic_compare_exchange_n(&p->enqueue_pos, &pos, pos + 1, true, __ATOMIC_RELAXED, __ATOMIC_RELAXED)) {
                break;
            }
        } else if (diff < 0) {
            return false; // Queue is full
        } else {
            pos = __atomic_load_n(&p->enqueue_pos, __ATOMIC_RELAXED);
        }
    }

    cell->data = data;
    __atomic_store_n(&cell->sequence, pos + 1, __ATOMIC_RELEASE);
    return true;
}

// Lock-Free MPMC Dequeue
bool mpmc_dequeue(struct MPMCBoundedQueue *p, int *data) {
    struct Cell *cell;
    size_t pos = __atomic_load_n(&p->dequeue_pos, __ATOMIC_RELAXED);

    while (1) {
        cell = &p->buffer[pos & p->buffer_mask];
        size_t seq = __atomic_load_n(&cell->sequence, __ATOMIC_ACQUIRE);
        intptr_t diff = (intptr_t)seq - (intptr_t)(pos + 1);

        if (diff == 0) {
            // Slot is ready for dequeue; attempt CAS to claim slot
            if (__atomic_compare_exchange_n(&p->dequeue_pos, &pos, pos + 1, true, __ATOMIC_RELAXED, __ATOMIC_RELAXED)) {
                break;
            }
        } else if (diff < 0) {
            return false; // Queue is empty
        } else {
            pos = __atomic_load_n(&p->dequeue_pos, __ATOMIC_RELAXED);
        }
    }

    *data = cell->data;
    __atomic_store_n(&cell->sequence, pos + p->buffer_mask + 1, __ATOMIC_RELEASE);
    return true;
}

void free_mpmc_queue(struct MPMCBoundedQueue *p) {
    if (p != NULL) {
        free(p);
    }
}

int main() {
    printf("=== Lock-Free MPMC Bounded Array Queue (Vyukov Algorithm) ===\n");
    struct MPMCBoundedQueue *queue = create_mpmc_queue();

    printf("Pushing values: 10, 20, 30, 40\n");
    mpmc_enqueue(queue, 10);
    mpmc_enqueue(queue, 20);
    mpmc_enqueue(queue, 30);
    mpmc_enqueue(queue, 40);

    int val;
    printf("Popping values:\n");
    while (mpmc_dequeue(queue, &val)) {
        printf("  -> Dequeued: %d\n", val);
    }

    free_mpmc_queue(queue);
    printf("[PASS] Lock-Free MPMC Bounded Queue execution completed cleanly.\n");
    return 0;
}