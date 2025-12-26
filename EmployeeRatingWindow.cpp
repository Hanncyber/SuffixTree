#include "EmployeeRatingWindow.h"
#include <QMessageBox>
#include <QFont>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <sstream>

EmployeeRatingWindow::EmployeeRatingWindow(QWidget *parent)
    : QWidget(parent), empRating(nullptr), numEmployees(0), isInitialized(false), isBuilt(false) {
    setupUI();
    setWindowTitle("Employee Rating System");
    resize(1000, 800);
}

EmployeeRatingWindow::~EmployeeRatingWindow() {
    if (empRating) delete empRating;
}

void EmployeeRatingWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    QLabel *title = new QLabel("Employee Rating & Performance System", this);
    QFont titleFont = title->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #9B59B6;");
    mainLayout->addWidget(title);

    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(15);


    QWidget *controlsWidget = new QWidget(this);
    QVBoxLayout *controlsLayout = new QVBoxLayout(controlsWidget);
    controlsLayout->setContentsMargins(10, 10, 10, 10);
    controlsLayout->setSpacing(10);

    QLabel *instructions = new QLabel(
        "This system manages employee hierarchy and ratings.\n"
        "Step 1: Initialize company with number of employees (H is root, then A, B, C, ...)\n"
        "Step 2: Add subordinates and set initial ratings\n"
        "Step 3: Build the hierarchy\n"
        "Step 4: Perform updates (Type 0) or queries (Type 1)",
        controlsWidget);
    instructions->setStyleSheet("QLabel { font-size: 11px; color: #7F8C8D; padding: 5px; }");
    instructions->setWordWrap(true);
    controlsLayout->addWidget(instructions);

   QGroupBox *initGroup = new QGroupBox("Step 1: Initialize Company", controlsWidget);
    QHBoxLayout *initLayout = new QHBoxLayout();
    QLabel *numEmpLabel = new QLabel("Number of employees (including head H):", controlsWidget);
    numEmployeesInput = new QSpinBox(controlsWidget);
    numEmployeesInput->setRange(1, 26);
    numEmployeesInput->setValue(5);
    numEmployeesInput->setStyleSheet("QSpinBox { font-size: 13px; padding: 4px; }");
    initButton = new QPushButton("Initialize", controlsWidget);
    initButton->setStyleSheet(
        "QPushButton { background-color: #9B59B6; color: white; font-weight: bold; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: #8E44AD; }"
        );
    connect(initButton, &QPushButton::clicked, this, &EmployeeRatingWindow::initializeCompany);
    initLayout->addWidget(numEmpLabel);
    initLayout->addWidget(numEmployeesInput);
    initLayout->addWidget(initButton);
    initGroup->setLayout(initLayout);
    controlsLayout->addWidget(initGroup);

    QGroupBox *hierarchyGroup = new QGroupBox("Step 2: Build Hierarchy & Set Ratings", controlsWidget);
    QVBoxLayout *hierarchyLayout = new QVBoxLayout();

    QHBoxLayout *subordinateLayout = new QHBoxLayout();
    QLabel *managerLabel = new QLabel("Manager:", controlsWidget);
    managerInput = new QLineEdit(controlsWidget);
    managerInput->setPlaceholderText("e.g., H");
    managerInput->setMaxLength(1);
    managerInput->setStyleSheet("QLineEdit { font-size: 13px; padding: 4px; }");
    managerInput->setEnabled(false);
    QLabel *subordinateLabel = new QLabel("Subordinate:", controlsWidget);
    subordinateInput = new QLineEdit(controlsWidget);
    subordinateInput->setPlaceholderText("e.g., A");
    subordinateInput->setMaxLength(1);
    subordinateInput->setStyleSheet("QLineEdit { font-size: 13px; padding: 4px; }");
    subordinateInput->setEnabled(false);
    addSubordinateButton = new QPushButton("Add Subordinate", controlsWidget);
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

    QHBoxLayout *ratingLayout = new QHBoxLayout();
    QLabel *empRatingLabel = new QLabel("Employee:", controlsWidget);
    employeeRatingInput = new QLineEdit(controlsWidget);
    employeeRatingInput->setPlaceholderText("e.g., H");
    employeeRatingInput->setMaxLength(1);
    employeeRatingInput->setStyleSheet("QLineEdit { font-size: 13px; padding: 4px; }");
    employeeRatingInput->setEnabled(false);
    QLabel *ratingLabel = new QLabel("Rating:", controlsWidget);
    ratingValueInput = new QSpinBox(controlsWidget);
    ratingValueInput->setRange(1, 100);
    ratingValueInput->setValue(10);
    ratingValueInput->setStyleSheet("QSpinBox { font-size: 13px; padding: 4px; }");
    ratingValueInput->setEnabled(false);
    setRatingButton = new QPushButton("Set Rating", controlsWidget);
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
    controlsLayout->addWidget(hierarchyGroup);

    buildButton = new QPushButton("Step 3: Build Hierarchy Tree", controlsWidget);
    buildButton->setStyleSheet(
        "QPushButton { background-color: #E67E22; color: white; font-weight: bold; border-radius: 5px; padding: 10px; font-size: 14px; }"
        "QPushButton:hover { background-color: #D35400; }"
        "QPushButton:disabled { background-color: #BDC3C7; }"
        );
    buildButton->setEnabled(false);
    connect(buildButton, &QPushButton::clicked, this, &EmployeeRatingWindow::buildHierarchy);
    controlsLayout->addWidget(buildButton);

    QGroupBox *queryGroup = new QGroupBox("Step 4: Perform Operations", controlsWidget);
    QVBoxLayout *queryLayout = new QVBoxLayout();

    QHBoxLayout *updateLayout = new QHBoxLayout();
    QLabel *updateLabel = new QLabel("Type 0 - Update Subtree:", controlsWidget);
    updateLabel->setStyleSheet("font-weight: bold;");
    queryLayout->addWidget(updateLabel);

    QLabel *updateEmpLabel = new QLabel("Employee:", controlsWidget);
    updateEmployeeInput = new QLineEdit(controlsWidget);
    updateEmployeeInput->setPlaceholderText("e.g., A");
    updateEmployeeInput->setMaxLength(1);
    updateEmployeeInput->setStyleSheet("QLineEdit { font-size: 13px; padding: 4px; }");
    updateEmployeeInput->setEnabled(false);

    QLabel *updateValLabel = new QLabel("Update by:", controlsWidget);
    updateValueInput = new QSpinBox(controlsWidget);
    updateValueInput->setRange(-100, 100);
    updateValueInput->setValue(5);
    updateValueInput->setStyleSheet("QSpinBox { font-size: 13px; padding: 4px; }");
    updateValueInput->setEnabled(false);

    updateButton = new QPushButton("Update Subtree", controlsWidget);
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

    QHBoxLayout *queryPerformanceLayout = new QHBoxLayout();
    QLabel *queryLabel = new QLabel("Type 1 - Query Performance (GCD):", controlsWidget);
    queryLabel->setStyleSheet("font-weight: bold;");
    queryPerformanceLayout->addWidget(queryLabel);

    QLabel *queryEmpLabel = new QLabel("Employee:", controlsWidget);
    queryEmployeeInput = new QLineEdit(controlsWidget);
    queryEmployeeInput->setPlaceholderText("e.g., H");
    queryEmployeeInput->setMaxLength(1);
    queryEmployeeInput->setStyleSheet("QLineEdit { font-size: 13px; padding: 4px; }");
    queryEmployeeInput->setEnabled(false);

    queryButton = new QPushButton("Query Performance", controlsWidget);
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
    queryGroup->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    controlsLayout->addWidget(queryGroup);

    resultText = new QTextEdit(controlsWidget);
    resultText->setReadOnly(true);
    resultText->setStyleSheet(
        "QTextEdit { font-size: 13px; background-color: #2C3E50; color: #ECF0F1; border: 1px solid #34495E; }"
        );
    resultText->setMinimumHeight(120);
    resultText->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    controlsLayout->addWidget(resultText);

    controlsLayout->addStretch();

    QScrollArea *controlsScroll = new QScrollArea(this);
    controlsScroll->setWidgetResizable(true);
    controlsScroll->setFrameShape(QFrame::NoFrame);
    controlsScroll->setWidget(controlsWidget);
    contentLayout->addWidget(controlsScroll, 1);

    QWidget *visualizationWidget = new QWidget(this);
    QVBoxLayout *visualizationLayout = new QVBoxLayout(visualizationWidget);
    visualizationLayout->setContentsMargins(10, 10, 10, 10);
    visualizationLayout->setSpacing(5);

    QLabel *vizLabel = new QLabel("Employee Hierarchy Tree Visualization", visualizationWidget);
    QFont vizFont = vizLabel->font();
    vizFont.setPointSize(14);
    vizFont.setBold(true);
    vizLabel->setFont(vizFont);
    vizLabel->setAlignment(Qt::AlignCenter);
    vizLabel->setStyleSheet("color: #9B59B6; padding: 5px;");
    visualizationLayout->addWidget(vizLabel);

    scrollArea = new QScrollArea(visualizationWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: 1px solid #34495E; background-color: #FAFAFF; }");
    treeVisualizer = new EmployeeTreeVisualizer(visualizationWidget);
    scrollArea->setWidget(treeVisualizer);
    visualizationLayout->addWidget(scrollArea);

    contentLayout->addWidget(visualizationWidget, 1);

    mainLayout->addLayout(contentLayout);

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
    if (empRating) delete empRating;
    empRating = new EmployeeRating(numEmployees);
    isInitialized = true;
    isBuilt = false;
    enableHierarchyInputs(true);
    numEmployeesInput->setEnabled(false);
    initButton->setEnabled(false);
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

void EmployeeRatingWindow::updateTreeVisualization() {
    if (empRating && isInitialized && isBuilt && treeVisualizer) {
        int numEmps = empRating->getNumEmployees();
        int* const* treeData = empRating->getTree();
        const int* childCountData = empRating->getChildCount();
        const int* ratingData = empRating->getRating();
        if (treeData && childCountData && ratingData && numEmps > 0) {
            treeVisualizer->setEmployeeData(numEmps, treeData, childCountData, ratingData);
        }
    }
}

void EmployeeRatingWindow::addSubordinateToHierarchy() {
    if (!isInitialized) { QMessageBox::warning(this,"Error","Please initialize the company first."); return; }
    QString managerStr = managerInput->text().toUpper().trimmed();
    QString subordinateStr = subordinateInput->text().toUpper().trimmed();
    if (managerStr.isEmpty() || subordinateStr.isEmpty()) { QMessageBox::warning(this,"Error","Please enter both manager and subordinate."); return; }
    char manager = managerStr[0].toLatin1();
    char subordinate = subordinateStr[0].toLatin1();
    if (!isValidEmployeeChar(manager)) { QMessageBox::warning(this,"Error","Invalid manager character."); return; }
    if (!isValidEmployeeChar(subordinate)) { QMessageBox::warning(this,"Error","Invalid subordinate character."); return; }
    std::string errorMsg = empRating->addSubordinate(manager, subordinate);
    if (!errorMsg.empty()) { QMessageBox::warning(this,"Error",QString::fromStdString(errorMsg)); return; }
    resultText->append(QString("✓ Added %1 as subordinate of %2").arg(subordinateStr).arg(managerStr));
    managerInput->clear();
    subordinateInput->clear();
}

void EmployeeRatingWindow::setEmployeeRating() {
    if (!isInitialized) { QMessageBox::warning(this,"Error","Please initialize the company first."); return; }
    QString empStr = employeeRatingInput->text().toUpper().trimmed();
    if (empStr.isEmpty()) { QMessageBox::warning(this,"Error","Please enter an employee."); return; }
    char employee = empStr[0].toLatin1();
    int rating = ratingValueInput->value();
    if (!isValidEmployeeChar(employee)) { QMessageBox::warning(this,"Error","Invalid employee character."); return; }
    empRating->setInitialRating(employee, rating);
    resultText->append(QString("✓ Set rating of %1 to %2").arg(empStr).arg(rating));
    employeeRatingInput->clear();
}

void EmployeeRatingWindow::buildHierarchy() {
    if (!isInitialized) { QMessageBox::warning(this,"Error","Please initialize the company first."); return; }
    empRating->build();
    isBuilt = true;
    enableHierarchyInputs(false);
    enableQueryInputs(true);
    updateTreeVisualization();
    resultText->append("\n✓ Hierarchy tree built successfully!");
    resultText->append("You can now perform updates (Type 0) and queries (Type 1).");
}

void EmployeeRatingWindow::performUpdate() {
    if (!isBuilt) { QMessageBox::warning(this,"Error","Please build the hierarchy first."); return; }
    QString empStr = updateEmployeeInput->text().toUpper().trimmed();
    if (empStr.isEmpty()) { QMessageBox::warning(this,"Error","Please enter an employee."); return; }
    char employee = empStr[0].toLatin1();
    int value = updateValueInput->value();
    if (!isValidEmployeeChar(employee)) { QMessageBox::warning(this,"Error","Invalid employee character."); return; }
    empRating->updateSubtree(employee, value);
    updateTreeVisualization();
    resultText->append(QString("\n✓ Type 0 Query: Updated ratings of %1 and all subordinates by %2").arg(empStr).arg(value));
    updateEmployeeInput->clear();
}

void EmployeeRatingWindow::performQuery() {
    if (!isBuilt) { QMessageBox::warning(this,"Error","Please build the hierarchy first."); return; }
    QString empStr = queryEmployeeInput->text().toUpper().trimmed();
    if (empStr.isEmpty()) { QMessageBox::warning(this,"Error","Please enter an employee."); return; }
    char employee = empStr[0].toLatin1();
    if (!isValidEmployeeChar(employee)) { QMessageBox::warning(this,"Error","Invalid employee character."); return; }
    int performance = empRating->queryPerformance(employee);
    resultText->append(QString("\n✓ Type 1 Query: Performance (GCD) of %1 = %2").arg(empStr).arg(performance));
    queryEmployeeInput->clear();
}

void EmployeeRatingWindow::goBack() {
    close();
}
