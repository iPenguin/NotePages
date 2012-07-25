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

    void loadPage();
    void savePage();
    bool isSaved();
    //delete and remove data.
    void deletePage();

    QUndoStack* undoStack() { return mUndoStack; }

    //Set properties on selected text, ie, bold, italics, underline, etc.
    void setTextProperties(TextProperty property, bool state);

    int currentZoomLevel();

    void addLinkToNote(QStringList link);

    void setDrawLines(bool state) { Q_ASSERT(mScene); mScene->setDrawLines(state); }

    Note* currentNote();
    void setDefaultNoteType(NoteType::Id type);

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
