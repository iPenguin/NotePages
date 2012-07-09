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
#include <QString>

#include <QDebug>

NoteText::NoteText(QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsTextItem(parent, scene),
    mParent(parent),
    mSize(QSizeF(100,50))
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setTextInteractionFlags(Qt::TextBrowserInteraction); //Qt::TextEditorInteraction);
    setCursor(QCursor(Qt::IBeamCursor));
    setOpenExternalLinks(false);

    connect(this, SIGNAL(linkHovered(QString)), SLOT(hoveringOverLink(QString)));
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


    /** Code needed to make Spell Checking work for suggested replacements, when right clicking..
     *QTextCursor c = textCursor();
     *c.setPosition(document()->documentLayout()->hitTest(event->pos(), Qt::FuzzyHit));
     *c.select(QTextCursor::WordUnderCursor);
     *qDebug() << "selected text:" << c.selectedText();
     **/

    QGraphicsTextItem::mousePressEvent(e);
}

void NoteText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e)
{
    if (textInteractionFlags() == Qt::TextBrowserInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);

    QGraphicsTextItem::mouseDoubleClickEvent(e);
}

void NoteText::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{
    QGraphicsTextItem::hoverMoveEvent(e);
}

void NoteText::focusOutEvent(QFocusEvent *e)
{
    setTextInteractionFlags(Qt::TextBrowserInteraction);
    QGraphicsTextItem::focusOutEvent(e);
}

void NoteText::hoveringOverLink(QString link)
{
    setCursor(QCursor(Qt::PointingHandCursor));
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

void NoteText::addLink(QStringList link)
{

    textCursor().insertHtml("<a href=\""+link.last()+"\">"+link.first()+"</a>");
}
