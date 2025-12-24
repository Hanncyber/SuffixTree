# Employee Rating System - Feature Documentation

## Overview

The Employee Rating System is a new feature added to the Suffix Tree Application that manages company hierarchies and employee performance ratings. It implements the solution to Murphy's magazine publishing problem where:

- A company hierarchy is represented as a tree with employees
- Each employee has a rating that can be updated
- Performance is calculated as the GCD (Greatest Common Divisor) of all employees in a subtree
- Supports two types of queries:
  - **Type 0**: Update ratings of an employee and all subordinates
  - **Type 1**: Query the performance (GCD) of an employee's subtree

## Problem Statement

Murphy is the chairman of a magazine that publishes performance statistics of employees. For each company, they maintain a hierarchy in the form of a tree with:
- Head of company (H) at the root
- N total employees including the head
- Each employee has a rating
- Performance = GCD of all employees working under someone

The system supports:
1. **Type 0 Query**: `0 U Y` - Update employee U and all subordinates by value Y
2. **Type 1 Query**: `1 V` - Get performance (GCD) of employee V

## GUI Interface

### Accessing the Feature

1. Launch the application: `./SuffixTreeGUI`
2. Click the **"Employee Rating System"** button (purple color) on the main menu

### Using the GUI

#### Step 1: Initialize Company
- Enter the number of employees (1-26, including head)
- Employees are named: H (head), then A, B, C, etc.
- Click **"Initialize"** button

#### Step 2: Build Hierarchy & Set Ratings

**Add Subordinates:**
- Enter Manager (e.g., H)
- Enter Subordinate (e.g., A)
- Click **"Add Subordinate"**
- Repeat for all reporting relationships

**Set Initial Ratings:**
- Enter Employee (e.g., H)
- Enter Rating value (1-100)
- Click **"Set Rating"**
- Repeat for all employees

#### Step 3: Build Hierarchy Tree
- Click **"Step 3: Build Hierarchy Tree"** button
- This finalizes the hierarchy and enables query operations

#### Step 4: Perform Operations

**Type 0 - Update Subtree:**
- Enter Employee to update
- Enter value to add/subtract
- Click **"Update Subtree"**
- Updates the employee and all subordinates

**Type 1 - Query Performance:**
- Enter Employee to query
- Click **"Query Performance"**
- Shows the GCD of all ratings in that subtree

### Example Workflow

```
1. Initialize: 5 employees (H, A, B, C, D)

2. Build hierarchy:
   H -> A
   H -> B
   A -> C
   A -> D

3. Set ratings:
   H: 12
   A: 18
   B: 24
   C: 6
   D: 30

4. Build tree

5. Query performance of H:
   Result: 6 (GCD of 12, 18, 24, 6, 30)

6. Update A's subtree by +6:
   A: 18 -> 24
   C: 6 -> 12
   D: 30 -> 36

7. Query performance of H again:
   Result: 12 (GCD of 12, 24, 24, 12, 36)
```

## CLI Test Program

A standalone test program is provided: `test_employee_rating`

### Building
```bash
g++ -std=c++11 -o test_employee_rating test_employee_rating.cpp employeerating.cpp SuffixTree.cpp
```

### Running
```bash
./test_employee_rating
```

The program includes:
- **Example 1**: Simple company hierarchy demonstration
- **Example 2**: Magazine publishing scenario with bonuses
- **Interactive Mode**: Create and query your own hierarchy

### Interactive Mode Usage

```
1. Enter number of employees
2. Add subordinate relationships
3. Set initial ratings
4. Perform Type 0 and Type 1 queries
```

## Implementation Details

### Files
- `employeerating.h` - Header file with class definition
- `employeerating.cpp` - Implementation of employee rating logic
- `EmployeeRatingWindow.h` - Qt GUI window header
- `EmployeeRatingWindow.cpp` - Qt GUI window implementation
- `test_employee_rating.cpp` - Standalone CLI test program

### Data Structures
- **Tree representation**: Adjacency list for hierarchy
- **DFS linearization**: Flattens tree for efficient queries
- **Time stamps**: Entry/exit times for subtree identification
- **Suffix tree**: Used for advanced pattern matching (optional)

### Algorithms
- **GCD calculation**: Euclidean algorithm
- **Subtree updates**: O(N) traversal using DFS times
- **Performance queries**: O(N) GCD computation over subtree
- **DFS traversal**: O(N) for tree linearization

## Visual Design

The Employee Rating Window features:
- **Purple theme** (#9B59B6) matching the main menu button
- **Step-by-step workflow** with clear sections
- **Color-coded buttons**:
  - Initialize: Purple
  - Add Subordinate: Blue
  - Set Rating: Green
  - Build Tree: Orange
  - Update: Orange
  - Query: Teal
  - Back: Gray
- **Dark result display** for better readability
- **Detailed instructions** at the top

## Examples

### Example 1: Tech Company

```
Hierarchy:
       H (CEO - Rating: 100)
      / \
     A   B (VPs - Ratings: 80, 60)
    / \   \
   C   D   E (Managers - Ratings: 40, 20, 30)

Initial Performance:
- CEO H: GCD(100, 80, 60, 40, 20, 30) = 10
- VP A: GCD(80, 40, 20) = 20
- VP B: GCD(60, 30) = 30

After giving B's division +10 bonus:
- VP B: GCD(70, 40) = 10

After giving A's division -10 adjustment:
- VP A: GCD(70, 30, 10) = 10
- CEO H: GCD(100, 70, 70, 30, 10, 40) = 10
```

### Example 2: Small Startup

```
Hierarchy:
     H (Founder - 12)
    / \
   A   B (12, 18)
  / \
 C   D (6, 18)

Performance:
- H: GCD(12, 12, 18, 6, 18) = 6
- A: GCD(12, 6, 18) = 6
- B: GCD(18) = 18
```

## Mathematics Behind GCD

The Greatest Common Divisor (GCD) measures the largest common factor:
- GCD(12, 18) = 6 (both divisible by 6)
- GCD(10, 15, 20) = 5 (all divisible by 5)
- GCD(7, 11) = 1 (coprime numbers)

In the context of employee ratings:
- Higher GCD = more consistent performance across team
- Lower GCD = more variation in ratings
- Useful for evaluating team cohesion and alignment

## Keyboard Shortcuts (GUI)

- **Tab**: Navigate between fields
- **Enter**: Submit form (when applicable)
- **Esc**: Close window (back to main menu)

## Tips for Best Experience

1. **Plan your hierarchy** before entering it
2. **Use consistent rating scales** (e.g., multiples of 5 or 10)
3. **Start with simple hierarchies** to understand the GCD behavior
4. **Experiment with updates** to see how performance changes
5. **Try the test program** first to understand the concepts

## Error Handling

The GUI validates:
- Employee identifiers (must be H or A-Z within range)
- Rating values (positive integers)
- Query execution order (must initialize → build → query)
- Valid subordinate relationships

## Future Enhancements

Potential improvements:
- Visual tree diagram in GUI
- Export hierarchy to file
- Batch query processing
- Performance history tracking
- Comparison between time periods
- Custom employee naming schemes

## Technical Notes

- **Thread safety**: Not thread-safe (single-threaded Qt GUI)
- **Memory management**: Automatic cleanup in destructors
- **Maximum employees**: 26 (limited by single-character naming)
- **Rating range**: Integer values (typically 1-100)
- **Performance**: O(N) per query where N = number of employees

## References

- Problem inspired by competitive programming scenarios
- GCD algorithm: Euclidean method
- Tree traversal: Depth-First Search (DFS)
- Subtree queries: DFS linearization technique

## Support

For issues or questions:
1. Check the GUI instructions (top of window)
2. Try the CLI test program for debugging
3. Review the example workflows
4. Examine the test_employee_rating.cpp source code

---

**Feature Status**: ✅ Complete and Tested
**GUI Integration**: ✅ Fully integrated into main menu
**Documentation**: ✅ Comprehensive guide available
**Test Coverage**: ✅ CLI test program with examples
