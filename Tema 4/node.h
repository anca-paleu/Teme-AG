#ifndef NODE_H
#define NODE_H

#include <QPoint>
#include <QColor>
#include <QString>

class Node
{
    QPoint m_coord;
    int m_index;
    QString m_text;
    QColor m_color;
    bool m_isOval;

public:
    Node(int index, const QPoint& coord, QString text = "");

    void setCoord(const QPoint& coord);
    QPoint getCoord() const;

    int getIndex() const;

    QString getText() const;
    void setText(const QString& text);

    QColor getColor() const;
    void setColor(const QColor& color);

    bool isOval() const;
    void setIsOval(bool oval);

    int getX() const;
    int getY() const;
    bool isUnderMouse(const QPoint& mousePos) const;
};

#endif // NODE_H
