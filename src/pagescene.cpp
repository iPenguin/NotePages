#include "pagescene.h"

#include "note.h"
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

#include <QDebug>

PageScene::PageScene(QObject *parent) :
    QGraphicsScene(parent)
{
    setSceneRect(-100, -100, 2000,2000);

}


void PageScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{

    Note *n = qgraphicsitem_cast<Note*>(itemAt(e->pos().x(), e->pos().y()));
    if(!n)
        return;

    //QString text = n->document()->documentLayout()->anchorAt(e->scenePos());

    //qDebug() << text;

    QGraphicsScene::mouseReleaseEvent(e);
}
