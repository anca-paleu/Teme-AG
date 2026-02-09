#include "mainwindow.h"
#include "qevent.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <fstream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_first = nullptr;
    m_draggedNode = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *m)
{
    if (m->button() == Qt::LeftButton)
    {
        m_draggedNode = m_graph.getNodeAt(m->pos());
        if (m_draggedNode != nullptr)
            m_isDragging = true;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *m)
{
    if (m_draggedNode != nullptr && m_isDragging && (m->buttons() & Qt::LeftButton))
    {
        QPoint new_pos = m->pos();
        const auto& nodes = m_graph.getNodes();
        bool position_free = true;

        for (const auto* n : nodes)
        {
            if (n == m_draggedNode) continue;
            if (std::abs(n->getX() - new_pos.x()) < 20 &&
                std::abs(n->getY() - new_pos.y()) < 20)
            {
                position_free = false;
                break;
            }
        }

        if (position_free)
        {
            m_draggedNode->setCoord(new_pos);
            update();
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *m)
{
    if (m_isDragging)
    {
        m_isDragging = false;
        m_draggedNode = nullptr;
        saveMatrixToFile();
        return;
    }

    if (m->button() == Qt::RightButton)
    {
        QPoint new_pos = m->pos();
        if (m_graph.getNodeAt(new_pos) == nullptr)
        {
            m_graph.addNode(new_pos);
            update();
            saveMatrixToFile();
        }
    }
    else if (m->button() == Qt::LeftButton)
    {
        Node* selected = m_graph.getNodeAt(m->pos());

        if (selected != nullptr)
        {
            if (m_first != nullptr)
            {
                if (m_first == selected)
                {
                    m_first = nullptr;
                    update();
                    return;
                }

                if (!m_graph.edgeExists(m_first, selected))
                {
                    m_graph.addEdge(m_first, selected);
                    update();
                    saveMatrixToFile();
                }
                m_first = nullptr;
                update();
            }
            else
            {
                m_first = selected;
                update();
            }
        }
        else
        {
            m_first = nullptr;
            update();
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Desenăm muchiile
    const auto& edges = m_graph.getEdges();
    for(auto* ed : edges)
    {
        Node* first = ed->getFirst();
        Node* second = ed->getSecond();
        if (!first || !second) continue;

        QPoint p1(first->getX(), first->getY());
        QPoint p2(second->getX(), second->getY());

        if (m_isOriented)
        {
            double NODE_RADIUS_OFFSET = 10.0;
            double ANGLE_DEG = 30.0;
            double PI_DEGREES = 180.0;
            double ANGLE_RAD = ANGLE_DEG / PI_DEGREES * M_PI;
            double dx = p2.x() - p1.x();
            double dy = p2.y() - p1.y();
            double angle = std::atan2(-dy, -dx);
            double arrowSize = 10.0;

            QPointF p_end(p2.x() + NODE_RADIUS_OFFSET * std::cos(angle),
                          p2.y() + NODE_RADIUS_OFFSET * std::sin(angle));

            p.drawLine(p1, p_end.toPoint());

            QPointF arrowP1(p_end.x() + arrowSize * std::cos(angle + ANGLE_RAD),
                            p_end.y() + arrowSize * std::sin(angle + ANGLE_RAD));

            QPointF arrowP2(p_end.x() + arrowSize * std::cos(angle - ANGLE_RAD),
                            p_end.y() + arrowSize * std::sin(angle - ANGLE_RAD));

            p.drawLine(p_end.toPoint(), arrowP1.toPoint());
            p.drawLine(p_end.toPoint(), arrowP2.toPoint());
        }
        else
        {
            p.drawLine(p1, p2);
        }
    }

    // Desenăm nodurile fără galben
    const auto& nodes = m_graph.getNodes();
    for(auto* n : nodes)
    {
        QRect r(n->getX() -10, n->getY()-10, 20, 20);
        p.setBrush(Qt::white);
        p.drawEllipse(r);

        QString s = QString::number(n->getIndex());
        p.drawText(r, Qt::AlignCenter, s);
    }
}

void MainWindow::saveMatrixToFile()
{
    std::ofstream fout("matrice.txt");
    if (!fout.is_open()) return;

    auto matrix = m_graph.getAdjacencyMatrix(m_isOriented);
    int N = matrix.size();
    fout << N << "\n";

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            fout << matrix[i][j];
            if (j < N - 1) fout << " ";
        }
        fout << "\n";
    }
    fout.close();
}

void MainWindow::on_radioOrientat_toggled(bool checked)
{
    m_isOriented = checked;
    update();
    saveMatrixToFile();
}
