#include "TreeVisualizer.h"
#include <QPainter>
#include <QPainterPath>
#include <QFontMetrics>
#include <algorithm>

const int TreeVisualizer::NODE_RADIUS = 16;
const int TreeVisualizer::VERTICAL_SPACING = 120;
const int TreeVisualizer::MIN_HORIZONTAL_SPACING = 50;


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


void TreeVisualizer::setTree(SuffixNode* root, const std::string& text) {
    rootNode = root;
    treeText = text;
    nodePositions.clear();
    calculatePositions();
    update();
}


void TreeVisualizer::calculatePositions() {
    if (!rootNode) return;

    nodePositions.clear();
    QFontMetrics fm(font());
    int totalWidth = calculateSubtreePositions(rootNode, 0, 0, fm);

    int minX = INT_MAX, maxX = INT_MIN;
    for (auto& it : nodePositions) {
        minX = std::min(minX, it.second.x);
        maxX = std::max(maxX, it.second.x);
    }

    int shiftX = 20 - minX;
    for (auto& it : nodePositions) it.second.x += shiftX;

    treeHeight = 40 + getTreeDepth(rootNode) * VERTICAL_SPACING + 50;
    setMinimumSize(std::max(maxX - minX + 60, 800), treeHeight);
}


int TreeVisualizer::getTreeDepth(SuffixNode* node) {
    if (!node) return 0;
    int maxChildDepth = 0;
    for (int i = 0; i < 128; ++i)
        if (node->children[i])
            maxChildDepth = std::max(maxChildDepth, getTreeDepth(node->children[i]));
    return 1 + maxChildDepth;
}


int TreeVisualizer::calculateSubtreePositions(SuffixNode* node, int xOffset, int depth, QFontMetrics &fm) {
    if (!node) return 0;

    std::vector<SuffixNode*> children;
    for (int i = 0; i < 128; ++i)
        if (node->children[i]) children.push_back(node->children[i]);

    int subtreeWidth = 0;
    std::vector<int> childCenters;

    for (auto child : children) {
        int labelWidth = fm.horizontalAdvance(getEdgeLabel(child));
        int w = calculateSubtreePositions(child, xOffset + subtreeWidth, depth + 1, fm);
        w = std::max(w, labelWidth + MIN_HORIZONTAL_SPACING);
        childCenters.push_back(xOffset + subtreeWidth + w / 2);
        subtreeWidth += w + MIN_HORIZONTAL_SPACING;
    }

    if (children.empty()) {
        subtreeWidth = std::max(NODE_RADIUS * 2, MIN_HORIZONTAL_SPACING);
        nodePositions[node] = {xOffset, 40 + depth * VERTICAL_SPACING, node};
    } else {
        int parentX = childCenters.front() + (childCenters.back() - childCenters.front()) / 2;
        nodePositions[node] = {parentX, 40 + depth * VERTICAL_SPACING, node};
        for (auto child : children)
            nodePositions[child].y = 40 + (depth + 1) * VERTICAL_SPACING;
    }

    return subtreeWidth;
}


void TreeVisualizer::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Dark background
    painter.fillRect(rect(), QColor("#1E1E2F"));

    QFont mono("Consolas");
    mono.setPointSize(10);
    painter.setFont(mono);

    if (!rootNode) {
        painter.setPen(QColor("#ECF0F1"));
        painter.drawText(rect(), Qt::AlignCenter, "No tree to display");
        return;
    }

    painter.setPen(QPen(QColor("#C8A2F7"), 1));
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

    for (const auto& it : nodePositions) {
        drawNode(painter, it.first, it.second.x, it.second.y);
    }
}

void TreeVisualizer::drawNode(QPainter& painter,
                              SuffixNode* node,
                              int x,
                              int y)
{
    QColor fillColor = node->suffix_index != -1 ? QColor("#C8A2F7") : QColor("#9B59B6");
    painter.setBrush(fillColor);
    painter.setPen(QPen(QColor("#ECF0F1"), 1));
    painter.drawEllipse(QPoint(x, y), NODE_RADIUS, NODE_RADIUS);

    if (node->suffix_index != -1) {
        painter.setPen(QColor("#1E1E2F"));
        painter.drawText(QRect(x - NODE_RADIUS, y - NODE_RADIUS,
                               NODE_RADIUS * 2, NODE_RADIUS * 2),
                         Qt::AlignCenter,
                         QString::number(node->suffix_index));
    }
}

void TreeVisualizer::drawEdge(QPainter& painter,
                              int x1, int y1,
                              int x2, int y2,
                              const QString& label)
{
    QPainterPath path;
    path.moveTo(x1, y1 + NODE_RADIUS);
    int midY = (y1 + y2) / 2;
    path.cubicTo(x1, midY, x2, midY, x2, y2 - NODE_RADIUS);
    painter.setPen(QPen(QColor("#C8A2F7"), 1));
    painter.drawPath(path);

    if (!label.isEmpty()) {
        int mx = (x1 + x2) / 2;
        int my = midY - 5;
        painter.setPen(QColor("#ECF0F1"));
        painter.drawText(mx, my, label);
    }
}


QString TreeVisualizer::getEdgeLabel(SuffixNode* node) {
    if (!node || node->start < 0) return "";

    int start = node->start;
    int end = node->end ? *node->end : (int)treeText.size() - 1;
    if (start > end) return "";

    int len = end - start + 1;
    if (len > 8) return QString::fromStdString(treeText.substr(start, 8)) + "...";

    return QString::fromStdString(treeText.substr(start, len));
}


QSize TreeVisualizer::sizeHint() const {
    return QSize(width(), treeHeight);
}
