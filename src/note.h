#ifndef NOTE_H
#define NOTE_H

#include <QGraphicsItem>
#include <QDateTime>

class Note : public QGraphicsItem
{

public:
    explicit Note(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    int contentType();

    QDateTime lastModified() { return mLastModified; }
    QDateTime addedDate() { return mAdded; }

    void setLastModified(QDateTime dt);
    void setAddedDate(QDateTime dt);

signals:
    
public slots:

private:
    QDateTime mLastModified;
    QDateTime mAdded;

    QString mText;
};

#endif // NOTE_H
