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
                                        "ability to do cross-references like an online wiki.</p>"
                                        "<p></p>"
                                        "<p>%1 is distributed under the terms of the General Public License (GPL) version 2.</p>")
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
