#include "SearchWindow.h"
#include <QMessageBox>
#include <QFont>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <sstream>

SearchWindow::SearchWindow(QWidget *parent)
    : QWidget(parent), tree(nullptr), parentWindow(parent) {
    setupUI();
    setWindowTitle("Search Pattern");
    resize(900, 700);
}

SearchWindow::~SearchWindow() {
    if (tree) delete tree;
}

void SearchWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    // ---------- Title ----------
    QLabel *title = new QLabel("Suffix Tree Pattern Search", this);
    QFont titleFont = title->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #2ECC71;");
    mainLayout->addWidget(title);

    // ---------- Input text ----------
    QHBoxLayout *textLayout = new QHBoxLayout();
    textInput = new QLineEdit(this);
    textInput->setPlaceholderText("Enter text to build suffix tree, e.g., banana");
    textInput->setStyleSheet("QLineEdit { font-size: 13px; padding: 4px; }");
    buildButton = new QPushButton("Build Tree", this);
    buildButton->setStyleSheet(
        "QPushButton { background-color: #16A085; color: white; font-weight: bold; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: #1ABC9C; }"
        );
    connect(buildButton, &QPushButton::clicked, this, &SearchWindow::buildTree);
    textLayout->addWidget(textInput, 3);
    textLayout->addWidget(buildButton, 1);
    mainLayout->addLayout(textLayout);

    // ---------- Pattern input ----------
    QHBoxLayout *patternLayout = new QHBoxLayout();
    patternInput = new QLineEdit(this);
    patternInput->setPlaceholderText("Enter pattern, e.g., ana");
    patternInput->setStyleSheet("QLineEdit { font-size: 13px; padding: 4px; }");
    patternInput->setEnabled(false);

    searchButton = new QPushButton("Search", this);
    searchButton->setStyleSheet(
        "QPushButton { background-color: #F39C12; color: white; font-weight: bold; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: #E67E22; }"
        "QPushButton:disabled { background-color: #7F8C8D; }"
        );
    searchButton->setEnabled(false);
    connect(searchButton, &QPushButton::clicked, this, &SearchWindow::searchPattern);

    patternLayout->addWidget(patternInput, 3);
    patternLayout->addWidget(searchButton, 1);
    mainLayout->addLayout(patternLayout);

    // ---------- Result display ----------
    resultText = new QTextEdit(this);
    resultText->setReadOnly(true);
    resultText->setStyleSheet(
        "QTextEdit { font-size: 13px; background-color: #2C3E50; color: #ECF0F1; border: 1px solid #34495E; }"
        );
    resultText->setMaximumHeight(80);
    mainLayout->addWidget(resultText);

    // ---------- Tree visualizer ----------
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(400);
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
    connect(backButton, &QPushButton::clicked, this, &SearchWindow::goBack);
    mainLayout->addWidget(backButton, 0, Qt::AlignRight);
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
        resultText->setText("Suffix tree built successfully!");
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
    if (count == 0)
        result << "Pattern '" << pattern.toStdString() << "' not found.";
    else {
        result << "Pattern '" << pattern.toStdString() << "' found at positions: ";
        for (int i = 0; i < count; i++) {
            result << positions[i];
            if (i < count - 1) result << ", ";
        }
    }
    resultText->setText(QString::fromStdString(result.str()));
}

void SearchWindow::goBack() {
    if (parentWindow) parentWindow->show();
    this->close();
}
