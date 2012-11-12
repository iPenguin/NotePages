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
#include "notetext.h"

Page::Page(QString pagePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page),
    mDeleted(false)
{
    ui->setupUi(this);

    mScene = new PageScene(this);
    mView->setScene(mScene);
    mView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    mView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
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
    //FIXME: don't hard code paths. Simplify and streamline paths.
    QString pagePath = mScene->pagePath();
    QString xmlIndex = pagePath + "/page.xml";
    QString xmlIndexTemp = xmlIndex + ".tmp";

    if(!QFileInfo(xmlIndexTemp).exists()) {
        if(!QFileInfo(pagePath).exists()) {
            QDir d("");
            d.mkpath(pagePath);
        }
    }

    QFile file(xmlIndexTemp);

    if(!file.open(QIODevice::WriteOnly)) {
        //TODO: some nice dialog to warn the user.
        qWarning() << "Couldn't open file for reading:" << xmlIndexTemp;
        return;
    }

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("npage_page");

        foreach(QGraphicsItem *i, mScene->items()) {
            if(i->type() != Note::Type) {
                continue;
            }

            Note *n = qgraphicsitem_cast<Note*>(i);
            n->saveNote(&stream);
        }

        foreach(QGraphicsItem *i, mScene->items()) {
            if(i->type() != Arrow::Type) {
                continue;
            }

            Arrow *a = qgraphicsitem_cast<Arrow*>(i);
            a->saveArrow(&stream);
        }
    stream.writeEndElement(); //npage_page
    stream.writeEndDocument();
    file.close();
    QDir d(pagePath);
    d.remove("page.xml");
    if(file.copy(xmlIndex))
        file.remove();
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

            if (name == "note") {
                Note *n = new Note(&stream, pagePath, 0, mScene);
                mScene->incrementMaxNoteId();
                updateSceneRect(n);
                connect(n, SIGNAL(pageLinkClicked(QString)), SLOT(nextPage(QString)));
            } else if (name == "arrow") {
                int start, end;
                Note *startNote = 0, *endNote = 0;
                start = stream.attributes().value("start-id").toString().toInt();
                end = stream.attributes().value("end-id").toString().toInt();

                foreach(QGraphicsItem *i, mScene->items()) {

                    if(i->type() == Note::Type) {
                        Note *n = qgraphicsitem_cast<Note*>(i);
                        if(n->id() == start)
                            startNote = n;
                        if(n->id() == end)
                            endNote = n;
                    }
                }
                new Arrow(startNote, endNote, 0, mScene);


            } else if (name == "group") {
                qDebug() << "TODO: Create groups of notes where the edges are ";
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
    if(i->type() != NoteType::Text) {
        qWarning() << "Not a Note: " << i->type();
        return;
    }

    NoteText *nt = static_cast<NoteText*>(i);
    if(!nt) {
        qWarning() << "Couldn't find noteText!";
        return;
    }

    nt->addLink(link);

}

Note *Page::currentNote()
{
    if(mScene->selectedItems().count() <= 0)
        return 0;

    QGraphicsItem *i = mScene->selectedItems().first();
    if(!i)
        return 0;

    if(i->type() == Note::Type)
        return qgraphicsitem_cast<Note*>(i);
    else if(i->type() != Arrow::Type) //FIXME: do a metter job of checking the item type.
        return qgraphicsitem_cast<Note*>(i->parentItem());

    return 0;
}

void Page::setDefaultNoteType(NoteType::Id type)
{
    mScene->setDefaultNoteType(type);
}

void Page::zoomChanged(int value)
{
    ui->graphicsView->zoomLevel(value);
}

void Page::nextPage(QString link)
{
    emit changePage(link);
}
