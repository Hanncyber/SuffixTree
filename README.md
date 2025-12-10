# Suffix Tree - Ukkonen's Algorithm Implementation

This repository contains a correct implementation of Ukkonen's algorithm for building suffix trees in O(n) time complexity.

## ✅ Verification Status

The implementation has been thoroughly tested and verified to be **CORRECT**:

- ✓ Builds suffix trees correctly using Ukkonen's algorithm
- ✓ Handles all edge cases (repeated characters, unique characters)
- ✓ Maintains correct suffix links and internal structure
- ✓ Produces accurate search results
- ✓ Counts unique substrings correctly
- ✓ Provides tree visualization for GUI display

## Features

### Core Functionality
- **Suffix Tree Construction**: O(n) time complexity using Ukkonen's algorithm
- **Pattern Searching**: Fast substring search with occurrence indices
- **Contains Check**: Verify if a pattern exists in the text
- **Count Occurrences**: Count how many times a pattern appears

### Advanced Features
- **Longest Repeated Substring**: Find the longest substring that appears at least twice
- **Shortest Unique Substring**: Find the shortest substring that appears only once
- **Lexicographically First Suffix**: Get the suffix that appears first in dictionary order
- **Most Frequent Substring**: Find the most frequently occurring substring of minimum length
- **Unique Substring Count**: Count all unique substrings in the text
- **Autocomplete**: Get all possible completions for a given prefix
- **Context Search**: Get surrounding text for a pattern match
- **Longest Palindrome**: Find the longest palindromic substring

### Visualization (GUI-Ready)
- **printTree()**: Displays the tree structure with edges and suffix indices
- **printStats()**: Shows statistics (nodes, depth, unique substrings, etc.)

## Building

Use the provided Makefile:

```bash
make all          # Build both example and test programs
make example      # Build the example program
make test         # Build the test suite
make run-example  # Run the example program
make run-test     # Run the test suite
make clean        # Clean build artifacts
```

Or compile manually:

```bash
g++ -std=c++17 -o example example.cpp SuffixTree.cpp
g++ -std=c++17 -o test test_suffix_tree.cpp SuffixTree.cpp
```

## Usage Example

```cpp
#include "Suffixtree.h"
#include <iostream>

int main() {
    // Build suffix tree
    std::string text = "banana$";
    SuffixTree tree(text);
    
    // Display tree structure (GUI-ready)
    tree.printTree();
    tree.printStats();
    
    // Search for patterns
    std::vector<int> positions = tree.getOccurrenceIndices("ana");
    // positions = {1, 3}
    
    // Check if pattern exists
    bool exists = tree.contains("ana");  // true
    
    // Count occurrences
    int count = tree.countOccurrences("ana");  // 2
    
    // Get longest repeated substring
    std::string lrs = tree.getLongestRepeatedSubstring();  // "ana"
    
    // Autocomplete
    std::vector<std::string> completions = tree.autoComplete("ban");
    // completions = {"banana$"}
    
    return 0;
}
```

## Test Results

### Test 1: "banana$"
- Suffixes correctly identified: 7 suffixes (0-6)
- Pattern "ana" found at positions: 1, 3 ✓
- Pattern "na" found at positions: 2, 4 ✓
- Pattern "a" found at positions: 1, 3, 5 ✓
- Unique substrings: 22 ✓
- Longest repeated substring: "ana" ✓

### Test 2: "abcabxabcd$"
- Suffixes correctly identified: 11 suffixes (0-10)
- Pattern "abc" found at positions: 0, 6 ✓
- Pattern "ab" found at positions: 0, 3, 6 ✓
- Pattern "abcd" found at position: 6 ✓
- Unique substrings: 57 ✓
- Longest repeated substring: "abc" ✓

## Tree Structure

The suffix tree for "banana$" is visualized as:

```
Suffix Tree for: "banana$"
===================================
├─ [6,6] "$" (Suffix: 6)
├─ [1,1] "a"
│  ├─ [6,6] "$" (Suffix: 5)
│  ├─ [2,3] "na"
│  │  ├─ [6,6] "$" (Suffix: 3)
│  │  ├─ [4,6] "na$" (Suffix: 1)
├─ [0,6] "banana$" (Suffix: 0)
├─ [2,3] "na"
│  ├─ [6,6] "$" (Suffix: 4)
│  ├─ [4,6] "na$" (Suffix: 2)
===================================
```

This shows:
- Edge labels with start and end positions
- Suffix indices for leaf nodes
- Hierarchical tree structure
- All 7 suffixes properly represented

## Implementation Details

### Ukkonen's Algorithm
The implementation follows Ukkonen's online construction algorithm with:
- **Active Point**: Tracks current position in the tree (activeNode, activeEdge, activeLength)
- **Remainder**: Counts suffixes yet to be added
- **Suffix Links**: Quick navigation between related nodes
- **Edge Compression**: Efficient storage using start/end pointers

### Data Structure
- **SuffixNode**: Contains children map, edge range (start, end), suffix index, and suffix link
- **Root Node**: Special node with end pointer shared by all leaf edges
- **Leaf Nodes**: Automatically extend as the tree grows

## Time Complexity
- **Construction**: O(n) where n is the text length
- **Search**: O(m + k) where m is pattern length and k is number of occurrences
- **Space**: O(n) for storing the tree

## Files
- `Suffixtree.h` - Header file with class declaration
- `SuffixTree.cpp` - Implementation of all methods
- `example.cpp` - Example usage demonstrating GUI-ready output
- `test_suffix_tree.cpp` - Comprehensive test suite
- `Makefile` - Build configuration

## GUI Integration

The `printTree()` and `printStats()` methods provide text-based visualization that can be:
1. Displayed directly in a console application
2. Parsed and rendered in a graphical interface
3. Used to generate graph visualizations (nodes and edges)
4. Exported to formats like DOT for graphviz rendering

The tree structure is correct and ready for GUI visualization in any framework (SFML, Qt, web-based, etc.).

## License

This implementation is provided for educational and research purposes.
