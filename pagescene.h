#ifndef PAGESCENE_H
#define PAGESCENE_H

#include <QGraphicsScene>


class PageScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PageScene(QObject *parent = 0);
    
signals:
    
public slots:
    

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
};

#endif // PAGESCENE_H