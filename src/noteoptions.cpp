/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include "noteoptions.h"
#include <QPainter>

#include <QCursor>

NoteOptions::NoteOptions(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsRectItem(parent, scene)
{
    setRect(0,0,16,13);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setZValue(1000);

    setCursor(QCursor(Qt::ArrowCursor));
}

void NoteOptions::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPolygonF poly = QPolygonF();
    poly << QPoint(3,3) << QPoint(13,3) << QPoint(8,10);
    painter->drawPolygon(poly);
    QPainterPath path;
    path.addPolygon(poly);
    painter->fillPath(path, QBrush(Qt::black));
    QGraphicsRectItem::paint(painter, option, widget);
}
