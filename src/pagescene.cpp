/*********************************************************************************************\
    Note Pages - the note taking and cross reference application 
    Copyright (c) 2012 Brian C. Milco <bcmilco AT gmail DOT com>       

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; version 2
    of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
\*********************************************************************************************/
#include "pagescene.h"

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

#include <QUrl>
#include <QString>

#include "arrow.h"
#include "note.h"

#include <QDebug>

PageScene::PageScene(QObject *parent) :
    QGraphicsScene(parent),
    mCurMaxNoteId(1),
    mDrawLines(false),
    mLineStart(0),
    mDefaultNoteType(NoteType::Text),
    mTempLine(0)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setSceneRect(0, 0, 1500,1500);

}

void PageScene::setDefaultNoteType(NoteType::Id type)
{
    mDefaultNoteType = type;
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

void PageScene::addFileAsNote()
{

    QFileDialog* fd = new QFileDialog(qApp->activeWindow(), Qt::Sheet);
    fd->setDirectory(mPagePath);
    fd->setObjectName("addfiledialog");
    fd->setViewMode(QFileDialog::List);
    fd->setAcceptMode(QFileDialog::AcceptOpen);
    fd->open(this, SLOT(loadFile(QString)));

}

void PageScene::loadFile(QString fileName)
{
    Note *n = createNewNote(-1, mDefaultNoteType);
    n->setPos(mMouseReleasePos);
    n->setPath(mPagePath);

    if(fileName.isEmpty()) {
        n->deleteNote();
        n->deleteLater();
        return;
    }

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
    n->setFile(fInfo.fileName());
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
    //If we didn't click on something create a note.
    if(!i) {
        Note *n = createNewNote();
        n->setPos(e->scenePos());
    }

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
        if(i == mLineStart) {
            qDebug() << "end == start";
            return;
        }

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
        } else {
            if(mDefaultNoteType != NoteType::Text) {
                mMouseReleasePos = e->scenePos();
                addFileAsNote();
            } else {
                Note *n = createNewNote(-1, NoteType::Text);
                n->setPos(e->scenePos());
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

    if (mime->hasImage()) {

        Note *n = createNewNote(-1, NoteType::Image);
        QString fileName = QString::number(n->id()) + ".png";

        n->setPos(e->scenePos());

        QFile f(mPagePath + "/" + fileName);
        QDataStream ds(&f);
        mime->imageData().save(ds);
        f.close();

        n->setFile(fileName);
        n->setPixmap(mime->imageData().toByteArray());
        e->setAccepted(true);

    } else if (mime->hasText()) {
        QString text = mime->text();
        Note *n = createNewNote(-1, NoteType::Text);
        n->setPos(e->scenePos());
        n->setHtml(text);
        e->setAccepted(true);

    } else {
        QList<QUrl> urls = mime->urls();
        foreach(QUrl u, urls) {

            Note *n = 0;

            QString url = u.toString().remove(QRegExp("^[a-zA-Z].*://"));
            QFile::copy(QFileInfo(url).path() + "/" + QFileInfo(url).fileName(), mPagePath + "/" + QFileInfo(url).fileName());

            QImageReader *ireader = new QImageReader(mPagePath + "/" + QFileInfo(url).fileName());

            if(ireader->canRead()) {
                n = createNewNote(-1, NoteType::Image);
            } else {
                n = createNewNote(-1, NoteType::Document);
            }

            n->setFile(QFileInfo(url).fileName());
            n->setPos(e->scenePos());

        }
        e->setAccepted(true);
    }
}

void PageScene::showNoteOptions(QPointF screenPos)
{
    QList<QGraphicsItem *> items = selectedItems();

    if(items.count() == 1) {
        if(items.first()->type() == NoteOptions::Type) {

            Note *n = qgraphicsitem_cast<Note*>(items.first()->parentItem());
            n->contextMenu()->exec(screenPos.toPoint());
        }
    }
}

Note* PageScene::createNewNote(int noteId, NoteType::Id type)
{

    if(noteId >= mCurMaxNoteId)
        mCurMaxNoteId = noteId + 1;

    int newId;
    Note *n = new Note(type, 0, this);

    if (noteId > -1) { //load an existing note.
        newId = noteId;

    } else { //create a new note.
        mCurMaxNoteId++;
        newId = mCurMaxNoteId;
        n->setTextEditMode( (type == NoteType::Text ? true : false) );
    }

    n->setId(newId);
    n->setSize(QSizeF(100,50));
    connect(n, SIGNAL(pageLinkClicked(QString)), SLOT(pageLinkClicked(QString)));
    return n;
}
