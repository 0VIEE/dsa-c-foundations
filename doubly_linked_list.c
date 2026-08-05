#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
    struct Node *prev; // New backward pointer!
};

// Function to create a Doubly Linked List node
struct Node* create_node(int val) {
    struct Node *p = (struct Node*)malloc(sizeof(struct Node));
    if (p == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    p->data = val;
    p->next = NULL;
    p->prev = NULL;
    return p;
}

struct Node* insert_head(struct Node *head, int val) {
    struct Node *p = create_node(val);
    if (p == NULL) return head;

    if (head != NULL) {
        p->next = head;
        head->prev = p; // Connect old head's back pointer to the new node
    }

    head = p; // New node becomes head
    return head;
}

// Forward traversal
void print_forward(struct Node *head) {
    struct Node *p = head;
    printf("Forward: ");
    while (p != NULL) {
        printf("%d -> ", p->data);
        p = p->next;
    }
    printf("NULL\n");
}

// Backward traversal starting from the tail node
void print_backward(struct Node *head) {
    if (head == NULL) return;
    
    // Step 1: Traverse to the tail
    struct Node *p = head;
    while (p->next != NULL) {
        p = p->next;
    }

    // Step 2: Walk backward using prev
    printf("Backward: ");
    while (p != NULL) {
        printf("%d -> ", p->data);
        p = p->prev;
    }
    printf("NULL\n");
}

// Function to delete the head node in a Doubly Linked List
struct Node* delete_head(struct Node *head) {
    if (head == NULL) {
        printf("List is empty, nothing to delete!\n");
        return NULL;
    }

    struct Node *p = head; // Save current head
    head = head->next;    // Advance head to the next node

    if (head != NULL) {
        head->prev = NULL; // Unlink backward pointer of the new head
    }

    free(p); // Release old head memory
    return head;
}

int main() {
    struct Node *head = NULL;

    // Build the list using insert_head
    head = insert_head(head, 10);
    head = insert_head(head, 20);
    head = insert_head(head, 30);

    printf("--- Before Deletion ---\n");
    print_forward(head);  // Prints: Forward: 30 -> 20 -> 10 -> NULL
    print_backward(head); // Prints: Backward: 10 -> 20 -> 30 -> NULL

    // Delete head node (removes 30)
    head = delete_head(head);

    printf("\n--- After Deleting Head ---\n");
    print_forward(head);  // Prints: Forward: 20 -> 10 -> NULL
    print_backward(head); // Prints: Backward: 10 -> 20 -> NULL

    return 0;
}