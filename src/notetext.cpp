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
#include "note.h"

#include "debug.h"
#include <QDebug>

NoteText::NoteText(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsTextItem(parent, scene), NoteContent(parent, scene),
      mParent(parent),
      mSize(QSizeF(100,50))
{

    setFlag(QGraphicsItem::ItemIsSelectable);
    setTextInteractionFlags(Qt::TextBrowserInteraction);
    setCursor(QCursor(Qt::IBeamCursor));
    setOpenExternalLinks(false);

    setupContextMenu();
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
    painter->setPen(Qt::gray);
    painter->setBrush(QColor(Qt::white));
    painter->drawRoundedRect(boundingRect(), 5, 5);

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

void NoteText::focusOutEvent(QFocusEvent *e)
{
    setTextInteractionFlags(Qt::TextBrowserInteraction);
    QGraphicsTextItem::focusOutEvent(e);
}

void NoteText::setSize(QSizeF size)
{
    if(size.width() < 5)
        size.setWidth(5);
    setTextWidth(size.width());

    if(size.height() < document()->size().height())
        size.setHeight(document()->size().height());
    if(size.width() < document()->size().width())
        size.setWidth(document()->size().width());
    mSize = size;
}

void NoteText::setPos(const QPointF &pos)
{
    return QGraphicsTextItem::setPos(pos);
}

void NoteText::mergeFormatOnSelection(QTextCharFormat format)
{

    if(!textCursor().hasSelection())
        textCursor().select(QTextCursor::WordUnderCursor);

    textCursor().mergeCharFormat(format);
}

void NoteText::setupContextMenu()
{

    mContextMenu = new QMenu();
    QAction *delNote = new QAction(tr("Delete Note"), 0);

    connect(delNote, SIGNAL(triggered()), pageScene(), SLOT(deleteNote()));

    mContextMenu->addSeparator();
    mContextMenu->addAction(delNote);
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

void NoteText::setTextEditMode(bool value)
{
    setTextInteractionFlags(value ? Qt::TextEditorInteraction : Qt::TextBrowserInteraction);
}

void NoteText::addLink(QStringList link)
{

    textCursor().insertHtml("<a href=\""+link.last()+"\">"+link.first()+"</a>");
}

void NoteText::loadContent(QXmlStreamReader *stream)
{

    QString fileName = stream->attributes().value("file").toString();
    QFile f(pageScene()->pagePath() + "/" + fileName);

    if(!f.open(QIODevice::ReadOnly)) {
        qDebug() << "Error opening note - " << f.fileName();
        return;
    }

    QString text = f.readAll();
    QGraphicsTextItem::setHtml(text);
}

void NoteText::saveContent(QXmlStreamWriter *stream)
{

    QString noteFile = "note" + QString::number(note()->id()) + ".html";
    stream->writeAttribute("file", noteFile);

    if(!toHtml().isEmpty()) {
        QFile f(pageScene()->pagePath() + "/" + noteFile);

        if(!f.open(QFile::WriteOnly)) {
            qDebug() << "error opening file for writing: " << f.fileName();
        }

        QTextStream out(&f);
        out << toHtml();
        f.flush();
        f.close();
    }

}

void NoteText::deleteContent()
{
    QString noteFile = pageScene()->pagePath() + "/note" + QString::number(note()->id()) + ".html";

    if(QFileInfo(noteFile).exists()) {
        QDir d(pageScene()->pagePath());
        d.remove(noteFile);
    }

}

QString NoteText::toHtml()
{
    return document()->toHtml();
}
