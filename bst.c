#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *left;
    struct Node *right;
};

// Create a new BST node
struct Node* create_node(int val) {
    struct Node *node = (struct Node*)malloc(sizeof(struct Node));
    if (node == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    node->data = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Recursive BST Insertion - O(log N) average
struct Node* insert_bst(struct Node *root, int val) {
    if (root == NULL) {
        return create_node(val);
    }
    if (val < root->data) {
        root->left = insert_bst(root->left, val);
    } else if (val > root->data) {
        root->right = insert_bst(root->right, val);
    }
    return root;
}

// Recursive BST Search - O(log N) average
int search_bst(struct Node *root, int key) {
    if (root == NULL) {
        return 0; // Not found
    }
    if (root->data == key) {
        return 1; // Found
    }
    if (key < root->data) {
        return search_bst(root->left, key);
    } else {
        return search_bst(root->right, key);
    }
}

// Inorder Traversal prints sorted order
void inorder(struct Node *root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

int main() {
    struct Node *root = NULL;

    printf("=== Inserting elements into BST ===\n");
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++) {
        root = insert_bst(root, values[i]);
    }

    printf("Sorted Inorder Traversal: ");
    inorder(root); // Output: 20 30 40 50 60 70 80
    printf("\n\n");

    // Search tests
    int key1 = 40, key2 = 99;
    printf("Search %d: %s\n", key1, search_bst(root, key1) ? "FOUND" : "NOT FOUND");
    printf("Search %d: %s\n", key2, search_bst(root, key2) ? "FOUND" : "NOT FOUND");

    return 0;
}