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
#ifndef NOTEDOCUMENT_H
#define NOTEDOCUMENT_H

#include <QGraphicsPixmapItem>
#include <QIcon>

#include "notecontent.h"

/*!
 * \class NoteDocumentName
 * An interactive document name for NoteDocument.
 */
class NoteDocumentName : public QGraphicsTextItem
{

public:
    NoteDocumentName(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0) :
        QGraphicsTextItem(parent, scene)
    {
        mScene = qobject_cast<PageScene*>(scene);
        setFlag(QGraphicsItem::ItemIsSelectable);
        setCursor(QCursor(Qt::PointingHandCursor));

    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        //painter->fillRect(boundingRect(), Qt::green);
        QGraphicsTextItem::paint(painter, option, widget);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
    {
        Q_UNUSED(e);
        QDesktopServices::openUrl(QUrl("file://" + mScene->pagePath() +"/" + toPlainText()));
    }

private:
    PageScene *mScene;

};

/*!
 * Define a new content type for documents, this consists of an icon,
 * a document name as defined by a NoteDocumentName, and the NoteOptions
 * widget.
 */
class NoteDocument : public QGraphicsPixmapItem, public NoteContent
{
public:
    NoteDocument(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type () const { return NoteType::Document; }
    virtual int contentType() const { return NoteType::Document; }

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    void setSize(QSizeF size);
    QSizeF size();
    void setPos(const QPointF &pos);

    //short file name.
    void setFile(QString file);

    void loadContent(QXmlStreamReader *stream);
    void saveContent(QXmlStreamWriter *stream);
    void deleteContent();

    void setupContextMenu();

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
    NoteDocumentName *mFileName;
    QPixmap mPix;
};

#endif // NOTEDOCUMENT_H
