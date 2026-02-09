#ifndef MAZEGRAPH_H
#define MAZEGRAPH_H

#include <QVector>
#include <QPoint>
#include <QHash>
#include <QQueue>

class MazeGraph
{
public:
    MazeGraph();

    bool loadMazeFromFile();
    void buildGraph();

    void runBFS();
    void reconstructAllPaths();
    const QVector<QVector<int>>& getPaths() const;

    int getNumRows() const;
    int getNumCols() const;
    const QVector<QVector<int>>& getMazeMatrix() const;
    const QHash<int, QPoint>& getNodeIdToCoord() const;
    const QHash<int, QVector<int>>& getAdjacencyList() const;

private:
    void reconstructPath(int exitNode);

    QHash<int, int> parentMap;
    QVector<QVector<int>> allPaths;

    QVector<QVector<int>> mazeMatrix;
    int numRows;
    int numCols;

    QPoint startPoint;
    QVector<QPoint> exitPoints;

    QHash<int, QVector<int>> adjacencyList;
    QHash<QPoint, int> coordToNodeId;
    QHash<int, QPoint> nodeIdToCoord;

    int startNodeId;
    QVector<int> exitNodeIds;
};

#endif // MAZEGRAPH_H
