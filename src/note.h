/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef NOTE_H
#define NOTE_H

#include <QGraphicsItem>
#include <QObject>
#include <QDateTime>
#include <QXmlStreamReader>

#include "notetext.h"
#include "noteattachment.h"
#include "noteoptions.h"

#include "arrow.h"

class Note : public QObject, public QGraphicsItem
{
    Q_OBJECT
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

    QString attachment() { return mNoteAttachment->file(); }
    void setAttachment(QString attchmnt);
    bool hasAttachment() const { return !mNoteAttachment->file().isEmpty(); }
    void removeAttachment();

    void setSize(QSizeF size);

    void setHtml(QString html) { Q_ASSERT(mNoteText); mNoteText->setHtml(html); }
    QString html() const {  Q_ASSERT(mNoteText); return mNoteText->toHtml(); }

    QSizeF size() { Q_ASSERT(mNoteText); return mNoteText->size(); }

    //if size = (-1,-1) use native image size.
    void setImage(QString img, QSizeF size);
    QString image() { return mImage; }

    QString path();
    void setPath(QString p) { mPath = p; }

    void loadNote(QXmlStreamReader *stream, QString pagePath);
    void saveNote(QXmlStreamWriter *stream);

    //perminantly delete the contents of the note.
    void deleteNote();

    void setTextEditMode(bool value) { mNoteText->setTextInteractionFlags(value ? Qt::TextEditorInteraction
                                             : Qt::TextBrowserInteraction); }
    QString textSelection();
signals:
    void pageLinkClicked(QString link);

public slots:
    void signalSend(QString link);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *e);
    //void hoverMoveEvent(QGraphicsSceneHoverEvent *e);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

public:
    QPixmap mPixmap;

    void addArrow(Arrow *a);
    void removeArrow(Arrow *a);

private:
    bool mSizeHandle;

    QDateTime mLastModified;
    QDateTime mAdded;

    QString mImage;

    QPointF mDiff;
    QSizeF mOldSize;

    int mId;

    NoteText *mNoteText;
    NoteAttachment *mNoteAttachment;
    NoteOptions* mNoteOptions;
    QGraphicsPixmapItem *mNoteImage;

    QList<Arrow*> mArrows;

    QString mPath;

    bool mHovering;
};

#endif // NOTE_H
