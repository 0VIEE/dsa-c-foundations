#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *link;
};

// Function to create a node
struct Node* create_node(int val) {
    struct Node *p = (struct Node*)malloc(sizeof(struct Node));
    if (p == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    p->data = val;
    p->link = NULL;
    return p;
}

// Function to insert at the head
struct Node* insert_head(struct Node *head, int val) {
    struct Node *p = create_node(val);
    if (p == NULL) return head;
    
    p->link = head;
    head = p;
    return head;
}

// Function to print the list
void print_list(struct Node *head) {
    struct Node *p = head;
    while (p != NULL) {
        printf("%d -> ", p->data);
        p = p->link;
    }
    printf("NULL\n");
}

struct Node* insert_tail(struct Node *head, int val) {
    struct Node *temp = create_node(val);
    if (temp == NULL) return head;

    // If list is empty, new node becomes head
    if (head == NULL) {
        return temp;
    }

    // Traverse to the last node
    struct Node *p = head;
    while (p->link != NULL) {
        p = p->link;
    }

    // Link the last node to the new node
    p->link = temp;
    return head;
}

// Function to delete the head node
struct Node* delete_head(struct Node *head) {
    if (head == NULL) {
        printf("List is empty, nothing to delete!\n");
        return NULL;
    }

    struct Node *p = head;
    head = head->link;
    free(p);

    return head;
}

// Function to delete the tail node
struct Node* delete_tail(struct Node *head) {
    if (head == NULL) {
        printf("List is empty, nothing to delete!\n");
        return NULL;
    }

    // Special case: Only one node in the list
    if (head->link == NULL) {
        free(head);
        return NULL;
    }

    // Traverse to the second-to-last node using pointer p
    struct Node *p = head;
    while (p->link->link != NULL) {
        p = p->link;
    }

    // Free the tail node and break the link
    free(p->link);
    p->link = NULL;

    return head;
}

// Function to search for a value in the list
int search_node(struct Node *head, int key) {
    struct Node *p = head;
    int pos = 1;

    while (p != NULL) {
        if (p->data == key) {
            return pos; // Found key at this position
        }
        p = p->link;
        pos++;
    }

    return -1; // Key not found
}

// Function to count total nodes in the list
int get_length(struct Node *head) {
    int count = 0;
    struct Node *p = head;

    while (p != NULL) {
        count++;
        p = p->link;
    }

    return count;
}

int main() {
    struct Node *head = NULL;

    // 1. Build the list using insert_tail
    head = insert_tail(head, 10);
    head = insert_tail(head, 20);
    head = insert_tail(head, 30);
    head = insert_tail(head, 40);

    printf("Original List:\n");
    print_list(head); // Prints: 10 -> 20 -> 30 -> 40 -> NULL

    // Check length of original list
    printf("Total nodes: %d\n", get_length(head)); // Prints: 4

    // 2. Test search_node before deletion
    int target = 30;
    int pos = search_node(head, target);
    if (pos != -1) {
        printf("Element %d found at position: %d\n", target, pos);
    } else {
        printf("Element %d not found in the list.\n", target);
    }

    // 3. Delete tail
    head = delete_tail(head);

    printf("\nList after delete_tail:\n");
    print_list(head); // Prints: 10 -> 20 -> 30 -> NULL

    // Check length after deletion
    printf("Total nodes: %d\n", get_length(head)); // Prints: 3

    // 4. Test search_node for deleted element
    target = 40;
    pos = search_node(head, target);
    if (pos != -1) {
        printf("Element %d found at position: %d\n", target, pos);
    } else {
        printf("Element %d not found in the list.\n", target);
    }

    return 0;
}