#include "SuffixTree.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    cout << "=== Suffix Tree Interactive Tester ===\n";

    string referenceText;
    cout << "Enter the reference text (will build suffix tree): ";
    getline(cin, referenceText);

    // Build suffix tree
    SuffixTree tree(referenceText);
    cout << "Suffix Tree built successfully.\n";

    while (true) {
        cout << "\n--- Menu ---\n";
        cout << "1. Search for a pattern\n";
        cout << "2. Detect DNA mutations in a sample\n";
        cout << "3. Find longest repeated pattern\n";
        cout << "4. Predict completions for a prefix\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore(); // discard newline

        if (choice == 1) {
            string pattern;
            cout << "Enter pattern to search: ";
            getline(cin, pattern);
            int count;
            int* positions = tree.search(pattern, count);
            if (count == 0)
                cout << "Pattern not found.\n";
            else {
                cout << "Pattern found at positions: ";
                for (int i = 0; i < count; i++)
                    cout << positions[i] << " ";
                cout << endl;
            }

        } else if (choice == 2) {
            string sample;
            cout << "Enter DNA sample to check for mutations: ";
            getline(cin, sample);
            tree.detectDNAMutationss(sample);

        } else if (choice == 3) {
            tree.detect_longest_pattern();

        } else if (choice == 4) {
            string prefix;
            int maxSug;
            cout << "Enter prefix for completion: ";
            getline(cin, prefix);
            cout << "Maximum suggestions to show: ";
            cin >> maxSug;
            cin.ignore();
            tree.predictCompletions(prefix, maxSug);

        } else if (choice == 5) {
            cout << "Exiting...\n";
            break;

        } else {
            cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}