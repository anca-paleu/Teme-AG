#include "node.h"

Node::Node() : m_coord(0, 0), m_index(0) {}

Node::Node(int index, const QPoint& coord) : m_coord(coord), m_index(index) {}

void Node::setIndex(int index) {
    m_index = index;
}

void Node::setCoord(const QPoint& coord) {
    m_coord = coord;
}

int Node::getIndex() const {
    return m_index;
}

QPoint Node::getCoord() const {
    return m_coord;
}

int Node::getX() const {
    return m_coord.x();
}

int Node::getY() const {
    return m_coord.y();
}
