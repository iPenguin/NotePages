/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef NOTECONTENT_H
#define NOTECONTENT_H

#include <QObject>
#include <QGraphicsItem>
#include <QtGui>

#include <QXmlStreamWriter>
#include "pagescene.h"
#include "debug.h"

class Note;

namespace NoteType
{
    enum Id { Text = Qt::UserRole + 10, Image, Document, Unknown = Qt::UserRole + 255 };
}


class NoteContent
{
public:
    NoteContent(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    virtual int type() const = 0;

    virtual int contentType() const { return NoteType::Text; }

    //short file name w/o the path.
    virtual QString file() { return mFile; }
    virtual void setFile(QString file) { mFile = file; }

    virtual QString toHtml() { debug("unimplemented"); return ""; }
    virtual void setHtml(QString html) { debug("unimplemented"); Q_UNUSED(html); }
    virtual void setTextEditMode(bool value) { debug("unimplemented"); Q_UNUSED(value); }

    virtual void setImage(QString image, QSizeF size) { debug("unimplemented"); Q_UNUSED(image); Q_UNUSED(size); }
    virtual void setImage(QByteArray imageData) { debug("unimplemented"); Q_UNUSED(imageData); }
    virtual QPixmap toPixmap() { debug("unimplemented"); return QPixmap(); }

    virtual void setDocument(QString path, QString doc) { debug("unimplemented"); Q_UNUSED(path); Q_UNUSED(doc); }

    virtual void setPos(const QPointF &pos) = 0;
    virtual QSizeF size() = 0;
    virtual void setSize(QSizeF size) = 0;

    virtual void loadContent(QXmlStreamReader *stream) = 0;
    virtual void saveContent(QXmlStreamWriter *stream) = 0;
    virtual void deleteContent() = 0;

    PageScene* pageScene() { return mScene; }
    Note* note() { return mNote; }

private:
    QString mFile;
    PageScene *mScene;
    Note *mNote;
};

//Q_DECLARE_INTERFACE(NoteContent, "com.notebook-pages.npp.NoteContent/1.0")

#endif // NOTECONTENT_H
