#ifndef NOTEATTACHMENT_H
#define NOTEATTACHMENT_H

#include <QGraphicsTextItem>

class NoteAttachment : public QGraphicsTextItem
{
public:
    NoteAttachment(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);


};

#endif // NOTEATTACHMENT_H
