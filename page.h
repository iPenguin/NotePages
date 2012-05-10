#ifndef PAGE_H
#define PAGE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

class Page : public QWidget
{
    Q_OBJECT
public:
    explicit Page(QGraphicsView* view, QWidget *parent = 0);
    
signals:
    
public slots:
    
private:
    QGraphicsScene *mScene;

};

#endif // PAGE_H
