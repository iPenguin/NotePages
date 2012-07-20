/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef NOTEDOCUMENT_H
#define NOTEDOCUMENT_H

#include <QGraphicsObject>
#include <QIcon>

#include "notecontent.h"

class NoteDocument : public QGraphicsObject, public NoteContent
{
public:
    NoteDocument(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type () const { return NoteType::Document; }

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    void setSize(QSizeF size);
    QSizeF size();
    void setPos(const QPointF &pos);

    void setDocument(QString path, QString fileName);

    //short file name.
    QString file() { return mFile; }

    void loadContent(QXmlStreamReader *stream);
    void saveContent(QXmlStreamWriter *stream);
    void deleteContent();

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
    QGraphicsPixmapItem *mIcon;
    QGraphicsTextItem *mFileName;

    QString mPath;
    QString mFile;
};

#endif // NOTEDOCUMENT_H
