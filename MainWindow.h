#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QStackedWidget>
#include <QScrollArea>
#include <QPropertyAnimation>

// Forward declarations
class SearchWindow;
class PatternWindow;
class PredictionWindow;
class EmployeeRatingWindow;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleSidebar();
    void showHomeView();
    void showSearchView();
    void showPatternView();
    void showPredictionView();
    void showEmployeeRatingView();
    void exitApplication();

private:
    void setupUI();
    void createSidebar();
    void createContentArea();
    QWidget* createHomeWidget();
    
    // Main layout components
    QWidget *centralWidget;
    QHBoxLayout *mainLayout;
    
    // Sidebar components
    QWidget *sidebar;
    QVBoxLayout *sidebarLayout;
    QPushButton *toggleButton;
    QPushButton *homeButton;
    QPushButton *searchButton;
    QPushButton *patternButton;
    QPushButton *predictionButton;
    QPushButton *employeeRatingButton;
    QPushButton *exitButton;
    bool sidebarVisible;
    
    // Content area
    QStackedWidget *contentStack;
    QWidget *homeWidget;
    SearchWindow *searchWindow;
    PatternWindow *patternWindow;
    PredictionWindow *predictionWindow;
    EmployeeRatingWindow *employeeRatingWindow;
};

#endif // MAINWINDOW_H
