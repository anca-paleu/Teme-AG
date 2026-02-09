#include "mainwindow.h"
#include <QPainter>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_firstSelected(nullptr), m_draggedNode(nullptr), m_isDragging(false)
{
    resize(900, 600);
    m_graph = new Graph();

    // --- BUTONUL ---
    m_btnOriented = new QPushButton("Graf: NEORIENTAT", this);
    m_btnOriented->setGeometry(10, 10, 160, 40);
    m_btnOriented->setCheckable(true);
    m_btnOriented->setFocusPolicy(Qt::NoFocus);
    connect(m_btnOriented, &QPushButton::clicked, this, &MainWindow::toggleOrientation);
}

MainWindow::~MainWindow() {
    delete m_graph;
}

void MainWindow::toggleOrientation() {
    bool isOriented = m_btnOriented->isChecked();

    if(isOriented) {
        m_btnOriented->setText("Graf: ORIENTAT");
        m_graph->setOriented(true);
    } else {
        m_btnOriented->setText("Graf: NEORIENTAT");
        m_graph->setOriented(false);
    }

    // Reset culori
    for (auto n : m_graph->getNodes()) n->setColor(Qt::white);
    update();
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Space) {
        if (m_graph->isOriented()) {
            // Tema 4.2: graf condensat
            Graph* condensed = m_graph->solveStronglyConnectedComponents();
            if (condensed) {
                delete m_graph;
                m_graph = condensed;
                m_firstSelected = nullptr;
            }
        } else {
            // Tema 4.1: componente conexe colorate
            m_graph->solveConnectedComponents();
        }
        update();
    } else {
        QMainWindow::keyPressEvent(e);
    }
}

void MainWindow::drawEdge(QPainter& p, const QPoint& p1, const QPoint& p2, bool isOvalStart, bool isOvalEnd) {
    QLineF line(p1, p2);
    double angle = std::atan2(line.dy(), line.dx());

    double rX_start = isOvalStart ? 40 : 20;
    double rY_start = 20;
    double rX_end = isOvalEnd ? 40 : 20;
    double rY_end = 20;

    QPointF startP(p1.x() + rX_start * cos(angle), p1.y() + rY_start * sin(angle));
    QPointF endP(p2.x() - rX_end * cos(angle), p2.y() - rY_end * sin(angle));

    p.drawLine(startP, endP);

    if (m_graph->isOriented()) {
        double arrowSize = 10;
        QPointF a1(endP.x() - arrowSize * cos(angle - M_PI/6), endP.y() - arrowSize * sin(angle - M_PI/6));
        QPointF a2(endP.x() - arrowSize * cos(angle + M_PI/6), endP.y() - arrowSize * sin(angle + M_PI/6));
        QPolygonF arrowHead;
        arrowHead << endP << a1 << a2;
        p.setBrush(Qt::black);
        p.drawPolygon(arrowHead);
    }
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    auto& nodes = m_graph->getNodes();
    auto& adj = m_graph->getAdjacencyMatrix();
    int N = nodes.size();

    // 1. Muchii
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (adj[i][j]) {
                drawEdge(p, nodes[i]->getCoord(), nodes[j]->getCoord(), nodes[i]->isOval(), nodes[j]->isOval());
            }
        }
    }

    // 2. Noduri
    for (auto n : nodes) {
        p.setBrush(n->getColor());
        p.setPen(Qt::black);

        if (n->isOval()) {
            QRect rect(n->getX() - 40, n->getY() - 20, 80, 40);
            p.drawEllipse(rect);
            p.drawText(rect, Qt::AlignCenter, n->getText());
        } else {
            QRect rect(n->getX() - 20, n->getY() - 20, 40, 40);
            p.drawEllipse(rect);
            p.drawText(rect, Qt::AlignCenter, n->getText());
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *m) {
    if (m->button() == Qt::LeftButton) {
        Node* clicked = m_graph->getNodeAt(m->pos());
        if (clicked) {
            if (!m_firstSelected) {
                m_firstSelected = clicked;
                m_firstSelected->setColor(Qt::green);
                m_draggedNode = clicked;
            } else {
                if (m_firstSelected != clicked) {
                    m_graph->addEdge(m_firstSelected, clicked);
                }
                m_firstSelected->setColor(Qt::white);
                m_firstSelected = nullptr;
            }
        } else {
            if (m_firstSelected) { m_firstSelected->setColor(Qt::white); m_firstSelected = nullptr; }
            m_graph->addNode(m->pos());
        }
        update();
    } else if (m->button() == Qt::RightButton) {
        if (m_firstSelected) { m_firstSelected->setColor(Qt::white); m_firstSelected = nullptr; update(); }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *m) {
    if (m_draggedNode && (m->buttons() & Qt::LeftButton)) {
        m_draggedNode->setCoord(m->pos());
        m_isDragging = true;
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *) {
    m_isDragging = false;
    m_draggedNode = nullptr;
}
