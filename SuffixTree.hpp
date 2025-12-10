#ifndef SUFFIXTREE_HPP
#define SUFFIXTREE_HPP

class Node {
public:
    Edge* children[256]; // each child is an edge
    Node* suffixLink;    // suffix link for internal nodes
    int suffixIndex;     // -1 for internal nodes
    Node();              // constructor
};

class Edge {
public:
    Node* dest;   // destination node
    int start;    // start index in text
    int* end;     // pointer, shared for leaves
    Edge(Node* destNode, int s, int* e);
    
    int length() { return *end - start + 1; } // convenient
};


class SuffixTree {
public:
    char* text;          // input text + $
    int size;

    Node* root;          // root of tree
    Node* activeNode;    // active point node
    int activeEdge;      // index in text for active edge
    int activeLength;    // how far along active edge
    int remainingSuffixCount; // pending suffixes
    int* leafEnd;        // shared end for leaves
    Node* lastNewNode;   // last created internal node

    SuffixTree(const char* input);
    ~SuffixTree();

    void build();        // build tree
    void extend(int pos); // extend tree with text[pos]
};

#endif
