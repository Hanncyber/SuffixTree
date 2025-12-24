#include "EmployeeRatingWindow.h"
#include <QMessageBox>
#include <QFont>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <sstream>

EmployeeRatingWindow::EmployeeRatingWindow(QWidget *parent)
    : QWidget(parent), empRating(nullptr), numEmployees(0), isInitialized(false), isBuilt(false) {
    setupUI();
    setWindowTitle("Employee Rating System");
    resize(900, 700);
}

EmployeeRatingWindow::~EmployeeRatingWindow() {
    if (empRating) delete empRating;
}

void EmployeeRatingWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    // ---------- Title ----------
    QLabel *title = new QLabel("Employee Rating & Performance System", this);
    QFont titleFont = title->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #9B59B6;");
    mainLayout->addWidget(title);

    // ---------- Instructions ----------
    QLabel *instructions = new QLabel(
        "This system manages employee hierarchy and ratings.\n"
        "Step 1: Initialize company with number of employees (H is root, then A, B, C, ...)\n"
        "Step 2: Add subordinates and set initial ratings\n"
        "Step 3: Build the hierarchy\n"
        "Step 4: Perform updates (Type 0) or queries (Type 1)",
        this);
    instructions->setStyleSheet("QLabel { font-size: 11px; color: #7F8C8D; padding: 5px; }");
    instructions->setWordWrap(true);
    mainLayout->addWidget(instructions);

    // ---------- Company Initialization Section ----------
    QGroupBox *initGroup = new QGroupBox("Step 1: Initialize Company", this);
    QHBoxLayout *initLayout = new QHBoxLayout();
    
    QLabel *numEmpLabel = new QLabel("Number of employees (including head H):", this);
    numEmployeesInput = new QSpinBox(this);
    numEmployeesInput->setRange(1, 26);
    numEmployeesInput->setValue(5);
    numEmployeesInput->setStyleSheet("QSpinBox { font-size: 13px; padding: 4px; }");
    
    initButton = new QPushButton("Initialize", this);
    initButton->setStyleSheet(
        "QPushButton { background-color: #9B59B6; color: white; font-weight: bold; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: #8E44AD; }"
        );
    connect(initButton, &QPushButton::clicked, this, &EmployeeRatingWindow::initializeCompany);
    
    initLayout->addWidget(numEmpLabel);
    initLayout->addWidget(numEmployeesInput);
    initLayout->addWidget(initButton);
    initGroup->setLayout(initLayout);
    mainLayout->addWidget(initGroup);

    // ---------- Hierarchy Building Section ----------
    QGroupBox *hierarchyGroup = new QGroupBox("Step 2: Build Hierarchy & Set Ratings", this);
    QVBoxLayout *hierarchyLayout = new QVBoxLayout();
    
    // Add subordinate
    QHBoxLayout *subordinateLayout = new QHBoxLayout();
    QLabel *managerLabel = new QLabel("Manager:", this);
    managerInput = new QLineEdit(this);
    managerInput->setPlaceholderText("e.g., H");
    managerInput->setMaxLength(1);
    managerInput->setStyleSheet("QLineEdit { font-size: 13px; padding: 4px; }");
    managerInput->setEnabled(false);
    
    QLabel *subordinateLabel = new QLabel("Subordinate:", this);
    subordinateInput = new QLineEdit(this);
    subordinateInput->setPlaceholderText("e.g., A");
    subordinateInput->setMaxLength(1);
    subordinateInput->setStyleSheet("QLineEdit { font-size: 13px; padding: 4px; }");
    subordinateInput->setEnabled(false);
    
    addSubordinateButton = new QPushButton("Add Subordinate", this);
    addSubordinateButton->setStyleSheet(
        "QPushButton { background-color: #3498DB; color: white; font-weight: bold; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: #2980B9; }"
        "QPushButton:disabled { background-color: #BDC3C7; }"
        );
    addSubordinateButton->setEnabled(false);
    connect(addSubordinateButton, &QPushButton::clicked, this, &EmployeeRatingWindow::addSubordinateToHierarchy);
    
    subordinateLayout->addWidget(managerLabel);
    subordinateLayout->addWidget(managerInput);
    subordinateLayout->addWidget(subordinateLabel);
    subordinateLayout->addWidget(subordinateInput);
    subordinateLayout->addWidget(addSubordinateButton);
    hierarchyLayout->addLayout(subordinateLayout);
    
    // Set rating
    QHBoxLayout *ratingLayout = new QHBoxLayout();
    QLabel *empRatingLabel = new QLabel("Employee:", this);
    employeeRatingInput = new QLineEdit(this);
    employeeRatingInput->setPlaceholderText("e.g., H");
    employeeRatingInput->setMaxLength(1);
    employeeRatingInput->setStyleSheet("QLineEdit { font-size: 13px; padding: 4px; }");
    employeeRatingInput->setEnabled(false);
    
    QLabel *ratingLabel = new QLabel("Rating:", this);
    ratingValueInput = new QSpinBox(this);
    ratingValueInput->setRange(1, 100);
    ratingValueInput->setValue(10);
    ratingValueInput->setStyleSheet("QSpinBox { font-size: 13px; padding: 4px; }");
    ratingValueInput->setEnabled(false);
    
    setRatingButton = new QPushButton("Set Rating", this);
    setRatingButton->setStyleSheet(
        "QPushButton { background-color: #2ECC71; color: white; font-weight: bold; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: #27AE60; }"
        "QPushButton:disabled { background-color: #BDC3C7; }"
        );
    setRatingButton->setEnabled(false);
    connect(setRatingButton, &QPushButton::clicked, this, &EmployeeRatingWindow::setEmployeeRating);
    
    ratingLayout->addWidget(empRatingLabel);
    ratingLayout->addWidget(employeeRatingInput);
    ratingLayout->addWidget(ratingLabel);
    ratingLayout->addWidget(ratingValueInput);
    ratingLayout->addWidget(setRatingButton);
    hierarchyLayout->addLayout(ratingLayout);
    
    hierarchyGroup->setLayout(hierarchyLayout);
    mainLayout->addWidget(hierarchyGroup);

    // ---------- Build Button ----------
    buildButton = new QPushButton("Step 3: Build Hierarchy Tree", this);
    buildButton->setStyleSheet(
        "QPushButton { background-color: #E67E22; color: white; font-weight: bold; border-radius: 5px; padding: 10px; font-size: 14px; }"
        "QPushButton:hover { background-color: #D35400; }"
        "QPushButton:disabled { background-color: #BDC3C7; }"
        );
    buildButton->setEnabled(false);
    connect(buildButton, &QPushButton::clicked, this, &EmployeeRatingWindow::buildHierarchy);
    mainLayout->addWidget(buildButton);

    // ---------- Query Section ----------
    QGroupBox *queryGroup = new QGroupBox("Step 4: Perform Operations", this);
    QVBoxLayout *queryLayout = new QVBoxLayout();
    
    // Type 0: Update
    QHBoxLayout *updateLayout = new QHBoxLayout();
    QLabel *updateLabel = new QLabel("Type 0 - Update Subtree:", this);
    updateLabel->setStyleSheet("font-weight: bold;");
    queryLayout->addWidget(updateLabel);
    
    QLabel *updateEmpLabel = new QLabel("Employee:", this);
    updateEmployeeInput = new QLineEdit(this);
    updateEmployeeInput->setPlaceholderText("e.g., A");
    updateEmployeeInput->setMaxLength(1);
    updateEmployeeInput->setStyleSheet("QLineEdit { font-size: 13px; padding: 4px; }");
    updateEmployeeInput->setEnabled(false);
    
    QLabel *updateValLabel = new QLabel("Update by:", this);
    updateValueInput = new QSpinBox(this);
    updateValueInput->setRange(-100, 100);
    updateValueInput->setValue(5);
    updateValueInput->setStyleSheet("QSpinBox { font-size: 13px; padding: 4px; }");
    updateValueInput->setEnabled(false);
    
    updateButton = new QPushButton("Update Subtree", this);
    updateButton->setStyleSheet(
        "QPushButton { background-color: #F39C12; color: white; font-weight: bold; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: #E67E22; }"
        "QPushButton:disabled { background-color: #BDC3C7; }"
        );
    updateButton->setEnabled(false);
    connect(updateButton, &QPushButton::clicked, this, &EmployeeRatingWindow::performUpdate);
    
    updateLayout->addWidget(updateEmpLabel);
    updateLayout->addWidget(updateEmployeeInput);
    updateLayout->addWidget(updateValLabel);
    updateLayout->addWidget(updateValueInput);
    updateLayout->addWidget(updateButton);
    queryLayout->addLayout(updateLayout);
    
    // Type 1: Query
    QHBoxLayout *queryPerformanceLayout = new QHBoxLayout();
    QLabel *queryLabel = new QLabel("Type 1 - Query Performance (GCD):", this);
    queryLabel->setStyleSheet("font-weight: bold;");
    queryLayout->addWidget(queryLabel);
    
    QLabel *queryEmpLabel = new QLabel("Employee:", this);
    queryEmployeeInput = new QLineEdit(this);
    queryEmployeeInput->setPlaceholderText("e.g., H");
    queryEmployeeInput->setMaxLength(1);
    queryEmployeeInput->setStyleSheet("QLineEdit { font-size: 13px; padding: 4px; }");
    queryEmployeeInput->setEnabled(false);
    
    queryButton = new QPushButton("Query Performance", this);
    queryButton->setStyleSheet(
        "QPushButton { background-color: #1ABC9C; color: white; font-weight: bold; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: #16A085; }"
        "QPushButton:disabled { background-color: #BDC3C7; }"
        );
    queryButton->setEnabled(false);
    connect(queryButton, &QPushButton::clicked, this, &EmployeeRatingWindow::performQuery);
    
    queryPerformanceLayout->addWidget(queryEmpLabel);
    queryPerformanceLayout->addWidget(queryEmployeeInput);
    queryPerformanceLayout->addWidget(queryButton);
    queryPerformanceLayout->addStretch();
    queryLayout->addLayout(queryPerformanceLayout);
    
    queryGroup->setLayout(queryLayout);
    mainLayout->addWidget(queryGroup);

    // ---------- Result display ----------
    resultText = new QTextEdit(this);
    resultText->setReadOnly(true);
    resultText->setStyleSheet(
        "QTextEdit { font-size: 13px; background-color: #2C3E50; color: #ECF0F1; border: 1px solid #34495E; }"
        );
    resultText->setMaximumHeight(100);
    mainLayout->addWidget(resultText);

    // ---------- Tree visualizer ----------
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(350);
    scrollArea->setStyleSheet("QScrollArea { border: 1px solid #34495E; }");
    treeVisualizer = new EmployeeTreeVisualizer(this);
    scrollArea->setWidget(treeVisualizer);
    mainLayout->addWidget(scrollArea);

    // ---------- Back button ----------
    backButton = new QPushButton("Back to Main Menu", this);
    backButton->setStyleSheet(
        "QPushButton { background-color: #95A5A6; color: white; font-weight: bold; border-radius: 5px; padding: 8px; }"
        "QPushButton:hover { background-color: #7F8C8D; }"
        );
    connect(backButton, &QPushButton::clicked, this, &EmployeeRatingWindow::goBack);
    mainLayout->addWidget(backButton);
}

void EmployeeRatingWindow::initializeCompany() {
    numEmployees = numEmployeesInput->value();
    
    if (empRating) {
        delete empRating;
    }
    
    empRating = new EmployeeRating(numEmployees);
    isInitialized = true;
    isBuilt = false;
    
    // Enable hierarchy inputs
    enableHierarchyInputs(true);
    
    // Disable initialization controls
    numEmployeesInput->setEnabled(false);
    initButton->setEnabled(false);
    
    // Update tree visualizer with initial data
    treeVisualizer->setEmployeeData(empRating->getNumEmployees(), 
                                     empRating->getTree(), 
                                     empRating->getChildCount(), 
                                     empRating->getRating());
    
    resultText->clear();
    resultText->append(QString("✓ Company initialized with %1 employees.").arg(numEmployees));
    resultText->append("Employee identifiers: H (head), then A, B, C, ...");
    resultText->append("Now add subordinates and set initial ratings, then build the hierarchy.");
}

void EmployeeRatingWindow::enableHierarchyInputs(bool enable) {
    managerInput->setEnabled(enable);
    subordinateInput->setEnabled(enable);
    addSubordinateButton->setEnabled(enable);
    employeeRatingInput->setEnabled(enable);
    ratingValueInput->setEnabled(enable);
    setRatingButton->setEnabled(enable);
    buildButton->setEnabled(enable);
}

void EmployeeRatingWindow::enableQueryInputs(bool enable) {
    updateEmployeeInput->setEnabled(enable);
    updateValueInput->setEnabled(enable);
    updateButton->setEnabled(enable);
    queryEmployeeInput->setEnabled(enable);
    queryButton->setEnabled(enable);
}

bool EmployeeRatingWindow::isValidEmployeeChar(char c) {
    return (c == 'H' || (c >= 'A' && c < 'A' + numEmployees - 1));
}

void EmployeeRatingWindow::addSubordinateToHierarchy() {
    if (!isInitialized) {
        QMessageBox::warning(this, "Error", "Please initialize the company first.");
        return;
    }
    
    QString managerStr = managerInput->text().toUpper().trimmed();
    QString subordinateStr = subordinateInput->text().toUpper().trimmed();
    
    if (managerStr.isEmpty() || subordinateStr.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter both manager and subordinate.");
        return;
    }
    
    char manager = managerStr[0].toLatin1();
    char subordinate = subordinateStr[0].toLatin1();
    
    // Validate characters
    if (!isValidEmployeeChar(manager)) {
        QMessageBox::warning(this, "Error", "Invalid manager character.");
        return;
    }
    if (!isValidEmployeeChar(subordinate)) {
        QMessageBox::warning(this, "Error", "Invalid subordinate character.");
        return;
    }
    
    empRating->addSubordinate(manager, subordinate);
    
    // Update tree visualization
    treeVisualizer->setEmployeeData(empRating->getNumEmployees(), 
                                     empRating->getTree(), 
                                     empRating->getChildCount(), 
                                     empRating->getRating());
    
    resultText->append(QString("✓ Added %1 as subordinate of %2")
                      .arg(subordinateStr)
                      .arg(managerStr));
    
    managerInput->clear();
    subordinateInput->clear();
}

void EmployeeRatingWindow::setEmployeeRating() {
    if (!isInitialized) {
        QMessageBox::warning(this, "Error", "Please initialize the company first.");
        return;
    }
    
    QString empStr = employeeRatingInput->text().toUpper().trimmed();
    
    if (empStr.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter an employee.");
        return;
    }
    
    char employee = empStr[0].toLatin1();
    int rating = ratingValueInput->value();
    
    // Validate character
    if (!isValidEmployeeChar(employee)) {
        QMessageBox::warning(this, "Error", "Invalid employee character.");
        return;
    }
    
    empRating->setInitialRating(employee, rating);
    
    // Update tree visualization with new rating
    treeVisualizer->updateEmployeeData(empRating->getRating());
    
    resultText->append(QString("✓ Set rating of %1 to %2")
                      .arg(empStr)
                      .arg(rating));
    
    employeeRatingInput->clear();
}

void EmployeeRatingWindow::buildHierarchy() {
    if (!isInitialized) {
        QMessageBox::warning(this, "Error", "Please initialize the company first.");
        return;
    }
    
    empRating->build();
    isBuilt = true;
    
    // Disable hierarchy inputs
    enableHierarchyInputs(false);
    
    // Enable query inputs
    enableQueryInputs(true);
    
    resultText->append("\n✓ Hierarchy tree built successfully!");
    resultText->append("You can now perform updates (Type 0) and queries (Type 1).");
}

void EmployeeRatingWindow::performUpdate() {
    if (!isBuilt) {
        QMessageBox::warning(this, "Error", "Please build the hierarchy first.");
        return;
    }
    
    QString empStr = updateEmployeeInput->text().toUpper().trimmed();
    
    if (empStr.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter an employee.");
        return;
    }
    
    char employee = empStr[0].toLatin1();
    int value = updateValueInput->value();
    
    // Validate character
    if (!isValidEmployeeChar(employee)) {
        QMessageBox::warning(this, "Error", "Invalid employee character.");
        return;
    }
    
    empRating->updateSubtree(employee, value);
    
    // Update tree visualization with new ratings
    treeVisualizer->updateEmployeeData(empRating->getRating());
    
    resultText->append(QString("\n✓ Type 0 Query: Updated ratings of %1 and all subordinates by %2")
                      .arg(empStr)
                      .arg(value));
    
    updateEmployeeInput->clear();
}

void EmployeeRatingWindow::performQuery() {
    if (!isBuilt) {
        QMessageBox::warning(this, "Error", "Please build the hierarchy first.");
        return;
    }
    
    QString empStr = queryEmployeeInput->text().toUpper().trimmed();
    
    if (empStr.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter an employee.");
        return;
    }
    
    char employee = empStr[0].toLatin1();
    
    // Validate character
    if (!isValidEmployeeChar(employee)) {
        QMessageBox::warning(this, "Error", "Invalid employee character.");
        return;
    }
    
    int performance = empRating->queryPerformance(employee);
    
    resultText->append(QString("\n✓ Type 1 Query: Performance (GCD) of %1 = %2")
                      .arg(empStr)
                      .arg(performance));
    
    queryEmployeeInput->clear();
}

void EmployeeRatingWindow::goBack() {
    close();
}
