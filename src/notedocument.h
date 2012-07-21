/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef NOTEDOCUMENT_H
#define NOTEDOCUMENT_H

#include <QGraphicsPixmapItem>
#include <QIcon>

#include "notecontent.h"

class NoteDocument : public QGraphicsPixmapItem, public NoteContent
{
public:
    NoteDocument(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type () const { return NoteType::Document; }

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
    QGraphicsTextItem *mFileName;
    QPixmap mPix;
};

#endif // NOTEDOCUMENT_H
