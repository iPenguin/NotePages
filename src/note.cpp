#include "note.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <QGraphicsScene>
#include <QDebug>

#include <QTextDocument>
#include <math.h>

Note::Note(QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsItemGroup(parent, scene),
    mSizeHandle(false),
    mDiff(QPointF(3,20)),
    mOldSize(QSizeF(0,0))
{

    mNoteText = new NoteText(this, scene);
    QRectF r = mNoteText->boundingRect();

    mAdded = QDateTime::currentDateTime();

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);

    mNoteAttachment = new NoteAttachment(this, scene);
    mNoteAttachment->setPos(18, -26);
    mNoteAttachment->hide();

    mNoteText->setPos(0,0);

    mNoteOptions = new NoteOptions(this, scene);
    mNoteOptions->setPos(0,-26);

}

QRectF Note::boundingRect() const
{
    int topMargin = -3;
    int bottomMargin = 45;

    QRectF rect = childrenBoundingRect().adjusted(-3,topMargin,0,0);
    rect.setWidth(mNoteText->size().width() + 6);
    rect.setHeight(mNoteText->size().height() + bottomMargin);
    return rect;
}

void Note::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsItemGroup::paint(painter, option, widget);
    QRectF br = boundingRect();

    painter->setPen(Qt::gray);
    painter->setBrush(QBrush(QColor(225,225,225, 128)));
    painter->drawRoundedRect(br.toRect(), 5, 5);

    //draw resize handle.
    painter->drawLine(QPointF(br.right(), br.bottom() - 15), QPointF(br.right() - 15, br.bottom()));
    painter->drawLine(QPointF(br.right() -3, br.bottom() - 7), QPointF(br.right() - 7, br.bottom() - 3));

}

void Note::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItemGroup::mousePressEvent(e);

    QRectF rect = boundingRect();
    QPointF pt = mapToScene(rect.bottomRight());

    if(e->scenePos().x() >= (pt.x() - 25) &&
            e->scenePos().y() >= (pt.y() - 25)) {
        mSizeHandle = true;
        mOldSize = mNoteText->size();
    }
}

void Note::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if(mSizeHandle) {
        prepareGeometryChange();
        mDiff = (e->scenePos() - e->buttonDownScenePos(Qt::LeftButton));

        QSizeF newSize = QSizeF(mOldSize.width() + mDiff.x(), mOldSize.height() + mDiff.y());
        if(mNoteAttachment->document()->size().width() + mNoteOptions->rect().width() > newSize.width())
            newSize.setWidth(mNoteAttachment->document()->size().width() + mNoteOptions->rect().width());
        mNoteText->setSize(newSize);

        update();
        return;
    }

    QGraphicsItemGroup::mouseMoveEvent(e);
}

void Note::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{

    if(mSizeHandle) {
        mDiff = QPointF(0,0);
        mSizeHandle = false;
        update();
    }

    QGraphicsItemGroup::mouseReleaseEvent(e);

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

    mAttachment = attchmnt;

    if(!mAttachment.isEmpty()) {
        mNoteAttachment->setHtml("<a href=\"file://" + mAttachment +"\"><img src=\"/Users/brian/projects/desktopWiki/images/attachment.svg\" height=16 width=16 />" + mAttachment + "</a>");
        mNoteAttachment->show();
    }
}
