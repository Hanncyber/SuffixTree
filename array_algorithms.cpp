#include "array_algorithms.h"
#include <iostream>
#include <climits>

using namespace std;

/**
 * Brute-force maximum subarray sum (Kadane-free O(n^2) approach).
 *
 * For every possible starting index i, accumulate the running sum for every
 * ending index j >= i and track the global maximum.
 *
 * Complexity analysis:
 *   - Two nested loops: outer runs n times, inner runs up to n times.
 *   - Total iterations: n*(n+1)/2  =>  O(n^2) time.
 *   - No extra memory proportional to n  =>  O(1) space.
 */
int maxSubarraySum(const int arr[], int n, int& start, int& end) {
    int maxSum = INT_MIN;
    start = 0;
    end   = 0;

    for (int i = 0; i < n; i++) {
        int currentSum = 0;
        for (int j = i; j < n; j++) {
            currentSum += arr[j];
            if (currentSum > maxSum) {
                maxSum = currentSum;
                start  = i;
                end    = j;
            }
        }
    }
    return maxSum;
}

/**
 * Find all pairs (arr[i], arr[j]) where i < j and arr[i] + arr[j] == k.
 *
 * Brute-force: check every distinct pair once.
 *
 * Complexity analysis:
 *   - Two nested loops: outer runs n-1 times, inner up to n-1 times.
 *   - Total iterations: n*(n-1)/2  =>  O(n^2) time.
 *   - No extra memory proportional to n  =>  O(1) space.
 */
void findPairsWithSum(const int arr[], int n, int k) {
    bool found = false;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] + arr[j] == k) {
                cout << "(" << arr[i] << ", " << arr[j] << ")" << endl;
                found = true;
            }
        }
    }
    if (!found) {
        cout << "No pairs found with sum " << k << endl;
    }
}
