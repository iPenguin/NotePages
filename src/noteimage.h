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
#ifndef NOTEIMAGE_H
#define NOTEIMAGE_H

#include <QGraphicsPixmapItem>
#include "notecontent.h"

/*!
 * Images can be displayed on the Note Pages.
 */
class NoteImage : public QGraphicsPixmapItem, public NoteContent
{
public:
    NoteImage(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type () const { return NoteType::Image; }

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    QSizeF size();
    void setSize(QSizeF size);
    void setPos(const QPointF &pos);

    void setFile(QString f);
    //! Load an image note from image data.
    void setImage(QByteArray imageData);

    void loadContent(QXmlStreamReader *stream);
    void saveContent(QXmlStreamWriter *stream);
    void deleteContent();

    void setupContextMenu();

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
    QSizeF mOldSize;
    QPointF mDiff;
};

#endif // NOTEIMAGE_H
