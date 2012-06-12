/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef NOTE_H
#define NOTE_H

#include <QGraphicsItem>
#include <QDateTime>
#include <QXmlStreamReader>

#include "notetext.h"
#include "noteattachment.h"
#include "noteoptions.h"

class Note : public QGraphicsItem
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
    QString html() const {  Q_ASSERT(mNoteText); return mNoteText->toHtml(); }

    QSizeF size() { Q_ASSERT(mNoteText); return mNoteText->size(); }

    void setImage(QString img);
    QString image() { return mImage; }

    QString path();
    void setPath(QString p) { mPath = p; }

    void loadNote(QXmlStreamReader *stream, QString pagePath);
    void saveNote(QXmlStreamWriter *stream);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
    bool mSizeHandle;

    QDateTime mLastModified;
    QDateTime mAdded;

    QString mAttachment;
    QString mImage;

    QPointF mDiff;
    QSizeF mOldSize;

    int mId;

    NoteText *mNoteText;
    NoteAttachment *mNoteAttachment;
    NoteOptions* mNoteOptions;
    QGraphicsPixmapItem *mNoteImage;

    QString mPath;
};

#endif // NOTE_H
