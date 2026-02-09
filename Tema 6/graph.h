#ifndef GRAPH_H
#define GRAPH_H

#include "structures.h"
#include <vector>
#include <string>

class Graph {
public:
    void loadFromFile(const std::string& filename);
    void runFloydWarshall();
    void runKruskal();
    void generateTSP();

    const std::vector<City>& getCities() const;
    const std::vector<Edge>& getInitialEdges() const;
    const std::vector<std::vector<double>>& getAdjacencyMatrix() const;
    const std::vector<Edge>& getMSTEdges() const;
    const std::vector<int>& getTSPCircuit() const;

private:
    std::vector<City> cities;
    std::vector<Edge> initialEdges;
    std::vector<std::vector<double>> adjMatrix;
    std::vector<Edge> mstEdges;
    std::vector<int> tspCircuit;

    void dfsPreorder(int node, const std::vector<std::vector<int>>& mstAdj, std::vector<bool>& visited);
};

#endif
