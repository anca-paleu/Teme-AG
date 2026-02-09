#include "graph.h"
#include <queue>
#include <stack>
#include <algorithm>
#include <QColor>
#include <functional>

Graph::Graph() : m_isOriented(false) {}

Graph::~Graph() { clear(); }

void Graph::clear() {
    for (auto n : m_nodes) delete n;
    m_nodes.clear();
    m_adjMatrix.clear();
}

void Graph::setOriented(bool oriented) { m_isOriented = oriented; }
bool Graph::isOriented() const { return m_isOriented; }

void Graph::addNode(const QPoint& p, QString label) {
    int idx = m_nodes.size() + 1;
    Node* n = new Node(idx, p, label);
    m_nodes.push_back(n);

    // corect: extindem matricea de adiacenta cu un rand si o coloana
    int N = m_nodes.size();
    // adaugam noul rand initializat cu 0
    m_adjMatrix.push_back(std::vector<int>(N, 0));
    // extindem fiecare rand existent (inclusiv noul rand, dar pentru cele anterioare)
    for (int i = 0; i < N - 1; ++i) {
        m_adjMatrix[i].push_back(0);
    }
    // daca e nevoie (noul rand are deja marimea N)
}

void Graph::addEdge(Node* f, Node* s) {
    if (!f || !s) return;
    int i = f->getIndex() - 1;
    int j = s->getIndex() - 1;
    if (i < 0 || j < 0 || i >= (int)m_adjMatrix.size() || j >= (int)m_adjMatrix.size()) return;

    // evitam duplicate
    if (m_adjMatrix[i][j] == 0) {
        m_adjMatrix[i][j] = 1;
    }

    if (!m_isOriented) {
        if (m_adjMatrix[j][i] == 0) m_adjMatrix[j][i] = 1;
    }
}

Node* Graph::getNodeAt(const QPoint& p) {
    for (auto n : m_nodes) if (n->isUnderMouse(p)) return n;
    return nullptr;
}

std::vector<Node*>& Graph::getNodes() { return m_nodes; }
const std::vector<Node*>& Graph::getNodes() const { return m_nodes; }

std::vector<std::vector<int>>& Graph::getAdjacencyMatrix() { return m_adjMatrix; }
const std::vector<std::vector<int>>& Graph::getAdjacencyMatrix() const { return m_adjMatrix; }

// --- ALGORITMI ---

void Graph::solveConnectedComponents() {
    if (m_isOriented) return; // doar pentru neorientat

    // Reset culori
    for (auto n : m_nodes) n->setColor(Qt::white);

    int N = m_nodes.size();
    if (N == 0) return;
    std::vector<bool> visited(N, false);

    // CULORILE FIXE PENTRU TEMA 4.1 (prima = rosu, a doua = albastru)
    std::vector<QColor> colors = { Qt::red, Qt::blue, Qt::green, Qt::yellow, Qt::magenta, Qt::cyan };
    int colorIdx = 0;

    for (int i = 0; i < N; ++i) {
        if (!visited[i]) {
            QColor currentColor = colors[colorIdx % colors.size()];
            colorIdx++;

            // BFS
            std::queue<int> q;
            q.push(i);
            visited[i] = true;
            m_nodes[i]->setColor(currentColor);

            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v = 0; v < N; ++v) {
                    if (m_adjMatrix[u][v] && !visited[v]) {
                        visited[v] = true;
                        m_nodes[v]->setColor(currentColor);
                        q.push(v);
                    }
                }
            }
        }
    }
}

Graph* Graph::solveStronglyConnectedComponents() {
    if (!m_isOriented) return nullptr; // doar pentru orientat

    int N = m_nodes.size();
    if (N == 0) return new Graph();

    std::vector<int> disc(N, -1), low(N, -1);
    std::vector<bool> onStack(N, false);
    std::stack<int> st;
    int timer = 0;
    std::vector<std::vector<int>> SCCs;

    std::function<void(int)> dfs = [&](int u) {
        disc[u] = low[u] = ++timer;
        st.push(u);
        onStack[u] = true;

        for (int v = 0; v < N; ++v) {
            if (m_adjMatrix[u][v]) {
                if (disc[v] == -1) {
                    dfs(v);
                    low[u] = std::min(low[u], low[v]);
                } else if (onStack[v]) {
                    low[u] = std::min(low[u], disc[v]);
                }
            }
        }

        if (low[u] == disc[u]) {
            std::vector<int> currentSCC;
            while (true) {
                int v = st.top(); st.pop();
                onStack[v] = false;
                currentSCC.push_back(v);
                if (u == v) break;
            }
            std::sort(currentSCC.begin(), currentSCC.end());
            SCCs.push_back(currentSCC);
        }
    };

    for (int i = 0; i < N; ++i) if (disc[i] == -1) dfs(i);

    // Construire Graf Condensat
    Graph* condensed = new Graph();
    condensed->setOriented(true);

    // mapare vechi->nou
    std::vector<int> oldToNew(N, -1);

    // 1. Noduri: pentru fiecare componenta punem un nod la centrul mediu si eticheta "a, b, c"
    for (size_t compIdx = 0; compIdx < SCCs.size(); ++compIdx) {
        auto& comp = SCCs[compIdx];

        long long sx = 0, sy = 0;
        QString label;
        for (size_t k = 0; k < comp.size(); ++k) {
            int orig = comp[k];
            sx += m_nodes[orig]->getX();
            sy += m_nodes[orig]->getY();
            oldToNew[orig] = compIdx;

            label += QString::number(m_nodes[orig]->getIndex());
            if (k + 1 < comp.size()) label += ", ";
        }
        QPoint center(sx / (int)comp.size(), sy / (int)comp.size());
        condensed->addNode(center, label);
        // daca componenta contine >1 nod o facem oval
        if (comp.size() > 1) {
            condensed->getNodes().back()->setIsOval(true);
        }
    }

    // 2. Muchii intre componente (fara duplicate)
    int M = SCCs.size();
    for (int u = 0; u < N; ++u) {
        for (int v = 0; v < N; ++v) {
            if (m_adjMatrix[u][v]) {
                int a = oldToNew[u];
                int b = oldToNew[v];
                if (a != b && a >= 0 && b >= 0) {
                    // adaugam muchia daca nu exista deja
                    Node* nu = condensed->getNodes()[a];
                    Node* nv = condensed->getNodes()[b];
                    // verificam in matrice daca exista deja
                    if (condensed->getAdjacencyMatrix().size() > 0) {
                        if (condensed->getAdjacencyMatrix()[a][b] == 0) {
                            condensed->addEdge(nu, nv);
                        }
                    } else {
                        condensed->addEdge(nu, nv);
                    }
                }
            }
        }
    }

    return condensed;
}
