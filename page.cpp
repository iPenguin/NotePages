#include "page.h"

Page::Page(QGraphicsView* view, QWidget *parent) :
    QWidget(parent)
{

    mScene = new QGraphicsScene(this);
    view->setScene(mScene);

}
