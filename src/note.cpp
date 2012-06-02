#include "note.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <QGraphicsScene>
#include <QDebug>

#include <math.h>

Note::Note(QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsItemGroup(parent, scene),
    mSizeHandle(false),
    mDiff(QPointF(3,20))
{

    mNoteText = new NoteText(this);
    QRectF r = mNoteText->boundingRect();

    //mNoteSizeHandler = new NoteHandler(this);

    addToGroup(mNoteText);
    //addToGroup(mNoteSizeHandler);

    //mNoteSizeHandler->setRect(100,50,10,10);

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

    QRectF rect = childrenBoundingRect().adjusted(-3,topMargin,0,0);
    rect.setWidth(mNoteText->mSize.width() + mDiff.x());
    rect.setHeight(mNoteText->mSize.height() + mDiff.y());
    qDebug() << "br diff" << mDiff << rect;
    return rect;
}

void Note::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsItemGroup::paint(painter, option, widget);
    QRectF br = boundingRect();

    painter->setPen(Qt::gray);
    painter->setBrush(QBrush(QColor(225,225,225, 128)));
    painter->drawRoundedRect(br.toRect(), 5, 5);
    painter->drawText(0,0, QString::number(br.width()));

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
        mOldBoundingRect = rect;
    }
    qDebug() << "mpe" << boundingRect();
    QGraphicsItemGroup::mousePressEvent(e);
}

void Note::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
    if(mSizeHandle) {
        prepareGeometryChange();
        mDiff = (e->scenePos() - e->buttonDownScenePos(Qt::LeftButton));// - boundingRect().bottomRight();
        qDebug() << "mme" << mDiff << e->scenePos() << e->buttonDownScenePos(Qt::LeftButton) << boundingRect();

        if(mDiff.x() < 0)
            mDiff.setX(0);
        if(mDiff.y() < 0)
            mDiff.setY(0);

        mNoteText->setTextWidth(boundingRect().width());

        update();
        return;
    }

    QGraphicsItemGroup::mouseMoveEvent(e);
}

void Note::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    mSizeHandle = false;
    qDebug() << "mre br" << boundingRect();
    update();
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
    mNoteText->mSize = size;
}
