#include <stdio.h>
#include <stdlib.h>

// Binary Tree Node Struct
struct Node {
    int data;
    struct Node *left;
    struct Node *right;
};

// Function to allocate and initialize a new node
struct Node* create_node(int val) {
    struct Node *node = (struct Node*)malloc(sizeof(struct Node));
    if (node == NULL) {
        printf("Memory Allocation Failed!\n");
        return NULL;
    }
    node->data = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Recursive Inorder Traversal: Left -> Root -> Right
void inorder_traversal(struct Node *root) {
    if (root == NULL) {
        return;
    }
    
    inorder_traversal(root->left);  // 1. Traverse Left Subtree
    printf("%d ", root->data);      // 2. Visit Root Node
    inorder_traversal(root->right); // 3. Traverse Right Subtree
}

int main() {
    printf("=== Building Binary Tree ===\n");
    /*
             1
            / \
           2   3
          / \
         4   5
    */
    struct Node *root = create_node(1);
    root->left = create_node(2);
    root->right = create_node(3);
    root->left->left = create_node(4);
    root->left->right = create_node(5);

    printf("Inorder Traversal Output: ");
    inorder_traversal(root); // Output: 4 2 5 1 3
    printf("\n");

    return 0;
}