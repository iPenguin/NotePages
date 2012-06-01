#include "note.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <QGraphicsScene>
#include <QDebug>

#include <math.h>

Note::Note(QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsItemGroup(parent, scene),
    mSizeHandle(false),
    mMargins(QPointF(6,20)),
    mSize(QSize(0,0))
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
    QRectF rect = QGraphicsItemGroup::childrenBoundingRect().adjusted(-3,topMargin,mMargins.x(), mMargins.y());
    rect.setSize(mSize);

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

    if(e->scenePos().x() >= (rect.width() - 25) &&
            e->scenePos().y() >= (rect.height() - 25)) {
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
       // QPointF delta = e->scenePos() - e->buttonDownScenePos()
        QPointF diff = (e->scenePos() - e->buttonDownScenePos(Qt::LeftButton));// - boundingRect().bottomRight();

        //qDebug() << mDiff << e->scenePos() << mapToScene(e->buttonDownScenePos(Qt::LeftButton));
        QRectF newR = boundingRect();
        //qreal width = qMax(mOldBoundingRect.width(), newR.width());
        //qreal height = qMax(mOldBoundingRect.height(), newR.height());

        //update(QRectF(newR.x(), newR.y(), width, height));
        update();
        return;
    }

    QGraphicsItemGroup::mouseMoveEvent(e);
}

void Note::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
    mSizeHandle = false;
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
qDebug() << size;
    mNoteText->setTextWidth(size.width() - (2 * 3));
}

void Note::setHtml(QString html)
{
    Q_ASSERT(mNoteText);
    mNoteText->setHtml(html);

    mSize = QGraphicsItemGroup::childrenBoundingRect().size();
}
