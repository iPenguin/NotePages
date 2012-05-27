#ifndef PAGE_H
#define PAGE_H

#include <QWidget>
#include "pagescene.h"
#include <QGraphicsView>

#include <QXmlStreamReader>

class Page : public QWidget
{
    Q_OBJECT
public:
    explicit Page(QString pagePath, QWidget *parent = 0);

    int id() { return mId; }
    void setId(int id) { mId = id; }

    QString pagePath() { return mPagePath; }
    void setPagePath(QString pagePath) { mPagePath = pagePath; }

signals:
    
public slots:
    
private:
    QGraphicsView  *mView;
    PageScene *mScene;

    QString mPagePath;

    void createNote(QXmlStreamReader *stream);

    int mId;
};

#endif // PAGE_H
