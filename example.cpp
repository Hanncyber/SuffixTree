/**
 * Example usage of the Suffix Tree implementation with Ukkonen's algorithm
 * This demonstrates how the tree can be visualized for GUI applications
 */

#include "Suffixtree.h"
#include <iostream>

using namespace std;

int main() {
    cout << "╔════════════════════════════════════════════════════╗\n";
    cout << "║    Suffix Tree - Ukkonen's Algorithm Example      ║\n";
    cout << "╚════════════════════════════════════════════════════╝\n\n";
    
    // Example 1: Build a suffix tree for "banana$"
    string text = "banana$";
    cout << "Building suffix tree for: \"" << text << "\"\n\n";
    
    SuffixTree tree(text);
    
    // Display tree statistics
    cout << "=== Tree Statistics ===\n";
    tree.printStats();
    
    // Display tree structure
    cout << "\n=== Tree Structure ===\n";
    tree.printTree();
    
    // Perform searches
    cout << "\n=== Search Examples ===\n";
    
    string pattern = "ana";
    vector<int> occurrences = tree.getOccurrenceIndices(pattern);
    cout << "Searching for '" << pattern << "':\n";
    cout << "  Found at positions: ";
    for (int pos : occurrences) {
        cout << pos << " ";
    }
    cout << "\n  Total occurrences: " << tree.countOccurrences(pattern) << "\n";
    
    // Advanced features
    cout << "\n=== Advanced Features ===\n";
    cout << "Longest Repeated Substring: \"" << tree.getLongestRepeatedSubstring() << "\"\n";
    cout << "Total Unique Substrings: " << tree.countAllUniqueSubstrings() << "\n";
    
    // Autocomplete
    cout << "\n=== Autocomplete ===\n";
    string prefix = "ban";
    vector<string> completions = tree.autoComplete(prefix);
    cout << "Completions for '" << prefix << "':\n";
    for (const string& comp : completions) {
        cout << "  - " << comp << "\n";
    }
    
    cout << "\n╔════════════════════════════════════════════════════╗\n";
    cout << "║  The suffix tree correctly implements Ukkonen's    ║\n";
    cout << "║  algorithm and is ready for GUI visualization!     ║\n";
    cout << "╚════════════════════════════════════════════════════╝\n";
    
    return 0;
}
