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
    : NoteContent(parent, scene)
{
    mIcon = new QGraphicsPixmapItem(parent, scene);
    mFileName = new QGraphicsTextItem(parent, scene);

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setCursor(QCursor(Qt::PointingHandCursor));
}

QRectF NoteDocument::boundingRect() const
{
    QRectF icon = mIcon->boundingRect();
    QRectF fName = mFileName->boundingRect();
    QRectF final = icon.unite(fName);
    return final; //.adjusted(0,0,64,64);
}

void NoteDocument::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    mIcon->paint(painter, option, widget);
    mFileName->paint(painter, option, widget);
}

void NoteDocument::setSize(QSizeF size)
{
    //TODO: give the text the bottom 12pxs or 24 and the icon the rest.
    mIcon->setPixmap(QPixmap(pageScene()->pagePath() + "/" + file()).scaled(size.toSize()));
}

QSizeF NoteDocument::size()
{
    return boundingRect().size();
}

void NoteDocument::setPos(const QPointF &pos)
{
    return QGraphicsObject::setPos(pos);
}

void NoteDocument::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{

    QDesktopServices::openUrl(QUrl("file://"+mPath +"/" +mFile));
    QGraphicsObject::mouseReleaseEvent(e);
}

void NoteDocument::setDocument(QString path, QString fileName)
{
    //FIXME: make this work like the old Attachment line worked.
    if(!fileName.isEmpty()) {
        QFileInfo fInfo(mPath + "/" + fileName);
        QFileIconProvider *fip = new QFileIconProvider();
        mIcon->setPixmap(fip->icon(fInfo).pixmap(128));
    }

    if(fileName.isEmpty()) {
        mFileName->setHtml("");
        mFile = fileName;
    } else {
        mFileName->setHtml("<a href=\"file://" + fileName +"\">" + fileName + "</a>");
        mPath = path;
        mFile = fileName;
    }
}

void NoteDocument::loadContent(QXmlStreamReader *stream)
{
    //TODO: test unsafe data first.
    mFile = stream->attributes().value("file").toString();
}

void NoteDocument::saveContent(QXmlStreamWriter *stream)
{
    stream->writeAttribute("file", file());
}

void NoteDocument::deleteContent()
{
    if(QFileInfo(file()).exists()) {
        QDir d(mPath);
        d.remove(file());
    }
}
