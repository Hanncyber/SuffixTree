#include "MainWindow.h"
#include "SearchWindow.h"
#include "MutationWindow.h"
#include "PatternWindow.h"
#include "PredictionWindow.h"
#include "EmployeeRatingWindow.h"
#include <QFont>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUI();
    setWindowTitle("Suffix Tree Application");
    resize(600, 450);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // ---------- Title ----------
    QLabel *titleLabel = new QLabel("Suffix Tree Application", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // ---------- Buttons ----------
    struct BtnInfo { QString text; QString color; std::function<void()> slot; };
    QList<BtnInfo> buttons = {
        { "Search Pattern", "#3498DB", [this]() { openSearchWindow(); } },
        { "Longest Repeated Pattern", "#F39C12", [this]() { openPatternWindow(); } },
        { "Predict Completions", "#2ECC71", [this]() { openPredictionWindow(); } },
        { "Employee Rating System", "#9B59B6", [this]() { openEmployeeRatingWindow(); } },
        { "Exit", "#95A5A6", [this]() { exitApplication(); } }
    };

    for (auto &info : buttons) {
        QPushButton *btn = new QPushButton(info.text, this);
        btn->setMinimumHeight(50);

        QColor color(info.color);                 // convert string to QColor
        QColor hoverColor = color.darker(120);   // darker version for hover

        btn->setStyleSheet(QString(
                               "QPushButton {"
                               "   background-color: %1;"
                               "   color: white;"
                               "   font-size: 16px;"
                               "   font-weight: bold;"
                               "   border-radius: 8px;"
                               "}"
                               "QPushButton:hover { background-color: %2; }"
                               ).arg(color.name(), hoverColor.name()));

        mainLayout->addWidget(btn);
        connect(btn, &QPushButton::clicked, info.slot);
    }


    mainLayout->addStretch();
}

void MainWindow::openSearchWindow() {
    SearchWindow *w = new SearchWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    connect(w, &QWidget::destroyed, this, &QWidget::show);
    hide();
    w->show();
}

void MainWindow::openMutationWindow() {
    MutationWindow *w = new MutationWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    connect(w, &QWidget::destroyed, this, &QWidget::show);
    hide();
    w->show();
}

void MainWindow::openPatternWindow() {
    PatternWindow *w = new PatternWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    connect(w, &QWidget::destroyed, this, &QWidget::show);
    hide();
    w->show();
}

void MainWindow::openPredictionWindow() {
    PredictionWindow *w = new PredictionWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    connect(w, &QWidget::destroyed, this, &QWidget::show);
    hide();
    w->show();
}

void MainWindow::openEmployeeRatingWindow() {
    EmployeeRatingWindow *w = new EmployeeRatingWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    connect(w, &QWidget::destroyed, this, &QWidget::show);
    hide();
    w->show();
}

void MainWindow::exitApplication() {
    QApplication::quit();
}
