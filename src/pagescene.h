/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef PAGESCENE_H
#define PAGESCENE_H

#include <QGraphicsScene>
#include "pageglobals.h"

class Note;

class PageScene : public QGraphicsScene
{
    Q_OBJECT
    friend class Page;
public:
    explicit PageScene(QObject *parent = 0);
    
    QString pagePath() { return mPagePath; }
    void setPagePath(QString pp) { mPagePath = pp; }

    void incrementMaxNoteId() { mCurMaxNoteId++; }
    void setDrawLines(bool state) { mDrawLines = state; }

signals:
    void changePage(QString newPage);

public slots:
    void deleteNote();
    void addDocument();
    void loadDocument(QString fileName);
    void addImage();
    void loadImage(QString fileName);

    void pageLinkClicked(QString link);

private slots:
    void showNoteOptions(QPointF screenPos);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

    void keyReleaseEvent(QKeyEvent *e);

    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

    void dragEnterEvent(QGraphicsSceneDragDropEvent *e);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *e);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *e);
    void dropEvent(QGraphicsSceneDragDropEvent *e);

    Note* createNewNote(int noteId = -1, NoteType::Id type = NoteType::Text);

private:

    int mCurMaxNoteId;
    bool mDrawLines;
    Note *mLineStart;

    QGraphicsLineItem *mTempLine;
    //requires a path seperator after it.
    QString mPagePath;
};

#endif // PAGESCENE_H
