#include "pagescene.h"

#include "note.h"
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QPainter>

#include <QMenu>
#include <QAction>
#include "noteoptions.h"

#include <QDebug>

PageScene::PageScene(QObject *parent) :
    QGraphicsScene(parent)
{
    setSceneRect(-500, -500, 1500,1500);

    connect(this, SIGNAL(selectionChanged()), SLOT(showNoteOptions()));
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
    QGraphicsScene::mouseReleaseEvent(e);
}

void PageScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e)
{

    QGraphicsScene::mouseDoubleClickEvent(e);
}

void PageScene::showNoteOptions()
{
    QList<QGraphicsItem *> items = selectedItems();

    qDebug() << "show note Options" << items.count();
    if(items.count() == 1) {
        qDebug() << "single item type" << items.first()->type() << NoteOptions::Type;
        if(items.first()->type() == NoteOptions::Type) {
            QMenu menu;
            QAction* copyAction = new QAction(tr("Copy"), 0);
            QAction* cutAction = new QAction(tr("Cut"), 0);
            QAction* pasteAction = new QAction(tr("Paste"), 0);

            //connect(copyAction, SIGNAL(triggered()), SLOT(copy()));
            //connect(cutAction, SIGNAL(triggered()), SLOT(cut()));
            //connect(pasteAction, SIGNAL(triggered()), SLOT(paste()));

            menu.addAction(copyAction);
            menu.addAction(cutAction);
            menu.addAction(pasteAction);
            menu.exec(items.first()->pos().toPoint());
        }
    }

}
