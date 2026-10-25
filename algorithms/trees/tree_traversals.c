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

// 1. Preorder Traversal: Root -> Left -> Right
void preorder_traversal(struct Node *root) {
    if (root == NULL) return;
    printf("%d ", root->data);       // 1. Root
    preorder_traversal(root->left);  // 2. Left
    preorder_traversal(root->right); // 3. Right
}

// 2. Postorder Traversal: Left -> Right -> Root
void postorder_traversal(struct Node *root) {
    if (root == NULL) return;
    postorder_traversal(root->left);   // 1. Left
    postorder_traversal(root->right);  // 2. Right
    printf("%d ", root->data);        // 3. Root
}

int main() {
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

    printf("=== Tree Traversal Testing ===\n");
    
    printf("Preorder  (Root -> Left -> Right): ");
    preorder_traversal(root);  // Output: 1 2 4 5 3
    printf("\n");

    printf("Postorder (Left -> Right -> Root): ");
    postorder_traversal(root); // Output: 4 5 2 3 1
    printf("\n");

    return 0;
}