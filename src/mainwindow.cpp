#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDomDocument>
#include <QDomElement>
#include <QTreeWidgetItem>

#include "settings.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupMenubars();
    setWindowState(Qt::WindowMaximized);

    QString currentFolder = Settings::inst()->value("currentWiki").toString();
    loadFile(currentFolder);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMenubars()
{

//File
    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(open()));
    connect(ui->actionClose, SIGNAL(triggered()), SLOT(closeFile()));
    connect(ui->actionSave, SIGNAL(triggered()), SLOT(save()));

    connect(ui->actionQuit, SIGNAL(triggered()), SLOT(quit()));

//Edit
    connect(ui->actionCopy, SIGNAL(triggered()), SLOT(copy()));
    connect(ui->actionCut, SIGNAL(triggered()), SLOT(cut()));
    connect(ui->actionPaste, SIGNAL(triggered()), SLOT(paste()));


//Tree
    connect(ui->pageTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(pageSelected(QTreeWidgetItem*)));
}

void MainWindow::open()
{
    load();
}

void MainWindow::closeFile()
{

}

void MainWindow::quit()
{
    QApplication::quit();
}

void MainWindow::copy()
{

}

void MainWindow::cut()
{

}

void MainWindow::paste()
{

}

void MainWindow::load()
{
    QFileDialog* fd = new QFileDialog(this, Qt::Sheet);
      fd->setDirectory(mLastDirectory);
      fd->setObjectName("fileopendialog");
      fd->setViewMode(QFileDialog::List);
      fd->setFileMode( QFileDialog::Directory );
      fd->setAcceptMode(QFileDialog::AcceptOpen);
      fd->open(this, SLOT(loadFile(QString)));

}

void MainWindow::save()
{
    mLastDirectory = QFileDialog::getSaveFileName(this, tr("Save a Desktop Wiki"),
                                                    "", tr("Desktop Wiki (*.dwiki)"));

    if(mLastDirectory.isEmpty())
        return;

    saveFile(mLastDirectory);

}

void MainWindow::loadFile(QString folder)
{
    if(folder.isEmpty())
        return;

    mPath = folder;

    Settings::inst()->setValue("currentWiki", QVariant(folder));

    QString fileName = folder + "/index.wiki";

    QDomDocument doc("dWikiIndex");
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "failed to open file";
        return;
    }

    QString errorMsg;
    int errorLine;
    if (!doc.setContent(&file, &errorMsg, &errorLine)) {
        qWarning() << "failed to load wiki data: " << errorMsg << "line" << errorLine;

        file.close();
        return;
    }

    file.close();
    m_wikiFile = fileName;

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    while(!n.isNull()) {

        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            if(e.tagName() == "name") {
                m_name = e.text();
                //TODO: set titlebar.
            } else if (e.tagName() == "page") {

                QTreeWidgetItem *item = loadPage(e);
                ui->pageTree->addTopLevelItem(item);
                item->setExpanded(true);
                //TODO: expand all...
            }
        }
        n = n.nextSibling();
    }

}

void MainWindow::saveFile(QString fileName)
{


}

QTreeWidgetItem* MainWindow::loadPage(QDomElement element)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();

    qDebug() << element.attribute("id") << element.attribute("title") << element.attribute("icon");
    item->setData(0, Qt::UserRole, QVariant(element.attribute("id")));
    item->setData(0, Qt::DisplayRole, QVariant(element.attribute("title")));
    item->setData(0, Qt::DecorationRole, QVariant(element.attribute("icon")));

    QDomNode n = element.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement();

        if (e.tagName() == "page") {
            item->addChild(loadPage(e));
        }
        n = n.nextSibling();
    }
    return item;
}


void MainWindow::pageSelected(QTreeWidgetItem *page)
{

    int pageNumber = page->data(0, Qt::UserRole).toInt();

    QString pagePath = mPath + "/pages/" + QString::number(pageNumber);

    Page *p;

    //if the page hasn't been loaded, load it.
    if(!mPages.contains(pageNumber)) {

        p = new Page(pagePath);

        mPages.insert(pageNumber, p);
        ui->tabWidget->addTab(p, "tab");
    } else {

        p = mPages.value(pageNumber);
    }

    //display page.
    ui->tabWidget->setCurrentWidget(p);

}

