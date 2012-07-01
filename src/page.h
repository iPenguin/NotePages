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
    enum TextProperty { TxtBold = 1, TxtItalic,
                        TxtUnderline, TxtLeftJustify, TxtCenterJustify, TxtRightJustify, TxtJustify };

    explicit Page(QString pagePath, QWidget *parent = 0);
    ~Page();

    int id() { return mId; }
    void setId(int id) { mId = id; }

    void savePage();

    bool isSaved();

    QUndoStack* undoStack() { return mUndoStack; }
    
    void loadPage();

    void setTextProperties(TextProperty property, bool state);

    int currentZoomLevel();

    void deletePage();

signals:
    void zoomLevelChanged(int value);
    void changePage(QString link);

public slots:
    void zoomChanged(int value);
    void nextPage(QString link);

protected:
    void updateSceneRect(Note *n);

private:
    Ui::Page *ui;

    PageScene *mScene;

    int mId;

    QUndoStack *mUndoStack;

    bool mDeleted;
};

#endif // PAGE_H
