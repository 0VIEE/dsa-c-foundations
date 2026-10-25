#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>

struct StackNode {
    int data;
    struct StackNode *link; // Link-style traversal pointer
};

struct LockFreeStack {
    _Atomic(struct StackNode*) top;
};

struct LockFreeStack* create_lf_stack() {
    struct LockFreeStack *p = (struct LockFreeStack*)malloc(sizeof(struct LockFreeStack));
    atomic_init(&p->top, NULL);
    return p;
}

// Push node onto stack without mutex locks using atomic CAS loop
void lf_stack_push(struct LockFreeStack *stack, int val) {
    struct StackNode *p = (struct StackNode*)malloc(sizeof(struct StackNode));
    p->data = val;
    struct StackNode *old_top = atomic_load(&stack->top);

    do {
        p->link = old_top;
    } while (!atomic_compare_exchange_weak(&stack->top, &old_top, p));

    printf("  [Lock-Free Push] Pushed value %d onto atomic stack\n", val);
}

// Pop node from stack without mutex locks using atomic CAS loop
bool lf_stack_pop(struct LockFreeStack *stack, int *out_val) {
    struct StackNode *old_top = atomic_load(&stack->top);
    struct StackNode *new_top = NULL;

    do {
        if (old_top == NULL) {
            return false; // Stack is empty
        }
        new_top = old_top->link;
    } while (!atomic_compare_exchange_weak(&stack->top, &old_top, new_top));

    *out_val = old_top->data;
    free(old_top); // Note: In multi-threaded production, hazard pointers/epoch reclamation prevents ABA
    return true;
}

void free_lf_stack(struct LockFreeStack *stack) {
    struct StackNode *curr = atomic_load(&stack->top);
    while (curr != NULL) {
        struct StackNode *temp = curr;
        curr = curr->link;
        free(temp);
    }
    free(stack);
}

int main() {
    printf("=== Lock-Free Treiber Stack (ANSI C11 Atomics) ===\n");
    struct LockFreeStack *stack = create_lf_stack();

    printf("Pushing values onto lock-free stack:\n");
    lf_stack_push(stack, 10);
    lf_stack_push(stack, 20);
    lf_stack_push(stack, 30);

    printf("\nPopping values from lock-free stack:\n");
    int val = 0;
    while (lf_stack_pop(stack, &val)) {
        printf("  -> [Lock-Free Pop] Popped value: %d\n", val);
    }

    free_lf_stack(stack);
    printf("[PASS] Treiber Stack execution completed cleanly.\n");
    return 0;
}