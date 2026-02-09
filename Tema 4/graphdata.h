#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include <QPoint>
#include <QString>
#include <QColor>

struct Node {
    int id;           // ID numeric unic
    QPoint pos;       // Pozitia pe ecran
    QString label;    // Textul afisat (ex: "1" sau "1,2,5")
    QColor color;     // Culoarea nodului (pentru componente conexe)

    // Constructor default
    Node(int _id = 0, QPoint _pos = QPoint(), QString _lbl = "")
        : id(_id), pos(_pos), label(_lbl), color(Qt::white) {}
};

struct Edge {
    int source;
    int dest;
};

#endif // GRAPHDATA_H
