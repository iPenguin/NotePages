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
    initDemoVersion();
    mRecentFiles = value("recentFiles").toStringList();
}

Settings::~Settings()
{
    setValue("recentFiles", QVariant(mRecentFiles));
}

void Settings::initDemoVersion()
{
/*
#ifndef APPLE_APP_STORE
    QString license = value("license").toString();
    QString sn      = value("serialNumber").toString();
    QString email   = value("email").toString();

    if(!License::isValidSerialNumber(sn)) {
        mIsDemoVersion = true;
        return;
    }
    if(!License::isValidLicense(license, sn, email)) {
        mIsDemoVersion = true;
        return;
    }
#endif
*/
    mIsDemoVersion = false;

}

void Settings::trialVersionMessage(QWidget* parent)
{
    /*
    QMessageBox msgbox(parent);
    msgbox.setWindowTitle(AppInfo::inst()->appName);
    msgbox.setText(tr("This feature is disabled in the demo version."));
    msgbox.setInformativeText(tr("There are example output files and screenshots available at http://%1/%2.")
        .arg(AppInfo::inst()->appOrgDomain).arg(AppInfo::inst()->appName));
    msgbox.setStandardButtons(QMessageBox::Ok);
    msgbox.setIcon(QMessageBox::Information);
    
    msgbox.exec();
    */
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
    mValueList["currentWiki"] = QVariant("");

    mValueList["geometry"] = QVariant("");
    mValueList["windowState"] = QVariant(" "); //use a space because it works for the comparison when saving variables.

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

