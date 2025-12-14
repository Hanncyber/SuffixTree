#include "PredictionWindow.h"
#include <QMessageBox>
#include <QFont>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <sstream>
#include <iostream>


PredictionWindow::PredictionWindow(QWidget *parent)
    : QWidget(parent), tree(nullptr), parentWindow(parent) {
    setupUI();
    setWindowTitle("Predict Completions");
    resize(900, 700);
}

PredictionWindow::~PredictionWindow() {
    if (tree) delete tree;
}

void PredictionWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    // ---------- Title ----------
    QLabel *title = new QLabel("Suffix Tree Completion Predictor", this);
    QFont titleFont = title->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #F39C12;");
    mainLayout->addWidget(title);

    // ---------- Text input ----------
    QHBoxLayout *textLayout = new QHBoxLayout();
    textInput = new QLineEdit(this);
    textInput->setPlaceholderText("Enter text to build suffix tree, e.g., hello world");
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

    // ---------- Prefix input ----------
    QHBoxLayout *prefixLayout = new QHBoxLayout();
    prefixInput = new QLineEdit(this);
    prefixInput->setPlaceholderText("Enter prefix, e.g., hel");
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

    // ---------- Max suggestions ----------
    QHBoxLayout *maxLayout = new QHBoxLayout();
    QLabel *maxLabel = new QLabel("Max suggestions:", this);
    maxLabel->setStyleSheet("QLabel { font-size: 13px; font-weight: bold; }");
    maxLayout->addWidget(maxLabel);

    maxSuggestionsInput = new QSpinBox(this);
    maxSuggestionsInput->setMinimum(1);
    maxSuggestionsInput->setMaximum(20);
    maxSuggestionsInput->setValue(5);
    maxSuggestionsInput->setStyleSheet("QSpinBox { font-size: 13px; padding: 4px; }");
    maxSuggestionsInput->setEnabled(false);
    maxLayout->addWidget(maxSuggestionsInput);
    maxLayout->addStretch();
    mainLayout->addLayout(maxLayout);

    // ---------- Result display ----------
    resultText = new QTextEdit(this);
    resultText->setReadOnly(true);
    resultText->setMaximumHeight(120);
    resultText->setStyleSheet(
        "QTextEdit { font-size: 13px; background-color: #2C3E50; color: #ECF0F1; font-family: monospace; border: 1px solid #34495E; padding: 4px; }"
        );
    mainLayout->addWidget(resultText);

    // ---------- Tree visualizer ----------
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(250);
    scrollArea->setStyleSheet("QScrollArea { border: 1px solid #34495E; border-radius: 4px; }");

    treeVisualizer = new TreeVisualizer(nullptr, "", this);
    scrollArea->setWidget(treeVisualizer);
    mainLayout->addWidget(scrollArea);

    // ---------- Back button ----------
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
        maxSuggestionsInput->setEnabled(true);
        predictButton->setEnabled(true);
        resultText->setText("Suffix tree built successfully!");
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

    // Capture output
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    tree->predictCompletions(prefix.toStdString(), maxSuggestions);
    std::cout.rdbuf(old);

    resultText->setText(QString::fromStdString(buffer.str()));
}

void PredictionWindow::goBack() {
    if (parentWindow) parentWindow->show();
    this->close();
}
