#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

// Push onto dynamic stack - O(1)
struct Node* push_ll(struct Node *top, int val) {
    struct Node *p = (struct Node*)malloc(sizeof(struct Node));
    if (p == NULL) {
        printf("Stack Overflow! Memory allocation failed.\n");
        return top;
    }
    p->data = val;
    p->next = top; // New node points to old top
    top = p;       // Update top pointer
    printf("Pushed %d onto stack\n", val);
    return top;
}

// Pop from dynamic stack - O(1)
struct Node* pop_ll(struct Node *top, int *popped_val) {
    if (top == NULL) {
        printf("Stack Underflow! Stack is empty.\n");
        *popped_val = -1;
        return NULL;
    }
    struct Node *p = top;
    *popped_val = p->data;
    top = top->next; // Advance top to next node
    free(p);         // Release memory
    return top;
}

// Peek top element - O(1)
int peek_ll(struct Node *top) {
    if (top == NULL) {
        printf("Stack is empty!\n");
        return -1;
    }
    return top->data;
}

int main() {
    printf("=== Testing Linked List-Based Stack ===\n");
    struct Node *top = NULL; // Initialize empty stack

    // Push 100, 200, 300
    top = push_ll(top, 100);
    top = push_ll(top, 200);
    top = push_ll(top, 300);

    // Peek top element (should be 300)
    printf("Current Top Element (Peek): %d\n", peek_ll(top));

    // Pop top element (removes 300)
    int popped;
    top = pop_ll(top, &popped);
    printf("Popped Element: %d\n", popped);

    // Peek top element again (should be 200)
    printf("New Top Element (Peek): %d\n", peek_ll(top));

    return 0;
}