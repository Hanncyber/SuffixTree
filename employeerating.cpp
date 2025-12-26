#include "employeerating.h"
#include "SuffixTree.h"

EmployeeRating::EmployeeRating(int n) {
    this->n = n;
    timer = 0;

    // Allocate arrays for employee ratings and DFS times
    rating = new int[n];
    tin = new int[n];
    tout = new int[n];

    // Allocate adjacency lists for hierarchy
    tree = new int*[n];
    childCount = new int[n];

    for (int i = 0; i < n; i++) {
        tree[i] = new int[n];   // maximum n children possible
        childCount[i] = 0;      // no subordinates yet
        rating[i] = 0;          // initialize ratings to 0
        tin[i] = 0;
        tout[i] = 0;
    }
    parent = new int[n];
    for (int i = 0; i < n; i++) {
        parent[i] = -1;
    }

    dfsString = "";            // DFS-linearized sequence not built yet
    suffixTree = nullptr;      // suffix tree will be built later
}

EmployeeRating::EmployeeRating() {
    n = 0;
    timer = 0;

    rating = nullptr;
    tin = nullptr;
    tout = nullptr;

    tree = nullptr;
    childCount = nullptr;
    parent = nullptr;

    dfsString = "";
    suffixTree = nullptr;
}

EmployeeRating::~EmployeeRating() {
    if (rating) delete[] rating;
    if (tin) delete[] tin;
    if (tout) delete[] tout;

    if (childCount) delete[] childCount;
    if (parent) delete[] parent;
    if (tree) {
        for (int i = 0; i < n; i++) {
            if (tree[i]) delete[] tree[i];
        }
        delete[] tree;
    }

    if (suffixTree) delete suffixTree;
}

string EmployeeRating::addSubordinate(char manager, char employee) {
    int m = charToIndex(manager);
    int e = charToIndex(employee);

    if (e == 0) {
        return "Root employee H cannot have a manager.";
    }

    if (parent[e] != -1) {
        return "Employee already has a manager.";
    }

    if (m == e) {
        return "Employee cannot manage themselves.";
    }

    parent[e] = m;


    tree[m][childCount[m]++] = e;
    return "";  // Empty string means success
}

// ------------------- Set initial rating -------------------
void EmployeeRating::setInitialRating(char employee, int value) {
    int index = charToIndex(employee); // map employee to internal index
    rating[index] = value;
}

void EmployeeRating::dfs(int employee) {
    // Record entry time
    tin[employee] = timer++;

    // Add this employee's rating to the dfsString
    // We map ratings to characters using offset 1-127
    char ratingChar = char((rating[employee] + 33)); // 33 to stay in printable ASCII
    dfsString += ratingChar;

    // Visit all subordinates
    for (int i = 0; i < childCount[employee]; i++) {
        int child = tree[employee][i];
        dfs(child);
    }

    // Record exit time
    tout[employee] = timer - 1;
}

// ------------------- Build DFS + Suffix Tree -------------------
void EmployeeRating::build() {
    dfsString = "";   // reset string
    timer = 0;

    // Start DFS from root: H -> index 0
    dfs(0);

    // Build suffix tree using DFS-linearized string
    if (suffixTree) delete suffixTree;  // delete previous suffix tree if exists
    suffixTree = new SuffixTree(dfsString);
}

int gcd(int a, int b) {
    while (b != 0) {
        int tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}


void EmployeeRating::updateSubtree(char employee, int value) {
    int idx = charToIndex(employee);

    for (int i = 0; i < n; i++) {
        if (tin[i] >= tin[idx] && tin[i] <= tout[idx]) {
            rating[i] += value;
        }
    }


}


int EmployeeRating::queryPerformance(char employee) {
    int idx = charToIndex(employee);
    int result = 0;

    // Compute GCD over all employees in the subtree
    for (int i = 0; i < n; i++) {
        if (tin[i] >= tin[idx] && tin[i] <= tout[idx]) {
            if (result == 0)
                result = rating[i];
            else
                result = gcd(result, rating[i]);
        }
    }

    return result;
}
