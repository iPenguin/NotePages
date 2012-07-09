/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
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

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(bool autoLoad = true, QWidget *parent = 0);
    ~MainWindow();

    //This function should mainly be called from the History class, for undo/redo.
    void selectPage(int pageNumber);

public slots:
    void loadFile(QString folder);

private slots:
//File
    void fileOpen();
    void fileClose();
    void fileQuit();
    void fileSave();

    void fileNew();

//Edit
    void editCopy();
    void editCut();
    void editPaste();

    void pageSelected(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void toolsSettings();

//Help
    void helpAbout();

    void addNewPage();
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

private slots:
    void setTextProperties();

    void updateItemIcon(QListWidgetItem *newItem, QListWidgetItem *oldItem);

    void addLink();
    void addLinkToNote(QStringList link);

private:
    Ui::MainWindow *ui;
    LinkDialog *mLinkDialog;
    SettingsUi *mSettingsUi;

    void load();

    QTreeWidgetItem* findPage(int pageNumber);

    //update the titlebar, and other pointers to the current page.
    void setCurrentPage(int pageId);

    void saveFile(QString fileName);

    QTreeWidgetItem* loadPage(QDomElement element);

    void openPage(int pageNumber);

    void setupStatusBar();
    void setupMenubars();

    int mCurrentMaxPageId;

    //A mapping of page_id to object.
    QMap <int, Page*> mPages;

    QToolButton *mAdd,
                *mRemove,
                *mUp,
                *mDown,
                *mConfigure;

    QSlider *mZoom;

    QString mPagesFile;
    QString mName;
    QString mPath;

    QUndoStack *mHistory;

    //This is a temporary holder of information when loading files.
    QList<QTreeWidgetItem*> mExpandedItems;
};

#endif // MAINWINDOW_H
