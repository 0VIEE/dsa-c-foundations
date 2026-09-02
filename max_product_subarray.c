#include <stdio.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

// Maximum Product Subarray in O(N) time and O(1) space
int max_product_subarray(int arr[], int n) {
    if (n == 0) return 0;

    int max_so_far = arr[0];
    int max_ending_here = arr[0];
    int min_ending_here = arr[0];

    for (int i = 1; i < n; i++) {
        // If current element is negative, swap max and min
        if (arr[i] < 0) {
            int temp = max_ending_here;
            max_ending_here = min_ending_here;
            min_ending_here = temp;
        }

        // Update max and min ending at index i
        max_ending_here = max(arr[i], max_ending_here * arr[i]);
        min_ending_here = min(arr[i], min_ending_here * arr[i]);

        // Update global max product
        max_so_far = max(max_so_far, max_ending_here);
    }

    return max_so_far;
}

int main() {
    /*
        Sample Array: {2, 3, -2, 4, -1}
        Subarrays:
        {2, 3} -> 6
        {2, 3, -2, 4, -1} -> 2 * 3 * (-2) * 4 * (-1) = 48
    */
    int arr[] = {2, 3, -2, 4, -1};
    int n = sizeof(arr) / sizeof(arr[0]);

    int result = max_product_subarray(arr, n);

    printf("=== Maximum Product Subarray ===\n");
    printf("Array: {2, 3, -2, 4, -1}\n");
    printf("Maximum Contiguous Product: %d\n", result); // Expected: 48

    return 0;
}