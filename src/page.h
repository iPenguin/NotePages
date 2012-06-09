#ifndef PAGE_H
#define PAGE_H

#include <QWidget>
#include "pagescene.h"
#include <QGraphicsView>

#include <QXmlStreamReader>
#include <QUndoStack>
#include <QMap>

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

    QUndoStack* undoStack() { return mUndoStack; }

signals:
    
public slots:

private:
    QGraphicsView  *mView;
    PageScene *mScene;

    //requires a path seperator after it.
    QString mPagePath;

    void createNote(QXmlStreamReader *stream);
    void saveNote(Note *n, QXmlStreamWriter *stream);

    int mId;

    QMap<QString, QMap<int, int> > mLines;

    QUndoStack *mUndoStack;
};

#endif // PAGE_H
