#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *link;
};

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

void print_list(struct Node *head) {
    struct Node *p = head;
    while (p != NULL) {
        printf("%d -> ", p->data);
        p = p->link;
    }
    printf("NULL\n");
}

int main() {
    struct Node *head = create_node(10);
    head->link = create_node(20);
    head->link->link = create_node(30);

    print_list(head);
    return 0;
}