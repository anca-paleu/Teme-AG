#include "graphmanager.h"

GraphManager::GraphManager(int n) : n(n) {
    adj.resize(n);
}

void GraphManager::addEdge(int u, int v, int cap) {
    adj[u].push_back({v, cap, 0, (int)adj[v].size()});
    adj[v].push_back({u, 0, 0, (int)adj[u].size() - 1});
}

bool GraphManager::findPath(int s, int t, std::vector<int>& parent, std::vector<int>& edgeIdx) {
    std::fill(parent.begin(), parent.end(), -1);
    std::queue<int> q;
    q.push(s);
    parent[s] = s;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int i = 0; i < (int)adj[u].size(); i++) {
            Edge &e = adj[u][i];
            if (parent[e.to] == -1 && e.capacity - e.flow > 0) {
                parent[e.to] = u;
                edgeIdx[e.to] = i;
                q.push(e.to);
                if (e.to == t) return true;
            }
        }
    }
    return false;
}

std::vector<std::pair<int, int>> GraphManager::computeMinCut(int s) {
    std::vector<bool> visited(n, false);
    std::queue<int> q;
    q.push(s);
    visited[s] = true;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto &e : adj[u]) {
            if (e.capacity - e.flow > 0 && !visited[e.to]) {
                visited[e.to] = true;
                q.push(e.to);
            }
        }
    }

    std::vector<std::pair<int, int>> cut;
    for (int u = 0; u < n; u++) {
        if (visited[u]) {
            for (auto &e : adj[u]) {
                if (!visited[e.to] && e.capacity > 0) {
                    cut.push_back({u, e.to});
                }
            }
        }
    }
    return cut;
}
