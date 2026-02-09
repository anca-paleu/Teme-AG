#include "graph.h"
#include <algorithm>
#include <cmath>

Graph::Graph() {}

Graph::~Graph()
{
    for (auto* edge : m_edges) {
        delete edge;
    }
    for (auto* node : m_nodes) {
        delete node;
    }
}

Node* Graph::addNode(QPoint p)
{
    Node* n = new Node(m_nodes.size() + 1, p);
    m_nodes.push_back(n);
    return n;
}

Edge* Graph::addEdge(Node* f, Node* s)
{
    Edge* e = new Edge(f, s);
    m_edges.push_back(e);
    return e;
}

const std::vector<Node*>& Graph::getNodes() const
{
    return m_nodes;
}

const std::vector<Edge*>& Graph::getEdges() const
{
    return m_edges;
}

bool Graph::edgeExists(Node* f, Node* s) const
{
    for (const auto* edge : m_edges) {
        if (edge->getFirst() == f && edge->getSecond() == s) {
            return true;
        }
    }
    return false;
}

std::vector<std::vector<int>> Graph::getAdjacencyMatrix(bool is_oriented) const
{
    int N = m_nodes.size();
    std::vector<std::vector<int>> matrix(N, std::vector<int>(N, 0));
    for (const auto* edge : m_edges) {
        int i = edge->getFirst()->getIndex() - 1;
        int j = edge->getSecond()->getIndex() - 1;
        if (i >= 0 && i < N && j >= 0 && j < N) {
            matrix[i][j] = 1;
            if (!is_oriented) {
                matrix[j][i] = 1;
            }
        }
    }
    return matrix;
}

Node* Graph::getNodeAt(QPoint p) const
{
    for (auto* n : m_nodes)
    {
        if (std::abs(n->getX() - p.x()) < 10 &&
            std::abs(n->getY() - p.y()) < 10)
        {
            return n;
        }
    }
    return nullptr;
}
