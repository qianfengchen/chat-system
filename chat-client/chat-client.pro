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
    user.cpp \
    cchatgui.cpp \
    io.cpp

HEADERS  += mainwindow.h \
    constant-macro.h \
    user.h \
    cchatgui.h \
    io.h

FORMS    += mainwindow.ui \
    cchatgui.ui

LIBS += -lws2_32
