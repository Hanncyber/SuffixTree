#include "TreeVisualizer.h"
#include <QPainter>
#include <QFontMetrics>
#include <cmath>
#include <algorithm>

// ----------------- define static constants -----------------
const int TreeVisualizer::NODE_RADIUS = 25;
const int TreeVisualizer::VERTICAL_SPACING = 100;
const int TreeVisualizer::MIN_HORIZONTAL_SPACING = 60;

// ----------------- Constructor -----------------
TreeVisualizer::TreeVisualizer(SuffixNode* root, const std::string& text, QWidget *parent)
    : QWidget(parent), rootNode(root), treeText(text), treeWidth(800), treeHeight(600) {
    setMinimumSize(800, 600);
    calculatePositions();
}

// ----------------- Set new tree -----------------
void TreeVisualizer::setTree(SuffixNode* root, const std::string& text) {
    rootNode = root;
    treeText = text;
    nodePositions.clear();
    calculatePositions();
    update();
}

// ----------------- Calculate positions -----------------
void TreeVisualizer::calculatePositions() {
    if (!rootNode) return;
    
    nodePositions.clear();
    treeWidth = calculateSubtreePositions(rootNode, 400, 50, 200);
    treeHeight = 600;
    
    setMinimumSize(std::max(800, treeWidth + 100), treeHeight);
}

// ----------------- Recursive subtree positioning -----------------
int TreeVisualizer::calculateSubtreePositions(SuffixNode* node, int x, int y, int horizontalSpacing) {
    if (!node) return x;
    
    NodePosition pos;
    pos.x = x;
    pos.y = y;
    pos.node = node;
    nodePositions[node] = pos;
    
    std::vector<SuffixNode*> children;
    for (int i = 0; i < 128; i++) {
        if (node->children[i]) {
            children.push_back(node->children[i]);
        }
    }
    
    if (children.empty()) return x;
    
    int childSpacing = std::max(MIN_HORIZONTAL_SPACING, horizontalSpacing / (int)children.size());
    int startX = x - (childSpacing * ((int)children.size() - 1)) / 2;
    int maxWidth = x;
    
    for (size_t i = 0; i < children.size(); i++) {
        int childX = startX + (int)i * childSpacing;
        int childY = y + VERTICAL_SPACING;
        int width = calculateSubtreePositions(children[i], childX, childY, childSpacing);
        maxWidth = std::max(maxWidth, width);
    }
    
    return maxWidth;
}

// ----------------- Paint event -----------------
void TreeVisualizer::paintEvent(QPaintEvent * /*event*/) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    painter.fillRect(rect(), QColor(250, 250, 255));
    
    if (!rootNode) {
        painter.setPen(Qt::black);
        painter.drawText(rect(), Qt::AlignCenter, "No tree to display");
        return;
    }
    
    // Draw edges
    for (const auto& pair : nodePositions) {
        SuffixNode* node = pair.first;
        NodePosition pos = pair.second;
        
        for (int i = 0; i < 128; i++) {
            if (node->children[i]) {
                auto childIt = nodePositions.find(node->children[i]);
                if (childIt != nodePositions.end()) {
                    QString label = getEdgeLabel(node->children[i]);
                    drawEdge(painter, pos.x, pos.y, childIt->second.x, childIt->second.y, label);
                }
            }
        }
    }
    
    // Draw nodes
    for (const auto& pair : nodePositions) {
        NodePosition pos = pair.second;
        drawNode(painter, pos.node, pos.x, pos.y);
    }
}

// ----------------- Draw node -----------------
void TreeVisualizer::drawNode(QPainter& painter, SuffixNode* node, int x, int y) {
    QColor nodeColor = getNodeColor(node);
    
    QRadialGradient gradient(x, y, NODE_RADIUS);
    gradient.setColorAt(0, nodeColor.lighter(120));
    gradient.setColorAt(1, nodeColor);
    
    painter.setBrush(gradient);
    painter.setPen(QPen(nodeColor.darker(120), 2));
    painter.drawEllipse(QPoint(x, y), NODE_RADIUS, NODE_RADIUS);
    
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setBold(true);
    font.setPointSize(9);
    painter.setFont(font);
    
    if (node->suffix_index != -1) {
        painter.drawText(QRect(x - NODE_RADIUS, y - NODE_RADIUS, NODE_RADIUS * 2, NODE_RADIUS * 2),
                         Qt::AlignCenter, QString::number(node->suffix_index));
    } else {
        painter.drawText(QRect(x - NODE_RADIUS, y - NODE_RADIUS, NODE_RADIUS * 2, NODE_RADIUS * 2),
                         Qt::AlignCenter, "•");
    }
}

// ----------------- Draw edge -----------------
void TreeVisualizer::drawEdge(QPainter& painter, int x1, int y1, int x2, int y2, const QString& label) {
    QPen pen(QColor(100, 100, 200), 2);
    painter.setPen(pen);
    painter.drawLine(x1, y1 + NODE_RADIUS, x2, y2 - NODE_RADIUS);
    
    if (!label.isEmpty()) {
        int midX = (x1 + x2) / 2;
        int midY = (y1 + y2) / 2;
        
        QFont font = painter.font();
        font.setPointSize(8);
        font.setBold(true);
        painter.setFont(font);
        
        QFontMetrics fm(font);
        QRect textRect = fm.boundingRect(label);
        textRect.moveCenter(QPoint(midX, midY));
        textRect.adjust(-4, -2, 4, 2);
        
        painter.setBrush(QColor(255, 255, 200, 230));
        painter.setPen(QPen(QColor(150, 150, 0), 1));
        painter.drawRoundedRect(textRect, 3, 3);
        
        painter.setPen(QColor(50, 50, 100));
        painter.drawText(textRect, Qt::AlignCenter, label);
    }
}

// ----------------- Get edge label -----------------
QString TreeVisualizer::getEdgeLabel(SuffixNode* node) {
    if (!node || node->start < 0) return "";
    
    int start = node->start;
    int end = (node->end ? *(node->end) : (int)treeText.length() - 1);
    
    if (start > end || start >= (int)treeText.length()) return "";
    
    int len = end - start + 1;
    if (len > 10) {
        std::string substr = treeText.substr(start, 8);
        return QString::fromStdString(substr) + "...";
    } else {
        std::string substr = treeText.substr(start, len);
        return QString::fromStdString(substr);
    }
}

// ----------------- Get node color -----------------
QColor TreeVisualizer::getNodeColor(SuffixNode* node) {
    if (node->suffix_index != -1) {
        int hue = (node->suffix_index * 137) % 360;
        return QColor::fromHsv(hue, 200, 220);
    } else {
        return QColor(100, 149, 237);
    }
}

// ----------------- Size hint -----------------
QSize TreeVisualizer::sizeHint() const {
    return QSize(treeWidth, treeHeight);
}
