/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef NOTEDOCUMENT_H
#define NOTEDOCUMENT_H

#include <QGraphicsPixmapItem>
#include <QIcon>

#include "notecontent.h"

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

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
    {

        QDesktopServices::openUrl(QUrl("file://" + mScene->pagePath() +"/" + toPlainText()));
    }

private:
    PageScene *mScene;

};


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

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
    NoteDocumentName *mFileName;
    QPixmap mPix;
};

#endif // NOTEDOCUMENT_H
