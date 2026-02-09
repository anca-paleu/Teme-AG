#include "KDTree.h"
#include <algorithm>
#include <limits>

KDTree::KDTree(const MapGraph& graph) : refGraph(graph), root(nullptr) {
    std::vector<int> indices(graph.nodes.size());
    for (size_t i = 0; i < graph.nodes.size(); ++i) indices[i] = i;
    root = build(indices, 0);
}

KDTree::~KDTree() {
    freeTree(root);
}

int KDTree::findNearest(double x, double y) {
    if (!root) return -1;
    int bestIdx = -1;
    double bestDist = std::numeric_limits<double>::max();
    search(root, x, y, 0, bestIdx, bestDist);
    return bestIdx;
}

KDNode* KDTree::build(std::vector<int>& indices, int depth) {
    if (indices.empty()) return nullptr;

    int axis = depth % 2;
    size_t mid = indices.size() / 2;

    std::nth_element(indices.begin(), indices.begin() + mid, indices.end(),
        [&](int a, int b) {
            return (axis == 0) ? (refGraph.nodes[a].longitude < refGraph.nodes[b].longitude)
                               : (refGraph.nodes[a].latitude < refGraph.nodes[b].latitude);
        });

    KDNode* node = new KDNode(indices[mid]);

    std::vector<int> leftIndices(indices.begin(), indices.begin() + mid);
    std::vector<int> rightIndices(indices.begin() + mid + 1, indices.end());

    node->left = build(leftIndices, depth + 1);
    node->right = build(rightIndices, depth + 1);

    return node;
}

void KDTree::search(KDNode* node, double x, double y, int depth, int& bestIdx, double& bestDist) {
    if (!node) return;

    double d = distSq(refGraph.nodes[node->nodeIndex].longitude, refGraph.nodes[node->nodeIndex].latitude, x, y);
    if (d < bestDist) {
        bestDist = d;
        bestIdx = node->nodeIndex;
    }

    int axis = depth % 2;
    double diff = (axis == 0) ? (x - refGraph.nodes[node->nodeIndex].longitude)
                              : (y - refGraph.nodes[node->nodeIndex].latitude);

    KDNode* near = (diff < 0) ? node->left : node->right;
    KDNode* far = (diff < 0) ? node->right : node->left;

    search(near, x, y, depth + 1, bestIdx, bestDist);

    if (diff * diff < bestDist) {
        search(far, x, y, depth + 1, bestIdx, bestDist);
    }
}

void KDTree::freeTree(KDNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

double KDTree::distSq(double x1, double y1, double x2, double y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}
