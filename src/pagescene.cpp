/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include "pagescene.h"

#include "note.h"
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QPainter>

#include <QDataStream>
#include <QMenu>
#include <QAction>

#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>

#include <QImageReader>
#include <QUrl>
#include <QString>

#include "arrow.h"

#include <QDebug>

PageScene::PageScene(QObject *parent) :
    QGraphicsScene(parent),
    mCurMaxNoteId(1),
    mDrawLines(false),
    mLineStart(0),
    mTempLine(0)
{
    setSceneRect(0, 0, 1500,1500);

}

void PageScene::deleteNote()
{

    QList<QGraphicsItem*> items = selectedItems();
    QGraphicsItem *i = items.first();
    Note *n = qgraphicsitem_cast<Note*>(i->parentItem());
    n->removeAllArrows();
    n->deleteNote();
    delete n;
    i = 0; n = 0;
    items.removeFirst();
}

void PageScene::addDocument()
{
    QAction *a = qobject_cast<QAction*>(sender());

    QList<QGraphicsItem*> items = selectedItems();
    QGraphicsItem *i = items.first();
    Note *n = qgraphicsitem_cast<Note*>(i->parentItem());

    if(a->text() == tr("Add Document")) {

        if(n) {
            //TODO: Add an document to the note.
            //find file with dialog
            QFileDialog* fd = new QFileDialog(0, Qt::Sheet);
            fd->setDirectory(mPagePath);
            fd->setObjectName("adddocumentdialog");
            fd->setViewMode(QFileDialog::List);
            fd->setAcceptMode(QFileDialog::AcceptOpen);
            fd->open(this, SLOT(loadDocument(QString)));
        }
    } else {
        n->removeDocument();
    }
}

void PageScene::loadDocument(QString fileName)
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
    n->setDocument(fInfo.fileName());
}

void PageScene::addImage()
{

    QList<QGraphicsItem*> items = selectedItems();
    QGraphicsItem *i = items.first();
    Note *n = qgraphicsitem_cast<Note*>(i->parentItem());
    if(n) {
        //FIXME: Add a document to the note.
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

void PageScene::pageLinkClicked(QString link)
{
    emit changePage(link);
}

void PageScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawBackground(painter, rect);
}

void PageScene::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Delete || e->key() == Qt::Key_Backspace) {
        QGraphicsItem *i = selectedItems().first();
        if(!i)
            return;

        if(i->type() == Arrow::Type) {
            Arrow *a = qgraphicsitem_cast<Arrow*>(i);
            a->startItem()->removeArrow(a);
            a->endItem()->removeArrow(a);
            delete i;
        }
    }

    QGraphicsScene::keyReleaseEvent(e);
}

void PageScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem *i = itemAt(e->scenePos());

    if(mDrawLines) {

        if(!i)
            return;

        Note *n = 0;
        if(i->type() == Note::Type)
            n = qgraphicsitem_cast<Note*>(i);
        else {
            n = qgraphicsitem_cast<Note*>(i->parentItem());
        }
        mLineStart = n;

        mTempLine = new QGraphicsLineItem(QLineF(e->scenePos(), e->scenePos()), 0, this);

    } else {

        if(!i) {
            Note *n = createNewNote();
            n->setPos(e->scenePos());
            n->setPath(mPagePath);
        }

        QGraphicsScene::mousePressEvent(e);
    }
}

void PageScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{

    if (mDrawLines && mTempLine != 0) {
        QLineF newLine(mTempLine->line().p1(), e->scenePos());
        mTempLine->setLine(newLine);
    }

    QGraphicsScene::mouseMoveEvent(e);
}

void PageScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem *i = itemAt(e->scenePos());

    if(mDrawLines && mLineStart) {

        if(!i)
            return;
        if(i == mLineStart)
            return;

        Note *n = 0;
        if(i->type() == Note::Type)
            n = qgraphicsitem_cast<Note*>(i);
        else {
            n = qgraphicsitem_cast<Note*>(i->parentItem());
        }

        if(!n)
            return;

        new Arrow(mLineStart, n, 0, this);
        delete mTempLine;
        mTempLine = 0;
    } else {

        if(i) {
            foreach(QGraphicsItem *itm, selectedItems()) {
                itm->setSelected(false);
            }
            i->setSelected(true);

            if(i->type() == NoteOptions::Type) {
                showNoteOptions(e->screenPos());
            }
        }

        QGraphicsScene::mouseReleaseEvent(e);
    }
}

void PageScene::dragEnterEvent(QGraphicsSceneDragDropEvent *e)
{
    Q_UNUSED(e);
}

void PageScene::dragMoveEvent(QGraphicsSceneDragDropEvent *e)
{
    Q_UNUSED(e);
}

void PageScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *e)
{
    Q_UNUSED(e);
}

void PageScene::dropEvent(QGraphicsSceneDragDropEvent *e)
{
    const QMimeData* mime = e->mimeData();

    if (mime->hasUrls()) {
        QList<QUrl> urls = mime->urls();
        foreach(QUrl u, urls) {

            QString url = u.toString().remove(QRegExp("^[a-zA-Z].*://"));

            QFile::copy(QFileInfo(url).path() + "/" + QFileInfo(url).fileName(), mPagePath + "/" + QFileInfo(url).fileName());

            Note *n = createNewNote();
            n->setPos(e->scenePos());

            if(!true) {
                n->setDocument(QFileInfo(url).fileName());
            } else {
                n->setImage(QFileInfo(url).fileName(), QSizeF());
            }
        }
        e->setAccepted(true);

    } else if (mime->hasText()) {
        QString text = mime->text();
        Note *n = createNewNote();
        n->setPos(e->scenePos());
        n->setHtml(text);
        e->setAccepted(true);

    } else if (mime->hasImage()) {
        Note *n = createNewNote();
        QString fileName = QString::number(n->id()) + ".png";
        QImageReader *ireader = new QImageReader(fileName);

        n->setPos(e->scenePos());
        n->setSize(QSizeF(ireader->size()));

        QFile f(mPagePath + "/" + fileName);
        QDataStream ds(&f);
        mime->imageData().save(ds);
        f.close();

        qDebug() << "fileName" << mPagePath + "/" + fileName << QFileInfo(mPagePath + "/" + fileName).exists();
        n->setImage(fileName, ireader->size());
        n->setPixmap(mime->imageData().toByteArray());
        e->setAccepted(true);
    }

}

void PageScene::showNoteOptions(QPointF screenPos)
{
    QList<QGraphicsItem *> items = selectedItems();

    if(items.count() == 1) {
        if(items.first()->type() == NoteOptions::Type) {
            QMenu menu;
            Note *n = qgraphicsitem_cast<Note*>(items.first()->parentItem());

            QAction *attach;
            if(n->hasDocument()) {
                attach = new QAction(tr("Remove Document"), 0);
            } else {
                attach = new QAction(tr("Add Document"), 0);
            }
            QAction *addImg = new QAction(tr("Add Image"), 0);
            QAction *delNote = new QAction(tr("Delete Note"), 0);

            connect(attach, SIGNAL(triggered()), SLOT(addDocument()));
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
    Note *n = new Note(NoteType::Text);
    if (noteId > -1) { //load an existing note.
        newId = noteId;

    } else { //create a new note.
        mCurMaxNoteId++;
        newId = mCurMaxNoteId;
        n->setTextEditMode(true);
    }

    n->setId(newId);
    n->setSize(QSizeF(100,50));
    addItem(n);
    connect(n, SIGNAL(pageLinkClicked(QString)), SLOT(pageLinkClicked(QString)));
    return n;
}
