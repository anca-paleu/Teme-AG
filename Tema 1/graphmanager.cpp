#include "GraphManager.h"

GraphManager::GraphManager() {}

void GraphManager::addNode(QPoint pos) {
    int id = nodes.size() + 1;
    nodes.emplace_back(id, pos, QString::number(id));
}

void GraphManager::addEdge(int sourceIndex, int destIndex) {
    if(sourceIndex == destIndex) return;
    edges.push_back({sourceIndex, destIndex});
}

void GraphManager::clear() {
    nodes.clear();
    edges.clear();
}

const std::vector<Node>& GraphManager::getNodes() const { return nodes; }
const std::vector<Edge>& GraphManager::getEdges() const { return edges; }

int GraphManager::getNodeIndexAt(QPoint p, int radius) const {
    for (size_t i = 0; i < nodes.size(); ++i) {
        QPoint center = nodes[i].pos;
        if (std::pow(p.x() - center.x(), 2) + std::pow(p.y() - center.y(), 2) <= radius * radius) {
            return i;
        }
    }
    return -1;
}

void GraphManager::solveConnectedComponents() {
    int n = nodes.size();
    if (n == 0) return;

    for(auto& node : nodes) node.color = Qt::white;

    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        adj[e.source].push_back(e.dest);
        adj[e.dest].push_back(e.source);
    }

    std::vector<bool> visited(n, false);
    std::vector<QColor> palette = {Qt::red, Qt::green, Qt::cyan, Qt::magenta, Qt::yellow, Qt::gray};
    int colorIndex = 0;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            QColor currentColor = palette[colorIndex % palette.size()];
            colorIndex++;

            std::vector<int> q;
            q.push_back(i);
            visited[i] = true;
            nodes[i].color = currentColor;

            size_t head = 0;
            while(head < q.size()){
                int u = q[head++];
                for(int v : adj[u]){
                    if(!visited[v]){
                        visited[v] = true;
                        nodes[v].color = currentColor;
                        q.push_back(v);
                    }
                }
            }
        }
    }
}

GraphManager GraphManager::generateCondensedGraph(int w, int h) {
    int n = nodes.size();
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        adj[e.source].push_back(e.dest);
    }

    int timer = 0;
    std::vector<int> disc(n, -1), low(n, -1);
    std::vector<bool> stackMember(n, false);
    std::stack<int> st;
    std::vector<std::vector<int>> sccs;

    for (int i = 0; i < n; i++) {
        if (disc[i] == -1)
            tarjanDFS(i, timer, disc, low, st, stackMember, sccs, adj);
    }

    GraphManager resultGraph;
    std::vector<int> nodeToSCCIndex(n);

    for (size_t i = 0; i < sccs.size(); ++i) {
        QString labelStr;
        std::sort(sccs[i].begin(), sccs[i].end(), [&](int a, int b){ return nodes[a].id < nodes[b].id; });

        for(size_t j = 0; j < sccs[i].size(); ++j) {
            int originalNodeIdx = sccs[i][j];
            nodeToSCCIndex[originalNodeIdx] = i;
            labelStr += QString::number(nodes[originalNodeIdx].id);
            if(j < sccs[i].size() - 1) labelStr += ", ";
        }

        double angle = (2 * M_PI * i) / sccs.size();
        int radius = 200;
        QPoint pos(w/2 + radius * cos(angle), h/2 + radius * sin(angle));

        resultGraph.nodes.emplace_back((int)i, pos, labelStr);
    }

    std::set<std::pair<int, int>> existingEdges;
    for (const auto& e : edges) {
        int u = nodeToSCCIndex[e.source];
        int v = nodeToSCCIndex[e.dest];

        if (u != v) {
            if (existingEdges.find({u, v}) == existingEdges.end()) {
                resultGraph.addEdge(u, v);
                existingEdges.insert({u, v});
            }
        }
    }

    return resultGraph;
}

void GraphManager::tarjanDFS(int u, int& timer, std::vector<int>& disc, std::vector<int>& low,
                             std::stack<int>& st, std::vector<bool>& stackMember,
                             std::vector<std::vector<int>>& sccs, const std::vector<std::vector<int>>& adj) {
    disc[u] = low[u] = ++timer;
    st.push(u);
    stackMember[u] = true;

    for (int v : adj[u]) {
        if (disc[v] == -1) {
            tarjanDFS(v, timer, disc, low, st, stackMember, sccs, adj);
            low[u] = std::min(low[u], low[v]);
        } else if (stackMember[v]) {
            low[u] = std::min(low[u], disc[v]);
        }
    }

    if (low[u] == disc[u]) {
        std::vector<int> component;
        while (true) {
            int v = st.top();
            st.pop();
            stackMember[v] = false;
            component.push_back(v);
            if (u == v) break;
        }
        sccs.push_back(component);
    }
}
