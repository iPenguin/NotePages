#include "note.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <QGraphicsScene>
#include <QDebug>
#include <QFileInfo>

#include <QTextDocument>
#include <math.h>

#include <QCursor>

Note::Note(QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsItem(parent, scene),
    mSizeHandle(false),
    mImage(""),
    mDiff(QPointF(3,20)),
    mOldSize(QSizeF(0,0))
{

    mNoteText = new NoteText(this, scene);
    QRectF r = mNoteText->boundingRect();

    mAdded = QDateTime::currentDateTime();

    setFlag(QGraphicsItem::ItemIsMovable);

    mNoteAttachment = new NoteAttachment(this, scene);
    mNoteAttachment->setPos(18, -26);
    mNoteAttachment->hide();

    mNoteText->setPos(0,0);

    mNoteOptions = new NoteOptions(this, scene);
    mNoteOptions->setPos(0,-26);

    setCursor(QCursor(Qt::OpenHandCursor));

    //TODO: make the images display properly on the note.
    mNoteImage = new QGraphicsPixmapItem(this, scene);
    mNoteImage->setPos(0,0);
    mNoteImage->hide();
    mNoteText->setSize(QSizeF(100,50));

    mNoteText->setTextInteractionFlags(Qt::TextEditorInteraction);
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
    //QGraphicsItemGroup::paint(painter, option, widget);
    QRectF br = boundingRect();

    painter->setPen(Qt::gray);
    painter->setBrush(QBrush(QColor(225,225,225, 128)));
    painter->drawRoundedRect(br.toRect(), 5, 5);

    painter->drawText(0,0, QString::number(id()));
    //draw resize handle.
    painter->drawLine(QPointF(br.right(), br.bottom() - 15), QPointF(br.right() - 15, br.bottom()));
    painter->drawLine(QPointF(br.right() -3, br.bottom() - 7), QPointF(br.right() - 7, br.bottom() - 3));

}

void Note::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    QGraphicsItem::mousePressEvent(e);

    QRectF rect = boundingRect();
    QPointF pt = mapToScene(rect.bottomRight());

    if(e->scenePos().x() >= (pt.x() - 25) &&
            e->scenePos().y() >= (pt.y() - 25)) {
        mSizeHandle = true;
        setCursor(QCursor(Qt::SizeFDiagCursor));
        mOldSize = mNoteText->size();
    }

    setCursor(QCursor(Qt::ClosedHandCursor));
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

    mAttachment = attchmnt;

    if(!mAttachment.isEmpty()) {
        mNoteAttachment->setHtml("<a href=\"file://" + mAttachment +"\"><img src=\"/Users/brian/projects/desktopWiki/images/attachment.svg\" height=16 width=16 />" + mAttachment + "</a>");
        mNoteAttachment->show();
        //FIXME: simplify the addition of html and file, path into a function for the NoteAttachment class.
        mNoteAttachment->mPath = mPath;
        mNoteAttachment->mFile = mAttachment;
    }
}

void Note::setImage(QString img)
{
    mImage = img;

    mNoteImage->setPixmap(QPixmap(mImage));
}
