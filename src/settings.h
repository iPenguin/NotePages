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
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QStringList>

class MainWindow;

class Settings : public QObject
{
    Q_OBJECT
public:
    static Settings* inst();
    ~Settings();

    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key) const;

    /**
     * The folder where the user's settings are stored. W/trailing slash.
     */
    QString userSettingsFolder();

    /**
     * return the default value of a given key.
     */
    QVariant defaultValue ( const QString& key ) const;

    /**
     * list of open files. All files should be added toLower().
     */
    QMap<QString, MainWindow*> files;

    void saveSettings() { mSettings.sync(); }

    void addRecentFile(QString fileName);

    QStringList recentFiles() { return mRecentFiles; }
    void setRecentFiles(QStringList files) { mRecentFiles = files; }
    
private:
    /**
     * list of recent files. All strings added case sensitively (they'll be used to open the files).
     */
    QStringList mRecentFiles;
  
    static Settings* mInstance;
    
    Settings();

    void setupValueList();
    
    QSettings mSettings;

    QMap<QString, QVariant> mValueList;

};

#endif //SETTINGS_H
