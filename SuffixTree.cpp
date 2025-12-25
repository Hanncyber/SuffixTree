#include "SuffixTree.h"
#include <iostream>
using namespace std;
SuffixTree::SuffixTree(string text) {
    treeText = text + "$"; 
    leafptr= new int(-1);

    root = new SuffixNode(-1, leafptr);
    activeNode = root;

    activeEdge = -1;
    activeLength = 0;
    remainder = 0;
    size = -1;

    for (int i = 0; i < (int)treeText.length(); i++)
        extendSuffixTree(i);

    markLeafPositions(root, 0);
}

SuffixTree::~SuffixTree() {
    freeTree(root);
    delete leafptr;
}


void SuffixTree::freeTree(SuffixNode* node) {
    if (!node) return;

    for (int i = 0; i < 128; i++)
        freeTree(node->children[i]);

    if (node->end != leafptr)
        delete node->end;

    delete node;
}


int SuffixTree::edgeLength(SuffixNode* n) {
    if (n == root) return 0;
    return *(n->end) - n->start + 1;
}

void SuffixTree::markLeafPositions(SuffixNode* n, int labelHeight) {
    if (!n) return;

    bool isLeaf = true;

    for (int i = 0; i < 128; i++) {
        if (n->children[i]) {
            isLeaf = false;
            markLeafPositions(
                n->children[i],
                labelHeight + edgeLength(n->children[i])
            );
        }
    }

    if (isLeaf)
        n->suffix_index = treeText.length() - labelHeight;
}


void SuffixTree::extendSuffixTree(int pos) {
    size++;
    *leafptr = size;
    remainder++;

    SuffixNode* lastNewNode = nullptr;

    while (remainder > 0) {

        if (activeLength == 0)
            activeEdge = pos;

        unsigned char currentChar = treeText[activeEdge];

        if (!activeNode->children[currentChar]) {

            activeNode->children[currentChar] =
                new SuffixNode(pos, leafptr);

            if (lastNewNode) {
                lastNewNode->suffixLink = activeNode;
                lastNewNode = nullptr;
            }

        } else {

            SuffixNode* next = activeNode->children[currentChar];
            int edgeLen = edgeLength(next);

            if (activeLength >= edgeLen) {
                activeEdge += edgeLen;
                activeLength -= edgeLen;
                activeNode = next;
                continue;
            }

            if (treeText[next->start + activeLength] == treeText[pos]) {
                if (lastNewNode && activeNode != root) {
                    lastNewNode->suffixLink = activeNode;
                    lastNewNode = nullptr;
                }
                activeLength++;
                break;
            }

            int* splitEnd = new int(next->start + activeLength - 1);
            SuffixNode* split = new SuffixNode(next->start, splitEnd);

            activeNode->children[currentChar] = split;

            split->children[
                (unsigned char)treeText[next->start + activeLength]
            ] = next;

            next->start += activeLength;

            split->children[(unsigned char)treeText[pos]] =
                new SuffixNode(pos, leafptr);

            if (lastNewNode)
                lastNewNode->suffixLink = split;

            lastNewNode = split;
        }

        remainder--;

        if (activeNode == root && activeLength > 0) {
            activeLength--;
            activeEdge = pos - remainder + 1;
        } else if (activeNode != root) {
            activeNode = activeNode->suffixLink ?
                         activeNode->suffixLink : root;
        }
    }
}


void SuffixTree::collectLeafIndices(
    SuffixNode* n,
    int result[],
    int& count
) {
    if (!n) return;

    if (n->suffix_index != -1) {
        result[count++] = n->suffix_index;
        return;
    }

    for (int i = 0; i < 128; i++)
        collectLeafIndices(n->children[i], result, count);
}


int* SuffixTree::search(const string& pattern, int& count) {
    static int result[1000];
    count = 0;

    SuffixNode* curr = root;
    int i = 0;

    while (i < (int)pattern.length()) {
        unsigned char c = pattern[i];

        if (!curr->children[c])
            return result;

        SuffixNode* next = curr->children[c];
        int edgeLen = edgeLength(next);

        for (int k = 0; k < edgeLen && i < (int)pattern.length(); k++, i++) {
            if (treeText[next->start + k] != pattern[i])
                return result;
        }

        curr = next;
    }

    collectLeafIndices(curr, result, count);
    return result;
}

//find deepest internal node (longest repeated substring)
void SuffixTree::find_longest_repeatedSubstring(
    SuffixNode* node,
    int labelheight,
    int& maxLength,
    int& startIndex
) {
    if (!node) return;

    // Internal node (not leaf)
    bool is_leaf = (node->suffix_index != -1);

    if (!is_leaf) {
        for (int i = 0; i < 128; i++)
        {
            if (node->children[i])
            {
                find_longest_repeatedSubstring(
                    node->children[i],
                    labelheight + edgeLength(node->children[i]),
                    maxLength,
                    startIndex
                );
            }
        }

        // Update longest substring if this internal node is deeper
        if (labelheight > maxLength) {
            maxLength = labelheight;
            startIndex = *(node->end) - labelheight + 1;
        }
    }
}

void SuffixTree::detect_longest_pattern() 
{
    int maxlength = 0;
    int startindex = -1;

    find_longest_repeatedSubstring(root, 0, maxlength, startindex);

    if (maxlength > 0) 
    {
        cout << "-longest repeated pattern-\n";
        cout << " pattern:\"" << treeText.substr(startindex, maxlength) << "\"\n";
        cout << "  length:  " << maxlength << "\n";
        cout << "  start index: " << startindex << "\n";
    }
    else 
    {
        cout << "No repeated pattern found.\n";
    }
}

void SuffixTree::predictCompletions(const string& prefix, int maxSuggestions, int no)
{
    SuffixNode* cur = root;
    int i = 0;
    while (i < (int)prefix.length()){
        char c = prefix[i];
        if (!cur->children[(int)c]) {
            cout << "No suggestions found for \"" << prefix << "\"\n";
            return;
        }
        SuffixNode* next = cur->children[(int)c];
        int edgelen = edgeLength(next);

        for (int k = 0; k < edgelen && i < (int)prefix.length(); k++, i++)
        {
            if (treeText[next->start + k] != prefix[i]) {
                cout << "No suggestions found for \"" << prefix << "\"\n";
                return;
            }
        }
        cur = next;
    }
    int positions[1000];
    int count = 0;
    collectLeafIndices(cur, positions, count);

    cout << "Prefix given \"" << prefix << "\":\n";
    if (count > no) {
        cout << "No suggestions found.\n";
        return;
    }
    int suggestionsCount = 0;
    for (int j = 0; j < count && suggestionsCount < maxSuggestions; j++)
    {
        int start = positions[j];
        string word;
        for (int k = start; k < (int)treeText.length(); k++) {
            char ch = treeText[k];
            if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '.' || ch == ',' || ch == '$') break;
            word += ch;
        }
        bool found = true;
        for (int k = 0; k < (int)prefix.length(); k++) {
            if (k >= (int)word.length() || word[k] != prefix[k]) {
                found = false;
                break;
            }
        }
        if (!found) continue;

        cout << "  " << word << "\n";
        suggestionsCount++;
    }
    if (suggestionsCount == 0) {
        cout << "  No suggestions found.\n";
    }
}
