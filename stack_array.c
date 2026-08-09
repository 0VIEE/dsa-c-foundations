#include <stdio.h>
#include <stdlib.h>

#define MAX 100

struct Stack {
    int arr[MAX];
    int top;
};

// Initialize stack
void init_stack(struct Stack *s) {
    s->top = -1;
}

// Push element - O(1)
void push(struct Stack *s, int val) {
    if (s->top == MAX - 1) {
        printf("Stack Overflow! Cannot push %d\n", val);
        return;
    }
    s->top++;
    s->arr[s->top] = val;
    printf("Pushed %d onto stack\n", val);
}

// Pop element - O(1)
int pop(struct Stack *s) {
    if (s->top == -1) {
        printf("Stack Underflow! Stack is empty\n");
        return -1;
    }
    int val = s->arr[s->top];
    s->top--;
    return val;
}

// Peek top element - O(1)
int peek(struct Stack *s) {
    if (s->top == -1) {
        printf("Stack is empty!\n");
        return -1;
    }
    return s->arr[s->top];
}

int main() {
    printf("=== Testing Array-Based Stack ===\n");
    struct Stack s;
    init_stack(&s);

    push(&s, 10);
    push(&s, 20);
    push(&s, 30);

    printf("Current Top Element (Peek): %d\n", peek(&s)); // 30

    printf("Popped Element: %d\n", pop(&s));              // Removes 30
    printf("New Top Element (Peek): %d\n", peek(&s));     // 20

    return 0;
}