#include "note.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

Note::Note(QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsTextItem(parent, scene)
{
    mAdded = QDateTime::currentDateTime();

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

QRectF Note::boundingRect() const
{
    return QGraphicsTextItem::boundingRect();
}

void Note::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsTextItem::paint(painter, option, widget);
}

int Note::contentType()
{
    return 1;
}

void Note::setLastModified(QDateTime dt)
{
    mLastModified = dt;
}

void Note::setAddedDate(QDateTime dt)
{
    mLastModified = dt;
}
