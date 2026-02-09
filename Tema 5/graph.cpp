#include "graph.h"
#include <algorithm>
#include <QFile>
#include <QXmlStreamReader>

Graph::Graph() {}

void Graph::setDirected(bool directed)
{
    m_isDirected = directed;
    int N = m_nodes.size();
    m_adjacencyMatrix.assign(N, std::vector<int>(N, 0));
    for (const auto& e : m_edges) {
        updateMatrixEdge(e.getFirst().getIndex() - 1, e.getSecond().getIndex() - 1, 1);
    }
}

bool Graph::isDirected() const
{
    return m_isDirected;
}

void Graph::resizeMatrix(int newSize)
{
    for (auto& row : m_adjacencyMatrix) {
        row.resize(newSize, 0);
    }
    m_adjacencyMatrix.resize(newSize, std::vector<int>(newSize, 0));
}

void Graph::updateMatrixEdge(int u, int v, int val)
{
    if (u >= 0 && u < (int)m_adjacencyMatrix.size() &&
        v >= 0 && v < (int)m_adjacencyMatrix.size()) {
        m_adjacencyMatrix[u][v] = val;
        if (!m_isDirected) {
            m_adjacencyMatrix[v][u] = val;
        }
    }
}

void Graph::addNode(QPoint p)
{
    int newIndex = m_nodes.size() + 1;
    Node n(newIndex, p);
    m_nodes.push_back(n);
    resizeMatrix(newIndex);
    m_kdTree.build(m_nodes);
}

void Graph::addParsedNode(int id, int x, int y)
{
    Node n(id, QPoint(x, y));
    m_nodes.push_back(n);
    if ((int)m_adjacencyMatrix.size() < id) {
        resizeMatrix(id);
    }
}

void Graph::addParsedEdge(int idSource, int idDest)
{
    auto it1 = std::find_if(m_nodes.begin(), m_nodes.end(), [idSource](const Node& n){ return n.getIndex() == idSource; });
    auto it2 = std::find_if(m_nodes.begin(), m_nodes.end(), [idDest](const Node& n){ return n.getIndex() == idDest; });

    if (it1 != m_nodes.end() && it2 != m_nodes.end()) {
        addEdge(*it1, *it2);
    }
}

void Graph::addEdge(Node f, Node e)
{
    m_edges.push_back(Edge(f, e));
    updateMatrixEdge(f.getIndex() - 1, e.getIndex() - 1, 1);
}

const std::vector<Node>& Graph::getNodes() const { return m_nodes; }
const std::vector<Edge>& Graph::getEdges() const { return m_edges; }
const std::vector<std::vector<int>>& Graph::getAdjacencyMatrix() const { return m_adjacencyMatrix; }

bool Graph::edgeExists(Node f, Node s) const
{
    int u = f.getIndex() - 1;
    int v = s.getIndex() - 1;
    if (u >= 0 && u < (int)m_adjacencyMatrix.size() &&
        v >= 0 && v < (int)m_adjacencyMatrix.size()) {
        return m_adjacencyMatrix[u][v] == 1;
    }
    return false;
}

void Graph::updateNodePosition(const Node& updatedNode)
{
    auto it = std::find_if(m_nodes.begin(), m_nodes.end(),
                           [&updatedNode](const Node& n) {
                               return n.getIndex() == updatedNode.getIndex();
                           });

    if (it != m_nodes.end()) {
        it->setCoord(updatedNode.getCoord());
    }

    for (auto& edge : m_edges) {
        if (edge.getFirst().getIndex() == updatedNode.getIndex()) edge.setFirst(*it);
        if (edge.getSecond().getIndex() == updatedNode.getIndex()) edge.setSecond(*it);
    }

    m_kdTree.build(m_nodes);
}

Node Graph::getNodeAtPos(QPoint p, double radius) const
{
    return m_kdTree.findNearest(p, radius);
}

void Graph::rebuildSpatialIndex()
{
    m_kdTree.build(m_nodes);
}

void Graph::clear()
{
    m_nodes.clear();
    m_edges.clear();
    m_adjacencyMatrix.clear();
    m_kdTree.clear();
}

bool Graph::loadFromXML(const QString& filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    clear();
    QXmlStreamReader xml(&file);

    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartElement) {
            if (xml.name() == QString("node")) {
                int id = xml.attributes().value("id").toInt();
                int x = xml.attributes().value("x").toInt();
                int y = xml.attributes().value("y").toInt();
                addParsedNode(id, x, y);
            }
            else if (xml.name() == QString("edge")) {
                int src = xml.attributes().value("source").toInt();
                int dst = xml.attributes().value("target").toInt();
                addParsedEdge(src, dst);
            }
        }
    }

    int N = m_nodes.size();
    resizeMatrix(N);
    for(const auto& e : m_edges) {
        updateMatrixEdge(e.getFirst().getIndex()-1, e.getSecond().getIndex()-1, 1);
    }
    m_kdTree.build(m_nodes);

    return !xml.hasError();
}
