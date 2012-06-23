/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include "note.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <QGraphicsScene>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

#include <QTextDocument>
#include <math.h>

#include <QCursor>

Note::Note(QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsItem(parent, scene),
    mSizeHandle(false),
    mImage(""),
    mDiff(QPointF(3,20)),
    mOldSize(QSizeF(100,50)),
    mNoteImage(0)
{

    mNoteText = new NoteText(this, scene);
    mNoteText->setPos(0,0);
    mNoteText->setTextInteractionFlags(Qt::TextEditorInteraction);

    setFlag(QGraphicsItem::ItemIsMovable);
    setCursor(QCursor(Qt::OpenHandCursor));
    mAdded = QDateTime::currentDateTime();

    mNoteAttachment = new NoteAttachment(this, scene);
    mNoteAttachment->setPos(18, -24);
    mNoteAttachment->hide();

    mNoteOptions = new NoteOptions(this, scene);
    mNoteOptions->setPos(0,-24);

}

QRectF Note::boundingRect() const
{
    int topMargin = -3;
    int leftMargin = -3;
    int bottomMargin = 30;

    QSizeF size;

    QRectF rect = childrenBoundingRect().adjusted(leftMargin,topMargin,0,0);

    if(mNoteImage) {
        size = mNoteImage->boundingRect().size();
    } else {
        size = mNoteText->size();
    }
    size += QSize(6, bottomMargin);
    rect.setSize(size);

    return rect;
}

void Note::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF br = boundingRect();

    painter->setPen(Qt::gray);
    painter->setBrush(QBrush(QColor(225,225,225, 128)));
    painter->drawRoundedRect(br.toRect(), 5, 5);

    //DEBUG: show id
    //painter->drawText(0,0, QString::number(id()));

    //draw resize handle.
    painter->drawLine(QPointF(br.right(), br.bottom() - 15), QPointF(br.right() - 15, br.bottom()));
    painter->drawLine(QPointF(br.right() -3, br.bottom() - 7), QPointF(br.right() - 7, br.bottom() - 3));

}

void Note::deleteNote()
{
    QString noteFile = mPath + "/note" + QString::number(mId) + ".html";
    qDebug() << noteFile;
    if(QFileInfo(noteFile).exists()) {
        qDebug() << mPath;
        QDir d(mPath);
        d.remove(noteFile);

    }

    //FIXME: offer to delete any attachments

    //FIXME: offer to delete any images.

}

void Note::loadNote(QXmlStreamReader* stream, QString pagePath)
{
    //set all the note properties.
    qreal x = stream->attributes().value("x").toString().toFloat();
    qreal y = stream->attributes().value("y").toString().toFloat();

    qreal width = stream->attributes().value("width").toString().toFloat();
    qreal height = stream->attributes().value("height").toString().toFloat();
    qreal zValue = stream->attributes().value("z").toString().toInt();

    int id = stream->attributes().value("id").toString().toInt();
    setId(id);

    setPath(pagePath);
    setPos(x, y);
    setSize(QSizeF(width, height));
    setZValue(zValue);

    QDateTime lastMod = QDateTime::fromString(stream->attributes().value("lastModified").toString(), "");
    QDateTime added = QDateTime::fromString(stream->attributes().value("lastModified").toString(), "");
    setLastModified(lastMod);
    setAddedDate(added);


    while(!(stream->isEndElement() && stream->name() == "note")) {

        stream->readNextStartElement();
        QString tag = stream->name().toString();

        if (tag == "connect") {
            int connectedNote = stream->attributes().value("id").toString().toInt();
            //TODO:  make this a qlist that is used to id connections to draw in the painting routines. ?
            if(connectedNote > 0) {
                qDebug() << "TODO: connect this note to note" << connectedNote;

            }
            stream->skipCurrentElement();

        } else if (tag == "image") {
            QString imageFile = stream->attributes().value("file").toString();
            qreal width = stream->attributes().value("width").toString().toFloat();
            qreal height = stream->attributes().value("height").toString().toFloat();
            setImage(imageFile, QSizeF(width, height));
            stream->skipCurrentElement();

        } else if (tag == "attachment") {
            QString attachment = stream->attributes().value("file").toString();
            setAttachment(attachment);
            stream->skipCurrentElement();

        } else if (tag == "text") {
            QString textFile = stream->attributes().value("file").toString();
            QFile f(pagePath + "/" + textFile);

            if(!f.open(QIODevice::ReadOnly)) {
                qDebug() << "Error opening note - " << f.fileName();

            } else {
                QString text = f.readAll();
                setHtml(text);
            }
            stream->skipCurrentElement();

        } else {
            qDebug() << "Unknown note element, skipping" << stream->name().toString();
        }

    }
}

void Note::saveNote(QXmlStreamWriter *stream)
{

    stream->writeStartElement("note");
    stream->writeAttribute("id", QString::number(id()));
    stream->writeAttribute("x", QString::number(pos().x()));
    stream->writeAttribute("y", QString::number(pos().y()));
    stream->writeAttribute("z", QString::number(zValue()));
    stream->writeAttribute("width", QString::number(size().width()));
    stream->writeAttribute("height", QString::number(size().height()));
    stream->writeAttribute("lastModified", lastModified().toString("yyyy-MM-dd hh:mm:ss"));
    stream->writeAttribute("added", addedDate().toString("yyyy-MM-dd hh:mm:ss"));

    qDebug() << "TODO: connect string for pointers";
    //TODO: foreach connection create an xml fragment.
    stream->writeStartElement("connect");
    stream->writeAttribute("id", "");
    stream->writeEndElement(); //connect

    stream->writeStartElement("attachment");
    stream->writeAttribute("file", attachment());
    stream->writeEndElement(); //attachment

    if(mNoteImage) {
        stream->writeStartElement("image");
        stream->writeAttribute("file", image());
        stream->writeAttribute("width", QString::number(mNoteImage->boundingRect().width()));
        stream->writeAttribute("height", QString::number(mNoteImage->boundingRect().height()));
        stream->writeEndElement(); //image
    }

    QString noteFile = "note" + QString::number(id()) + ".html";
    stream->writeStartElement("text");
    stream->writeAttribute("file", noteFile);
    stream->writeEndElement(); //text

    stream->writeEndElement(); //note

    if(!html().isEmpty()) {
        QFile f(mPath + "/" + noteFile);

        if(!f.open(QFile::WriteOnly)) {
            qDebug() << "error opeing file for writing: " << f.fileName();
        }

        QTextStream out(&f);
        out << html();
        f.flush();
        f.close();
    }

}

void Note::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem::mousePressEvent(e);

    QRectF rect = boundingRect();
    QPointF pt = mapToScene(rect.bottomRight());

    if(e->scenePos().x() >= (pt.x() - 25) &&
            e->scenePos().y() >= (pt.y() - 25)) {
        mSizeHandle = true;

        mOldSize = (mNoteImage ? QSizeF(mNoteImage->pixmap().size()) : mNoteText->size());
    }

    setCursor(QCursor(Qt::ClosedHandCursor));
}

void Note::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if(mSizeHandle) {
        prepareGeometryChange();
        mDiff = (e->scenePos() - e->buttonDownScenePos(Qt::LeftButton));

        QSizeF newSize = QSizeF(mOldSize.width() + mDiff.x(), mOldSize.height() + mDiff.y());

        if(mNoteImage) {
            if(e->modifiers() == Qt::ShiftModifier) {
                qreal ratio = qreal(mPixmap.size().width()) / qreal(mPixmap.size().height());
                newSize.setWidth(ratio * newSize.height());
            }
        }

        qreal headerWidth = mNoteAttachment->document()->size().width() + mNoteOptions->rect().width();
        if( headerWidth > newSize.width())
            newSize.setWidth(headerWidth);

        if(newSize.height() < 1)
            newSize.setHeight(1);

        if(mNoteText->isVisible()) {
            mNoteText->setSize(newSize);
        }

        if(mNoteImage)
            mNoteImage->setPixmap(mPixmap.scaled(newSize.toSize()));

        update();
        return;
    }

    QGraphicsItem::mouseMoveEvent(e);
}

void Note::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{

    if(mSizeHandle) {
        mDiff = QPointF(0,0);
        setCursor(QCursor(Qt::OpenHandCursor));
        mSizeHandle = false;
        update();
    }

    QGraphicsItem::mouseReleaseEvent(e);

    setCursor(QCursor(Qt::OpenHandCursor));
}


void Note::setLastModified(QDateTime dt)
{
    mLastModified = dt;
}

void Note::setAddedDate(QDateTime dt)
{
    mLastModified = dt;
}

void Note::setSize(QSizeF size)
{
    Q_ASSERT(mNoteText);

    mNoteText->setSize(size);
    update();
}

void Note::setAttachment(QString attchmnt)
{
    mNoteAttachment->setAttachment(mPath, attchmnt);
}

void Note::removeAttachment()
{
    QString file = mNoteAttachment->file();
    mNoteAttachment->setAttachment(mPath, "");

    if(QFileInfo(mPath + "/" + file).exists()) {
        QDir(mPath).remove(file);
    }
}

void Note::setImage(QString img, QSizeF size)
{

    QRectF br = boundingRect();

    mImage = img;
    if(!mImage.isEmpty()) {
        mPixmap = QPixmap(size.isEmpty() ? br.size().toSize() : size.toSize());
        mPixmap.load(mPath + "/" + img);

        mNoteImage = scene()->addPixmap(mPixmap.scaled(size.isEmpty() ? br.size().toSize() : size.toSize()));
        mNoteImage->setParentItem(this);
        mNoteImage->setPos(0,0);

        mNoteText->hide();
        mNoteText->setTextWidth(1);
    } else
        mNoteText->show();

}
