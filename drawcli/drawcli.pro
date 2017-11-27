#-------------------------------------------------
#
# Project created by QtCreator 2015-05-29T17:15:10
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = drawcli
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    drawscene.cpp \
    drawtool.cpp \
    sizehandle.cpp \
    drawobj.cpp

HEADERS  += mainwindow.h \
    drawscene.h \
    drawtool.h \
    sizehandle.h \
    drawobj.h

FORMS    += mainwindow.ui

RESOURCES += resource.qrc
