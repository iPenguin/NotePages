/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include "history.h"

#include "mainwindow.h"
#include <QObject>

#include <QDebug>

History::History(MainWindow *mainWindow, int currentPage, int newPage, QUndoCommand *parent)
    :QUndoCommand(parent),
      mWindow(mainWindow)
{
    qDebug() << "history ctor" << currentPage << newPage;
    setText(QObject::tr(""));
    mCurrentPage = currentPage;
    mNewPage = newPage;
}

void History::undo()
{
    qDebug() << "history undo" << mCurrentPage;
    mWindow->selectPage(mCurrentPage);
}

void History::redo()
{
    qDebug() << "history redo" << mNewPage;
    mWindow->selectPage(mNewPage);
}

