#include "note.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QTextDocument>
#include <QGraphicsTextItem>
#include <QStyleOptionGraphicsItem>

#include <QDebug>

Note::Note(QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsTextItem(parent, scene)
{
    mAdded = QDateTime::currentDateTime();

    //setAcceptHoverEvents(true);
    //setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    //setTextInteractionFlags(Qt::LinksAccessibleByMouse);

}

int Note::type() const
{
    return QGraphicsItem::UserType + 10;
}

QRectF Note::boundingRect() const
{
    QRectF r = QGraphicsTextItem::boundingRect();
    //r.setHeight(r.height() + 25);
    //r.setWidth(r.width() + 25);
    return r;
}

void Note::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF r = boundingRect();
    painter->fillRect(r, Qt::white);
    painter->drawRect(r);

    QGraphicsTextItem::paint(painter, option, widget);
}

void Note::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(e);
}

void Note::focusOutEvent(QFocusEvent *e)
{

    setTextInteractionFlags(Qt::NoTextInteraction);
    //emit lostFocus(this);

    QGraphicsTextItem::focusOutEvent(e);
}

void Note::setLastModified(QDateTime dt)
{
    mLastModified = dt;
}

void Note::setAddedDate(QDateTime dt)
{
    mLastModified = dt;
}
