#ifndef EDGE_H
#define EDGE_H
#include "node.h"

class Edge
{
    Node* m_first;
    Node* m_second;

public:
    Edge(Node* f, Node* s);
    Node* getFirst() const;
    Node* getSecond() const;
    void setFirst(Node* n);
    void setSecond(Node* n);
};

#endif // EDGE_H
