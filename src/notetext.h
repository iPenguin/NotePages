/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef NOTETEXT_H
#define NOTETEXT_H

#include <QGraphicsTextItem>
#include <QDateTime>

class QTextCharFormat;

class NoteText : public QGraphicsTextItem
{
    Q_OBJECT

public:
    NoteText(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    enum { Type = UserType + 2 };
    int type () const { return NoteText::Type; }

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    void setSize(QSizeF size);
    QSizeF size() { return mSize; }

    void setBold(bool state);
    void setItalic(bool state);
    void setUnderline(bool state);

    void setTextBlockAlignment(Qt::Alignment align);

signals:
    void pageLink(QString link);

protected slots:
    void hoveringOverLink(QString link);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);

    void hoverMoveEvent(QGraphicsSceneHoverEvent *e);
    void focusOutEvent(QFocusEvent *e);

    void mergeFormatOnSelection(QTextCharFormat format);
private:
    QGraphicsItem *mParent;

    QSizeF mSize;
};

#endif // NOTETEXT_H
