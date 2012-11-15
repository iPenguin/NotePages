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

class Page : public QWidget
{
    Q_OBJECT
    
public:
    enum TextProperty { TxtBold = 1, TxtItalic,
                        TxtUnderline, TxtLeftJustify, TxtCenterJustify, TxtRightJustify, TxtJustify };

    explicit Page(QString pagePath, QWidget *parent = 0);
    ~Page();

    int id() { return mId; }
    void setId(int id) { mId = id; }

    void loadPage();
    void savePage();
    bool isSaved();
    //delete and remove data.
    void deletePage();

    QUndoStack* undoStack() { return mUndoStack; }

    //Set properties on selected text, ie, bold, italics, underline, etc.
    void setTextProperties(TextProperty property, bool state);

    int currentZoomLevel();

    //Add page link to note.
    void addLinkToNote(QStringList link);

    void setDrawLines(bool state) { Q_ASSERT(mScene); mScene->setDrawLines(state); }

    Note* currentNote();
    void setDefaultNoteType(NoteType::Id type);

    void setPagePath(QString pp) { Q_ASSERT(mScene); mScene->setPagePath(pp + "/pages/" + QString::number(id())); }

signals:
    void zoomLevelChanged(int value);
    void changePage(QString link);

public slots:
    void zoomChanged(int value);
    void nextPage(QString link);

protected:
    void updateSceneRect(Note *n);

private:
    Ui::Page *ui;
    PageScene *mScene;

    int mId;

    QUndoStack *mUndoStack;
    bool mDeleted;
};

#endif // PAGE_H
