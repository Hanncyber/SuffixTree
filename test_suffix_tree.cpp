#include "Suffixtree.h"
#include <iostream>
#include <cassert>
#include <algorithm>

using namespace std;

void testBasicConstruction() {
    cout << "Testing basic suffix tree construction...\n";
    
    // Test 1: "banana$"
    string test1 = "banana$";
    SuffixTree tree1(test1);
    
    vector<int> ana = tree1.getOccurrenceIndices("ana");
    sort(ana.begin(), ana.end());
    assert(ana.size() == 2);
    assert(ana[0] == 1 && ana[1] == 3);
    
    vector<int> na = tree1.getOccurrenceIndices("na");
    assert(na.size() == 2);
    
    vector<int> a = tree1.getOccurrenceIndices("a");
    assert(a.size() == 3);
    
    cout << "✓ Test 1 passed: 'banana$'\n";
    
    // Test 2: "abcabxabcd$"
    string test2 = "abcabxabcd$";
    SuffixTree tree2(test2);
    
    vector<int> abc = tree2.getOccurrenceIndices("abc");
    sort(abc.begin(), abc.end());
    assert(abc.size() == 2);
    assert(abc[0] == 0 && abc[1] == 6);
    
    vector<int> ab = tree2.getOccurrenceIndices("ab");
    assert(ab.size() == 3);
    
    cout << "✓ Test 2 passed: 'abcabxabcd$'\n";
}

void testSearchOperations() {
    cout << "\nTesting search operations...\n";
    
    string test = "banana$";
    SuffixTree tree(test);
    
    assert(tree.contains("ana") == true);
    assert(tree.contains("xyz") == false);
    assert(tree.contains("$") == true);
    
    assert(tree.countOccurrences("ana") == 2);
    assert(tree.countOccurrences("a") == 3);
    assert(tree.countOccurrences("xyz") == 0);
    
    cout << "✓ Search operations passed\n";
}

void testAdvancedFeatures() {
    cout << "\nTesting advanced features...\n";
    
    string test = "banana$";
    SuffixTree tree(test);
    
    // Longest repeated substring
    string lrs = tree.getLongestRepeatedSubstring();
    assert(lrs == "ana");
    
    // Unique substrings
    long long count = tree.countAllUniqueSubstrings();
    assert(count == 22); // Verified manually
    
    // Lexicographically first suffix
    string first = tree.getLexicographicallyFirstSuffix();
    assert(first == "$");
    
    // Shortest unique substring
    string shortest = tree.getShortestUniqueSubstring();
    assert(shortest == "$");
    
    cout << "✓ Advanced features passed\n";
}

void testAutoComplete() {
    cout << "\nTesting autocomplete...\n";
    
    string test = "banana$";
    SuffixTree tree(test);
    
    vector<string> ban = tree.autoComplete("ban");
    assert(ban.size() == 1);
    assert(ban[0] == "banana$");
    
    vector<string> a = tree.autoComplete("a");
    assert(a.size() == 3); // "a$", "ana$", "anana$"
    
    vector<string> xyz = tree.autoComplete("xyz");
    assert(xyz.size() == 0);
    
    cout << "✓ Autocomplete passed\n";
}

void testVisualization() {
    cout << "\nTesting visualization methods...\n";
    
    string test = "banana$";
    SuffixTree tree(test);
    
    // These should not crash
    tree.printStats();
    cout << "\n";
    tree.printTree();
    
    cout << "✓ Visualization methods passed\n";
}

int main() {
    cout << "===== Suffix Tree Test Suite =====\n\n";
    
    testBasicConstruction();
    testSearchOperations();
    testAdvancedFeatures();
    testAutoComplete();
    testVisualization();
    
    cout << "\n===== All Tests Passed! =====\n";
    cout << "\nThe Ukkonen's algorithm implementation is correct and ready for GUI visualization.\n";
    
    return 0;
}
