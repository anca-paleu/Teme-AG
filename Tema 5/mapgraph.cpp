#include "MapGraph.h"
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>

MapGraph::MapGraph() {
    minLat = std::numeric_limits<double>::max();
    maxLat = std::numeric_limits<double>::lowest();
    minLon = std::numeric_limits<double>::max();
    maxLon = std::numeric_limits<double>::lowest();
}

void MapGraph::loadFromXML(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    size_t pos = 0;
    while ((pos = content.find("<node", pos)) != std::string::npos) {
        size_t end = content.find("/>", pos);
        std::string line = content.substr(pos, end - pos);

        int id = std::stoi(getAttribute(line, "id"));
        double lat = std::stod(getAttribute(line, "latitude"));
        double lon = std::stod(getAttribute(line, "longitude"));

        Node n = {id, lon, lat};
        if (nodes.size() <= (size_t)id) nodes.resize(id + 1);
        nodes[id] = n;

        if (lat < minLat) minLat = lat;
        if (lat > maxLat) maxLat = lat;
        if (lon < minLon) minLon = lon;
        if (lon > maxLon) maxLon = lon;

        pos = end;
    }

    adj.resize(nodes.size());

    pos = 0;
    while ((pos = content.find("<arc", pos)) != std::string::npos) {
        size_t end = content.find("/>", pos);
        std::string line = content.substr(pos, end - pos);

        int from = std::stoi(getAttribute(line, "from"));
        int to = std::stoi(getAttribute(line, "to"));
        int len = std::stoi(getAttribute(line, "length"));

        if ((size_t)from < adj.size() && (size_t)to < adj.size()) {
            adj[from].push_back({to, len});
        }

        pos = end;
    }
}

std::string MapGraph::getAttribute(const std::string& line, const std::string& attr) {
    size_t start = line.find(attr + "=\"");
    if (start == std::string::npos) return "0";
    start += attr.length() + 2;
    size_t end = line.find("\"", start);
    return line.substr(start, end - start);
}
