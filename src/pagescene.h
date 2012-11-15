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
#ifndef PAGESCENE_H
#define PAGESCENE_H

#include <QGraphicsScene>
#include "pageglobals.h"

class Note;

class PageScene : public QGraphicsScene
{
    Q_OBJECT
    friend class Page;
public:
    explicit PageScene(QObject *parent = 0);
    
    QString pagePath() { return mPagePath; }
    void setPagePath(QString pp) { mPagePath = pp; }

    void incrementMaxNoteId() { mCurMaxNoteId++; }
    void setDrawLines(bool state) { mDrawLines = state; }

    void setDefaultNoteType(NoteType::Id type);

signals:
    void changePage(QString newPage);

public slots:
    void deleteNote();
    void addFileAsNote();
    void loadFile(QString fileName);

    void pageLinkClicked(QString link);

private slots:
    void showNoteOptions(QPointF screenPos);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

    void keyReleaseEvent(QKeyEvent *e);

    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

    void dragEnterEvent(QGraphicsSceneDragDropEvent *e);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *e);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *e);
    void dropEvent(QGraphicsSceneDragDropEvent *e);

    Note* createNewNote(int noteId = -1, NoteType::Id type = NoteType::Text);

private:

    int mCurMaxNoteId;
    bool mDrawLines;
    Note *mLineStart;
    NoteType::Id mDefaultNoteType;

    QGraphicsLineItem *mTempLine;

    QPointF mMouseReleasePos;

    //requires a path seperator after it.
    QString mPagePath;
};

#endif // PAGESCENE_H
