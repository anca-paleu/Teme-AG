#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <QString>
#include <QPoint>
#include "node.h"

class Graph
{
    std::vector<Node*> m_nodes;
    std::vector<std::vector<int>> m_adjMatrix;
    bool m_isOriented;

public:
    Graph();
    ~Graph();

    void setOriented(bool oriented);
    bool isOriented() const;

    void addNode(const QPoint& p, QString label = "");
    void addEdge(Node* f, Node* s);
    Node* getNodeAt(const QPoint& p);

    // oferă și versiunile non-const pentru a modifica nodurile rezultate (ex. setIsOval)
    std::vector<Node*>& getNodes();
    const std::vector<Node*>& getNodes() const;

    const std::vector<std::vector<int>>& getAdjacencyMatrix() const;
    std::vector<std::vector<int>>& getAdjacencyMatrix();

    void clear();

    // Algoritmii ceruti in tema
    void solveConnectedComponents();          // Tema 4.1
    Graph* solveStronglyConnectedComponents(); // Tema 4.2
};

#endif // GRAPH_H
