/*
 * test_suffixtree.cpp - Simple test program for SuffixTree
 * 
 * This program tests the basic functionality of the SuffixTree class
 * without requiring GUI components.
 * 
 * Compile: g++ -std=c++11 test_suffixtree.cpp SuffixTree.cpp -o test_suffixtree
 * Run: ./test_suffixtree
 */

#include "SuffixTree.hpp"
#include <iostream>
#include <string>
#include <vector>

// Constants
const int ALPHABET_SIZE = 256;

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::exception;

// Helper function to count nodes in the tree
int countNodes(Node* node, bool& hasLeaf) {
    if (!node) return 0;
    
    int count = 1;
    bool hasAnyLeaf = (node->suffixIndex != -1);
    
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != nullptr) {
            bool childHasLeaf = false;
            count += countNodes(node->children[i]->dest, childHasLeaf);
            hasAnyLeaf = hasAnyLeaf || childHasLeaf;
        }
    }
    
    hasLeaf = hasAnyLeaf;
    return count;
}

// Test building suffix tree step by step
void testStepByStep() {
    cout << "\n=== Test: Step-by-step tree building ===" << endl;
    
    string input = "banana";
    SuffixTree tree(input.c_str());
    
    cout << "Building tree for: '" << input << "'" << endl;
    
    for (size_t i = 0; i < input.length() + 1; i++) {
        tree.extend(i);
        
        bool hasLeaf = false;
        int nodeCount = countNodes(tree.root, hasLeaf);
        
        cout << "Step " << i << ": " 
             << "Nodes = " << nodeCount 
             << ", Active node = " << (tree.activeNode == tree.root ? "root" : "internal")
             << ", Active length = " << tree.activeLength
             << endl;
    }
    
    cout << "✓ Step-by-step building completed successfully" << endl;
}

// Test building complete tree at once
void testCompleteBuild() {
    cout << "\n=== Test: Complete tree building ===" << endl;
    
    string input = "banana";
    SuffixTree tree(input.c_str());
    
    cout << "Building complete tree for: '" << input << "'" << endl;
    tree.build();
    
    bool hasLeaf = false;
    int nodeCount = countNodes(tree.root, hasLeaf);
    
    cout << "Total nodes: " << nodeCount << endl;
    cout << "Tree size (text length): " << tree.size << endl;
    cout << "Expected suffixes: " << tree.size << endl;
    
    if (nodeCount > 0 && hasLeaf) {
        cout << "✓ Complete tree building successful" << endl;
    } else {
        cout << "✗ Tree building may have issues" << endl;
    }
}

// Test with different inputs
void testDifferentInputs() {
    cout << "\n=== Test: Different input strings ===" << endl;
    
    vector<string> testCases = {
        "abc",
        "aaa",
        "abcab",
        "mississippi"
    };
    
    for (const string& input : testCases) {
        SuffixTree tree(input.c_str());
        tree.build();
        
        bool hasLeaf = false;
        int nodeCount = countNodes(tree.root, hasLeaf);
        
        cout << "Input: '" << input << "' -> Nodes: " << nodeCount 
             << " (Expected suffixes: " << (input.length() + 1) << ")" << endl;
    }
    
    cout << "✓ All test cases completed" << endl;
}

// Test tree structure
void testTreeStructure() {
    cout << "\n=== Test: Tree structure ===" << endl;
    
    string input = "ab";
    SuffixTree tree(input.c_str());
    tree.build();
    
    // Check root exists
    if (tree.root == nullptr) {
        cout << "✗ Root is null" << endl;
        return;
    }
    cout << "✓ Root exists" << endl;
    
    // Check root has children
    bool hasChildren = false;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (tree.root->children[i] != nullptr) {
            hasChildren = true;
            break;
        }
    }
    
    if (hasChildren) {
        cout << "✓ Root has children" << endl;
    } else {
        cout << "✗ Root has no children" << endl;
    }
    
    // Check text is stored correctly
    if (tree.text != nullptr) {
        cout << "✓ Text stored: '" << tree.text << "'" << endl;
    }
}

int main() {
    cout << "======================================" << endl;
    cout << "  Suffix Tree Test Suite" << endl;
    cout << "======================================" << endl;
    
    try {
        testTreeStructure();
        testStepByStep();
        testCompleteBuild();
        testDifferentInputs();
        
        cout << "\n======================================" << endl;
        cout << "  All tests completed!" << endl;
        cout << "======================================" << endl;
        
        return 0;
    }
    catch (const exception& e) {
        cerr << "\n✗ Test failed with exception: " << e.what() << endl;
        return 1;
    }
}
