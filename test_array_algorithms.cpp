#include "array_algorithms.h"
#include <iostream>

using namespace std;

void printSeparator() {
    cout << "\n========================================\n";
}

void printHeader(const char* title) {
    printSeparator();
    cout << "\n" << title << "\n";
    printSeparator();
}

// Helper to print an array
void printArray(const int arr[], int n) {
    cout << "{ ";
    for (int i = 0; i < n; i++) {
        cout << arr[i];
        if (i < n - 1) cout << ", ";
    }
    cout << " }";
}

// -----------------------------------------------------------------
// Tests for maxSubarraySum
// -----------------------------------------------------------------

void testMaxSubarraySum() {
    printHeader("ALGORITHM 1: Maximum Subarray Sum (Brute Force)");

    // Test 1: mixed positive/negative values
    {
        int arr[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
        int n = 9;
        int start, end;
        int result = maxSubarraySum(arr, n, start, end);
        cout << "\nTest 1 - Array: ";
        printArray(arr, n);
        cout << "\n  Maximum subarray sum : " << result << endl;
        cout << "  Subarray indices     : [" << start << ", " << end << "]" << endl;
        cout << "  Subarray             : ";
        printArray(arr + start, end - start + 1);
        cout << "\n  Expected             : 6  (subarray: {4,-1,2,1})" << endl;
    }

    // Test 2: all negative (should pick the least negative element)
    {
        int arr[] = {-3, -1, -4, -2};
        int n = 4;
        int start, end;
        int result = maxSubarraySum(arr, n, start, end);
        cout << "\nTest 2 - Array: ";
        printArray(arr, n);
        cout << "\n  Maximum subarray sum : " << result << endl;
        cout << "  Subarray indices     : [" << start << ", " << end << "]" << endl;
        cout << "  Expected             : -1  (single element)" << endl;
    }

    // Test 3: all positive
    {
        int arr[] = {1, 2, 3, 4, 5};
        int n = 5;
        int start, end;
        int result = maxSubarraySum(arr, n, start, end);
        cout << "\nTest 3 - Array: ";
        printArray(arr, n);
        cout << "\n  Maximum subarray sum : " << result << endl;
        cout << "  Expected             : 15 (entire array)" << endl;
    }

    // Test 4: single element
    {
        int arr[] = {7};
        int n = 1;
        int start, end;
        int result = maxSubarraySum(arr, n, start, end);
        cout << "\nTest 4 - Array: ";
        printArray(arr, n);
        cout << "\n  Maximum subarray sum : " << result << endl;
        cout << "  Expected             : 7" << endl;
    }
}

// -----------------------------------------------------------------
// Tests for findPairsWithSum
// -----------------------------------------------------------------

void testFindPairsWithSum() {
    printHeader("ALGORITHM 2: Find All Pairs with Given Sum (Brute Force)");

    // Test 1: example from problem statement
    {
        int arr[] = {1, 5, 7, -1, 5};
        int n = 5;
        int k = 6;
        cout << "\nTest 1 - Array: ";
        printArray(arr, n);
        cout << ", k = " << k << endl;
        cout << "  Pairs with sum " << k << ":\n  ";
        findPairsWithSum(arr, n, k);
        cout << "  Expected: (1, 5), (7, -1), (1, 5)" << endl;
    }

    // Test 2: no pairs
    {
        int arr[] = {1, 2, 3, 4};
        int n = 4;
        int k = 10;
        cout << "\nTest 2 - Array: ";
        printArray(arr, n);
        cout << ", k = " << k << endl;
        cout << "  Pairs with sum " << k << ":\n  ";
        findPairsWithSum(arr, n, k);
    }

    // Test 3: negative numbers involved
    {
        int arr[] = {-5, 1, -4, 3, 2, -1};
        int n = 6;
        int k = -2;
        cout << "\nTest 3 - Array: ";
        printArray(arr, n);
        cout << ", k = " << k << endl;
        cout << "  Pairs with sum " << k << ":\n  ";
        findPairsWithSum(arr, n, k);
        cout << "  Expected: (-5, 3), (1, -3 is absent), (-4, 2), (3, -5 already counted as ordered)..." << endl;
        cout << "  Note: pairs printed in left-to-right index order (i < j)" << endl;
    }
}

int main() {
    cout << "╔════════════════════════════════════════════════════╗\n";
    cout << "║       ARRAY ALGORITHMS DEMO & TEST                 ║\n";
    cout << "║  1. Brute-Force Maximum Subarray Sum  O(n^2)       ║\n";
    cout << "║  2. Find All Pairs with Sum = k       O(n^2)       ║\n";
    cout << "╚════════════════════════════════════════════════════╝\n";

    testMaxSubarraySum();
    testFindPairsWithSum();

    printSeparator();
    cout << "\nAll tests complete.\n";
    printSeparator();

    return 0;
}
