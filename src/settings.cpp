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
#include "settings.h"

#include <QDebug>
#include "appinfo.h"
#include <QDesktopServices>
#include <QFileInfo>

#include <QMessageBox>

#include <QDir>

// Global static pointer
Settings* Settings::mInstance = NULL;

// singleton constructor:
Settings* Settings::inst()
{
   if (!mInstance)   // Only allow one instance of the settings.
      mInstance = new Settings();
   return mInstance;
}

Settings::Settings()
{
    setupValueList();
    mRecentFiles = value("recentFiles").toStringList();
}

Settings::~Settings()
{
    setValue("recentFiles", QVariant(mRecentFiles));
}

void Settings::setValue(const QString &key, const QVariant &value)
{
    //only save values that aren't defaults, this allows for undefined values to change with updates,
    //while defined values are fixed.
    if(mValueList[key] != value)
        mSettings.setValue(key, value);
    else
        mSettings.remove(key);

}

QVariant Settings::value(const QString &key) const
{
    return mSettings.value(key, defaultValue(key));
}

QVariant Settings::defaultValue ( const QString& key ) const
{
    QVariant value = mValueList.value(key);
    if(!value.isValid())
        qWarning() << "There is no default value for the option:" << key;
    return value;
}

void Settings::setupValueList()
{
    //look up values for setting server/webpage for license and update testing.
    mValueList["updatePage"] = QVariant(AppInfo::inst()->liveUpdatePage + AppInfo::inst()->liveUpdatePageVals);
    
    mValueList["currentNotePages"] = QVariant("");

    mValueList["geometry"] = QVariant("");
    mValueList["windowState"] = QVariant(" "); //use a space because it works for the comparison when saving variables.

    QString userDocs = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);

    mValueList["checkForUpdates"] = QVariant(true);
    mValueList["fileLocation"] = QVariant(userDocs);
    mValueList["maxRecentFiles"] = QVariant(5);
    
    mValueList["currentNotePages"] = userDocs + "/My Notes";
}

void Settings::addRecentFile(QString fileName)
{

    if(mRecentFiles.contains(fileName))
        mRecentFiles.removeAll(fileName);
    mRecentFiles.prepend(fileName);
    setValue("recentFiles", QVariant(mRecentFiles));
}

QString Settings::userSettingsFolder()
{
    QString folder = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    if(!QFileInfo(folder).exists())
        QDir(folder).mkpath(folder);
    return folder + "/";
}

