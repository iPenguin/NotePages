#include "notetext.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QTextDocument>
#include <QGraphicsTextItem>
#include <QStyleOptionGraphicsItem>

#include <QDebug>

NoteText::NoteText(QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsTextItem(parent, scene),
    mParent(parent),
    mSize(QSizeF(10,10))
{

}

QRectF NoteText::boundingRect() const
{
    QRectF rect = QGraphicsTextItem::boundingRect();
    rect.setWidth(mSize.width());
    rect.setHeight(mSize.height());
    return rect;
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

void NoteText::setSize(QSizeF size)
{
    if(size.width() < 5)
        size.setWidth(5);
    if(size.height() < 5)
        size.setHeight(5);
    mSize = size;
}
