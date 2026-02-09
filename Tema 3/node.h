#ifndef NODE_H
#define NODE_H
#include <qpoint.h>

class Node
{
    QPoint m_coord;
    int m_index;

public:
    Node(int index = 0, QPoint coord = QPoint(0,0));
    void setIndex(int index);
    void setCoord(QPoint coord);
    int getIndex() const;
    QPoint getCoord() const;
    int getX() const;
    int getY() const;
};

#endif // NODE_H
