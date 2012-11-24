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
#ifndef APPINFO_H
#define APPINFO_H

#include "version.h"
#include <QString>
#include <QRegExp>
#include <qobject.h>

class SettingsUi;

/*!
 * \class AppInfo
 * Singleton class to supply general application-wide info to all classes.
 * This class also contains some common dialogs like "Help" >> "About" because
 * otherwise they won't be available for the Mac Menu Bar class.
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

    const QString liveUpdatePage; //!< Url to a webpage where you can get current version information.
    const QString liveUpdatePageVals; //!< A string of the values needed to get the update information.
    
    //! Globally available "Help" >> "About" dialog.
    void helpAbout();
    //! Globally available "Tools" >> "Settings" dialog.
    void toolsSettings();    
private:
    static AppInfo* mInstance;
    SettingsUi *mSettingsUi;
    
    AppInfo();
    
};

#endif // APPINFO_H
