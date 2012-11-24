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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDomElement>
#include <QTreeWidgetItem>
#include <QMap>
#include <QToolButton>

class QListWidgetItem;
class QXmlStreamWriter;
class LinkDialog;
class SettingsUi;

#include "page.h"
#include "updater.h"

namespace Ui {
    class MainWindow;
}

/*!
 * The primary class for the application and the UI 
 * including menus and tool bars.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //! autoLoad automatically loads the default set of pages as defined in the settings.
    explicit MainWindow(bool autoLoad = true, QWidget *parent = 0);
    ~MainWindow();

    //! This function should mainly be called from the History class, for undo/redo.
    void selectPage(int pageNumber);

public slots:
    void loadFile(QString folder);

private slots:
//File Menu
    void fileOpen();
    void fileClose();
    void fileQuit();
    void fileSave();

    void fileNew();

//Edit Menu
    void editCopy();
    void editCut();
    void editPaste();

    //! Change the selected Page, and track them in the History.
    void pageSelected(QTreeWidgetItem *current, QTreeWidgetItem *previous);

//View Menu
    void viewFullScreen();

//Tools Menu
    void toolsConnect();
    void toolsCheckForUpdates();
    void toolsSettings();

//Help Menu
    void helpAbout();
    
    //! Add a Page to the Tree as the child of the currently selected item.
    void addNewPageTreeItem();
    //! Remove Page from the tree and close the tab.
    void removePages();

    //! Move a tree item up.
    void moveItemUp();
    //! Move a tree item down.
    void moveItemDown();

    //! Show the configure options for the tree items. 
    void configureItem();

    int currentMaxPageId();
    void setCurrentMaxPageId(int newId);
    int useNextPageId() { mCurrentMaxPageId++; return mCurrentMaxPageId; }

    void changeItem(QTreeWidgetItem *item, int column);
    //! Load a Page from an npages:// link, and add it to the History.
    void loadPageFromLink(QString link);

protected slots:
    void closeTab(int tabNumber);

    //! Set zoom slider to the specified value.
    void updateZoomLevel(int percent);
    void zoomPage(int percent);

    void tabChanged(int newTab);

protected:
    void saveIndex(QString path);
    void saveIndexPages(QXmlStreamWriter *stream, QTreeWidgetItem *item);

    void populateIconList();

    //! Return the current tab as a Page, or 0 if no page.
    Page* currentPage();

private slots:
    void setTextProperties();

    void updateItemIcon(QListWidgetItem *newItem, QListWidgetItem *oldItem);

    void addLink();
    void addLinkToNote(QStringList link);

    void addNoteType();

private:
    Ui::MainWindow *ui;
    LinkDialog *mLinkDialog;
    SettingsUi *mSettingsUi;
    Updater* mUpdater;
    
    void checkUpdates(bool silent = true);
    
    void load();

    QTreeWidgetItem* findPage(int pageNumber);

    //! Update the title bar, and other pointers to the current page.
    void setCurrentPage(int pageId);

    //! Save page file.
    void saveFile(QString fileName);

    QTreeWidgetItem* loadPage(QDomElement element);

    void openPage(int pageNumber);

    void setupStatusBar();
    void setupMenubars();
    QString loadNewFolder();

    int mCurrentMaxPageId;

    //! A mapping of Page::id() to object.
    QMap <int, Page*> mPages;

    QToolButton *mAdd,
                *mRemove,
                *mUp,
                *mDown,
                *mConfigure;

    QAction *mAddTextNote,
            *mAddImageNote,
            *mAddDocumentNote;

    QSlider *mZoom;

    QString mPagesFile;
    QString mName;
    QString mPath;

    QUndoStack *mHistory;

    //! This is a temporary holder of information when loading files.
    QList<QTreeWidgetItem*> mExpandedItems;
};

#endif // MAINWINDOW_H
