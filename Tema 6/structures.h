#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string>
#include <vector>

struct City {
    int id;
    std::string name;
    double x;
    double y;
};

struct Edge {
    int u;
    int v;
    double weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

enum DrawMode {
    MODE_INITIAL,
    MODE_COMPLETE,
    MODE_MST,
    MODE_TSP
};

#endif
