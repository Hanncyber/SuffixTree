# Sidebar Navigation UI - Design Documentation

## Overview
Complete UI redesign with collapsible sidebar navigation system. All feature windows are now embedded within the main window for a seamless single-window experience.

## Layout Structure

### Full View (Sidebar Open)
```
┌──────────────────────────────────────────────────────────────────────────────┐
│  [◀]                                                                         │
│ ┌────────────┬─────────────────────────────────────────────────────────────┐│
│ │            │                                                             ││
│ │   Home     │                    CONTENT AREA                             ││
│ │            │                                                             ││
│ │  Search    │          ┌───────────────────────┐                         ││
│ │  Pattern   │          │  Suffix Tree Image    │                         ││
│ │            │          │      (200×200)        │                         ││
│ │  Longest   │          └───────────────────────┘                         ││
│ │  Repeated  │                                                             ││
│ │            │    Welcome to Suffix Tree Applications                     ││
│ │  Predict   │                                                             ││
│ │Completions │    ┌────────────────────────────────────┐                  ││
│ │            │    │ What is a Suffix Tree?             │                  ││
│ │  Employee  │    │                                    │                  ││
│ │  Rating    │    │ Explanation text with HTML...      │                  ││
│ │            │    │                                    │                  ││
│ │            │    │ • Fast Pattern Matching            │                  ││
│ │            │    │ • Longest Repeated Substring       │                  ││
│ │            │    │ • Auto-completion                  │                  ││
│ │            │    │ • DNA Analysis                     │                  ││
│ │            │    │                                    │                  ││
│ │            │    └────────────────────────────────────┘                  ││
│ │   [Exit]   │                                                             ││
│ │            │                                                             ││
│ └────────────┴─────────────────────────────────────────────────────────────┘│
└──────────────────────────────────────────────────────────────────────────────┘
    220px          980px
```

### Collapsed View (Sidebar Hidden)
```
┌──────────────────────────────────────────────────────────────────────────────┐
│ [▶]                                                                          │
│ ┌────────────────────────────────────────────────────────────────────────────┐
│ │                                                                            │
│ │                          FULL SCREEN CONTENT                               │
│ │                                                                            │
│ │                      ┌───────────────────────┐                            │
│ │                      │  Suffix Tree Image    │                            │
│ │                      │      (200×200)        │                            │
│ │                      └───────────────────────┘                            │
│ │                                                                            │
│ │              Welcome to Suffix Tree Applications                          │
│ │                                                                            │
│ │              ┌────────────────────────────────────┐                       │
│ │              │ What is a Suffix Tree?             │                       │
│ │              │                                    │                       │
│ │              │ Explanation text...                │                       │
│ │              │                                    │                       │
│ │              └────────────────────────────────────┘                       │
│ │                                                                            │
│ └────────────────────────────────────────────────────────────────────────────┘
└──────────────────────────────────────────────────────────────────────────────┘
                              1200px
```

## Component Specifications

### Window
- **Size:** 1200×700px
- **Background:** #1A1A1A (dark charcoal)
- **Title:** "Suffix Tree Applications"

### Toggle Button
- **Size:** 35×50px
- **Position:** Floating, moves with sidebar state
  - Open: (220, 10) - at edge of sidebar
  - Closed: (10, 10) - at left edge
- **Icons:** ◀ (hide) / ▶ (show)
- **Background:** #3A3A3A
- **Hover:** #4A4A4A
- **Always visible:** Yes

### Sidebar (220px wide)
**Background:** #2A2A2A (dark gray)
**Border:** 1px solid #3A3A3A on right edge
**Padding:** 10px all sides

#### Buttons:
1. **Home Button** (Top)
   - Text: "Home"
   - Shows welcome page with explanation

2. **Navigation Buttons** (Middle)
   - Search Pattern
   - Longest Repeated
   - Predict Completions
   - Employee Rating

3. **Exit Button** (Bottom)
   - Text: "Exit"
   - Red background (#B91C1C)
   - Hover: #DC2626

**Button Styling:**
```css
Background: #3A3A3A
Hover: #7C6DB0 (purple)
Pressed: #6B5AA6 (dark purple)
Border-radius: 8px
Height: 45px minimum
Padding: 12px
Font: 14px Arial
Color: #EDEDED
Text-align: left
```

### Content Area (QStackedWidget)

#### 1. Home View (Default)
**Components:**
- **Image Placeholder**
  - Size: 200×200px
  - Position: Top center
  - Background: #7C6DB0
  - Border: 3px solid #9F91D8
  - Border-radius: 15px
  - Text: "Suffix Tree\nImage"

- **Title**
  - Text: "Welcome to Suffix Tree Applications"
  - Font: Verdana, 24pt, Bold
  - Color: #EDEDED
  - Alignment: Center

- **Explanation Text** (QTextEdit)
  - Background: #2A2A2A
  - Border: 1px solid #3A3A3A
  - Border-radius: 10px
  - Padding: 20px
  - HTML formatted with:
    - Section headers in purple (#7C6DB0)
    - Bullet points
    - Bold text for emphasis
    - Explanations of suffix tree features and applications

**Content Includes:**
- What is a Suffix Tree?
- Key Features:
  - Fast Pattern Matching
  - Longest Repeated Substring
  - Auto-completion
  - DNA Analysis
- Applications in real-world scenarios

#### 2-5. Feature Windows
All feature windows (SearchWindow, PatternWindow, PredictionWindow, EmployeeRatingWindow) are embedded as child widgets in the QStackedWidget.

## Color Scheme

### Background Colors
- Main Window: #1A1A1A (dark charcoal)
- Sidebar: #2A2A2A (dark gray)
- Buttons: #3A3A3A (medium gray)
- Text Areas: #2A2A2A (dark gray)

### Accent Colors
- Primary Purple: #7C6DB0
- Hover Purple: #8E7FD1
- Pressed Purple: #6B5AA6
- Border Purple: #9F91D8

### Functional Colors
- Exit Red: #B91C1C
- Exit Hover: #DC2626
- Exit Pressed: #991B1B

### Text Colors
- Primary Text: #EDEDED (light gray)
- White Text: #FFFFFF

## Interaction Flow

### Toggle Sidebar
1. User clicks toggle button (◀)
2. Sidebar becomes hidden
3. Toggle button moves to left edge (10, 10)
4. Icon changes to ▶
5. Content area expands to full width
6. Reverse process when clicked again

### Navigation
1. User clicks navigation button in sidebar
2. QStackedWidget switches to corresponding view
3. Previous view is hidden
4. New view displays immediately
5. No separate windows are opened

### Home View
1. Always shown on application start
2. Displays educational content about suffix trees
3. Can be returned to at any time via Home button

## Technical Details

### Qt Components
- **QMainWindow** - Main application window
- **QHBoxLayout** - Main layout (sidebar + content)
- **QVBoxLayout** - Sidebar layout (vertical stack of buttons)
- **QStackedWidget** - Content area manager
- **QPushButton** - All navigation and action buttons
- **QLabel** - Image placeholder and titles
- **QTextEdit** - Formatted explanation text
- **QWidget** - Container widgets

### Memory Management
- All windows created once during initialization
- Windows persist in memory and are reused
- No dynamic creation/deletion on navigation
- Proper parent-child relationships ensure cleanup

### Key Differences from Previous Design

**Removed:**
- Circular buttons with emojis
- Logo placeholder in header
- Separate window instances
- Grid layout for buttons
- Bottom-right exit button placement

**Added:**
- Collapsible sidebar navigation
- Embedded content windows
- Home view with explanation
- Toggle button for sidebar
- Simple rectangular buttons
- Single-window application design

## User Experience Benefits

1. **Single Window Interface:** All features accessible without multiple windows
2. **Persistent State:** No losing work when switching between features
3. **Space Efficient:** Collapsible sidebar maximizes content area
4. **Clear Navigation:** Sidebar always shows available features
5. **Educational:** Home view introduces users to suffix trees
6. **Consistent:** Same UI pattern throughout application

## Future Enhancements

Possible improvements:
- Smooth animations for sidebar collapse/expand
- Button highlighting to show active view
- Keyboard shortcuts for navigation
- Custom image upload for home view
- Resizable sidebar
- Theme customization options
