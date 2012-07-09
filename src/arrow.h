#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>

class Note;

class Arrow : public QGraphicsLineItem
{
public:
    Arrow(Note *start, Note *end, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    void updatePosition();
private:

    Note *mStart,
         *mEnd;
};

#endif // ARROW_H
