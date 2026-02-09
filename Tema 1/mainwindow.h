#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QCheckBox>
#include "graph.h"
#include <QMouseEvent>
#include <QPaintEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent* e) override;
    void mousePressEvent(QMouseEvent* m) override;
    void mouseMoveEvent(QMouseEvent* m) override;
    void mouseReleaseEvent(QMouseEvent* m) override;
    void keyPressEvent(QKeyEvent* e) override;

private slots:
    void onOrientationChanged(int state);
    void onReset();

private:
    Graph* m_graph; // Folosim pointer ca sa il putem inlocui usor cu cel condensat

    Node* m_firstSelected; // Pentru tragere muchii
    Node* m_draggedNode;   // Pentru mutare noduri
    bool m_isDragging;

    // UI Controls
    QCheckBox* m_chkOriented;
    QPushButton* m_btnReset;
};

#endif // MAINWINDOW_H
