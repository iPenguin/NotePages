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
    QGraphicsScene(parent),
    mCurMaxNoteId(1)
{
    setSceneRect(-500, -500, 1500,1500);

}

void PageScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawBackground(painter, rect);
}

void PageScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem *i = itemAt(e->scenePos());
    if(!i) {
        Note *n = createNewNote();
        n->setPos(e->scenePos());

    }
    QGraphicsScene::mousePressEvent(e);
}

void PageScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem *i = itemAt(e->scenePos());
    if(i && i->type() == NoteOptions::Type) {
        showNoteOptions(e->screenPos());
    }

    QGraphicsScene::mouseReleaseEvent(e);
}

void PageScene::showNoteOptions(QPointF screenPos)
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
            menu.exec(screenPos.toPoint());
        }
    }

}

Note* PageScene::createNewNote(int noteId)
{
    qDebug() << mCurMaxNoteId;
    if(noteId >= mCurMaxNoteId)
        mCurMaxNoteId = noteId + 1;

    int newId;
    Note *n = new Note();
    if (noteId > -1) { //load an existing note.
        newId = noteId;

    } else { //create a new note.
        newId = mCurMaxNoteId;
        mCurMaxNoteId++;
    }

    n->setId(newId);
    n->setSize(QSizeF(100,50));
    addItem(n);

    return n;
}
