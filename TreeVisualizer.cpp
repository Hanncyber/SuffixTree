#include "TreeVisualizer.h"
#include <QPainter>
#include <QPainterPath>
#include <QFontMetrics>
#include <algorithm>

// ----------------- static constants -----------------
const int TreeVisualizer::NODE_RADIUS = 16;
const int TreeVisualizer::VERTICAL_SPACING = 90;
const int TreeVisualizer::MIN_HORIZONTAL_SPACING = 40;

// ----------------- Constructor -----------------
TreeVisualizer::TreeVisualizer(SuffixNode* root,
                               const std::string& text,
                               QWidget *parent)
    : QWidget(parent),
    rootNode(root),
    treeText(text)
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
    int totalWidth = calculateSubtreePositions(rootNode, 0);
    // Shift tree to center
    for (auto& it : nodePositions) {
        it.second.x += width() / 2 - totalWidth / 2;
    }
    treeHeight = 600;
    setMinimumSize(std::max(800, totalWidth + 100), treeHeight);
}

// ----------------- Recursive layout -----------------
int TreeVisualizer::calculateSubtreePositions(SuffixNode* node, int xOffset, int depth) {
    if (!node) return 0;

    std::vector<SuffixNode*> children;
    for (int i = 0; i < 128; ++i)
        if (node->children[i]) children.push_back(node->children[i]);

    int subtreeWidth = 0;
    std::vector<int> childCenters;

    for (auto child : children) {
        int w = calculateSubtreePositions(child, xOffset + subtreeWidth, depth + 1);
        childCenters.push_back(xOffset + subtreeWidth + w / 2);
        subtreeWidth += w + MIN_HORIZONTAL_SPACING;
    }

    if (children.empty()) {
        subtreeWidth = MIN_HORIZONTAL_SPACING;
        nodePositions[node] = {xOffset, 40 + depth * VERTICAL_SPACING, node};
    } else {
        int parentX = childCenters.front() + (childCenters.back() - childCenters.front()) / 2;
        nodePositions[node] = {parentX, 40 + depth * VERTICAL_SPACING, node};
        for (auto child : children) {
            nodePositions[child].y = 40 + (depth + 1) * VERTICAL_SPACING;
        }
    }

    return subtreeWidth;
}


// ----------------- Paint event -----------------
void TreeVisualizer::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);

    QFont mono("Consolas");
    mono.setPointSize(9);
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
                    drawEdge(painter, pos.x, pos.y,
                             childIt->second.x, childIt->second.y,
                             getEdgeLabel(node->children[i]));
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
    painter.setBrush(node->suffix_index != -1 ? Qt::yellow : Qt::white);
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

// ----------------- Draw curved edge -----------------
void TreeVisualizer::drawEdge(QPainter& painter,
                              int x1, int y1,
                              int x2, int y2,
                              const QString& label)
{
    QPainterPath path;
    path.moveTo(x1, y1 + NODE_RADIUS);
    int midY = (y1 + y2) / 2;
    path.cubicTo(x1, midY, x2, midY, x2, y2 - NODE_RADIUS);
    painter.setPen(QPen(Qt::black, 1));
    painter.drawPath(path);

    if (!label.isEmpty()) {
        int mx = (x1 + x2) / 2;
        int my = midY - 5;
        painter.drawText(mx, my, label);
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
    return QSize(width(), treeHeight);
}
