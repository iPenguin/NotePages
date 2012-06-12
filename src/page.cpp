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

#include "note.h"

Page::Page(QString pagePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Page),
    mPagePath(pagePath)
{
    ui->setupUi(this);

    mScene = new PageScene(this);
    ui->graphicsView->setScene(mScene);

    //FIXME: move this code into a seperate function.
    mUndoStack = new QUndoStack(this);

    mPagePath = pagePath;

    loadPage();
}

Page::~Page()
{
    delete ui;
}

void Page::savePage()
{

    QString xmlIndex = mPagePath + "/page.xml";
    if(!QFileInfo(xmlIndex).exists()) {
        if(!QFileInfo(mPagePath).exists()) {
            QString parentDir = QFileInfo(mPagePath).path();
            QString dir = QFileInfo(mPagePath).baseName();
            qDebug() << parentDir << dir;
            QDir d(parentDir);
            d.mkdir(dir);

        }

        qDebug() << "TODO: Create" << xmlIndex;
        //TODO: create pagePath and some basic data.
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

    stream.writeStartElement("dwiki_page");

        stream.writeStartElement("properties");
        stream.writeAttribute("bgColor", "#ff00ff");
        stream.writeAttribute("bgImage", "");
        stream.writeEndElement(); //properties

        foreach(QGraphicsItem *i, mScene->items()) {
            if(i->type() != Note::Type) {
                qDebug() << "Unknown QGraphicsItem::Type" << i->type();
                continue;
            }

            Note *n = qgraphicsitem_cast<Note*>(i);
            n->saveNote(&stream);

        }

    stream.writeEndElement(); //dwiki_page
    stream.writeEndDocument();
    file.close();

}

void Page::loadPage()
{

    QString xmlIndex = mPagePath + "/page.xml";

    if(!QFileInfo(xmlIndex).exists()) {
        qDebug() << "TODO: Create" << xmlIndex;
        //TODO: create pagePath and some basic data.
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
                Note *n = new Note();
                n->loadNote(&stream, mPagePath);
                mScene->addItem(n);

            } else if (name == "group") {
                qDebug() << "TODO: for each child element load each note.";
            }
        }
    }
}
