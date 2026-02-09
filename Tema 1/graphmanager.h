#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include <vector>
#include <stack>
#include <set>
#include <algorithm>
#include <cmath>
#include "GraphData.h"

class GraphManager
{
public:
    GraphManager();

    void addNode(QPoint pos);
    void addEdge(int sourceIndex, int destIndex);
    void clear();

    const std::vector<Node>& getNodes() const;
    const std::vector<Edge>& getEdges() const;
    int getNodeIndexAt(QPoint p, int radius) const;

    void solveConnectedComponents();
    GraphManager generateCondensedGraph(int canvasWidth, int canvasHeight);

private:
    std::vector<Node> nodes;
    std::vector<Edge> edges;

    void tarjanDFS(int u, int& timer, std::vector<int>& disc, std::vector<int>& low,
                   std::stack<int>& st, std::vector<bool>& stackMember,
                   std::vector<std::vector<int>>& sccs, const std::vector<std::vector<int>>& adj);
};

#endif
