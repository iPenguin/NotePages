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
#ifndef NOTETEXT_H
#define NOTETEXT_H

#include <QGraphicsTextItem>
#include <QDateTime>

#include "notecontent.h"

class QTextCharFormat;

/*!
 * \class NoteText
 * A rich text note that can be edited on the Page.
 * 
 */
class NoteText : public QGraphicsTextItem, public NoteContent
{
    Q_OBJECT

public:
    NoteText(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type () const { return NoteType::Text; }

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    void setSize(QSizeF size);
    QSizeF size() { return mSize; }
    void setPos(const QPointF &pos);

    void setBold(bool state);
    void setItalic(bool state);
    void setUnderline(bool state);

    void setTextBlockAlignment(Qt::Alignment align);
    void setTextEditMode(bool value);

    void addLink(QStringList link);

    void loadContent(QXmlStreamReader *stream);
    void saveContent(QXmlStreamWriter *stream);
    void deleteContent();

    QString toHtml();
    void setHtml(const QString &html);

    QMenu* contextMenu();

signals:
    void pageLink(QString link);
    void linkActivated(QString link);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);
    void focusOutEvent(QFocusEvent *e);

    void mergeFormatOnSelection(QTextCharFormat format);

    void setupContextMenu();

private:
    QGraphicsItem *mParent;

    QSizeF mSize;
};

#endif // NOTETEXT_H
