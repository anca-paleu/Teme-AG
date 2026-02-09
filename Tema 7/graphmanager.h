#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include <vector>
#include <queue>
#include <algorithm>
#include "edge.h"

class GraphManager {
public:
    int n;
    std::vector<std::vector<Edge>> adj;

    GraphManager(int n);
    void addEdge(int u, int v, int cap);
    bool findPath(int s, int t, std::vector<int>& parent, std::vector<int>& edgeIdx);
    std::vector<std::pair<int, int>> computeMinCut(int s);
};

#endif
