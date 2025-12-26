#include "MainWindow.h"
#include "SearchWindow.h"
#include "MutationWindow.h"
#include "PatternWindow.h"
#include "PredictionWindow.h"
#include "EmployeeRatingWindow.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <QWidget>
#include <QStackedWidget>
#include <QScrollArea>
#include <QTextEdit>
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), sidebarVisible(true)
{
    setupUI();
    setWindowTitle("Suffix Tree Applications");
    resize(1200, 700);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    this->setStyleSheet(R"(
        QWidget {
            background-color: #1A1A1A;
            font-family: Arial;
        }
        
        QLabel {
            color: #EDEDED;
        }
    )");

    createSidebar();
    createContentArea();

    toggleButton = new QPushButton("◀", centralWidget);
    toggleButton->setFixedSize(35, 50);
    toggleButton->setStyleSheet(R"(
        QPushButton {
            background-color: #3A3A3A;
            border: none;
            border-radius: 5px;
            color: #EDEDED;
            font-size: 18px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #4A4A4A;
        }
    )");
    toggleButton->setCursor(Qt::PointingHandCursor);
    toggleButton->move(220, 10);
    toggleButton->raise();

    connect(toggleButton, &QPushButton::clicked, this, &MainWindow::toggleSidebar);

    showHomeView();
}

void MainWindow::createSidebar()
{
    sidebar = new QWidget(this);
    sidebar->setFixedWidth(220);
    sidebar->setStyleSheet(R"(
        QWidget {
            background-color: #2A2A2A;
            border-right: 1px solid #3A3A3A;
        }
    )");

    sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setSpacing(5);
    sidebarLayout->setContentsMargins(10, 10, 10, 10);

    sidebarLayout->addSpacing(45);

    QString buttonStyle = R"(
        QPushButton {
            background-color: #3A3A3A;
            border: none;
            border-radius: 8px;
            color: #EDEDED;
            padding: 12px;
            text-align: left;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #7C6DB0;
        }
        QPushButton:pressed {
            background-color: #6B5AA6;
        }
    )";

    // Home button
    homeButton = new QPushButton("Home", sidebar);
    homeButton->setStyleSheet(buttonStyle);
    homeButton->setMinimumHeight(45);
    homeButton->setCursor(Qt::PointingHandCursor);
    sidebarLayout->addWidget(homeButton);

    sidebarLayout->addSpacing(10);

    // Navigation buttons
    searchButton = new QPushButton("Search Pattern", sidebar);
    searchButton->setStyleSheet(buttonStyle);
    searchButton->setMinimumHeight(45);
    searchButton->setCursor(Qt::PointingHandCursor);
    sidebarLayout->addWidget(searchButton);

    patternButton = new QPushButton("Longest Repeated", sidebar);
    patternButton->setStyleSheet(buttonStyle);
    patternButton->setMinimumHeight(45);
    patternButton->setCursor(Qt::PointingHandCursor);
    sidebarLayout->addWidget(patternButton);

    predictionButton = new QPushButton("Predict Completions", sidebar);
    predictionButton->setStyleSheet(buttonStyle);
    predictionButton->setMinimumHeight(45);
    predictionButton->setCursor(Qt::PointingHandCursor);
    sidebarLayout->addWidget(predictionButton);

    employeeRatingButton = new QPushButton("Employee Rating", sidebar);
    employeeRatingButton->setStyleSheet(buttonStyle);
    employeeRatingButton->setMinimumHeight(45);
    employeeRatingButton->setCursor(Qt::PointingHandCursor);
    sidebarLayout->addWidget(employeeRatingButton);

    sidebarLayout->addStretch();

    // Exit button at the bottom
    exitButton = new QPushButton("Exit", sidebar);
    exitButton->setStyleSheet(R"(
        QPushButton {
            background-color: #B91C1C;
            border: none;
            border-radius: 8px;
            color: #FFFFFF;
            padding: 12px;
            text-align: center;
            font-size: 14px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #DC2626;
        }
        QPushButton:pressed {
            background-color: #991B1B;
        }
    )");
    exitButton->setMinimumHeight(45);
    exitButton->setCursor(Qt::PointingHandCursor);
    sidebarLayout->addWidget(exitButton);

    mainLayout->addWidget(sidebar);

    connect(homeButton, &QPushButton::clicked, this, &MainWindow::showHomeView);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::showSearchView);
    connect(patternButton, &QPushButton::clicked, this, &MainWindow::showPatternView);
    connect(predictionButton, &QPushButton::clicked, this, &MainWindow::showPredictionView);
    connect(employeeRatingButton, &QPushButton::clicked, this, &MainWindow::showEmployeeRatingView);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::exitApplication);
}

void MainWindow::createContentArea()
{
    contentStack = new QStackedWidget(this);
    contentStack->setStyleSheet("background-color: #1A1A1A;");
    
    homeWidget = createHomeWidget();
    contentStack->addWidget(homeWidget);

    searchWindow = new SearchWindow();
    contentStack->addWidget(searchWindow);

    patternWindow = new PatternWindow();
    contentStack->addWidget(patternWindow);

    predictionWindow = new PredictionWindow();
    contentStack->addWidget(predictionWindow);

    employeeRatingWindow = new EmployeeRatingWindow();
    contentStack->addWidget(employeeRatingWindow);

    mainLayout->addWidget(contentStack);
}

QWidget* MainWindow::createHomeWidget()
{
    QWidget *home = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(home);
    layout->setContentsMargins(40, 30, 40, 30);
    layout->setSpacing(20);

    QLabel *imageLabel = new QLabel(home);
    imageLabel->setFixedSize(200, 200);

    QPixmap pix("D:/g712.png");   // or "C:/path/to/image.png"
    imageLabel->setPixmap(pix);
    
    QHBoxLayout *imageLayout = new QHBoxLayout();
    imageLayout->addStretch();
    imageLayout->addWidget(imageLabel);
    imageLayout->addStretch();
    layout->addLayout(imageLayout);

    layout->addSpacing(20);

    QLabel *titleLabel = new QLabel("Welcome to Suffix Tree Applications", home);
    QFont titleFont;
    titleFont.setFamily("Verdana");
    titleFont.setPointSize(24);
    titleFont.setWeight(QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #EDEDED;");
    layout->addWidget(titleLabel);

    layout->addSpacing(10);

    QTextEdit *textEdit = new QTextEdit(home);
    textEdit->setReadOnly(true);
    textEdit->setStyleSheet(R"(
        QTextEdit {
            background-color: #2A2A2A;
            border: 1px solid #3A3A3A;
            border-radius: 10px;
            color: #EDEDED;
            padding: 20px;
            font-size: 14px;
            line-height: 1.6;
        }
    )");
    
    QString explanation = R"(
<h2 style="color: #7C6DB0;">What is a Suffix Tree?</h2>

<p>A suffix tree is a compressed trie containing all the suffixes of a given text as their keys and positions in the text as their values. It is a powerful data structure that enables efficient string operations.</p>

<h3 style="color: #8E7FD1;">Key Features:</h3>
<ul>
    <li><b>Fast Pattern Matching:</b> Find all occurrences of a pattern in O(m) time where m is the pattern length</li>
    <li><b>Longest Repeated Substring:</b> Identify the longest substring that appears more than once</li>
    <li><b>Auto-completion:</b> Generate predictions based on prefix matching</li>
</ul>

<h3 style="color: #8E7FD1;">Applications:</h3>
<p>Suffix trees are used in text compression, data mining, and many other fields requiring efficient string processing.</p>

<p style="margin-top: 20px; font-style: italic; color: #9F91D8;">Use the sidebar navigation to explore different suffix tree operations.</p>
    )";
    
    textEdit->setHtml(explanation);
    layout->addWidget(textEdit);

    return home;
}

void MainWindow::toggleSidebar()
{
    if (sidebarVisible) {
        sidebar->setVisible(false);
        toggleButton->setText("▶");
        toggleButton->move(10, 10);
        sidebarVisible = false;
    } else {
        sidebar->setVisible(true);
        toggleButton->setText("◀");
        toggleButton->move(220, 10);
        sidebarVisible = true;
    }
}

void MainWindow::showHomeView()
{
    contentStack->setCurrentWidget(homeWidget);
}

void MainWindow::showSearchView()
{
    contentStack->setCurrentWidget(searchWindow);
}

void MainWindow::showPatternView()
{
    contentStack->setCurrentWidget(patternWindow);
}

void MainWindow::showPredictionView()
{
    contentStack->setCurrentWidget(predictionWindow);
}

void MainWindow::showEmployeeRatingView()
{
    contentStack->setCurrentWidget(employeeRatingWindow);
}

void MainWindow::exitApplication()
{
    QApplication::quit();
}
