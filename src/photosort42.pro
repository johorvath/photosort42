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
    DetectAndRecognize/detection.cpp \
    DetectAndRecognize/recognition.cpp \
    DetectAndRecognize/recog_Eigen.cpp \
    DetectAndRecognize/recog_Fisher.cpp \

HEADERS  += gui/mainwindow.h \
    src/helper.hpp \
    libs/easylogging++.h \
    DetectAndRecognize/detection.hpp \
    DetectAndRecognize/recognition.hpp \
    DetectAndRecognize/recog_Eigen.hpp \
    DetectAndRecognize/recog_Fisher.hpp \

FORMS    += gui/mainwindow.ui
