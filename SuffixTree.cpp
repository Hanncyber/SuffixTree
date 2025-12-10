#include "Suffixtree.h"
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

// Print tree structure for visualization
void SuffixTree::printTree() {
    cout << "Suffix Tree for: \"" << treeText << "\"\n";
    cout << "===================================\n";
    
    function<void(SuffixNode*, string, int)> printNode = [&](SuffixNode* node, string prefix, int depth) {
        if (!node) return;
        
        for (auto const& [key, child] : node->children) {
            int start = child->start;
            int end = *(child->end);
            
            // Get the edge label
            string edgeLabel = treeText.substr(start, end - start + 1);
            
            // Print the edge
            cout << prefix << "├─ [" << start << "," << end << "] \"" << edgeLabel << "\"";
            
            // If it's a leaf, print suffix index
            if (child->suffixIndex != -1) {
                cout << " (Suffix: " << child->suffixIndex << ")";
            }
            cout << "\n";
            
            // Recurse to children
            printNode(child, prefix + "│  ", depth + 1);
        }
    };
    
    printNode(root, "", 0);
    cout << "===================================\n";
}

// Print statistics about the tree
void SuffixTree::printStats() {
    int nodeCount = 0;
    int leafCount = 0;
    int internalCount = 0;
    int maxDepth = 0;
    
    function<void(SuffixNode*, int)> countNodes = [&](SuffixNode* node, int depth) {
        if (!node) return;
        
        nodeCount++;
        maxDepth = max(maxDepth, depth);
        
        if (node->children.empty()) {
            leafCount++;
        } else {
            internalCount++;
        }
        
        for (auto const& [key, child] : node->children) {
            countNodes(child, depth + 1);
        }
    };
    
    countNodes(root, 0);
    
    cout << "Suffix Tree Statistics:\n";
    cout << "===================================\n";
    cout << "Text: \"" << treeText << "\"\n";
    cout << "Text Length: " << treeText.length() << "\n";
    cout << "Total Nodes: " << nodeCount << "\n";
    cout << "Leaf Nodes: " << leafCount << "\n";
    cout << "Internal Nodes: " << internalCount << "\n";
    cout << "Max Depth: " << maxDepth << "\n";
    cout << "Unique Substrings: " << countAllUniqueSubstrings() << "\n";
    cout << "===================================\n";
}

// Check if pattern exists in the text
bool SuffixTree::contains(string pattern) {
    return !getOccurrenceIndices(pattern).empty();
}

// Count occurrences of pattern
int SuffixTree::countOccurrences(string pattern) {
    return getOccurrenceIndices(pattern).size();
}

// Count leaf nodes in subtree
int SuffixTree::countLeafNodes(SuffixNode* n) {
    if (!n) return 0;
    
    if (n->children.empty()) {
        return 1;
    }
    
    int count = 0;
    for (auto const& [key, child] : n->children) {
        count += countLeafNodes(child);
    }
    return count;
}

// Get longest repeated substring
string SuffixTree::getLongestRepeatedSubstring() {
    int maxHeight = 0;
    int startIndex = -1;
    
    findLRS(root, 0, &maxHeight, &startIndex);
    
    if (maxHeight == 0 || startIndex == -1) {
        return "";
    }
    
    return treeText.substr(startIndex, maxHeight);
}

// Helper to find longest repeated substring
void SuffixTree::findLRS(SuffixNode* n, int labelHeight, int* maxHeight, int* startIndex) {
    if (!n) return;
    
    // Internal node with at least 2 children means repeated substring
    if (!n->children.empty() && labelHeight > *maxHeight) {
        *maxHeight = labelHeight;
        
        // Find a leaf to get the starting index
        SuffixNode* temp = n;
        while (!temp->children.empty()) {
            temp = temp->children.begin()->second;
        }
        *startIndex = temp->suffixIndex;
    }
    
    // Recurse to children
    for (auto const& [key, child] : n->children) {
        findLRS(child, labelHeight + edgeLength(child), maxHeight, startIndex);
    }
}

// Get longest common substring (for generalized suffix tree)
string SuffixTree::getLongestCommonSubstring() {
    // This requires a generalized suffix tree with multiple strings
    // For now, return empty string
    return "";
}

// Helper for LCS
int SuffixTree::findLCS(SuffixNode* n, int labelHeight, int* maxHeight, int* startIndex, int splitIndex) {
    // Implementation for generalized suffix tree
    return 0;
}

// Autocomplete suggestions
vector<string> SuffixTree::autoComplete(string prefix) {
    vector<string> results;
    
    // Find the node corresponding to the prefix
    SuffixNode* curr = root;
    int i = 0;
    
    while (i < prefix.length()) {
        char c = prefix[i];
        
        if (curr->children.find(c) == curr->children.end()) {
            return results; // Prefix not found
        }
        
        SuffixNode* next = curr->children[c];
        int edgeLen = edgeLength(next);
        
        // Match characters along this edge
        for (int j = 0; j < edgeLen && i < prefix.length(); j++) {
            if (treeText[next->start + j] != prefix[i]) {
                return results; // Mismatch
            }
            i++;
        }
        curr = next;
    }
    
    // Now collect all suffixes from this node
    function<void(SuffixNode*)> collectSuffixes = [&](SuffixNode* node) {
        if (node->suffixIndex != -1) {
            // Leaf node - we have a complete suffix starting at suffixIndex
            results.push_back(treeText.substr(node->suffixIndex));
            return;
        }
        
        for (auto const& [key, child] : node->children) {
            collectSuffixes(child);
        }
    };
    
    collectSuffixes(curr);
    return results;
}

// Get context around pattern
string SuffixTree::getContext(string pattern, int contextSize) {
    vector<int> occurrences = getOccurrenceIndices(pattern);
    
    if (occurrences.empty()) {
        return "";
    }
    
    // Get context for first occurrence
    int pos = occurrences[0];
    int start = max(0, pos - contextSize);
    int len = min((int)treeText.length() - start, (int)(pattern.length() + 2 * contextSize));
    
    return treeText.substr(start, len);
}

// Add string to generalized suffix tree
void SuffixTree::addString(string newString) {
    // For now, just append to existing text with a unique separator
    // A full implementation would require tracking which string each suffix belongs to
    treeText += "#" + newString;
    
    // Extend tree with new characters
    for (int i = treeText.length() - newString.length() - 1; i < treeText.length(); i++) {
        extendSuffixTree(i);
    }
    
    // Re-label suffix indices
    int labelHeight = 0;
    setSuffixIndexByDFS(root, labelHeight);
}

// Get longest palindrome
string SuffixTree::getLongestPalindrome() {
    // This requires building a generalized suffix tree with the string and its reverse
    // For a basic implementation, we can search for palindromes manually
    string longest = "";
    
    for (int center = 0; center < treeText.length(); center++) {
        // Odd length palindromes
        int left = center, right = center;
        while (left >= 0 && right < treeText.length() && treeText[left] == treeText[right]) {
            int len = right - left + 1;
            if (len > longest.length()) {
                longest = treeText.substr(left, len);
            }
            left--;
            right++;
        }
        
        // Even length palindromes
        left = center;
        right = center + 1;
        while (left >= 0 && right < treeText.length() && treeText[left] == treeText[right]) {
            int len = right - left + 1;
            if (len > longest.length()) {
                longest = treeText.substr(left, len);
            }
            left--;
            right++;
        }
    }
    
    return longest;
}


