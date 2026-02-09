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

    // Operatii de baza
    void addNode(QPoint pos);
    void addEdge(int sourceIndex, int destIndex);
    void clear();

    // Getters pentru desenare
    const std::vector<Node>& getNodes() const;
    const std::vector<Edge>& getEdges() const;
    int getNodeIndexAt(QPoint p, int radius) const;

    // ALGORITMI
    // Tema 4 pct 1: Coloreaza componentele conexe (neorientat)
    void solveConnectedComponents();

    // Tema 4 pct 2: Returneaza un NOU GraphManager ce contine graful condensat
    GraphManager generateCondensedGraph(int canvasWidth, int canvasHeight);

private:
    std::vector<Node> nodes;
    std::vector<Edge> edges;

    // Helper pentru Tarjan
    void tarjanDFS(int u, int& timer, std::vector<int>& disc, std::vector<int>& low,
                   std::stack<int>& st, std::vector<bool>& stackMember,
                   std::vector<std::vector<int>>& sccs, const std::vector<std::vector<int>>& adj);
};

#endif // GRAPHMANAGER_H
