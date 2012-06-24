/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include "appinfo.h"

#include <QMessageBox>
#include <QApplication>

AppInfo* AppInfo::mInstance = NULL;

AppInfo::AppInfo() :
    appName("DesktopWiki"),
    appOrg("Brian C. Milco"),
    
    appOrgDomain("desktopWiki.org"),
    appOrgContact("http://desktopWiki.org/contact"),

    appVersion(gGIT_VERSION),
    appVersionShort(gGIT_VERSION_SHORT),
    appBuildInfo(QString(__DATE__) + " " + QString(__TIME__)),

    emailRegExp(QRegExp("[a-zA-Z0-9._%+-]+@(?:[a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,4}")),

    liveLicensePage("http://desktopWiki.org/custom/license.php"),
    liveLicensePageVals("?dw_sn=%1&dw_email=%2&dw_fname=%3&dw_lname=%4"),

    liveUpdatePage("http://desktopWiki.org/custom/update.php"),
    liveUpdatePageVals("?dw_software=%1&dw_version=%2&dw_os=%3&dw_sn=%4&dw_arch=%5"),

    demoString(QObject::tr("DesktopWiki - Demo Version  -  ")),

    magicNumber(0x95969530),
    magicNumberSet(0x53095969)
{
}

void AppInfo::helpAbout()
{

    QString aboutInfo = QString(QObject::tr("<h1>%1</h1>"
                                       "<p>Version: %2 (built on %3)</p>"
                                       "<p>Copyright (c) 2012 Brian Milco</p>"
                                       "<p>This software is a note taking application with the"
                                        "ability to do cross-references like an online wiki.</p>")
                                    .arg(qApp->applicationName())
                                    .arg(qApp->applicationVersion())
                                    .arg(appBuildInfo)
                                    );

    QMessageBox::about(qApp->activeWindow(), QObject::tr("About desktopWiki"), aboutInfo);
}
