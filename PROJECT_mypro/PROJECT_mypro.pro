#-------------------------------------------------
#
# Project created by QtCreator 2018-07-27T15:16:20
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PROJECT_mypro
TEMPLATE = app


SOURCES += main.cpp\
    mythread.cpp \
    myserver.cpp \
    serverstartwindow.cpp \
    server_sql.cpp \
    servermenuwindow.cpp

HEADERS  += \
    mythread.h \
    myserver.h \
    serverstartwindow.h \
    server_sql.h \
    servermenuwindow.h

FORMS    += mainwindow.ui \
    server_show.ui

RESOURCES += \
    tupian.qrc
