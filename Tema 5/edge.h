#ifndef EDGE_H
#define EDGE_H

#include "node.h"

class Edge
{
public:
    Edge(Node first, Node second);

    Node getFirst() const;
    Node getSecond() const;

    void setFirst(const Node& n);
    void setSecond(const Node& n);

private:
    Node m_first;
    Node m_second;
};

#endif
