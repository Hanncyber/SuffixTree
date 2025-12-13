#ifndef MUTATIONWINDOW_H
#define MUTATIONWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include "SuffixTree.h"
#include "TreeVisualizer.h"

class MutationWindow : public QWidget {
    Q_OBJECT

public:
    explicit MutationWindow(QWidget *parent = nullptr);
    ~MutationWindow();

private slots:
    void buildTree();
    void detectMutations();
    void goBack();

private:
    void setupUI();
    
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QLineEdit *referenceInput;
    QPushButton *buildButton;
    QLineEdit *sampleInput;
    QPushButton *detectButton;
    QTextEdit *resultText;
    QPushButton *backButton;
    QScrollArea *scrollArea;
    TreeVisualizer *treeVisualizer;
    
    SuffixTree *tree;
    QWidget *parentWindow;
};

#endif // MUTATIONWINDOW_H
