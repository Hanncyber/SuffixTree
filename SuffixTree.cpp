#include "SuffixTree.hpp"
#include <iostream>
#include <cstring>

using namespace std;

// Node constructor
Node::Node() {
    for (int i = 0; i < 256; i++)
        children[i] = nullptr;
    suffixLink = nullptr;
    suffixIndex = -1;
}

// Edge constructor
Edge::Edge(Node* destNode, int s, int* e) {
    dest = destNode;
    start = s;
    end = e;
}

// SuffixTree constructor
SuffixTree::SuffixTree(const char* input) {
    size = strlen(input);
    text = new char[size + 2]; // +1 for '$' +1 for '\0'
    strcpy(text, input);
    text[size] = '$';
    text[size + 1] = '\0';
    size++;

    root = new Node();
    activeNode = root;
    activeEdge = -1;
    activeLength = 0;
    remainingSuffixCount = 0;
    leafEnd = new int(-1);
    lastNewNode = nullptr;
}

// Destructor
SuffixTree::~SuffixTree() {
    delete[] text;
    delete leafEnd;
}

// Build tree skeleton
void SuffixTree::build() {
    for (int i = 0; i < size; i++)
        extend(i); // we will implement extend next
}

void SuffixTree::extend(int pos) {
    (*leafEnd) = pos;           // all leaves grow automatically
    remainingSuffixCount++;     // new suffix to add
    lastNewNode = nullptr;      // reset last created internal node

    while (remainingSuffixCount > 0) {
        if (activeLength == 0)
            activeEdge = pos;   // start a new active edge

        int index = text[activeEdge];
        Edge* edge = activeNode->children[index];

        // Case 1: no edge starting with current char → create leaf
        if (edge == nullptr) {
            Node* leafNode = new Node();
            leafNode->suffixIndex = pos - remainingSuffixCount + 1;
            activeNode->children[index] = new Edge(leafNode, pos, leafEnd);

            if (lastNewNode != nullptr) {
                lastNewNode->suffixLink = activeNode;
                lastNewNode = nullptr;
            }
        }
        else { // Edge exists → walk along it
            int edgeLength = edge->length();

            if (activeLength >= edgeLength) {
                // move active point down the edge
                activeEdge += edgeLength;
                activeLength -= edgeLength;
                activeNode = edge->dest;
                continue;
            }

            char nextChar = text[edge->start + activeLength];
            if (text[pos] == nextChar) { // Rule 3: match
                activeLength++;
                if (lastNewNode != nullptr) {
                    lastNewNode->suffixLink = activeNode;
                    lastNewNode = nullptr;
                }
                break;
            }

            // Rule 2: mismatch → split edge
            int* splitEnd = new int(edge->start + activeLength - 1);
            Node* splitNode = new Node();

            // Create edge from active node to splitNode
            Edge* edgeToSplit = new Edge(splitNode, edge->start, splitEnd);
            activeNode->children[index] = edgeToSplit;

            // Update old edge to start after split
            edge->start += activeLength;
            splitNode->children[text[edge->start]] = edge;

            // Create new leaf for current suffix
            Node* leafNode = new Node();
            leafNode->suffixIndex = pos - remainingSuffixCount + 1;
            splitNode->children[text[pos]] = new Edge(leafNode, pos, leafEnd);

            if (lastNewNode != nullptr) lastNewNode->suffixLink = splitNode;
            lastNewNode = splitNode;
        }

        // Move active point
        remainingSuffixCount--;
        if (activeNode == root && activeLength > 0) {
            activeLength--;
            activeEdge = pos - remainingSuffixCount + 1;
        }
        else if (activeNode != root) {
            activeNode = activeNode->suffixLink != nullptr ? activeNode->suffixLink : root;
        }
    }
}

