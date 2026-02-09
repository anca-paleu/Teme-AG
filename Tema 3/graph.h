#ifndef GRAPH_H
#define GRAPH_H
#include "edge.h"
#include <vector>

class Graph
{
    std::vector<Node*> m_nodes;
    std::vector<Edge*> m_edges;
public:
    Graph();
    ~Graph();

    Node* addNode(QPoint p);
    Edge* addEdge(Node* f, Node* s);

    const std::vector<Node*>& getNodes() const;
    const std::vector<Edge*>& getEdges() const;

    bool edgeExists(Node* f, Node* s) const;
    std::vector<std::vector<int>> getAdjacencyMatrix(bool is_oriented) const;

    Node* getNodeAt(QPoint p) const;
};

#endif // GRAPH_H
