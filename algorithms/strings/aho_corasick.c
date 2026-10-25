#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 128
#define ALPHABET_SIZE 26

struct ACNode {
    int next[ALPHABET_SIZE];
    int link;      // Failure link
    int dict_link; // Dictionary suffix link
    int is_terminal;
    char pattern[32];
};

struct ACAry {
    struct ACNode nodes[MAX_NODES];
    int sz;
};

struct ACAry* create_ac() {
    struct ACAry *ac = (struct ACAry*)malloc(sizeof(struct ACAry));
    ac->sz = 1;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        ac->nodes[0].next[i] = 0;
    }
    ac->nodes[0].link = 0;
    ac->nodes[0].dict_link = 0;
    ac->nodes[0].is_terminal = 0;
    return ac;
}

void insert_pattern(struct ACAry *ac, const char *pat) {
    int u = 0;
    for (int i = 0; pat[i] != '\0'; i++) {
        int c = pat[i] - 'a';
        if (ac->nodes[u].next[c] == 0) {
            int new_node = ac->sz++;
            for (int j = 0; j < ALPHABET_SIZE; j++) {
                ac->nodes[new_node].next[j] = 0;
            }
            ac->nodes[new_node].link = 0;
            ac->nodes[new_node].dict_link = 0;
            ac->nodes[new_node].is_terminal = 0;
            ac->nodes[u].next[c] = new_node;
        }
        u = ac->nodes[u].next[c];
    }
    ac->nodes[u].is_terminal = 1;
    strcpy(ac->nodes[u].pattern, pat);
}

void build_automaton(struct ACAry *ac) {
    int queue[MAX_NODES];
    int head = 0, tail = 0;

    // Initialize root's children
    for (int c = 0; c < ALPHABET_SIZE; c++) {
        int v = ac->nodes[0].next[c];
        if (v != 0) {
            ac->nodes[v].link = 0;
            queue[tail++] = v;
        }
    }

    while (head < tail) {
        int u = queue[head++];
        
        // Set dictionary link
        int fail = ac->nodes[u].link;
        if (ac->nodes[fail].is_terminal) {
            ac->nodes[u].dict_link = fail;
        } else {
            ac->nodes[u].dict_link = ac->nodes[fail].dict_link;
        }

        for (int c = 0; c < ALPHABET_SIZE; c++) {
            int v = ac->nodes[u].next[c];
            if (v != 0) {
                ac->nodes[v].link = ac->nodes[fail].next[c];
                queue[tail++] = v;
            } else {
                ac->nodes[u].next[c] = ac->nodes[fail].next[c];
            }
        }
    }
}

void search_text(struct ACAry *ac, const char *text) {
    int u = 0;
    printf("Searching text: \"%s\"\n", text);
    for (int i = 0; text[i] != '\0'; i++) {
        int c = text[i] - 'a';
        u = ac->nodes[u].next[c];

        // Check current node and dictionary links for matches
        int temp = u;
        while (temp != 0) {
            if (ac->nodes[temp].is_terminal) {
                printf("  -> Found pattern \"%s\" ending at index %d\n", 
                       ac->nodes[temp].pattern, i);
            }
            temp = ac->nodes[temp].dict_link;
        }
    }
}

int main() {
    /*
        Patterns to search: "he", "she", "his", "hers"
        Text: "ushers"
    */
    struct ACAry *ac = create_ac();
    insert_pattern(ac, "he");
    insert_pattern(ac, "she");
    insert_pattern(ac, "his");
    insert_pattern(ac, "hers");

    build_automaton(ac);

    char *text = "ushers";
    search_text(ac, text);

    free(ac);
    return 0;
}