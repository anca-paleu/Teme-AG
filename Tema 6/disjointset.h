#ifndef DISJOINTSET_H
#define DISJOINTSET_H

#include <vector>

class DisjointSet {
public:
    DisjointSet(int n);
    int find(int node);
    void unite(int u, int v);

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

#endif
