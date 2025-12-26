#include "PredictionWindow.h"
#include <QMessageBox>
#include <QFont>
#include <QHBoxLayout>
#include <sstream>
#include <iostream>

PredictionWindow::PredictionWindow(QWidget *parent)
    : QWidget(parent), tree(nullptr), parentWindow(parent) {
    setupUI();
    setWindowTitle("Predict Word Completions");
    resize(900, 700);
}

PredictionWindow::~PredictionWindow() {
    if (tree) delete tree;
}

void PredictionWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // Title
    QLabel *title = new QLabel("Word Prediction from Prefix", this);
    QFont titleFont = title->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("QLabel { color: #F39C12; }");
    mainLayout->addWidget(title);

    // Text input layout
    QHBoxLayout *textLayout = new QHBoxLayout();
    textInput = new QLineEdit(this);
    textInput->setPlaceholderText("Enter paragraph or text...");
    textInput->setStyleSheet("QLineEdit { font-size: 13px; padding: 4px; }");
    buildButton = new QPushButton("Build Tree", this);
    buildButton->setStyleSheet(
        "QPushButton { background-color: #F39C12; color: white; font-weight: bold; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: #E67E22; }"
        );
    connect(buildButton, &QPushButton::clicked, this, &PredictionWindow::buildTree);

    textLayout->addWidget(textInput, 3);
    textLayout->addWidget(buildButton, 1);
    mainLayout->addLayout(textLayout);

    // Prefix input layout
    QHBoxLayout *prefixLayout = new QHBoxLayout();
    prefixInput = new QLineEdit(this);
    prefixInput->setPlaceholderText("Enter prefix to predict...");
    prefixInput->setStyleSheet("QLineEdit { font-size: 13px; padding: 4px; }");
    prefixInput->setEnabled(false);

    predictButton = new QPushButton("Predict", this);
    predictButton->setStyleSheet(
        "QPushButton { background-color: #2ECC71; color: white; font-weight: bold; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: #27AE60; }"
        "QPushButton:disabled { background-color: #BDC3C7; }"
        );
    predictButton->setEnabled(false);
    connect(predictButton, &QPushButton::clicked, this, &PredictionWindow::predictCompletions);

    prefixLayout->addWidget(prefixInput, 3);
    prefixLayout->addWidget(predictButton, 1);
    mainLayout->addLayout(prefixLayout);

    // Result display
    resultText = new QTextEdit(this);
    resultText->setReadOnly(true);
    resultText->setMaximumHeight(120);
    resultText->setStyleSheet(
        "QTextEdit { font-size: 13px; background-color: #2C3E50; color: #ECF0F1; font-family: monospace; border: 1px solid #34495E; }"
        );
    mainLayout->addWidget(resultText);

    // Tree visualizer in scroll area
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(400);
    scrollArea->setStyleSheet("QScrollArea { border: 1px solid #34495E; }");

    treeVisualizer = new TreeVisualizer(nullptr, "", this);
    scrollArea->setWidget(treeVisualizer);
    mainLayout->addWidget(scrollArea);

    // Back button
    backButton = new QPushButton("Back to Main Menu", this);
    backButton->setStyleSheet(
        "QPushButton { background-color: #95A5A6; color: white; font-weight: bold; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: #7F8C8D; }"
        );
    connect(backButton, &QPushButton::clicked, this, &PredictionWindow::goBack);
    mainLayout->addWidget(backButton, 0, Qt::AlignRight);
}

void PredictionWindow::buildTree() {
    QString text = textInput->text();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter text to build the tree.");
        return;
    }

    if (tree) delete tree;

    try {
        tree = new SuffixTree(text.toStdString());
        treeVisualizer->setTree(tree->getRoot(), tree->getText());

        prefixInput->setEnabled(true);
        predictButton->setEnabled(true);
        resultText->setText("Suffix tree built successfully! Enter a prefix to predict words.");
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", QString("Failed to build tree: %1").arg(e.what()));
    }
}

void PredictionWindow::predictCompletions() {
    if (!tree) {
        QMessageBox::warning(this, "Error", "Please build a tree first.");
        return;
    }

    QString prefix = prefixInput->text();
    if (prefix.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a prefix.");
        return;
    }

    // Redirect output to resultText
    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

    tree->predictCompletions(prefix.toStdString(), 10); // up to 10 suggestions

    std::cout.rdbuf(old);

    resultText->setText(QString::fromStdString(buffer.str()));
}

void PredictionWindow::goBack() {
    if (parentWindow) parentWindow->show();
    this->close();
}
