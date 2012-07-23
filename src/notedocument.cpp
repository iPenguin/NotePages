/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include "notedocument.h"

#include <QPainter>
#include <QDebug>

#include <QDesktopServices>
#include <QCursor>
#include <QUrl>

NoteDocument::NoteDocument(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPixmapItem(parent, scene), NoteContent(parent, scene)
{
    mFileName = new NoteDocumentName(parent, scene);
    mFileName->setPos(20,125);

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setCursor(QCursor(Qt::PointingHandCursor));
}

QRectF NoteDocument::boundingRect() const
{
    QRectF icon = mPix.rect();
    QRectF fName = mFileName->boundingRect();
    QRectF final = icon.unite(fName);

    qreal diff = (mFileName->boundingRect().width() +20 ) - mPix.width();
    qreal start = diff / 2.0;
    if(start < final.left())
        final.setLeft(start);
    return final; //.adjusted(0,0,64,64);
}

void NoteDocument::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);
    //FIXME: draw the icon center text - 1/2 diff width.
    qreal diff = (mFileName->boundingRect().width() +20 ) - mPix.width();
    qreal start = diff / 2.0;
    //painter->drawRect(boundingRect());
    painter->drawPixmap(start,0, mPix);
}

void NoteDocument::setSize(QSizeF size)
{
    setPixmap(QPixmap(pageScene()->pagePath() + "/" + file()).scaled(size.toSize()));
}

QSizeF NoteDocument::size()
{
    return boundingRect().size();
}

void NoteDocument::setPos(const QPointF &pos)
{
    return QGraphicsPixmapItem::setPos(pos);
}

void NoteDocument::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsPixmapItem::mouseReleaseEvent(e);
}

void NoteDocument::setFile(QString file)
{
    //FIXME: make this work like the old Attachment line worked.
    if(!file.isEmpty()) {
        QFileInfo fInfo(pageScene()->pagePath() + "/" + file);
        QFileIconProvider *fip = new QFileIconProvider();
        mPix = fip->icon(fInfo).pixmap(128,128);
        QGraphicsPixmapItem::setPixmap(mPix);

        mFileName->setHtml("<a href=\"file://" + file +"\">" + file + "</a>");
    } else {
        mFileName->setHtml("");
    }

    NoteContent::setFile(file);
}

void NoteDocument::loadContent(QXmlStreamReader *stream)
{
    //TODO: test unsafe data first.
    setFile(stream->attributes().value("file").toString());

}

void NoteDocument::saveContent(QXmlStreamWriter *stream)
{
    stream->writeAttribute("file", file());
}

void NoteDocument::deleteContent()
{
    if(QFileInfo(file()).exists()) {
        QDir d(pageScene()->pagePath());
        d.remove(file());
    }
}
