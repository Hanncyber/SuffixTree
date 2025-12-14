#include "PatternWindow.h"
#include <QMessageBox>
#include <QFont>
#include <QHBoxLayout>
#include <sstream>
#include <iostream>

PatternWindow::PatternWindow(QWidget *parent)
    : QWidget(parent), tree(nullptr), parentWindow(parent) {
    setupUI();
    setWindowTitle("Longest Repeated Pattern");
    resize(900, 700);
}

PatternWindow::~PatternWindow() {
    if (tree) delete tree;
}

void PatternWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // ---------- Title ----------
    QLabel *title = new QLabel("Longest Repeated Pattern Finder", this);
    QFont titleFont = title->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #2ECC71;");
    mainLayout->addWidget(title);

    // ---------- Text input ----------
    QHBoxLayout *textLayout = new QHBoxLayout();
    textInput = new QLineEdit(this);
    textInput->setPlaceholderText("Enter text to analyze, e.g., abcabcabc");
    textInput->setStyleSheet("QLineEdit { font-size: 13px; padding: 4px; }");

    buildButton = new QPushButton("Build Tree", this);
    buildButton->setStyleSheet(
        "QPushButton { background-color: #2ECC71; color: white; font-weight: bold; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: #27AE60; }"
        );
    connect(buildButton, &QPushButton::clicked, this, &PatternWindow::buildTree);

    textLayout->addWidget(textInput, 3);
    textLayout->addWidget(buildButton, 1);
    mainLayout->addLayout(textLayout);

    // ---------- Find pattern button ----------
    findButton = new QPushButton("Find Longest Repeated Pattern", this);
    findButton->setStyleSheet(
        "QPushButton { background-color: #F39C12; color: white; font-weight: bold; border-radius: 5px; padding: 8px; }"
        "QPushButton:hover { background-color: #E67E22; }"
        "QPushButton:disabled { background-color: #BDC3C7; }"
        );
    findButton->setEnabled(false);
    connect(findButton, &QPushButton::clicked, this, &PatternWindow::findPattern);
    mainLayout->addWidget(findButton);

    // ---------- Result area ----------
    resultText = new QTextEdit(this);
    resultText->setReadOnly(true);
    resultText->setMaximumHeight(80);
    resultText->setStyleSheet(
        "QTextEdit { font-size: 13px; background-color: #2C3E50; color: #ECF0F1; font-family: monospace; border: 1px solid #34495E; }"
        );
    mainLayout->addWidget(resultText);

    // ---------- Tree visualizer ----------
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(350);
    scrollArea->setStyleSheet("QScrollArea { border: 1px solid #34495E; }");

    treeVisualizer = new TreeVisualizer(nullptr, "", this);
    scrollArea->setWidget(treeVisualizer);
    mainLayout->addWidget(scrollArea);

    // ---------- Back button ----------
    backButton = new QPushButton("Back to Main Menu", this);
    backButton->setStyleSheet(
        "QPushButton { background-color: #95A5A6; color: white; font-weight: bold; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: #7F8C8D; }"
        );
    connect(backButton, &QPushButton::clicked, this, &PatternWindow::goBack);
    mainLayout->addWidget(backButton, 0, Qt::AlignRight);
}

void PatternWindow::buildTree() {
    QString text = textInput->text();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter text to build the tree.");
        return;
    }

    if (tree) delete tree;

    try {
        tree = new SuffixTree(text.toStdString());
        treeVisualizer->setTree(tree->getRoot(), tree->getText());

        findButton->setEnabled(true);
        resultText->setText("Suffix tree built successfully! Click 'Find Longest Repeated Pattern' to analyze.");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to build tree: %1").arg(e.what()));
    }
}

void PatternWindow::findPattern() {
    if (!tree) {
        QMessageBox::warning(this, "Error", "Please build a tree first.");
        return;
    }

    // Capture output from detect_longest_pattern() if needed
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    tree->detect_longest_pattern();  // Make sure this prints to std::cout

    std::cout.rdbuf(old);

    resultText->setText(QString::fromStdString(buffer.str()));
}

void PatternWindow::goBack() {
    if (parentWindow) parentWindow->show();
    this->close();
}
