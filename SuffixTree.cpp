#include "SuffixTree.h"
#include <functional>

//Constructor
SuffixTree::SuffixTree(string text) {
    this->treeText = text;
    rootEnd = new int (-1); //end pointer
    root = new SuffixNode(-1, rootEnd); //root node 

    activeNode =root; //start at root
    activeEdge = -1; //no active edge
    activeLength = 0; //no walked length
    remainder=0; //no suffixes waiting
    size = -1; //current length of text

    for (int i = 0; i < text.length(); i++) {
        extendSuffixTree(i); //build tree
    }

    int labelHeight = 0;
    setSuffixIndexByDFS(root, labelHeight); //lable tree
}

//labels every leaf with its starting index
void SuffixTree::setSuffixIndexByDFS(SuffixNode *n, int labelHeight) {
    if (!n) return;

    bool isLeaf = true;
    for (auto const& [key, child] : n->children) { //loop through every item by unpacking char and node pointer
        isLeaf = false;
        // Recurse down, adding the edge length to our depth
        setSuffixIndexByDFS(child, labelHeight + edgeLength(child));
    }

    if (isLeaf) {
        // Formula: Index = Total Length - Path Length
        // e.g. Text "ABC$" (Len 4). Path "C$" (Len 2). Index = 4 - 2 = 2.
        n->suffixIndex = treeText.length() - labelHeight;
    }
}

//search
vector<int> SuffixTree::getOccurrenceIndices(string pattern) {
    vector<int> result;

    //traverse the tree to match the pattern
    SuffixNode* curr = root;
    int i = 0;

    while (i < pattern.length()) {
        char c = pattern[i];

        //if no edge starts with this char, pattern doesn't exist
        if (curr->children.find(c) == curr->children.end())
            return result;

        SuffixNode* next = curr->children[c];
        int edgeLen = edgeLength(next);

        //match characters along this edge
        for (int j = 0; j < edgeLen && i < pattern.length(); j++) {
            if (treeText[next->start + j] != pattern[i])
                return result; //mismatch
            i++;
        }
        curr = next;
    }

    //we found the node,collect all indices below it
    collectLeafIndices(curr, result);
    return result;
}

// Recursive helper to gather indices from the subtree
void SuffixTree::collectLeafIndices(SuffixNode* n, vector<int>& result) {
    //if it's a leaf, add its index
    if (n->suffixIndex != -1) {
        result.push_back(n->suffixIndex);
        return;
    }

    //if internal node, check all children
    for (auto const& [key, child] : n->children) {
        collectLeafIndices(child, result);
    }
}


//Destructor
SuffixTree::~SuffixTree() {
    freeTree(root);
    delete rootEnd; //delete the shared end pointer
}

void SuffixTree::freeTree(SuffixNode* node) { //delete nodes
    if (!node) return;

    // delete all children
    for (auto const& [key, child] : node->children) { //delete children first
        freeTree(child);
    }

    if (node->end != rootEnd) //deletes pointer to end
        delete node->end;

    delete node;
}

//Calc edge length
int SuffixTree::edgeLength(SuffixNode *n) {
    if (n == root) return 0;
    return *(n->end) - (n->start) + 1; //extra check to get length
}

void SuffixTree::extendSuffixTree(int pos) {
    //extend all leaves by 1
    size++;
    *rootEnd = size;
    //count suffixes to process
    remainder++;
    //keep track of last node made
    SuffixNode *lastNewNode = nullptr;
    while (remainder > 0) {
        if (activeLength == 0) {
            activeEdge=pos;
        }
        char currentEdgeChar = treeText[activeEdge]; //track which char we want
        //edge doesnt exist
        if (activeNode->children.find(currentEdgeChar)==activeNode->children.end()) {
            activeNode->children[currentEdgeChar] = new SuffixNode(pos, rootEnd); //make new leaf
            //link nodes
            if (lastNewNode != nullptr) {
                lastNewNode->suffixLink = activeNode;
                lastNewNode = nullptr;
            }
            //edge does exist
        }else {
            SuffixNode *next = activeNode->children[currentEdgeChar];
            //traverse
            int edgeLen = edgeLength(next);
            if (activeLength >= edgeLen) {
                activeEdge += edgeLen;
                activeLength -= edgeLen;
                activeNode = next;
                continue;
            }
            //char already exists
            if (treeText[next->start + activeLength] == treeText[pos]) {
                //linking
                if (lastNewNode != nullptr && activeNode != root) {
                    lastNewNode->suffixLink = activeNode;
                    lastNewNode = nullptr;
                }
                //move our cursor forward.
                activeLength++;
                break;
            }
            //char doesnt exist
            //split node
            int *splitEnd = new int(next->start + activeLength - 1);
            SuffixNode *split = new SuffixNode(next->start, splitEnd);
            //link
            activeNode->children[currentEdgeChar] = split; //parent to split
            split->children[treeText[next->start + activeLength]] = next; //split to old child
            next->start += activeLength;
            split->children[treeText[pos]] = new SuffixNode(pos, rootEnd); //split to new child
            if (lastNewNode != nullptr) {
                lastNewNode->suffixLink = split;
            }
            lastNewNode = split;
        }//inserted successfully
        remainder--;
        if (activeNode == root && activeLength > 0) {
            activeLength--;
            activeEdge = pos - remainder + 1; // Shift right
        } else if (activeNode != root) {
            activeNode = (activeNode->suffixLink != nullptr) ? activeNode->suffixLink : root;
        }
    }
}


long long SuffixTree::countAllUniqueSubstrings() {
    long long totalCount = 0;

    //define the recursive function locally
    function<void(SuffixNode*)> dfs = [&](SuffixNode* n) {
        if (!n) return;
        //add length of the edge leading to this node
        totalCount += edgeLength(n);
        //loop through the map
        for (auto const& [key, child] : n->children) {
            dfs(child);//go deeper
        }
    };
    //start recursion
    dfs(root);
    return totalCount;
}

string SuffixTree::getLexicographicallyFirstSuffix() {
    SuffixNode* curr = root;
    string result = "";

    //loop till we hit a leaf
    while (!curr->children.empty()) {
        //place iterator on the first element
        auto it = curr->children.begin(); //get smallest char key
        SuffixNode* next = it->second; //node pointer
        //append the text on this edge
        int len = edgeLength(next);
        for (int i = 0; i < len; i++) {
            result += treeText[next->start + i];
        }
        //move down
        curr = next;
    }
    return result;
}

string SuffixTree::getMostFrequentSubstring(int minLength) {
    int maxFreq = 0;
    string bestString = "";

    //local recursive function
    function<int(SuffixNode*, int)> dfs = [&](SuffixNode* n, int depth) -> int {
        //base case: leaf node = 1 occurrence
        if (n->children.empty()) {
            return 1;
        }

        int totalLeaves = 0; //my total count
        //count total leaves
        for (auto const& [key, child] : n->children) {
            totalLeaves += dfs(child, depth + edgeLength(child));
        }

        //check if this is the new most frequent
        if (depth >= minLength && totalLeaves > maxFreq) {
            maxFreq = totalLeaves;

            // Reconstruct the string roughly
            //find a random leaf below to get the valid text
            SuffixNode* temp = n;
            //dive until we hit a leaf
            while(!temp->children.empty()) temp = temp->children.begin()->second;
            //use leaf index to find string
            bestString = treeText.substr(temp->suffixIndex, depth);
        }
        return totalLeaves;
    };

    dfs(root, 0);
    return bestString;
}

string SuffixTree::getShortestUniqueSubstring() {
    string shortest = "";
    int minLen = treeText.length() + 1; // Start with infinity

    function<void(SuffixNode*, int)> dfs = [&](SuffixNode* n, int depth) {
        for (auto const& [key, child] : n->children) {
            // Calculate depth of the CHILD
            int childDepth = depth + edgeLength(child);

            // Is the child a leaf?
            if (child->children.empty()) {
                // Yes! This path leads to a unique suffix.
                // The unique part starts 1 char after the parent node.
                int candidateLen = depth + 1;

                if (candidateLen < minLen) {
                    minLen = candidateLen;
                    shortest = treeText.substr(child->suffixIndex, candidateLen);
                }
            } else {
                // Not a leaf, go deeper
                dfs(child, childDepth);
            }
        }
    };

    dfs(root, 0);

    if (minLen > treeText.length()) return "No unique substring found.";
    return shortest;
}


