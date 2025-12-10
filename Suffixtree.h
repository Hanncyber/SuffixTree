#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

using namespace std;

struct SuffixNode {
    //store children
    map<char, SuffixNode*> children; //key is char , value is pointer
    
    int start;          //starting index of the substring on this edge
    int *end;           //pointer to end index
    int suffixIndex;    //stores the starting index of the suffix
    SuffixNode *suffixLink; //link to the node representing the suffix of this node

    SuffixNode(int start, int *end) 
        : start(start), end(end), suffixIndex(-1), suffixLink(nullptr) {}
};

class SuffixTree {
public:
    //constructor/destructor
    SuffixTree(string text);
    ~SuffixTree();
    
    //add a new string (with unique terminator) for Generalized Tree operations
    void addString(string newString); 

    // search operations
    bool contains(string pattern);
    int countOccurrences(string pattern);
    
    //returns a list of all starting indices where pattern appears
    vector<int> getOccurrenceIndices(string pattern);

    //extra methods
    //longest repeated substring
    string getLongestRepeatedSubstring();
    
    //longest substring between 2 strings
    string getLongestCommonSubstring(); 

    //gui
    //shows likely completions for the given prefix
    vector<string> autoComplete(string prefix); 
    
    //returns the pattern surrounding by 'contextSize' chars (e.g., for search snippets)
    string getContext(string pattern, int contextSize);

    //debugging
    void printStats(); // Prints complexity metrics (nodes, size, time)
    void printTree();  // Console visualization of the tree structure


    //extra functions
    string getLongestPalindrome();
    string getShortestUniqueSubstring();
    string getLexicographicallyFirstSuffix(); //appears first in dictionary
    long long countAllUniqueSubstrings();
    string getMostFrequentSubstring(int minLength);

private:
    string treeText;
    SuffixNode *root;
    
    //ukkonen variables
    SuffixNode *activeNode; // The node we are currently looking at
    int activeEdge;         // The index of the character identifying the edge we are on
    int activeLength;       // How far down that edge we are
    int remainder;          // Number of suffixes remaining to be added
    
    int *rootEnd;           // Global end index for internal nodes
    int *splitEnd;          // Helper to manage split edges
    int size;               // Current total length of text

    // --- Internal Helpers ---
    void extendSuffixTree(int pos);          // The main logic of Ukkonen's algorithm
    int edgeLength(SuffixNode *n);           // Helper to calculate length of edge safely
    void freeTree(SuffixNode *node);         // Recursive destructor
    void setSuffixIndexByDFS(SuffixNode *n, int labelHeight); // Labels leaves with their suffix index

    // --- Recursive Backends for Public Functions ---
    int countLeafNodes(SuffixNode* n); 
    void collectLeafIndices(SuffixNode* n, vector<int>& result); 
    
    // Traverses tree to find deepest internal node (Repeated Substring)
    void findLRS(SuffixNode* n, int labelHeight, int* maxHeight, int* startIndex);
    
    // Traverses tree to find deepest node shared by different source strings (LCS)
    int findLCS(SuffixNode* n, int labelHeight, int* maxHeight, int* startIndex, int splitIndex);

};

#endif // SUFFIX_TREE_H