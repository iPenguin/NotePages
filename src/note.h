/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef NOTE_H
#define NOTE_H

#include <QGraphicsItem>
#include <QObject>
#include <QDateTime>
#include <QXmlStreamReader>

#include "notecontent.h"
#include "pagescene.h"

#include "arrow.h"
#include "pageglobals.h"

/*********************************************************
 * class NoteOptions:
 * Draws the arrow item for editing the note.
 *
 *********************************************************/
class NoteOptions : public QGraphicsRectItem
{
public:
    NoteOptions(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    enum { Type = UserType + 11 };
    int type () const { return NoteOptions::Type; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
};


/*********************************************************
 * class Note:
 * Draws the outline for the note, and contains the content class.
 *
 *********************************************************/
class Note : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Note(NoteType::Id contentType, QGraphicsItem *parent, QGraphicsScene *scene);
    Note(QXmlStreamReader *stream, QString pagePath, QGraphicsItem *parent, QGraphicsScene *scene);
    
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    enum { Type = UserType + 1 };
    int type() const { return Note::Type; }

    QDateTime lastModified() { return mLastModified; }
    QDateTime addedDate() { return mAdded; }

    void setLastModified(QDateTime dt);
    void setAddedDate(QDateTime dt);

    int id() { return mId; }
    void setId(int id) { mId = id; }

    QString file() { return mContent->file(); }
    void setFile(QString file);
    bool hasFile() const { return !mContent->file().isEmpty(); }
    void removeFile();

    void setSize(QSizeF size);

    void setHtml(QString html) { Q_ASSERT(mContent); mContent->setHtml(html); }
    QString html() const {  Q_ASSERT(mContent); return mContent->toHtml(); }

    QSizeF size() { Q_ASSERT(mContent); return mContent->size(); }

    QString path();
    void setPath(QString p) { mPath = p; }

    void loadNote(QXmlStreamReader *stream, QString pagePath);
    void saveNote(QXmlStreamWriter *stream);

    //perminantly delete the contents of the note.
    void deleteNote();

    void setTextEditMode(bool value);
    QString textSelection();

    void setPixmap(QByteArray imageData);

    QMenu* contextMenu() { return mContent->contextMenu(); }

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
    void addArrow(Arrow *a);
    void removeArrow(Arrow *a);
    void removeAllArrows();

private:
    bool mSizeHandle;

    QDateTime mLastModified;
    QDateTime mAdded;

    QPointF mDiff;
    QSizeF mOldSize;

    int mId;
    NoteOptions* mNoteOptions;
    NoteContent* mContent;

    QList<Arrow*> mArrows;

    QString mPath;
    PageScene *mScene;

    bool mHovering;
};

#endif // NOTE_H
