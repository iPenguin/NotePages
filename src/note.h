#ifndef Note_H
#define Note_H

#include <QGraphicsItemGroup>
#include <QDateTime>

#include "notetext.h"
#include "notehandler.h"

class Note : public QGraphicsItemGroup
{

public:
    explicit Note(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    int type() const;

    QDateTime lastModified() { return mLastModified; }
    QDateTime addedDate() { return mAdded; }

    void setLastModified(QDateTime dt);
    void setAddedDate(QDateTime dt);

    int id() { return mId; }
    void setId(int id) { mId = id; }

    QString attachment() { return mAttachment; }
    void setAttachment(QString attchmnt) { mAttachment = attchmnt; }

    void setSize(QSizeF size);
    void setHtml(QString html);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

signals:
    
public slots:
    
private:
    bool mSizeHandle;

    QDateTime mLastModified;
    QDateTime mAdded;

    QString mAttachment;

    QPointF mMargins;
    QRectF mOldBoundingRect;

    QSizeF mSize;

    int mId;
public:
    NoteText *mNoteText;
    NoteHandler *mNoteSizeHandler;

};

#endif // Note_H
