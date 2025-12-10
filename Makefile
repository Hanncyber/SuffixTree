# Makefile for Suffix Tree Animation
# 
# This Makefile builds the suffix tree animation GUI application
# 
# Prerequisites:
#   - C++ compiler (g++ or clang++)
#   - SFML library (Simple and Fast Multimedia Library)
#
# Installation of SFML:
#   Ubuntu/Debian: sudo apt-get install libsfml-dev
#   macOS: brew install sfml
#   Windows: Download from https://www.sfml-dev.org/download.php

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Target executable
TARGET = SuffixTreeAnimation

# Source files
SOURCES = SuffixTreeGUI.cpp SuffixTree.cpp
HEADERS = SuffixTree.hpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(SFML_LIBS)
	@echo "Build successful! Run with: ./$(TARGET)"

# Compile source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Cleaned build artifacts"

# Run the application
run: $(TARGET)
	./$(TARGET)

# Check if SFML is installed
check-sfml:
	@echo "Checking for SFML installation..."
	@pkg-config --exists sfml-graphics && echo "✓ SFML found" || echo "✗ SFML not found. Please install SFML."

# Help target
help:
	@echo "Suffix Tree Animation - Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  make          - Build the application"
	@echo "  make run      - Build and run the application"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make check-sfml - Check if SFML is installed"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Prerequisites:"
	@echo "  Install SFML library:"
	@echo "    Ubuntu/Debian: sudo apt-get install libsfml-dev"
	@echo "    macOS: brew install sfml"

.PHONY: all clean run check-sfml help
