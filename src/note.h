#ifndef NOTE_H
#define NOTE_H

#include <QGraphicsTextItem>
#include <QDateTime>

class Note : public QGraphicsTextItem
{

public:
    Note(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    int type() const;

    QDateTime lastModified() { return mLastModified; }
    QDateTime addedDate() { return mAdded; }

    void setLastModified(QDateTime dt);
    void setAddedDate(QDateTime dt);

    QSizeF mSize;

    int id() { return mId; }
    void setId(int id) { mId = id; }

    QString attachment() { return mAttachment; }
    void setAttachment(QString attchmnt) { mAttachment = attchmnt; }

signals:
    
public slots:

private:
    QDateTime mLastModified;
    QDateTime mAdded;

    QString mText;

    QString mAttachment;

    int mId;
};

#endif // NOTE_H
