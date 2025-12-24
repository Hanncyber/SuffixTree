#ifndef EMPLOYEERATINGWINDOW_H
#define EMPLOYEERATINGWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include "employeerating.h"

class EmployeeRatingWindow : public QWidget {
    Q_OBJECT

public:
    explicit EmployeeRatingWindow(QWidget *parent = nullptr);
    ~EmployeeRatingWindow();

private slots:
    void initializeCompany();
    void addSubordinateToHierarchy();
    void setEmployeeRating();
    void buildHierarchy();
    void performUpdate();
    void performQuery();
    void goBack();

private:
    void setupUI();
    void enableHierarchyInputs(bool enable);
    void enableQueryInputs(bool enable);
    bool isValidEmployeeChar(char c);
    
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    
    // Company initialization
    QSpinBox *numEmployeesInput;
    QPushButton *initButton;
    
    // Hierarchy building
    QLineEdit *managerInput;
    QLineEdit *subordinateInput;
    QPushButton *addSubordinateButton;
    
    QLineEdit *employeeRatingInput;
    QSpinBox *ratingValueInput;
    QPushButton *setRatingButton;
    
    QPushButton *buildButton;
    
    // Query inputs
    QLineEdit *updateEmployeeInput;
    QSpinBox *updateValueInput;
    QPushButton *updateButton;
    
    QLineEdit *queryEmployeeInput;
    QPushButton *queryButton;
    
    // Results
    QTextEdit *resultText;
    QPushButton *backButton;
    
    EmployeeRating *empRating;
    int numEmployees;
    bool isInitialized;
    bool isBuilt;
};

#endif // EMPLOYEERATINGWINDOW_H
