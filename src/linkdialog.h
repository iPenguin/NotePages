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
#ifndef LINKDIALOG_H
#define LINKDIALOG_H

#include <QDialog>
#include <QTreeWidget>

namespace Ui {
class LinkDialog;
}

class LinkDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LinkDialog(QTreeWidget *tree, QWidget *parent = 0);
    ~LinkDialog();
    
    QString linkText();
    void generateBasketList(QTreeWidget *tree, QTreeWidgetItem *item, int indent);

    void setLinkText(QString text);

signals:
    void linkCreated(QStringList link);

private:
    Ui::LinkDialog *ui;

private slots:
    void generateLink();

};

#endif // LINKDIALOG_H
