#include <stdio.h>
#include <limits.h>

// Kadane's Algorithm with Subarray Index Tracking
void max_subarray_sum(int arr[], int n) {
    int max_so_far = INT_MIN;
    int max_ending_here = 0;
    
    int start = 0, end = 0, s = 0;

    for (int i = 0; i < n; i++) {
        max_ending_here += arr[i];

        if (max_so_far < max_ending_here) {
            max_so_far = max_ending_here;
            start = s;
            end = i;
        }

        // If running sum drops below zero, reset starting point
        if (max_ending_here < 0) {
            max_ending_here = 0;
            s = i + 1;
        }
    }

    printf("=== Maximum Subarray Sum (Kadane's Algorithm) ===\n");
    printf("Maximum Contiguous Sum: %d\n", max_so_far);
    printf("Subarray Range: Index %d to %d\n", start, end);
    printf("Subarray Elements: [ ");
    for (int i = start; i <= end; i++) {
        printf("%d ", arr[i]);
    }
    printf("]\n");
}

int main() {
    /*
        Sample Array: {-2, -3, 4, -1, -2, 1, 5, -3}
        Optimal Subarray: {4, -1, -2, 1, 5} -> Sum = 7
    */
    int arr[] = {-2, -3, 4, -1, -2, 1, 5, -3};
    int n = sizeof(arr) / sizeof(arr[0]);

    max_subarray_sum(arr, n);

    return 0;
}