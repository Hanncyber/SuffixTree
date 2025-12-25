#ifndef PREDICTIONWINDOW_H
#define PREDICTIONWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QScrollArea>
#include "SuffixTree.h"
#include "TreeVisualizer.h"

class PredictionWindow : public QWidget {
    Q_OBJECT

public:
    explicit PredictionWindow(QWidget *parent = nullptr);
    ~PredictionWindow();

private slots:
    void buildTree();
    void predictCompletions();
    void goBack();

private:
    void setupUI();
    
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QLineEdit *textInput;
    QPushButton *buildButton;
    QLineEdit *prefixInput;
    QSpinBox *maxSuggestionsInput;
    QSpinBox *freqThresholdInput;
    QPushButton *predictButton;
    QTextEdit *resultText;
    QPushButton *backButton;
    QScrollArea *scrollArea;
    TreeVisualizer *treeVisualizer;
    
    SuffixTree *tree;
    QWidget *parentWindow;
};

#endif // PREDICTIONWINDOW_H
