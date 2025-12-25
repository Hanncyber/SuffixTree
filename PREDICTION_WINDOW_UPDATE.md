# Prediction Window GUI Update

## Overview
The Prediction Window GUI has been updated to support the new functionality in the `predictCompletions` function, which now accepts a third parameter: `no` (occurrence threshold).

## Changes Made

### 1. New UI Controls
Two SpinBox controls have been added to the Prediction Window:

#### Max Suggestions SpinBox
- **Label**: "Max Suggestions:"
- **Range**: 1-50
- **Default Value**: 10
- **Tooltip**: "Maximum number of word suggestions to display"
- **Purpose**: Controls the `maxSuggestions` parameter in predictCompletions()

#### Occurrence Threshold SpinBox
- **Label**: "Occurrence Threshold:"
- **Range**: 1-1000
- **Default Value**: 100
- **Tooltip**: "If prefix appears more than this many times, no suggestions will be shown"
- **Purpose**: Controls the `no` parameter in predictCompletions()

### 2. Updated Function Call
The `predictCompletions()` slot now passes three arguments to the SuffixTree function:
```cpp
tree->predictCompletions(prefix.toStdString(), maxSuggestions, threshold);
```

Previously, it was hardcoded:
```cpp
tree->predictCompletions(prefix.toStdString(), 10);
```

### 3. Modified Files
- `PredictionWindow.h`: Added `QSpinBox *thresholdInput;` member variable
- `PredictionWindow.cpp`: 
  - Added UI setup for both spinbox controls
  - Enabled both controls when tree is built
  - Updated predictCompletions() to read and pass both values

## UI Layout
The new parameters section is positioned between the prefix input field and the results display area:

```
+------------------------------------------+
| Text Input Field        | Build Tree Btn |
+------------------------------------------+
| Prefix Input Field      | Predict Btn    |
+------------------------------------------+
| Max Suggestions: [10]  Occurrence       |
|                        Threshold: [100]  |
+------------------------------------------+
| Results Display Area                     |
|                                          |
+------------------------------------------+
```

## How the Threshold Works
The `no` parameter (Occurrence Threshold) controls how many times a prefix can appear in the text before the function decides it's too common to provide useful suggestions:

- If the prefix appears **more than** the threshold value, the function returns "No suggestions found."
- This helps filter out extremely common prefixes that would generate too many unhelpful suggestions
- Default value of 100 provides a good balance for most use cases
- Users can adjust this based on their text corpus size and needs

## Example Usage

1. Enter text: "hello world hello there hello everyone hello friend"
2. Build the suffix tree
3. Set Max Suggestions to 5
4. Set Occurrence Threshold to 2
5. Enter prefix: "hello"
6. Click Predict

Since "hello" appears 4 times (> 2), the result will be "No suggestions found."

If you increase the threshold to 5, suggestions will be shown.
