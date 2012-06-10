/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QStringList>

class MainWindow;

class Settings : public QObject
{
    Q_OBJECT
public:
    static Settings* inst();
    ~Settings();

    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key) const;

    bool isDemoVersion() { return mIsDemoVersion; }
    void setDemoVersion(bool isDemo) { mIsDemoVersion = isDemo; }

    void trialVersionMessage(QWidget* parent);
    
    /**
     * The folder where the user's settings are stored. W/trailing slash.
     */
    QString userSettingsFolder();

    /**
     * return the default value of a given key.
     */
    QVariant defaultValue ( const QString& key ) const;

    /**
     * list of open files. All files should be added toLower().
     */
    QMap<QString, MainWindow*> files;

    void saveSettings() { mSettings.sync(); }

    void addRecentFile(QString fileName);

    QStringList recentFiles() { return mRecentFiles; }
    void setRecentFiles(QStringList files) { mRecentFiles = files; }
    
private:
    /**
     * list of recent files. All strings added case sensitively (they'll be used to open the files).
     */
    QStringList mRecentFiles;
    
    void initDemoVersion();
    static Settings* mInstance;
    
    Settings();

    void setupValueList();
    
    QSettings mSettings;

    bool mIsDemoVersion;

    QMap<QString, QVariant> mValueList;

};

#endif //SETTINGS_H
