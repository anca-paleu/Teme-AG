#pragma once

#include <QMainWindow>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include "MapGraph.h"
#include "KDTree.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    MapGraph map;
    KDTree* kdtree;

    int startNode = -1;
    int endNode = -1;
    std::vector<int> currentPath;

    double scale = 1.0;
    double offsetX = 0.0;
    double offsetY = 0.0;
    bool isDragging = false;
    QPoint lastMousePos;

    QPointF mapToScreen(double lon, double lat);
    QPointF screenToMap(int x, int y);
    void fitMapToScreen();
};
