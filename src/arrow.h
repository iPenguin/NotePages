#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>
#include <QXmlStreamWriter>
#include <QPointer>

class Note;

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

private:

    QPointer<Note> mStart,
                   mEnd;
};

#endif // ARROW_H
