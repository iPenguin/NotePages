#ifndef NOTE_H
#define NOTE_H

#include <QGraphicsItem>

class Note : public QGraphicsItem
{

public:
    explicit Note(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    

    int contentType();

signals:
    
public slots:
    
};

#endif // NOTE_H
