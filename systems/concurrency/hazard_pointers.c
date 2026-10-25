#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>

#define MAX_THREADS 4
#define RETIRED_THRESHOLD 2

// Global array of hazard pointers (one per thread)
_Atomic(void*) hazard_pointers[MAX_THREADS];

struct Node {
    int data;
    struct Node *link; // Traversal pointer
};

struct RetiredList {
    struct Node *nodes[16];
    int count;
};

void init_hazard_pointers() {
    for (int i = 0; i < MAX_THREADS; i++) {
        atomic_init(&hazard_pointers[i], NULL);
    }
}

// Publish address thread is actively reading
void set_hazard_pointer(int thread_id, void *ptr) {
    atomic_store(&hazard_pointers[thread_id], ptr);
}

// Clear hazard pointer when finished reading
void clear_hazard_pointer(int thread_id) {
    atomic_store(&hazard_pointers[thread_id], NULL);
}

// Scan hazard pointers and reclaim retired nodes that are safe to free
void reclaim_retired_nodes(struct RetiredList *rl) {
    printf("  [Hazard Pointer Scan] Reclaiming memory safely...\n");
    int remaining_count = 0;
    struct Node *remaining_nodes[16];

    for (int i = 0; i < rl->count; i++) {
        struct Node *node_to_free = rl->nodes[i];
        bool is_hazardous = false;

        // Check if any active thread is referencing this pointer
        for (int t = 0; t < MAX_THREADS; t++) {
            void *hp = atomic_load(&hazard_pointers[t]);
            if (hp == node_to_free) {
                is_hazardous = true;
                break;
            }
        }

        if (!is_hazardous) {
            printf("  -> [Safe Free] Node %d (Address %p) safely reclaimed!\n", node_to_free->data, (void*)node_to_free);
            free(node_to_free);
        } else {
            printf("  -> [Hazard Blocked] Node %d (Address %p) held by active thread. Deferring free.\n", node_to_free->data, (void*)node_to_free);
            remaining_nodes[remaining_count++] = node_to_free;
        }
    }

    rl->count = remaining_count;
    for (int i = 0; i < remaining_count; i++) {
        rl->nodes[i] = remaining_nodes[i];
    }
}

void retire_node(struct RetiredList *rl, struct Node *p) {
    rl->nodes[rl->count++] = p;
    if (rl->count >= RETIRED_THRESHOLD) {
        reclaim_retired_nodes(rl);
    }
}

int main() {
    printf("=== Hazard Pointer Memory Reclamation Engine (ANSI C11) ===\n");
    init_hazard_pointers();

    struct RetiredList rl = { .count = 0 };

    struct Node *node1 = (struct Node*)malloc(sizeof(struct Node));
    node1->data = 100;
    node1->link = NULL;

    struct Node *node2 = (struct Node*)malloc(sizeof(struct Node));
    node2->data = 200;
    node2->link = NULL;

    printf("Simulating Active Thread 1 Reading Node 100:\n");
    set_hazard_pointer(0, node1); // Thread 0 advertises hazard on node1

    printf("\nRetiring Node 100 and Node 200 after popping from stack:\n");
    retire_node(&rl, node1);
    retire_node(&rl, node2);

    printf("\nThread 1 finishes reading Node 100, clearing hazard pointer:\n");
    clear_hazard_pointer(0);

    // Force second reclamation pass
    reclaim_retired_nodes(&rl);

    printf("[PASS] Hazard Pointer execution completed cleanly.\n");
    return 0;
}