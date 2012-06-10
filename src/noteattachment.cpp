/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include "noteattachment.h"

#include <QPainter>
#include <QDebug>

#include <QDesktopServices>
#include <QCursor>
#include <QUrl>

NoteAttachment::NoteAttachment(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsTextItem(parent, scene)
{
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setCursor(QCursor(Qt::PointingHandCursor));
}

QRectF NoteAttachment::boundingRect() const
{

    return QGraphicsTextItem::boundingRect();
}

void NoteAttachment::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QGraphicsTextItem::paint(painter, option, widget);
}

void NoteAttachment::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{

    QDesktopServices::openUrl(QUrl("file://"+mPath +"/" +mFile));
    QGraphicsTextItem::mouseReleaseEvent(e);
}
