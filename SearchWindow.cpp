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
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    QLabel *title = new QLabel("Suffix Tree Pattern Search", this);
    QFont titleFont;
    titleFont.setFamily("Verdana");
    titleFont.setPointSize(24);
    titleFont.setWeight(QFont::DemiBold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #C8A2C8;");
    mainLayout->addWidget(title);

    QHBoxLayout *textLayout = new QHBoxLayout();
    textInput = new QLineEdit(this);
    textInput->setPlaceholderText("Enter text to build suffix tree, e.g., banana");

    buildButton = new QPushButton("Build Tree", this);
    buildButton->setStyleSheet(
        "QPushButton { background-color: #7C6DB0; color: white; font-weight: bold; border-radius: 5px; padding: 6px; }"
        "QPushButton:hover { background-color: #8E7FD1; }"
        );
    connect(buildButton, &QPushButton::clicked, this, &SearchWindow::buildTree);

    textLayout->addWidget(textInput, 3);
    textLayout->addWidget(buildButton, 1);
    mainLayout->addLayout(textLayout);

    QHBoxLayout *patternLayout = new QHBoxLayout();
    patternInput = new QLineEdit(this);
    patternInput->setPlaceholderText("Enter pattern, e.g., ana");
    patternInput->setEnabled(false);

    searchButton = new QPushButton("Search", this);
    searchButton->setEnabled(false);
    searchButton->setStyleSheet(
        "QPushButton { background-color: #7C6DB0; color: white; font-weight: bold; border-radius: 5px; padding: 6px; }"
        "QPushButton:hover { background-color: #8E7FD1; }"
        "QPushButton:disabled { background-color: #555; color: #AAA; }"
        );
    connect(searchButton, &QPushButton::clicked, this, &SearchWindow::searchPattern);

    patternLayout->addWidget(patternInput, 3);
    patternLayout->addWidget(searchButton, 1);
    mainLayout->addLayout(patternLayout);

    resultText = new QTextEdit(this);
    resultText->setReadOnly(true);
    resultText->setMaximumHeight(80);
    mainLayout->addWidget(resultText);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(400);
    scrollArea->setStyleSheet("QScrollArea { border: 1px solid #444; }");
    treeVisualizer = new TreeVisualizer(nullptr, "", this);
    scrollArea->setWidget(treeVisualizer);
    mainLayout->addWidget(scrollArea);

    backButton = new QPushButton("Back to Main Menu", this);
    backButton->setStyleSheet(
        "QPushButton { background-color: #B91C1C; color: #FFFFFF; font-weight: bold; border-radius: 5px; padding: 6px; }"
        "QPushButton:hover { background-color: #DC2626; }"
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
