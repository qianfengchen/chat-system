#-------------------------------------------------
#
# Project created by QtCreator 2018-09-22T16:09:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chat-client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    user.cpp

HEADERS  += mainwindow.h \
    constant-macro.h \
    user.h

FORMS    += mainwindow.ui

LIBS += -lws2_32
