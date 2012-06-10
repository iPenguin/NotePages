/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef NOTETEXT_H
#define NOTETEXT_H

#include <QGraphicsTextItem>
#include <QDateTime>

class NoteText : public QGraphicsTextItem
{

public:
    NoteText(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    enum { Type = UserType + 2 };
    int type () const { return NoteText::Type; }

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    void setSize(QSizeF size);
    QSizeF size() { return mSize; }

public slots:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void focusOutEvent(QFocusEvent *e);

private:
    QGraphicsItem *mParent;

    QSizeF mSize;
};

#endif // NOTETEXT_H
