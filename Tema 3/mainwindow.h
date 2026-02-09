#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <graph.h>
#include <QMouseEvent>
#include <QPaintEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void mouseReleaseEvent(QMouseEvent* m) override;
    void paintEvent(QPaintEvent* e) override;
    void mousePressEvent(QMouseEvent* m) override;
    void mouseMoveEvent(QMouseEvent* m) override;
private:
    Ui::MainWindow *ui;
    Graph m_graph;
    Node* m_first;
    bool m_isOriented = false;
    void saveMatrixToFile();
    bool m_isDragging = false;
    Node* m_draggedNode;

private slots:
    void on_radioOrientat_toggled(bool checked);
};
#endif // MAINWINDOW_H
