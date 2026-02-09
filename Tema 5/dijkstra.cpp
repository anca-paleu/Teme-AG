#include "Dijkstra.h"
#include <queue>
#include <algorithm>
#include <limits>

std::vector<int> Dijkstra::findPath(const MapGraph& graph, int startId, int endId) {
    const int INF = std::numeric_limits<int>::max();
    size_t n = graph.nodes.size();
    std::vector<int> dist(n, INF);
    std::vector<int> parent(n, -1);

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

    dist[startId] = 0;
    pq.push({0, startId});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;
        if (u == endId) break;

        for (const auto& edge : graph.adj[u]) {
            if (dist[u] + edge.length < dist[edge.to]) {
                dist[edge.to] = dist[u] + edge.length;
                parent[edge.to] = u;
                pq.push({dist[edge.to], edge.to});
            }
        }
    }

    std::vector<int> path;
    if (dist[endId] == INF) return path;

    for (int curr = endId; curr != -1; curr = parent[curr]) {
        path.push_back(curr);
    }
    std::reverse(path.begin(), path.end());
    return path;
}
