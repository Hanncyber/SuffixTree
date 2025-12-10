CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Source files
SOURCES = SuffixTree.cpp
HEADERS = Suffixtree.h

# Targets
all: example test

example: example.cpp $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o example example.cpp $(SOURCES)

test: test_suffix_tree.cpp $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o test_suffix_tree test_suffix_tree.cpp $(SOURCES)

# Qt GUI application
gui:
	qmake SuffixTreeGUI.pro -o Makefile.qt
	$(MAKE) -f Makefile.qt

run-gui: gui
	./SuffixTreeGUI

run-example: example
	./example

run-test: test
	./test_suffix_tree

clean:
	rm -f example test_suffix_tree
	rm -f SuffixTreeGUI *.o moc_*.cpp moc_predefs.h .qmake.stash Makefile.qt

.PHONY: all clean run-example run-test gui run-gui
