#ifndef PATTERNWINDOW_H
#define PATTERNWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include "SuffixTree.h"
#include "TreeVisualizer.h"

class PatternWindow : public QWidget {
    Q_OBJECT

public:
    explicit PatternWindow(QWidget *parent = nullptr);
    ~PatternWindow();

private slots:
    void buildTree();
    void findPattern();
    void goBack();

private:
    void setupUI();
    
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QLineEdit *textInput;
    QPushButton *buildButton;
    QPushButton *findButton;
    QTextEdit *resultText;
    QPushButton *backButton;
    QScrollArea *scrollArea;
    TreeVisualizer *treeVisualizer;
    
    SuffixTree *tree;
    QWidget *parentWindow;
};

#endif // PATTERNWINDOW_H
