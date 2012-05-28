#include "notecontainer.h"

#include <QPainter>

#include <QDebug>

NoteContainer::NoteContainer(QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsItemGroup(parent, scene)
{
    //setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    //setTextInteractionFlags(Qt::LinksAccessibleByMouse);
}

QRectF NoteContainer::boundingRect() const
{

    return QGraphicsItemGroup::childrenBoundingRect().adjusted(-5,-30,5,20);
}

void NoteContainer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    painter->setPen(Qt::gray);
    painter->setBrush(QBrush(QColor(225,225,225, 128)));
    painter->drawRoundedRect(boundingRect().toRect(), 0.5, 0.5);
    QGraphicsItemGroup::paint(painter, option, widget);
}

void NoteContainer::addItem(QGraphicsItem *item)
{
    QRectF rect;

    addToGroup(item);

    qDebug() << "boundingRect" << boundingRect() << item->boundingRect();

    if(childItems().count() <= 1) {
        rect.setX(item->pos().x());
        rect.setY(item->pos().y());
    }

    foreach(QGraphicsItem *i, childItems()) {
        qDebug() << i->boundingRect();

    }

}
