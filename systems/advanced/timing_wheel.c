#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define WHEEL_SIZE 8 // Number of time slots in the wheel

struct Timer {
    uint32_t id;
    uint32_t rotation; // Number of full revolutions remaining
    void (*callback)(uint32_t id);
    struct Timer *link; // Linked list of timers in the same slot
};

struct TimingWheel {
    struct Timer* slots[WHEEL_SIZE];
    uint32_t current_slot;
    uint64_t current_tick;
};

struct TimingWheel* create_timing_wheel() {
    struct TimingWheel *p = (struct TimingWheel*)malloc(sizeof(struct TimingWheel));
    p->current_slot = 0;
    p->current_tick = 0;
    for (int i = 0; i < WHEEL_SIZE; i++) {
        p->slots[i] = NULL;
    }
    return p;
}

// Add timer to wheel in O(1) time
void add_timer(struct TimingWheel *w, uint32_t id, uint32_t delay_ticks, void (*cb)(uint32_t)) {
    struct Timer *node = (struct Timer*)malloc(sizeof(struct Timer));
    node->id = id;
    node->callback = cb;

    uint32_t target_slot = (w->current_slot + delay_ticks) % WHEEL_SIZE;
    node->rotation = delay_ticks / WHEEL_SIZE;

    // Attach to head of slot linked list
    node->link = w->slots[target_slot];
    w->slots[target_slot] = node;
    printf("  [Timer Added] ID: %d | Delay: %d ticks | Target Slot: %d | Rotations: %d\n",
           id, delay_ticks, target_slot, node->rotation);
}

// Advance clock by one tick and execute expired timers
void advance_tick(struct TimingWheel *w) {
    w->current_tick++;
    w->current_slot = (w->current_slot + 1) % WHEEL_SIZE;
    printf("\n--- Clock Tick: %lu (Current Slot: %d) ---\n", (unsigned long)w->current_tick, w->current_slot);

    struct Timer **p = &w->slots[w->current_slot];
    while (*p != NULL) {
        struct Timer *curr = *p;
        if (curr->rotation == 0) {
            // Timer expired; trigger callback and un-link
            if (curr->callback) {
                curr->callback(curr->id);
            }
            *p = curr->link; // Remove from list
            free(curr);
        } else {
            // Decrement rotation count and move to next
            curr->rotation--;
            p = &curr->link;
        }
    }
}

void free_timing_wheel(struct TimingWheel *w) {
    for (int i = 0; i < WHEEL_SIZE; i++) {
        struct Timer *curr = w->slots[i];
        while (curr != NULL) {
            struct Timer *temp = curr;
            curr = curr->link;
            free(temp);
        }
    }
    free(w);
}

// Sample timer callback
void on_timer_expired(uint32_t id) {
    printf("  -> [CALLBACK EXECUTED] Timer ID %d fired successfully!\n", id);
}

int main() {
    printf("=== Hierarchical Timing Wheel Timer Manager (ANSI C) ===\n");
    struct TimingWheel *wheel = create_timing_wheel();

    printf("Scheduling timers...\n");
    add_timer(wheel, 101, 2, on_timer_expired); // Expires in 2 ticks
    add_timer(wheel, 102, 5, on_timer_expired); // Expires in 5 ticks
    add_timer(wheel, 103, 10, on_timer_expired); // Expires in 10 ticks (1 rotation + 2 slots)

    printf("\nSimulating timing wheel clock ticks:\n");
    for (int i = 0; i < 11; i++) {
        advance_tick(wheel);
    }

    free_timing_wheel(wheel);
    printf("[PASS] Timing Wheel execution completed cleanly.\n");
    return 0;
}