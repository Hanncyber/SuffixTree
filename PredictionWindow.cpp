#include "PredictionWindow.h"
#include <QMessageBox>
#include <QFont>
#include <QHBoxLayout>
#include <sstream>
#include <iostream>

PredictionWindow::PredictionWindow(QWidget *parent)
    : QWidget(parent), tree(nullptr), parent(parent) {
    setupUI();
    setWindowTitle("Predict Completions");
    resize(1000, 800);
}

PredictionWindow::~PredictionWindow() {
    if (tree) delete tree;
}

void PredictionWindow::setupUI() {
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    titleLabel = new QLabel("Predict Completions", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { color: #F39C12; margin-bottom: 10px; }");
    mainLayout->addWidget(titleLabel);

    // Text input
    QLabel *textLabel = new QLabel("Enter text to build suffix tree:", this);
    textLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; }");
    mainLayout->addWidget(textLabel);
    
    textInput = new QLineEdit(this);
    textInput->setPlaceholderText("e.g., hello world, hello there, help me");
    textInput->setMinimumHeight(35);
    textInput->setStyleSheet("QLineEdit { padding: 5px; font-size: 13px; }");
    mainLayout->addWidget(textInput);

    buildButton = new QPushButton("Build Suffix Tree", this);
    buildButton->setMinimumHeight(40);
    buildButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #F39C12;"
        "   color: white;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover { background-color: #E67E22; }"
    );
    connect(buildButton, &QPushButton::clicked, this, &PredictionWindow::buildTree);
    mainLayout->addWidget(buildButton);

    // Prefix input
    QLabel *prefixLabel = new QLabel("Enter prefix for completion:", this);
    prefixLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; margin-top: 10px; }");
    mainLayout->addWidget(prefixLabel);
    
    prefixInput = new QLineEdit(this);
    prefixInput->setPlaceholderText("e.g., hel");
    prefixInput->setMinimumHeight(35);
    prefixInput->setStyleSheet("QLineEdit { padding: 5px; font-size: 13px; }");
    prefixInput->setEnabled(false);
    mainLayout->addWidget(prefixInput);

    // Max suggestions
    QHBoxLayout *suggestionsLayout = new QHBoxLayout();
    QLabel *maxLabel = new QLabel("Maximum suggestions:", this);
    maxLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; }");
    suggestionsLayout->addWidget(maxLabel);
    
    maxSuggestionsInput = new QSpinBox(this);
    maxSuggestionsInput->setMinimum(1);
    maxSuggestionsInput->setMaximum(20);
    maxSuggestionsInput->setValue(5);
    maxSuggestionsInput->setMinimumHeight(35);
    maxSuggestionsInput->setStyleSheet("QSpinBox { padding: 5px; font-size: 13px; }");
    maxSuggestionsInput->setEnabled(false);
    suggestionsLayout->addWidget(maxSuggestionsInput);
    suggestionsLayout->addStretch();
    mainLayout->addLayout(suggestionsLayout);

    predictButton = new QPushButton("Predict Completions", this);
    predictButton->setMinimumHeight(40);
    predictButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #2ECC71;"
        "   color: white;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover { background-color: #27AE60; }"
        "QPushButton:disabled { background-color: #BDC3C7; }"
    );
    predictButton->setEnabled(false);
    connect(predictButton, &QPushButton::clicked, this, &PredictionWindow::predictCompletions);
    mainLayout->addWidget(predictButton);

    // Result text
    resultText = new QTextEdit(this);
    resultText->setReadOnly(true);
    resultText->setMaximumHeight(150);
    resultText->setStyleSheet("QTextEdit { font-size: 12px; background-color: #ECF0F1; font-family: monospace; }");
    mainLayout->addWidget(resultText);

    // Tree visualizer in scroll area
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(250);
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
    connect(backButton, &QPushButton::clicked, this, &PredictionWindow::goBack);
    mainLayout->addWidget(backButton);
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
        maxSuggestionsInput->setEnabled(true);
        predictButton->setEnabled(true);
        resultText->setText("Suffix tree built successfully! You can now enter a prefix for predictions.");
    } catch (const std::exception& e) {
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

    int maxSuggestions = maxSuggestionsInput->value();

    // Capture cout output
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    tree->predictCompletions(prefix.toStdString(), maxSuggestions);
    
    std::cout.rdbuf(old);
    
    resultText->setText(QString::fromStdString(buffer.str()));
}

void PredictionWindow::goBack() {
    if (parent) {
        parent->show();
    }
    this->close();
}
