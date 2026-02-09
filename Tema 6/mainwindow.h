#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_btnLoad_clicked();
    void on_btnFloyd_clicked();
    void on_btnMST_clicked();
    void on_btnTSP_clicked();

private:
    Ui::MainWindow *ui;
    Graph graph;
    DrawMode currentMode;

    QPointF scaleCoordinates(double x, double y, double width, double height);
    void drawGraph(QPainter& painter);
};
#endif
