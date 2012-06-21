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

class QXmlStreamWriter;

#include "page.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
//File
    void open();
    void closeFile();
    void quit();
    void save();

//Edit
    void copy();
    void cut();
    void paste();

    void loadFile(QString folder);

    void pageSelected(QTreeWidgetItem* page);
//Help
    void about();

    void addNewPage();
    void removePages();

    int currentMaxPageId();
    void setCurrentMaxPageId(int newId);
    int useNextPageId() { mCurrentMaxPageId++; return mCurrentMaxPageId; }

    void changeItem(QTreeWidgetItem *item, int column);

protected slots:
    void closeTab(int tabNumber);

    //set zoom slider to the specified value.
    void updateZoomLevel(int percent);
    void zoomPage(int percent);

    void tabChanged(int newTab);

protected:
    void saveIndex(QString path);
    void saveIndexPages(QXmlStreamWriter *stream, QTreeWidgetItem *item);

private slots:
    void setTextProperties();

private:
    Ui::MainWindow *ui;

    void load();

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

    QString mWikiFile;
    QString mName;
    QString mPath;

    //This is a temporary holder of information when loading files.
    QList<QTreeWidgetItem*> mExpandedItems;
};

#endif // MAINWINDOW_H
