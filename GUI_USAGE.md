# Suffix Tree GUI - User Guide

## Overview

The Suffix Tree GUI provides an interactive graphical interface for exploring suffix trees built using Ukkonen's algorithm. The application is built with Qt 5 and provides a clean, intuitive way to construct suffix trees, search for patterns, and view detailed statistics.

## Interface Layout

The application window is divided into several sections:

### 1. Input Section (Top)
- **Text Input Field**: Enter the text for which you want to build a suffix tree
  - Default text: "banana$"
  - The text must end with a unique terminator character '$'
  - If you forget the '$', the application will prompt you to add it
- **Build Suffix Tree Button**: Click to construct the suffix tree from the input text

### 2. Search Section (Below Input)
- **Pattern Input Field**: Enter a pattern to search for in the tree
- **Search Button**: Execute the search operation
- **Clear Button**: Reset all fields and clear the visualization

### 3. Tree Visualization Area (Left Panel)
- Large graphics view displaying the suffix tree graph
- Visual representation with nodes and edges
- Scrollable and draggable for easy navigation
- Shows edge labels with substrings and character ranges
- Leaf nodes display suffix indices
- Hierarchical layout with automatic spacing

### 4. Statistics Panel (Top Right)
Displays comprehensive tree statistics including:
- Input text and length
- Total unique substrings count
- Longest repeated substring
- Shortest unique substring
- Build confirmation message

### 5. Search Results Panel (Bottom Right)
Shows detailed search results:
- Whether the pattern was found
- Number of occurrences
- List of positions where the pattern appears
- Context snippets showing text around each match
- Autocomplete suggestions for the search pattern

## How to Use

### Building a Suffix Tree

1. **Enter Text**:
   ```
   Type your text in the "Text" input field
   Example: banana$
   ```

2. **Build Tree**:
   ```
   Click "Build Suffix Tree" button
   ```

3. **View Statistics**:
   ```
   The Statistics panel will display:
   - Text: "banana$"
   - Length: 7
   - Unique Substrings: 22
   - Longest Repeated Substring: "ana"
   - Shortest Unique Substring: "b"
   ```

### Searching for Patterns

1. **Enter Pattern**:
   ```
   Type the pattern you want to find
   Example: ana
   ```

2. **Execute Search**:
   ```
   Click "Search" button
   ```

3. **View Results**:
   ```
   The Results panel will show:
   ✓ Pattern found!
   Occurrences: 2
   
   Positions:
     - Index 1
     - Index 3
   
   Context:
   [contextual text snippets]
   
   Possible completions:
     - ana
     - ana$
   ```

### Advanced Features

**Autocomplete**:
- When you search for a pattern, the application suggests possible completions
- Useful for exploring what extensions of your pattern exist in the text

**Context Display**:
- See the text surrounding each match
- Helps understand where patterns occur in the original text

**Statistics**:
- Real-time calculation of unique substrings
- Identification of longest repeated substrings
- Discovery of shortest unique substrings

## Example Workflows

### Example 1: Analyzing "banana$"

1. Input: `banana$`
2. Build tree
3. Search for: `ana`
4. Results:
   - Found at positions 1 and 3
   - Longest repeated substring: "ana"
   - Total unique substrings: 22

### Example 2: Exploring "abcabxabcd$"

1. Input: `abcabxabcd$`
2. Build tree
3. Search for: `abc`
4. Results:
   - Found at positions 0 and 6
   - Longest repeated substring: "abc"
   - Total unique substrings: 57

### Example 3: Pattern Not Found

1. Input: `banana$`
2. Build tree
3. Search for: `xyz`
4. Results:
   - ✗ Pattern not found in the text.

## Tips and Best Practices

1. **Always use a terminator**: End your text with '$' to ensure proper tree construction
2. **Start simple**: Try short texts like "banana$" before moving to longer texts
3. **Explore patterns**: Use the search feature to understand how patterns are stored
4. **Check statistics**: Review the statistics to understand tree complexity
5. **Use autocomplete**: Let the suggestions guide you to valid patterns

## Technical Details

**Built with**:
- Qt 5.15+ (Qt Widgets framework)
- C++17 standard
- Ukkonen's algorithm for O(n) construction

**Features**:
- Real-time tree construction
- Pattern search with O(m + k) complexity
- Comprehensive statistics display
- User-friendly error handling
- Responsive interface design

## Keyboard Shortcuts

- **Enter** in text field: Build tree
- **Enter** in search field: Execute search
- **Ctrl+C**: Copy text from results panels
- **Mouse drag**: Navigate tree visualization

## Troubleshooting

**Problem**: Application won't build
- **Solution**: Ensure Qt 5 development packages are installed
  ```bash
  sudo apt-get install qt5-qmake qtbase5-dev
  ```

**Problem**: "Please build a suffix tree first"
- **Solution**: Enter text and click "Build Suffix Tree" before searching

**Problem**: Text must end with '$'
- **Solution**: Add '$' at the end of your input text, or accept the automatic addition

**Problem**: Empty search results
- **Solution**: Ensure the pattern you're searching for actually exists in the text

## Future Enhancements

Potential improvements for future versions:
- Enhanced tree layout algorithms for larger trees
- Export tree structure to DOT format
- Load text from files
- Multiple text comparison
- Animation of tree construction
- Save/load tree sessions
- Theme customization

## Support

For issues, questions, or contributions, please refer to the main README.md file or the project repository.
