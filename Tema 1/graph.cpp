#include "graph.h"
#include <queue>
#include <stack>
#include <algorithm>
#include <map>
#include <QRandomGenerator>

Graph::Graph() : m_isOriented(false) {}

Graph::~Graph() { clear(); }

void Graph::clear() {
    for(auto n : m_nodes) delete n;
    m_nodes.clear();
    m_adjMatrix.clear();
}

void Graph::setOriented(bool oriented) { m_isOriented = oriented; }
bool Graph::isOriented() const { return m_isOriented; }

void Graph::addNode(const QPoint& p, QString label) {
    int idx = m_nodes.size() + 1;
    Node* n = new Node(idx, p, label);
    m_nodes.push_back(n);

    // Redimensionare matrice
    int N = m_nodes.size();
    m_adjMatrix.resize(N);
    for(auto& row : m_adjMatrix) row.resize(N, 0);

    // Noile coloane sunt 0
    for(int i=0; i<N; ++i) m_adjMatrix[i].resize(N, 0);
}

void Graph::addEdge(Node* f, Node* s) {
    if (!f || !s) return;
    int i = f->getIndex() - 1;
    int j = s->getIndex() - 1;

    m_adjMatrix[i][j] = 1;
    if (!m_isOriented) {
        m_adjMatrix[j][i] = 1;
    }
}

Node* Graph::getNodeAt(const QPoint& p) {
    for (auto n : m_nodes) {
        if (n->isUnderMouse(p)) return n;
    }
    return nullptr;
}

const std::vector<Node*>& Graph::getNodes() const { return m_nodes; }
const std::vector<std::vector<int>>& Graph::getAdjacencyMatrix() const { return m_adjMatrix; }

// --- ALGORITMI ---

void Graph::solveConnectedComponents() {
    if (m_isOriented) return; // Doar pt neorientat

    // Reset culori
    for(auto n : m_nodes) n->setColor(Qt::white);

    int N = m_nodes.size();
    std::vector<bool> visited(N, false);

    for (int i = 0; i < N; ++i) {
        if (!visited[i]) {
            // Culoare random pentru componenta curenta
            QColor c = QColor::fromRgb(QRandomGenerator::global()->generate());

            // BFS
            std::queue<int> q;
            q.push(i);
            visited[i] = true;
            m_nodes[i]->setColor(c);

            while (!q.empty()) {
                int u = q.front();
                q.pop();

                for (int v = 0; v < N; ++v) {
                    if (m_adjMatrix[u][v] && !visited[v]) {
                        visited[v] = true;
                        m_nodes[v]->setColor(c);
                        q.push(v);
                    }
                }
            }
        }
    }
}

Graph* Graph::getCondensedGraph() {
    if (!m_isOriented) return nullptr;

    int N = m_nodes.size();
    std::vector<int> disc(N, -1), low(N, -1);
    std::vector<bool> onStack(N, false);
    std::stack<int> st;
    int timer = 0;

    std::vector<std::vector<int>> SCCs; // Vector de vectori cu indicii nodurilor

    // Lambda pentru DFS (Tarjan)
    std::function<void(int)> dfs = [&](int u) {
        disc[u] = low[u] = ++timer;
        st.push(u);
        onStack[u] = true;

        for (int v = 0; v < N; ++v) {
            if (m_adjMatrix[u][v]) { // Daca exista muchie
                if (disc[v] == -1) {
                    dfs(v);
                    low[u] = std::min(low[u], low[v]);
                } else if (onStack[v]) {
                    low[u] = std::min(low[u], disc[v]);
                }
            }
        }

        if (low[u] == disc[u]) {
            std::vector<int> currentComponent;
            while (true) {
                int v = st.top();
                st.pop();
                onStack[v] = false;
                currentComponent.push_back(v);
                if (u == v) break;
            }
            SCCs.push_back(currentComponent);
        }
    };

    for (int i = 0; i < N; ++i) {
        if (disc[i] == -1) dfs(i);
    }

    // --- CONSTRUIRE GRAF NOU ---
    Graph* condensed = new Graph();
    condensed->setOriented(true);

    std::vector<Node*> newNodes;
    std::vector<int> oldNodeToNewIndex(N);

    // 1. Creare noduri condensate
    for (const auto& comp : SCCs) {
        long long sumX = 0, sumY = 0;
        QString label;
        // Calculam centrul si textul (ex: "1, 2, 5")
        // Sortam indicii ca sa arate frumos
        std::vector<int> sortedComp = comp;
        std::sort(sortedComp.begin(), sortedComp.end());

        for (size_t k = 0; k < sortedComp.size(); ++k) {
            int originalIdx = sortedComp[k];
            sumX += m_nodes[originalIdx]->getX();
            sumY += m_nodes[originalIdx]->getY();

            label += QString::number(m_nodes[originalIdx]->getIndex());
            if (k < sortedComp.size() - 1) label += ", ";

            // Mapam indexul vechi la indexul noului nod (care va fi size-ul curent al condensed)
            oldNodeToNewIndex[originalIdx] = condensed->getNodes().size();
        }

        QPoint center(sumX / comp.size(), sumY / comp.size());
        condensed->addNode(center, label);
    }

    // 2. Adaugare muchii intre componente
    const auto& newNodesList = condensed->getNodes();

    for (int u = 0; u < N; ++u) {
        for (int v = 0; v < N; ++v) {
            if (m_adjMatrix[u][v]) {
                int sccU = oldNodeToNewIndex[u]; // Index in graful nou
                int sccV = oldNodeToNewIndex[v]; // Index in graful nou

                if (sccU != sccV) {
                    // Verificam sa nu adaugam aceeasi muchie de mai multe ori
                    // (Implementarea curenta addEdge permite duplicate in matrice daca setam 1 repetat, dar e ok pt 0/1)
                    condensed->addEdge(newNodesList[sccU], newNodesList[sccV]);
                }
            }
        }
    }

    return condensed;
}
