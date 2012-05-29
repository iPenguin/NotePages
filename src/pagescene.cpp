#include "pagescene.h"

#include "note.h"
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QPainter>

#include <QDebug>

PageScene::PageScene(QObject *parent) :
    QGraphicsScene(parent)
{
    setSceneRect(-500, -500, 1500,1500);

}

void PageScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->drawEllipse(QPoint(0,0),5, 5);

    QGraphicsScene::drawBackground(painter, rect);
}

void PageScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsScene::mousePressEvent(e);

}

void PageScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
/*
    QGraphicsItem *i = itemAt(e->pos().x(), e->pos().y());

    if(i && i->type() == Note::Type) {
        Note* n = qgraphicsitem_cast<Note *>(i);

        n->setTextInteractionFlags(Qt::NoTextInteraction);
    }
*/
    QGraphicsScene::mouseReleaseEvent(e);
}

void PageScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e)
{
/*
    QGraphicsItem *i = itemAt(e->pos());

    if(i && i->type() == Note::Type) {
        Note *n = qgraphicsitem_cast<Note*>(i);
        n->setTextInteractionFlags(Qt::TextEditable);
    }
*/
    QGraphicsScene::mouseDoubleClickEvent(e);
}
