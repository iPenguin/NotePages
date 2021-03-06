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
#include "settingsui.h"
#include "ui_settings.h"

#include <QDebug>
#include <QFileDialog>

#include "settings.h"
#include <QPainter>
#include <QColorDialog>

#include <QComboBox>


SettingsUi::SettingsUi(QWidget* parent)
    : QDialog(parent), ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    
    //in case the form gets saved on the wrong tab.
    ui->tabWidget->setCurrentIndex(0);

#ifdef Q_WS_MAC
    setWindowTitle(tr("Preferences"));
#else
    setWindowTitle(tr("Options"));
#endif //Q_WS_MAC

    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(buttonClicked(QAbstractButton*)));
    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(saveSettings()));
    setupDialogWidgets();

    for(int i = 0; i < ui->tabWidget->count(); ++i) {
        foreach(QObject* obj, ui->tabWidget->widget(i)->children()) {
            if(isSettingsWidget(obj))
                load(obj);
        }
    }

}

SettingsUi::~SettingsUi()
{
    delete ui;
    ui = 0;
}

void SettingsUi::saveSettings()
{
    for(int i = 0; i < ui->tabWidget->count(); ++i) {
        foreach(QObject* obj, ui->tabWidget->widget(i)->children()) {
            if(isSettingsWidget(obj))
                save(obj);
        }
    }
    saveDialogWidgets();
}

void SettingsUi::buttonClicked(QAbstractButton* button)
{
    if(ui->buttonBox->buttonRole(button) ==  QDialogButtonBox::ResetRole) {
        for(int i = 0; i < ui->tabWidget->count(); ++i) {
            foreach(QObject* obj, ui->tabWidget->widget(i)->children()) {
                if(isSettingsWidget(obj))
                    loadDefualt(obj);
            }
        }
        resetDialogWidgets();
    }
}

void SettingsUi::load(QObject* w)
{
    QVariant value = Settings::inst()->value(w->objectName());
    
    if(w->inherits("QLineEdit")) {
        qobject_cast<QLineEdit*>(w)->setText(value.toString());
    } else if (w->inherits("QCheckBox")) {
        qobject_cast<QCheckBox*>(w)->setChecked(value.toBool());
    } else if (w->inherits("QSpinBox")) {
        qobject_cast<QSpinBox*>(w)->setValue(value.toInt());
    } else if (w->inherits("QComboBox")) {
        QComboBox* cb = qobject_cast<QComboBox*>(w);
        int index = cb->findText(value.toString());
        cb->setCurrentIndex(index);
    } else {
        qWarning() << "Trying to load unknown settings type";
    }
        
}

void SettingsUi::loadDefualt(QObject* w)
{
    QVariant value = Settings::inst()->defaultValue(w->objectName());
    
    if(w->inherits("QLineEdit")) {
        qobject_cast<QLineEdit*>(w)->setText(value.toString());
    } else if (w->inherits("QCheckBox")) {
        qobject_cast<QCheckBox*>(w)->setChecked(value.toBool());
    } else if (w->inherits("QSpinBox")) {
        qobject_cast<QSpinBox*>(w)->setValue(value.toInt());
    } else if (w->inherits("QComboBox")) {
        QComboBox* cb = qobject_cast<QComboBox*>(w);
        int index = cb->findText(value.toString());
        cb->setCurrentIndex(index);
    } else {
        qWarning() << "Trying to load unknown settings type";
    }
}

void SettingsUi::save(QObject* w)
{
    QVariant value;
    if(w->inherits("QLineEdit")) {
        value = QVariant(qobject_cast<QLineEdit*>(w)->text());
    } else if (w->inherits("QCheckBox")) {
        value = QVariant(qobject_cast<QCheckBox*>(w)->isChecked());
    } else if (w->inherits("QSpinBox")) {
        value = QVariant(qobject_cast<QSpinBox*>(w)->value());
    } else if (w->inherits("QComboBox")) {
        value = QVariant(qobject_cast<QComboBox*>(w)->currentText());
    } else {
        qWarning() << "Trying to save unknown settings type";
    }
    
    if(value.isValid())
        Settings::inst()->setValue(w->objectName(), value);
}

bool SettingsUi::isSettingsWidget(QObject* obj)
{
    if(obj->inherits("QLineEdit"))
        return true;
    if(obj->inherits("QCheckBox"))
        return true;
    if(obj->inherits("QSpinBox"))
        return true;
    if(obj->inherits("QComboBox"))
        return true;

    return false;
}

void SettingsUi::selectFolder()
{
    QString defLoc = Settings::inst()->value("fileLocation").toString();

    QString folder = QFileDialog::getExistingDirectory(this, tr("Select Default Folder"), defLoc);

    if(folder.isEmpty())
        return;

    ui->fileLocation->setText(folder);
}

QPixmap SettingsUi::drawColorBox(QColor color, QSize size)
{
    QPixmap pix = QPixmap(size);
    QPainter p;
    p.begin(&pix);
    p.fillRect(QRect(QPoint(0, 0), size), QColor(color));
    p.drawRect(0, 0, size.width() - 1, size.height() - 1);
    p.end();
    
    return pix;
}

void SettingsUi::setupDialogWidgets()
{


}

void SettingsUi::saveDialogWidgets()
{

}

void SettingsUi::resetDialogWidgets()
{

}
