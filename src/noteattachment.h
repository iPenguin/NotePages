/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef NOTEATTACHMENT_H
#define NOTEATTACHMENT_H

#include <QGraphicsTextItem>

class NoteAttachment : public QGraphicsTextItem
{
public:
    NoteAttachment(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    enum { Type = UserType + 10 };
    int type () const { return NoteAttachment::Type; }

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    void setAttachment(QString path, QString fileName);

    QString file() { return mFile; }
protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
    QString mPath;
    QString mFile;
};

#endif // NOTEATTACHMENT_H
