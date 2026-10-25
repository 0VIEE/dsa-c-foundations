#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int compute_lps_array(const char *pat, size_t length, size_t lps[]) {
    if (pat == NULL || lps == NULL || length == 0) {
        return 0;
    }

    size_t matched = 0;
    lps[0] = 0;

    for (size_t i = 1; i < length;) {
        if (pat[i] == pat[matched]) {
            lps[i++] = ++matched;
        } else if (matched != 0) {
            matched = lps[matched - 1];
        } else {
            lps[i++] = 0;
        }
    }

    return 1;
}

/* Returns 1 after a successful search, or 0 for invalid input/allocation failure. */
int kmp_search(const char *txt, const char *pat) {
    if (txt == NULL || pat == NULL || pat[0] == '\0') {
        return 0;
    }

    size_t n = strlen(txt);
    size_t m = strlen(pat);
    if (m > SIZE_MAX / sizeof(size_t)) {
        return 0;
    }

    size_t *lps = (size_t *)malloc(m * sizeof(*lps));
    if (lps == NULL) {
        return 0;
    }

    if (!compute_lps_array(pat, m, lps)) {
        free(lps);
        return 0;
    }

    printf("Search Pattern: \"%s\"\n", pat);
    printf("Precomputed LPS Array: [ ");
    for (size_t k = 0; k < m; k++) {
        printf("%zu ", lps[k]);
    }
    printf("]\n");

    size_t i = 0;
    size_t j = 0;
    size_t match_count = 0;

    printf("Occurrences found at text indices: [ ");
    while (i < n) {
        if (pat[j] == txt[i]) {
            i++;
            j++;
        }

        if (j == m) {
            printf("%zu ", i - j);
            match_count++;
            j = lps[j - 1];
        } else if (i < n && pat[j] != txt[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    printf("] (Total: %zu)\n\n", match_count);

    free(lps);
    return 1;
}

#ifndef KMP_SEARCH_NO_MAIN
int main(void) {
    const char *text = "ababcabcabababd";
    const char *pattern1 = "ababd";
    const char *pattern2 = "ab";

    printf("=== Knuth-Morris-Pratt (KMP) String Search ===\n");
    printf("Primary Text: \"%s\"\n\n", text);

    if (!kmp_search(text, pattern1) || !kmp_search(text, pattern2)) {
        fputs("KMP search failed.\n", stderr);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
#endif
