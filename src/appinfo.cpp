/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include "appinfo.h"

AppInfo* AppInfo::mInstance = NULL;

AppInfo::AppInfo() :
    appName("DesktopWiki"),
    appOrg("DesktopWiki.org"),
    
    appOrgDomain("desktopWiki.org"),
    appOrgContact("http://desktopWiki.org/contact"),

    appVersion(gGIT_VERSION),
    appVersionShort(gGIT_VERSION_SHORT),
    appBuildInfo(QString(__DATE__) + " " + QString(__TIME__))
{
}
