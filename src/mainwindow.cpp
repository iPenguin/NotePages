/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDomDocument>
#include <QDomElement>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include "appinfo.h"

#include <QXmlStreamWriter>
#include <QTimer>

#include "settings.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mCurrentMaxPageId(0)
{
    ui->setupUi(this);

    setupStatusBar();
    setupMenubars();
    setWindowState(Qt::WindowMaximized);

    setUnifiedTitleAndToolBarOnMac(true);

    ui->pageTree->setStyleSheet("QTreeWidget { background: #D6DDE5 }");
    ui->pageTree->setAttribute(Qt::WA_MacShowFocusRect, 0);
    QString currentFolder = Settings::inst()->value("currentWiki").toString();
    loadFile(currentFolder);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupStatusBar()
{
    mAdd = new QToolButton(this);
    mAdd->setIcon(QIcon(":/images/plus.svg"));
    mAdd->setStyleSheet( "background-color: rgba( 255, 255, 255, 0% );" );
    ui->statusBar->addWidget(mAdd);

    mRemove = new QToolButton(this);
    mRemove->setIcon(QIcon(":/images/minus.svg"));
    mRemove->setStyleSheet( "background-color: rgba( 255, 255, 255, 0% );" );
    ui->statusBar->addWidget(mRemove);

    mUp = new QToolButton(this);
    mUp->setIcon(QIcon(":/images/arrow_up.svg"));
    mUp->setStyleSheet( "background-color: rgba( 255, 255, 255, 0% );" );
    ui->statusBar->addWidget(mUp);


    mDown = new QToolButton(this);
    mDown->setIcon(QIcon(":/images/arrow_down.svg"));
    mDown->setStyleSheet( "background-color: rgba( 255, 255, 255, 0% );" );
    ui->statusBar->addWidget(mDown);

    mZoom = new QSlider(this);
    mZoom->setOrientation(Qt::Horizontal);
    mZoom->setMinimum(1);
    mZoom->setMaximum(200);
    mZoom->setValue(100);
    mZoom->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
    ui->statusBar->addPermanentWidget(mZoom);
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

//Toolbar
    QActionGroup *g = new QActionGroup(this);
    g->addAction(ui->actionLeftJustify);
    g->addAction(ui->actionCenterJustify);
    g->addAction(ui->actionRightJustify);
    g->addAction(ui->actionJustify);
    g->setExclusive(true);

    connect(ui->actionBold, SIGNAL(triggered()), SLOT(setTextProperties()));
    connect(ui->actionItalic, SIGNAL(triggered()), SLOT(setTextProperties()));
    connect(ui->actionUnderline, SIGNAL(triggered()), SLOT(setTextProperties()));

    connect(ui->actionLeftJustify, SIGNAL(triggered()), SLOT(setTextProperties()));
    connect(ui->actionCenterJustify, SIGNAL(triggered()), SLOT(setTextProperties()));
    connect(ui->actionRightJustify, SIGNAL(triggered()), SLOT(setTextProperties()));
    connect(ui->actionJustify, SIGNAL(triggered()), SLOT(setTextProperties()));

//Help
    connect(ui->actionAbout_DesktopWiki, SIGNAL(triggered()), SLOT(about()));

//Tree
    connect(ui->pageTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(pageSelected(QTreeWidgetItem*)));
    connect(mAdd, SIGNAL(clicked()), SLOT(addNewPage()));
    connect(mRemove, SIGNAL(clicked()), SLOT(removePages()));
    connect(ui->pageTree, SIGNAL(itemChanged(QTreeWidgetItem*,int)), SLOT(changeItem(QTreeWidgetItem*,int)));

//TabWidget
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), SLOT(closeTab(int)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(tabChanged(int)));

//TODO: on tab change update the zoom slider.
    connect(mZoom, SIGNAL(valueChanged(int)), SLOT(zoomPage(int)));
}

void MainWindow::updateZoomLevel(int percent)
{
    int value = 100;
    if(percent <= mZoom->maximum() && percent >= mZoom->minimum())
        value = percent;
    else if(percent > mZoom->maximum())
        value = mZoom->maximum();
    else if (percent < mZoom->minimum())
        value = mZoom->minimum();

    mZoom->blockSignals(true);
    mZoom->setValue(value);
    mZoom->blockSignals(false);
}

void MainWindow::zoomPage(int percent)
{
    QWidget *w = ui->tabWidget->currentWidget();
    if(!w)
        return;
    Page *p = qobject_cast<Page*>(w);
    p->zoomChanged(percent);
}

void MainWindow::tabChanged(int newTab)
{
    QWidget *w = ui->tabWidget->widget(newTab);
    if(!w)
        return;

    Page *p = qobject_cast<Page*>(w);

    int zoomLevel = p->currentZoomLevel();

    mZoom->setValue(zoomLevel);
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
      fd->setDirectory(mPath);
      fd->setObjectName("fileopendialog");
      fd->setViewMode(QFileDialog::List);
      fd->setFileMode( QFileDialog::Directory );
      fd->setAcceptMode(QFileDialog::AcceptOpen);
      fd->open(this, SLOT(loadFile(QString)));

}

void MainWindow::save()
{
    //FIXME: use a QFileDialog as load() above.
    if(mPath.isEmpty()) {
        mPath = QFileDialog::getSaveFileName(this, tr("Save a Desktop Wiki"),
                                                    "", tr("Desktop Wiki (*.dwiki)"));

        if(mPath.isEmpty())
            return;
    }

    saveIndex(mPath);
    saveFile(mPath);

}

void MainWindow::loadFile(QString folder)
{
    if(folder.isEmpty())
        return;

    mPath = folder;

    setWindowFilePath(folder);

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
    mWikiFile = fileName;

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    while(!n.isNull()) {

        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            if(e.tagName() == "name") {
                mName = e.text();
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

    foreach(QTreeWidgetItem *i, mExpandedItems)
        i->setExpanded(true);
}

void MainWindow::saveFile(QString fileName)
{

    QWidget *w = ui->tabWidget->currentWidget();

    if(!w)
        return;

    Page *p = qobject_cast<Page*>(w);

    if(!p)
        return;

    p->savePage();


}

QTreeWidgetItem* MainWindow::loadPage(QDomElement element)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    QString id = element.attribute("id");

    if(id.toInt() > currentMaxPageId())
        setCurrentMaxPageId(id.toInt() + 1);

    item->setData(0, Qt::UserRole, QVariant(id));
    item->setData(0, Qt::DisplayRole, QVariant(element.attribute("title")));
    item->setIcon(0, QIcon(element.attribute("icon")));
    item->setData(0, Qt::UserRole + 1, QVariant(element.attribute("icon")));
    bool expanded = bool(element.attribute("expanded").toInt());

    QDomNode n = element.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement();

        if (e.tagName() == "page") {
            item->addChild(loadPage(e));
        }
        n = n.nextSibling();
    }

    if(expanded)
        mExpandedItems.append(item);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    return item;
}

void MainWindow::pageSelected(QTreeWidgetItem *page)
{

    int pageNumber = page->data(0, Qt::UserRole).toInt();

    QString pagePath = mPath + "/pages/" + QString::number(pageNumber);

    Page *p = 0;

    //if the page hasn't been loaded, load it.
    if(!mPages.contains(pageNumber)) {

        p = new Page(pagePath);

        //connect(p, SIGNAL(zoomLevelChanged(int)), SLOT(updateZoomLevel(int)));
        mPages.insert(pageNumber, p);
        ui->tabWidget->addTab(p, page->icon(0), page->data(0, Qt::DisplayRole).toString());
    } else {
        p = mPages.value(pageNumber);
        if(ui->tabWidget->indexOf(p) < 0) {
            ui->tabWidget->addTab(p, page->icon(0), page->data(0, Qt::DisplayRole).toString());
        }
    }

    //display page.
    ui->tabWidget->setCurrentWidget(p);
}


void MainWindow::about()
{
    QString aboutInfo = QString(tr("<h1>%1</h1>"
                                       "<p>Version: %2 (built on %3)</p>"
                                       "<p>Copyright (c) 2012 Brian Milco</p>"
                                       "<p>This software is a note taking application with the"
                                        "ability to do cross-references like an online wiki.</p>")
                                    .arg(qApp->applicationName())
                                    .arg(qApp->applicationVersion())
                                    .arg(AppInfo::inst()->appBuildInfo)
                                    );

    QMessageBox::about(this, tr("About desktopWiki"), aboutInfo);

}

void MainWindow::addNewPage()
{
    QTreeWidgetItem *item = 0;

    if(ui->pageTree->selectedItems().count() <= 0)
        item = ui->pageTree->topLevelItem(0);
    else
        item = ui->pageTree->selectedItems().first();

    QTreeWidgetItem *ni = new QTreeWidgetItem();

    ni->setData(0, Qt::UserRole, QVariant(useNextPageId()));
    ni->setData(0, Qt::DisplayRole, QVariant("New item"));
    ni->setIcon(0, QIcon(":/images/filenew.png"));
    ni->setData(0, Qt::UserRole + 1, QVariant(":/images/filenew.png"));

    ni->setFlags(ni->flags() | Qt::ItemIsEditable);
    item->addChild(ni);
    item->setExpanded(true);
}

void MainWindow::removePages()
{

    if(ui->pageTree->selectedItems().count() <= 0)
        return;

    foreach(QTreeWidgetItem *i, ui->pageTree->selectedItems()) {
        //TODO: remove pages from the tabWidget.
        //TODO: remove items from the pageTree.
    }
}

int MainWindow::currentMaxPageId()
{
    return mCurrentMaxPageId;
}

void MainWindow::setCurrentMaxPageId(int newId)
{
    mCurrentMaxPageId = newId;
}

void MainWindow::changeItem(QTreeWidgetItem *item, int column)
{
    qDebug() << "changeItem";
    int pageNumber = item->data(column, Qt::UserRole).toInt();
    Page *p = mPages.value(pageNumber);
    if(!p)
        return;

    int tabIndex = ui->tabWidget->indexOf(p);
    QString tabText = ui->tabWidget->tabText(tabIndex);
    if(tabText != item->text(0))
        ui->tabWidget->setTabText(tabIndex, item->text(0));

    //TODO: find an easy way to only change the icon if it actually changes.
    ui->tabWidget->setTabIcon(tabIndex, item->icon(0));

}

void MainWindow::closeTab(int tabNumber)
{
    QWidget *w = ui->tabWidget->widget(tabNumber);
    ui->tabWidget->removeTab(tabNumber);
    //TODO: test if the tab has been completely saved before even thinking of closing it.
    if(!w)
        return;

    Page *p = qobject_cast<Page*>(w);
    if(!p->isSaved()) {
        qDebug() << "prompt to save file";
        p->savePage();
    }

    //TODO: pass tab closure onto the undo stack.
    //undoStack->push(new CCloseTab());

    //NOTE: if I decide to delete the Page then you have to remove the page from the mPages QMap.
    //delete w;
    //w = 0;

}

void MainWindow::saveIndex(QString path)
{
    qDebug() << "save index" << path;
    if(!QFileInfo(path).exists()) {
        QDir d(path);
        d.mkpath(path);
    }

    QString xmlIndex = path + "/" + "index.wiki";
    QFile file(xmlIndex);

    if(!file.open(QIODevice::WriteOnly)) {
        //TODO: some nice dialog to warn the user.
        qWarning() << "Couldn't open file for reading:" << xmlIndex;
        return;
    }

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("dwiki_index");

    stream.writeTextElement("name", mName);

    saveIndexPages(&stream, ui->pageTree->topLevelItem(0));

    stream.writeEndElement(); //dwiki_index
    stream.writeEndDocument();
}

void MainWindow::saveIndexPages(QXmlStreamWriter *stream, QTreeWidgetItem *item)
{
    stream->writeStartElement("page");
    stream->writeAttribute("id", item->data(0,Qt::UserRole).toString());
    stream->writeAttribute("title", item->data(0, Qt::DisplayRole).toString());
    stream->writeAttribute("icon", item->data(0, Qt::UserRole + 1).toString());
    stream->writeAttribute("expanded", QString::number(item->isExpanded()));

    for(int i = 0; i < item->childCount(); i++) {
        QTreeWidgetItem *child = item->child(i);
        saveIndexPages(stream, child);
    }

    stream->writeEndElement(); //page
}

void MainWindow::setTextProperties()
{
    Page::TextProperty property;
    QAction *a = qobject_cast<QAction*>(sender());

    if(a == ui->actionBold) {
        property = Page::TxtBold;
    } else if (a == ui->actionItalic) {
        property = Page::TxtItalic;
    } else if (a == ui->actionUnderline) {
        property = Page::TxtUnderline;
    } else if (a == ui->actionLeftJustify) {
        property = Page::TxtLeftJustify;
    } else if (a == ui->actionCenterJustify) {
        property = Page::TxtCenterJustify;
    } else if (a == ui->actionRightJustify) {
        property = Page::TxtRightJustify;
    } else if (a == ui->actionJustify) {
        property = Page::TxtJustify;
    }

    QWidget *w = ui->tabWidget->currentWidget();

    if(!w)
        return;

    Page *p = qobject_cast<Page*>(w);
    if(!p)
        return;
    p->setTextProperties(property, a->isChecked());
}
