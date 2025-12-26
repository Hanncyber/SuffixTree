#include "PredictionWindow.h"
#include <QMessageBox>
#include <QFont>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QSpinBox>
#include <sstream>
#include <iostream>

PredictionWindow::PredictionWindow(QWidget *parent)
    : QWidget(parent), tree(nullptr), parentWindow(parent) {
    setupUI();
    setWindowTitle("Google Text Prediction - Minimum Prefix Finder");
    resize(900, 700);
}

PredictionWindow::~PredictionWindow() {
    if (tree) delete tree;
}

void PredictionWindow::setupUI() {
    this->setStyleSheet(R"(
        QWidget {
            background-color: #1A1A1A;
            font-family: Arial;
        }
        QLabel {
            color: #EDEDED;
        }
        QLineEdit, QSpinBox {
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

    QLabel *title = new QLabel("Google Text Prediction - Minimum Prefix Finder", this);
    QFont titleFont("Verdana", 24, QFont::DemiBold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #C8A2C8;");
    mainLayout->addWidget(title);

    QHBoxLayout *textLayout = new QHBoxLayout();
    textInput = new QLineEdit(this);
    textInput->setPlaceholderText("Enter paragraph or text...");
    buildButton = new QPushButton("Build Tree", this);
    buildButton->setStyleSheet(
        "QPushButton { background-color: #7C6DB0; color: white; font-weight: bold; border-radius: 5px; padding: 6px; }"
        "QPushButton:hover { background-color: #8E7FD1; }"
        );
    connect(buildButton, &QPushButton::clicked, this, &PredictionWindow::buildTree);
    textLayout->addWidget(textInput, 3);
    textLayout->addWidget(buildButton, 1);
    mainLayout->addLayout(textLayout);

    QHBoxLayout *prefixLayout = new QHBoxLayout();
    prefixInput = new QLineEdit(this);
    prefixInput->setPlaceholderText("Enter query string (must be substring of text)...");
    prefixInput->setEnabled(false);

    predictButton = new QPushButton("Predict", this);
    predictButton->setEnabled(false);
    predictButton->setStyleSheet(
        "QPushButton { background-color: #7C6DB0; color: white; font-weight: bold; border-radius: 5px; padding: 6px; }"
        "QPushButton:hover { background-color: #8E7FD1; }"
        "QPushButton:disabled { background-color: #555; color: #AAA; }"
        );
    connect(predictButton, &QPushButton::clicked, this, &PredictionWindow::predictCompletions);

    prefixLayout->addWidget(prefixInput, 3);
    prefixLayout->addWidget(predictButton, 1);
    mainLayout->addLayout(prefixLayout);

    QHBoxLayout *paramsLayout = new QHBoxLayout();
    QLabel *thresholdLabel = new QLabel("Maximum Predictions (X):", this);
    thresholdLabel->setStyleSheet("QLabel { font-size: 12px; }");

    thresholdInput = new QSpinBox(this);
    thresholdInput->setMinimum(1);
    thresholdInput->setMaximum(1000);
    thresholdInput->setValue(5);
    thresholdInput->setEnabled(false);
    thresholdInput->setToolTip("Predictions are shown when distinct substrings with matching prefix ≤ X");

    paramsLayout->addWidget(thresholdLabel);
    paramsLayout->addWidget(thresholdInput);
    paramsLayout->addStretch();
    mainLayout->addLayout(paramsLayout);

    resultText = new QTextEdit(this);
    resultText->setReadOnly(true);
    resultText->setMaximumHeight(120);
    mainLayout->addWidget(resultText);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(400);
    treeVisualizer = new TreeVisualizer(nullptr, "", this);
    scrollArea->setWidget(treeVisualizer);
    mainLayout->addWidget(scrollArea);

    backButton = new QPushButton("Back to Main Menu", this);
    backButton->setStyleSheet(
        "QPushButton { background-color: #B91C1C; color: #FFFFFF; font-weight: bold; border-radius: 5px; padding: 6px; }"
        "QPushButton:hover { background-color: #DC2626; }"
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
        thresholdInput->setEnabled(true);
        predictButton->setEnabled(true);
        resultText->setText("Suffix tree built successfully! Enter a query string to find minimum prefix index.");
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", QString("Failed to build tree: %1").arg(e.what()));
    }
}

void PredictionWindow::predictCompletions() {
    if (!tree) {
        QMessageBox::warning(this, "Error", "Please build a tree first.");
        return;
    }

    QString query = prefixInput->text();
    if (query.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a query string.");
        return;
    }

    int maxPredictions = thresholdInput->value();
    std::stringstream buffer;
    std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
    tree->predictCompletions(query.toStdString(), maxPredictions, maxPredictions);
    std::cout.rdbuf(old);

    resultText->setText(QString::fromStdString(buffer.str()));
}

void PredictionWindow::goBack() {
    if (parentWindow) parentWindow->show();
    this->close();
}
