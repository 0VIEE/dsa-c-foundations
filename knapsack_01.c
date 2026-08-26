#include <stdio.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

// Bottom-Up 2D DP for 0/1 Knapsack
int knapsack(int W, int wt[], int val[], int n) {
    int dp[n + 1][W + 1];

    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (i == 0 || w == 0) {
                dp[i][w] = 0;
            } else if (wt[i - 1] <= w) {
                dp[i][w] = max(dp[i - 1][w], 
                               val[i - 1] + dp[i - 1][w - wt[i - 1]]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    return dp[n][W];
}

int main() {
    /*
        Items:
        Item 0: Val = 60,  Wt = 10
        Item 1: Val = 100, Wt = 20
        Item 2: Val = 120, Wt = 30
        Knapsack Capacity W = 50
    */
    int val[] = {60, 100, 120};
    int wt[] = {10, 20, 30};
    int W = 50;
    int n = sizeof(val) / sizeof(val[0]);

    int max_val = knapsack(W, wt, val, n);

    printf("=== 0/1 Knapsack Optimization ===\n");
    printf("Total Items: %d\n", n);
    printf("Knapsack Capacity: %d\n", W);
    printf("Maximum Attainable Value: %d\n", max_val); // Expected: 220 (Items 1 + 2)

    return 0;
}