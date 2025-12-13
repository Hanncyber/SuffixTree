#include "SuffixTree.h"
#include <iostream>

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

void SuffixTree::detectDNAMutationss(const string& sample) {
    cout << "Mutation Report: "<<endl;

    int n = sample.length();

    for (int i = 0; i < n; ) {

        SuffixNode* curr = root;
        int pos = i;
        bool mismatch = false;

        int mismatchPositionn= -1;
        int mismatchEdgeStartttt = -1;

        while (pos < n) {

            if (!curr->children[(int)sample[pos]]) { //make sure en 3adad el sample ad 3adad el refrence 
                //insertion mutation 
                  mismatch = true;
                break;
            }

            SuffixNode* next = curr->children[(int)sample[pos]];
            int eLen = edgeLength(next);

            for (int k = 0; k < eLen && pos < n; k++, pos++) {
                if (treeText[next->start + k] != sample[pos]) {
                    mismatch = true;
                    mismatchPositionn = k;
                    mismatchEdgeStartttt = next->start;
                    break;
                }
            }

            if (mismatch) break;
            curr = next;
        }

        if (!mismatch) {
            i = pos;
            continue;
        }

        cout << "mutationn at sample index " << i << ":" << endl;

        if (mismatchPositionn == -1) {
            cout << "  type: Insertion"<<endl;
            cout << "  Sample base: " << sample[i] << endl;
            cout << endl;
            i++;
            continue;
        }

        cout << "  Reference base: "
            << treeText[mismatchPositionn + mismatchEdgeStartttt] << endl;
        cout << "  sample base:    " << sample[pos] << endl;
        cout << "  type: Substitution" << endl;
        cout << endl;
        i++;
    }

    cout << "end of report. "<<endl;
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

void SuffixTree::predictCompletions(const string& prefix, int maxSuggestions)
{
    SuffixNode* cur = root;
    int i = 0;
      // Traverse the tree along the prefix
    while (i < (int)prefix.length()){
        char c = prefix[i];
        if (!cur->children[(int)c]) {
            cout << "no suggestions/prid found \"" << prefix << "\"\n";
            return;
        }
        SuffixNode* next = cur->children[(int)c];
        int edgeLen = edgeLength(next);

        for (int k = 0; k < edgeLen && i < (int)prefix.length(); k++, i++) 
        {
            if (treeText[next->start + k] != prefix[i]) {
                cout << "No suggestions/prid found  \"" << prefix << "\".\n";
                return;
            }
        }
        cur = next;
    }
    int result[1000];
    int count = 0;
    collectLeafIndices(cur, result, count);
    cout << "prefix given \"" << prefix << "\":\n";

    for (int j = 0; j < count && j < maxSuggestions; j++)//print
    {
        int start = result[j];
        int len = 20; 
        string completion = treeText.substr(start, len);
        cout << "  " << completion << "\n";
    }
}
