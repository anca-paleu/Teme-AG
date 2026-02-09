#include "mainwindow.h"
#include "Dijkstra.h"
#include <QPainter>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), kdtree(nullptr)
{
    map.loadFromXML("Harta_Luxemburg.xml");
    if (!map.nodes.empty()) {
        kdtree = new KDTree(map);
        resize(800, 600);
        fitMapToScreen();
    }
    else {
        qDebug() << "Harta este goala. Verificati continutul fisierului XML.";
    }
}

MainWindow::~MainWindow() {
    delete kdtree;
}

void MainWindow::fitMapToScreen() {
    if (map.nodes.empty()) return;

    double mapW = map.maxLon - map.minLon;
    double mapH = map.maxLat - map.minLat;

    double scaleX = (width() * 0.9) / mapW;
    double scaleY = (height() * 0.9) / mapH;

    scale = std::min(scaleX, scaleY);

    offsetX = (width() - mapW * scale) / 2.0 - map.minLon * scale;
    offsetY = (height() - mapH * scale) / 2.0 + map.maxLat * scale;
}

QPointF MainWindow::mapToScreen(double lon, double lat) {
    double x = lon * scale + offsetX;
    double y = -lat * scale + offsetY;
    return QPointF(x, y);
}

QPointF MainWindow::screenToMap(int x, int y) {
    double lon = (x - offsetX) / scale;
    double lat = -(y - offsetY) / scale;
    return QPointF(lon, lat);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), Qt::white);

    if (map.nodes.empty()) return;

    painter.setPen(QPen(QColor(200, 200, 200), 1));
    for (size_t u = 0; u < map.adj.size(); ++u) {
        if (map.nodes.size() <= u) continue;
        QPointF p1 = mapToScreen(map.nodes[u].longitude, map.nodes[u].latitude);

        for (const auto& edge : map.adj[u]) {
            if (map.nodes.size() <= (size_t)edge.to) continue;
            QPointF p2 = mapToScreen(map.nodes[edge.to].longitude, map.nodes[edge.to].latitude);
            painter.drawLine(p1, p2);
        }
    }

    if (!currentPath.empty()) {
        QPen pathPen(Qt::red, 2);
        painter.setPen(pathPen);
        for (size_t i = 0; i < currentPath.size() - 1; ++i) {
            int u = currentPath[i];
            int v = currentPath[i+1];
            QPointF p1 = mapToScreen(map.nodes[u].longitude, map.nodes[u].latitude);
            QPointF p2 = mapToScreen(map.nodes[v].longitude, map.nodes[v].latitude);
            painter.drawLine(p1, p2);
        }
    }

    double radius = 4.0;
    if (startNode != -1) {
        painter.setBrush(Qt::green);
        painter.setPen(Qt::NoPen);
        QPointF p = mapToScreen(map.nodes[startNode].longitude, map.nodes[startNode].latitude);
        painter.drawEllipse(p, radius, radius);
    }

    if (endNode != -1) {
        painter.setBrush(Qt::blue);
        painter.setPen(Qt::NoPen);
        QPointF p = mapToScreen(map.nodes[endNode].longitude, map.nodes[endNode].latitude);
        painter.drawEllipse(p, radius, radius);
    }
}

void MainWindow::wheelEvent(QWheelEvent *event) {
    double zoomFactor = 1.1;
    if (event->angleDelta().y() < 0) {
        scale /= zoomFactor;
        offsetX = event->position().x() - (event->position().x() - offsetX) / zoomFactor;
        offsetY = event->position().y() - (event->position().y() - offsetY) / zoomFactor;
    } else {
        scale *= zoomFactor;
        offsetX = event->position().x() - (event->position().x() - offsetX) * zoomFactor;
        offsetY = event->position().y() - (event->position().y() - offsetY) * zoomFactor;
    }
    update();
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (!kdtree) return;
        QPointF coord = screenToMap(event->pos().x(), event->pos().y());
        int clickedNode = kdtree->findNearest(coord.x(), coord.y());

        if (startNode == -1) {
            startNode = clickedNode;
            endNode = -1;
            currentPath.clear();
        } else if (endNode == -1) {
            endNode = clickedNode;
            currentPath = Dijkstra::findPath(map, startNode, endNode);
        } else {
            startNode = clickedNode;
            endNode = -1;
            currentPath.clear();
        }
        update();
    } else if (event->button() == Qt::RightButton) {
        isDragging = true;
        lastMousePos = event->pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (isDragging) {
        int dx = event->pos().x() - lastMousePos.x();
        int dy = event->pos().y() - lastMousePos.y();
        offsetX += dx;
        offsetY += dy;
        lastMousePos = event->pos();
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        isDragging = false;
    }
}
