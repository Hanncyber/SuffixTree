#include "MainWindow.h"
#include "SearchWindow.h"
#include "MutationWindow.h"
#include "PatternWindow.h"
#include "PredictionWindow.h"
#include <QFont>
#include <QApplication>
#include <QCloseEvent>

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

    // Buttons
    createButton("Search Pattern", "#3498DB", "#2980B9", &MainWindow::openSearchWindow);
    createButton("DNA Mutation Detection", "#E74C3C", "#C0392B", &MainWindow::openMutationWindow);
    createButton("Longest Repeated Pattern", "#2ECC71", "#27AE60", &MainWindow::openPatternWindow);
    createButton("Predict Completions", "#F39C12", "#E67E22", &MainWindow::openPredictionWindow);

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
        "} "
        "QPushButton:hover {"
        "   background-color: #7F8C8D;"
        "}"
    );
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::exitApplication);
    mainLayout->addWidget(exitButton);

    mainLayout->addStretch();
}

// Helper function to create buttons
void MainWindow::createButton(const QString& text, const QString& color, const QString& hoverColor, void (MainWindow::*slot)()) {
    QPushButton* button = new QPushButton(text, this);
    button->setMinimumHeight(60);
    button->setStyleSheet(
        QString("QPushButton {"
                "   background-color: %1;"
                "   color: white;"
                "   font-size: 16px;"
                "   font-weight: bold;"
                "   border-radius: 10px;"
                "   padding: 10px;"
                "} "
                "QPushButton:hover {"
                "   background-color: %2;"
                "}")
            .arg(color)
            .arg(hoverColor)
    );
    connect(button, &QPushButton::clicked, this, slot);
    mainLayout->addWidget(button);
}

// Open windows with independent lifetime
void MainWindow::openSearchWindow() {
    openChildWindow<SearchWindow>();
}

void MainWindow::openMutationWindow() {
    openChildWindow<MutationWindow>();
}

void MainWindow::openPatternWindow() {
    openChildWindow<PatternWindow>();
}

void MainWindow::openPredictionWindow() {
    openChildWindow<PredictionWindow>();
}

// Template to open any child window
template <typename T>
void MainWindow::openChildWindow() {
    T* childWindow = new T(); // no parent
    childWindow->setAttribute(Qt::WA_DeleteOnClose); // auto-delete when closed
    childWindow->show();
    this->hide();

    // Restore main window when child closes
    connect(childWindow, &QWidget::destroyed, [this]() {
        this->show();
    });
}

void MainWindow::exitApplication() {
    QApplication::quit();
}
