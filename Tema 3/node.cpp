#include "node.h"

Node::Node(int index,QPoint coord): m_coord(coord), m_index(index){}

void Node::setIndex(int index)
{
    m_index = index;
}

void Node::setCoord(QPoint coord)
{
    m_coord = coord;
}

int Node::getIndex() const
{
    return m_index;
}

QPoint Node::getCoord() const
{
    return m_coord;
}

int Node::getX() const
{
    return m_coord.x();
}

int Node::getY() const
{
    return m_coord.y();
}
