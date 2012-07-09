#include "arrow.h"

#include "note.h"
#include <QPen>

Arrow::Arrow(Note *start, Note *end, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene),
      mStart(start),
      mEnd(end)
{
    mStart->addArrow(this);
    mEnd->addArrow(this);

    setFlag(QGraphicsItem::ItemIsSelectable);
    setPen(QPen(QBrush(Qt::black), 3));
    setLine(QLineF(mStart->pos(), mEnd->pos()));

}

QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QGraphicsLineItem::paint(painter, option, widget);
}

void Arrow::updatePosition()
{
    QLineF line(mapFromItem(mStart, 0, 0), mapFromItem(mEnd, 0, 0));
    setLine(line);
}
