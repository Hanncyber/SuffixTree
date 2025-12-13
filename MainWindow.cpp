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

MainWindow::~MainWindow() {
}

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
    titleLabel->setStyleSheet("QLabel { color: #2C3E50; margin-bottom: 20px; }");
    mainLayout->addWidget(titleLabel);

    // Search Pattern Button
    searchButton = new QPushButton("Search Pattern", this);
    searchButton->setMinimumHeight(60);
    searchButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498DB;"
        "   color: white;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980B9;"
        "}"
    );
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::openSearchWindow);
    mainLayout->addWidget(searchButton);

    // DNA Mutation Detection Button
    mutationButton = new QPushButton("DNA Mutation Detection", this);
    mutationButton->setMinimumHeight(60);
    mutationButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #E74C3C;"
        "   color: white;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #C0392B;"
        "}"
    );
    connect(mutationButton, &QPushButton::clicked, this, &MainWindow::openMutationWindow);
    mainLayout->addWidget(mutationButton);

    // Longest Repeated Pattern Button
    patternButton = new QPushButton("Longest Repeated Pattern", this);
    patternButton->setMinimumHeight(60);
    patternButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #2ECC71;"
        "   color: white;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #27AE60;"
        "}"
    );
    connect(patternButton, &QPushButton::clicked, this, &MainWindow::openPatternWindow);
    mainLayout->addWidget(patternButton);

    // Prediction Completions Button
    predictionButton = new QPushButton("Predict Completions", this);
    predictionButton->setMinimumHeight(60);
    predictionButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #F39C12;"
        "   color: white;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #E67E22;"
        "}"
    );
    connect(predictionButton, &QPushButton::clicked, this, &MainWindow::openPredictionWindow);
    mainLayout->addWidget(predictionButton);

    // Exit Button
    exitButton = new QPushButton("Exit", this);
    exitButton->setMinimumHeight(50);
    exitButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #95A5A6;"
        "   color: white;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #7F8C8D;"
        "}"
    );
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::exitApplication);
    mainLayout->addWidget(exitButton);

    mainLayout->addStretch();
}

void MainWindow::openSearchWindow() {
    SearchWindow *searchWindow = new SearchWindow(this);
    searchWindow->show();
    this->hide();
}

void MainWindow::openMutationWindow() {
    MutationWindow *mutationWindow = new MutationWindow(this);
    mutationWindow->show();
    this->hide();
}

void MainWindow::openPatternWindow() {
    PatternWindow *patternWindow = new PatternWindow(this);
    patternWindow->show();
    this->hide();
}

void MainWindow::openPredictionWindow() {
    PredictionWindow *predictionWindow = new PredictionWindow(this);
    predictionWindow->show();
    this->hide();
}

void MainWindow::exitApplication() {
    QApplication::quit();
}
