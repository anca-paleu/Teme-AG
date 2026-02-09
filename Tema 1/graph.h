#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "node.h"
#include <QPoint>
#include <QPair>

class Graph
{
    std::vector<Node*> m_nodes; // Folosim pointeri pentru a evita copierea/invalidarea
    std::vector<std::vector<int>> m_adjMatrix;
    bool m_isOriented;

public:
    Graph();
    ~Graph();

    void setOriented(bool oriented);
    bool isOriented() const;

    void addNode(const QPoint& p, QString label = "");
    void addEdge(Node* f, Node* s);

    // Returneaza nodul de la coordonatele date (sau nullptr)
    Node* getNodeAt(const QPoint& p);

    const std::vector<Node*>& getNodes() const;
    const std::vector<std::vector<int>>& getAdjacencyMatrix() const;

    void clear(); // Goleste graful

    // --- Algoritmi ---

    // TEMA 4.1: Coloreaza componentele conexe (pt graf neorientat)
    void solveConnectedComponents();

    // TEMA 4.2: Returneaza un graf NOU condensat (pt graf orientat)
    Graph* getCondensedGraph();
};

#endif // GRAPH_H
