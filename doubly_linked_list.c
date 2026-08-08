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

// Function to insert at the tail in O(1) time
struct Node* insert_tail(struct Node *head, struct Node **tail, int val) {
    struct Node *p = create_node(val);
    if (p == NULL) return head;

    if (head == NULL) {
        *tail = p;
        return p; // List was empty, p is now head and tail
    }

    (*tail)->next = p;
    p->prev = *tail;
    *tail = p; // Update tail pointer
    
    return head;
}

// Function to delete tail in O(1) time
struct Node* delete_tail(struct Node *head, struct Node **tail) {
    if (*tail == NULL) {
        printf("List is empty!\n");
        return NULL;
    }

    struct Node *p = *tail;

    if (head == *tail) { // Only one node in list
        head = NULL;
        *tail = NULL;
    } else {
        *tail = (*tail)->prev;
        (*tail)->next = NULL;
    }

    free(p);
    return head;
}

// Function to reverse a Doubly Linked List in-place
struct Node* reverse_dll(struct Node *head) {
    if (head == NULL || head->next == NULL) {
        return head; // 0 or 1 node, no reversal needed
    }

    struct Node *curr = head;
    struct Node *temp = NULL;

    // Swap next and prev pointers for all nodes
    while (curr != NULL) {
        temp = curr->prev;
        curr->prev = curr->next;
        curr->next = temp;

        // Move to the next node in original list (which is now curr->prev)
        curr = curr->prev;
    }

    // Update head to point to the new front node
    if (temp != NULL) {
        head = temp->prev;
    }

    return head;
}

// Function to insert at any given position in a Doubly Linked List
struct Node* insert_at_pos(struct Node *head, struct Node **tail, int val, int pos) {
    if (pos < 1) {
        printf("Invalid position!\n");
        return head;
    }

    // 1. Position 1 is Head Insertion
    if (pos == 1) {
        return insert_head(head, val);
    }

    struct Node *curr = head;
    // Step pos - 2 times to land on the node right BEFORE the target spot
    for (int i = 1; i < pos - 1 && curr != NULL; i++) {
        curr = curr->next;
    }

    if (curr == NULL) {
        printf("Position out of bounds!\n");
        return head;
    }

    // 2. Inserting at the very end is Tail Insertion
    if (curr->next == NULL) {
        return insert_tail(head, tail, val);
    }

    // 3. Middle Insertion (Connect 4 pointers cleanly)
    struct Node *p = create_node(val);
    if (p == NULL) return head;

    p->next = curr->next;
    p->prev = curr;
    curr->next->prev = p; // Connect node AFTER p back to p
    curr->next = p;       // Connect curr node forward to p

    return head;
}

int main() {
    struct Node *head = NULL;
    struct Node *tail = NULL;

    // 1. Build list using insert_tail (O(1))
    head = insert_tail(head, &tail, 10);
    head = insert_tail(head, &tail, 30);

    printf("--- Initial List (10 -> 30) ---\n");
    print_forward(head);  // Forward: 10 -> 30 -> NULL
    print_backward(head); // Backward: 30 -> 10 -> NULL

    // 2. Insert 20 at position 2 (Middle Insertion)
    head = insert_at_pos(head, &tail, 20, 2);

    printf("\n--- After Inserting 20 at Position 2 ---\n");
    print_forward(head);  // Forward: 10 -> 20 -> 30 -> NULL
    print_backward(head); // Backward: 30 -> 20 -> 10 -> NULL

    // 3. Reverse in-place
    head = reverse_dll(head);

    printf("\n--- After In-Place Reversal ---\n");
    print_forward(head);  // Forward: 30 -> 20 -> 10 -> NULL
    print_backward(head); // Backward: 10 -> 20 -> 30 -> NULL

    return 0;
}