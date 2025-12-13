#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include "SuffixTree.h"
#include "TreeVisualizer.h"

class SearchWindow : public QWidget {
    Q_OBJECT

public:
    explicit SearchWindow(QWidget *parent = nullptr);
    ~SearchWindow();

private slots:
    void buildTree();
    void searchPattern();
    void goBack();

private:
    void setupUI();
    
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QLineEdit *textInput;
    QPushButton *buildButton;
    QLineEdit *patternInput;
    QPushButton *searchButton;
    QTextEdit *resultText;
    QPushButton *backButton;
    QScrollArea *scrollArea;
    TreeVisualizer *treeVisualizer;
    
    SuffixTree *tree;
    QWidget *parent;
};

#endif // SEARCHWINDOW_H
