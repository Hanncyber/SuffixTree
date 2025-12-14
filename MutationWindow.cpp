#include "MutationWindow.h"
#include <QMessageBox>
#include <QFont>
#include <QHBoxLayout>
#include <sstream>
#include <iostream>

MutationWindow::MutationWindow(QWidget *parent)
    : QWidget(parent), tree(nullptr), parentWindow(parent) {
    setupUI();
    setWindowTitle("DNA Mutation Detection");
    resize(900, 700);
}

MutationWindow::~MutationWindow() {
    if (tree) delete tree;
}

void MutationWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // ---------- Title ----------
    QLabel *title = new QLabel("DNA Mutation Detection", this);
    QFont titleFont = title->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #E74C3C;");
    mainLayout->addWidget(title);

    // ---------- Reference DNA input ----------
    QHBoxLayout *refLayout = new QHBoxLayout();
    referenceInput = new QLineEdit(this);
    referenceInput->setPlaceholderText("Enter reference DNA, e.g., ACGTACGT");
    referenceInput->setStyleSheet("QLineEdit { font-size: 13px; padding: 4px; }");

    buildButton = new QPushButton("Build Reference Tree", this);
    buildButton->setStyleSheet(
        "QPushButton { background-color: #E74C3C; color: white; font-weight: bold; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: #C0392B; }"
        );
    connect(buildButton, &QPushButton::clicked, this, &MutationWindow::buildTree);

    refLayout->addWidget(referenceInput, 3);
    refLayout->addWidget(buildButton, 1);
    mainLayout->addLayout(refLayout);

    // ---------- Sample DNA input ----------
    QHBoxLayout *sampleLayout = new QHBoxLayout();
    sampleInput = new QLineEdit(this);
    sampleInput->setPlaceholderText("Enter sample DNA, e.g., ACTTACGT");
    sampleInput->setStyleSheet("QLineEdit { font-size: 13px; padding: 4px; }");
    sampleInput->setEnabled(false);

    detectButton = new QPushButton("Detect Mutations", this);
    detectButton->setStyleSheet(
        "QPushButton { background-color: #2ECC71; color: white; font-weight: bold; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: #27AE60; }"
        "QPushButton:disabled { background-color: #BDC3C7; }"
        );
    detectButton->setEnabled(false);
    connect(detectButton, &QPushButton::clicked, this, &MutationWindow::detectMutations);

    sampleLayout->addWidget(sampleInput, 3);
    sampleLayout->addWidget(detectButton, 1);
    mainLayout->addLayout(sampleLayout);

    // ---------- Result area ----------
    resultText = new QTextEdit(this);
    resultText->setReadOnly(true);
    resultText->setMaximumHeight(100);
    resultText->setStyleSheet(
        "QTextEdit { font-size: 13px; background-color: #2C3E50; color: #ECF0F1; font-family: monospace; border: 1px solid #34495E; }"
        );
    mainLayout->addWidget(resultText);

    // ---------- Tree visualizer ----------
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(300);
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
    connect(backButton, &QPushButton::clicked, this, &MutationWindow::goBack);
    mainLayout->addWidget(backButton, 0, Qt::AlignRight);
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
        resultText->setText("Reference tree built successfully! Enter a sample DNA to detect mutations.");
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

    std::cout.rdbuf(old);

    resultText->setText(QString::fromStdString(buffer.str()));
}

void MutationWindow::goBack() {
    if (parentWindow) parentWindow->show();
    this->close();
}
