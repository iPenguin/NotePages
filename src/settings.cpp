/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include "settings.h"

//#include "license.h"

#include <QDebug>
//#include "appinfo.h"
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

