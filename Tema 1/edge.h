#ifndef EDGE_H
#define EDGE_H

class Node;

class Edge
{
    Node* m_first;
    Node* m_second;

public:
    Edge(Node* f, Node* s);
    Node* getFirst() const;
    Node* getSecond() const;
};

#endif // EDGE_H
