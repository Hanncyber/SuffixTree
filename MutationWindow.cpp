#include "MutationWindow.h"
#include <QMessageBox>
#include <QFont>
#include <sstream>
#include <iostream>

MutationWindow::MutationWindow(QWidget *parent)
    : QWidget(parent), tree(nullptr), parent(parent) {
    setupUI();
    setWindowTitle("DNA Mutation Detection");
    resize(1000, 800);
}

MutationWindow::~MutationWindow() {
    if (tree) delete tree;
}

void MutationWindow::setupUI() {
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    titleLabel = new QLabel("DNA Mutation Detection", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { color: #E74C3C; margin-bottom: 10px; }");
    mainLayout->addWidget(titleLabel);

    // Reference DNA input
    QLabel *refLabel = new QLabel("Enter reference DNA sequence:", this);
    refLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; }");
    mainLayout->addWidget(refLabel);
    
    referenceInput = new QLineEdit(this);
    referenceInput->setPlaceholderText("e.g., ACGTACGT");
    referenceInput->setMinimumHeight(35);
    referenceInput->setStyleSheet("QLineEdit { padding: 5px; font-size: 13px; }");
    mainLayout->addWidget(referenceInput);

    buildButton = new QPushButton("Build Reference Tree", this);
    buildButton->setMinimumHeight(40);
    buildButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #E74C3C;"
        "   color: white;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover { background-color: #C0392B; }"
    );
    connect(buildButton, &QPushButton::clicked, this, &MutationWindow::buildTree);
    mainLayout->addWidget(buildButton);

    // Sample DNA input
    QLabel *sampleLabel = new QLabel("Enter sample DNA sequence:", this);
    sampleLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; margin-top: 10px; }");
    mainLayout->addWidget(sampleLabel);
    
    sampleInput = new QLineEdit(this);
    sampleInput->setPlaceholderText("e.g., ACTTACGT");
    sampleInput->setMinimumHeight(35);
    sampleInput->setStyleSheet("QLineEdit { padding: 5px; font-size: 13px; }");
    sampleInput->setEnabled(false);
    mainLayout->addWidget(sampleInput);

    detectButton = new QPushButton("Detect Mutations", this);
    detectButton->setMinimumHeight(40);
    detectButton->setStyleSheet(
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
    detectButton->setEnabled(false);
    connect(detectButton, &QPushButton::clicked, this, &MutationWindow::detectMutations);
    mainLayout->addWidget(detectButton);

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
    connect(backButton, &QPushButton::clicked, this, &MutationWindow::goBack);
    mainLayout->addWidget(backButton);
}

void MutationWindow::buildTree() {
    QString text = referenceInput->text();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter reference DNA sequence.");
        return;
    }

    if (tree) delete tree;
    
    try {
        tree = new SuffixTree(text.toStdString());
        treeVisualizer->setTree(tree->getRoot(), tree->getText());
        
        sampleInput->setEnabled(true);
        detectButton->setEnabled(true);
        resultText->setText("Reference tree built successfully! You can now enter a sample sequence for mutation detection.");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to build tree: %1").arg(e.what()));
    }
}

void MutationWindow::detectMutations() {
    if (!tree) {
        QMessageBox::warning(this, "Error", "Please build a reference tree first.");
        return;
    }

    QString sample = sampleInput->text();
    if (sample.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a sample DNA sequence.");
        return;
    }

    // Capture cout output
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    tree->detectDNAMutationss(sample.toStdString());
    
    std::cout.rdbuf(old);
    
    resultText->setText(QString::fromStdString(buffer.str()));
}

void MutationWindow::goBack() {
    if (parent) {
        parent->show();
    }
    this->close();
}
