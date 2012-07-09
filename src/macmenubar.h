/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef MACMENUBAR_H
#define MACMENUBAR_H

#include <QObject>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include <QList>
#include <QFileDialog>
#include <QString>

#include <QDebug>

#include "appinfo.h"

#include <QApplication>
#include "mainwindow.h"

class MacMenuBar : public QObject
{
    Q_OBJECT
public:
    MacMenuBar()
    {
        //reconnectMenu();

        connect(qApp, SIGNAL(lastWindowClosed()), SLOT(reconnectMenu()));
    }

    QList<QMenu*> menus;

public slots:
    void newDocument()
    {
        MainWindow *mainWin = new MainWindow(false);
        mainWin->show();
    }

    void openDocument()
    {
        QString document = QFileDialog::getExistingDirectory(0, "Open Document");

        if(document.isEmpty())
            return;

        MainWindow *mainWin = new MainWindow(false);
        mainWin->loadFile(document);
        mainWin->show();
    }

    void toolsOptions()
    {
        AppInfo::inst()->toolsSettings();
    }

    void helpAbout()
    {

        AppInfo::inst()->helpAbout();
    }

    void reconnectMenu()
    {

        QMenuBar *menuBar = new QMenuBar(0);
        qDebug() << "reconnect menus";
        //TODO: Add Help, About, and etc that should work even when the window is closed.
        QMenu *fileMenu = new QMenu("&File");
        QAction *newAction = fileMenu->addAction("New...");
        QAction *openAction = fileMenu->addAction("Open");
        connect(newAction, SIGNAL(triggered()), SLOT(newDocument()));
        connect(openAction, SIGNAL(triggered()), SLOT(openDocument()));

        QMenu *toolMenu = new QMenu("&Tools");
        QAction *optionsAction = toolMenu->addAction("&Options");
        connect(optionsAction, SIGNAL(triggered()), SLOT(toolsOptions()));

        QMenu *helpMenu = new QMenu("&Help");
        QAction *about = helpMenu->addAction("About Note Pages");
        connect(about, SIGNAL(triggered()), SLOT(helpAbout()));

        menuBar->addMenu(fileMenu);
        menuBar->addMenu(toolMenu);
        menuBar->addMenu(helpMenu);
    }
};

#endif // MACMENUBAR_H
