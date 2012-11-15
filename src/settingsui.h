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
#ifndef SETTINGSUI_H
#define SETTINGSUI_H

#include <QDialog>

class QAbstractButton;
class QPixmap;

namespace Ui {
    class SettingsDialog;
}

class SettingsUi : public QDialog
{
    Q_OBJECT
public:
    SettingsUi(QWidget* parent);
    ~SettingsUi();

public slots:
    void selectFolder();

private slots:

    void buttonClicked(QAbstractButton* button);
    void saveSettings();

private:

    //TODO: Using the same code in MainWindow
    QPixmap drawColorBox(QColor color, QSize size);

    void setupDialogWidgets();
    void resetDialogWidgets();
    void saveDialogWidgets();
    
    void load(QObject* w);
    void loadDefualt(QObject* w);
    void save(QObject* w);

    bool isSettingsWidget(QObject* obj);
    
    Ui::SettingsDialog* ui;

    QColor mPrimaryColor;
    QColor mAlternateColor;
    QColor mDotColor;
    QColor mKeywordColor;
    
};

#endif //SETTINGSUI_H
