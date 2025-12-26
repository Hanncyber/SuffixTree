#include "MainWindow.h"
#include "SearchWindow.h"
#include "MutationWindow.h"
#include "PatternWindow.h"
#include "PredictionWindow.h"
#include "EmployeeRatingWindow.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <QWidget>
#include <QList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setWindowTitle("Suffix Tree Applications");
    resize(620, 460);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(14);
    mainLayout->setContentsMargins(40, 35, 40, 35);

    this->setStyleSheet(R"(
        QWidget {
            background-color: #1A1A1A;   /* lighter dark background */
            font-family: Arial;
        }

        QLabel {
            color: #EDEDED;
        }

        QPushButton {
            background-color: #7C6DB0;   /* lilac */
            color: #FFFFFF;
            border: 1px solid #9F91D8;
            border-radius: 6px;
            padding: 12px;
            font-size: 15px;
            font-weight: 500;
        }

        QPushButton:hover {
            background-color: #8E7FD1;
        }

        QPushButton:pressed {
            background-color: #6B5AA6;
        }
    )");

    QLabel *titleLabel = new QLabel("Suffix Tree Applications", this);
    QFont titleFont;
    titleFont.setFamily("Verdana");
    titleFont.setPointSize(26);
    titleFont.setWeight(QFont::DemiBold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(20);
    QPushButton *searchBtn   = new QPushButton("Search Pattern", this);
    QPushButton *patternBtn  = new QPushButton("Longest Repeated Pattern", this);
    QPushButton *predictBtn  = new QPushButton("Predict Completions", this);
    QPushButton *employeeBtn = new QPushButton("Employee Rating System", this);
    QPushButton *exitBtn     = new QPushButton("Exit", this);

    QList<QPushButton*> buttons = {
        searchBtn, patternBtn, predictBtn, employeeBtn, exitBtn
    };

    for (QPushButton *btn : buttons) {
        btn->setMinimumHeight(48);
        btn->setCursor(Qt::PointingHandCursor);
        mainLayout->addWidget(btn);
    }
    exitBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #B91C1C;
            border: none;
            color: #FFFFFF;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #DC2626;
        }
        QPushButton:pressed {
            background-color: #991B1B;
        }
    )");

    mainLayout->addStretch();
    connect(searchBtn,   &QPushButton::clicked, this, &MainWindow::openSearchWindow);
    connect(patternBtn,  &QPushButton::clicked, this, &MainWindow::openPatternWindow);
    connect(predictBtn,  &QPushButton::clicked, this, &MainWindow::openPredictionWindow);
    connect(employeeBtn, &QPushButton::clicked, this, &MainWindow::openEmployeeRatingWindow);
    connect(exitBtn,     &QPushButton::clicked, this, &MainWindow::exitApplication);
}

void MainWindow::openSearchWindow()
{
    SearchWindow *w = new SearchWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    connect(w, &QWidget::destroyed, this, &QWidget::show);
    hide();
    w->show();
}

void MainWindow::openPatternWindow()
{
    PatternWindow *w = new PatternWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    connect(w, &QWidget::destroyed, this, &QWidget::show);
    hide();
    w->show();
}

void MainWindow::openPredictionWindow()
{
    PredictionWindow *w = new PredictionWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    connect(w, &QWidget::destroyed, this, &QWidget::show);
    hide();
    w->show();
}

void MainWindow::openEmployeeRatingWindow()
{
    EmployeeRatingWindow *w = new EmployeeRatingWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    connect(w, &QWidget::destroyed, this, &QWidget::show);
    hide();
    w->show();
}

void MainWindow::exitApplication()
{
    QApplication::quit();
}
