#-------------------------------------------------
#
# Project created by QtCreator 2012-04-29T00:53:30
#
#-------------------------------------------------

QT       += core gui xml svg

TARGET = desktopWiki
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/page.cpp \
    src/note.cpp \
    src/pagescene.cpp \
    src/settings.cpp \
    src/settingsui.cpp

HEADERS  += src/mainwindow.h \
    src/page.h \
    src/note.h \
    src/pagescene.h \
    src/settings.h \
    src/settingsui.h

FORMS    += src/mainwindow.ui \
            src/settings.ui
