QT += core gui widgets

TARGET = SuffixTreeGUI
TEMPLATE = app

CONFIG += c++11

SOURCES += \
    main_gui.cpp \
    MainWindow.cpp \
    SearchWindow.cpp \
    MutationWindow.cpp \
    PatternWindow.cpp \
    PredictionWindow.cpp \
    TreeVisualizer.cpp \
    SuffixTree.cpp

HEADERS += \
    MainWindow.h \
    SearchWindow.h \
    MutationWindow.h \
    PatternWindow.h \
    PredictionWindow.h \
    TreeVisualizer.h \
    SuffixTree.h
