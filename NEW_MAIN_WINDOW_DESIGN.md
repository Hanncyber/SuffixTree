# New Main Window Design

## Layout Overview

The main window has been redesigned with the following structure:

### Header Section (Top)
```
┌────────────────────────────────────────────────────────────┐
│  ┌──────┐                                                   │
│  │ LOGO │  Suffix Tree apps                                │
│  └──────┘                                                   │
└────────────────────────────────────────────────────────────┘
```
- **Logo Placeholder**: 80x80 pixel square on the top left with purple background
  - Styled with rounded corners
  - Contains text "LOGO" (placeholder for an actual logo image)
- **Title**: "Suffix Tree apps" text beside the logo
  - Large, bold Verdana font (28pt)
  - Left-aligned to logo

### Main Content (Center)
```
┌────────────────────────────────────────────────────────────┐
│                                                              │
│         ┌─────────┐         ┌─────────┐                    │
│         │    🔍   │         │    🔄   │                    │
│         │ Search  │         │ Longest │                    │
│         │ Pattern │         │Repeated │                    │
│         └─────────┘         └─────────┘                    │
│                                                              │
│         ┌─────────┐         ┌─────────┐                    │
│         │    💡   │         │    👥   │                    │
│         │ Predict │         │Employee │                    │
│         │Completion│        │ Rating  │                    │
│         └─────────┘         └─────────┘                    │
│                                                              │
└────────────────────────────────────────────────────────────┘
```
- **4 Round Buttons** in a 2x2 grid layout:
  1. **Search Pattern** (🔍) - Top left
  2. **Longest Repeated** (🔄) - Top right  
  3. **Predict Completion** (💡) - Bottom left
  4. **Employee Rating** (👥) - Bottom right

- Each button:
  - 150x150 pixels
  - Circular shape (border-radius: 75px)
  - Purple background (#7C6DB0)
  - Contains an emoji icon
  - Multi-line text label
  - Hover effect (lighter purple)
  - Hand cursor on hover

### Footer Section (Bottom Right)
```
┌────────────────────────────────────────────────────────────┐
│                                           ┌──────────┐      │
│                                           │   Exit   │      │
│                                           └──────────┘      │
└────────────────────────────────────────────────────────────┘
```
- **Exit Button**: Bottom right corner
  - Red background (#B91C1C)
  - 120x45 pixels
  - Rounded corners
  - Positioned at bottom right

## Color Scheme

- **Background**: Dark (#1A1A1A)
- **Primary Buttons**: Purple (#7C6DB0)
- **Button Hover**: Light Purple (#8E7FD1)
- **Button Pressed**: Dark Purple (#6B5AA6)
- **Exit Button**: Red (#B91C1C)
- **Exit Hover**: Bright Red (#DC2626)
- **Text**: White (#FFFFFF) for buttons, Light gray (#EDEDED) for labels

## Window Size

- Width: 800 pixels
- Height: 600 pixels

## Key Features

1. ✅ Image placeholder for logo at top left
2. ✅ "Suffix Tree apps" text beside logo
3. ✅ 4 round buttons with icons in centered grid
4. ✅ Exit button at bottom right corner
5. ✅ Professional, modern dark theme
6. ✅ Responsive hover effects
7. ✅ Proper spacing and alignment

## Implementation Details

### Files Modified

1. **MainWindow.h**
   - Added QHBoxLayout and QGridLayout includes
   - Added member variables for new layout components

2. **MainWindow.cpp**
   - Completely redesigned setupUI() function
   - Added header layout with logo and title
   - Created 2x2 grid for 4 round buttons
   - Positioned exit button at bottom right
   - Updated window size to 800x600
   - Added emoji icons to buttons
   - Implemented circular button styling

### Button Mapping

- Search Pattern → opens SearchWindow
- Longest Repeated → opens PatternWindow
- Predict Completion → opens PredictionWindow
- Employee Rating → opens EmployeeRatingWindow
- Exit → closes application

Note: The mutation window option has been removed to meet the 4-button requirement.
