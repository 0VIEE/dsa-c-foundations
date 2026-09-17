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

// Compute total distinct substrings using link lengths in O(sz) = O(N)
long long count_distinct_substrings(struct SuffixAutomaton *sam) {
    long long total = 0;
    for (int i = 1; i < sam->sz; i++) {
        total += (sam->states[i].len - sam->states[sam->states[i].link].len);
    }
    return total;
}

int main() {
    struct SuffixAutomaton *sam = create_sam();
    const char *text = "ababa";

    for (int i = 0; text[i] != '\0'; i++) {
        sam_extend(sam, text[i]);
    }

    /*
        For "ababa":
        Length 1: "a", "b" (2)
        Length 2: "ab", "ba" (2)
        Length 3: "aba", "bab" (2)
        Length 4: "abab", "baba" (2)
        Length 5: "ababa" (1)
        Total distinct non-empty substrings = 9
    */
    long long distinct_count = count_distinct_substrings(sam);

    printf("=== Suffix Automaton: Distinct Substring Count ===\n");
    printf("Input String: \"%s\"\n", text);
    printf("Total SAM States: %d\n", sam->sz);
    printf("Calculated Distinct Substrings: %lld (Expected: 9)\n", distinct_count);

    free(sam);
    return 0;
}