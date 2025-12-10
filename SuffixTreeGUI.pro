QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SuffixTreeGUI
TEMPLATE = app

CONFIG += c++17

SOURCES += \
    main_gui.cpp \
    MainWindow.cpp \
    SuffixTree.cpp

HEADERS += \
    MainWindow.h \
    Suffixtree.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
