# Prediction Window GUI - Visual Mockup

## Updated Prediction Window Interface

```
╔════════════════════════════════════════════════════════════════════════════╗
║                      Word Prediction from Prefix                           ║
║                                                                            ║
╠════════════════════════════════════════════════════════════════════════════╣
║                                                                            ║
║  ┌────────────────────────────────────────────────┐  ┌─────────────────┐  ║
║  │ Enter paragraph or text...                     │  │ Build Tree      │  ║
║  └────────────────────────────────────────────────┘  └─────────────────┘  ║
║                                                                            ║
║  ┌────────────────────────────────────────────────┐  ┌─────────────────┐  ║
║  │ Enter prefix to predict...                     │  │ Predict         │  ║
║  └────────────────────────────────────────────────┘  └─────────────────┘  ║
║                                                                            ║
║  ┌────────────────────────────┐   ┌────────────────────────────────────┐  ║
║  │ Max Suggestions:   [10 ▼] │   │ Occurrence Threshold:  [100 ▼]    │  ║
║  └────────────────────────────┘   └────────────────────────────────────┘  ║
║  ⓘ Maximum number of word       ⓘ Prefixes appearing more than this    ║
║    suggestions to display          number of times will be filtered out  ║
║                                                                            ║
╠════════════════════════════════════════════════════════════════════════════╣
║ Results:                                                                   ║
║ ┌────────────────────────────────────────────────────────────────────────┐ ║
║ │ Prefix given "hel":                                                    │ ║
║ │   hello                                                                │ ║
║ │   help                                                                 │ ║
║ │   helmet                                                               │ ║
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

## Detailed Component Layout

### NEW Components (highlighted in the mockup):

#### 1. Max Suggestions SpinBox
```
┌────────────────────────────┐
│ Max Suggestions:   [10 ▼] │
└────────────────────────────┘
   Range: 1-50
   Default: 10
   Tooltip: "Maximum number of word suggestions to display"
```

#### 2. Occurrence Threshold SpinBox
```
┌────────────────────────────────────┐
│ Occurrence Threshold:  [100 ▼]    │
└────────────────────────────────────┘
   Range: 1-1000
   Default: 100
   Tooltip: "Occurrence threshold - prefixes appearing more than 
            this number of times will be filtered out"
```

## Color Scheme (as defined in code)

- **Title**: Orange (#F39C12)
- **Build Tree Button**: Orange (#F39C12), hover: darker orange (#E67E22)
- **Predict Button**: Green (#2ECC71), hover: darker green (#27AE60)
- **Results Area**: Dark blue background (#2C3E50), light text (#ECF0F1)
- **Back Button**: Gray (#95A5A6), hover: darker gray (#7F8C8D)
- **Disabled Controls**: Light gray (#BDC3C7)

## Interactive States

### Initial State (before tree is built):
- Text input: ✓ Enabled
- Build Tree button: ✓ Enabled
- Prefix input: ✗ Disabled
- Max Suggestions spinbox: ✗ Disabled
- Occurrence Threshold spinbox: ✗ Disabled
- Predict button: ✗ Disabled

### After Tree is Built:
- Text input: ✓ Enabled
- Build Tree button: ✓ Enabled
- Prefix input: ✓ Enabled
- Max Suggestions spinbox: ✓ Enabled
- Occurrence Threshold spinbox: ✓ Enabled
- Predict button: ✓ Enabled

## Example Usage Flow

```
1. User enters text: "hello world hello there hello everyone"
   └─> Clicks "Build Tree"
       └─> Results: "Suffix tree built successfully! Enter a prefix to predict words."

2. User enters prefix: "hel"
   User sets Max Suggestions: 5
   User sets Occurrence Threshold: 100
   └─> Clicks "Predict"
       └─> Results:
           Prefix given "hel":
             hello
             hello
             hello

3. User adjusts Occurrence Threshold: 2
   └─> Clicks "Predict"
       └─> Results:
           Prefix given "hel":
           No suggestions found.
           (because "hello" appears 3 times, which is > 2)

4. User adjusts Max Suggestions: 2
   User sets Occurrence Threshold: 100
   └─> Clicks "Predict"
       └─> Results:
           Prefix given "hel":
             hello
             hello
           (only shows first 2 even though more exist)
```

## Responsive Behavior

- **Tooltips**: Appear on hover over spinbox controls
- **Button Hover**: Color changes to provide visual feedback
- **Scroll Area**: Tree visualization area is scrollable for large trees
- **Window Size**: 900x700 pixels default, resizable

## Accessibility Features

- Clear labels for all controls
- Tooltips provide context-sensitive help
- Disabled state is visually distinct
- Monospace font for results (easier to read)
- Sufficient contrast ratios for all text
- Logical tab order through controls

## Technical Notes

The spinbox controls use Qt's QSpinBox widget with:
- Increment/decrement buttons
- Direct numeric input capability
- Range validation
- Tooltip support
- Enable/disable state management
