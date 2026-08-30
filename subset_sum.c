#include <stdio.h>
#include <stdbool.h>

// Function to check if a subset with given sum exists
bool is_subset_sum(int arr[], int n, int sum) {
    bool dp[n + 1][sum + 1];

    // Base case: If sum is 0, answer is true
    for (int i = 0; i <= n; i++) {
        dp[i][0] = true;
    }

    // Base case: If sum is not 0 and set is empty, answer is false
    for (int j = 1; j <= sum; j++) {
        dp[0][j] = false;
    }

    // Fill the DP table
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= sum; j++) {
            if (arr[i - 1] <= j) {
                dp[i][j] = dp[i - 1][j] || dp[i - 1][j - arr[i - 1]];
            } else {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }

    return dp[n][sum];
}

int main() {
    /*
        Set: {3, 34, 4, 12, 5, 2}
        Target Sum: 9 (Subset: {4, 5} or {3, 4, 2}) -> True
        Target Sum: 30 -> False
    */
    int arr[] = {3, 34, 4, 12, 5, 2};
    int n = sizeof(arr) / sizeof(arr[0]);
    int sum1 = 9;
    int sum2 = 30;

    printf("=== Subset Sum Decision DP ===\n");
    printf("Set: {3, 34, 4, 12, 5, 2}\n");

    if (is_subset_sum(arr, n, sum1)) {
        printf("Target Sum %d: Found valid subset! [YES]\n", sum1);
    } else {
        printf("Target Sum %d: No subset found. [NO]\n", sum1);
    }

    if (is_subset_sum(arr, n, sum2)) {
        printf("Target Sum %d: Found valid subset! [YES]\n", sum2);
    } else {
        printf("Target Sum %d: No subset found. [NO]\n", sum2);
    }

    return 0;
}