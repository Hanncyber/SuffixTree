#include "PatternWindow.h"
#include <QMessageBox>
#include <QFont>
#include <sstream>
#include <iostream>

PatternWindow::PatternWindow(QWidget *parent)
    : QWidget(parent), tree(nullptr), parent(parent) {
    setupUI();
    setWindowTitle("Longest Repeated Pattern");
    resize(1000, 800);
}

PatternWindow::~PatternWindow() {
    if (tree) delete tree;
}

void PatternWindow::setupUI() {
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    titleLabel = new QLabel("Find Longest Repeated Pattern", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { color: #2ECC71; margin-bottom: 10px; }");
    mainLayout->addWidget(titleLabel);

    // Text input
    QLabel *textLabel = new QLabel("Enter text to analyze:", this);
    textLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; }");
    mainLayout->addWidget(textLabel);
    
    textInput = new QLineEdit(this);
    textInput->setPlaceholderText("e.g., abcabcabc");
    textInput->setMinimumHeight(35);
    textInput->setStyleSheet("QLineEdit { padding: 5px; font-size: 13px; }");
    mainLayout->addWidget(textInput);

    buildButton = new QPushButton("Build Suffix Tree", this);
    buildButton->setMinimumHeight(40);
    buildButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #2ECC71;"
        "   color: white;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover { background-color: #27AE60; }"
    );
    connect(buildButton, &QPushButton::clicked, this, &PatternWindow::buildTree);
    mainLayout->addWidget(buildButton);

    findButton = new QPushButton("Find Longest Repeated Pattern", this);
    findButton->setMinimumHeight(40);
    findButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #F39C12;"
        "   color: white;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover { background-color: #E67E22; }"
        "QPushButton:disabled { background-color: #BDC3C7; }"
    );
    findButton->setEnabled(false);
    connect(findButton, &QPushButton::clicked, this, &PatternWindow::findPattern);
    mainLayout->addWidget(findButton);

    // Result text
    resultText = new QTextEdit(this);
    resultText->setReadOnly(true);
    resultText->setMaximumHeight(100);
    resultText->setStyleSheet("QTextEdit { font-size: 12px; background-color: #ECF0F1; font-family: monospace; }");
    mainLayout->addWidget(resultText);

    // Tree visualizer in scroll area
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(350);
    scrollArea->setStyleSheet("QScrollArea { border: 2px solid #BDC3C7; border-radius: 5px; }");
    
    treeVisualizer = new TreeVisualizer(nullptr, "", this);
    scrollArea->setWidget(treeVisualizer);
    mainLayout->addWidget(scrollArea);

    // Back button
    backButton = new QPushButton("Back to Main Menu", this);
    backButton->setMinimumHeight(40);
    backButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #95A5A6;"
        "   color: white;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover { background-color: #7F8C8D; }"
    );
    connect(backButton, &QPushButton::clicked, this, &PatternWindow::goBack);
    mainLayout->addWidget(backButton);
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

    // Capture cout output
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    tree->detect_longest_pattern();
    
    std::cout.rdbuf(old);
    
    resultText->setText(QString::fromStdString(buffer.str()));
}

void PatternWindow::goBack() {
    if (parent) {
        parent->show();
    }
    this->close();
}
