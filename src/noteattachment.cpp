#include "noteattachment.h"

#include <QPainter>
#include <QDebug>

NoteAttachment::NoteAttachment(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsTextItem(parent, scene)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
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

void NoteAttachment::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{

    qDebug() << "Attachment Mouse Release Event";
    QGraphicsTextItem::mouseReleaseEvent(e);
}
