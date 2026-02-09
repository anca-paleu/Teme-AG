#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QtGlobal>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->solveButton, &QPushButton::clicked, this, &MainWindow::on_solveButton_clicked);

    if (maze.loadMazeFromFile()) {
        maze.buildGraph();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_solveButton_clicked()
{
    maze.runBFS();
    maze.reconstructAllPaths();
    solvedPaths = maze.getPaths();

    update();
}

void MainWindow::paintEvent(QPaintEvent *event )
{
    Q_UNUSED(event);

    int numRows = maze.getNumRows();
    int numCols = maze.getNumCols();

    if (numRows == 0 || numCols == 0) {
        return;
    }

    const auto& mazeMatrix = maze.getMazeMatrix();

    QPainter painter(this);

    int topOffset = 50;
    float cellWidth = (float)width() / numCols;
    float cellHeight = (float)(height() - topOffset) / numRows;

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {

            QRectF rect(j * cellWidth,
                        i * cellHeight + topOffset,
                        cellWidth,
                        cellHeight);

            QColor cellColor;

            switch (mazeMatrix[i][j]) {
            case 0:
                cellColor = Qt::black;
                break;
            case 1:
                cellColor = Qt::white;
                break;
            case 2:
                cellColor = Qt::red;
                break;
            case 3:
                cellColor = Qt::blue;
                break;
            default:
                cellColor = Qt::gray;
                break;
            }

            painter.fillRect(rect, cellColor);
            painter.setPen(Qt::darkGray);
            painter.drawRect(rect);
        }
    }

    if (!solvedPaths.isEmpty())
    {
        const auto& nodeIdToCoord = maze.getNodeIdToCoord();

        painter.setBrush(Qt::green);
        painter.setPen(Qt::darkGray);

        for (const auto &path : std::as_const(solvedPaths))
        {
            for (const auto &nodeId : std::as_const(path))
            {
                QPoint coord = nodeIdToCoord.value(nodeId);
                int val = mazeMatrix[coord.y()][coord.x()];
                if (val == 1)
                {
                    QRectF rect(coord.x() * cellWidth,
                                coord.y() * cellHeight + topOffset,
                                cellWidth,
                                cellHeight);

                    painter.fillRect(rect, Qt::green);
                    painter.drawRect(rect);
                }
            }
        }
    }
}
