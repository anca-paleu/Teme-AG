#pragma once
#include <vector>
#include <string>

struct Node {
    int id;
    double longitude;
    double latitude;
};

struct Edge {
    int to;
    int length;
};

class MapGraph {
public:
    std::vector<Node> nodes;
    std::vector<std::vector<Edge>> adj;
    double minLat, maxLat, minLon, maxLon;

    MapGraph();
    void loadFromXML(const std::string& filename);

private:
    std::string getAttribute(const std::string& line, const std::string& attr);
};
