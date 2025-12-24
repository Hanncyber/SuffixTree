#ifndef EMPLOYEE_RATING_H
#define EMPLOYEE_RATING_H

#include "SuffixTree.h"
#include <string>

class EmployeeRating {
private:
    int n;              // number of employees
    int timer;          // DFS timer

    int* rating;        // integer rating per employee
    int* tin;           // DFS entry time
    int* tout;          // DFS exit time

    int** tree;         // adjacency list (children per employee)
    int* childCount;
    int* parent;


    string dfsString; // DFS-linearized rating string
    SuffixTree* suffixTree;

    void dfs(int employee);

    // Map character to internal index
    int charToIndex(char c) const {
        if (c == 'H') return 0;        // root
        return c - 'A' + 1;           // A->1, B->2, ...
    }

public:
    EmployeeRating();
    EmployeeRating(int n);
    ~EmployeeRating();

    // Add subordinate: manager and employee as characters
    // Returns empty string on success, error message on failure
    string addSubordinate(char manager, char employee);

    // Set initial rating using character
    void setInitialRating(char employee, int value);

    // Build DFS linearization and suffix tree
    void build();

    // Type 0: update subtree of employee by value
    void updateSubtree(char employee, int value);

    // Type 1: query performance of employee
    int queryPerformance(char employee);

    // Getters for tree visualization (const to prevent external modification)
    int getNumEmployees() const { return n; }
    int* const* getTree() const { return tree; }
    const int* getChildCount() const { return childCount; }
    const int* getRating() const { return rating; }
};

#endif
