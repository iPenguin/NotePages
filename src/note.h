#ifndef Note_H
#define Note_H

#include <QGraphicsItemGroup>
#include <QDateTime>

#include "notetext.h"
#include "noteattachment.h"
#include "noteoptions.h"

class Note : public QGraphicsItemGroup
{

public:
    explicit Note(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    enum { Type = UserType + 1 };
    int type () const { return Note::Type; }

    QDateTime lastModified() { return mLastModified; }
    QDateTime addedDate() { return mAdded; }

    void setLastModified(QDateTime dt);
    void setAddedDate(QDateTime dt);

    int id() { return mId; }
    void setId(int id) { mId = id; }

    QString attachment() { return mAttachment; }
    void setAttachment(QString attchmnt);

    void setSize(QSizeF size);
    void setHtml(QString html) { Q_ASSERT(mNoteText); mNoteText->setHtml(html); }

    QSizeF size() { Q_ASSERT(mNoteText); return mNoteText->size(); }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

signals:
    
public slots:
    
private slots:
    void showNoteMenu();

private:
    bool mSizeHandle;

    QDateTime mLastModified;
    QDateTime mAdded;

    QString mAttachment;

    QPointF mDiff;
    QSizeF mOldSize;

    int mId;

    NoteText *mNoteText;
    NoteAttachment *mNoteAttachment;
    NoteOptions* mNoteOptions;

};

#endif // Note_H
