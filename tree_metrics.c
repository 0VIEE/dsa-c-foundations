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

// 1. Calculate Maximum Depth / Height of Tree
int get_height(struct Node *root) {
    if (root == NULL) return 0;
    
    int left_h = get_height(root->left);
    int right_h = get_height(root->right);
    
    return (left_h > right_h ? left_h : right_h) + 1;
}

// 2. Count Total Number of Leaf Nodes
int count_leaves(struct Node *root) {
    if (root == NULL) return 0;
    if (root->left == NULL && root->right == NULL) return 1;
    
    return count_leaves(root->left) + count_leaves(root->right);
}

int main() {
    /*
             10
            /  \
           5    20
          / \     \
         3   7     30
    */
    struct Node *root = create_node(10);
    root->left = create_node(5);
    root->right = create_node(20);
    root->left->left = create_node(3);
    root->left->right = create_node(7);
    root->right->right = create_node(30);

    printf("=== Binary Tree Metrics ===\n");
    printf("Tree Height: %d\n", get_height(root));       // Output: 3
    printf("Leaf Count:  %d\n", count_leaves(root));     // Output: 3 (nodes 3, 7, 30)

    return 0;
}