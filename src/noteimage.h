#ifndef NOTEIMAGE_H
#define NOTEIMAGE_H

#include <QGraphicsPixmapItem>
#include "notecontent.h"

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

    void setImage(QString image, QSizeF size);
    void setImage(QByteArray imageData);

    void loadContent(QXmlStreamReader *stream);
    void saveContent(QXmlStreamWriter *stream);
    void deleteContent();

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
    QSizeF mOldSize;
    QPointF mDiff;
};

#endif // NOTEIMAGE_H
