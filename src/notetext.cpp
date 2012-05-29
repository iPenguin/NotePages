#include "notetext.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QTextDocument>
#include <QGraphicsTextItem>
#include <QStyleOptionGraphicsItem>

#include <QDebug>

NoteText::NoteText(QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsTextItem(parent, scene)
{

}

QRectF NoteText::boundingRect() const
{
    return QGraphicsTextItem::boundingRect();
}

void NoteText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF r = boundingRect();
    painter->fillRect(r, Qt::white);
    painter->setPen(Qt::gray);
    painter->drawRect(r);

    QGraphicsTextItem::paint(painter, option, widget);
}

void NoteText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e)
{

    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(e);
}

void NoteText::focusOutEvent(QFocusEvent *e)
{

    setTextInteractionFlags(Qt::NoTextInteraction);

    QGraphicsTextItem::focusOutEvent(e);
}
