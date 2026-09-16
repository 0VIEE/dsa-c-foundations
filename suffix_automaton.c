#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26
#define MAX_LEN 100
#define MAX_STATES (2 * MAX_LEN)

struct State {
    int len;
    int link;
    int next[ALPHABET_SIZE];
};

struct SuffixAutomaton {
    struct State states[MAX_STATES];
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

// Extend SAM by one character c
void sam_extend(struct SuffixAutomaton *sam, char c) {
    int cur = sam->sz++;
    int ch = c - 'a';

    sam->states[cur].len = sam->states[sam->last].len + 1;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sam->states[cur].next[i] = -1;
    }

    int p = sam->last;
    while (p != -1 && sam->states[p].next[ch] == -1) {
        sam->states[p].next[ch] = cur;
        p = sam->states[p].link;
    }

    if (p == -1) {
        sam->states[cur].link = 0;
    } else {
        int q = sam->states[p].next[ch];
        if (sam->states[p].len + 1 == sam->states[q].len) {
            sam->states[cur].link = q;
        } else {
            int clone = sam->sz++;
            sam->states[clone].len = sam->states[p].len + 1;
            for (int i = 0; i < ALPHABET_SIZE; i++) {
                sam->states[clone].next[i] = sam->states[q].next[i];
            }
            sam->states[clone].link = sam->states[q].link;

            while (p != -1 && sam->states[p].next[ch] == q) {
                sam->states[p].next[ch] = clone;
                p = sam->states[p].link;
            }

            sam->states[q].link = clone;
            sam->states[cur].link = clone;
        }
    }

    sam->last = cur;
}

// Query if pattern exists as a substring in O(|pattern|)
bool sam_contains_substring(struct SuffixAutomaton *sam, const char *pattern) {
    int cur = 0;
    for (int i = 0; pattern[i] != '\0'; i++) {
        int ch = pattern[i] - 'a';
        if (sam->states[cur].next[ch] == -1) {
            return false;
        }
        cur = sam->states[cur].next[ch];
    }
    return true;
}

int main() {
    struct SuffixAutomaton *sam = create_sam();
    const char *text = "abbab";

    for (int i = 0; text[i] != '\0'; i++) {
        sam_extend(sam, text[i]);
    }

    printf("=== Suffix Automaton Substring Queries ===\n");
    printf("Base string: \"%s\"\n", text);
    printf("Total SAM states created: %d\n", sam->sz);

    const char *test1 = "bab";
    const char *test2 = "bba";
    const char *test3 = "ab";
    const char *test4 = "ba";
    const char *test5 = "bbb";

    printf("Contains \"%s\": %s\n", test1, sam_contains_substring(sam, test1) ? "true" : "false");
    printf("Contains \"%s\": %s\n", test2, sam_contains_substring(sam, test2) ? "true" : "false");
    printf("Contains \"%s\": %s\n", test3, sam_contains_substring(sam, test3) ? "true" : "false");
    printf("Contains \"%s\": %s\n", test4, sam_contains_substring(sam, test4) ? "true" : "false");
    printf("Contains \"%s\": %s\n", test5, sam_contains_substring(sam, test5) ? "true" : "false");

    free(sam);
    return 0;
}