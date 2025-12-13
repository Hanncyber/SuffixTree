#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openSearchWindow();
    void openMutationWindow();
    void openPatternWindow();
    void openPredictionWindow();
    void exitApplication();

private:
    void setupUI();
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QPushButton *searchButton;
    QPushButton *mutationButton;
    QPushButton *patternButton;
    QPushButton *predictionButton;
    QPushButton *exitButton;
};

#endif // MAINWINDOW_H
