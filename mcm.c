#include <stdio.h>
#include <limits.h>

// Matrix Chain Multiplication using Dynamic Programming
int matrix_chain_order(int p[], int n) {
    int m[n][n];

    // Cost is zero when multiplying one matrix
    for (int i = 1; i < n; i++) {
        m[i][i] = 0;
    }

    // L is chain length (from 2 matrices up to n-1)
    for (int L = 2; L < n; L++) {
        for (int i = 1; i < n - L + 1; i++) {
            int j = i + L - 1;
            m[i][j] = INT_MAX;

            for (int k = i; k <= j - 1; k++) {
                int q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (q < m[i][j]) {
                    m[i][j] = q;
                }
            }
        }
    }

    return m[1][n - 1];
}

int main() {
    /*
        Matrices dimensions:
        A1: 10 x 20
        A2: 20 x 30
        A3: 30 x 40
        A4: 40 x 30
        p[] = {10, 20, 30, 40, 30}
    */
    int arr[] = {10, 20, 30, 40, 30};
    int size = sizeof(arr) / sizeof(arr[0]);

    int min_ops = matrix_chain_order(arr, size);

    printf("=== Matrix Chain Multiplication Optimization ===\n");
    printf("Number of Matrices: %d\n", size - 1);
    printf("Minimum Scalar Multiplications: %d\n", min_ops); // Expected: 30000

    return 0;
}