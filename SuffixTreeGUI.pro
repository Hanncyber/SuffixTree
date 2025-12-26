QT += core gui widgets

TARGET = SuffixTreeGUI
TEMPLATE = app

CONFIG += c++11

SOURCES += \
    main_gui.cpp \
    MainWindow.cpp \
    SearchWindow.cpp \
    PatternWindow.cpp \
    PredictionWindow.cpp \
    TreeVisualizer.cpp \
    SuffixTree.cpp \
    employeerating.cpp \
    EmployeeRatingWindow.cpp \
    EmployeeTreeVisualizer.cpp

HEADERS += \
    MainWindow.h \
    SearchWindow.h \
    PatternWindow.h \
    PredictionWindow.h \
    TreeVisualizer.h \
    SuffixTree.h \
    employeerating.h \
    EmployeeRatingWindow.h \
    EmployeeTreeVisualizer.h
