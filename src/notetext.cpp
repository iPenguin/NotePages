/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include "notetext.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>
#include <QTextDocument>
#include <QGraphicsTextItem>
#include <QStyleOptionGraphicsItem>

#include <QFocusEvent>

#include <QDebug>

NoteText::NoteText(QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsTextItem(parent, scene),
    mParent(parent),
    mSize(QSizeF(100,50))
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setCursor(QCursor(Qt::IBeamCursor));
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

void NoteText::mousePressEvent(QGraphicsSceneMouseEvent *e)
{

    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);

    QGraphicsTextItem::mousePressEvent(e);
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
    setTextWidth(size.width());
    if(size.height() < document()->size().height())
        size.setHeight(document()->size().height());

    mSize = size;
}
