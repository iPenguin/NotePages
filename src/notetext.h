#ifndef NOTE_H
#define NOTE_H

#include <QGraphicsTextItem>
#include <QDateTime>

class NoteText : public QGraphicsTextItem
{

public:
    NoteText(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    QSizeF mSize;

signals:
    
public slots:

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);
    void focusOutEvent(QFocusEvent *e);

private:

};

#endif // NOTE_H
