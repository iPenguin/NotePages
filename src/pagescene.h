#ifndef PAGESCENE_H
#define PAGESCENE_H

#include <QGraphicsScene>

class Note;

class PageScene : public QGraphicsScene
{
    Q_OBJECT
    friend class Page;
public:
    explicit PageScene(QObject *parent = 0);
    
signals:
    
public slots:


private slots:
    void showNoteOptions(QPointF screenPos);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

    Note* createNewNote(int noteId = -1);

private:

    int mCurMaxNoteId;
};

#endif // PAGESCENE_H
