#include "page.h"

#include <QLayout>

#include <QDomDocument>
#include <QFile>
#include <QFileInfo>

#include <QDebug>

Page::Page(QString pagePath, QWidget *parent) :
    QWidget(parent)
{
    setLayout(new QVBoxLayout);
    mView = new QGraphicsView(this);
    layout()->addWidget(mView);
    mScene = new QGraphicsScene(this);
    mView->setScene(mScene);

    if(!QFileInfo(pagePath).exists()) {
        qDebug() << "TODO: Create" << pagePath;
        //TODO: create pagePath and some basic data.
    }

    QDomDocument doc("dWikiPage");
    QFile file(pagePath);

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

    //TODO: associate specific pageNumber with a given instance of a Page, then switch to that instance.
    //      the class can then decide if it needs to open file/load data or just switch to what it currently has loaded.

    //TODO: pass loading Page content to the Page class.
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();

    while(!n.isNull()) {

        QDomElement e = n.toElement(); // try to convert the node to an element.
        qDebug() << e.nodeName();
        if(!e.isNull()) {
            if(e.tagName() == "title") {
                //m_name = e.text();
                //TODO: set titlebar: dWikiName - PageName
            } else if (e.tagName() == "properties") {
                qDebug() << "TODO: properties - create parser function to get properties.";

            } else if (e.tagName() == "note") {

                qDebug() << "TODO: load note";
            } else if (e.tagName() == "group") {
                qDebug() << "TODO: for each child element load each note.";
            }
        }
        n = n.nextSibling();
    }

}
