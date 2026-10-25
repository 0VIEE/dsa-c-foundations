#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 128
#define ALPHABET_SIZE 26
#define MAX_PATTERN_LENGTH 31

struct ACNode {
    int next[ALPHABET_SIZE];
    int link;
    int dict_link;
    int is_terminal;
    char pattern[MAX_PATTERN_LENGTH + 1];
};

struct ACAry {
    struct ACNode nodes[MAX_NODES];
    int sz;
    int built;
};

static int lowercase_index(unsigned char ch) {
    if (ch < 'a' || ch > 'z') {
        return -1;
    }
    return ch - 'a';
}

struct ACAry *create_ac(void) {
    struct ACAry *ac = (struct ACAry *)calloc(1, sizeof(*ac));
    if (ac == NULL) {
        return NULL;
    }

    ac->sz = 1; /* Node zero is the root. */
    return ac;
}

/*
 * Insert a non-empty lowercase pattern. Duplicate strings are treated as a
 * single pattern. Returns 1 on success and 0 for invalid input or capacity
 * limits. Insertions are not allowed after the automaton has been built.
 */
int insert_pattern(struct ACAry *ac, const char *pat) {
    if (ac == NULL || pat == NULL || ac->built) {
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

    /* Count missing nodes before changing the trie, so capacity failure is safe. */
    int node = 0;
    size_t matched = 0;
    while (matched < len) {
        int c = lowercase_index((unsigned char)pat[matched]);
        int next = ac->nodes[node].next[c];
        if (next == 0) {
            break;
        }
        node = next;
        matched++;
    }

    if (matched == len && ac->nodes[node].is_terminal) {
        return 1;
    }

    size_t missing = len - matched;
    if ((size_t)ac->sz + missing > MAX_NODES) {
        return 0;
    }

    node = 0;
    for (size_t i = 0; i < len; i++) {
        int c = lowercase_index((unsigned char)pat[i]);
        if (ac->nodes[node].next[c] == 0) {
            int new_node = ac->sz++;
            ac->nodes[node].next[c] = new_node;
        }
        node = ac->nodes[node].next[c];
    }

    ac->nodes[node].is_terminal = 1;
    memcpy(ac->nodes[node].pattern, pat, len + 1);
    return 1;
}

int build_automaton(struct ACAry *ac) {
    if (ac == NULL) {
        return 0;
    }
    if (ac->built) {
        return 1;
    }

    int queue[MAX_NODES];
    int head = 0;
    int tail = 0;

    for (int c = 0; c < ALPHABET_SIZE; c++) {
        int child = ac->nodes[0].next[c];
        if (child != 0) {
            ac->nodes[child].link = 0;
            ac->nodes[child].dict_link = 0;
            queue[tail++] = child;
        }
    }

    while (head < tail) {
        int node = queue[head++];
        int fail = ac->nodes[node].link;

        ac->nodes[node].dict_link = ac->nodes[fail].is_terminal
            ? fail
            : ac->nodes[fail].dict_link;

        for (int c = 0; c < ALPHABET_SIZE; c++) {
            int child = ac->nodes[node].next[c];
            if (child != 0) {
                ac->nodes[child].link = ac->nodes[fail].next[c];
                queue[tail++] = child;
            } else {
                ac->nodes[node].next[c] = ac->nodes[fail].next[c];
            }
        }
    }

    ac->built = 1;
    return 1;
}

/* Returns 1 after a successful search, or 0 for invalid arguments/input. */
int search_text(struct ACAry *ac, const char *text) {
    if (ac == NULL || text == NULL || !ac->built) {
        return 0;
    }

    size_t len = strlen(text);
    for (size_t i = 0; i < len; i++) {
        if (lowercase_index((unsigned char)text[i]) < 0) {
            return 0;
        }
    }

    int node = 0;
    printf("Searching text: \"%s\"\n", text);
    for (size_t i = 0; i < len; i++) {
        int c = lowercase_index((unsigned char)text[i]);
        node = ac->nodes[node].next[c];

        int match = node;
        while (match != 0) {
            if (ac->nodes[match].is_terminal) {
                printf("  -> Found pattern \"%s\" ending at index %zu\n",
                       ac->nodes[match].pattern, i);
            }
            match = ac->nodes[match].dict_link;
        }
    }

    return 1;
}

void free_ac(struct ACAry *ac) {
    free(ac);
}

#ifndef AHO_CORASICK_NO_MAIN
int main(void) {
    struct ACAry *ac = create_ac();
    if (ac == NULL) {
        fputs("Failed to allocate the Aho-Corasick automaton.\n", stderr);
        return EXIT_FAILURE;
    }

    if (!insert_pattern(ac, "he") ||
        !insert_pattern(ac, "she") ||
        !insert_pattern(ac, "his") ||
        !insert_pattern(ac, "hers") ||
        !build_automaton(ac) ||
        !search_text(ac, "ushers")) {
        fputs("Failed to build or run the Aho-Corasick example.\n", stderr);
        free_ac(ac);
        return EXIT_FAILURE;
    }

    free_ac(ac);
    return EXIT_SUCCESS;
}
#endif
