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

static int lowercase_index(unsigned char ch) {
    if (ch < 'a' || ch > 'z') {
        return -1;
    }
    return ch - 'a';
}

static int valid_word(const char *word, int allow_empty) {
    if (word == NULL) {
        return 0;
    }

    size_t len = strlen(word);
    if ((!allow_empty && len == 0) || len >= MAX_WORD_LEN) {
        return 0;
    }

    for (size_t i = 0; i < len; i++) {
        if (lowercase_index((unsigned char)word[i]) < 0) {
            return 0;
        }
    }
    return 1;
}

struct TrieNode *create_node(void) {
    return (struct TrieNode *)calloc(1, sizeof(struct TrieNode));
}

/* Returns 1 on success; accepts non-empty lowercase words shorter than 50 chars. */
int insert(struct TrieNode *root, const char *key) {
    if (root == NULL || !valid_word(key, 0)) {
        return 0;
    }

    struct TrieNode *node = root;
    for (size_t i = 0; key[i] != '\0'; i++) {
        int index = lowercase_index((unsigned char)key[i]);
        if (node->link[index] == NULL) {
            node->link[index] = create_node();
            if (node->link[index] == NULL) {
                return 0;
            }
        }
        node = node->link[index];
    }

    node->is_end_of_word = true;
    return 1;
}

void suggestions_rec(struct TrieNode *root, char curr_prefix[], size_t level) {
    if (root == NULL) {
        return;
    }

    if (root->is_end_of_word) {
        curr_prefix[level] = '\0';
        printf(" -> %s\n", curr_prefix);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->link[i] != NULL) {
            curr_prefix[level] = (char)('a' + i);
            suggestions_rec(root->link[i], curr_prefix, level + 1);
        }
    }
}

/* Returns 1 for a valid prefix, including when no words match it. */
int print_autocomplete_suggestions(struct TrieNode *root, const char *prefix) {
    if (root == NULL || !valid_word(prefix, 1)) {
        return 0;
    }

    size_t len = strlen(prefix);
    struct TrieNode *node = root;
    for (size_t i = 0; i < len; i++) {
        int index = lowercase_index((unsigned char)prefix[i]);
        if (node->link[index] == NULL) {
            printf("No suggestions found for prefix \"%s\"\n", prefix);
            return 1;
        }
        node = node->link[index];
    }

    char curr_prefix[MAX_WORD_LEN];
    memcpy(curr_prefix, prefix, len + 1);

    printf("Autocomplete suggestions for \"%s\":\n", prefix);
    suggestions_rec(node, curr_prefix, len);
    return 1;
}

void free_trie(struct TrieNode *root) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        free_trie(root->link[i]);
    }
    free(root);
}

#ifndef TRIE_AUTOCOMPLETE_NO_MAIN
int main(void) {
    struct TrieNode *root = create_node();
    if (root == NULL) {
        fputs("Failed to allocate the trie root.\n", stderr);
        return EXIT_FAILURE;
    }

    const char *dict[] = {
        "cat", "cater", "category", "cattle", "candy",
        "can", "candle", "dog", "dodge", "door"
    };
    size_t count = sizeof(dict) / sizeof(dict[0]);

    for (size_t i = 0; i < count; i++) {
        if (!insert(root, dict[i])) {
            fputs("Failed to insert a dictionary word.\n", stderr);
            free_trie(root);
            return EXIT_FAILURE;
        }
    }

    printf("=== Trie Autocomplete ===\n");
    if (!print_autocomplete_suggestions(root, "cat") ||
        !print_autocomplete_suggestions(root, "can") ||
        !print_autocomplete_suggestions(root, "do") ||
        !print_autocomplete_suggestions(root, "xyz")) {
        fputs("Invalid autocomplete prefix.\n", stderr);
        free_trie(root);
        return EXIT_FAILURE;
    }

    free_trie(root);
    return EXIT_SUCCESS;
}
#endif
