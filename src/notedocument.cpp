/*********************************************************************************************\
    Note Pages - the note taking and cross reference application 
    Copyright (c) 2012 Brian C. Milco <bcmilco AT gmail DOT com>       

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; version 2
    of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
\*********************************************************************************************/
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

    //TODO: set a default icon and name, so you don't get just the arrow when a new file is set.

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setCursor(QCursor(Qt::PointingHandCursor));

    setupContextMenu();
}

QRectF NoteDocument::boundingRect() const
{
    QRectF icon = mPix.rect();
    QRectF fName = mFileName ? mFileName->boundingRect() : QRectF(10,10,10,10);
    QRectF final = icon.united(fName);

    qreal diff = (fName.width() +20 ) - mPix.width();
    qreal start = diff / 2.0;
    if(start < final.left())
        final.setLeft(start);

    // put the dotted line around the entire 'document'
    final.setWidth( final.width() + 20);
    return final;
}

void NoteDocument::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);
    //FIXME: draw the icon center text - 1/2 diff width.
    qreal diff = (mFileName->boundingRect().width() +20 ) - mPix.width();
    qreal start = diff / 2.0;

    //painter->setPen(Qt::DotLine);
    //painter->drawRect(boundingRect());
    //painter->fillRect(boundingRect(), Qt::blue);
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
    if(QFileInfo(pageScene()->pagePath() + "/" + file()).exists()) {
        QDir d(pageScene()->pagePath());
        d.remove(file());
    }
}

void NoteDocument::setupContextMenu()
{

    mContextMenu = new QMenu();
    QAction *delNote = new QAction(QObject::tr("Delete Note"), 0);

    QObject::connect(delNote, SIGNAL(triggered()), pageScene(), SLOT(deleteNote()));

    mContextMenu->addSeparator();
    mContextMenu->addAction(delNote);
}
