#ifndef NOTEOPTIONS_H
#define NOTEOPTIONS_H

#include <QGraphicsRectItem>

class NoteOptions : public QGraphicsRectItem
{
public:
    NoteOptions(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    enum { Type = UserType + 11 };
    int type () const { return NoteOptions::Type; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
};

#endif // NOTEOPTIONS_H
