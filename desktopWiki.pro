#-------------------------------------------------
#
# Project created by QtCreator 2012-04-29T00:53:30
#
#-------------------------------------------------

QT       += core gui xml

TARGET = desktopWiki
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    page.cpp \
    note.cpp \
    pagescene.cpp \
    settings.cpp \
    settingsui.cpp

HEADERS  += mainwindow.h \
    page.h \
    note.h \
    pagescene.h \
    settings.h \
    settingsui.h

FORMS    += mainwindow.ui \
            settings.ui
