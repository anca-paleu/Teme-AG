#include "edge.h"

Edge::Edge(Node first, Node second)
{
    m_first = first;
    m_second = second;
}

Node Edge::getFirst() const
{
    return m_first;
}

Node Edge::getSecond() const
{
    return m_second;
}

void Edge::setFirst(const Node& n)
{
    m_first = n;
}

void Edge::setSecond(const Node& n)
{
    m_second = n;
}
