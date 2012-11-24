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

/*!
 * \class PageScene
 * This class extends the QGraphicsScene class and includes code to
 * handle Notes and Arrows.
 */
class PageScene : public QGraphicsScene
{
    Q_OBJECT
    friend class Page;
public:
    explicit PageScene(QObject *parent = 0);
    
    //! Return the path where the files of this page are saved.
    QString pagePath() { return mPagePath; }
    //! Set the path where the files of this page are saved.
    void setPagePath(QString pp) { mPagePath = pp; }

    void incrementMaxNoteId() { mCurMaxNoteId++; }
    
    //! Set if we are in Draw Lines Mode.
    void setDrawLinesMode(bool state) { mDrawLines = state; }

    //! Use this NoteType when creating a new Note.
    void setDefaultNoteType(NoteType::Id type);

signals:
    //! This signal is emitted when an npages:// link is clicked and
    //! the user wants to change to the \a newPage Page.
    void changePage(QString newPage);

public slots:
    //! Delete the first selected item that is a Note.
    void deleteNote();
    
    //! Select a file to add as a Note.
    void addFileAsNote();
    //! Copy the file \a fileName into the Page and create a note.
    void addFileToPage(QString fileName);

    //! Perform any actions needed and then emit the changePage() signal.
    void pageLinkClicked(QString link);

private slots: 
    //! Display the context menu for the Note at \a screenPos.
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

    /*!
     * \brief Create a Note add add it to this Page.
     * 
     * If \a noteId = -1 then this is a new note, if it's >= 0 then we are loading an existing note.
     * 
     */
    Note* createNote(int noteId = -1, NoteType::Id type = NoteType::Text);

private:
    //! The last Note Id assigned, the next note id to be assigned should be mCurMaxNoteId + 1.
    int mCurMaxNoteId;
    bool mDrawLines;
    
    //! Holds the Note where the creation of an Arrow starts. 0 when not in use.
    Note *mLineStart;
    
    //! When creating a new note use this NoteType.
    NoteType::Id mDefaultNoteType;

    QGraphicsLineItem *mTempLine;

    //! The position where the user released the mouse button while creating a new Note.
    QPointF mMouseReleasePos;

    //! requires a path separator after it.
    QString mPagePath;
};

#endif // PAGESCENE_H
