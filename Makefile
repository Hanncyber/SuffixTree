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

run-example: example
	./example

run-test: test
	./test_suffix_tree

clean:
	rm -f example test_suffix_tree

.PHONY: all clean run-example run-test
