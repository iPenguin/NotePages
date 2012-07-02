/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include "appinfo.h"

#include <QMessageBox>
#include <QApplication>

AppInfo* AppInfo::mInstance = NULL;

AppInfo::AppInfo() :
    appName("Note Pages"),
    appOrg("Brian C. Milco"),
    
    appOrgDomain("NotePages.org"),
    appOrgContact("http://NotePages.org/contact"),

    appVersion(gGIT_VERSION),
    appVersionShort(gGIT_VERSION_SHORT),
    appBuildInfo(QString(__DATE__) + " " + QString(__TIME__)),

    emailRegExp(QRegExp("[a-zA-Z0-9._%+-]+@(?:[a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,4}")),

    liveLicensePage("http://NotePages.org/custom/license.php"),
    liveLicensePageVals("?np_sn=%1&np_email=%2&np_fname=%3&np_lname=%4"),

    liveUpdatePage("http://NotePages.org/custom/update.php"),
    liveUpdatePageVals("?np_software=%1&np_version=%2&np_os=%3&np_sn=%4&np_arch=%5")

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

    QMessageBox::about(qApp->activeWindow(), QObject::tr("About Note Pages"), aboutInfo);
}
