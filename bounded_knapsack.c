#include <stdio.h>
#include <stdlib.h>

#define MAX_ITEMS 100
#define MAX_WEIGHT 1000

int max(int a, int b) {
    return (a > b) ? a : b;
}

// Bounded Knapsack using Binary Splitting into 0/1 Knapsack
int bounded_knapsack(int W, int wt[], int val[], int count[], int n) {
    int new_wt[MAX_ITEMS];
    int new_val[MAX_ITEMS];
    int new_n = 0;

    // Binary Splitting of each item
    for (int i = 0; i < n; i++) {
        int c = count[i];
        int k = 1;

        while (c > 0) {
            int take = (c < k) ? c : k;
            new_wt[new_n] = take * wt[i];
            new_val[new_n] = take * val[i];
            new_n++;

            c -= take;
            k *= 2;
        }
    }

    // 1D Space-Optimized 0/1 Knapsack
    int dp[MAX_WEIGHT] = {0};

    for (int i = 0; i < new_n; i++) {
        for (int w = W; w >= new_wt[i]; w--) {
            dp[w] = max(dp[w], new_val[i] + dp[w - new_wt[i]]);
        }
    }

    return dp[W];
}

int main() {
    /*
        Item 0: Wt = 2, Val = 10, Count = 5
        Item 1: Wt = 3, Val = 14, Count = 2
        Item 2: Wt = 5, Val = 26, Count = 3
        Capacity W = 15
    */
    int wt[] = {2, 3, 5};
    int val[] = {10, 14, 26};
    int count[] = {5, 2, 3};
    int n = sizeof(wt) / sizeof(wt[0]);
    int W = 15;

    int max_val = bounded_knapsack(W, wt, val, count, n);

    printf("=== Bounded Knapsack (Binary Splitting) ===\n");
    printf("Knapsack Capacity: %d\n", W);
    printf("Maximum Attainable Value: %d\n", max_val); // Expected: 76

    return 0;
}