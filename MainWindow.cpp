#include "MainWindow.h"
#include <QSplitter>
#include <QMessageBox>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), suffixTree(nullptr) {
    setupUI();
    setWindowTitle("Suffix Tree Visualizer - Ukkonen's Algorithm");
    resize(1200, 800);
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    // Create central widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Input section
    QGroupBox* inputGroup = new QGroupBox("Input", this);
    QVBoxLayout* inputLayout = new QVBoxLayout(inputGroup);
    
    QLabel* textLabel = new QLabel("Text (add $ at the end):", this);
    textInput = new QLineEdit(this);
    textInput->setPlaceholderText("Enter text, e.g., banana$");
    textInput->setText("banana$");
    
    buildButton = new QPushButton("Build Suffix Tree", this);
    connect(buildButton, &QPushButton::clicked, this, &MainWindow::onBuildTree);
    
    inputLayout->addWidget(textLabel);
    inputLayout->addWidget(textInput);
    inputLayout->addWidget(buildButton);

    // Search section
    QGroupBox* searchGroup = new QGroupBox("Search", this);
    QHBoxLayout* searchLayout = new QHBoxLayout(searchGroup);
    
    QLabel* searchLabel = new QLabel("Pattern:", this);
    searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText("Enter pattern to search");
    
    searchButton = new QPushButton("Search", this);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchPattern);
    
    clearButton = new QPushButton("Clear", this);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onClear);
    
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchInput);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(clearButton);

    // Create splitter for tree view and info panels
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    
    // Tree visualization
    QGroupBox* treeGroup = new QGroupBox("Tree Visualization", this);
    QVBoxLayout* treeLayout = new QVBoxLayout(treeGroup);
    
    treeScene = new QGraphicsScene(this);
    treeView = new QGraphicsView(treeScene, this);
    treeView->setRenderHint(QPainter::Antialiasing);
    treeView->setDragMode(QGraphicsView::ScrollHandDrag);
    treeView->setMinimumWidth(600);
    
    treeLayout->addWidget(treeView);

    // Right panel with stats and results
    QWidget* rightPanel = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    
    // Statistics display
    QGroupBox* statsGroup = new QGroupBox("Statistics", this);
    QVBoxLayout* statsLayout = new QVBoxLayout(statsGroup);
    
    statsDisplay = new QTextEdit(this);
    statsDisplay->setReadOnly(true);
    statsDisplay->setMaximumHeight(200);
    
    statsLayout->addWidget(statsDisplay);
    
    // Results display
    QGroupBox* resultsGroup = new QGroupBox("Search Results", this);
    QVBoxLayout* resultsLayout = new QVBoxLayout(resultsGroup);
    
    resultsDisplay = new QTextEdit(this);
    resultsDisplay->setReadOnly(true);
    
    resultsLayout->addWidget(resultsDisplay);
    
    rightLayout->addWidget(statsGroup);
    rightLayout->addWidget(resultsGroup);

    // Add to splitter
    splitter->addWidget(treeGroup);
    splitter->addWidget(rightPanel);
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);

    // Add all to main layout
    mainLayout->addWidget(inputGroup);
    mainLayout->addWidget(searchGroup);
    mainLayout->addWidget(splitter, 1);
}

void MainWindow::onBuildTree() {
    QString text = textInput->text();
    
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter text to build the suffix tree.");
        return;
    }
    
    if (!text.endsWith("$")) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Input Error", 
            "Text must end with a unique terminator character '$'.\nDo you want to add it automatically?",
            QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::Yes) {
            text += "$";
            textInput->setText(text);
        } else {
            return;
        }
    }
    
    currentText = text;
    
    try {
        // Build the suffix tree
        suffixTree = std::make_unique<SuffixTree>(text.toStdString());
        
        // Display statistics
        statsDisplay->clear();
        statsDisplay->append("=== Suffix Tree Statistics ===\n");
        statsDisplay->append(QString("Text: \"%1\"").arg(text));
        statsDisplay->append(QString("Length: %1").arg(text.length()));
        statsDisplay->append(QString("Unique Substrings: %1")
            .arg(suffixTree->countAllUniqueSubstrings()));
        
        std::string lrs = suffixTree->getLongestRepeatedSubstring();
        if (!lrs.empty()) {
            statsDisplay->append(QString("Longest Repeated Substring: \"%1\"")
                .arg(QString::fromStdString(lrs)));
        }
        
        std::string shortestUnique = suffixTree->getShortestUniqueSubstring();
        statsDisplay->append(QString("Shortest Unique Substring: \"%1\"")
            .arg(QString::fromStdString(shortestUnique)));
        
        statsDisplay->append("\n✓ Tree built successfully!");
        
        // Visualize the tree
        visualizeTree(suffixTree.get());
        
        // Clear previous search results
        resultsDisplay->clear();
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", 
            QString("Failed to build suffix tree: %1").arg(e.what()));
    }
}

void MainWindow::onSearchPattern() {
    if (!suffixTree) {
        QMessageBox::warning(this, "Error", "Please build a suffix tree first.");
        return;
    }
    
    QString pattern = searchInput->text();
    
    if (pattern.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a pattern to search.");
        return;
    }
    
    try {
        std::string patternStr = pattern.toStdString();
        
        // Check if pattern exists
        bool exists = suffixTree->contains(patternStr);
        
        resultsDisplay->clear();
        resultsDisplay->append(QString("=== Search Results for \"%1\" ===\n").arg(pattern));
        
        if (exists) {
            // Get occurrence indices
            std::vector<int> indices = suffixTree->getOccurrenceIndices(patternStr);
            int count = suffixTree->countOccurrences(patternStr);
            
            resultsDisplay->append(QString("✓ Pattern found!"));
            resultsDisplay->append(QString("Occurrences: %1").arg(count));
            resultsDisplay->append("\nPositions:");
            
            for (int idx : indices) {
                resultsDisplay->append(QString("  - Index %1").arg(idx));
            }
            
            // Get context if available
            std::string context = suffixTree->getContext(patternStr, 10);
            if (!context.empty()) {
                resultsDisplay->append(QString("\nContext:\n%1")
                    .arg(QString::fromStdString(context)));
            }
            
            // Try autocomplete
            std::vector<std::string> completions = suffixTree->autoComplete(patternStr);
            if (!completions.empty() && completions.size() < 10) {
                resultsDisplay->append("\nPossible completions:");
                for (const auto& comp : completions) {
                    resultsDisplay->append(QString("  - %1")
                        .arg(QString::fromStdString(comp)));
                }
            }
            
        } else {
            resultsDisplay->append("✗ Pattern not found in the text.");
        }
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", 
            QString("Search failed: %1").arg(e.what()));
    }
}

void MainWindow::onClear() {
    textInput->clear();
    searchInput->clear();
    statsDisplay->clear();
    resultsDisplay->clear();
    treeScene->clear();
    suffixTree.reset();
    currentText.clear();
}

void MainWindow::visualizeTree(SuffixTree* tree) {
    if (!tree) return;
    
    treeScene->clear();
    
    // Add title
    QGraphicsTextItem* title = treeScene->addText("Suffix Tree Structure");
    QFont titleFont = title->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setPos(-200, -80);
    
    // Get the root node and start drawing
    SuffixNode* root = tree->getRoot();
    if (!root) return;
    
    // Set initial drawing parameters
    qreal startX = -200;
    qreal startY = 0;
    qreal horizontalSpacing = 150;
    
    // Draw the tree starting from root
    drawNode(root, startX, startY, horizontalSpacing, 0, tree);
    
    // Fit the view to show all content
    treeView->fitInView(treeScene->sceneRect().adjusted(-50, -50, 50, 50), Qt::KeepAspectRatio);
}

void MainWindow::drawNode(SuffixNode* node, qreal x, qreal y, qreal horizontalSpacing, 
                          int depth, SuffixTree* tree) {
    if (!node) return;
    
    const qreal NODE_RADIUS = 20;
    const qreal VERTICAL_SPACING = 80;
    
    std::string text = tree->getText();
    
    // Draw current node as a circle
    treeScene->addEllipse(
        x - NODE_RADIUS, y - NODE_RADIUS, 
        NODE_RADIUS * 2, NODE_RADIUS * 2,
        QPen(Qt::black, 2),
        QBrush(node->children.empty() ? Qt::lightGray : Qt::white)
    );
    
    // Add node label (show if it's a leaf with suffix index)
    if (node->suffixIndex != -1) {
        QGraphicsTextItem* label = treeScene->addText(QString::number(node->suffixIndex));
        QFont font = label->font();
        font.setPointSize(8);
        label->setFont(font);
        label->setPos(x - 8, y - 8);
    }
    
    // Calculate positions for children
    int childCount = node->children.size();
    if (childCount == 0) return;
    
    qreal totalWidth = horizontalSpacing * (childCount - 1);
    qreal childStartX = x - totalWidth / 2;
    
    int childIndex = 0;
    for (auto const& [edgeChar, child] : node->children) {
        if (!child) continue;
        
        // Calculate child position
        qreal childX = childStartX + childIndex * horizontalSpacing;
        qreal childY = y + VERTICAL_SPACING;
        
        // Draw edge line
        treeScene->addLine(x, y + NODE_RADIUS, childX, childY - NODE_RADIUS, 
                          QPen(Qt::black, 2));
        
        // Get edge label text
        int start = child->start;
        if (!child->end) continue; // Skip if end pointer is null
        int end = *(child->end);
        
        // Validate bounds before accessing text
        if (start < 0 || start >= static_cast<int>(text.length())) continue;
        
        int edgeLength = end - start + 1;
        int availableLength = text.length() - start;
        
        // Limit edge label length for display
        std::string edgeLabel = text.substr(start, std::min({edgeLength, 10, availableLength}));
        if (edgeLength > 10) edgeLabel += "...";
        
        // Draw edge label
        QGraphicsTextItem* edgeLabelItem = treeScene->addText(
            QString::fromStdString(edgeLabel)
        );
        QFont edgeFont = edgeLabelItem->font();
        edgeFont.setPointSize(8);
        edgeLabelItem->setFont(edgeFont);
        edgeLabelItem->setDefaultTextColor(Qt::blue);
        
        // Position edge label on the line
        qreal labelX = (x + childX) / 2 - 15;
        qreal labelY = (y + childY) / 2 - 20;
        edgeLabelItem->setPos(labelX, labelY);
        
        // Add range indicator
        QGraphicsTextItem* rangeLabel = treeScene->addText(
            QString("[%1,%2]").arg(start).arg(end)
        );
        QFont rangeFont = rangeLabel->font();
        rangeFont.setPointSize(7);
        rangeLabel->setFont(rangeFont);
        rangeLabel->setDefaultTextColor(Qt::darkGray);
        rangeLabel->setPos(labelX, labelY + 15);
        
        // Recursively draw child with reduced spacing
        drawNode(child, childX, childY, horizontalSpacing * 0.6, depth + 1, tree);
        
        childIndex++;
    }
}
