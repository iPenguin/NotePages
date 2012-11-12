/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#include <QtGui/QApplication>
#include "mainwindow.h"

#include "appinfo.h"

//#include "errorhandler.h"

int main(int argc, char *argv[])
{

    //qInstallMsgHandler(myMessageOutput);

    qApp->setApplicationName(AppInfo::inst()->appName);
    qApp->setApplicationVersion(AppInfo::inst()->appVersion);
    qApp->setOrganizationName(AppInfo::inst()->appOrg);
    qApp->setOrganizationDomain(AppInfo::inst()->appOrgDomain);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
