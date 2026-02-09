#pragma once
#include "MapGraph.h"
#include <vector>

struct KDNode {
    int nodeIndex;
    KDNode* left = nullptr;
    KDNode* right = nullptr;
    KDNode(int idx) : nodeIndex(idx) {}
};

class KDTree {
public:
    KDTree(const MapGraph& graph);
    ~KDTree();
    int findNearest(double x, double y);

private:
    const MapGraph& refGraph;
    KDNode* root;

    KDNode* build(std::vector<int>& indices, int depth);
    void search(KDNode* node, double x, double y, int depth, int& bestIdx, double& bestDist);
    void freeTree(KDNode* node);
    double distSq(double x1, double y1, double x2, double y2);
};
