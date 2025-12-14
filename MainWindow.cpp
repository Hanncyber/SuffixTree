#include "MainWindow.h"
#include "SearchWindow.h"
#include "MutationWindow.h"
#include "PatternWindow.h"
#include "PredictionWindow.h"
#include <QFont>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUI();
    setWindowTitle("Suffix Tree Application");
    resize(600, 500);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(50, 50, 50, 50);

    // Title
    titleLabel = new QLabel("Suffix Tree Application", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    searchButton = new QPushButton("Search Pattern", this);
    mutationButton = new QPushButton("DNA Mutation Detection", this);
    patternButton = new QPushButton("Longest Repeated Pattern", this);
    predictionButton = new QPushButton("Predict Completions", this);
    exitButton = new QPushButton("Exit", this);

    QList<QPushButton*> buttons = {
        searchButton, mutationButton, patternButton, predictionButton
    };

    for (auto *btn : buttons) {
        btn->setMinimumHeight(60);
        btn->setStyleSheet(
            "QPushButton { font-size: 16px; font-weight: bold; border-radius: 10px; }"
        );
        mainLayout->addWidget(btn);
    }

    connect(searchButton, &QPushButton::clicked, this, &MainWindow::openSearchWindow);
    connect(mutationButton, &QPushButton::clicked, this, &MainWindow::openMutationWindow);
    connect(patternButton, &QPushButton::clicked, this, &MainWindow::openPatternWindow);
    connect(predictionButton, &QPushButton::clicked, this, &MainWindow::openPredictionWindow);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::exitApplication);

    mainLayout->addWidget(exitButton);
    mainLayout->addStretch();
}

void MainWindow::openSearchWindow() {
    auto *w = new SearchWindow(this);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWindow::openMutationWindow() {
    auto *w = new MutationWindow(this);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWindow::openPatternWindow() {
    auto *w = new PatternWindow(this);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWindow::openPredictionWindow() {
    auto *w = new PredictionWindow(this);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWindow::exitApplication() {
    QApplication::quit();
}
