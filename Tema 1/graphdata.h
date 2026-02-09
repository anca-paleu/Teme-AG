#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include <QPoint>
#include <QString>
#include <QColor>

struct Node {
    int id;
    QPoint pos;
    QString label;
    QColor color;

    Node(int _id = 0, QPoint _pos = QPoint(), QString _lbl = "")
        : id(_id), pos(_pos), label(_lbl), color(Qt::white) {}
};

struct Edge {
    int source;
    int dest;
};

#endif
