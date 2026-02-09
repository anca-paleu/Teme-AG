#include "Graph.h"
#include "DisjointSet.h"
#include <fstream>
#include <limits>
#include <algorithm>
#include <cmath>

void Graph::loadFromFile(const std::string& filename) {
    std::ifstream fin(filename);
    int n, m;
    fin >> n;
    cities.clear();
    for (int i = 0; i < n; ++i) {
        City c;
        fin >> c.id >> c.name >> c.x >> c.y;
        cities.push_back(c);
    }

    double inf = std::numeric_limits<double>::max();
    adjMatrix.assign(n + 1, std::vector<double>(n + 1, inf));
    for(int i=1; i<=n; ++i) adjMatrix[i][i] = 0;

    fin >> m;
    initialEdges.clear();
    for (int i = 0; i < m; ++i) {
        int u, v;
        double w;
        fin >> u >> v >> w;
        initialEdges.push_back({u, v, w});
        adjMatrix[u][v] = w;
        adjMatrix[v][u] = w;
    }
    fin.close();
}

void Graph::runFloydWarshall() {
    int n = cities.size();
    for (int k = 1; k <= n; ++k) {
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (adjMatrix[i][k] != std::numeric_limits<double>::max() &&
                    adjMatrix[k][j] != std::numeric_limits<double>::max()) {
                    if (adjMatrix[i][j] > adjMatrix[i][k] + adjMatrix[k][j]) {
                        adjMatrix[i][j] = adjMatrix[i][k] + adjMatrix[k][j];
                    }
                }
            }
        }
    }
}

void Graph::runKruskal() {
    int n = cities.size();
    std::vector<Edge> allEdges;

    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            if (adjMatrix[i][j] != std::numeric_limits<double>::max()) {
                allEdges.push_back({i, j, adjMatrix[i][j]});
            }
        }
    }

    std::sort(allEdges.begin(), allEdges.end());

    DisjointSet ds(n);
    mstEdges.clear();

    for (const auto& edge : allEdges) {
        if (ds.find(edge.u) != ds.find(edge.v)) {
            ds.unite(edge.u, edge.v);
            mstEdges.push_back(edge);
        }
    }
}

void Graph::dfsPreorder(int node, const std::vector<std::vector<int>>& mstAdj, std::vector<bool>& visited) {
    visited[node] = true;
    tspCircuit.push_back(node);

    for (int neighbor : mstAdj[node]) {
        if (!visited[neighbor]) {
            dfsPreorder(neighbor, mstAdj, visited);
        }
    }
}

void Graph::generateTSP() {
    int n = cities.size();
    std::vector<std::vector<int>> mstAdj(n + 1);

    for (const auto& edge : mstEdges) {
        mstAdj[edge.u].push_back(edge.v);
        mstAdj[edge.v].push_back(edge.u);
    }

    tspCircuit.clear();
    std::vector<bool> visited(n + 1, false);

    dfsPreorder(1, mstAdj, visited);

    if (!tspCircuit.empty()) {
        tspCircuit.push_back(tspCircuit.front());
    }
}

const std::vector<City>& Graph::getCities() const { return cities; }
const std::vector<Edge>& Graph::getInitialEdges() const { return initialEdges; }
const std::vector<std::vector<double>>& Graph::getAdjacencyMatrix() const { return adjMatrix; }
const std::vector<Edge>& Graph::getMSTEdges() const { return mstEdges; }
const std::vector<int>& Graph::getTSPCircuit() const { return tspCircuit; }
