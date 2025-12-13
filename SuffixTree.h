#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H

#include <string>
using namespace std;

class SuffixNode {
public:
    int start;              
    int* end;               
    int suffix_index;       
    SuffixNode* suffixLink; 
    SuffixNode* children[128];

    SuffixNode(int s, int* e) {
        start = s;
        end = e;
        suffix_index = -1;
        suffixLink = nullptr;

        for (int i = 0; i < 128; i++)
            children[i] = nullptr;
    }
};
class SuffixTree {
private:
    string treeText;        
    SuffixNode* root;
    SuffixNode* activeNode;
    int activeEdge;
    int activeLength;
    int remainder;
    int size;
    int* leafptr;

    void extendSuffixTree(int pos);
    int edgeLength(SuffixNode* n);
    void markLeafPositions(SuffixNode* n, int labelHeight);
    void freeTree(SuffixNode* node);
    void collectLeafIndices(SuffixNode* n, int result[], int& count);

public:
    SuffixTree(string text);
    ~SuffixTree();
    int* search(const string& pattern, int& count);
    //APPLICATIONS
    // Tfor DNA mutation detection
    void detectDNAMutationss(const string& sample);
    // for longest repeated pattern
    void detect_longest_pattern();
    void  find_longest_repeatedSubstring(SuffixNode* node, int labelHeight, int& maxLength, int& startIndex);
    // for google predicition
    void predictCompletions(const string& prefix, int maxSuggestions = 5);
    // Getters for GUI visualization
    SuffixNode* getRoot() const { return root; }
    string getText() const { return treeText; }
};

#endif
