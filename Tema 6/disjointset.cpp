#include "DisjointSet.h"

DisjointSet::DisjointSet(int n) {
    parent.resize(n + 1);
    rank.resize(n + 1, 0);
    for (int i = 0; i <= n; ++i) {
        parent[i] = i;
    }
}

int DisjointSet::find(int node) {
    if (parent[node] != node) {
        parent[node] = find(parent[node]);
    }
    return parent[node];
}

void DisjointSet::unite(int u, int v) {
    int rootU = find(u);
    int rootV = find(v);

    if (rootU != rootV) {
        if (rank[rootU] < rank[rootV]) {
            parent[rootU] = rootV;
        } else if (rank[rootU] > rank[rootV]) {
            parent[rootV] = rootU;
        } else {
            parent[rootV] = rootU;
            rank[rootU]++;
        }
    }
}
