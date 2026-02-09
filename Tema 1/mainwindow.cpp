#include "mainwindow.h"
#include <QPainter>
#include <cmath>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_firstSelected(nullptr), m_draggedNode(nullptr), m_isDragging(false)
{
    resize(800, 600);

    // Initializare Graf
    m_graph = new Graph();

    // Creare UI simplu
    m_chkOriented = new QCheckBox("Graf Orientat", this);
    m_chkOriented->setGeometry(10, 10, 120, 30);
    connect(m_chkOriented, &QCheckBox::stateChanged, this, &MainWindow::onOrientationChanged);

    m_btnReset = new QPushButton("Reset", this);
    m_btnReset->setGeometry(140, 10, 80, 30);
    connect(m_btnReset, &QPushButton::clicked, this, &MainWindow::onReset);
}

MainWindow::~MainWindow()
{
    delete m_graph;
}

void MainWindow::onReset() {
    m_graph->clear();
    m_firstSelected = nullptr;
    update();
}

void MainWindow::onOrientationChanged(int state) {
    bool oriented = (state == Qt::Checked);
    // Cand schimbam orientarea, e mai safe sa resetam graful sau doar culorile
    // Aici doar setam flag-ul si resetam culorile
    m_graph->setOriented(oriented);
    for(auto n : m_graph->getNodes()) n->setColor(Qt::white);
    update();
}

void MainWindow::mousePressEvent(QMouseEvent *m) {
    if (m->button() == Qt::LeftButton) {
        Node* clicked = m_graph->getNodeAt(m->pos());

        if (clicked) {
            // Logica de selectare pentru muchii
            if (!m_firstSelected) {
                m_firstSelected = clicked;
                m_firstSelected->setColor(Qt::green); // Feedback vizual selectie
                // Pregatim si dragging
                m_draggedNode = clicked;
            } else {
                // Tragem muchie
                if (m_firstSelected != clicked) {
                    m_graph->addEdge(m_firstSelected, clicked);
                    m_firstSelected->setColor(Qt::white);
                    m_firstSelected = nullptr;
                } else {
                    // Click pe acelasi nod -> deselect
                    m_firstSelected->setColor(Qt::white);
                    m_firstSelected = nullptr;
                    m_draggedNode = clicked; // Doar drag
                }
            }
        } else {
            // Click pe gol -> Nod nou
            // Daca aveam selectie, o anulam
            if (m_firstSelected) {
                m_firstSelected->setColor(Qt::white);
                m_firstSelected = nullptr;
            }
            m_graph->addNode(m->pos());
        }
        update();
    }
    else if (m->button() == Qt::RightButton) {
        // Optional: click dreapta pentru anulare selectie
        if (m_firstSelected) {
            m_firstSelected->setColor(Qt::white);
            m_firstSelected = nullptr;
            update();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *m) {
    if (m_draggedNode && (m->buttons() & Qt::LeftButton)) {
        m_draggedNode->setCoord(m->pos());
        m_isDragging = true;
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *m) {
    if (m_isDragging) {
        m_draggedNode = nullptr;
        m_isDragging = false;
    }
    // Nu resetam m_firstSelected aici pentru a permite tragerea muchiei (click-click style)
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Space) {
        if (m_graph->isOriented()) {
            // TEMA 4.2: Graf Condensat
            Graph* condensed = m_graph->getCondensedGraph();
            if (condensed) {
                delete m_graph; // Stergem vechiul graf
                m_graph = condensed; // Il punem pe cel nou
                m_firstSelected = nullptr;
            }
        } else {
            // TEMA 4.1: Componente Conexe (Colorare)
            m_graph->solveConnectedComponents();
        }
        update();
    }
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    auto& nodes = m_graph->getNodes();
    auto& adj = m_graph->getAdjacencyMatrix();
    int N = nodes.size();

    // 1. Desenare muchii
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (adj[i][j]) {
                QPoint p1 = nodes[i]->getCoord();
                QPoint p2 = nodes[j]->getCoord();

                // Calcul pentru a nu desena linia peste nod (estetica)
                QLineF line(p1, p2);
                int rad = 15;
                double angle = std::atan2(line.dy(), line.dx());

                // Offset start si end ca sa plece de pe marginea cercului
                QPointF startP(p1.x() + rad * cos(angle), p1.y() + rad * sin(angle));
                QPointF endP(p2.x() - rad * cos(angle), p2.y() - rad * sin(angle));

                p.drawLine(startP, endP);

                if (m_graph->isOriented()) {
                    // Desenare sageata
                    double arrowSize = 10;
                    QPointF arrowP1 = endP - QPointF(sin(angle + M_PI / 3) * arrowSize, cos(angle + M_PI / 3) * arrowSize);
                    QPointF arrowP2 = endP - QPointF(sin(angle - M_PI / 3) * arrowSize, cos(angle - M_PI / 3) * arrowSize); // Ajustat unghiuri pt sageata

                    // Mai simplu cu rotatie standard:
                    QPointF a1(endP.x() - arrowSize * cos(angle - M_PI/6), endP.y() - arrowSize * sin(angle - M_PI/6));
                    QPointF a2(endP.x() - arrowSize * cos(angle + M_PI/6), endP.y() - arrowSize * sin(angle + M_PI/6));

                    QPolygonF arrowHead;
                    arrowHead << endP << a1 << a2;
                    p.drawPolygon(arrowHead);
                }
            }
        }
    }

    // 2. Desenare noduri
    for (auto n : nodes) {
        int r = 15; // Raza
        QRect rect(n->getX() - r, n->getY() - r, 2*r, 2*r);

        p.setBrush(n->getColor());
        p.setPen(Qt::black);
        p.drawEllipse(rect);

        // Daca avem text lung (graf condensat), ajustam desenarea
        if (n->getText().length() > 2) {
            // Desenam textul langa nod sau marim nodul. Aici il punem peste.
            // Putem desena un dreptunghi rounded daca e text lung
            QRect textRect(n->getX() - 40, n->getY() - r, 80, 2*r);
            p.drawText(textRect, Qt::AlignCenter, n->getText());
        } else {
            p.drawText(rect, Qt::AlignCenter, n->getText());
        }
    }
}
