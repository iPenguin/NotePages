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

#include <QFileIconProvider>
#include <QDesktopServices>
#include <QUrl>

#include <QTextCursor>
#include <QCursor>

#include "notetext.h"
#include "noteimage.h"
#include "notedocument.h"

/*********************************************************
 *
 * functions for NoteOptions class.
 *
 *********************************************************/
NoteOptions::NoteOptions(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsRectItem(parent, scene)
{
    setRect(0,0,16,13);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setZValue(1000);

    setCursor(QCursor(Qt::ArrowCursor));
}

void NoteOptions::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPolygonF poly = QPolygonF();
    poly << QPoint(3,3) << QPoint(13,3) << QPoint(8,10);
    painter->drawPolygon(poly);
    QPainterPath path;
    path.addPolygon(poly);
    painter->fillPath(path, QBrush(Qt::black));
    QGraphicsRectItem::paint(painter, option, widget);
}



/*********************************************************
 *
 * functions for Note class.
 *
 *********************************************************/
Note::Note(NoteType::Id contentType, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsItem(parent, scene),
      mSizeHandle(false),
      mDiff(QPointF(3,20)),
      mOldSize(QSizeF(100,50)),
      mId(0),
      mContent(0),
      mHovering(false)
{

    switch(contentType) {
        case NoteType::Image:
            mContent = new NoteImage(this, scene);
            break;

        case NoteType::Document:
            mContent = new NoteDocument(this, scene);
            break;

        case NoteType::Text:
        default:
            NoteText *nt;
            mContent = nt = new NoteText(this, scene);
            QObject::connect(nt, SIGNAL(linkActivated(QString)), SLOT(signalSend(QString)));
            break;
    }

    mContent->setPos(QPointF(0,0));
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setCursor(QCursor(Qt::OpenHandCursor));
    setAcceptHoverEvents(true);

    mAdded = QDateTime::currentDateTime();

    mNoteOptions = new NoteOptions(this, scene);

    if(contentType == NoteType::Document) {
        mNoteOptions->setPos(0,130);
    } else {
        mNoteOptions->setPos(0,-18);
    }
}

Note::Note(QXmlStreamReader *stream, QString pagePath, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsItem(parent, scene),
      mSizeHandle(false),
      mDiff(QPointF(3,20)),
      mOldSize(QSizeF(100,50)),
      mId(0),
      mContent(0),
      mHovering(false)
{
    loadNote(stream, pagePath);

    mContent->setPos(QPointF(0,0));
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setCursor(QCursor(Qt::OpenHandCursor));
    setAcceptHoverEvents(true);

    mAdded = QDateTime::currentDateTime();

    mNoteOptions = new NoteOptions(this, scene);
    if(mContent->contentType() == NoteType::Document) {
        mNoteOptions->setPos(0,130);
    } else {
        mNoteOptions->setPos(0,-18);
    }

}

QRectF Note::boundingRect() const
{
    int topMargin = -3;
    int leftMargin = -3;
    int bottomMargin = 24;

    QSizeF size;

    QRectF rect = childrenBoundingRect().adjusted(leftMargin,topMargin,0,0);

    if(mContent) {
        size = mContent->size();
    }
    size += QSizeF(6, bottomMargin);
    rect.setSize(size);

    return rect;
}

void Note::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(mContent->contentType() != NoteType::Document) {
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
}

void Note::deleteNote()
{

    mContent->deleteContent();

    foreach(Arrow *a, mArrows) {
        removeArrow(a);
    }
}

void Note::setTextEditMode(bool value)
{
    Q_ASSERT(mContent);

    mContent->setTextEditMode(value);
}

QString Note::textSelection()
{
    Q_ASSERT(mContent);
    if(mContent->contentType() != NoteType::Text)
        return "";

    NoteText *nt = static_cast<NoteText*>(mContent);
    return nt->textCursor().selectedText();
}

void Note::setPixmap(QByteArray imageData)
{
    mContent->setImage(imageData);

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

    int contentType = stream->attributes().value("type").toString().toInt();

    setId(id);
    setPath(pagePath);

    QDateTime lastMod = QDateTime::fromString(stream->attributes().value("lastModified").toString(), "");
    QDateTime added = QDateTime::fromString(stream->attributes().value("lastModified").toString(), "");
    setLastModified(lastMod);
    setAddedDate(added);

    switch(contentType) {
        case NoteType::Image:
            mContent = new NoteImage(this, scene());
            break;

        case NoteType::Document:
            mContent = new NoteDocument(this, scene());
            break;

        case NoteType::Text:
        default:
            NoteText *nt = new NoteText(this, scene());
            mContent = nt;
            connect(nt, SIGNAL(linkActivated(QString)), SLOT(signalSend(QString)));
            break;
    }

    mContent->loadContent(stream);

    setPos(x, y);
    setSize(QSizeF(width, height));
    setZValue(zValue);
}

void Note::saveNote(QXmlStreamWriter *stream)
{

    stream->writeStartElement("note");
    stream->writeAttribute("id", QString::number(id()));
    stream->writeAttribute("type", QString::number(mContent->type()));
    stream->writeAttribute("x", QString::number(pos().x()));
    stream->writeAttribute("y", QString::number(pos().y()));
    stream->writeAttribute("z", QString::number(zValue()));
    stream->writeAttribute("width", QString::number(size().width()));
    stream->writeAttribute("height", QString::number(size().height()));
    stream->writeAttribute("lastModified", lastModified().toString("yyyy-MM-dd hh:mm:ss"));
    stream->writeAttribute("added", addedDate().toString("yyyy-MM-dd hh:mm:ss"));

    mContent->saveContent(stream);

    stream->writeEndElement(); //note

}

void Note::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem::mousePressEvent(e);

    QRectF rect = boundingRect();
    QPointF pt = mapToScene(rect.bottomRight());

    if(e->scenePos().x() >= (pt.x() - 25) &&
            e->scenePos().y() >= (pt.y() - 25)) {
        mSizeHandle = true;

        mOldSize = mContent->size();
    }

    setCursor(QCursor(Qt::ClosedHandCursor));
}

void Note::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if(mSizeHandle) {
        prepareGeometryChange();
        mDiff = (e->scenePos() - e->buttonDownScenePos(Qt::LeftButton));

        QSizeF newSize = QSizeF(mOldSize.width() + mDiff.x(), mOldSize.height() + mDiff.y());

        qreal headerWidth = mNoteOptions->rect().width();
        if( headerWidth > newSize.width())
            newSize.setWidth(headerWidth);

        if(newSize.height() < 1)
            newSize.setHeight(1);

        mContent->setSize(newSize);

        foreach(Arrow *a, mArrows) {
            a->updatePosition();
        }

        update();
        return;
    }

    if(scene()) {
        QRectF itemRect = scene()->itemsBoundingRect();
        QRectF sceneRect = scene()->sceneRect();
        QRectF final = itemRect.unite(sceneRect);

        scene()->setSceneRect(final);
    }

    QGraphicsItem::mouseMoveEvent(e);
}

void Note::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{

    if(mSizeHandle) {
        mDiff = QPointF(0,0);
        mSizeHandle = false;
        update();
    }

    QGraphicsItem::mouseReleaseEvent(e);

    setCursor(QCursor(Qt::OpenHandCursor));
}

void Note::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{

    mHovering = true;
    setZValue(100);
    QGraphicsItem::hoverEnterEvent(e);
}

void Note::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
    mHovering = false;
    setZValue(0);
    QGraphicsItem::hoverLeaveEvent(e);
}

QVariant Note::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{

    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Arrow *arrow, mArrows) {
            arrow->updatePosition();
        }
    }

    return value; //QGraphicsItem::itemChange(change, value);
}

void Note::addArrow(Arrow *a)
{
    mArrows.append(a);
}

void Note::removeArrow(Arrow *a)
{
    int index = mArrows.indexOf(a);

    if (index != -1) {
        scene()->removeItem(a);
        mArrows.removeAt(index);
    }
}

void Note::removeAllArrows()
{
    foreach(Arrow *a, mArrows) {
        removeArrow(a);
    }
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
    Q_ASSERT(mContent);

    mContent->setSize(size);
    update();
}

void Note::setDocument(QString doc)
{
    mContent->setFile(doc);

}

void Note::removeDocument()
{
    QString file = mContent->file();
    mContent->setFile("");

    if(QFileInfo(mPath + "/" + file).exists()) {
        QDir(mPath).remove(file);
    }
}

void Note::setImage(QString img, QSizeF size)
{

    QRectF br = boundingRect();

    mContent->setImage(img, size);

}

void Note::signalSend(QString link)
{
    if(link.startsWith("npage://"))
        emit pageLinkClicked(link);
    else
        QDesktopServices::openUrl(QUrl(link));
}
