#ifndef NOTEATTACHMENT_H
#define NOTEATTACHMENT_H

#include <QGraphicsTextItem>

class NoteAttachment : public QGraphicsTextItem
{
public:
    NoteAttachment(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    enum { Type = UserType + 10 };
    int type () const { return NoteAttachment::Type; }

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
};

#endif // NOTEATTACHMENT_H
