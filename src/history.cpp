/*********************************************************************************************\
    Note Pages - the note taking and cross reference application 
    Copyright (c) 2012 Brian C. Milco <bcmilco AT gmail DOT com>       

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; version 2
    of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
\*********************************************************************************************/
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

