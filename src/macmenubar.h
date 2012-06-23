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

#include "mainwindow.h"
class MacMenuBar : public QObject
{
    Q_OBJECT
public:
    MacMenuBar() {

        QMenu* fileMenu = new QMenu("&File");
        QAction *newAction = fileMenu->addAction("New...");
        QAction *openAction = fileMenu->addAction("Open");
        connect(newAction, SIGNAL(triggered()), SLOT(newDocument()));
        connect(openAction, SIGNAL(triggered()), SLOT(openDocument()));

        menus.append(fileMenu);
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
};

#endif // MACMENUBAR_H
