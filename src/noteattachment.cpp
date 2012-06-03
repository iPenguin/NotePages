#include "noteattachment.h"

#include <QPainter>

NoteAttachment::NoteAttachment(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsTextItem(parent, scene)
{

}

QRectF NoteAttachment::boundingRect() const
{

    return QGraphicsTextItem::boundingRect();
}

void NoteAttachment::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    //painter->drawRect(boundingRect());
    QGraphicsTextItem::paint(painter, option, widget);
}
