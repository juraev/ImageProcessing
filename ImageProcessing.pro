#-------------------------------------------------
#
# Project created by QtCreator 2018-12-17T20:46:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageProcessing
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS


CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        graphicsview.cpp
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -lX11
HEADERS += \
        mainwindow.h \
    graphicsview.hpp \
    image.h \
    CImg.h

FORMS += \
        mainwindow.ui


