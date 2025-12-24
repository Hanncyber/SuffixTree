# UI Mockup - Employee Rating Window with Tabs

## Main Window - Controls Tab (Default View)

```
╔═══════════════════════════════════════════════════════════════════════════════╗
║                                                                               ║
║              Employee Rating & Performance System                             ║
║                                                                               ║
╠═══════════════════════════════════════════════════════════════════════════════╣
║                                                                               ║
║  ┌──────────────────────────────────────────────────────────────────────┐   ║
║  │  [ Controls ]   Tree Visualization                                   │   ║
║  ├──────────────────────────────────────────────────────────────────────┤   ║
║  │                                                                       │   ║
║  │  This system manages employee hierarchy and ratings.                 │   ║
║  │  Step 1: Initialize company (H is root, then A, B, C, ...)          │   ║
║  │  Step 2: Add subordinates and set initial ratings                    │   ║
║  │  Step 3: Build the hierarchy                                         │   ║
║  │  Step 4: Perform updates (Type 0) or queries (Type 1)               │   ║
║  │                                                                       │   ║
║  │  ╔════════════════════════════════════════════════════════════════╗  │   ║
║  │  ║ Step 1: Initialize Company                                     ║  │   ║
║  │  ╠════════════════════════════════════════════════════════════════╣  │   ║
║  │  ║ Number of employees: [  5  ▼]  [ Initialize ]                 ║  │   ║
║  │  ╚════════════════════════════════════════════════════════════════╝  │   ║
║  │                                                                       │   ║
║  │  ╔════════════════════════════════════════════════════════════════╗  │   ║
║  │  ║ Step 2: Build Hierarchy & Set Ratings                         ║  │   ║
║  │  ╠════════════════════════════════════════════════════════════════╣  │   ║
║  │  ║ Manager: [    ] Subordinate: [    ] [ Add Subordinate ]       ║  │   ║
║  │  ║                                                                ║  │   ║
║  │  ║ Employee: [    ] Rating: [ 10 ▼] [ Set Rating ]              ║  │   ║
║  │  ╚════════════════════════════════════════════════════════════════╝  │   ║
║  │                                                                       │   ║
║  │  ┌────────────────────────────────────────────────────────────────┐  │   ║
║  │  │     [ Step 3: Build Hierarchy Tree ]                           │  │   ║
║  │  └────────────────────────────────────────────────────────────────┘  │   ║
║  │                                                                       │   ║
║  │  ╔════════════════════════════════════════════════════════════════╗  │   ║
║  │  ║ Step 4: Perform Operations                                     ║  │   ║
║  │  ╠════════════════════════════════════════════════════════════════╣  │   ║
║  │  ║ Type 0 - Update Subtree:                                       ║  │   ║
║  │  ║ Employee: [    ] Update by: [  5  ▼] [ Update Subtree ]      ║  │   ║
║  │  ║                                                                ║  │   ║
║  │  ║ Type 1 - Query Performance (GCD):                              ║  │   ║
║  │  ║ Employee: [    ] [ Query Performance ]                         ║  │   ║
║  │  ╚════════════════════════════════════════════════════════════════╝  │   ║
║  │                                                                       │   ║
║  │  ┌────────────────────────────────────────────────────────────────┐  │   ║
║  │  │ ✓ Company initialized with 5 employees.                        │  │   ║
║  │  │ Employee identifiers: H (head), then A, B, C, ...              │  │   ║
║  │  │ Now add subordinates and set initial ratings...                │  │   ║
║  │  │                                                                 │  │   ║
║  │  └────────────────────────────────────────────────────────────────┘  │   ║
║  │                                                                       │   ║
║  └──────────────────────────────────────────────────────────────────────┘   ║
║                                                                               ║
║  ┌──────────────────────────────────────────────────────────────────────┐   ║
║  │                      [ Back to Main Menu ]                           │   ║
║  └──────────────────────────────────────────────────────────────────────┘   ║
║                                                                               ║
╚═══════════════════════════════════════════════════════════════════════════════╝
```

## Main Window - Tree Visualization Tab

```
╔═══════════════════════════════════════════════════════════════════════════════╗
║                                                                               ║
║              Employee Rating & Performance System                             ║
║                                                                               ║
╠═══════════════════════════════════════════════════════════════════════════════╣
║                                                                               ║
║  ┌──────────────────────────────────────────────────────────────────────┐   ║
║  │  Controls   [ Tree Visualization ]                                   │   ║
║  ├──────────────────────────────────────────────────────────────────────┤   ║
║  │                                                                       │   ║
║  │        Employee Hierarchy Tree Visualization                          │   ║
║  │                                                                       │   ║
║  │  ┌────────────────────────────────────────────────────────────────┐  │   ║
║  │  │                                                                 │  │   ║
║  │  │                          ┌─────┐                               │  │   ║
║  │  │                          │  H  │                               │  │   ║
║  │  │                          │R: 12│                               │  │   ║
║  │  │                          └──┬──┘                               │  │   ║
║  │  │                   ┌─────────┴─────────┐                        │  │   ║
║  │  │                   │                   │                        │  │   ║
║  │  │               ┌───┴──┐            ┌───┴──┐                    │  │   ║
║  │  │               │  A   │            │  B   │                    │  │   ║
║  │  │               │R: 18 │            │R: 24 │                    │  │   ║
║  │  │               └───┬──┘            └──────┘                    │  │   ║
║  │  │           ┌───────┴────────┐                                  │  │   ║
║  │  │           │                │                                  │  │   ║
║  │  │       ┌───┴──┐         ┌───┴──┐                              │  │   ║
║  │  │       │  C   │         │  D   │                              │  │   ║
║  │  │       │R: 6  │         │R: 30 │                              │  │   ║
║  │  │       └──────┘         └──────┘                              │  │   ║
║  │  │                                                                 │  │   ║
║  │  │  Legend:                                                        │  │   ║
║  │  │  • Purple node = Root (H)                                       │  │   ║
║  │  │  • Blue nodes = Employees (A, B, C, D, ...)                   │  │   ║
║  │  │  • R: XX = Employee rating                                      │  │   ║
║  │  │                                                                 │  │   ║
║  │  │  [Scroll down/up to view large hierarchies]                    │  │   ║
║  │  │                                                                 │  │   ║
║  │  └────────────────────────────────────────────────────────────────┘  │   ║
║  │                                                                       │   ║
║  └──────────────────────────────────────────────────────────────────────┘   ║
║                                                                               ║
║  ┌──────────────────────────────────────────────────────────────────────┐   ║
║  │                      [ Back to Main Menu ]                           │   ║
║  └──────────────────────────────────────────────────────────────────────┘   ║
║                                                                               ║
╚═══════════════════════════════════════════════════════════════════════════════╝
```

## Key Differences

### Before (Single View)
- Controls and tree visualization competed for vertical space
- Tree visualization was limited to ~350px minimum height
- Result text area limited to 100px maximum height
- Users had to scroll the entire window to see both controls and tree
- Window size: 900x700

### After (Tabbed View)
- Controls and tree visualization have dedicated tabs
- Tree visualization can use the full tab area (much larger)
- Result text area can expand as needed (120px minimum)
- Users can switch tabs to focus on controls or visualization
- Window size: 1000x800 (more space overall)

## Tab Switching Behavior

1. **User works in Controls tab** (default):
   - Initializes company
   - Adds subordinates: H→A, H→B
   - Sets ratings
   - Builds hierarchy
   - Performs update: "Update A by +5"
   - Sees result: "✓ Type 0 Query: Updated ratings of A..."

2. **User switches to Tree Visualization tab**:
   - Clicks on "Tree Visualization" tab
   - Sees the complete tree with updated ratings
   - Can scroll to see large hierarchies
   - Tree has full vertical space (no controls competing)

3. **User switches back to Controls tab**:
   - Clicks on "Controls" tab
   - Performs another operation
   - Sees result immediately in text area
   - Can switch back to visualization to see changes

## Visual Improvements

### Tab Styling
- **Unselected tabs**: Light gray background (#ECF0F1), dark text
- **Selected tab**: Purple background (#9B59B6), white text
- **Hover effect**: Medium gray background (#BDC3C7)
- **Tab border**: 2px purple border around the tab content pane

### Space Utilization
- **Controls Tab**: All vertical space for inputs and results
- **Tree Visualization Tab**: All vertical space for tree display
- **Better aspect ratio**: Tree can grow horizontally and vertically

### User Experience
- **Clear separation**: Input vs. output
- **Focus mode**: One task at a time (controls OR visualization)
- **Familiar pattern**: Standard tabbed interface
- **Easy navigation**: Single click to switch views
