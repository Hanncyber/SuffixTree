#ifndef EMPLOYEETREEVISUALIZER_H
#define EMPLOYEETREEVISUALIZER_H

#include <QWidget>
#include <QPainter>
#include <QScrollArea>

#include <map>

struct EmployeeNodePosition {
    int x, y;
    int employeeIndex;
    char employeeLabel;
    int rating;
};

class EmployeeTreeVisualizer : public QWidget {
    Q_OBJECT

public:
    explicit EmployeeTreeVisualizer(QWidget *parent = nullptr);
    void setEmployeeData(int numEmployees, int* const* tree, const int* childCount, const int* rating);
    void updateEmployeeData(const int* rating);
    void clear();

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private:
    void calculatePositions();
    int calculateSubtreePositions(int employeeIndex, int xOffset, int depth = 0);
    void drawNode(QPainter& painter, int x, int y, char label, int rating, bool isRoot);
    void drawEdge(QPainter& painter, int x1, int y1, int x2, int y2);
    char indexToChar(int index) const;

    int numEmployees;
    int* const* employeeTree;
    const int* childCount;
    const int* employeeRating;
    
    std::map<int, EmployeeNodePosition> nodePositions;
    int treeWidth;
    int treeHeight;

    static const int NODE_WIDTH;
    static const int NODE_HEIGHT;
    static const int VERTICAL_SPACING;
    static const int MIN_HORIZONTAL_SPACING;
};

#endif // EMPLOYEETREEVISUALIZER_H
