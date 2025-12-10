#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <memory>
#include "Suffixtree.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onBuildTree();
    void onSearchPattern();
    void onClear();

private:
    void setupUI();
    void visualizeTree(SuffixTree* tree);
    void drawNode(SuffixNode* node, qreal x, qreal y, qreal horizontalSpacing, 
                  int depth, SuffixTree* tree);

    // UI Components
    QLineEdit* textInput;
    QLineEdit* searchInput;
    QPushButton* buildButton;
    QPushButton* searchButton;
    QPushButton* clearButton;
    QTextEdit* statsDisplay;
    QTextEdit* resultsDisplay;
    QGraphicsView* treeView;
    QGraphicsScene* treeScene;

    // Data
    std::unique_ptr<SuffixTree> suffixTree;
    QString currentText;
};

#endif // MAINWINDOW_H
