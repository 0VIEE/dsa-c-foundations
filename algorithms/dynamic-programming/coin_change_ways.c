#include <stdio.h>
#include <stdlib.h>

// Compute total distinct ways to make change for amount V
int total_ways_coin_change(int coins[], int n, int V) {
    int *dp = (int*)malloc((V + 1) * sizeof(int));
    if (dp == NULL) return -1;

    // Base case: 1 way to make 0 amount
    dp[0] = 1;

    // Initialize all other target values to 0
    for (int i = 1; i <= V; i++) {
        dp[i] = 0;
    }

    // Outer loop over coins ensures combinations, not permutations
    for (int i = 0; i < n; i++) {
        int coin = coins[i];
        for (int w = coin; w <= V; w++) {
            dp[w] += dp[w - coin];
        }
    }

    int result = dp[V];
    free(dp);
    return result;
}

int main() {
    /*
        Coins available: {1, 2, 5}
        Target amount: 5
        Combinations:
        1. 1+1+1+1+1
        2. 1+1+1+2
        3. 1+2+2
        4. 5
        Total = 4 distinct ways
    */
    int coins[] = {1, 2, 5};
    int n = sizeof(coins) / sizeof(coins[0]);
    int amount = 5;

    int ways = total_ways_coin_change(coins, n, amount);

    printf("=== Coin Change II: Total Combinations ===\n");
    printf("Coin Denominations: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", coins[i]);
    }
    printf("\nTarget Amount: %d\n", amount);
    printf("Total Distinct Combinations: %d\n", ways); // Expected: 4

    return 0;
}