#ifndef NOTECONTAINER_H
#define NOTECONTAINER_H

#include <QGraphicsItemGroup>

class NoteContainer : public QGraphicsItemGroup
{

public:
    explicit NoteContainer(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    void addItem(QGraphicsItem *item);
signals:
    
public slots:
    
};

#endif // NOTECONTAINER_H
