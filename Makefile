# Makefile for Suffix Tree CLI and GUI

CXX = g++
CXXFLAGS = -std=c++11 -Wall

# CLI targets
CLI_TARGET = suffix_tree_cli
CLI_SOURCES = main.cpp SuffixTree.cpp
CLI_OBJECTS = $(CLI_SOURCES:.cpp=.o)

# GUI targets
GUI_TARGET = SuffixTreeGUI
GUI_PRO = SuffixTreeGUI.pro

.PHONY: all cli gui clean run-cli run-gui

all: cli gui

# CLI build
cli: $(CLI_TARGET)

$(CLI_TARGET): $(CLI_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(CLI_TARGET) $(CLI_OBJECTS)

main.o: main.cpp SuffixTree.h
	$(CXX) $(CXXFLAGS) -c main.cpp

SuffixTree.o: SuffixTree.cpp SuffixTree.h
	$(CXX) $(CXXFLAGS) -c SuffixTree.cpp

# GUI build
gui: $(GUI_TARGET)

$(GUI_TARGET): $(GUI_PRO)
	qmake $(GUI_PRO)
	make -f Makefile.gui

Makefile.gui: $(GUI_PRO)
	qmake $(GUI_PRO) -o Makefile.gui

# Run targets
run-cli: cli
	./$(CLI_TARGET)

run-gui: gui
	./$(GUI_TARGET)

# Clean
clean:
	rm -f $(CLI_OBJECTS) $(CLI_TARGET)
	rm -f Makefile.gui
	rm -f *.o moc_*.cpp qrc_*.cpp ui_*.h
	rm -f $(GUI_TARGET)
	rm -rf .qmake.stash

distclean: clean
	rm -rf build/
