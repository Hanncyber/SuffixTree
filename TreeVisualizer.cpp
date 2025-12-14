#include "TreeVisualizer.h"
#include <QPainter>
#include <QFontMetrics>
#include <algorithm>

// ----------------- static constants -----------------
const int TreeVisualizer::NODE_RADIUS = 14;
const int TreeVisualizer::VERTICAL_SPACING = 90;
const int TreeVisualizer::MIN_HORIZONTAL_SPACING = 70;

// ----------------- Constructor -----------------
TreeVisualizer::TreeVisualizer(SuffixNode* root,
                               const std::string& text,
                               QWidget *parent)
    : QWidget(parent),
    rootNode(root),
    treeText(text),
    treeWidth(800),
    treeHeight(600)
{
    setMinimumSize(800, 600);
    calculatePositions();
}

// ----------------- Update tree -----------------
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
    treeWidth = calculateSubtreePositions(rootNode, width() / 2, 40, 300);
    treeHeight = 600;

    setMinimumSize(std::max(800, treeWidth + 100), treeHeight);
}

// ----------------- Recursive layout -----------------
int TreeVisualizer::calculateSubtreePositions(SuffixNode* node,
                                              int x,
                                              int y,
                                              int spacing)
{
    if (!node) return x;

    nodePositions[node] = { x, y, node };

    std::vector<SuffixNode*> children;
    for (int i = 0; i < 128; ++i) {
        if (node->children[i]) {
            children.push_back(node->children[i]);
        }
    }

    if (children.empty()) return x;

    int childSpacing = std::max(MIN_HORIZONTAL_SPACING,
                                spacing / (int)children.size());
    int startX = x - (childSpacing * ((int)children.size() - 1)) / 2;

    int maxX = x;
    for (size_t i = 0; i < children.size(); ++i) {
        maxX = std::max(
            maxX,
            calculateSubtreePositions(
                children[i],
                startX + (int)i * childSpacing,
                y + VERTICAL_SPACING,
                childSpacing
                )
            );
    }

    return maxX;
}

// ----------------- Paint event -----------------
void TreeVisualizer::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), Qt::white);

    QFont mono("Consolas");
    mono.setPointSize(8);
    painter.setFont(mono);

    if (!rootNode) {
        painter.drawText(rect(), Qt::AlignCenter, "No tree to display");
        return;
    }

    // Draw edges first
    for (const auto& it : nodePositions) {
        SuffixNode* node = it.first;
        const NodePosition& pos = it.second;

        for (int i = 0; i < 128; ++i) {
            if (node->children[i]) {
                auto childIt = nodePositions.find(node->children[i]);
                if (childIt != nodePositions.end()) {
                    drawEdge(
                        painter,
                        pos.x, pos.y,
                        childIt->second.x,
                        childIt->second.y,
                        getEdgeLabel(node->children[i])
                        );
                }
            }
        }
    }

    // Draw nodes on top
    for (const auto& it : nodePositions) {
        drawNode(painter, it.first, it.second.x, it.second.y);
    }
}

// ----------------- Draw node -----------------
void TreeVisualizer::drawNode(QPainter& painter,
                              SuffixNode* node,
                              int x,
                              int y)
{
    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::black, 1));
    painter.drawEllipse(QPoint(x, y), NODE_RADIUS, NODE_RADIUS);

    if (node->suffix_index != -1) {
        painter.drawText(
            QRect(x - NODE_RADIUS, y - NODE_RADIUS,
                  NODE_RADIUS * 2, NODE_RADIUS * 2),
            Qt::AlignCenter,
            QString::number(node->suffix_index)
            );
    }
}

// ----------------- Draw edge -----------------
void TreeVisualizer::drawEdge(QPainter& painter,
                              int x1, int y1,
                              int x2, int y2,
                              const QString& label)
{
    painter.setPen(QPen(Qt::black, 1));
    painter.drawLine(x1, y1 + NODE_RADIUS,
                     x2, y2 - NODE_RADIUS);

    if (!label.isEmpty()) {
        int mx = (x1 + x2) / 2;
        int my = (y1 + y2) / 2;
        painter.drawText(mx + 3, my - 2, label);
    }
}

// ----------------- Edge label -----------------
QString TreeVisualizer::getEdgeLabel(SuffixNode* node) {
    if (!node || node->start < 0) return "";

    int start = node->start;
    int end = node->end ? *node->end : (int)treeText.size() - 1;
    if (start > end) return "";

    int len = end - start + 1;
    if (len > 6) {
        return QString::fromStdString(treeText.substr(start, 6)) + "...";
    }

    return QString::fromStdString(treeText.substr(start, len));
}

// ----------------- Size hint -----------------
QSize TreeVisualizer::sizeHint() const {
    return QSize(treeWidth, treeHeight);
}
