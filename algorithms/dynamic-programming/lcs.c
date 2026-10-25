#include <stdio.h>
#include <string.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

// Compute LCS Length and print the subsequence string
void lcs(char *s1, char *s2) {
    int m = strlen(s1);
    int n = strlen(s2);
    int dp[m + 1][n + 1];

    // Build DP table
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0) {
                dp[i][j] = 0;
            } else if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = 1 + dp[i - 1][j - 1];
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    int lcs_len = dp[m][n];
    printf("=== Longest Common Subsequence (LCS) ===\n");
    printf("String 1: %s\n", s1);
    printf("String 2: %s\n", s2);
    printf("LCS Length: %d\n", lcs_len);

    // Backtrack to reconstruct the LCS string
    char lcs_str[lcs_len + 1];
    lcs_str[lcs_len] = '\0';

    int i = m, j = n, index = lcs_len - 1;
    while (i > 0 && j > 0) {
        if (s1[i - 1] == s2[j - 1]) {
            lcs_str[index--] = s1[i - 1];
            i--;
            j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }

    printf("LCS String: %s\n", lcs_str);
}

int main() {
    char s1[] = "AGGTAB";
    char s2[] = "GXTXAYB";

    lcs(s1, s2);

    return 0;
}