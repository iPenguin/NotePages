#include "note.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

Note::Note(QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsItemGroup(parent, scene),
    mSizeHandle(false),
    mDragStart(QPointF(0,0)),
    mDiff(QPointF(0,0))
{

    mNoteText = new NoteText();
    addToGroup(mNoteText);

    mAdded = QDateTime::currentDateTime();

    //setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFiltersChildEvents(false);
    //setTextInteractionFlags(Qt::LinksAccessibleByMouse);
}

int Note::type() const
{
    return QGraphicsItem::UserType + 10;
}

QRectF Note::boundingRect() const
{
    int topMargin = -5;
    if(!mAttachment.isEmpty()) {
        topMargin = -24;
    }

    return QGraphicsItemGroup::childrenBoundingRect().adjusted(-3,topMargin,mDiff.x() + 3, mDiff.y() + 20);
}

void Note::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsItemGroup::paint(painter, option, widget);

    painter->setPen(Qt::gray);
    painter->setBrush(QBrush(QColor(225,225,225, 128)));
    painter->drawRoundedRect(boundingRect().toRect(), 5, 5);

    if(!mAttachment.isEmpty()) {
        painter->setPen(QColor(50,50,50));
        painter->drawText(20,-8, mAttachment);
        painter->drawPixmap(0,-20,16,16, QPixmap(":/images/attachment.svg"));
    }

}

void Note::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QRectF rect = QGraphicsItemGroup::childrenBoundingRect().adjusted(-5,-30,mDiff.x() + 5, mDiff.y() + 20);

    if(e->pos().x() >= rect.width() - 25 &&
            e->pos().y() >= rect.height() - 25) {
        qDebug() << "mpe cbr" << rect;
        mSizeHandle = true;
    }

    QGraphicsItemGroup::mousePressEvent(e);
}

void Note::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if(mSizeHandle) {
        mDiff = e->pos() - mDragStart;
        update();
        return;
    }

    QGraphicsItemGroup::mouseMoveEvent(e);
}

void Note::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    mSizeHandle = false;

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
