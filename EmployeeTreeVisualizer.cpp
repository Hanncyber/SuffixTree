#include "EmployeeTreeVisualizer.h"
#include <QPainter>
#include <QPainterPath>
#include <QFontMetrics>
#include <algorithm>

const int EmployeeTreeVisualizer::NODE_WIDTH = 60;
const int EmployeeTreeVisualizer::NODE_HEIGHT = 50;
const int EmployeeTreeVisualizer::VERTICAL_SPACING = 100;
const int EmployeeTreeVisualizer::MIN_HORIZONTAL_SPACING = 80;

EmployeeTreeVisualizer::EmployeeTreeVisualizer(QWidget *parent)
    : QWidget(parent),
    numEmployees(0),
    employeeTree(nullptr),
    childCount(nullptr),
    employeeRating(nullptr),
    treeWidth(0),
    treeHeight(0)
{
    setMinimumSize(800, 600);
}

void EmployeeTreeVisualizer::setEmployeeData(int numEmployees, int* const* tree, const int* childCount, const int* rating) {
    if (numEmployees <= 0 || !tree || !childCount || !rating) {
        this->numEmployees = 0;
        this->employeeTree = nullptr;
        this->childCount = nullptr;
        this->employeeRating = nullptr;
        nodePositions.clear();
        update();
        return;
    }

    this->numEmployees = numEmployees;
    this->employeeTree = tree;
    this->childCount = childCount;
    this->employeeRating = rating;
    nodePositions.clear();
    calculatePositions();
    update();
}

void EmployeeTreeVisualizer::updateEmployeeData(const int* rating) {
    this->employeeRating = rating;
    update();
}

void EmployeeTreeVisualizer::clear() {
    numEmployees = 0;
    employeeTree = nullptr;
    childCount = nullptr;
    employeeRating = nullptr;
    nodePositions.clear();
    update();
}

void EmployeeTreeVisualizer::calculatePositions() {
    if (numEmployees == 0 || !employeeTree) {
        treeWidth = 0;
        treeHeight = 0;
        return;
    }

    nodePositions.clear();

    int totalWidth = calculateSubtreePositions(0, 0);

    for (auto& it : nodePositions) {
        it.second.x += 50;
    }

    treeWidth = totalWidth + 100;
    treeHeight = 600;

    setMinimumSize(treeWidth, treeHeight);
    resize(treeWidth, treeHeight);
}

int EmployeeTreeVisualizer::calculateSubtreePositions(int employeeIndex, int xOffset, int depth) {
    if (employeeIndex < 0 || employeeIndex >= numEmployees) return 0;
    if (!employeeTree || !childCount || !employeeRating) return 0;

    std::vector<int> children;
    int numChildren = childCount[employeeIndex];

    if (numChildren < 0 || numChildren >= numEmployees) {
        return MIN_HORIZONTAL_SPACING;
    }

    for (int i = 0; i < numChildren; ++i) {
        int childIndex = employeeTree[employeeIndex][i];
        if (childIndex >= 0 && childIndex < numEmployees) {
            children.push_back(childIndex);
        }
    }

    int subtreeWidth = 0;
    std::vector<int> childCenters;

    for (int child : children) {
        int w = calculateSubtreePositions(child, xOffset + subtreeWidth, depth + 1);
        childCenters.push_back(xOffset + subtreeWidth + w / 2);
        subtreeWidth += w + MIN_HORIZONTAL_SPACING;
    }

    if (children.empty()) {
        subtreeWidth = MIN_HORIZONTAL_SPACING;
        nodePositions[employeeIndex] = {
            xOffset,
            40 + depth * VERTICAL_SPACING,
            employeeIndex,
            indexToChar(employeeIndex),
            employeeRating[employeeIndex]
        };
    } else {
        int parentX = childCenters.front() + (childCenters.back() - childCenters.front()) / 2;
        nodePositions[employeeIndex] = {
            parentX,
            40 + depth * VERTICAL_SPACING,
            employeeIndex,
            indexToChar(employeeIndex),
            employeeRating[employeeIndex]
        };
        for (int child : children) {
            nodePositions[child].y = 40 + (depth + 1) * VERTICAL_SPACING;
        }
    }

    return subtreeWidth;
}

void EmployeeTreeVisualizer::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(250, 250, 255));

    if (numEmployees == 0 || !employeeTree) {
        painter.setPen(Qt::gray);
        QFont font = painter.font();
        font.setPointSize(14);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter, "Employee hierarchy tree will appear here after initialization");
        return;
    }

    QFont mono("Consolas");
    mono.setPointSize(10);
    painter.setFont(mono);
    for (const auto& it : nodePositions) {
        int employeeIndex = it.first;
        const EmployeeNodePosition& pos = it.second;

        for (int i = 0; i < childCount[employeeIndex]; ++i) {
            if (!employeeTree[employeeIndex])
                continue;
            int childIndex = employeeTree[employeeIndex][i];
            auto childIt = nodePositions.find(childIndex);
            if (childIt != nodePositions.end()) {
                drawEdge(painter, pos.x, pos.y, childIt->second.x, childIt->second.y);
            }
        }
    }

    for (const auto& it : nodePositions) {
        const EmployeeNodePosition& pos = it.second;
        drawNode(painter, pos.x, pos.y, pos.employeeLabel, pos.rating, it.first == 0);
    }
}

void EmployeeTreeVisualizer::drawNode(QPainter& painter, int x, int y, char label, int rating, bool isRoot) {
    int halfWidth = NODE_WIDTH / 2;
    int halfHeight = NODE_HEIGHT / 2;

    QColor nodeColor = isRoot ? QColor(155, 89, 182) : QColor(52, 152, 219);

    painter.setBrush(nodeColor);
    painter.setPen(QPen(Qt::black, 2));
    painter.drawRoundedRect(x - halfWidth, y - halfHeight, NODE_WIDTH, NODE_HEIGHT, 8, 8);

    painter.setPen(Qt::white);
    QFont labelFont = painter.font();
    labelFont.setBold(true);
    labelFont.setPointSize(14);
    painter.setFont(labelFont);
    painter.drawText(QRect(x - halfWidth, y - halfHeight, NODE_WIDTH, NODE_HEIGHT / 2 + 5),
                     Qt::AlignCenter, QString(label));

    labelFont.setPointSize(10);
    labelFont.setBold(false);
    painter.setFont(labelFont);
    painter.drawText(QRect(x - halfWidth, y - halfHeight + NODE_HEIGHT / 2 - 5, NODE_WIDTH, NODE_HEIGHT / 2 + 5),
                     Qt::AlignCenter, QString("R: %1").arg(rating));
}

void EmployeeTreeVisualizer::drawEdge(QPainter& painter, int x1, int y1, int x2, int y2) {
    QPainterPath path;
    path.moveTo(x1, y1 + NODE_HEIGHT / 2);
    int midY = (y1 + y2) / 2;
    path.cubicTo(x1, midY, x2, midY, x2, y2 - NODE_HEIGHT / 2);
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.drawPath(path);
}

char EmployeeTreeVisualizer::indexToChar(int index) const {
    if (index < 0 || index >= numEmployees) return '?';
    if (index == 0) return 'H';
    if (index >= 27) return '?';
    return 'A' + index - 1;
}

QSize EmployeeTreeVisualizer::sizeHint() const {
    return QSize(treeWidth, treeHeight);
}
