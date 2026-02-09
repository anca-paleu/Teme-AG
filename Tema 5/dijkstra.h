#pragma once
#include "MapGraph.h"
#include <vector>

class Dijkstra {
public:
    static std::vector<int> findPath(const MapGraph& graph, int startId, int endId);
};
