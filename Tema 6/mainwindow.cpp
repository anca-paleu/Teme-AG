#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>
#include <limits>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentMode(MODE_INITIAL)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnLoad_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Graph File", QDir::currentPath(), "Text Files (*.txt)");

    if (fileName.isEmpty()) {
        fileName = "input.txt";
    }

    qDebug() << "Incercare incarcare fisier:" << fileName;

    QFileInfo check(fileName);
    if (check.exists() && check.isFile()) {
        graph.loadFromFile(fileName.toStdString());
        currentMode = MODE_INITIAL;
        update();
    } else {
        qDebug() << "Fisierul nu a fost gasit la calea:" << check.absoluteFilePath();
    }
}

void MainWindow::on_btnFloyd_clicked()
{
    graph.runFloydWarshall();
    currentMode = MODE_COMPLETE;
    update();
}

void MainWindow::on_btnMST_clicked()
{
    graph.runKruskal();
    currentMode = MODE_MST;
    update();
}

void MainWindow::on_btnTSP_clicked()
{
    graph.generateTSP();
    currentMode = MODE_TSP;
    update();
}

QPointF MainWindow::scaleCoordinates(double x, double y, double width, double height)
{
    const auto& cities = graph.getCities();
    if (cities.empty()) return QPointF(0, 0);

    double minX = cities[0].x, maxX = cities[0].x;
    double minY = cities[0].y, maxY = cities[0].y;

    for (const auto& c : cities) {
        if (c.x < minX) minX = c.x;
        if (c.x > maxX) maxX = c.x;
        if (c.y < minY) minY = c.y;
        if (c.y > maxY) maxY = c.y;
    }

    double padding = 100.0;
    double drawWidth = width - 2 * padding;
    double drawHeight = height - 2 * padding;

    double rangeX = (maxX - minX);
    double rangeY = (maxY - minY);

    if (rangeX == 0) rangeX = 1;
    if (rangeY == 0) rangeY = 1;

    double sX = padding + ((x - minX) / rangeX) * drawWidth;
    double sY = padding + ((y - minY) / rangeY) * drawHeight;

    return QPointF(sX, sY);
}
void MainWindow::drawGraph(QPainter& painter)
{
    const auto& cities = graph.getCities();

    if (cities.empty()) {
        painter.drawText(rect(), Qt::AlignCenter, "Niciun graf incarcat.");
        return;
    }

    double w = width();
    double h = height() - 60;

    auto drawWeight = [&](QPointF p1, QPointF p2, double weight, QColor color) {
        QPointF mid = (p1 + p2) / 2;
        QString text = QString::number(weight);
        painter.setPen(color);
        painter.drawText(mid + QPointF(-10, -10), text);
    };

    if (currentMode == MODE_INITIAL) {
        painter.setPen(QPen(Qt::gray, 1, Qt::SolidLine));
        const auto& edges = graph.getInitialEdges();
        for (const auto& e : edges) {
            QPointF p1 = scaleCoordinates(cities[e.u - 1].x, cities[e.u - 1].y, w, h);
            QPointF p2 = scaleCoordinates(cities[e.v - 1].x, cities[e.v - 1].y, w, h);
            painter.drawLine(p1, p2);
            drawWeight(p1, p2, e.weight, Qt::black);
        }
        painter.setPen(Qt::black);
        painter.drawText(20, 30, "Mod: Graf Initial");
    }
    else if (currentMode == MODE_COMPLETE) {
        painter.setPen(QPen(QColor(220, 220, 220), 1, Qt::SolidLine));
        const auto& mat = graph.getAdjacencyMatrix();
        int n = cities.size();
        for (int i = 1; i <= n; ++i) {
            for (int j = i + 1; j <= n; ++j) {
                if (mat[i][j] < std::numeric_limits<double>::max()) {
                    QPointF p1 = scaleCoordinates(cities[i - 1].x, cities[i - 1].y, w, h);
                    QPointF p2 = scaleCoordinates(cities[j - 1].x, cities[j - 1].y, w, h);
                    painter.drawLine(p1, p2);
                }
            }
        }
        painter.setPen(Qt::black);
        painter.drawText(20, 30, "Mod: Graf Complet");
    }
    else if (currentMode == MODE_MST) {
        painter.setPen(QPen(QColor(230, 230, 230), 1, Qt::SolidLine));
        const auto& mat = graph.getAdjacencyMatrix();
        int n = cities.size();
        for (int i = 1; i <= n; ++i)
            for (int j = i + 1; j <= n; ++j)
                if (mat[i][j] < std::numeric_limits<double>::max()) {
                    QPointF p1 = scaleCoordinates(cities[i - 1].x, cities[i - 1].y, w, h);
                    QPointF p2 = scaleCoordinates(cities[j - 1].x, cities[j - 1].y, w, h);
                    painter.drawLine(p1, p2);
                }

        const auto& edges = graph.getMSTEdges();
        for (const auto& e : edges) {
            QPointF p1 = scaleCoordinates(cities[e.u - 1].x, cities[e.u - 1].y, w, h);
            QPointF p2 = scaleCoordinates(cities[e.v - 1].x, cities[e.v - 1].y, w, h);

            painter.setPen(QPen(Qt::blue, 3, Qt::SolidLine));
            painter.drawLine(p1, p2);
            drawWeight(p1, p2, e.weight, Qt::black);
        }
        painter.setPen(Qt::black);
        painter.drawText(20, 30, "Mod: MST Kruskal");
    }
    else if (currentMode == MODE_TSP) {
        painter.setPen(QPen(Qt::lightGray, 1, Qt::SolidLine));
        const auto& mstEdges = graph.getMSTEdges();
        for(const auto& e : mstEdges) {
            QPointF p1 = scaleCoordinates(cities[e.u - 1].x, cities[e.u - 1].y, w, h);
            QPointF p2 = scaleCoordinates(cities[e.v - 1].x, cities[e.v - 1].y, w, h);
            painter.drawLine(p1, p2);
        }

        const auto& circuit = graph.getTSPCircuit();
        if (circuit.size() > 1) {
            painter.setPen(QPen(Qt::red, 3, Qt::SolidLine));
            for (size_t i = 0; i < circuit.size() - 1; ++i) {
                int u = circuit[i];
                int v = circuit[i + 1];
                QPointF p1 = scaleCoordinates(cities[u - 1].x, cities[u - 1].y, w, h);
                QPointF p2 = scaleCoordinates(cities[v - 1].x, cities[v - 1].y, w, h);
                painter.drawLine(p1, p2);
            }
        }
        painter.setPen(Qt::black);
        painter.drawText(20, 30, "Mod: Circuit TSP");
    }

    painter.setPen(Qt::black);
    painter.setBrush(Qt::green);
    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);
    QFontMetrics fm(font);

    for (const auto& c : cities) {
        QPointF p = scaleCoordinates(c.x, c.y, w, h);

        painter.setBrush(Qt::green);
        painter.drawEllipse(p, 6, 6);

        QString name = QString::fromStdString(c.name);
        int textWidth = fm.horizontalAdvance(name);
        double textX = p.x() - (textWidth / 2);
        double textY = p.y() - 12;
        painter.setPen(Qt::black);
        painter.drawText(QPointF(textX, textY), name);
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);
    drawGraph(painter);
}
