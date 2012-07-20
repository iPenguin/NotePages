/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include <QtGui/QApplication>
#include "mainwindow.h"

#include "appinfo.h"
#include "macmenubar.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qApp->setApplicationName(AppInfo::inst()->appName);
    qApp->setApplicationVersion(AppInfo::inst()->appVersion);
    qApp->setOrganizationName(AppInfo::inst()->appOrg);
    qApp->setOrganizationDomain(AppInfo::inst()->appOrgDomain);

#ifdef Q_OS_MAC

    new MacMenuBar();

#endif

    MainWindow *mainWin = new MainWindow;
    mainWin->show();

    return a.exec();
}


