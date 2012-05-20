#include "page.h"

#include <QLayout>

#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include <QtSvg/QGraphicsSvgItem>

#include "note.h"

Page::Page(QString pagePath, QWidget *parent) :
    QWidget(parent)
{
    setLayout(new QVBoxLayout);
    mView = new QGraphicsView(this);
    layout()->addWidget(mView);
    layout()->setMargin(0);
    layout()->setContentsMargins(0,0,0,0);
    mScene = new PageScene(this);
    mView->setScene(mScene);

    mPagePath = pagePath;
    QString xmlIndex = pagePath + "/page.xml";

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
            qDebug() << "stream" << name;
            if(name == "title") {
                //m_name = e.text();
                //TODO: set titlebar: dWikiName - PageName
            } else if (name == "properties") {
                qDebug() << "TODO: properties - create parser function to get properties.";

            } else if (name == "note") {
                createNote(&stream);

            } else if (name == "group") {
                qDebug() << "TODO: for each child element load each note.";
            }
        }
    }

    QGraphicsSvgItem* i = new QGraphicsSvgItem("/Users/brian/projects/desktopWiki/test.svg");
    mScene->addItem(i);
    i->setFlag(QGraphicsItem::ItemIsMovable);
    i->setFlag(QGraphicsItem::ItemIsSelectable);
    i->setPos(0, 0);


}

void Page::createNote(QXmlStreamReader* stream)
{

    Note *n = new Note();
    //set all the note properties.
    n->setPos(stream->attributes().value("x").toString().toInt(), stream->attributes().value("y").toString().toInt());
    n->setZValue(stream->attributes().value("z").toString().toInt());

    QDateTime lastMod = QDateTime::fromString(stream->attributes().value("lastModified").toString(), "");
    QDateTime added = QDateTime::fromString(stream->attributes().value("lastModified").toString(), "");
    n->setLastModified(lastMod);
    n->setAddedDate(added);


    while(!(stream->isEndElement() && stream->name() == "note")) {

        stream->readNextStartElement();
        QString tag = stream->name().toString();

        if(tag == "flags") {
            int flagBinary = stream->attributes().value("value").toString().toInt(0, 2);
            qDebug() << "TODO: load flags" << flagBinary;
            stream->skipCurrentElement();

        } else if (tag == "icon") {
            QString iconName = stream->attributes().value("file").toString();
            qDebug() << "TODO: load icon" << iconName;
            stream->skipCurrentElement();

        } else if (tag == "connect") {
            int connectedNote = stream->attributes().value("id").toString().toInt();
            //TODO:  make this a qlist that is used to id connections to draw in the painting routines. ?
            qDebug() << "TODO: connect this note to note" << connectedNote;
            stream->skipCurrentElement();

        } else if (tag == "image") {
            QString imageFile = stream->attributes().value("file").toString();
            qDebug() << "TODO: load image content" << imageFile;
            stream->skipCurrentElement();

        } else if (tag == "document") {
            QString doc = stream->attributes().value("file").toString();
            qDebug() << "TODO: load file preview / create 'Files attached' for" << doc;
            stream->skipCurrentElement();

        } else if (tag == "text") {
            QString textFile = stream->attributes().value("file").toString();
            qDebug() << "TODO: load text from html file " << mPagePath + "/" + textFile;
            QFile f(mPagePath + "/" + textFile);

            if(!f.open(QIODevice::ReadOnly)) {
                qDebug() << "Error opening note - " << mPagePath + "/" + textFile;

            } else {
                QString text = f.readAll();
                //n->setHtml(text);
            }
            stream->skipCurrentElement();


        } else {
            qDebug() << "Unknown note element, skipping" << stream->name().toString();
        }

    }

    mScene->addItem(n);
    n->ensureVisible();

}
