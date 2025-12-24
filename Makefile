# Makefile for Suffix Tree CLI and GUI

CXX = g++
CXXFLAGS = -std=c++11 -Wall

# CLI targets
CLI_TARGET = suffix_tree_cli
CLI_SOURCES = main.cpp SuffixTree.cpp
CLI_OBJECTS = main.o SuffixTree_cli.o

# Employee Rating Test
EMP_TARGET = test_employee_rating
EMP_OBJECTS = test_employee_rating.o employeerating.o SuffixTree_emp.o

.PHONY: all cli gui test-emp clean run-cli run-gui run-test-emp help

all: cli gui

help:
	@echo "Suffix Tree Build System"
	@echo "======================="
	@echo "Targets:"
	@echo "  make all          - Build both CLI and GUI"
	@echo "  make cli          - Build CLI version only"
	@echo "  make gui          - Build GUI version only"
	@echo "  make test-emp     - Build Employee Rating test program"
	@echo "  make run-cli      - Build and run CLI"
	@echo "  make run-gui      - Build and run GUI"
	@echo "  make run-test-emp - Build and run Employee Rating test"
	@echo "  make clean        - Remove build artifacts"

# CLI build
cli: $(CLI_TARGET)

$(CLI_TARGET): main.o SuffixTree_cli.o
	$(CXX) $(CXXFLAGS) -o $(CLI_TARGET) main.o SuffixTree_cli.o

main.o: main.cpp SuffixTree.h
	$(CXX) $(CXXFLAGS) -c main.cpp

SuffixTree_cli.o: SuffixTree.cpp SuffixTree.h
	$(CXX) $(CXXFLAGS) -c SuffixTree.cpp -o SuffixTree_cli.o

# GUI build
gui:
	@echo "Building GUI with Qt..."
	qmake SuffixTreeGUI.pro -o Makefile.qt
	make -f Makefile.qt
	@echo "GUI build complete: ./SuffixTreeGUI"

# Employee Rating Test build
test-emp: $(EMP_TARGET)

$(EMP_TARGET): test_employee_rating.o employeerating.o SuffixTree_emp.o
	$(CXX) $(CXXFLAGS) -o $(EMP_TARGET) test_employee_rating.o employeerating.o SuffixTree_emp.o

test_employee_rating.o: test_employee_rating.cpp employeerating.h SuffixTree.h
	$(CXX) $(CXXFLAGS) -c test_employee_rating.cpp

employeerating.o: employeerating.cpp employeerating.h SuffixTree.h
	$(CXX) $(CXXFLAGS) -c employeerating.cpp

SuffixTree_emp.o: SuffixTree.cpp SuffixTree.h
	$(CXX) $(CXXFLAGS) -c SuffixTree.cpp -o SuffixTree_emp.o

# Run targets
run-cli: cli
	@echo "Running CLI..."
	./$(CLI_TARGET)

run-gui: gui
	@echo "Running GUI..."
	./SuffixTreeGUI

run-test-emp: test-emp
	@echo "Running Employee Rating Test..."
	./$(EMP_TARGET)

# Clean
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(CLI_OBJECTS) main.o SuffixTree_cli.o
	rm -f $(EMP_OBJECTS) test_employee_rating.o employeerating.o SuffixTree_emp.o
	rm -f $(CLI_TARGET) $(EMP_TARGET)
	rm -f Makefile.qt
	rm -f *.o moc_*.cpp moc_*.h qrc_*.cpp ui_*.h
	rm -f SuffixTreeGUI
	rm -rf .qmake.stash
	rm -f moc_predefs.h
	@echo "Clean complete."

distclean: clean
	rm -rf build/
