#include "mazegraph.h"
#include <fstream>
#include <string>
#include <QQueue>
#include <algorithm>
#include <utility>

MazeGraph::MazeGraph()
{
    numRows = 0;
    numCols = 0;
    startNodeId = -1;
}

bool MazeGraph::loadMazeFromFile()
{
    mazeMatrix.clear();
    exitPoints.clear();
    numRows = 0;
    numCols = 0;

    std::ifstream fin("C:\\Users\\anca\\Desktop\\Tema2\\build\\Desktop_Qt_6_9_3_MinGW_64_bit-Debug\\debug\\matrice.txt");

    if (!fin.is_open()) {
        return false;
    }

    std::string line;

    while (fin >> line)
    {
        if (line.empty()) continue;

        QVector<int> row;
        if (numCols == 0) {
            numCols = line.length();
        }

        for (size_t j = 0; j < line.length(); ++j)
        {
            int val = line[j] - '0';
            row.append(val);

            if (val == 3) {
                startPoint = QPoint(j, numRows);
            } else if (val == 2) {
                exitPoints.append(QPoint(j, numRows));
            }
        }
        mazeMatrix.append(row);
        numRows++;
    }

    fin.close();
    return true;
}

void MazeGraph::buildGraph()
{
    adjacencyList.clear();
    coordToNodeId.clear();
    nodeIdToCoord.clear();
    exitNodeIds.clear();

    int currentNodeId = 0;

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            if (mazeMatrix[i][j] != 0) {
                QPoint currentPoint(j, i);
                coordToNodeId[currentPoint] = currentNodeId;
                nodeIdToCoord[currentNodeId] = currentPoint;
                if (mazeMatrix[i][j] == 3) {
                    startNodeId = currentNodeId;
                } else if (mazeMatrix[i][j] == 2) {
                    exitNodeIds.append(currentNodeId);
                }
                currentNodeId++;
            }
        }
    }

    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            if (mazeMatrix[i][j] == 0) continue;
            QPoint currentPoint(j, i);
            int A_NodeId = coordToNodeId[currentPoint];
            QPoint neighbors[4] = {
                QPoint(j, i - 1),
                QPoint(j, i + 1),
                QPoint(j - 1, i),
                QPoint(j + 1, i)
            };
            for (int k = 0; k < 4; ++k) {
                QPoint neighborPoint = neighbors[k];
                if (neighborPoint.y() >= 0 && neighborPoint.y() < numRows &&
                    neighborPoint.x() >= 0 && neighborPoint.x() < numCols)
                {
                    if (mazeMatrix[neighborPoint.y()][neighborPoint.x()] != 0) {
                        int B_NodeId = coordToNodeId[neighborPoint];
                        adjacencyList[A_NodeId].append(B_NodeId);
                    }
                }
            }
        }
    }
}

void MazeGraph::runBFS()
{
    parentMap.clear();
    QQueue<int> queue;
    QHash<int, bool> visited;

    queue.enqueue(startNodeId);
    visited[startNodeId] = true;
    parentMap[startNodeId] = -1;

    while (!queue.isEmpty())
    {
        int currentNode = queue.dequeue();

        for (const auto &neighborId : std::as_const(adjacencyList[currentNode]))
        {
            if (!visited.contains(neighborId))
            {
                visited[neighborId] = true;
                parentMap[neighborId] = currentNode;
                queue.enqueue(neighborId);
            }
        }
    }
}

void MazeGraph::reconstructPath(int exitNode)
{
    if (!parentMap.contains(exitNode)) {
        return;
    }

    QVector<int> currentPath;
    int temp = exitNode;

    while (temp != -1) {
        currentPath.append(temp);
        temp = parentMap[temp];
    }

    std::reverse(currentPath.begin(), currentPath.end());
    allPaths.append(currentPath);
}

void MazeGraph::reconstructAllPaths()
{
    allPaths.clear();

    for (int exitNode : std::as_const(exitNodeIds)) {
        reconstructPath(exitNode);
    }
}

const QVector<QVector<int>>& MazeGraph::getPaths() const
{
    return allPaths;
}
int MazeGraph::getNumRows() const
{
    return numRows;
}

int MazeGraph::getNumCols() const
{
    return numCols;
}

const QVector<QVector<int>>& MazeGraph::getMazeMatrix() const
{
    return mazeMatrix;
}

const QHash<int, QPoint>& MazeGraph::getNodeIdToCoord() const
{
    return nodeIdToCoord;
}

const QHash<int, QVector<int>>& MazeGraph::getAdjacencyList() const
{
    return adjacencyList;
}

