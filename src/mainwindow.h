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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //autoLoad automatically loads the default set of pages.
    explicit MainWindow(bool autoLoad = true, QWidget *parent = 0);
    ~MainWindow();

    //This function should mainly be called from the History class, for undo/redo.
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

    void pageSelected(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void toolsSettings();

//View Menu
    void viewFullScreen();

//Tools Menu
    void toolsConnect();
    void toolsCheckForUpdates();

//Help Menu
    void helpAbout();

    void addNewPageTreeItem();
    void removePages();

    void moveItemUp();
    void moveItemDown();

    void configureItem();

    int currentMaxPageId();
    void setCurrentMaxPageId(int newId);
    int useNextPageId() { mCurrentMaxPageId++; return mCurrentMaxPageId; }

    void changeItem(QTreeWidgetItem *item, int column);

    void loadPageFromLink(QString link);

protected slots:
    void closeTab(int tabNumber);

    //set zoom slider to the specified value.
    void updateZoomLevel(int percent);
    void zoomPage(int percent);

    void tabChanged(int newTab);

protected:
    void saveIndex(QString path);
    void saveIndexPages(QXmlStreamWriter *stream, QTreeWidgetItem *item);

    void populateIconList();

    //return the current tab as a Page, or 0 if no page.
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

    //update the titlebar, and other pointers to the current page.
    void setCurrentPage(int pageId);

    //save page file.
    void saveFile(QString fileName);

    QTreeWidgetItem* loadPage(QDomElement element);

    void openPage(int pageNumber);

    void setupStatusBar();
    void setupMenubars();
    QString loadNewFolder();

    int mCurrentMaxPageId;

    //A mapping of page_id to object.
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

    //This is a temporary holder of information when loading files.
    QList<QTreeWidgetItem*> mExpandedItems;
};

#endif // MAINWINDOW_H
