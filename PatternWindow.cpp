#include "PatternWindow.h"
#include <QMessageBox>
#include <QFont>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
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
    this->setStyleSheet(R"(
        QWidget {
            background-color: #1A1A1A;
            font-family: Arial;
        }
        QLabel {
            color: #EDEDED;
        }
        QLineEdit {
            font-size: 13px;
            padding: 6px;
            background-color: #2C2C2C;
            color: #FFFFFF;
            border: 1px solid #444;
            border-radius: 4px;
        }
        QTextEdit {
            font-size: 13px;
            background-color: #2C2C2C;
            color: #FFFFFF;
            font-family: monospace;
            border: 1px solid #444;
        }
        QPushButton {
            background-color: #7C6DB0;  /* lilac */
            color: #FFFFFF;
            border: 1px solid #9F91D8;
            border-radius: 5px;
            padding: 6px;
            font-weight: 500;
        }
        QPushButton:hover {
            background-color: #8E7FD1;
        }
        QPushButton:pressed {
            background-color: #6B5AA6;
        }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);


    QLabel *title = new QLabel("Longest Repeated Pattern Finder", this);
    QFont titleFont("Verdana", 24, QFont::DemiBold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #C8A2C8;");
    mainLayout->addWidget(title);

    QHBoxLayout *textLayout = new QHBoxLayout();
    textInput = new QLineEdit(this);
    textInput->setPlaceholderText("Enter text to analyze, e.g., abcabcabc");

    buildButton = new QPushButton("Build Tree", this);
    buildButton->setStyleSheet(
        "QPushButton { background-color: #7C6DB0; color: white; font-weight: bold; border-radius: 5px; padding: 6px; }"
        "QPushButton:hover { background-color: #8E7FD1; }"
        );
    connect(buildButton, &QPushButton::clicked, this, &PatternWindow::buildTree);

    textLayout->addWidget(textInput, 3);
    textLayout->addWidget(buildButton, 1);
    mainLayout->addLayout(textLayout);


    findButton = new QPushButton("Find Longest Repeated Pattern", this);
    findButton->setEnabled(false);
    findButton->setStyleSheet(
        "QPushButton { background-color: #7C6DB0; color: white; font-weight: bold; border-radius: 5px; padding: 8px; }"
        "QPushButton:hover { background-color: #8E7FD1; }"
        "QPushButton:disabled { background-color: #555; color: #AAA; }"
        );
    connect(findButton, &QPushButton::clicked, this, &PatternWindow::findPattern);
    mainLayout->addWidget(findButton);


    resultText = new QTextEdit(this);
    resultText->setReadOnly(true);
    resultText->setMaximumHeight(80);
    mainLayout->addWidget(resultText);


    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(350);
    treeVisualizer = new TreeVisualizer(nullptr, "", this);
    scrollArea->setWidget(treeVisualizer);
    mainLayout->addWidget(scrollArea);

    // ---------- Back button ----------
    backButton = new QPushButton("Back to Main Menu", this);
    backButton->setStyleSheet(
        "QPushButton { background-color: #B91C1C; color: #FFFFFF; font-weight: bold; border-radius: 5px; padding: 6px; }"
        "QPushButton:hover { background-color: #DC2626; }"
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

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    tree->detect_longest_pattern();

    std::cout.rdbuf(old);

    resultText->setText(QString::fromStdString(buffer.str()));
}

void PatternWindow::goBack() {
    if (parentWindow) parentWindow->show();
    this->close();
}
