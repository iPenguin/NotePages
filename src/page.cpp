/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include "page.h"
#include "ui_page.h"

#include <QFileInfo>
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QDir>
#include "pagescene.h"
#include <QTextDocument>

#include "note.h"

Page::Page(QString pagePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page),
    mDeleted(false)
{
    ui->setupUi(this);

    mScene = new PageScene(this);
    mScene->setPagePath(pagePath);

    connect(mScene, SIGNAL(changePage(QString)), SLOT(nextPage(QString)));
    ui->graphicsView->setScene(mScene);

    //FIXME: move this code into a seperate function.
    mUndoStack = new QUndoStack(this);

    connect(ui->graphicsView, SIGNAL(zoomLevelChanged(int)), SIGNAL(zoomLevelChanged(int)));

    loadPage();

    ui->graphicsView->ensureVisible(QRectF(0,0,50,50));

}

Page::~Page()
{
    if(!mDeleted)
        savePage();
    delete ui;
}

void Page::savePage()
{
    QString pagePath = mScene->pagePath();
    QString xmlIndex = pagePath + "/page.xml";
    if(!QFileInfo(xmlIndex).exists()) {
        if(!QFileInfo(pagePath).exists()) {
            QString parentDir = QFileInfo(pagePath).path();
            QString dir = QFileInfo(pagePath).baseName();
            QDir d(parentDir);
            d.mkdir(dir);
        }
    }

    QFile file(xmlIndex);

    if(!file.open(QIODevice::WriteOnly)) {
        //TODO: some nice dialog to warn the user.
        qWarning() << "Couldn't open file for reading:" << xmlIndex;
        return;
    }

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("npage_page");

        stream.writeStartElement("properties");
        stream.writeAttribute("bgColor", "#ff00ff");
        stream.writeAttribute("bgImage", "");
        stream.writeEndElement(); //properties

        foreach(QGraphicsItem *i, mScene->items()) {
            if(i->type() != Note::Type) {
                //qDebug() << "Unknown QGraphicsItem::Type" << i->type();
                continue;
            }

            Note *n = qgraphicsitem_cast<Note*>(i);
            n->saveNote(&stream);
        }

    stream.writeEndElement(); //npage_page
    stream.writeEndDocument();
    file.close();

}

bool Page::isSaved()
{
    //FIXME: check if the page has been saved.
    return false;
}

void Page::loadPage()
{
    QString pagePath = mScene->pagePath();
    QString xmlIndex = pagePath + "/page.xml";

    if(!QFileInfo(xmlIndex).exists()) {
        if(!QFileInfo(pagePath).exists()) {
            QString parentDir = QFileInfo(pagePath).path();
            QString dir = QFileInfo(pagePath).baseName();
            QDir d(parentDir);
            d.mkdir(dir);
        }
    }

    QFile file(xmlIndex);

    if(!file.open(QIODevice::ReadOnly)) {
        //TODO: some nice dialog to warn the user.
        qWarning() << "Couldn't open file for reading:" << xmlIndex;
        return;
    }

    QXmlStreamReader stream(file.readAll());
    file.close();

    if(stream.hasError()) {
        qWarning() << "Error loading saved file: " << stream.errorString();
        return;
    }

    //TODO: associate specific pageNumber with a given instance of a Page, then switch to that instance.
    //      the class can then decide if it needs to open file/load data or just switch to what it currently has loaded.

    //TODO: pass loading Page content to the Page class.
    while (!stream.atEnd() && !stream.hasError()) {

        stream.readNextStartElement();
        if (stream.isStartElement()) {
            QString name = stream.name().toString();

            if (name == "properties") {
                qDebug() << "TODO: properties - create parser function to get properties.";

            } else if (name == "note") {
                Note *n = new Note(0, mScene);
                n->loadNote(&stream, mScene->pagePath());
                mScene->incrementMaxNoteId();
                updateSceneRect(n);
                connect(n, SIGNAL(pageLinkClicked(QString)), SLOT(nextPage(QString)));
            } else if (name == "group") {
                qDebug() << "TODO: for each child element load each note.";
            }
        }
    }
}

void Page::updateSceneRect(Note *n)
{
    qreal left, right, top, bottom;

    if(n->pos().x() < mScene->sceneRect().left()) {
        left = n->pos().x();
    } else {
        left = mScene->sceneRect().left();
    }

    if(n->pos().y() < mScene->sceneRect().top()) {
        top = n->pos().y();
    } else {
        top = mScene->sceneRect().top();
    }

    if(n->boundingRect().right() > mScene->sceneRect().right()) {
        right = n->boundingRect().right();
    } else {
        right = mScene->sceneRect().right();
    }

    if(n->boundingRect().bottom() > mScene->sceneRect().bottom()) {
        bottom = n->boundingRect().bottom();
    } else {
        bottom = mScene->sceneRect().bottom();
    }

    //FIXME: don't hardcode a margin for the chrome of the notes.
    mScene->setSceneRect(left - 10, top - 10, right - left, bottom - top);
}

void Page::setTextProperties(Page::TextProperty property, bool state)
{
    if(mScene->selectedItems().count() <= 0) {
        return;
    }
    QGraphicsItem *i = mScene->selectedItems().first();
    if(!i)
        return;

    if(i->type() != NoteText::Type)
        return;

    NoteText *nt = qgraphicsitem_cast<NoteText*>(i);
    if(!nt)
        return;

    switch(property) {
        case Page::TxtBold:
            nt->setBold(state);
            break;
        case Page::TxtItalic:
            nt->setItalic(state);
            break;
        case Page::TxtUnderline:
            nt->setUnderline(state);
            break;
        case Page::TxtLeftJustify:
            nt->setTextBlockAlignment(Qt::AlignLeft);
            break;
        case Page::TxtCenterJustify:
            nt->setTextBlockAlignment(Qt::AlignHCenter);
            break;
        case Page::TxtRightJustify:
            nt->setTextBlockAlignment(Qt::AlignRight);
            break;
        case Page::TxtJustify:
            nt->setTextBlockAlignment(Qt::AlignJustify);
            break;
        default:
            qDebug() << "Unknown or Unhandled Text Property" << property;
    }
}

int Page::currentZoomLevel()
{
    return ui->graphicsView->zoomPercent();
}

void Page::deletePage()
{
    //FIXME: delete files on disk and data.
    foreach(QGraphicsItem *i, mScene->items()) {
        if(i->type() == Note::Type) {
            Note *n = qgraphicsitem_cast<Note*>(i);
            n->deleteNote();
        }
    }


    QDir d(QFileInfo(mScene->pagePath()).path());
    d.remove(QFileInfo(mScene->pagePath()).fileName() + "/page.xml");
    d.rmdir(QFileInfo(mScene->pagePath()).fileName());

    deleteLater();
    mDeleted = true;
}

void Page::addLinkToNote(QStringList link)
{
    if(mScene->selectedItems().count() <= 0) {
        qWarning() << "consider creating a note and adding the link to it";
        return;
    }

    QGraphicsItem *i = mScene->selectedItems().first();
    if(i->type() != NoteText::Type) {
        qWarning() << "Not a Note: " << i->type();
        return;
    }

    NoteText *nt = qgraphicsitem_cast<NoteText*>(i);
    if(!nt) {
        qDebug() << "Couldn't find noteText!";
        return;
    }

    nt->addLink(link);

}

void Page::zoomChanged(int value)
{
    ui->graphicsView->zoomLevel(value);
}

void Page::nextPage(QString link)
{
    emit changePage(link);
}
