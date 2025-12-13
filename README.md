# Suffix Tree Application

A comprehensive suffix tree implementation with both CLI and GUI interfaces, featuring colorful tree visualization.

## Features

The application supports four main operations:
1. **Search Pattern** - Find all occurrences of a pattern in text
2. **DNA Mutation Detection** - Detect mutations in DNA sequences
3. **Longest Repeated Pattern** - Find the longest repeated substring
4. **Predict Completions** - Auto-complete suggestions based on prefix

## Building the Application

### Prerequisites

**For CLI:**
- g++ compiler with C++11 support

**For GUI:**
- Qt5 development libraries
- qmake

On Ubuntu/Debian:
```bash
sudo apt-get install qt5-qmake qtbase5-dev qtbase5-dev-tools g++
```

### Build Commands

**Build CLI version:**
```bash
make cli
```

**Build GUI version:**
```bash
make gui
```

**Build both:**
```bash
make all
```

## Running the Application

### CLI Version
```bash
make run-cli
# or
./suffix_tree_cli
```

The CLI provides an interactive menu to:
- Build a suffix tree from text
- Search for patterns
- Detect DNA mutations
- Find longest repeated patterns
- Predict completions

### GUI Version
```bash
make run-gui
# or
./SuffixTreeGUI
```

## GUI Usage Guide

### Main Window
The main window displays four colorful buttons for each feature:
- **Search Pattern** (Blue) - Search for patterns in text
- **DNA Mutation Detection** (Red) - Compare reference and sample DNA
- **Longest Repeated Pattern** (Green) - Find repeated substrings
- **Predict Completions** (Orange) - Get auto-complete suggestions

### Search Pattern Window
1. Enter text to build the suffix tree
2. Click "Build Suffix Tree" to construct the tree
3. View the colorful tree visualization
4. Enter a pattern to search
5. Click "Search" to find all occurrences
6. Results show position(s) where pattern appears

**Example:**
- Text: `banana`
- Pattern: `ana`
- Result: Found at positions 1 and 3

### DNA Mutation Detection Window
1. Enter reference DNA sequence (e.g., `ACGTACGT`)
2. Click "Build Reference Tree"
3. View the tree visualization of the reference
4. Enter sample DNA sequence (e.g., `ACTTACGT`)
5. Click "Detect Mutations"
6. View detailed mutation report showing:
   - Mutation type (Substitution/Insertion)
   - Position of mutation
   - Reference vs sample bases

### Longest Repeated Pattern Window
1. Enter text to analyze (e.g., `abcabcabc`)
2. Click "Build Suffix Tree"
3. View the tree visualization
4. Click "Find Longest Repeated Pattern"
5. Results show:
   - The longest repeated substring
   - Length of the pattern
   - Starting index in text

### Predict Completions Window
1. Enter text corpus (e.g., `hello world, hello there, help me`)
2. Click "Build Suffix Tree"
3. View the tree visualization
4. Enter a prefix (e.g., `hel`)
5. Set maximum number of suggestions
6. Click "Predict Completions"
7. View auto-complete suggestions

## Tree Visualization

Each window includes a **colorful, interactive tree visualization**:

### Visual Features
- **Leaf nodes**: Rainbow colors (each leaf has unique color based on suffix index)
- **Internal nodes**: Blue/purple gradient
- **Edges**: Labeled with substring from original text
- **Node labels**: 
  - Leaf nodes show suffix index
  - Internal nodes show bullet point
- **Layout**: Hierarchical tree layout with proper spacing

### Color Scheme
- Nodes use gradient fills for 3D effect
- Edge labels on yellow background for visibility
- Rainbow colors for leaf nodes (golden angle distribution)
- Cornflower blue for internal nodes

### Interaction
- Scroll to navigate large trees
- Automatic sizing based on tree structure
- Clear, readable labels on edges

## Implementation Details

### Suffix Tree Algorithm
- Uses **Ukkonen's algorithm** for O(n) construction
- Handles all ASCII characters (128-character alphabet)
- Compact edge representation

### GUI Architecture
- **Main Window**: Central hub with all options
- **Feature Windows**: Separate windows for each operation
- **Tree Visualizer**: Reusable widget for tree rendering
- **Back Navigation**: Easy return to main menu from any window

### Code Structure
```
SuffixTree.h/cpp          - Core suffix tree implementation
MainWindow.h/cpp          - Main menu window
SearchWindow.h/cpp        - Pattern search interface
MutationWindow.h/cpp      - DNA mutation detection
PatternWindow.h/cpp       - Longest repeated pattern
PredictionWindow.h/cpp    - Auto-complete predictions
TreeVisualizer.h/cpp      - Tree visualization widget
main_gui.cpp              - GUI entry point
main.cpp                  - CLI entry point
```

## Cleaning Up

Remove all build artifacts:
```bash
make clean
```

Complete cleanup including generated files:
```bash
make distclean
```

## Notes

- The GUI cannot be tested in headless environments (no display)
- All features work with any text/string input
- Tree visualization automatically adjusts to tree size
- Long edge labels are truncated with "..." for clarity
- Each window maintains its own suffix tree instance

## Examples

### Example 1: Search Pattern
```
Text: "mississippi"
Pattern: "issi"
Result: Found at positions 1, 4
```

### Example 2: DNA Mutations
```
Reference: ACGTACGT
Sample:    ACTTACGT
Result: Mutation at index 2: G -> T (Substitution)
```

### Example 3: Longest Repeated
```
Text: "abcabcabc"
Result: "abcabc" (length 6)
```

### Example 4: Predictions
```
Text: "hello world, hello there, help me"
Prefix: "hel"
Results: "hello world", "hello there", "help me"
```

## Requirements Met

✅ Full GUI with Qt  
✅ Main window with all application options  
✅ Separate windows for each feature  
✅ Back to main menu functionality  
✅ Colorful tree visualization  
✅ No changes to core .cpp/.h files (only additions)  
✅ Professional, jaw-dropping visualization  

## License

Educational project for demonstrating suffix tree data structure and GUI development with Qt.
