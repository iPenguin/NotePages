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
