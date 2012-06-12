/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef PAGE_H
#define PAGE_H

#include <QWidget>
#include <QUndoStack>
#include "pagescene.h"

namespace Ui {
class Page;
}

class Page : public QWidget
{
    Q_OBJECT
    
public:
    explicit Page(QString pagePath, QWidget *parent = 0);
    ~Page();

    int id() { return mId; }
    void setId(int id) { mId = id; }

    void savePage();

    QUndoStack* undoStack() { return mUndoStack; }
    
    void loadPage();

private:
    Ui::Page *ui;

    PageScene *mScene;

    int mId;

    QUndoStack *mUndoStack;
};

#endif // PAGE_H
