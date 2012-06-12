/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
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
    
    QString pagePath() { return mPagePath; }
    void setPagePath(QString pp) { mPagePath = pp; }

signals:
    
public slots:
    void deleteNote();
    void addAttachment();
    void loadAttachment(QString fileName);

private slots:
    void showNoteOptions(QPointF screenPos);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

    Note* createNewNote(int noteId = -1);

private:

    int mCurMaxNoteId;

    //requires a path seperator after it.
    QString mPagePath;
};

#endif // PAGESCENE_H
