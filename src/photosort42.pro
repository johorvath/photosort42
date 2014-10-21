#-------------------------------------------------
#
# Project created by QtCreator 2014-10-17T16:35:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = photosort42
TEMPLATE = app

INCLUDEPATH += /usr/local/boost_1_56_0/include/
LIBS += -L/usr/local/boost_1_56_0/lib/ \
        -lboost_system \
        -lboost_filesystem \

LIBS += `pkg-config opencv --libs`


SOURCES += main.cpp\
        mainwindow.cpp \
    helper.cpp

HEADERS  += mainwindow.h \
    helper.hpp

FORMS    += mainwindow.ui
