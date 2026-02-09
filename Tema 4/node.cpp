#include "node.h"
#include <cmath>

Node::Node(int index, const QPoint& coord, QString text)
    : m_coord(coord), m_index(index), m_text(text), m_color(Qt::white), m_isOval(false)
{
    if (m_text.isEmpty()) m_text = QString::number(index);
}

void Node::setCoord(const QPoint& coord) { m_coord = coord; }
QPoint Node::getCoord() const { return m_coord; }
int Node::getIndex() const { return m_index; }

QString Node::getText() const { return m_text; }
void Node::setText(const QString& text) { m_text = text; }

QColor Node::getColor() const { return m_color; }
void Node::setColor(const QColor& color) { m_color = color; }

bool Node::isOval() const { return m_isOval; }
void Node::setIsOval(bool oval) { m_isOval = oval; }

int Node::getX() const { return m_coord.x(); }
int Node::getY() const { return m_coord.y(); }

bool Node::isUnderMouse(const QPoint& mousePos) const {
    int dx = m_coord.x() - mousePos.x();
    int dy = m_coord.y() - mousePos.y();

    int radiusX = m_isOval ? 40 : 20;
    int radiusY = 20;

    // verificare eliptica pentru a corespunde desenului
    double val = ((double)(dx*dx) / (radiusX * radiusX)) + ((double)(dy*dy) / (radiusY * radiusY));
    return val <= 1.0;
}
