#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define MAX_PATTERN_LENGTH 31

struct PatternOutput {
    int pattern_id;
    size_t length;
    struct PatternOutput *next;
};

struct TrieNode {
    struct TrieNode *link[ALPHABET_SIZE];
    struct TrieNode *fail;
    struct PatternOutput *outputs;
    int built; /* Used on the root to prevent insertion after building links. */
};

struct QueueNode {
    struct TrieNode *data;
    struct QueueNode *link;
};

struct Queue {
    struct QueueNode *front;
    struct QueueNode *rear;
};

static int lowercase_index(unsigned char ch) {
    if (ch < 'a' || ch > 'z') {
        return -1;
    }
    return ch - 'a';
}

struct TrieNode *create_node(void) {
    return (struct TrieNode *)calloc(1, sizeof(struct TrieNode));
}

/*
 * Insert a lowercase pattern with its index in the caller's pattern array.
 * Repeated strings may have multiple IDs and will report each registered ID.
 */
int insert_pattern(struct TrieNode *root, const char *pat, int id) {
    if (root == NULL || pat == NULL || id < 0 || root->built) {
        return 0;
    }

    size_t len = strlen(pat);
    if (len == 0 || len > MAX_PATTERN_LENGTH) {
        return 0;
    }

    for (size_t i = 0; i < len; i++) {
        if (lowercase_index((unsigned char)pat[i]) < 0) {
            return 0;
        }
    }

    struct TrieNode *node = root;
    for (size_t i = 0; i < len; i++) {
        int c = lowercase_index((unsigned char)pat[i]);
        if (node->link[c] == NULL) {
            node->link[c] = create_node();
            if (node->link[c] == NULL) {
                return 0;
            }
        }
        node = node->link[c];
    }

    for (struct PatternOutput *out = node->outputs; out != NULL; out = out->next) {
        if (out->pattern_id == id) {
            return 1;
        }
    }

    struct PatternOutput *out = (struct PatternOutput *)malloc(sizeof(*out));
    if (out == NULL) {
        return 0;
    }
    out->pattern_id = id;
    out->length = len;
    out->next = node->outputs;
    node->outputs = out;
    return 1;
}

struct Queue *create_queue(void) {
    return (struct Queue *)calloc(1, sizeof(struct Queue));
}

int enqueue(struct Queue *q, struct TrieNode *node) {
    if (q == NULL || node == NULL) {
        return 0;
    }

    struct QueueNode *item = (struct QueueNode *)malloc(sizeof(*item));
    if (item == NULL) {
        return 0;
    }
    item->data = node;
    item->link = NULL;

    if (q->rear == NULL) {
        q->front = item;
        q->rear = item;
    } else {
        q->rear->link = item;
        q->rear = item;
    }
    return 1;
}

struct TrieNode *dequeue(struct Queue *q) {
    if (q == NULL || q->front == NULL) {
        return NULL;
    }

    struct QueueNode *item = q->front;
    struct TrieNode *node = item->data;
    q->front = item->link;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(item);
    return node;
}

void free_queue(struct Queue *q) {
    if (q == NULL) {
        return;
    }
    while (q->front != NULL) {
        struct QueueNode *item = q->front;
        q->front = item->link;
        free(item);
    }
    free(q);
}

int build_failure_links(struct TrieNode *root) {
    if (root == NULL) {
        return 0;
    }
    if (root->built) {
        return 1;
    }

    struct Queue *q = create_queue();
    if (q == NULL) {
        return 0;
    }

    for (int c = 0; c < ALPHABET_SIZE; c++) {
        if (root->link[c] != NULL) {
            root->link[c]->fail = root;
            if (!enqueue(q, root->link[c])) {
                free_queue(q);
                return 0;
            }
        }
    }

    while (q->front != NULL) {
        struct TrieNode *node = dequeue(q);
        for (int c = 0; c < ALPHABET_SIZE; c++) {
            struct TrieNode *child = node->link[c];
            if (child == NULL) {
                continue;
            }

            struct TrieNode *fallback = node->fail;
            while (fallback != NULL && fallback->link[c] == NULL) {
                fallback = fallback->fail;
            }
            child->fail = (fallback != NULL) ? fallback->link[c] : root;

            if (!enqueue(q, child)) {
                free_queue(q);
                return 0;
            }
        }
    }

    free_queue(q);
    root->built = 1;
    return 1;
}

/* Returns 1 after a successful search, or 0 for invalid arguments/input. */
int aho_corasick_search(
    struct TrieNode *root,
    const char *text,
    const char *patterns[],
    size_t pattern_count
) {
    if (root == NULL || text == NULL || !root->built ||
        (pattern_count > 0 && patterns == NULL)) {
        return 0;
    }

    size_t text_len = strlen(text);
    for (size_t i = 0; i < text_len; i++) {
        if (lowercase_index((unsigned char)text[i]) < 0) {
            return 0;
        }
    }

    struct TrieNode *node = root;
    printf("Search Text: \"%s\"\n", text);
    printf("Matches detected:\n");

    for (size_t i = 0; i < text_len; i++) {
        int c = lowercase_index((unsigned char)text[i]);

        while (node != root && node->link[c] == NULL) {
            node = node->fail;
        }
        if (node->link[c] != NULL) {
            node = node->link[c];
        } else {
            node = root;
        }

        for (struct TrieNode *match = node; match != NULL; match = match->fail) {
            for (struct PatternOutput *out = match->outputs; out != NULL; out = out->next) {
                if (out->pattern_id < 0 ||
                    (size_t)out->pattern_id >= pattern_count ||
                    patterns[out->pattern_id] == NULL ||
                    strlen(patterns[out->pattern_id]) != out->length) {
                    return 0;
                }

                size_t start_pos = i + 1 - out->length;
                printf("  -> Pattern [ID %d: \"%s\"] at text index %zu\n",
                       out->pattern_id, patterns[out->pattern_id], start_pos);
            }
        }
    }

    printf("\n");
    return 1;
}

void free_trie(struct TrieNode *root) {
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        free_trie(root->link[i]);
    }

    struct PatternOutput *out = root->outputs;
    while (out != NULL) {
        struct PatternOutput *next = out->next;
        free(out);
        out = next;
    }
    free(root);
}

#ifndef AHO_CORASICK_NO_MAIN
int main(void) {
    struct TrieNode *root = create_node();
    if (root == NULL) {
        fputs("Failed to allocate the trie root.\n", stderr);
        return EXIT_FAILURE;
    }

    const char *patterns[] = {"he", "she", "his", "hers"};
    size_t pattern_count = sizeof(patterns) / sizeof(patterns[0]);

    for (size_t i = 0; i < pattern_count; i++) {
        if (!insert_pattern(root, patterns[i], (int)i)) {
            fputs("Failed to insert a pattern.\n", stderr);
            free_trie(root);
            return EXIT_FAILURE;
        }
    }

    if (!build_failure_links(root)) {
        fputs("Failed to build failure links.\n", stderr);
        free_trie(root);
        return EXIT_FAILURE;
    }

    printf("=== Aho-Corasick Multi-Pattern Search ===\n");
    printf("Registered Patterns: [ \"he\", \"she\", \"his\", \"hers\" ]\n\n");

    if (!aho_corasick_search(root, "ahishers", patterns, pattern_count)) {
        fputs("Failed to search the sample text.\n", stderr);
        free_trie(root);
        return EXIT_FAILURE;
    }

    free_trie(root);
    return EXIT_SUCCESS;
}
#endif
