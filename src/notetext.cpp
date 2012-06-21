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
    setTextInteractionFlags(Qt::TextBrowserInteraction);
    setCursor(QCursor(Qt::IBeamCursor));

    //FIXME: emit selection changed information and pass the correct status of text b/i/u left/right/center/justify.
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

void NoteText::mergeFormatOnSelection(QTextCharFormat format)
{

    if(!textCursor().hasSelection())
        textCursor().select(QTextCursor::WordUnderCursor);

    textCursor().mergeCharFormat(format);
    //mergeCurrentCharFormat(format);

}


void NoteText::setBold(bool state)
{
    QTextCharFormat format = textCursor().charFormat();
    format.setFontWeight(state ? QFont::Bold : QFont::Normal);

    mergeFormatOnSelection(format);

}

void NoteText::setItalic(bool state)
{
    QTextCharFormat format = textCursor().charFormat();
    format.setFontItalic(state);

    mergeFormatOnSelection(format);
}

void NoteText::setUnderline(bool state)
{
    QTextCharFormat format = textCursor().charFormat();
    format.setFontUnderline(state);

    mergeFormatOnSelection(format);
}

void NoteText::setTextBlockAlignment(Qt::Alignment align)
{
    QTextBlockFormat format = textCursor().blockFormat();
    format.setAlignment(align);

    textCursor().setBlockFormat(format);
}
