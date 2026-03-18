#ifndef ARRAY_ALGORITHMS_H
#define ARRAY_ALGORITHMS_H

/**
 * Brute-force algorithm to find the maximum subarray sum.
 *
 * Complexity: O(n^2) time, O(1) extra space.
 *
 * @param arr   Input array (may contain negative and positive integers).
 * @param n     Number of elements in arr.
 * @param start Output parameter: starting index of the maximum subarray.
 * @param end   Output parameter: ending index (inclusive) of the maximum subarray.
 * @return      Maximum subarray sum.
 */
int maxSubarraySum(const int arr[], int n, int& start, int& end);

/**
 * Find all pairs in arr[] whose sum equals k (brute-force).
 *
 * Complexity: O(n^2) time, O(1) extra space (pairs are printed directly).
 *
 * @param arr   Input array (may contain negative and positive integers).
 * @param n     Number of elements in arr.
 * @param k     Target sum.
 */
void findPairsWithSum(const int arr[], int n, int k);

#endif // ARRAY_ALGORITHMS_H
