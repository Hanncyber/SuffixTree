# Suffix Tree Animation

An interactive C++ GUI application that visualizes the step-by-step construction of a suffix tree using Ukkonen's algorithm.

## Overview

This project implements a suffix tree data structure with an animated visualization that shows how the tree is built character by character. The visualization helps understand the complex Ukkonen's algorithm through an interactive graphical interface.

## Features

- **Step-by-step Animation**: Watch the suffix tree being built one character at a time
- **Interactive Controls**: Play, pause, step forward/backward through the construction
- **Visual Feedback**: Active nodes are highlighted during construction
- **Edge Labels**: Shows the substrings associated with each edge
- **Active Point Display**: Real-time display of the algorithm's active point
- **User Input**: Build suffix trees for any text string

## Files

- `SuffixTree.hpp` - Suffix tree class definition
- `SuffixTree.cpp` - Suffix tree implementation (Ukkonen's algorithm)
- `SuffixTreeGUI.cpp` - GUI visualization and animation
- `Makefile` - Build configuration

## Prerequisites

### SFML Library

The GUI uses SFML (Simple and Fast Multimedia Library) for graphics and window management.

**Installation:**

- **Ubuntu/Debian:**
  ```bash
  sudo apt-get update
  sudo apt-get install libsfml-dev
  ```

- **macOS:**
  ```bash
  brew install sfml
  ```

- **Windows:**
  Download from [SFML Downloads](https://www.sfml-dev.org/download.php) and follow the installation guide.

### C++ Compiler

You need a C++11 compatible compiler:
- GCC 4.8+ or Clang 3.3+ (Linux/macOS)
- MinGW or Visual Studio (Windows)

## Building

### Using Make

```bash
make
```

This will compile the application and create the `SuffixTreeAnimation` executable.

### Manual Compilation

If you don't have Make, you can compile manually:

```bash
g++ -std=c++11 -o SuffixTreeAnimation SuffixTreeGUI.cpp SuffixTree.cpp -lsfml-graphics -lsfml-window -lsfml-system
```

### Checking Dependencies

To verify SFML is installed correctly:

```bash
make check-sfml
```

## Running

### Quick Start

```bash
make run
```

Or run the executable directly:

```bash
./SuffixTreeAnimation
```

### Usage

1. When prompted, enter a text string (e.g., "banana")
2. The GUI window will open showing the suffix tree construction
3. Use the controls to navigate through the animation

## Controls

| Key | Action |
|-----|--------|
| `SPACE` | Play/Pause the animation |
| `RIGHT ARROW` | Step forward (next character) |
| `LEFT ARROW` | Step backward (previous character) |
| `R` | Reset to beginning |
| `ESC` | Exit application |

## How It Works

### Suffix Tree

A suffix tree is a compressed trie of all suffixes of a given text. It's a powerful data structure used in:
- String matching
- Finding longest repeated substrings
- Pattern matching
- Bioinformatics (DNA sequence analysis)

### Ukkonen's Algorithm

The implementation uses Ukkonen's algorithm, which constructs the suffix tree in O(n) time by:
1. Processing the text character by character
2. Maintaining an "active point" for efficient insertions
3. Using suffix links for quick navigation
4. Implicitly representing all suffixes

### Visualization

The GUI shows:
- **Nodes**: Circular nodes representing tree vertices
- **Edges**: Lines with labels showing substring ranges
- **Active Node**: Highlighted in red during construction
- **Active Point Info**: Shows the current state of the algorithm
- **Step Counter**: Current progress through the text

## Examples

### Example 1: "banana"
```bash
./SuffixTreeAnimation
# Enter: banana
```

This classic example demonstrates:
- Edge compression
- Suffix links
- Active point movement
- Internal node creation

### Example 2: "abcabxabcd"
```bash
./SuffixTreeAnimation
# Enter: abcabxabcd
```

Shows more complex patterns with repeated substrings.

## Troubleshooting

### SFML Not Found
```
error: sfml/Graphics.hpp: No such file or directory
```
**Solution**: Install SFML using the instructions in the Prerequisites section.

### Font Loading Warning
```
Warning: Could not load font. Text may not display correctly.
```
**Solution**: The application will still work, but text might not render. You can:
- Install standard fonts on your system
- Modify the font path in `SuffixTreeGUI.cpp` to point to an available font

### Window Doesn't Open
**Solution**: Ensure your system supports OpenGL and has proper graphics drivers installed.

## Limitations

- For visualization clarity, input is limited to 15 characters
- Very deep trees may not fit well on screen
- Font rendering depends on system fonts being available

## Technical Details

### Class Structure

**SuffixTree Class:**
- `Node`: Represents a node in the suffix tree
- `Edge`: Represents an edge with start/end indices
- `build()`: Constructs the complete tree
- `extend(int pos)`: Extends the tree with character at position

**SuffixTreeVisualizer Class:**
- `run()`: Main event loop
- `calculateNodePositions()`: Layouts nodes using BFS
- `drawEdges()`: Renders edges with labels
- `drawNodes()`: Renders nodes
- `drawUI()`: Renders user interface elements

### Visualization Algorithm

1. **Layout**: Uses breadth-first search to assign tree levels
2. **Positioning**: Distributes nodes evenly across window width
3. **Edge Labels**: Extracts and displays substring ranges
4. **Highlighting**: Tracks and highlights the active node

## Contributing

Contributions are welcome! Areas for enhancement:
- Better layout algorithm for large trees
- Zoom and pan functionality
- Export tree as image
- Animation speed control
- More detailed step explanations

## License

This project is provided as-is for educational purposes.

## References

- Ukkonen, E. (1995). "On-line construction of suffix trees"
- SFML Documentation: https://www.sfml-dev.org/documentation/
- Suffix Tree Tutorial: https://en.wikipedia.org/wiki/Suffix_tree

## Author

Created for educational visualization of suffix tree construction.
