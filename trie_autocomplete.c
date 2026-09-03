#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define MAX_WORD_LEN 50

struct TrieNode {
    struct TrieNode *link[ALPHABET_SIZE];
    bool is_end_of_word;
};

// Create a new Trie node
struct TrieNode* create_node() {
    struct TrieNode *p = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    if (p == NULL) return NULL;

    p->is_end_of_word = false;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        p->link[i] = NULL;
    }
    return p;
}

// Insert word into Trie
void insert(struct TrieNode *root, const char *key) {
    struct TrieNode *p = root;
    int len = strlen(key);

    for (int i = 0; i < len; i++) {
        int index = key[i] - 'a';
        if (p->link[index] == NULL) {
            p->link[index] = create_node();
        }
        p = p->link[index];
    }
    p->is_end_of_word = true;
}

// Recursive DFS to find and print all words under a prefix node
void suggestions_rec(struct TrieNode *root, char curr_prefix[], int level) {
    if (root == NULL) return;

    if (root->is_end_of_word) {
        curr_prefix[level] = '\0';
        printf(" -> %s\n", curr_prefix);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->link[i] != NULL) {
            curr_prefix[level] = 'a' + i;
            suggestions_rec(root->link[i], curr_prefix, level + 1);
        }
    }
}

// Find prefix and initiate suggestions
void print_autocomplete_suggestions(struct TrieNode *root, const char *prefix) {
    struct TrieNode *p = root;
    int len = strlen(prefix);

    // Locate the node corresponding to prefix
    for (int i = 0; i < len; i++) {
        int index = prefix[i] - 'a';
        if (p->link[index] == NULL) {
            printf("No suggestions found for prefix \"%s\"\n", prefix);
            return;
        }
        p = p->link[index];
    }

    char curr_prefix[MAX_WORD_LEN];
    strcpy(curr_prefix, prefix);

    printf("Autocomplete suggestions for \"%s\":\n", prefix);
    suggestions_rec(p, curr_prefix, len);
}

// Post-order free of Trie memory
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

    char dict[][MAX_WORD_LEN] = {
        "cat", "cater", "category", "cattle", "candy", 
        "can", "candle", "dog", "dodge", "door"
    };
    int n = sizeof(dict) / sizeof(dict[0]);

    for (int i = 0; i < n; i++) {
        insert(root, dict[i]);
    }

    printf("=== Trie Autocomplete Engine ===\n");
    print_autocomplete_suggestions(root, "cat");
    printf("\n");
    print_autocomplete_suggestions(root, "can");
    printf("\n");
    print_autocomplete_suggestions(root, "do");
    printf("\n");
    print_autocomplete_suggestions(root, "xyz");

    free_trie(root);
    return 0;
}