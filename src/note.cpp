#include "note.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <QGraphicsScene>
#include <QDebug>

#include <math.h>

Note::Note(QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsItemGroup(parent, scene),
    mSizeHandle(false),
    mDiff(QPointF(3,20)),
    mOldSize(QSizeF(0,0))
{

    mNoteText = new NoteText(this);
    QRectF r = mNoteText->boundingRect();

    addToGroup(mNoteText);

    mAdded = QDateTime::currentDateTime();

    setFlag(QGraphicsItem::ItemIsMovable);
    //setFlag(QGraphicsItem::ItemIsSelectable);
    setFiltersChildEvents(false);

}

int Note::type() const
{
    return QGraphicsItem::UserType + 10;
}

QRectF Note::boundingRect() const
{
    int topMargin = -5;
    int bottomMargin = 26;
    if(!mAttachment.isEmpty()) {
        topMargin = -24;
        bottomMargin = 45;

    }

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

    if(!mAttachment.isEmpty()) {
        painter->setPen(QColor(50,50,50));
        painter->drawText(20,-8, mAttachment);
        painter->drawPixmap(0,-20,16,16, QPixmap(":/images/attachment.svg"));
    }

}

void Note::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QRectF rect = boundingRect();
    QPointF pt = mapToScene(rect.bottomRight());

    if(e->scenePos().x() >= (pt.x() - 25) &&
            e->scenePos().y() >= (pt.y() - 25)) {
        mSizeHandle = true;
        mOldSize = mNoteText->size();
        qDebug() << mOldSize;
    }
    qDebug() << "mpe" << boundingRect();
    QGraphicsItemGroup::mousePressEvent(e);
}

void Note::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if(mSizeHandle) {
        prepareGeometryChange();
        mDiff = (e->scenePos() - e->buttonDownScenePos(Qt::LeftButton));
        QSizeF newSize = QSizeF(mOldSize.width() + mDiff.x(), mOldSize.height() + mDiff.y());
        mNoteText->setSize(newSize);

        update();
        return;
    }

    QGraphicsItemGroup::mouseMoveEvent(e);
}

void Note::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    if(mSizeHandle) {
        //mNoteText->setSize(QSizeF(mOldSize.width() + mDiff.x(), mOldSize.height() + mDiff.y()));
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
    qDebug() << "setsize " << size;
    mNoteText->setSize(size);
    update();
}
