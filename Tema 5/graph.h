#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include "edge.h"
#include "kdtree.h"
#include <vector>
#include <QString>

class Graph
{
public:
    Graph();

    void addNode(QPoint p);
    void addEdge(Node f, Node s);

    void addParsedNode(int id, int x, int y);
    void addParsedEdge(int idSource, int idDest);
    void clear();

    const std::vector<Node>& getNodes() const;
    const std::vector<Edge>& getEdges() const;
    const std::vector<std::vector<int>>& getAdjacencyMatrix() const;

    bool edgeExists(Node f, Node s) const;
    void updateNodePosition(const Node& updatedNode);

    Node getNodeAtPos(QPoint p, double radius) const;
    void rebuildSpatialIndex();

    void setDirected(bool directed);
    bool isDirected() const;

    bool loadFromXML(const QString& filepath);

private:
    std::vector<Node> m_nodes;
    std::vector<Edge> m_edges;
    std::vector<std::vector<int>> m_adjacencyMatrix;

    KDTree m_kdTree;
    bool m_isDirected = false;

    void resizeMatrix(int newSize);
    void updateMatrixEdge(int uIdx, int vIdx, int val);
};

#endif
