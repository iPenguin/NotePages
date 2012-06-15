/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include "pagescene.h"

#include "note.h"
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QPainter>

#include <QMenu>
#include <QAction>
#include "noteoptions.h"

#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>

#include <QDebug>

PageScene::PageScene(QObject *parent) :
    QGraphicsScene(parent),
    mCurMaxNoteId(1)
{
    setSceneRect(-500, -500, 1500,1500);

}

void PageScene::deleteNote()
{

    QList<QGraphicsItem*> items = selectedItems();
    QGraphicsItem *i = items.first();
    Note *n = qgraphicsitem_cast<Note*>(i->parentItem());
    n->deleteNote();
    delete n;
    i = 0; n = 0;
    items.removeFirst();
}

void PageScene::addAttachment()
{
    QAction *a = qobject_cast<QAction*>(sender());

    QList<QGraphicsItem*> items = selectedItems();
    QGraphicsItem *i = items.first();
    Note *n = qgraphicsitem_cast<Note*>(i->parentItem());

    if(a->text() == tr("Add Attachment")) {

        if(n) {
            //TODO: Add an attachment to the note.
            //find file with dialog
            QFileDialog* fd = new QFileDialog(0, Qt::Sheet);
            fd->setDirectory(mPagePath);
            fd->setObjectName("addattachmentdialog");
            fd->setViewMode(QFileDialog::List);
            //fd->setFileMode( QFileDialog::Directory );
            fd->setAcceptMode(QFileDialog::AcceptOpen);
            fd->open(this, SLOT(loadAttachment(QString)));
        }
    } else {
        n->removeAttachment();
    }
}

void PageScene::loadAttachment(QString fileName)
{

    QList<QGraphicsItem*> items = selectedItems();
    QGraphicsItem *i = items.first();
    Note *n = qgraphicsitem_cast<Note*>(i->parentItem());
    //copy file into the directory
    QFileInfo fInfo(fileName);
    if(!fInfo.exists())
        return;

    QFile f(fileName);

    //If the destination file exists offer options.
    if(QFileInfo(mPagePath + fInfo.fileName()).exists()) {
        QMessageBox::information(0, tr("Destination file exists"), tr("The destination file exists"), 1);
        //TODO: more information, and figure out user response and react appropriately.
        return;
    }

    f.copy(mPagePath + "/" + fInfo.fileName());
    n->setAttachment(fInfo.fileName());
}

void PageScene::addImage()
{

    QList<QGraphicsItem*> items = selectedItems();
    QGraphicsItem *i = items.first();
    Note *n = qgraphicsitem_cast<Note*>(i->parentItem());
    if(n) {
        //FIXME: Add an attachment to the note.
        //find file with dialog
        QFileDialog* fd = new QFileDialog(0, Qt::Sheet);
          fd->setDirectory(mPagePath);
          fd->setObjectName("addimagedialog");
          fd->setViewMode(QFileDialog::List);
          //fd->setFileMode( QFileDialog::Directory );
          fd->setAcceptMode(QFileDialog::AcceptOpen);
          fd->open(this, SLOT(loadImage(QString)));
    }
}

void PageScene::loadImage(QString fileName)
{

    QList<QGraphicsItem*> items = selectedItems();
    QGraphicsItem *i = items.first();
    Note *n = qgraphicsitem_cast<Note*>(i->parentItem());
    //copy file into the directory
    QFileInfo fInfo(fileName);
    if(!fInfo.exists())
        return;

    QFile f(fileName);

    //If the destination file exists offer options.
    if(QFileInfo(mPagePath + fInfo.fileName()).exists()) {
        QMessageBox::information(0, tr("Destination file exists"), tr("The destination file exists"), 1);
        //TODO: more information, and figure out user response and react appropriately.
        return;
    }

    f.copy(mPagePath + "/" + fInfo.fileName());
    n->setImage(fInfo.fileName(), QSizeF());

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
        n->setPath(mPagePath);

    }
    QGraphicsScene::mousePressEvent(e);
}

void PageScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem *i = itemAt(e->scenePos());
    if(i && i->type() == NoteOptions::Type) {
        foreach(QGraphicsItem *itm, selectedItems()) {
            itm->setSelected(false);
        }
        i->setSelected(true);
        showNoteOptions(e->screenPos());
    }

    QGraphicsScene::mouseReleaseEvent(e);
}

void PageScene::showNoteOptions(QPointF screenPos)
{
    QList<QGraphicsItem *> items = selectedItems();

    if(items.count() == 1) {
        if(items.first()->type() == NoteOptions::Type) {
            QMenu menu;
            Note *n = qgraphicsitem_cast<Note*>(items.first()->parentItem());

            QAction *attach;
            if(n->hasAttachment()) {
                attach = new QAction(tr("Remove Attachment"), 0);
            } else {
                attach = new QAction(tr("Add Attachment"), 0);
            }
            QAction *addImg = new QAction(tr("Add Image"), 0);
            QAction *delNote = new QAction(tr("Delete Note"), 0);

            connect(attach, SIGNAL(triggered()), SLOT(addAttachment()));
            connect(addImg, SIGNAL(triggered()), SLOT(addImage()));
            connect(delNote, SIGNAL(triggered()), SLOT(deleteNote()));

            menu.addAction(attach);
            menu.addAction(addImg);
            menu.addSeparator();
            menu.addAction(delNote);

            menu.exec(screenPos.toPoint());
        }
    }
}

Note* PageScene::createNewNote(int noteId)
{

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
