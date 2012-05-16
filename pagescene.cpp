#include "pagescene.h"

#include "note.h"
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

#include <QDebug>

PageScene::PageScene(QObject *parent) :
    QGraphicsScene(parent)
{
}


void PageScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{

    Note *n = qgraphicsitem_cast<Note*>(itemAt(e->pos().x(), e->pos().y()));
    if(!n)
        return;

    QString text = n->document()->documentLayout()->anchorAt(e->pos());

    qDebug() << text;

}
