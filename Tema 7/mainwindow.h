#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "graphmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void nextIteration();
    void reset();

private:
    Ui::MainWindow *ui;
    GraphManager *gm;
    int totalFlow;
    bool isFinished;
    std::vector<std::pair<int, int>> minCutEdges;
    QLabel *infoLabel;
    QPushButton *btnNext;

    void setupGraph();
};

#endif
