#include "MainWindow.h"
#include "SearchWindow.h"
#include "MutationWindow.h"
#include "PatternWindow.h"
#include "PredictionWindow.h"
#include "EmployeeRatingWindow.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <QWidget>
#include <QList>
#include <QStringList>
#include <QPixmap>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setWindowTitle("Suffix Tree Applications");
    resize(800, 600);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 30, 40, 30);

    this->setStyleSheet(R"(
        QWidget {
            background-color: #1A1A1A;
            font-family: Arial;
        }

        QLabel {
            color: #EDEDED;
        }

        QPushButton {
            background-color: #7C6DB0;
            color: #FFFFFF;
            border: none;
            font-size: 14px;
            font-weight: 500;
        }

        QPushButton:hover {
            background-color: #8E7FD1;
        }

        QPushButton:pressed {
            background-color: #6B5AA6;
        }
    )");

    // Header layout with logo and title
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(15);
    
    // Logo placeholder
    QLabel *logoLabel = new QLabel(this);
    logoLabel->setFixedSize(80, 80);
    logoLabel->setStyleSheet(R"(
        QLabel {
            background-color: #7C6DB0;
            border: 2px solid #9F91D8;
            border-radius: 10px;
        }
    )");
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setText("LOGO");
    logoLabel->setFont(QFont("Arial", 10, QFont::Bold));
    
    headerLayout->addWidget(logoLabel);
    
    // Title label
    QLabel *titleLabel = new QLabel("Suffix Tree apps", this);
    QFont titleFont;
    titleFont.setFamily("Verdana");
    titleFont.setPointSize(28);
    titleFont.setWeight(QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    
    mainLayout->addLayout(headerLayout);
    mainLayout->addSpacing(30);

    // 4 Round buttons in a 2x2 grid
    QGridLayout *buttonsLayout = new QGridLayout();
    buttonsLayout->setSpacing(25);
    buttonsLayout->setHorizontalSpacing(40);
    buttonsLayout->setVerticalSpacing(40);
    
    QPushButton *searchBtn = new QPushButton(this);
    QPushButton *patternBtn = new QPushButton(this);
    QPushButton *predictBtn = new QPushButton(this);
    QPushButton *employeeBtn = new QPushButton(this);
    
    QList<QPushButton*> buttons = {searchBtn, patternBtn, predictBtn, employeeBtn};
    QStringList buttonTexts = {"Search\nPattern", "Longest\nRepeated", "Predict\nCompletion", "Employee\nRating"};
    QStringList buttonIcons = {"🔍", "🔄", "💡", "👥"};
    
    // Style all buttons as circles with icons
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i]->setFixedSize(150, 150);
        buttons[i]->setCursor(Qt::PointingHandCursor);
        buttons[i]->setStyleSheet(R"(
            QPushButton {
                background-color: #7C6DB0;
                border: 3px solid #9F91D8;
                border-radius: 75px;
                color: #FFFFFF;
                font-size: 13px;
                font-weight: 600;
            }
            QPushButton:hover {
                background-color: #8E7FD1;
                border: 3px solid #B5A7E8;
            }
            QPushButton:pressed {
                background-color: #6B5AA6;
            }
        )");
        
        // Create text with icon
        QString buttonLabel = buttonIcons[i] + "\n" + buttonTexts[i];
        buttons[i]->setText(buttonLabel);
        buttons[i]->setFont(QFont("Arial", 12, QFont::DemiBold));
    }
    
    // Add buttons to grid (2x2)
    buttonsLayout->addWidget(searchBtn, 0, 0, Qt::AlignCenter);
    buttonsLayout->addWidget(patternBtn, 0, 1, Qt::AlignCenter);
    buttonsLayout->addWidget(predictBtn, 1, 0, Qt::AlignCenter);
    buttonsLayout->addWidget(employeeBtn, 1, 1, Qt::AlignCenter);
    
    // Center the grid layout
    QHBoxLayout *buttonsCenterLayout = new QHBoxLayout();
    buttonsCenterLayout->addStretch();
    buttonsCenterLayout->addLayout(buttonsLayout);
    buttonsCenterLayout->addStretch();
    
    mainLayout->addLayout(buttonsCenterLayout);
    mainLayout->addStretch();
    
    // Exit button at bottom right
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    
    QPushButton *exitBtn = new QPushButton("Exit", this);
    exitBtn->setFixedSize(120, 45);
    exitBtn->setCursor(Qt::PointingHandCursor);
    exitBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #B91C1C;
            border: none;
            border-radius: 8px;
            color: #FFFFFF;
            font-weight: 600;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #DC2626;
        }
        QPushButton:pressed {
            background-color: #991B1B;
        }
    )");
    
    bottomLayout->addWidget(exitBtn);
    mainLayout->addLayout(bottomLayout);
    
    // Connect signals
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::openSearchWindow);
    connect(patternBtn, &QPushButton::clicked, this, &MainWindow::openPatternWindow);
    connect(predictBtn, &QPushButton::clicked, this, &MainWindow::openPredictionWindow);
    connect(employeeBtn, &QPushButton::clicked, this, &MainWindow::openEmployeeRatingWindow);
    connect(exitBtn, &QPushButton::clicked, this, &MainWindow::exitApplication);
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
