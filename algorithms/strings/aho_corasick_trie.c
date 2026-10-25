#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26
#define MAX_STATES 100

struct TrieNode {
    struct TrieNode *link[ALPHABET_SIZE];
    struct TrieNode *fail;
    int pattern_id; // -1 if not a terminal match
};

struct TrieNode* create_node() {
    struct TrieNode *p = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    p->fail = NULL;
    p->pattern_id = -1;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        p->link[i] = NULL;
    }
    return p;
}

// Insert pattern into Trie
void insert_pattern(struct TrieNode *root, const char *pat, int id) {
    struct TrieNode *p = root;
    for (int i = 0; pat[i] != '\0'; i++) {
        int idx = pat[i] - 'a';
        if (p->link[idx] == NULL) {
            p->link[idx] = create_node();
        }
        p = p->link[idx];
    }
    p->pattern_id = id;
}

// Queue for BFS failure link construction
struct QueueNode {
    struct TrieNode *data;
    struct QueueNode *link;
};

struct Queue {
    struct QueueNode *front;
    struct QueueNode *rear;
};

struct Queue* create_queue() {
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

void enqueue(struct Queue *q, struct TrieNode *node) {
    struct QueueNode *p = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    p->data = node;
    p->link = NULL;
    if (q->rear == NULL) {
        q->front = p;
        q->rear = p;
        return;
    }
    q->rear->link = p;
    q->rear = p;
}

struct TrieNode* dequeue(struct Queue *q) {
    if (q->front == NULL) return NULL;
    struct QueueNode *p = q->front;
    struct TrieNode *val = p->data;
    q->front = q->front->link;
    if (q->front == NULL) q->rear = NULL;
    free(p);
    return val;
}

bool is_queue_empty(struct Queue *q) {
    return q->front == NULL;
}

// Build BFS Failure Links
void build_failure_links(struct TrieNode *root) {
    struct Queue *q = create_queue();

    // Depth 1 states fail back to root
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->link[i] != NULL) {
            root->link[i]->fail = root;
            enqueue(q, root->link[i]);
        }
    }

    while (!is_queue_empty(q)) {
        struct TrieNode *u = dequeue(q);

        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (u->link[i] != NULL) {
                struct TrieNode *v = u->link[i];
                struct TrieNode *f = u->fail;

                while (f != NULL && f->link[i] == NULL) {
                    f = f->fail;
                }
                v->fail = (f != NULL) ? f->link[i] : root;
                enqueue(q, v);
            }
        }
    }
    free(q);
}

// Search text for all patterns simultaneously
void aho_corasick_search(struct TrieNode *root, const char *text, const char *patterns[]) {
    struct TrieNode *p = root;
    int n = strlen(text);

    printf("Search Text: \"%s\"\n", text);
    printf("Matches detected:\n");

    for (int i = 0; i < n; i++) {
        int idx = text[i] - 'a';

        // Follow failure links if current transition doesn't exist
        while (p != NULL && p->link[idx] == NULL) {
            p = p->fail;
        }

        p = (p == NULL) ? root : p->link[idx];

        // Check matches at current node and along failure chain
        struct TrieNode *temp = p;
        while (temp != NULL) {
            if (temp->pattern_id != -1) {
                int id = temp->pattern_id;
                int start_pos = i - strlen(patterns[id]) + 1;
                printf("  -> Pattern [ID %d: \"%s\"] at text index %d\n", id, patterns[id], start_pos);
            }
            temp = temp->fail;
        }
    }
    printf("\n");
}

void free_trie(struct TrieNode *root) {
    if (root == NULL) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->link[i] != NULL) {
            free_trie(root->link[i]);
        }
    }
    free(root);
}

int main() {
    struct TrieNode *root = create_node();

    const char *patterns[] = {"he", "she", "his", "hers"};
    int k = sizeof(patterns) / sizeof(patterns[0]);

    for (int i = 0; i < k; i++) {
        insert_pattern(root, patterns[i], i);
    }

    build_failure_links(root);

    printf("=== Aho-Corasick Multi-Pattern Search Engine ===\n");
    printf("Registered Patterns: [ \"he\", \"she\", \"his\", \"hers\" ]\n\n");

    const char *text = "ahishers";
    aho_corasick_search(root, text, patterns);

    free_trie(root);
    return 0;
}