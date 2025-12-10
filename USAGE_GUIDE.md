# Usage Guide - Suffix Tree Animation

## Quick Start Guide

### 1. Installation

First, install the required dependencies:

```bash
# On Ubuntu/Debian
sudo apt-get update
sudo apt-get install libsfml-dev

# On macOS
brew install sfml
```

### 2. Build the Project

```bash
make
```

This creates two executables:
- `SuffixTreeAnimation` - The GUI visualization
- `test_suffixtree` - The test suite

### 3. Run the GUI Application

```bash
./SuffixTreeAnimation
```

When prompted, enter a text string (e.g., "banana", "mississippi", "abcab").

**Note:** The input is limited to 15 characters for optimal visualization.

### 4. Run Tests (No GUI Required)

```bash
make test
```

This verifies the suffix tree implementation works correctly without needing a graphical display.

## Understanding the GUI

### Window Layout

```
┌─────────────────────────────────────────────────────────┐
│ Suffix Tree Animation - Ukkonen's Algorithm  Step: 3/7 │
│ Input: banana                                  [PAUSED] │
├─────────────────────────────────────────────────────────┤
│                                                         │
│                        (root)                          │
│                       /  |  \                          │
│                      /   |   \                         │
│                     /    |    \                        │
│                [b]     [a]    [na]                     │
│               /         |        \                     │
│            (leaf)    (node)    (leaf)                  │
│                        |                               │
│                      [na]                              │
│                        |                               │
│                     (leaf)                             │
│                                                         │
│ Active Point: root, edge=n, len=1                     │
│                                                         │
│ Controls:                                               │
│ SPACE - Play/Pause                                     │
│ RIGHT - Next Step                                      │
│ LEFT - Previous Step                                   │
│ R - Reset                                              │
│ ESC - Exit                                             │
└─────────────────────────────────────────────────────────┘
```

### Visual Elements

**Nodes:**
- **Blue circles**: Regular nodes in the tree
- **Red circles**: Active node (currently being processed)
- **Numbers inside**: Suffix index for leaf nodes

**Edges:**
- **Black lines**: Connections between nodes
- **Labels**: Show the substring associated with each edge
- Long edges are truncated with "..." for readability

**UI Elements:**
- **Title bar**: Shows the algorithm name and current step
- **Input display**: Shows the original text being processed
- **Status**: Shows whether animation is playing or paused
- **Active Point Info**: Shows the current state of the algorithm
- **Controls**: Keyboard shortcuts for navigation

## Step-by-Step Example: "banana"

### Step 0: Process 'b'
- Creates a single edge from root to a leaf
- Edge labeled 'b$...'

### Step 1: Process 'a'
- Creates a second edge from root
- Edge labeled 'a$...'

### Step 2: Process 'n'
- Creates a third edge from root
- Edge labeled 'na...'

### Step 3-5: Process 'ana'
- Active length increases
- Tree waits for rule 3 match or split

### Step 6: Process '$'
- Final character triggers implicit suffix splits
- Tree completes with all suffixes represented

## Animation Controls

### Keyboard Shortcuts

| Key | Action | Description |
|-----|--------|-------------|
| `SPACE` | Play/Pause | Toggle automatic animation |
| `→` (Right) | Next Step | Manually advance one character |
| `←` (Left) | Previous Step | Go back one character (rebuilds tree) |
| `R` | Reset | Return to beginning |
| `ESC` | Exit | Close the application |

### Animation Modes

**Paused Mode (Default):**
- Animation starts in paused mode
- Use arrow keys to step through manually
- Great for understanding each step in detail

**Playing Mode:**
- Press SPACE to start automatic animation
- Each step advances every 0.5 seconds
- Press SPACE again to pause

## Understanding Ukkonen's Algorithm

### Key Concepts Visualized

**Active Point:**
- The red highlighted node shows where the algorithm is currently working
- Displayed at bottom: "Active Point: root, edge=a, len=2"

**Suffix Links:**
- Internal connections that help the algorithm work efficiently
- Not currently visualized but used behind the scenes

**Implicit Suffixes:**
- Some suffixes are represented implicitly (through active length)
- Become explicit when the '$' terminator is processed

### Algorithm Phases

1. **Extension Phase**: Add one character at a time
2. **Rule 1**: Leaf edges extend automatically (implicit)
3. **Rule 2**: Create new leaf or split edge when mismatch occurs
4. **Rule 3**: Character already exists, just increase active length

## Common Usage Scenarios

### Learning Mode
```bash
./SuffixTreeAnimation
# Input: banana
# Use RIGHT arrow to step through slowly
# Observe how each character affects the tree
```

### Demo Mode
```bash
./SuffixTreeAnimation
# Input: mississippi
# Press SPACE to watch automatic animation
```

### Testing Without GUI
```bash
make test
# Runs automated tests
# Verifies correctness of implementation
```

## Troubleshooting

### Problem: Window doesn't open
**Solution:** 
- Ensure SFML is properly installed
- Check your system has graphics drivers
- Try running `make check-sfml`

### Problem: Font warnings
**Cause:** System fonts not found in default locations
**Impact:** Text still renders, might look different
**Solution:** Update font path in SuffixTreeGUI.cpp line ~56

### Problem: Display too small
**Cause:** Many nodes in deep tree
**Workaround:** Use shorter input strings (< 10 characters)

### Problem: Can't run on remote server
**Cause:** No X11 display available
**Solution:** Use `make test` instead to verify functionality

## Advanced Usage

### Testing Different Inputs

```bash
# Simple patterns
./SuffixTreeAnimation  # Input: abc
./SuffixTreeAnimation  # Input: aaa

# Repeating patterns
./SuffixTreeAnimation  # Input: abcabcabc

# Classic examples
./SuffixTreeAnimation  # Input: banana
./SuffixTreeAnimation  # Input: mississippi
```

### Understanding Node Count

The test output shows node counts:
```
Input: 'banana' -> Nodes: 11 (Expected suffixes: 7)
```

- **7 suffixes**: banana$, anana$, nana$, ana$, na$, a$, $
- **11 nodes**: Includes internal nodes created for branching
- More repeated patterns = more internal nodes

## Educational Value

This visualization helps understand:

1. **How suffix trees are constructed incrementally**
2. **Why Ukkonen's algorithm is O(n) linear time**
3. **The role of active point in efficient construction**
4. **Edge compression in suffix trees**
5. **Implicit suffix handling**

## Best Practices

- Start with simple inputs like "abc" or "aaa"
- Use manual stepping (arrow keys) when learning
- Try "banana" as the classic teaching example
- Use automatic mode for demonstrations
- Run tests to verify understanding

## Next Steps

After understanding the visualization:

1. Read the suffix tree theory
2. Study the code in `SuffixTree.cpp`
3. Try implementing pattern matching using the tree
4. Explore applications (DNA analysis, text search, etc.)
5. Modify the visualization code to show additional features

## References

- Original paper: Ukkonen (1995) "On-line construction of suffix trees"
- Interactive tutorial: https://visualgo.net/en/suffixtree
- Wikipedia: https://en.wikipedia.org/wiki/Suffix_tree
