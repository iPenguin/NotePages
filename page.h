#ifndef PAGE_H
#define PAGE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

class Page : public QWidget
{
    Q_OBJECT
public:
    explicit Page(QString pagePath, QWidget *parent = 0);

    int id() { return mId; }
signals:
    
public slots:
    
private:
    QGraphicsView  *mView;
    QGraphicsScene *mScene;

    int mId;
};

#endif // PAGE_H
