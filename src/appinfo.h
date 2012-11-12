/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef APPINFO_H
#define APPINFO_H

#include "version.h"
#include <QString>
#include <QRegExp>
#include <qobject.h>

class SettingsUi;
/**
 * singleton class to supply general application-wide info to all classes.
 */

class AppInfo
{
public:
    static AppInfo* inst()
    {
        if (!mInstance)   // Only allow one instance of the settings.
      mInstance = new AppInfo();
      return mInstance;
    }
    
    const QString appName;
    const QString appOrg;
    
    const QString appOrgDomain;
    const QString appOrgContact;
    
    const QString appVersion;
    const QString appVersionShort;
    const QString appBuildInfo;
    
private:
    static AppInfo* mInstance;
    SettingsUi *mSettingsUi;
    
    AppInfo();
    
};

#endif // APPINFO_H
