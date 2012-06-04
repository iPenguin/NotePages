#ifndef PAGE_H
#define PAGE_H

#include <QWidget>
#include "pagescene.h"
#include <QGraphicsView>

#include <QXmlStreamReader>
#include <QUndoStack>

class Note;

class Page : public QWidget
{
    Q_OBJECT
public:
    explicit Page(QString pagePath, QWidget *parent = 0);

    int id() { return mId; }
    void setId(int id) { mId = id; }

    QString pagePath() { return mPagePath; }
    void setPagePath(QString pagePath) { mPagePath = pagePath; }

    void save();

    void undoStack() { return mUndoStack; }

signals:
    
public slots:

private:
    QGraphicsView  *mView;
    PageScene *mScene;

    QString mPagePath;

    void createNote(QXmlStreamReader *stream);
    void saveNote(Note *n, QXmlStreamWriter *stream);

    int mId;

    QUndoStack *mUndoStack;
};

#endif // PAGE_H
