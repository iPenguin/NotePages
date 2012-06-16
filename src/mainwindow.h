/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDomElement>
#include <QTreeWidgetItem>
#include <QMap>

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

protected slots:
    void closeTab(int tabNumber);

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

    void setupMenubars();

    //A mapping of page_id to object.
    QMap <int, Page*> mPages;

    QString m_wikiFile;
    QString m_name;
    QString mPath;
};

#endif // MAINWINDOW_H
