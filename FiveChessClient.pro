#-------------------------------------------------
#
# Project created by QtCreator 2015-06-17T12:56:44
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FiveChessClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindlg.cpp \
    information.cpp \
    alterinfo.cpp \
    stringutil.cpp

HEADERS  += mainwindow.h \
    logindlg.h \
    information.h \
    alterinfo.h \
    stringutil.h

FORMS    += mainwindow.ui \
    logindlg.ui \
    information.ui \
    alterinfo.ui

RESOURCES += \
    fivechessclient.qrc

RC_FILE += icon.rc
