#ifndef TREEVISUALIZER_H
#define TREEVISUALIZER_H

#include <QWidget>
#include <QPainter>
#include <QScrollArea>
#include <vector>
#include <map>
#include "SuffixTree.h"

struct NodePosition {
    int x, y;
    SuffixNode* node;
};

class TreeVisualizer : public QWidget {
    Q_OBJECT

public:
    explicit TreeVisualizer(SuffixNode* root, const std::string& text, QWidget *parent = nullptr);
    void setTree(SuffixNode* root, const std::string& text);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private:
    void calculatePositions();
    int calculateSubtreePositions(SuffixNode* node, int xOffset, int depth, QFontMetrics &fm);
    int getTreeDepth(SuffixNode* node);
    void drawNode(QPainter& painter, SuffixNode* node, int x, int y);
    void drawEdge(QPainter& painter, int x1, int y1, int x2, int y2, const QString& label);
    QString getEdgeLabel(SuffixNode* node);
    QColor getNodeColor(SuffixNode* node);

    SuffixNode* rootNode;
    std::string treeText;
    std::map<SuffixNode*, NodePosition> nodePositions;
    int treeWidth;
    int treeHeight;

    // remove initialization here
    static const int NODE_RADIUS;
    static const int VERTICAL_SPACING;
    static const int MIN_HORIZONTAL_SPACING;
};

#endif // TREEVISUALIZER_H
