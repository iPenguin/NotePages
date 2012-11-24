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
#ifndef PAGE_H
#define PAGE_H

#include <QWidget>
#include <QUndoStack>
#include "pagescene.h"

namespace Ui {
class Page;
}

/*!
 * \class Page
 * Page is a container widget that holds a PageView via Ui::Page and the PageScene.
 * 
 */
class Page : public QWidget
{
    Q_OBJECT
    
public:
    enum TextProperty { TxtBold = 1, TxtItalic, TxtUnderline, 
                        TxtLeftJustify, TxtCenterJustify, TxtRightJustify, TxtJustify };

    explicit Page(QString pagePath, QWidget *parent = 0);
    ~Page();

    //! Return the Page id.
    int id() { return mId; }
    //! set the Page id.
    void setId(int id) { mId = id; }

    void loadPage();
    void savePage();
    bool isSaved();
    
    //! Delete and remove data.
    void deletePage();

    //! Return the undo stack for this page.
    QUndoStack* undoStack() { return mUndoStack; }

    //! Set properties on selected text, ie, bold, italics, underline, etc.
    void setTextProperties(TextProperty property, bool state);

    //! Return the current zoom level.
    int zoomLevel();

    //! Add page link to note.
    void addLinkToNote(QStringList link);

    //! Pass through to set if we are in Draw Lines Mode.
    void setDrawLinesMode(bool state) { Q_ASSERT(mScene); mScene->setDrawLinesMode(state); }

    //! Return the current note
    Note* currentNote();
    
    //! Pass through to set the default NoteType when creating a new Note on this Page.
    void setDefaultNoteType(NoteType::Id type);

    //! Set where the files for this page will be saved.
    void setPagePath(QString pp) { Q_ASSERT(mScene); mScene->setPagePath(pp + "/pages/" + QString::number(id())); }

signals:
    void zoomLevelChanged(int value);
    void changePage(QString link);

public slots:
    void changeZoom(int value);
    void nextPage(QString link);

protected:
    //! Expand the scene's bounding rect to contain \a n a Note.
    void updateSceneRect(Note *n);

private:
    Ui::Page *ui;
    PageScene *mScene;

    //! The page's unique id.
    int mId;

    QUndoStack *mUndoStack;
    
    //! If the Page has been deleted prevent it from being saved in the d'tor.
    bool mDeleted;
};

#endif // PAGE_H
