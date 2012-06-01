#include "notehandler.h"

#include <QDebug>

NoteHandler::NoteHandler(QGraphicsItem *parent) :
    QGraphicsRectItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

void NoteHandler::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QGraphicsRectItem::paint(painter, option, widget);
}


void NoteHandler::mousePressEvent(QGraphicsSceneMouseEvent *e)
{

    qDebug() << "nh: mouse press event";
    QGraphicsRectItem::mousePressEvent(e);
}
