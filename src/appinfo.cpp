/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include "appinfo.h"

#include <QMessageBox>
#include <QApplication>

#include "settingsui.h"

AppInfo* AppInfo::mInstance = NULL;

AppInfo::AppInfo() :
    appName("Note Pages"),
    appOrg("NotePages.org"),
    
    appOrgDomain("NotePages.org"),
    appOrgContact("http://NotePages.org/contact"),

    appVersion(gGIT_VERSION),
    appVersionShort(gGIT_VERSION_SHORT),
    appBuildInfo(QString(__DATE__) + " " + QString(__TIME__))
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

void AppInfo::toolsSettings()
{

    if(!mSettingsUi) {
        mSettingsUi = new SettingsUi(0);
    }
    mSettingsUi->open();
}
