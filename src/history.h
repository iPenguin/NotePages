/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef HISTORY_H
#define HISTORY_H

#include <QUndoCommand>
#include <QString>

class MainWindow;

class History : public QUndoCommand
{
public:
    History(MainWindow *mainWindow, int currentPage, int newPage, QUndoCommand *parent = 0);

    void undo();
    void redo();

private:
    int mCurrentPage;
    int mNewPage;
    MainWindow *mWindow;
};

#endif // HISTORY_H
