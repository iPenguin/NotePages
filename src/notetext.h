#ifndef NOTE_H
#define NOTE_H

#include <QGraphicsTextItem>
#include <QDateTime>

class NoteText : public QGraphicsTextItem
{

public:
    NoteText(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    enum { Type = UserType + 2 };
    int type () const { return NoteText::Type; }

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    void setSize(QSizeF size);
    QSizeF size() { return mSize; }

signals:
    
public slots:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void focusOutEvent(QFocusEvent *e);

private:
    QGraphicsItem *mParent;

    QSizeF mSize;
};

#endif // NOTE_H
