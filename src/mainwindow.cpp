/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "linkdialog.h"

#include <QFileDialog>
#include <QDomDocument>
#include <QDomElement>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include "appinfo.h"

#include <QFile>
#include <QPropertyAnimation>

#include <QXmlStreamWriter>
#include <QTimer>

#include <QListWidgetItem>

#include "settings.h"
#include "history.h"
#include "settingsui.h"
#include "note.h"

#include <QDebug>
//#include "debug.h"

MainWindow::MainWindow(bool autoLoad, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mLinkDialog(0),
    mSettingsUi(0),
    mCurrentMaxPageId(0),
    mHistory(new QUndoStack)
{
    ui->setupUi(this);
    ui->iconList->hide();

    setupMenubars();
    setWindowState(Qt::WindowMaximized);

    setUnifiedTitleAndToolBarOnMac(true);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->pageTree->setStyleSheet("QTreeWidget { background: #D6DDE5 }");
    ui->pageTree->setAttribute(Qt::WA_MacShowFocusRect, 0);
    QPalette palette = ui->pageTree->palette();
    QColor macSidebarColor(231, 237, 246);
    QColor macSidebarHighlightColor(168, 183, 205);
    palette.setColor(QPalette::Base, macSidebarColor);
    palette.setColor(QPalette::Highlight, macSidebarHighlightColor);
    ui->pageTree->setPalette(palette);
    ui->pageTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    if(autoLoad) {
        QString currentFolder = Settings::inst()->value("currentNotePages").toString();
        loadFile(currentFolder);
    }

    setupStatusBar();

#ifdef Q_OS_MACX
    QApplication::setQuitOnLastWindowClosed(false);
#endif
    //QSettings settings;

    restoreGeometry(Settings::inst()->value("Geometry").toByteArray());
}

MainWindow::~MainWindow()
{
    saveIndex(mPath);
    delete ui;
    //Settings::inst()->setValue("Geometry", saveGeometry());
    QSettings settings;
    settings.setValue("Geometry", saveGeometry());
}

void MainWindow::loadPageFromLink(QString link)
{

    if(link.startsWith("npage://"))
        link = link.remove("npage://");

    QString page, note;
    if(link.contains("#")) {
        int idx = link.indexOf("#");
        note = link.right(link.length() - (idx + 1));
        page = link.left(idx);
    } else {
        page = link;
    }

    int currentPage = ui->pageTree->currentItem()->data(0, Qt::UserRole).toInt();

    foreach(QTreeWidgetItem *i ,ui->pageTree->selectedItems()) {
        if(i) {
            i->setSelected(false);
            ui->pageTree->setCurrentItem(i);
        }
    }

    mHistory->push(new History(this, currentPage, page.toInt()));
}

void MainWindow::setupStatusBar()
{
    mAdd = new QToolButton(this);
    mAdd->setIcon(QIcon(":/images/list_add.svgz"));
    mAdd->setStyleSheet( "background-color: rgba( 255, 255, 255, 0% );" );
    ui->statusBar->addWidget(mAdd);
    connect(mAdd, SIGNAL(clicked()), SLOT(addNewPageTreeItem()));

    mRemove = new QToolButton(this);
    mRemove->setIcon(QIcon(":/images/list_remove.svgz"));
    mRemove->setStyleSheet( "background-color: rgba( 255, 255, 255, 0% );" );
    ui->statusBar->addWidget(mRemove);
    connect(mRemove, SIGNAL(clicked()), SLOT(removePages()));

    mUp = new QToolButton(this);
    mUp->setIcon(QIcon(":/images/up.svgz"));
    mUp->setStyleSheet( "background-color: rgba( 255, 255, 255, 0% );" );
    ui->statusBar->addWidget(mUp);
    connect(mUp, SIGNAL(clicked()), SLOT(moveItemUp()));

    mDown = new QToolButton(this);
    mDown->setIcon(QIcon(":/images/down.svgz"));
    mDown->setStyleSheet( "background-color: rgba( 255, 255, 255, 0% );" );
    ui->statusBar->addWidget(mDown);
    connect(mDown, SIGNAL(clicked()), SLOT(moveItemDown()));

    mConfigure = new QToolButton(this);
    mConfigure->setIcon(QIcon(":/images/configure.svgz"));
    mConfigure->setStyleSheet( "background-color: rgba( 255, 255, 255, 0% );" );
    ui->statusBar->addWidget(mConfigure);
    connect(mConfigure, SIGNAL(clicked()), SLOT(configureItem()));

    mZoom = new QSlider(this);
    mZoom->setOrientation(Qt::Horizontal);
    mZoom->setMinimum(1);
    mZoom->setMaximum(200);
    mZoom->setValue(100);
    mZoom->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
    ui->statusBar->addPermanentWidget(mZoom);

    //TODO: on tab change update the zoom slider.
    connect(mZoom, SIGNAL(valueChanged(int)), SLOT(zoomPage(int)));

    populateIconList();

}

void MainWindow::setupMenubars()
{

//File
    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(fileOpen()));
    connect(ui->actionClose, SIGNAL(triggered()), SLOT(fileClose()));
    connect(ui->actionSave, SIGNAL(triggered()), SLOT(fileSave()));

    connect(ui->actionQuit, SIGNAL(triggered()), SLOT(fileQuit()));
    connect(ui->actionNew, SIGNAL(triggered()), SLOT(fileNew()));

//Edit
    connect(ui->actionCopy, SIGNAL(triggered()), SLOT(editCopy()));
    connect(ui->actionCut, SIGNAL(triggered()), SLOT(editCut()));
    connect(ui->actionPaste, SIGNAL(triggered()), SLOT(editPaste()));

//View
    connect(ui->actionFullScreen, SIGNAL(triggered()), SLOT(viewFullScreen()));

//Tools
    connect(ui->actionConnect, SIGNAL(triggered()), SLOT(toolsConnect()));

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

    connect(ui->actionAddLink, SIGNAL(triggered()), SLOT(addLink()));


    // make the dropdown toolbutton and the associated menu
    QToolButton *noteTypeBttn = new QToolButton(this);
    QMenu *noteTypeMenu = new QMenu( noteTypeBttn );
    noteTypeBttn->setMenu( noteTypeMenu );
    noteTypeBttn->setPopupMode( QToolButton::InstantPopup );

    QActionGroup *group = new QActionGroup(this);

    // now make the actions
    QAction *a = mAddTextNote = new QAction(tr("Add Text Note"), noteTypeBttn);
    a->setIcon(QIcon(":/images/add_text.svgz"));
    a->setCheckable(true);
    a->setChecked(true);
    group->addAction(a);
    noteTypeMenu->addAction(a);
    noteTypeBttn->setDefaultAction(a);
    connect(a, SIGNAL(triggered()), SLOT(addNoteType()));

    a = mAddImageNote = new QAction(tr("Add Image Note"), noteTypeBttn);
    a->setIcon(QIcon(":/images/add_image.svgz"));
    a->setCheckable(true);
    group->addAction(a);
    noteTypeMenu->addAction(a);
    connect(a, SIGNAL(triggered()), SLOT(addNoteType()));

    a = mAddDocumentNote = new QAction(tr("Add Document Note"), noteTypeBttn);
    a->setIcon(QIcon(":/images/add_document.svgz"));
    a->setCheckable(true);
    group->addAction(a);
    noteTypeMenu->addAction(a);
    connect(a, SIGNAL(triggered()), SLOT(addNoteType()));

    connect(group, SIGNAL(selected(QAction*)), noteTypeBttn, SLOT(setDefaultAction(QAction*)));
    ui->mainToolBar->addWidget(noteTypeBttn);
    noteTypeBttn->setToolButtonStyle(Qt::ToolButtonIconOnly);

//Tools
    connect(ui->actionOptions, SIGNAL(triggered()), SLOT(toolsSettings()));

//Help
    connect(ui->actionAboutNotePages, SIGNAL(triggered()), SLOT(helpAbout()));

//Tree
    connect(ui->pageTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), SLOT(pageSelected(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(ui->pageTree, SIGNAL(itemChanged(QTreeWidgetItem*,int)), SLOT(changeItem(QTreeWidgetItem*,int)));

//TabWidget
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), SLOT(closeTab(int)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(tabChanged(int)));

//History
    QAction *redo = mHistory->createRedoAction(this);
    QAction *undo = mHistory->createUndoAction(this);
    redo->setIcon(QIcon(":/images/forward.svgz"));
    undo->setIcon(QIcon(":/images/back.svgz"));
    ui->mainToolBar->insertAction(ui->actionCopy, undo);
    ui->mainToolBar->insertAction(ui->actionCopy, redo);
    ui->mainToolBar->insertSeparator(ui->actionCopy);

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
    Page *p = currentPage();
    if(!p)
        return;

    p->zoomChanged(percent);
}

void MainWindow::tabChanged(int newTab)
{
    QWidget *w = ui->tabWidget->widget(newTab);
    if(!w)
        return;

    Page *p = qobject_cast<Page*>(w);
    if(!p)
        return;

    int zoomLevel = p->currentZoomLevel();

    mZoom->setValue(zoomLevel);

    int pageNumber = mPages.key(p);
    selectPage(pageNumber);
}

void MainWindow::fileOpen()
{

    //FIXME: check if the file is already open and switch if it is.
    load();
}

void MainWindow::fileClose()
{
    deleteLater();
}

void MainWindow::fileQuit()
{

    QApplication::quit();
}

void MainWindow::editCopy()
{

}

void MainWindow::editCut()
{

}

void MainWindow::editPaste()
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

QTreeWidgetItem *MainWindow::findPage(int pageNumber)
{
    QTreeWidgetItemIterator it(ui->pageTree);
     while (*it) {
         if ((*it)->data(0, Qt::UserRole).toInt() == pageNumber)
             return *it;
         ++it;
     }

     return 0;
}

void MainWindow::fileSave()
{
    //FIXME: use a QFileDialog as load() above.
    if(mPath.isEmpty()) {
        mPath = QFileDialog::getSaveFileName(this, tr("Save Note Pages"),
                                                    "", tr("Note Pages (*.npages)"));

        if(mPath.isEmpty())
            return;
    }
  
    
    setWindowFilePath(mPath);
    saveIndex(mPath);
    
    saveFile(mPath);

}

void MainWindow::fileNew()
{
    MainWindow *mainWin = new MainWindow(false);
    mainWin->rect().setX(mainWin->rect().x() + 10);
    mainWin->rect().setY(mainWin->rect().y() + 10);
    mainWin->show();
}

void MainWindow::loadFile(QString folder)
{
    if(ui->pageTree->topLevelItemCount() > 0) {
        MainWindow *mw = new MainWindow(false);
        mw->loadFile(folder);
        mw->show();
        return;
    }

    if(folder.isEmpty())
        return;

    mPath = folder;

    setWindowFilePath(folder);

    Settings::inst()->setValue("currentNotePages", QVariant(folder));

    QString fileName = folder + "/pages.index";

    QDomDocument doc("nPagesIndex");
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "failed to open file";
        return;
    }

    QString errorMsg;
    int errorLine;
    if (!doc.setContent(&file, &errorMsg, &errorLine)) {
        qWarning() << "failed to load pages data: " << errorMsg << "line" << errorLine;

        file.close();
        return;
    }

    file.close();
    mPagesFile = fileName;

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

QString MainWindow::loadNewFolder() 
{
  QString folder = "";

  folder = QFileDialog::getExistingDirectory(this, tr("Save to folder"));
  
  return folder;
}

void MainWindow::saveFile(QString fileName)
{

    Page *p = currentPage();
    if(!p)
        return;
    qDebug() << "saveFile" << fileName << p->id();
    p->setPagePath(fileName);
    p->savePage();
}

QTreeWidgetItem* MainWindow::loadPage(QDomElement element)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    QString id = element.attribute("id");

    if(id.toInt() > currentMaxPageId())
        setCurrentMaxPageId(id.toInt() + 1);

    QString iconPath = element.attribute("icon");
    if(!iconPath.startsWith(":/"))
        iconPath.prepend(mPath + "/icons/");

    item->setData(0, Qt::UserRole, QVariant(id));
    item->setData(0, Qt::DisplayRole, QVariant(element.attribute("title")));
    item->setIcon(0, QIcon(iconPath));
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

void MainWindow::pageSelected(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{

    int curPgNumber = current->data(0, Qt::UserRole).toInt();
    int prevPgNumber = -1;
    if(previous) {
        prevPgNumber = previous->data(0, Qt::UserRole).toInt();
        previous->setSelected(false);
    }

    mHistory->push(new History(this, prevPgNumber, curPgNumber));

}

void MainWindow::toolsSettings()
{
    if(!mSettingsUi) {
        mSettingsUi = new SettingsUi(this);
    }

    mSettingsUi->open();

}

void MainWindow::viewFullScreen()
{
    if(isFullScreen())
        showNormal();
    else
        showFullScreen();
}

void MainWindow::toolsConnect()
{
    bool state = false;
    if(ui->actionConnect->isChecked()) {
        state = true;
    }

    Page *p = currentPage();
    if(!p)
        return;

    p->setDrawLines(state);
}

void MainWindow::selectPage(int pageNumber)
{

    QString pagePath = mPath + "/pages/" + QString::number(pageNumber);

    Page *p = 0;

    QTreeWidgetItem *page = findPage(pageNumber);

    if(!page) {
        return;
    }

    //update the user selection
    ui->pageTree->currentItem()->setSelected(false);
    page->setSelected(true);
    ui->pageTree->setCurrentItem(page);

    //if the page hasn't been loaded, load it.
    if(!mPages.contains(pageNumber)) {

        p = new Page(pagePath);
        p->setId(pageNumber);
        connect(p, SIGNAL(changePage(QString)), SLOT(loadPageFromLink(QString)));
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

    //FIXME: I'd prefer a matching with the full file path and name.
    QList<QListWidgetItem *> items = ui->iconList->findItems(QFileInfo(page->data(0, Qt::UserRole + 1).toString()).baseName(), Qt::MatchExactly);

    if(items.count() >= 1) {
        ui->iconList->blockSignals(true);
        ui->iconList->setCurrentItem(items.first());
        ui->iconList->blockSignals(false);
    }
}

void MainWindow::helpAbout()
{
    AppInfo::inst()->helpAbout();

}

void MainWindow::addNewPageTreeItem()
{
    QTreeWidgetItem *item = 0;

    if(ui->pageTree->selectedItems().count() <= 0) {
        item = ui->pageTree->topLevelItem(0);
    } else {
        item = ui->pageTree->selectedItems().first();
    }

    QTreeWidgetItem *ni = new QTreeWidgetItem();

    QString newIcon;
    if(ui->pageTree->currentItem())
        newIcon = ui->pageTree->currentItem()->data(0, Qt::UserRole + 1).toString();
    else
        newIcon = ":/images/filenew.svgz";

    ni->setData(0, Qt::UserRole, QVariant(useNextPageId()));
    ni->setData(0, Qt::DisplayRole, QVariant("New page"));
    ni->setIcon(0, QIcon(newIcon));
    ni->setData(0, Qt::UserRole + 1, QVariant(newIcon));

    ni->setFlags(ni->flags() | Qt::ItemIsEditable);

    if(item) {
        item->addChild(ni);
        item->setExpanded(true);
    } else {
        ui->pageTree->addTopLevelItem(ni);
    }
}

void MainWindow::removePages()
{

    if(ui->pageTree->selectedItems().count() <= 0)
        return;

    //TODO: prompt for confirmation.
    QTreeWidgetItem *i = ui->pageTree->currentItem();
    if(!i)
        return;
    int pageNumber = i->data(0,Qt::UserRole).toInt();

    Page *p = mPages.value(pageNumber);
    p->deletePage();
    mPages.remove(pageNumber);

    delete i;

    //TODO: foreach(QTreeWidgetItem *i, ui->pageTree->selectedItems())
        //TODO: remove pages from the tabWidget.
        //TODO: remove items from the pageTree.

}

void MainWindow::moveItemUp()
{
    qDebug() << "moveItemUp";
}

void MainWindow::moveItemDown()
{
    qDebug() << "moveItemDown";
}

void MainWindow::configureItem()
{
    ui->iconList->setVisible(!ui->iconList->isVisible());
/*TODO: make this slide in like a real Mac app.
    QPoint start, end;

    QRect hidden = ui->iconList->rect();
    hidden.setHeight(1);
    QRect visible = ui->iconList->rect();
    visible.setHeight(100);

    if(ui->iconList->isVisible()) {
        start = ui->iconList->pos();
        end = QPoint(0,ui->centralWidget->height() - ui->iconList->height());
    } else {
        start = QPoint(0,ui->centralWidget->height() + ui->iconList->height());
        end = QPoint(0,ui->centralWidget->height() - ui->iconList->height());
    }
    ui->iconList->setVisible(true);

    QPropertyAnimation *animation = new QPropertyAnimation(ui->iconList, "pos");
    animation->setDuration(150);
    animation->setStartValue(start);
    animation->setEndValue(end);
    animation->start();
*/
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

    if(!QFileInfo(path).exists()) {
        QDir d(path);
        d.mkpath(path);
        d.mkdir("icons");
    }

    QString xmlIndex = path + "/" + "pages.index";
    QFile file(xmlIndex);

    if(!file.open(QIODevice::WriteOnly)) {
        //TODO: some nice dialog to warn the user.
        qWarning() << "Couldn't open file for reading:" << xmlIndex;
        return;
    }

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("npage_index");

    stream.writeTextElement("name", mName);

    saveIndexPages(&stream, ui->pageTree->topLevelItem(0));

    stream.writeEndElement(); //npage_index
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

void MainWindow::populateIconList()
{
    //FIXME: reload the icons with each loaded file.

    //FIXME: don't hard code the first path!
    QDir d(":/icons/");

    QStringList el = d.entryList(QDir::Files);

   foreach(QString file, el) {
       file.prepend(":/icons/");
       QIcon icon = QIcon(file);
       QString name = QFileInfo(file).baseName();
       QListWidgetItem* item = new QListWidgetItem(icon, name, ui->iconList);
       item->setData(Qt::UserRole, QVariant(file));
       item->setToolTip(name);
       ui->iconList->addItem(item);
   }

   d.setPath(mPath+"/icons/");
   el = d.entryList(QDir::Files);

   foreach(QString file, el) {
       file.prepend(mPath+"/icons/");

       QIcon icon = QIcon(file);
       QString name = QFileInfo(file).baseName();
       QListWidgetItem* item = new QListWidgetItem(icon, name, ui->iconList);
       item->setData(Qt::UserRole, QVariant(file));
       item->setToolTip(name);
       ui->iconList->addItem(item);
   }

   connect(ui->iconList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), SLOT(updateItemIcon(QListWidgetItem*,QListWidgetItem*)));

}

Page *MainWindow::currentPage()
{
    QWidget *w = ui->tabWidget->currentWidget();
    if(!w)
        return 0;

    return qobject_cast<Page*>(w);
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

    Page *p = currentPage();
    if(!p)
        return;

    p->setTextProperties(property, a->isChecked());
}

void MainWindow::updateItemIcon(QListWidgetItem *newItem, QListWidgetItem *oldItem)
{
    Q_UNUSED(oldItem);

    QTreeWidgetItem *pageItem = ui->pageTree->currentItem();
    pageItem->setIcon(0, newItem->icon());
    pageItem->setData(0, Qt::UserRole + 1, newItem->data(Qt::UserRole));

}

void MainWindow::addLink()
{
    if(!mLinkDialog) {
        mLinkDialog = new LinkDialog(ui->pageTree, this);
        connect(mLinkDialog, SIGNAL(linkCreated(QStringList)), SLOT(addLinkToNote(QStringList)));
    }

    Page *p = currentPage();
    if(!p)
        return;

    Note *n = p->currentNote();
    if(!n)
        return;

    QString text = n->textSelection();
    mLinkDialog->open();
    mLinkDialog->setLinkText(text);
}

void MainWindow::addLinkToNote(QStringList link)
{
    Page *p = currentPage();

    if(!p)
        return;

    p->addLinkToNote(link);
}

void MainWindow::addNoteType()
{
    QAction *a = qobject_cast<QAction*>(sender());

    NoteType::Id type;

    if(a == mAddTextNote)
        type = NoteType::Text;
    else if (a == mAddImageNote)
        type = NoteType::Image;
    else if (a == mAddDocumentNote)
        type = NoteType::Document;

    Page *p = currentPage();
    if(!p)
        return;

    p->setDefaultNoteType(type);
}
