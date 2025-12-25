# Visual Mockup: Updated Google Text Prediction Interface

## Updated Interface

```
╔════════════════════════════════════════════════════════════════════════════╗
║            Google Text Prediction - Minimum Prefix Finder                  ║
║                                                                            ║
╠════════════════════════════════════════════════════════════════════════════╣
║                                                                            ║
║  ┌────────────────────────────────────────────────┐  ┌─────────────────┐  ║
║  │ Enter paragraph or text...                     │  │ Build Tree      │  ║
║  └────────────────────────────────────────────────┘  └─────────────────┘  ║
║                                                                            ║
║  ┌────────────────────────────────────────────────┐  ┌─────────────────┐  ║
║  │ Enter query string (must be substring of       │  │ Predict         │  ║
║  │ text)...                                       │  │                 │  ║
║  └────────────────────────────────────────────────┘  └─────────────────┘  ║
║                                                                            ║
║  Maximum Predictions (X): [5 ▼]                                           ║
║  ⓘ Predictions are shown when distinct substrings with matching           ║
║    prefix ≤ X                                                             ║
║                                                                            ║
╠════════════════════════════════════════════════════════════════════════════╣
║ Results:                                                                   ║
║ ┌────────────────────────────────────────────────────────────────────────┐ ║
║ │ Query string: "ABC"                                                    │ ║
║ │ Prefix "A" → 3 predictions                                             │ ║
║ │ Prefix "AB" → 3 predictions                                            │ ║
║ │ Prefix "ABC" → 2 predictions                                           │ ║
║ │ ----------------------------------                                     │ ║
║ │ Minimum prefix index = 3                                               │ ║
║ │                                                                        │ ║
║ └────────────────────────────────────────────────────────────────────────┘ ║
╠════════════════════════════════════════════════════════════════════════════╣
║                                                                            ║
║ ┌────────────────────────────────────────────────────────────────────────┐ ║
║ │                                                                        │ ║
║ │                       Tree Visualization Area                         │ ║
║ │                                                                        │ ║
║ │                     [Suffix Tree Graph Display]                       │ ║
║ │                                                                        │ ║
║ │                                                                        │ ║
║ └────────────────────────────────────────────────────────────────────────┘ ║
║                                                                            ║
╠════════════════════════════════════════════════════════════════════════════╣
║                                              ┌──────────────────────────┐  ║
║                                              │ Back to Main Menu        │  ║
║                                              └──────────────────────────┘  ║
╚════════════════════════════════════════════════════════════════════════════╝
```

## Key Changes from Previous Version

### ❌ Removed Components:
- **"Max Suggestions" spinbox** - Not needed for new algorithm

### ✏️ Modified Components:

#### 1. Window Title
- **Before**: "Predict Word Completions"
- **After**: "Google Text Prediction - Minimum Prefix Finder"

#### 2. Input Field
- **Before**: "Enter prefix to predict..."
- **After**: "Enter query string (must be substring of text)..."

#### 3. Parameter Control
- **Before**: "Occurrence Threshold: [100]"
  - Tooltip: "Occurrence threshold - prefixes appearing more than this number of times will be filtered out"
- **After**: "Maximum Predictions (X): [5]"
  - Tooltip: "Predictions are shown when distinct substrings with matching prefix ≤ X"

#### 4. Output Format
- **Before**: Lists of word suggestions
- **After**: Step-by-step prefix analysis with minimum index result

### Example Output Scenarios

#### Scenario 1: Success Case
```
Text: "GOOGLEGOOGLE"
Query: "GOO"
X: 2

Output:
Query string: "GOO"
Prefix "G" → 4 predictions
Prefix "GO" → 2 predictions
Prefix "GOO" → 2 predictions
----------------------------------
Minimum prefix index = 2
```

#### Scenario 2: Not Found Case
```
Text: "ABCABC"
Query: "ABC"
X: 1

Output:
Query string: "ABC"
Prefix "A" → 2 predictions
Prefix "AB" → 2 predictions
Prefix "ABC" → 2 predictions
----------------------------------
Minimum prefix index = -1
```
(Returns -1 because even the full query has 2 predictions, which is > 1)

#### Scenario 3: Immediate Match
```
Text: "ABCDEFGH"
Query: "A"
X: 5

Output:
Query string: "A"
Prefix "A" → 1 predictions
----------------------------------
Minimum prefix index = 1
```

## Problem Explanation

The Google Text Prediction problem asks:

> **When typing a search query character by character, at which character position do the search results narrow down to ≤ X possibilities?**

### How It Works:
1. Text is analyzed to build a suffix tree
2. User enters a query string (must exist in the text)
3. Algorithm checks each prefix (1st char, 1st+2nd chars, etc.)
4. For each prefix, counts how many distinct substrings in the text start with that prefix
5. Returns the length of the shortest prefix where count ≤ X
6. Returns -1 if even the full query has too many matches

### Real-World Application:
This simulates how search engines like Google decide when to show autocomplete suggestions. If there are too many possible matches, the system waits for more characters before offering predictions.

## UI Flow

1. **Enter Text** → Click "Build Tree"
   - System builds suffix tree data structure
   - Controls become enabled

2. **Set Parameter**
   - Adjust "Maximum Predictions (X)" (default: 5)
   - This controls how many predictions are "too many"

3. **Enter Query** → Click "Predict"
   - Query must be a substring of the original text
   - System analyzes each prefix incrementally
   - Shows prediction count for each prefix
   - Displays minimum prefix index (or -1)

## Technical Notes

- Uses 1-based indexing (as per problem specification)
- Query string must be guaranteed to exist in text
- Algorithm iterates through all prefixes sequentially
- Counts distinct substrings using suffix tree leaf nodes
- Default X=5 provides good balance for most use cases
