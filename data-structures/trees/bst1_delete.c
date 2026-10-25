#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *left;
    struct Node *right;
};

struct Node* create_node(int val) {
    struct Node *node = (struct Node*)malloc(sizeof(struct Node));
    if (node == NULL) return NULL;
    node->data = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

struct Node* insert(struct Node *root, int val) {
    if (root == NULL) return create_node(val);
    if (val < root->data) root->left = insert(root->left, val);
    else if (val > root->data) root->right = insert(root->right, val);
    return root;
}

// Find minimum value node (Inorder Successor helper)
struct Node* find_min(struct Node *root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

// BST Node Deletion
struct Node* delete_node(struct Node *root, int key) {
    if (root == NULL) return NULL;

    if (key < root->data) {
        root->left = delete_node(root->left, key);
    } else if (key > root->data) {
        root->right = delete_node(root->right, key);
    } else {
        // Target node found!

        // Case 1 & 2: 0 or 1 child
        if (root->left == NULL) {
            struct Node *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Node *temp = root->left;
            free(root);
            return temp;
        }

        // Case 3: 2 children
        struct Node *temp = find_min(root->right); // Inorder Successor
        root->data = temp->data;                    // Copy successor value
        root->right = delete_node(root->right, temp->data); // Delete successor
    }
    return root;
}

void inorder(struct Node *root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

int main() {
    struct Node *root = NULL;
    int keys[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++) root = insert(root, keys[i]);

    printf("Original Inorder: ");
    inorder(root);
    printf("\n");

    // Deleting leaf node (Case 1)
    root = delete_node(root, 20);
    printf("After deleting 20 (leaf): ");
    inorder(root);
    printf("\n");

    // Deleting node with 2 children (Case 3 - Root 50)
    root = delete_node(root, 50);
    printf("After deleting 50 (root with 2 kids): ");
    inorder(root);
    printf("\n");

    return 0;
}