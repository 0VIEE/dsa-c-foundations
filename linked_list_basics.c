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

// Function to reverse the linked list iteratively
struct Node* reverse_list(struct Node *head) {
    struct Node *prev = NULL;
    struct Node *p = head;
    struct Node *next = NULL;

    while (p != NULL) {
        next = p->link;  // Store next node
        p->link = prev;  // Reverse current node's pointer
        prev = p;        // Move prev forward
        p = next;        // Move p forward
    }

    head = prev; // New head is the last non-null node processed
    return head;
}

// Function to find the middle node of the list
struct Node* find_middle(struct Node *head) {
    if (head == NULL) return NULL;

    struct Node *slow = head;
    struct Node *fast = head;

    while (fast != NULL && fast->link != NULL) {
        slow = slow->link;         // Moves 1 step
        fast = fast->link->link;   // Moves 2 steps
    }

    return slow; // Points to the middle element
}

// Function to check if a linked list contains a cycle (Floyd's Algorithm)
int has_cycle(struct Node *head) {
    if (head == NULL) return 0; // 0 = No cycle

    struct Node *slow = head;
    struct Node *fast = head;

    while (fast != NULL && fast->link != NULL) {
        slow = slow->link;         // Moves 1 step
        fast = fast->link->link;   // Moves 2 steps

        if (slow == fast) {
            return 1; // 1 = Cycle detected!
        }
    }

    return 0; // Reached end of list -> No cycle
}

// Function to find the starting node of a cycle
struct Node* detect_cycle_start(struct Node *head) {
    if (head == NULL) return NULL;

    struct Node *slow = head;
    struct Node *fast = head;
    int has_loop = 0;

    // Phase 1: Detect if a cycle exists
    while (fast != NULL && fast->link != NULL) {
        slow = slow->link;
        fast = fast->link->link;

        if (slow == fast) {
            has_loop = 1;
            break;
        }
    }

    // If no cycle exists, return NULL
    if (!has_loop) return NULL;

    // Phase 2: Reset slow to head and move both pointers 1 step at a time
    slow = head;
    while (slow != fast) {
        slow = slow->link;
        fast = fast->link;
    }

    return slow; // Returns pointer to the exact entry node of the loop
}

// Function to remove a cycle from a linked list
struct Node* remove_cycle(struct Node *head) {
    struct Node *start_node = detect_cycle_start(head);

    // If no cycle exists, nothing to break
    if (start_node == NULL) {
        return head;
    }

    // Traverse to the node right before start_node inside the cycle
    struct Node *p = start_node;
    while (p->link != start_node) {
        p = p->link;
    }

    // Break the loop
    p->link = NULL;

    return head;
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

    // 2. Test detect_cycle_start on a normal linear list
    struct Node *cycle_start = detect_cycle_start(head);
    if (cycle_start != NULL) {
        printf("Cycle start detected at node value: %d\n", cycle_start->data);
    } else {
        printf("No cycle start found in linear list. (Correct!)\n");
    }

    // 3. Manually create a cycle for testing (40 -> 20)
    struct Node *p = head;
    while (p->link != NULL) {
        p = p->link;
    }
    // Point tail's link back to second node (20)
    p->link = head->link; 

    // 4. Test detect_cycle_start on cyclic list
    cycle_start = detect_cycle_start(head);
    if (cycle_start != NULL) {
        printf("Cycle start detected successfully at node value: %d (Expected: 20)\n", cycle_start->data);
    } else {
        printf("Failed to detect cycle start.\n");
    }

    // 5. Test remove_cycle to break the loop programmatically
    head = remove_cycle(head);

    printf("\nList after remove_cycle:\n");
    print_list(head); // Safely prints: 10 -> 20 -> 30 -> 40 -> NULL

    // Verify cycle is completely gone
    if (has_cycle(head)) {
        printf("Failed to remove cycle.\n");
    } else {
        printf("Cycle removed successfully! List is linear again. (Correct!)\n");
    }

    return 0;
}