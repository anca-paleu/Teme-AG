#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "graph.h"
#include <QMouseEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent* e) override;
    void mousePressEvent(QMouseEvent* m) override;
    void mouseReleaseEvent(QMouseEvent* m) override;
    void mouseMoveEvent(QMouseEvent* m) override;
    void keyPressEvent(QKeyEvent* e) override;

private slots:
    void toggleOrientation();

private:
    Graph* m_graph;
    Node* m_firstSelected;
    Node* m_draggedNode;
    bool m_isDragging;

    QPushButton* m_btnOriented;

    // helper pentru desen
    void drawEdge(QPainter& p, const QPoint& p1, const QPoint& p2, bool isOvalStart, bool isOvalEnd);
};

#endif // MAINWINDOW_H
