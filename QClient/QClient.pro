#-------------------------------------------------
#
# Project created by QtCreator 2018-07-26T21:53:37
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QClient
TEMPLATE = app


SOURCES += main.cpp\
    chat.cpp \
    cclient.cpp \
    regist.cpp \
    tcpsend.cpp

HEADERS  += \
    chat.h \
    cclient.h \
    regist.h \
    tcpsend.h

FORMS    += cclient.ui \
    regist.ui \
    chat.ui \
    tcpsend.ui
