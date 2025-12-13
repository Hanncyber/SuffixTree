#include "SearchWindow.h"
#include <QMessageBox>
#include <QFont>
#include <sstream>

SearchWindow::SearchWindow(QWidget *parent)
    : QWidget(parent), tree(nullptr), parentWindow(parent) {
    setupUI();
    setWindowTitle("Search Pattern");
    resize(1000, 800);
}

SearchWindow::~SearchWindow() {
    if (tree) delete tree;
}

void SearchWindow::setupUI() {
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    titleLabel = new QLabel("Search Pattern in Suffix Tree", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { color: #3498DB; margin-bottom: 10px; }");
    mainLayout->addWidget(titleLabel);

    // Text input
    QLabel *textLabel = new QLabel("Enter text to build suffix tree:", this);
    textLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; }");
    mainLayout->addWidget(textLabel);
    
    textInput = new QLineEdit(this);
    textInput->setPlaceholderText("e.g., banana");
    textInput->setMinimumHeight(35);
    textInput->setStyleSheet("QLineEdit { padding: 5px; font-size: 13px; }");
    mainLayout->addWidget(textInput);

    buildButton = new QPushButton("Build Suffix Tree", this);
    buildButton->setMinimumHeight(40);
    buildButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498DB;"
        "   color: white;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover { background-color: #2980B9; }"
    );
    connect(buildButton, &QPushButton::clicked, this, &SearchWindow::buildTree);
    mainLayout->addWidget(buildButton);

    // Pattern input
    QLabel *patternLabel = new QLabel("Enter pattern to search:", this);
    patternLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; margin-top: 10px; }");
    mainLayout->addWidget(patternLabel);
    
    patternInput = new QLineEdit(this);
    patternInput->setPlaceholderText("e.g., ana");
    patternInput->setMinimumHeight(35);
    patternInput->setStyleSheet("QLineEdit { padding: 5px; font-size: 13px; }");
    patternInput->setEnabled(false);
    mainLayout->addWidget(patternInput);

    searchButton = new QPushButton("Search", this);
    searchButton->setMinimumHeight(40);
    searchButton->setStyleSheet(
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
    searchButton->setEnabled(false);
    connect(searchButton, &QPushButton::clicked, this, &SearchWindow::searchPattern);
    mainLayout->addWidget(searchButton);

    // Result text
    resultText = new QTextEdit(this);
    resultText->setReadOnly(true);
    resultText->setMaximumHeight(100);
    resultText->setStyleSheet("QTextEdit { font-size: 12px; background-color: #ECF0F1; }");
    mainLayout->addWidget(resultText);

    // Tree visualizer in scroll area
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(300);
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
    connect(backButton, &QPushButton::clicked, this, &SearchWindow::goBack);
    mainLayout->addWidget(backButton);
}

void SearchWindow::buildTree() {
    QString text = textInput->text();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter text to build the tree.");
        return;
    }

    if (tree) delete tree;
    
    try {
        tree = new SuffixTree(text.toStdString());
        treeVisualizer->setTree(tree->getRoot(), tree->getText());
        
        patternInput->setEnabled(true);
        searchButton->setEnabled(true);
        resultText->setText("Suffix tree built successfully! You can now search for patterns.");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to build tree: %1").arg(e.what()));
    }
}

void SearchWindow::searchPattern() {
    if (!tree) {
        QMessageBox::warning(this, "Error", "Please build a tree first.");
        return;
    }

    QString pattern = patternInput->text();
    if (pattern.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a pattern to search.");
        return;
    }

    int count = 0;
    int* positions = tree->search(pattern.toStdString(), count);

    std::stringstream result;
    if (count == 0) {
        result << "Pattern '" << pattern.toStdString() << "' not found in the text.";
    } else {
        result << "Pattern '" << pattern.toStdString() << "' found at " << count << " position(s): ";
        for (int i = 0; i < count; i++) {
            result << positions[i];
            if (i < count - 1) result << ", ";
        }
    }

    resultText->setText(QString::fromStdString(result.str()));
}

void SearchWindow::goBack() {
    if (parentWindow) {
        parentWindow->show();
    }
    this->close();
}
