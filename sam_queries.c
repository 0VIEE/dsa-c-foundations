#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STATES 100
#define ALPHABET_SIZE 26

struct SAMState {
    int len;
    int link; // Suffix link
    int next[ALPHABET_SIZE];
};

struct SuffixAutomaton {
    struct SAMState states[MAX_STATES];
    int sz;
    int last;
};

struct SuffixAutomaton* create_sam() {
    struct SuffixAutomaton *sam = (struct SuffixAutomaton*)malloc(sizeof(struct SuffixAutomaton));
    sam->sz = 1;
    sam->last = 0;
    sam->states[0].len = 0;
    sam->states[0].link = -1;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sam->states[0].next[i] = -1;
    }
    return sam;
}

void sam_extend(struct SuffixAutomaton *sam, char c_char) {
    int c = c_char - 'a';
    int cur = sam->sz++;
    sam->states[cur].len = sam->states[sam->last].len + 1;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sam->states[cur].next[i] = -1;
    }

    int p = sam->last;
    while (p != -1 && sam->states[p].next[c] == -1) {
        sam->states[p].next[c] = cur;
        p = sam->states[p].link;
    }

    if (p == -1) {
        sam->states[cur].link = 0;
    } else {
        int q = sam->states[p].next[c];
        if (sam->states[p].len + 1 == sam->states[q].len) {
            sam->states[cur].link = q;
        } else {
            int clone = sam->sz++;
            sam->states[clone].len = sam->states[p].len + 1;
            for (int i = 0; i < ALPHABET_SIZE; i++) {
                sam->states[clone].next[i] = sam->states[q].next[i];
            }
            sam->states[clone].link = sam->states[q].link;
            while (p != -1 && sam->states[p].next[c] == q) {
                sam->states[p].next[c] = clone;
                p = sam->states[p].link;
            }
            sam->states[q].link = sam->states[cur].link = clone;
        }
    }
    sam->last = cur;
}

// O(|M|) Pattern Matching Query
bool contains_substring(struct SuffixAutomaton *sam, const char *pattern) {
    int curr = 0;
    for (int i = 0; pattern[i] != '\0'; i++) {
        int c = pattern[i] - 'a';
        if (sam->states[curr].next[c] == -1) {
            return false; // Mismatch
        }
        curr = sam->states[curr].next[c];
    }
    return true;
}

void free_sam(struct SuffixAutomaton *sam) {
    if (sam != NULL) {
        free(sam);
    }
}

int main() {
    printf("=== Suffix Automaton Pattern Matching & Queries ===\n");
    char *text = "abbcab";
    struct SuffixAutomaton *sam = create_sam();

    for (int i = 0; text[i] != '\0'; i++) {
        sam_extend(sam, text[i]);
    }

    printf("Text: \"%s\" (Automaton States Built: %d)\n", text, sam->sz);

    const char *p1 = "bca";
    const char *p2 = "acb";
    const char *p3 = "abb";

    printf("Query \"%s\": %s (Expected: Found)\n", p1, contains_substring(sam, p1) ? "Found" : "Not Found");
    printf("Query \"%s\": %s (Expected: Not Found)\n", p2, contains_substring(sam, p2) ? "Found" : "Not Found");
    printf("Query \"%s\": %s (Expected: Found)\n", p3, contains_substring(sam, p3) ? "Found" : "Not Found");

    free_sam(sam);
    printf("[PASS] Suffix Automaton queries executed successfully.\n");
    return 0;
}