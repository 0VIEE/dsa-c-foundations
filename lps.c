#include <stdio.h>
#include <string.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

// Compute length of Longest Palindromic Subsequence
int longest_palindromic_subseq(char *str) {
    int n = strlen(str);
    int dp[n][n];

    // Base case: Single characters have palindrome length 1
    for (int i = 0; i < n; i++) {
        dp[i][i] = 1;
    }

    // L is length of the substring (from 2 up to n)
    for (int L = 2; L <= n; L++) {
        for (int i = 0; i <= n - L; i++) {
            int j = i + L - 1;

            if (str[i] == str[j]) {
                if (L == 2) {
                    dp[i][j] = 2;
                } else {
                    dp[i][j] = dp[i + 1][j - 1] + 2;
                }
            } else {
                dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
            }
        }
    }

    return dp[0][n - 1];
}

int main() {
    /*
        Test 1: "BBABCBCAB" -> Longest LPS is "BABCBAB" or "BACBCAB" -> Length = 7
        Test 2: "GEEKSFORGEEKS" -> Longest LPS is "EEKSKEE" / "EEFEE" / etc. -> Length = 5
    */
    char str1[] = "BBABCBCAB";
    char str2[] = "GEEKSFORGEEKS";

    printf("=== Longest Palindromic Subsequence (LPS) ===\n");
    printf("String 1: %s\n", str1);
    printf("LPS Length: %d\n\n", longest_palindromic_subseq(str1)); // Expected: 7

    printf("String 2: %s\n", str2);
    printf("LPS Length: %d\n", longest_palindromic_subseq(str2)); // Expected: 5

    return 0;
}