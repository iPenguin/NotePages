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
#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>
#include <QXmlStreamWriter>
#include <QPointer>

class Note;
/*!
 * \class Arrow
 * This class draws the lines between Notes.
 */
class Arrow : public QGraphicsLineItem
{
public:
    Arrow(Note *start, Note *end, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    ~Arrow();

    enum { Type = UserType + 15 };
    int type () const { return Arrow::Type; }

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    void updatePosition();
    void saveArrow(QXmlStreamWriter *stream);

    Note* startItem() { return mStart; }
    Note* endItem() { return mEnd; }

private:

    QPointer<Note> mStart,
                   mEnd;
};

#endif // ARROW_H
