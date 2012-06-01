#ifndef NOTEHANDLER_H
#define NOTEHANDLER_H

#include <QGraphicsRectItem>

class NoteHandler : public QGraphicsRectItem
{

public:
    explicit NoteHandler(QGraphicsItem *parent = 0);
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *e);

signals:
    
public slots:
    
};

#endif // NOTEHANDLER_H
