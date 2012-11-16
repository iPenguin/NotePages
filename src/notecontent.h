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
#ifndef NOTECONTENT_H
#define NOTECONTENT_H

#include <QObject>
#include <QGraphicsItem>
#include <QtGui>

#include <QXmlStreamWriter>
#include "pagescene.h"
#include "debug.h"

#include "pageglobals.h"
class Note;

class NoteContent
{
    friend class NoteDocument;
public:
    NoteContent(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    virtual int type() const = 0;

    virtual int contentType() const { return NoteType::Text; }

    //short file name w/o the path.
    virtual QString file() { return mFile; }
    virtual void setFile(QString file) { mFile = file; }

    virtual QString toHtml() { debug("unimplemented"); return ""; }
    virtual void setHtml(const QString &html) { debug("unimplemented"); Q_UNUSED(html); }
    virtual void setTextEditMode(bool value) { debug("unimplemented"); Q_UNUSED(value); }

    virtual void setImage(QByteArray imageData) { debug("unimplemented"); Q_UNUSED(imageData); }
    virtual QPixmap toPixmap() { debug("unimplemented"); return QPixmap(); }

    virtual void setPos(const QPointF &pos) = 0;
    virtual QSizeF size() = 0;
    virtual void setSize(QSizeF size) = 0;

    virtual void loadContent(QXmlStreamReader *stream) = 0;
    virtual void saveContent(QXmlStreamWriter *stream) = 0;
    virtual void deleteContent() = 0;

    QMenu* contextMenu() { return mContextMenu; }

    PageScene* pageScene() { return mScene; }
    Note* note() { return mNote; }

protected:
    QString mFile;
    QMenu* mContextMenu;

private:
    PageScene *mScene;
    Note *mNote;


};

//Q_DECLARE_INTERFACE(NoteContent, "com.notebook-pages.npp.NoteContent/1.0")

#endif // NOTECONTENT_H