#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gm(nullptr)
    , totalFlow(0)
    , isFinished(false)
{
    ui->setupUi(this);

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->addStretch();

    infoLabel = new QLabel("Flux: 0", this);
    infoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(infoLabel);

    btnNext = new QPushButton("Pasul Urmator (Retea Reziduala)", this);
    connect(btnNext, &QPushButton::clicked, this, &MainWindow::nextIteration);
    layout->addWidget(btnNext);

    QPushButton *btnReset = new QPushButton("Reset", this);
    connect(btnReset, &QPushButton::clicked, this, &MainWindow::reset);
    layout->addWidget(btnReset);

    setCentralWidget(central);
    setupGraph();
}

MainWindow::~MainWindow() {
    delete ui;
    delete gm;
}

void MainWindow::setupGraph() {
    if (gm) delete gm;
    gm = new GraphManager(6);

    gm->addEdge(0, 1, 10);
    gm->addEdge(0, 2, 7);
    gm->addEdge(1, 3, 9);
    gm->addEdge(1, 2, 12);
    gm->addEdge(2, 4, 6);
    gm->addEdge(3, 5, 3);
    gm->addEdge(3, 4, 7);
    gm->addEdge(4, 5, 14);

    totalFlow = 0;
    isFinished = false;
    minCutEdges.clear();
    infoLabel->setText("Flux curent: 0");
    btnNext->setEnabled(true);
    update();
}

void MainWindow::nextIteration() {
    if (isFinished) return;

    std::vector<int> parent(6);
    std::vector<int> edgeIdx(6);

    if (gm->findPath(0, 5, parent, edgeIdx)) {
        int pathFlow = 2147483647;
        for (int v = 5; v != 0; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, gm->adj[u][edgeIdx[v]].capacity - gm->adj[u][edgeIdx[v]].flow);
        }

        for (int v = 5; v != 0; v = parent[v]) {
            int u = parent[v];
            gm->adj[u][edgeIdx[v]].flow += pathFlow;
            int revIdx = gm->adj[u][edgeIdx[v]].rev;
            gm->adj[v][revIdx].flow -= pathFlow;
        }

        totalFlow += pathFlow;
        infoLabel->setText(QString("Flux curent: %1").arg(totalFlow));
    } else {
        isFinished = true;
        btnNext->setEnabled(false);
        minCutEdges = gm->computeMinCut(0);
        infoLabel->setText(QString("Flux Maxim: %1 | Taietura Minima Gasita").arg(totalFlow));
    }
    update();
}

void MainWindow::reset() {
    setupGraph();
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int n = 6;
    std::vector<QPoint> pts;
    for (int i = 0; i < n; i++) {
        int x = width()/2 + 200 * cos(2 * M_PI * i / n);
        int y = height()/2 - 100 + 150 * sin(2 * M_PI * i / n);
        pts.push_back(QPoint(x, y));
    }

    for (int u = 0; u < n; u++) {
        for (auto &e : gm->adj[u]) {
            if (e.capacity > 0) {
                bool isMinCut = false;
                if (isFinished) {
                    for (auto &cut : minCutEdges) {
                        if (cut.first == u && cut.second == e.to) isMinCut = true;
                    }
                }

                QPen pen(isMinCut ? Qt::red : Qt::blue);
                pen.setWidth(isMinCut ? 3 : 1);
                painter.setPen(pen);

                painter.drawLine(pts[u], pts[e.to]);

                int mx = (pts[u].x() + pts[e.to].x()) / 2;
                int my = (pts[u].y() + pts[e.to].y()) / 2;
                painter.setPen(Qt::black);
                painter.drawText(mx, my, QString("%1/%2").arg(e.flow).arg(e.capacity));
            }
        }
    }

    for (int i = 0; i < n; i++) {
        painter.setBrush(Qt::yellow);
        painter.setPen(Qt::black);
        painter.drawEllipse(pts[i], 20, 20);
        painter.drawText(pts[i].x() - 5, pts[i].y() + 5, QString::number(i));
    }
}
