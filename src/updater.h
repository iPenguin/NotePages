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
#ifndef UPDATER_H
#define UPDATER_H

#include <QWidget>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QUrl>

#include <QProgressDialog>

class QFile;

class Updater : public QWidget
{
    Q_OBJECT
public:
    Updater(QWidget* parent = 0);
    ~Updater();

    void checkForUpdates(bool silent = false);
private:
    bool mSilent;

public:
    void downloadInstaller(QUrl url);
    
private slots:
    void startRequest();
    void httpFinished();
    void httpReadyRead();

    void httpFinishedInstaller();
    void httpReadyReadInstaller();
    void launchInstaller();

    void cancelDownload();
    void updateDataTransferProgress(qint64 readBytes, qint64 totalBytes);
    
private:
    QUrl mUrl;
    QNetworkAccessManager qnam;
    QNetworkReply* reply,
                 * instReply;

    QProgressDialog* mProgDialog;
    
    QByteArray mData;
    QFile* installer;
    
    int httpGetId;
    bool httpRequestAborted;
};
#endif //UPDATER_H
