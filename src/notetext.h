/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef NOTETEXT_H
#define NOTETEXT_H

#include <QGraphicsTextItem>
#include <QDateTime>

#include "notecontent.h"

class QTextCharFormat;

class NoteText : public QGraphicsTextItem, public NoteContent
{
    Q_OBJECT

public:
    NoteText(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type () const { return NoteType::Text; }

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    void setSize(QSizeF size);
    QSizeF size() { return mSize; }
    void setPos(const QPointF &pos);

    void setBold(bool state);
    void setItalic(bool state);
    void setUnderline(bool state);

    void setTextBlockAlignment(Qt::Alignment align);
    void setTextEditMode(bool value);

    void addLink(QStringList link);

    void loadContent(QXmlStreamReader *stream);
    void saveContent(QXmlStreamWriter *stream);
    void deleteContent();

    QString toHtml();

    QMenu* contextMenu();

signals:
    void pageLink(QString link);
    void linkActivated(QString link);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);
    void focusOutEvent(QFocusEvent *e);

    void mergeFormatOnSelection(QTextCharFormat format);

    void setupContextMenu();

private:
    QGraphicsItem *mParent;

    QSizeF mSize;
};

#endif // NOTETEXT_H
