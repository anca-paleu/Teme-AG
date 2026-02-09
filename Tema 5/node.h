#ifndef NODE_H
#define NODE_H

#include <QPoint>

class Node
{
public:
    Node() = default;
    Node(int index, QPoint coord);

    void setIndex(int index);
    void setCoord(QPoint coord);

    int getIndex() const;
    QPoint getCoord() const;
    int getX() const;
    int getY() const;

    bool operator==(const Node& other) const;
    bool operator!=(const Node& other) const;

private:
    QPoint m_coord;
    int m_index = 0;
};

#endif
