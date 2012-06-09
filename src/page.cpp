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


    mUndoStack = new QUndoStack(this);


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

            if (name == "properties") {
                qDebug() << "TODO: properties - create parser function to get properties.";

            } else if (name == "note") {
                createNote(&stream);

            } else if (name == "group") {
                qDebug() << "TODO: for each child element load each note.";
            }
        }
    }

}

void Page::createNote(QXmlStreamReader* stream)
{
    //set all the note properties.
    qreal x = stream->attributes().value("x").toString().toFloat();
    qreal y = stream->attributes().value("y").toString().toFloat();

    qreal width = stream->attributes().value("width").toString().toFloat();
    qreal height = stream->attributes().value("height").toString().toFloat();
    qreal zValue = stream->attributes().value("z").toString().toInt();

    int id = stream->attributes().value("id").toString().toInt();
    Note *n = mScene->createNewNote(id);

    n->setPath(mPagePath);
    n->setPos(x, y);
    n->setSize(QSizeF(width, height));
    n->setZValue(zValue);

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
            n->setImage(imageFile);
            stream->skipCurrentElement();

        } else if (tag == "attachment") {
            QString attachment = stream->attributes().value("file").toString();
            n->setAttachment(attachment);
            stream->skipCurrentElement();

        } else if (tag == "text") {
            QString textFile = stream->attributes().value("file").toString();
            QFile f(mPagePath + "/" + textFile);

            if(!f.open(QIODevice::ReadOnly)) {
                qDebug() << "Error opening note - " << mPagePath + "/" + textFile;

            } else {
                QString text = f.readAll();
                n->setHtml(text);
            }
            stream->skipCurrentElement();


        } else {
            qDebug() << "Unknown note element, skipping" << stream->name().toString();
        }

    }

}

void Page::save()
{

    QString xmlIndex = mPagePath + "/page.xml";
    qDebug() << xmlIndex;
    if(!QFileInfo(xmlIndex).exists()) {
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
            saveNote(n, &stream);

        }

    stream.writeEndElement(); //dwiki_page
    stream.writeEndDocument();
    file.close();

}

void Page::saveNote(Note *n, QXmlStreamWriter *stream)
{

    stream->writeStartElement("note");
    stream->writeAttribute("id", QString::number(n->id()));
    stream->writeAttribute("x", QString::number(n->pos().x()));
    stream->writeAttribute("y", QString::number(n->pos().y()));
    stream->writeAttribute("z", QString::number(n->zValue()));
    stream->writeAttribute("width", QString::number(n->size().width()));
    stream->writeAttribute("height", QString::number(n->size().height()));
    stream->writeAttribute("lastModified", n->lastModified().toString("yyyy-MM-dd hh:mm:ss"));
    stream->writeAttribute("added", n->addedDate().toString("yyyy-MM-dd hh:mm:ss"));

    qDebug() << "TODO: fill in flags";
    stream->writeStartElement("flags");
    //stream->writeAttribute("value", n->flags());
    stream->writeEndElement(); //flags

    qDebug() << "TODO: add note icons";
    stream->writeStartElement("icon");
    stream->writeAttribute("file", "");
    stream->writeEndElement(); //icon

    qDebug() << "TODO: connect string for pointers";
    stream->writeStartElement("connect");
    stream->writeAttribute("id", "");
    stream->writeEndElement(); //connect

    stream->writeStartElement("attachment");
    stream->writeAttribute("file", n->attachment());
    stream->writeEndElement(); //attachment

    stream->writeStartElement("image");
    stream->writeAttribute("file", n->image());
    stream->writeEndElement(); //image

    QString noteFile = "note" + QString::number(n->id()) + ".html";
    stream->writeStartElement("text");
    stream->writeAttribute("file", noteFile);
    stream->writeEndElement(); //text

    stream->writeEndElement(); //note

    if(!n->html().isEmpty()) {
        QFile f(mPagePath + "/" + noteFile);

        if(!f.open(QFile::WriteOnly)) {
            qDebug() << "error opeing file for writing: " << f.fileName();
        }

        QTextStream out(&f);
        out << n->html();
        f.close();
    }

}
