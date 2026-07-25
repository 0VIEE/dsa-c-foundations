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

struct Node* insert_head(struct Node *head, int val) {
    struct Node *p = create_node(val);
    if (p == NULL) return head;

    p->link = head;
    head = p;
    return head;
}

void print_list(struct Node *head) {
    struct Node *p = head;
    while (p != NULL) {
        printf("%d -> ", p->data);
        p = p->link;
    }
    printf("NULL\n");
}

void get_sum_and_count(struct Node *head) {
    struct Node *p = head;
    int sum = 0;
    int count = 0;

    while (p != NULL) {
        sum += p->data;
        count++;
        p = p->link;
    }
    printf("Total Nodes: %d\n", count);
    printf("Sum: %d\n", sum);
}

int find_max(struct Node *head){
    struct Node *p = head;

    int max = p->data;
    while (p != NULL) {
        if (p->data > max) {
            max = p->data;
        }
        p = p->link;
    }
    return max;
}

int search_key(struct Node *head, int key){
    struct Node *p = head;

    while (p != NULL){
        if (p -> data == key ){
            return 1;
        }
        p = p -> link;
    }
    return 0;
}

int main() 
{
    struct Node *head = NULL;

    // Inserting elements at the head
    head = insert_head(head, 30);
    head = insert_head(head, 20);
    head = insert_head(head, 10);

    printf("Linked List after insertions:\n");
    print_list(head);

    get_sum_and_count(head);

    int max = find_max(head);
    printf("Maximum value in the list: %d\n", max);

    int find_key = search_key(head, 20);
    
    if (find_key == 1){
        printf("The key is there");
    }
    else{
        printf("not present");
    }

    return 0;
}