#include "note.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QStyleOptionGraphicsItem>

#include <QDebug>

Note::Note(QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsTextItem(parent, scene)
{
    mAdded = QDateTime::currentDateTime();

    //setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setTextInteractionFlags(Qt::LinksAccessibleByMouse);

}

QRectF Note::boundingRect() const
{
    //return QRectF(QPointF(0,0),mSize);
    QRectF rect = QGraphicsTextItem::boundingRect();
    rect.setHeight(rect.height() + 64);
    rect.setWidth(rect.width() + 6);

    return rect;
}

void Note::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QRectF rect = QGraphicsTextItem::boundingRect();

    painter->fillRect(boundingRect(), Qt::white);
    painter->drawRect(boundingRect());

    //painter->drawText(rect, QString::number(x()) + ", " + QString::number(y()));
    if(!mAttachment.isEmpty()) {
        QPixmap attchIcon(":/images/attachment.svg");
        painter->drawPixmap(3, 3, 20, 20, attchIcon);
    }

    QStyleOptionGraphicsItem opt;
    QRect r = option->rect;
    r.setX(3);
    r.setY(32);
    r.setWidth(rect.width());
    r.setHeight(rect.height());

    opt.palette = option->palette;
    opt.direction = option->direction;
    opt.rect = r;
    //qDebug() <<option->rect << opt.rect;

    QGraphicsTextItem::paint(painter, &opt, widget);
}

int Note::type() const
{
    return QGraphicsItem::UserType + 10;
}

void Note::setLastModified(QDateTime dt)
{
    mLastModified = dt;
}

void Note::setAddedDate(QDateTime dt)
{
    mLastModified = dt;
}
