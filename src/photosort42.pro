#-------------------------------------------------
#
# Project created by QtCreator 2014-10-17T16:35:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++11

TARGET = photosort42
TEMPLATE = app

INCLUDEPATH += /usr/local/boost-1.56.0/include/
LIBS += -L/usr/local/boost-1.56.0/lib/ \
        -lboost_system \
        -lboost_filesystem \


LIBS += `pkg-config opencv --libs`

SOURCES += src/main.cpp\
    gui/mainwindow.cpp \
    src/helper.cpp \
    libs/flandmark_detector.cpp \
    libs/liblbp.cpp \
    DetectAndRecognize/detection.cpp \
    DetectAndRecognize/face_alignement.cpp \
    DetectAndRecognize/recognition.cpp \
    libs/linreg.cpp

HEADERS  += gui/mainwindow.h \
    src/helper.hpp \
    libs/easylogging++.h \
    libs/*.h \
    libs/*.hpp \
    DetectAndRecognize/detection.hpp \
    DetectAndRecognize/face_alignement.hpp \
    DetectAndRecognize/recognition.hpp \
    libs/linreg.h

FORMS    += gui/mainwindow.ui
