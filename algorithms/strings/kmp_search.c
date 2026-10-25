#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Precompute Longest Proper Prefix which is also Suffix (lps)
void compute_lps_array(const char *pat, int m, int lps[]) {
    int len = 0; // Length of the previous longest prefix suffix
    lps[0] = 0;  // Base case: length 1 prefix has no proper prefix
    int i = 1;

    while (i < m) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                // Fallback to previous prefix match without advancing i
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

// Search all occurrences of pattern in text using KMP
void kmp_search(const char *txt, const char *pat) {
    int n = strlen(txt);
    int m = strlen(pat);

    int *lps = (int*)malloc(m * sizeof(int));
    compute_lps_array(pat, m, lps);

    printf("Search Pattern: \"%s\"\n", pat);
    printf("Precomputed LPS Array: [ ");
    for (int k = 0; k < m; k++) {
        printf("%d ", lps[k]);
    }
    printf("]\n");

    int i = 0; // Index for txt
    int j = 0; // Index for pat
    int match_count = 0;

    printf("Occurrences found at text indices: [ ");
    while (i < n) {
        if (pat[j] == txt[i]) {
            i++;
            j++;
        }

        if (j == m) {
            printf("%d ", i - j);
            match_count++;
            j = lps[j - 1]; // Reset pattern pointer via failure link
        } else if (i < n && pat[j] != txt[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    printf("] (Total: %d)\n\n", match_count);

    free(lps);
}

int main() {
    const char *text = "ababcabcabababd";
    const char *pattern1 = "ababd";
    const char *pattern2 = "ab";

    printf("=== Knuth-Morris-Pratt (KMP) String Search ===\n");
    printf("Primary Text: \"%s\"\n\n", text);

    kmp_search(text, pattern1);
    kmp_search(text, pattern2);

    return 0;
}