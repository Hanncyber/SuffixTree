#include "employeerating.h"
#include <iostream>
#include <string>

using namespace std;

// Constants for employee limits
const int MIN_EMPLOYEES = 1;
const int MAX_EMPLOYEES = 26;

/**
 * Example program demonstrating the Employee Rating System
 * 
 * This program solves the problem described in the problem statement:
 * - Manages a company hierarchy as a tree with employees
 * - Each employee has a rating
 * - Performance is calculated as GCD of all employees in subtree
 * - Supports two query types:
 *   Type 0: Update employee and all subordinates' ratings by a value
 *   Type 1: Query performance (GCD) of an employee
 */

void printSeparator() {
    cout << "\n========================================\n";
}

void printExampleHeader(const string& title) {
    printSeparator();
    cout << "\n" << title << "\n";
    printSeparator();
}

void printExample1() {
    printExampleHeader("EXAMPLE 1: Simple Company Hierarchy");
    
    // Company with 5 employees: H (head), A, B, C, D
    EmployeeRating company(5);
    
    // Build hierarchy:
    //       H (rating: 12)
    //      / \
    //     A   B (ratings: 18, 24)
    //    / \
    //   C   D (ratings: 6, 30)
    
    cout << "Building hierarchy:\n";
    cout << "  H is the head\n";
    cout << "  H -> A, H -> B\n";
    cout << "  A -> C, A -> D\n";
    
    company.addSubordinate('H', 'A');
    company.addSubordinate('H', 'B');
    company.addSubordinate('A', 'C');
    company.addSubordinate('A', 'D');
    
    cout << "\nSetting initial ratings:\n";
    cout << "  H: 12, A: 18, B: 24, C: 6, D: 30\n";
    company.setInitialRating('H', 12);
    company.setInitialRating('A', 18);
    company.setInitialRating('B', 24);
    company.setInitialRating('C', 6);
    company.setInitialRating('D', 30);
    
    cout << "\nBuilding tree structure...\n";
    company.build();
    
    // Type 1: Query performance
    cout << "\n--- Type 1 Queries (Performance/GCD) ---\n";
    cout << "Performance of H (GCD of H,A,B,C,D): " << company.queryPerformance('H') << endl;
    cout << "  Explanation: GCD(12, 18, 24, 6, 30) = 6\n";
    
    cout << "\nPerformance of A (GCD of A,C,D): " << company.queryPerformance('A') << endl;
    cout << "  Explanation: GCD(18, 6, 30) = 6\n";
    
    cout << "\nPerformance of B: " << company.queryPerformance('B') << endl;
    cout << "  Explanation: GCD(24) = 24\n";
    
    // Type 0: Update subtree
    cout << "\n--- Type 0 Query (Update Subtree) ---\n";
    cout << "Updating A and all subordinates (C, D) by +6\n";
    company.updateSubtree('A', 6);
    
    cout << "\nAfter update, new ratings:\n";
    cout << "  H: 12 (unchanged), A: 24, B: 24 (unchanged), C: 12, D: 36\n";
    
    cout << "\n--- Type 1 Queries After Update ---\n";
    cout << "Performance of H: " << company.queryPerformance('H') << endl;
    cout << "  Explanation: GCD(12, 24, 24, 12, 36) = 12\n";
    
    cout << "\nPerformance of A: " << company.queryPerformance('A') << endl;
    cout << "  Explanation: GCD(24, 12, 36) = 12\n";
}

void printExample2() {
    printExampleHeader("EXAMPLE 2: Magazine Publishing Scenario");
    
    // Simulating Murphy's magazine scenario with 6 employees
    EmployeeRating company(6);
    
    cout << "Company: Tech Startup with 6 employees\n";
    cout << "Hierarchy:\n";
    cout << "       H (CEO)\n";
    cout << "      / \\\n";
    cout << "     A   B (VPs)\n";
    cout << "    /|   |\\\n";
    cout << "   C D   E (Managers)\n";
    
    company.addSubordinate('H', 'A');
    company.addSubordinate('H', 'B');
    company.addSubordinate('A', 'C');
    company.addSubordinate('A', 'D');
    company.addSubordinate('B', 'E');
    
    cout << "\nInitial Performance Ratings:\n";
    cout << "  CEO H: 100\n";
    cout << "  VP A: 80, VP B: 60\n";
    cout << "  Managers C: 40, D: 20, E: 30\n";
    
    company.setInitialRating('H', 100);
    company.setInitialRating('A', 80);
    company.setInitialRating('B', 60);
    company.setInitialRating('C', 40);
    company.setInitialRating('D', 20);
    company.setInitialRating('E', 30);
    
    company.build();
    
    cout << "\n--- Initial Performance (GCD) Calculations ---\n";
    cout << "CEO H's performance: " << company.queryPerformance('H') << endl;
    cout << "  (GCD of entire company: 100, 80, 60, 40, 20, 30)\n";
    
    cout << "\nVP A's performance: " << company.queryPerformance('A') << endl;
    cout << "  (GCD of A's division: 80, 40, 20)\n";
    
    cout << "\nVP B's performance: " << company.queryPerformance('B') << endl;
    cout << "  (GCD of B's division: 60, 30)\n";
    
    cout << "\n--- Year-End Bonus! ---\n";
    cout << "VP B's entire division gets +10 rating boost\n";
    company.updateSubtree('B', 10);
    
    cout << "\n--- After Bonus ---\n";
    cout << "VP B's performance: " << company.queryPerformance('B') << endl;
    cout << "  (B: 70, E: 40 -> GCD = 10)\n";
    
    cout << "\n--- Performance Review ---\n";
    cout << "A's division needs improvement: -10 rating adjustment\n";
    company.updateSubtree('A', -10);
    
    cout << "\nVP A's performance: " << company.queryPerformance('A') << endl;
    cout << "  (A: 70, C: 30, D: 10 -> GCD = 10)\n";
    
    cout << "\nCEO H's overall company performance: " << company.queryPerformance('H') << endl;
}

void interactiveMode() {
    printExampleHeader("INTERACTIVE MODE");
    
    int n;
    cout << "Enter number of employees (including head H): ";
    cin >> n;
    
    if (n < MIN_EMPLOYEES || n > MAX_EMPLOYEES) {
        cout << "Error: Number of employees must be between " 
             << MIN_EMPLOYEES << " and " << MAX_EMPLOYEES << "\n";
        return;
    }
    
    EmployeeRating company(n);
    
    cout << "\nEmployee naming: H (head), then A, B, C, ...\n";
    cout << "Example: For 5 employees, use H, A, B, C, D\n\n";
    
    cout << "How many subordinate relationships to add? ";
    int numRelations;
    cin >> numRelations;
    
    cout << "\nEnter subordinate relationships (manager subordinate):\n";
    for (int i = 0; i < numRelations; i++) {
        char manager, subordinate;
        cout << "  " << (i+1) << ". Manager Subordinate: ";
        cin >> manager >> subordinate;
        string errorMsg = company.addSubordinate(manager, subordinate);
        if (!errorMsg.empty()) {
            cout << "  Error: " << errorMsg << "\n";
        }
    }
    
    cout << "\nSet initial ratings for each employee:\n";
    for (int i = 0; i < n; i++) {
        char emp = (i == 0) ? 'H' : ('A' + i - 1);
        int rating;
        cout << "  Rating for " << emp << ": ";
        cin >> rating;
        company.setInitialRating(emp, rating);
    }
    
    cout << "\nBuilding hierarchy tree...\n";
    company.build();
    cout << "✓ Tree built successfully!\n";
    
    cout << "\n--- Query Processing ---\n";
    int numQueries;
    cout << "How many queries to perform? ";
    cin >> numQueries;
    
    for (int i = 0; i < numQueries; i++) {
        int type;
        cout << "\nQuery " << (i+1) << " - Type (0=Update, 1=Performance): ";
        cin >> type;
        
        if (type == 0) {
            char emp;
            int value;
            cout << "  Employee: ";
            cin >> emp;
            cout << "  Update by: ";
            cin >> value;
            company.updateSubtree(emp, value);
            cout << "  ✓ Updated " << emp << " and subordinates by " << value << endl;
        } else if (type == 1) {
            char emp;
            cout << "  Employee: ";
            cin >> emp;
            int performance = company.queryPerformance(emp);
            cout << "  Performance of " << emp << ": " << performance << endl;
        } else {
            cout << "  Invalid query type!\n";
        }
    }
    
    cout << "\n✓ All queries processed!\n";
}

int main() {
    cout << "╔════════════════════════════════════════════════════╗\n";
    cout << "║   EMPLOYEE RATING & PERFORMANCE SYSTEM DEMO        ║\n";
    cout << "║   Based on Murphy's Magazine Problem               ║\n";
    cout << "╚════════════════════════════════════════════════════╝\n";
    
    cout << "\nThis system manages company hierarchies where:\n";
    cout << "• Each employee has a rating\n";
    cout << "• Performance = GCD of all subordinates' ratings\n";
    cout << "• Type 0: Update employee & subordinates' ratings\n";
    cout << "• Type 1: Query employee's performance (GCD)\n";
    
    // Run examples
    printExample1();
    printExample2();
    
    // Interactive mode
    cout << "\n\n";
    char choice;
    cout << "Would you like to try INTERACTIVE MODE? (y/n): ";
    cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
        interactiveMode();
    }
    
    printSeparator();
    cout << "\nThank you for using the Employee Rating System!\n";
    cout << "For GUI version, run: ./SuffixTreeGUI\n";
    printSeparator();
    
    return 0;
}
